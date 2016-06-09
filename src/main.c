#include <tsc/sys.h>
#include <tsc/log.h>
#include <tsc/parse.h>
#include <tsc/dump.h>

int main(int argc, char **argv) {
  init_log();
  TSParseContext *context = parseFile("./examples/test.ts");
  char *str = NULL;
  CONCAT(str, "var DEFAULT_EXPORT = Symbol();\n");
  CONCAT(str, stringFromParseContext(context));
  LOG("%s\n", str);
  if (str) free(str);
  freeTSParseContext(context);
  return 0;
}
