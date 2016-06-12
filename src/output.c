#include <tsc/output.h>
#include <tsc/options.h>

void out(char *str) {
  if (OUTPUT_FILE == NULL) {
    fprintf(stdout, "%s", str);
  } else {
    FILE *file = fopen(OUTPUT_FILE, "a");
    if (file == NULL) {
      io_panic(concat("Can't open file ", OUTPUT_FILE));
    }
    fprintf(file, "%s", str);
    fclose(file);
  }
}