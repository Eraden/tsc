#include <cts/os.h>
#include <cts/sys.h>

const char __attribute__((__used__))*TS_LIBRARY_PATH = LIBRARY_OUTPUT_PATH;
const char *TS_PREDEFINED_PATH = LIBRARY_OUTPUT_PATH"/predefined.ts";

static const char *TS_getHome(void);
static const char *TS_getLibraryPath(void);

static const char *
TS_getHome(void) {
#ifdef __APPLE__
  return getenv("HOME");
#else
  struct passwd *pw = getpwuid(getuid());
  return pw->pw_dir;
#endif
}

static const char *TS_getLibraryPath(void) {
  char *path = calloc(sizeof(char), strlen(TS_PREDEFINED_PATH) + 1);
  strcpy(path, TS_PREDEFINED_PATH);
  return path;
}

const char *
TS_getUserLibraryPath() {
  const char *home = TS_getHome();
  if (home == NULL) {
    return TS_getLibraryPath();
  }
  char *path;
  const char *libPath = "/.cts/lib/predefined.ts";
  const size_t len = strlen(home) + strlen(libPath) + 1;
  path = calloc(sizeof(char), len);
  strcat(path, home);
  strcat(path, libPath);
  path[len - 1] = 0;
  FILE *file = fopen(path, "r");
  if (file) {
    fclose(file);
    return path;
  }
  memset(path, 0, len);
  free(path);
  return TS_getLibraryPath();
}