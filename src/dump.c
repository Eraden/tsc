#include <tsc/dump.h>

static void dumpFromTSClassDecorators(const TSClass *tsClass);

static void dumpFromTSClassDecoratorArgs(const TSDecorator *dec);

static void dumpFromFunctions(TSParseContext *context) {
  if (context == NULL) return;
  if (context->globalClassesSize == 0) return;
  const unsigned long int size = context->globalFunctionsSize;
  TSFunction *fn = NULL;
  TSArgument *arg = NULL;
  for (unsigned long int i = 0; i < size; i++) {
    fn = context->globalFunctions[i];
    if (fn == NULL) continue;
    out("  var ");
    out(fn->name);
    out(" = function (");
    for (unsigned long int j = 0; j < fn->argumentsSize; j++) {
      arg = fn->arguments[j];
      if (arg->isRest != 1) {
        if (j > 0) out(", ");
        out(arg->name);
      }
    }
    out(") {\n");
    arg = fn->arguments[fn->argumentsSize - 1];
    if (arg->isRest) {
      char length[256];
      sprintf(length, "%lu", fn->argumentsSize - 1);
      out("    var ");
      out(arg->name);
      out(" = [].slice.call(arguments, ");
      out(length);
      out(");\n");
    }
    for (unsigned long int j = 0; j < fn->argumentsSize; j++) {
      arg = fn->arguments[j];
      if (arg->value != NULL) {
        out("    if ( void(0) === ");
        out(arg->name);
        out(" )\n      ");
        out(arg->name);
        out(" = ");
        out(arg->value);
        out(";\n");
      }
    }
    out(fn->body);
    out("};\n");
  }
}

static void dumpFromClassMethods(TSClass *tsClass) {
  if (tsClass->methodsSize == 0) return;
  TSMethod *method = NULL;
  for (unsigned long int i = 0; i < tsClass->methodsSize; i++) {
    method = tsClass->methods[i];
    if (method->type == TSMethod_Type_Constructor) continue;
    out("    // Method\n");
    out("    proto['");
    out(method->name);
    out("'] = {\n");
    // TODO: Implement private access as meta table so extended class can't use or modify parent data
    out(
        "      'enumerable': false,\n"
            "      'writable': false,\n"
            "      'configurable': false,\n"
            "      'value': "
    );
    out(
        "function () {\n"
            "        "
    );
    if (method->body != NULL) out(method->body);
    out("\n      }");
    out(
        "\n"
            "    };\n"
    );
  }
}

static void dumpFromClassFields(TSClass *tsClass) {
  if (tsClass->fieldsSize == 0) return;
  TSField *field = NULL;
  for (unsigned long int i = 0; i < tsClass->fieldsSize; i++) {
    field = tsClass->fields[i];
    out("    // Field\n");
    out("    proto['");
    out(field->name);
    out("'] = {\n");
    // TODO: Implement private access as meta table so extended class can't use or modify parent data
    out(
        "      'enumerable': false,\n"
            "      'writable': true,\n"
            "      'configurable': false,\n"
            "      'value': "
    );
    out(field->value == NULL ? "null" : field->value);
    out(
        "\n"
            "    };\n"
    );
  }
}

static void dumpFromTSClassDecoratorArgs(const TSDecorator *dec) {
  TSArgument *arg;
  for (unsigned long int i = 0; i < dec->argumentsSize; i++) {
    arg = dec->arguments[i];
    if (i > 0) out(",");
    out(arg->value);
  }
}

static void dumpFromTSClassDecorators(const TSClass *tsClass) {
  TSDecorator *dec;
  for (unsigned long int i = 0; i < tsClass->decoratorsSize; i++) {
    dec = tsClass->decorators[i];
    out("  ");
    out(tsClass->name);
    out("=");
    out(dec->name);
    if (dec->argumentsSize > 0) {
      out("(");
      dumpFromTSClassDecoratorArgs(dec);
      out(")");
    }
    // args
    out("(");
    out(tsClass->name);
    out(");\n");
  }
}

static void dumpFromClasses(TSParseContext *context) {
  if (context == NULL) return;
  TSClass **coll = context->globalClasses;
  if (coll== NULL) return;
  const unsigned long int size = context->globalClassesSize;
  TSClass *tsClass = NULL;
  for (unsigned long int i = 0; i < size; i++) {
    tsClass = coll[i];
    out("  var ");
    out(tsClass->name);
    out(" = ");
    TSMethod *tsMethod = NULL;
    TSArgument *tsArgument = NULL;
    unsigned short int foundConstructor = 0;
    for (unsigned long int mthI = 0; mthI < tsClass->methodsSize; mthI++) {
      tsMethod = tsClass->methods[mthI];
      if (tsMethod->type == TSMethod_Type_Constructor) {
        out("function (");
        for (unsigned long int argI = 0; argI < tsMethod->argumentsSize; argI++) {
          tsArgument = tsMethod->arguments[argI];
          if (argI > 0) out(",");
          out(tsArgument->name);
        }
        out(") {\n    ");
        out(tsMethod->body);
        foundConstructor = 1;
        break;
      }
    }
    if (!foundConstructor) {
      out("function () {\n");
    }
    out("\n  };\n");
    if (tsClass->decoratorsSize > 0) {
      dumpFromTSClassDecorators(tsClass);
    }
    if (tsClass->methodsSize + tsClass->fieldsSize > 0) {
      out("  (function (T, proto) {\n");
      if (tsClass->fieldsSize > 0) {
        dumpFromClassFields(tsClass);
      }
      if (tsClass->methodsSize > 0) {
        dumpFromClassMethods(tsClass);
      }
      out("    Object.defineProperties(T.prototype, proto);\n");
      out("  }(");
      out(tsClass->name);
      out(", {}));\n");
    }
  }
}

static void dumpFromExports(TSParseContext *context) {
  if (context == NULL) return;
  if (context->exports == NULL) return;
  const long unsigned int size = context->exportsSize;
  TSExport *export;
  TSClass *class;
  for (unsigned long int i = 0; i < size; i++) {
    export = context->exports[i];
    switch (export->type) {
      case TSExportType_Class:
      {
        class = (TSClass*) export->meta;
        if (export->asDefault == 1) {
          out("  exports[DEFAULT_EXPORT] = ");
        } else {
          out("  exports[\"");
          out(class->name);
          out("\"] = ");
        }
        out(class->name);
        out(";\n");
        break;
      }
      default:
        break;
    }
  }
}

void dumpFromParseContext(TSParseContext *context) {
  out("(function () {/*FILE EXPORT*/\n");
  out("  var exports = {};\n(function () {\n");

  // functions
  dumpFromFunctions(context);

  // classes
  dumpFromClasses(context);

  // exports
  dumpFromExports(context);

  out("  }());\n");
  out("  return exports;\n}());\n\n");
}
