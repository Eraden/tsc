#include <tsc/dump.h>

//#################################################################################
// ToString
//#################################################################################

static char *stringFromTSClassDecorators(const TSClass *tsClass);

static char *stringFromTSClassDecoratorArgs(const TSDecorator *dec);

static char *stringFromFunctions(TSParseContext *context) {
  if (context == NULL) return NULL;
  if (context->globalClassesSize == 0) return NULL;
  const unsigned long int size = context->globalFunctionsSize;
  char *buffer = NULL;
  TSFunction *fn = NULL;
  TSArgument *arg = NULL;
  for (unsigned long int i = 0; i < size; i++) {
    fn = context->globalFunctions[i];
    if (fn == NULL) continue;
    CONCAT(buffer, "  var ");
    CONCAT(buffer, fn->name);
    CONCAT(buffer, " = function (");
    for (unsigned long int j = 0; j < fn->argumentsSize; j++) {
      arg = fn->arguments[j];
      if (arg->isRest != 1) {
        if (j > 0) CONCAT(buffer, ", ");
        CONCAT(buffer, arg->name);
      }
    }
    CONCAT(buffer, ") {\n");
    arg = fn->arguments[fn->argumentsSize - 1];
    if (arg->isRest) {
      char length[256];
      sprintf(length, "%lu", fn->argumentsSize - 1);
      CONCAT(buffer, "    var ");
      CONCAT(buffer, arg->name);
      CONCAT(buffer, " = [].slice.call(arguments, ");
      CONCAT(buffer, length);
      CONCAT(buffer, ");\n");
    }
    for (unsigned long int j = 0; j < fn->argumentsSize; j++) {
      arg = fn->arguments[j];
      if (arg->value != NULL) {
        CONCAT(buffer, "    if ( void(0) === ");
        CONCAT(buffer, arg->name);
        CONCAT(buffer, " )\n      ");
        CONCAT(buffer, arg->name);
        CONCAT(buffer, " = ");
        CONCAT(buffer, arg->value);
        CONCAT(buffer, ";\n");
      }
    }
    CONCAT(buffer, fn->body);
    CONCAT(buffer, "};\n");
  }
  return buffer;
}

static char *stringFromClassFields(TSClass *tsClass) {
  if (tsClass->fieldsSize == 0) return NULL;
  TSField *field = NULL;
  char *buffer = NULL;
  CONCAT(buffer, "  (function (T, proto) {\n");
//  CONCAT(
//      buffer,
//      "    proto[META_TABLE] = {\n"
//          "      'get': function () {\n"
//          "        this[PRIVATE] || this[PRIVATE] = {};\n"
//          "        return { 'private': this[PRIVATE] };\n"
//          "      }\n"
//          "    };\n"
//  );
  for (unsigned long int i = 0; i < tsClass->fieldsSize; i++) {
    field = tsClass->fields[i];
    CONCAT(buffer, "    proto['");
    CONCAT(buffer, field->name);
    CONCAT(
        buffer,
        "'] = {\n"
    );
    if (field->modifier == TSAccessModifier_Private) {
      // TODO: Implement private access as meta table so extended class can't use or modify parent data
      CONCAT(
          buffer,
          "      'enumerable': false,\n"
              "      'writable': true,\n"
              "      'configurable': false,\n"
              "      'value': "
      );
    } else {
      CONCAT(
          buffer,
          "      'enumerable': false,\n"
              "      'writable': true,\n"
              "      'configurable': false,\n"
              "      'value': "
      );
    }
    CONCAT(buffer, field->value == NULL ? "null" : field->value);
    CONCAT(
        buffer,
        "\n"
            "    };\n"
    );
  }
  CONCAT(buffer, "    Object.defineProperties(T.prototype, proto);\n");
  CONCAT(buffer, "  }(");
  CONCAT(buffer, tsClass->name);
  CONCAT(buffer, ", {}));\n");
  return buffer;
}

