#include <tsc/tsc.h>

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

  // TODO: Choose output via arguments
  output = stdout;

  init_log();
  TSParseContext *context = parseFile("./examples/test.ts");
  out("var DEFAULT_EXPORT = Symbol();\n");
  dumpFromParseContext(context);
  freeTSParseContext(context);
  return 0;
}
