#include <tsc/register.h>

static pthread_mutex_t
__attribute__(( visibility("hidden")))
    REGISTER_MUTEX_LOCK;

static TSRegisterEntry **
    __attribute__(( visibility("hidden")))
    TS_REGISTER = NULL;

static u_long
__attribute__(( visibility("hidden")))
    TS_REGISTER_SIZE = 0;

TSRegisterEntry *TS_register_file(TSFile *tsFile) {
  TSRegisterEntry *entry = calloc(sizeof(TSRegisterEntry), 1);
  entry->tsFile = tsFile;
  entry->classList = NULL;
  entry->listSize = 0;

  pthread_mutex_lock(&REGISTER_MUTEX_LOCK);

  TSRegisterEntry **newPointer = calloc(sizeof(TSRegisterEntry *), TS_REGISTER_SIZE + 1);
  if (TS_REGISTER) {
    memcpy(newPointer, TS_REGISTER, sizeof(TSRegisterEntry *) * TS_REGISTER_SIZE);
    free(TS_REGISTER);
  }
  TS_REGISTER = newPointer;

  TS_REGISTER[TS_REGISTER_SIZE] = entry;
  TS_REGISTER_SIZE += 1;

  pthread_mutex_unlock(&REGISTER_MUTEX_LOCK);
  return entry;
}

void TS_register_class(TSFile *file, TSParserToken *token) {
  TSRegisterEntry *entry = NULL;
  for (u_long entryIndex = 0; entryIndex < TS_REGISTER_SIZE; entryIndex++) {
    TSRegisterEntry *current = TS_REGISTER[entryIndex];
    if (current->tsFile == file) {
      entry = current;
      break;
    }
  }
  if (!entry) entry = TS_register_file(file);
  TSParserToken **newPointer = calloc(sizeof(TSParserToken *), entry->listSize + 1);
  if (entry->classList) {
    memcpy(newPointer, entry->classList, entry->listSize);
    free(entry->classList);
  }
  entry->classList = newPointer;
  entry->classList[entry->listSize] = token;
  entry->listSize += 1;
}

TSParserToken *
__attribute__(( __unused__ ))
TS_find_class(
    const wchar_t *file,
    const wchar_t *name
) {
  for (u_long entryIndex = 0; entryIndex < TS_REGISTER_SIZE; entryIndex++) {
    TSRegisterEntry *entry = TS_REGISTER[entryIndex];
    if (wcscmp(entry->tsFile->file, file) == 0) {
      for (u_long clsIndex = 0; clsIndex < entry->listSize; clsIndex++) {
        TSParserToken *cls = entry->classList[clsIndex];
        if (wcscmp(cls->classData->name, name) == 0) {
          return cls;
        }
      }
    }
  }
  return NULL;
}

void TS_destroy_register() {
  for (u_long i = 0; i < TS_REGISTER_SIZE; i++) {
    TSRegisterEntry *entry = TS_REGISTER[i];
    free(entry->classList);
    free(entry);
  }
  free(TS_REGISTER);
}
