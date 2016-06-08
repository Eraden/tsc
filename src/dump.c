#include <tsc/dump.h>

//#################################################################################
// ToString
//#################################################################################

static char* classesToString(TSParseContext *context) {
  FLOG("  %s\n", "creating classes...");
  if (context == NULL) return NULL;
  TSClass **coll = context->globalClasses;
  if (coll== NULL) return NULL;
  const unsigned long int size = context->globalClassesSize;
  FLOG("    size: %lu\n", size);
  char *buffer = (char*) malloc(1024 * 8);
  TSClass *class = NULL;
  for (unsigned long int i = 0; i < size; i++) {
    class = coll[i];
    strcat(buffer, "var ");
    strcat(buffer, class->name);
    strcat(buffer, " = function(){\n");
    if (class->parent != NULL) {
      strcat(buffer, "  ");
      strcat(buffer, class->parent);
      strcat(buffer, ".apply(this, arguments);\n");
      strcat(buffer, "};\n");
      if (class->decoratorsSize > 0) {
        TSDecorator *dec;
        for (unsigned long int j = 0; j < class->decoratorsSize; j++) {
          dec = class->decorators[j];
          strcat(buffer, class->name);
          strcat(buffer, " = ");
          strcat(buffer, dec->name);
          if (dec->argumentsSize > 0) {
            strcat(buffer, "(");
            TSArgument *arg;
            for (unsigned long int k = 0; k < dec->argumentsSize; k++) {
              arg = dec->arguments[k];
              if (k > 0) strcat(buffer, ",");
              strcat(buffer, arg->value);
            }
            strcat(buffer, ")");
          }
          // args
          strcat(buffer, "(");
          strcat(buffer, class->name);
          strcat(buffer, ");\n");
        }
      }
    }
  }
  return buffer;
}

static char *exportsToString(TSParseContext *context) {
  FLOG("  %s\n", "creating exports...");
  if (context == NULL) return NULL;
  if (context->exports == NULL) return NULL;
  const long unsigned int size = context->exportsSize;
  FLOG("    ! coll address: %p\n", context->exports);
  FLOG("    size: %lu\n", size);
  char *buffer = (char*) malloc(1024);
  memset(buffer, 0, 1024);
  TSExport *export;
  TSClass *class;
  for (unsigned long int i = 0; i < size; i++) {
    export = context->exports[i];
    switch (export->type) {
      case TSExportType_Class:
        {
          class = (TSClass*) export->meta;
          if (export->asDefault == 1) strcat(buffer, "exports[DEFAULT_EXPORT] = ");
          else {
            strcat(buffer, "exports[\"");
            strcat(buffer, class->name);
            strcat(buffer, "\"] = ");
          }
          strcat(buffer, class->name);
          strcat(buffer, ";\n");
          break;
        }
      default:
        break;
    }
  }
  return buffer;
}

char *stringFromParseContext(TSParseContext *context) {
  FLOG("%s\n", "Creating JavaScript log from from collected data...");
  char *buffer = (char*) malloc(1024);
  memset(buffer, 0, 1024);
  strcat(buffer, "(function () {/*FILE EXPORT*/\n");
  strcat(buffer, "var exports = {};\n(function () {\n");
  char *join;
  // classes
  join = classesToString(context);
  if (join) {
    strcat(buffer, join);
    free((void*) join);
  }

  // exports
  join = exportsToString(context);
  if (join) {
    strcat(buffer, join);
    free((void*) join);
  }
  strcat(buffer, "}());\n");
  strcat(buffer, "return exports;\n}());");
  return buffer;
}
