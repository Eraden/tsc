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

char *concat(const char *s1, const char *s2) {
  if (s1 == NULL && s2 == NULL) return NULL;
  if (s1 == NULL) return cloneString(s2);
  if (s2 == NULL) return cloneString(s1);
  size_t len1 = strlen(s1);
  size_t len2 = strlen(s2);
  char *result = (char*) calloc(sizeof(char), len1 + len2 + 1);
  if (result == NULL) exit(1);
  memcpy(result, s1, len1);
  memcpy(result + len1, s2, len2 + 1);
  return result;
}

