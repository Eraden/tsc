#include <tsc/parser.h>
#include <tsc/output.h>

//void printDir() {
//  DIR *dp;
//  struct dirent *ep;
//  dp = opendir ("./");
//  if (dp != NULL) {
//    ep = readdir (dp);
//    while (ep) {
//      if (ep->d_type == DT_REG) {
//        fprintf(stdout, "  File: %s\n", ep->d_name);
//      } else if (ep->d_type == DT_DIR) {
//        fprintf(stdout, "  Directory: %s\n", ep->d_name);
//      }
//      ep = readdir (dp);
//    }
//    (void) closedir (dp);
//  } else perror ("Couldn't open the directory");
//  puts("\n");
//}

int main(int argc, char **argv) {
  init_log();

  TSFile tsFile = TS_parse_file("./examples/test.ts");
  fprintf(stdout, "\n\n");
  for (int i = 0; i < 20; i++) fprintf(stdout, "#");
  fprintf(stdout, "\nFile: %s\n\n", tsFile.file);
  TS_print_stream(&tsFile, stdout);
  for (int i = 0; i < 20; i++) fprintf(stdout, "#");
  fprintf(stdout, "\n\n");
  return 0;
}
