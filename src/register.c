#include <cts/register.h>

static pthread_mutex_t
__attribute__(( visibility("hidden")))
    REGISTER_CLASS_MUTEX_LOCK, REGISTER_FILE_MUTEX_LOCK, TS_PREDEFINED_MUTEX_LOCK;

static RegisterCollection
__attribute__(( visibility("hidden")))
TS_REGISTER = NULL;

static u_long
__attribute__(( visibility("hidden")))
    TS_REGISTER_SIZE = 0;

TSFile *TS_PREDEFINED_FILE = NULL;

RegisterCollection
TS_PREDEFINED_REGISTER = NULL;

static u_long
__attribute__(( visibility("hidden")))
    TS_PREDEFINED_REGISTER_SIZE = 0;

TSRegisterEntry *
TS_register_file(
    TSFile *tsFile
) {
  pthread_mutex_lock(&REGISTER_FILE_MUTEX_LOCK);

  TSRegisterEntry *entry = calloc(sizeof(TSRegisterEntry), 1);
  entry->tsFile = tsFile;
  entry->classList = NULL;
  entry->listSize = 0;


  RegisterCollection newPointer = calloc(sizeof(TSRegisterEntry *), TS_REGISTER_SIZE + 1);
  if (TS_REGISTER) {
    memcpy(newPointer, TS_REGISTER, sizeof(TSRegisterEntry *) * TS_REGISTER_SIZE);
    free(TS_REGISTER);
  }
  TS_REGISTER = newPointer;

  TS_REGISTER[TS_REGISTER_SIZE] = entry;
  TS_REGISTER_SIZE += 1;

  pthread_mutex_unlock(&REGISTER_FILE_MUTEX_LOCK);
  return entry;
}

void
TS_register_remove_file(
    TSFile *file
) {
  if (file == NULL) return;

  pthread_mutex_lock(&REGISTER_FILE_MUTEX_LOCK);

  RegisterCollection newPointer = calloc(sizeof(TSRegisterEntry *), TS_REGISTER_SIZE - 1);
  u_long index = 0;

  for (u_long entryIndex = 0; entryIndex < TS_REGISTER_SIZE; entryIndex++) {
    TSRegisterEntry *entry = TS_REGISTER[entryIndex];
    if (entry->tsFile != file) {
      newPointer[index] = entry;
      index += 1;
    } else {
      free(entry->classList);
      free(entry);
    }
  }
  free(TS_REGISTER);
  TS_REGISTER = newPointer;
  TS_REGISTER_SIZE -= 1;
  pthread_mutex_unlock(&REGISTER_FILE_MUTEX_LOCK);
}

void
TS_register_class(
    TSFile *file,
    TSParserToken *token
) {
  pthread_mutex_lock(&REGISTER_CLASS_MUTEX_LOCK);

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
    memcpy(newPointer, entry->classList, sizeof(TSParserToken *) * entry->listSize);
    free(entry->classList);
  }

  entry->classList = newPointer;
  entry->classList[entry->listSize] = token;
  entry->listSize += 1;

  pthread_mutex_unlock(&REGISTER_CLASS_MUTEX_LOCK);
}

TSParserToken *
__attribute__(( __unused__ ))
TS_find_class(
    const wchar_t *file,
    const wchar_t *name
) {
  TSParserToken *result = NULL;

  pthread_mutex_lock(&REGISTER_CLASS_MUTEX_LOCK);
  pthread_mutex_lock(&REGISTER_FILE_MUTEX_LOCK);

  for (u_long entryIndex = 0; entryIndex < TS_REGISTER_SIZE; entryIndex++) {
    TSRegisterEntry *entry = TS_REGISTER[entryIndex];
    if (file == NULL || wcscmp(entry->tsFile->file, file) == 0) {
      for (u_long clsIndex = 0; clsIndex < entry->listSize; clsIndex++) {
        TSParserToken *cls = entry->classList[clsIndex];
        if (wcscmp(cls->classData->name, name) == 0) {
          result = cls;
          goto exit_find_class;
        }
      }
    }
  }

  for (u_long entryIndex = 0; entryIndex < TS_PREDEFINED_REGISTER_SIZE; entryIndex++) {
    TSRegisterEntry *entry = TS_PREDEFINED_REGISTER[entryIndex];
    for (u_long clsIndex = 0; clsIndex < entry->listSize; clsIndex++) {
      TSParserToken *cls = entry->classList[clsIndex];
      if (wcscmp(cls->classData->name, name) == 0) {
        result = cls;
        goto exit_find_class;
      }
    }
  }

  exit_find_class:
  pthread_mutex_unlock(&REGISTER_CLASS_MUTEX_LOCK);
  pthread_mutex_unlock(&REGISTER_FILE_MUTEX_LOCK);

  return result;
}

void
TS_destroy_register(
    void
) {
  pthread_mutex_lock(&REGISTER_CLASS_MUTEX_LOCK);
  pthread_mutex_lock(&REGISTER_FILE_MUTEX_LOCK);

  for (u_long i = 0; i < TS_REGISTER_SIZE; i++) {
    TSRegisterEntry *entry = TS_REGISTER[i];
    free(entry->classList);
    free(entry);
  }
  free(TS_REGISTER);

  pthread_mutex_unlock(&REGISTER_CLASS_MUTEX_LOCK);
  pthread_mutex_unlock(&REGISTER_FILE_MUTEX_LOCK);
}

void
TS_setup_predefined(
    void
) {
  pthread_mutex_lock(&TS_PREDEFINED_MUTEX_LOCK);
  if (TS_PREDEFINED_REGISTER == NULL) {
    TS_PREDEFINED_REGISTER = calloc(sizeof(TSRegisterEntry *), 1);

    swap(u_long, TS_PREDEFINED_REGISTER_SIZE, TS_REGISTER_SIZE);
    swap(RegisterCollection, TS_PREDEFINED_REGISTER, TS_REGISTER);

    TS_PREDEFINED_FILE = TS_parse_file(TS_PREDEFINED_PATH);
    if (TS_PREDEFINED_FILE->sanity != TS_FILE_VALID) {
      TS_free_tsFile(TS_PREDEFINED_FILE);
      fprintf(stderr, "Predefined classes file is malformed! Exiting...\n");
      exit(5);
    }

    swap(u_long, TS_PREDEFINED_REGISTER_SIZE, TS_REGISTER_SIZE);
    swap(RegisterCollection, TS_PREDEFINED_REGISTER, TS_REGISTER);
  }
  pthread_mutex_unlock(&TS_PREDEFINED_MUTEX_LOCK);
}

void
TS_remove_predefined() {
  pthread_mutex_lock(&TS_PREDEFINED_MUTEX_LOCK);
  if (TS_PREDEFINED_REGISTER != NULL) {
    swap(u_long, TS_PREDEFINED_REGISTER_SIZE, TS_REGISTER_SIZE);
    swap(RegisterCollection, TS_PREDEFINED_REGISTER, TS_REGISTER);

    TS_free_tsFile(TS_PREDEFINED_FILE);
    TS_destroy_register();

    swap(u_long, TS_PREDEFINED_REGISTER_SIZE, TS_REGISTER_SIZE);
    swap(RegisterCollection, TS_PREDEFINED_REGISTER, TS_REGISTER);

  }
  pthread_mutex_unlock(&TS_PREDEFINED_MUTEX_LOCK);
}
