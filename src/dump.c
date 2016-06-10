#include <tsc/dump.h>

//#################################################################################
// ToString
//#################################################################################

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

static char *stringFromClasses(TSParseContext *context) {
  // FLOG("  %s\n", "creating classes...");
  if (context == NULL) return NULL;
  TSClass **coll = context->globalClasses;
  if (coll== NULL) return NULL;
  const unsigned long int size = context->globalClassesSize;
  // FLOG("    size: %lu\n", size);
  char *buffer = NULL;
  TSClass *class = NULL;
  for (unsigned long int i = 0; i < size; i++) {
    class = coll[i];
    CONCAT(buffer, "  var ");
    CONCAT(buffer, class->name);
    CONCAT(buffer, " = function(){\n");
    if (class->parent != NULL) {
      CONCAT(buffer, "    ");
      CONCAT(buffer, class->parent);
      CONCAT(buffer, ".apply(this, arguments);\n");
      CONCAT(buffer, "  };\n");
      if (class->decoratorsSize > 0) {
        TSDecorator *dec;
        for (unsigned long int j = 0; j < class->decoratorsSize; j++) {
          dec = class->decorators[j];
          CONCAT(buffer, "  ");
          CONCAT(buffer, class->name);
          CONCAT(buffer, "=");
          CONCAT(buffer, dec->name);
          if (dec->argumentsSize > 0) {
            CONCAT(buffer, "(");
            TSArgument *arg;
            for (unsigned long int k = 0; k < dec->argumentsSize; k++) {
              arg = dec->arguments[k];
              if (k > 0) CONCAT(buffer, ",");
              CONCAT(buffer, arg->value);
            }
            CONCAT(buffer, ")");
          }
          // args
          CONCAT(buffer, "(");
          CONCAT(buffer, class->name);
          CONCAT(buffer, ");\n");
        }
      }
    }
  }
  return buffer;
}

static char *stringFromExports(TSParseContext *context) {
//  FLOG("  %s\n", "creating exports...");
  if (context == NULL) return NULL;
  if (context->exports == NULL) return NULL;
  const long unsigned int size = context->exportsSize;
  // FLOG("    ! coll address: %p\n", context->exports);
  // FLOG("    size: %lu\n", size);
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
  // FLOG("%s\n", "Creating JavaScript log from from collected data...");
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
