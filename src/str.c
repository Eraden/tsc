#include <tsc/str.h>

//#################################################################################
// string
//#################################################################################

char *appendChar(const char *content, char c) {
  size_t size = content == NULL ? sizeof(char) : strlen(content) + 1;
  char *new = (char *) calloc(sizeof(char), size);
  if (new == NULL) {
    ALLOC_FAILED;
    exit(1);
  }
  if (content != NULL) {
    if ((new = strcpy(new, content)) == NULL) {
      ALLOC_FAILED;
      exit(1);
    }
  }
  new[size - 1] = c;
  new[size] = 0;
  return new;
}

char *cloneString(const char *content) {
  if (content == NULL) return NULL;
  const size_t size = strlen(content) ;
  char *new = (char *) calloc(sizeof(char), size);
  strcpy(new, content);
  return new;
}

char *concat(char *destination, char *source) {
  if (destination == NULL && source == NULL) return NULL;
  if (destination == NULL) return cloneString(source);
  if (source == NULL) return cloneString(destination);
  const size_t len1 = strlen(destination);
  const size_t len2 = strlen(source);
  char *result = (char*) realloc(destination, len1 + len2 + 1);
  if (result == NULL) exit(1);
  if (result != destination) {
    // free(destination);
  }
  strncpy(result + len1, source, len2 + 1);
  return result;
}

