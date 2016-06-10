#include <tsc/sys.h>
#include <tsc/log.h>
#include <tsc/parse.h>
#include <tsc/dump.h>

void printDir() {
  DIR *dp;
  struct dirent *ep;
  dp = opendir ("./");
  if (dp != NULL) {
    ep = readdir (dp);
    while (ep) {
      if (ep->d_type == DT_REG) {
        fprintf(stdout, "  File: %s\n", ep->d_name);
      } else if (ep->d_type == DT_DIR) {
        fprintf(stdout, "  Directory: %s\n", ep->d_name);
      }
      ep = readdir (dp);
    }
    (void) closedir (dp);
  } else perror ("Couldn't open the directory");
  puts("\n");
}

int main(int argc, char **argv) {
  // printDir();
  init_log();
  TSParseContext *context = parseFile("./examples/test.ts");
  char *str = NULL;
  char *js = stringFromParseContext(context);
  CONCAT(str, "var DEFAULT_EXPORT = Symbol();\n");
  CONCAT(str, js);
  LOG("%s\n", str);
  FILE *f = fopen("./app.js", "w");
  if (f) {
    fprintf(f, "%s\n", js);
    fclose(f);
  }
  if (str) free(str);
  freeTSParseContext(context);
  return 0;
}