static char *stringFromTSClassDecoratorArgs(const TSDecorator *dec) {
  TSArgument *arg;
  char *buffer = NULL;
  for (unsigned long int i = 0; i < dec->argumentsSize; i++) {
    arg = dec->arguments[i];
    if (i > 0) CONCAT(buffer, ",");
    CONCAT(buffer, arg->value);
  }
  return buffer;
}

static char *stringFromTSClassDecorators(const TSClass *tsClass) {
  TSDecorator *dec;
  char *buffer = NULL;
  for (unsigned long int i = 0; i < tsClass->decoratorsSize; i++) {
    dec = tsClass->decorators[i];
    CONCAT(buffer, "  ");
    CONCAT(buffer, tsClass->name);
    CONCAT(buffer, "=");
    CONCAT(buffer, dec->name);
    if (dec->argumentsSize > 0) {
      CONCAT(buffer, "(");
      CONCAT(buffer, stringFromTSClassDecoratorArgs(dec));
      CONCAT(buffer, ")");
    }
    // args
    CONCAT(buffer, "(");
    CONCAT(buffer, tsClass->name);
    CONCAT(buffer, ");\n");
  }
  return buffer;
}

static char *stringFromClasses(TSParseContext *context) {
  if (context == NULL) return NULL;
  TSClass **coll = context->globalClasses;
  if (coll== NULL) return NULL;
  const unsigned long int size = context->globalClassesSize;
  char *buffer = NULL;
  TSClass *tsClass = NULL;
  for (unsigned long int i = 0; i < size; i++) {
    tsClass = coll[i];
    CONCAT(buffer, "  var ");
    CONCAT(buffer, tsClass->name);
    CONCAT(buffer, " = function () {\n");
    if (tsClass->parent != NULL) {
      CONCAT(buffer, "    ");
      CONCAT(buffer, tsClass->parent);
      CONCAT(buffer, ".apply(this, arguments);\n");
    }
    CONCAT(buffer, "  };\n");
    if (tsClass->decoratorsSize > 0) {
      CONCAT(buffer, stringFromTSClassDecorators(tsClass));
    }
    if (tsClass->fieldsSize > 0) {
      CONCAT(buffer, stringFromClassFields(tsClass));
    }
  }
  return buffer;
}

static char *stringFromExports(TSParseContext *context) {
  if (context == NULL) return NULL;
  if (context->exports == NULL) return NULL;
  const long unsigned int size = context->exportsSize;
  char *buffer = NULL;
  TSExport *export;
  TSClass *class;
  for (unsigned long int i = 0; i < size; i++) {
    export = context->exports[i];
    switch (export->type) {
      case TSExportType_Class:
      {
        class = (TSClass*) export->meta;
        if (export->asDefault == 1) {
          CONCAT(buffer, "  exports[DEFAULT_EXPORT] = ");
        } else {
          CONCAT(buffer, "  exports[\"");
          CONCAT(buffer, class->name);
          CONCAT(buffer, "\"] = ");
        }
        CONCAT(buffer, class->name);
        CONCAT(buffer, ";\n");
        break;
      }
      default:
        break;
    }
  }
  return buffer;
}

char *stringFromParseContext(TSParseContext *context) {
  char *buffer = NULL;
  CONCAT(buffer, "(function () {/*FILE EXPORT*/\n");
  CONCAT(buffer, "var exports = {};\n(function () {\n");
  char *join = NULL;

  // functions
  join = stringFromFunctions(context);
  if (join) {
    CONCAT(buffer, join);
    free(join);
  }

  // classes
  join = stringFromClasses(context);
  if (join) {
    CONCAT(buffer, join);
    free(join);
  }

  // exports
  join = stringFromExports(context);
  if (join) {
    CONCAT(buffer, join);
    free(join);
  }

  CONCAT(buffer, "}());\n");
  CONCAT(buffer, "return exports;\n}());");
  return buffer;
}
