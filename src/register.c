#include <cts/register.h>

TSParserToken *TS_ANY_TYPE = NULL;
TSParserToken *TS_UNDEFINED_TYPE = NULL;
TSParserToken *TS_NULL_TYPE = NULL;
static unsigned char registers_swapped = FALSE;

static pthread_mutex_t
__attribute__(( visibility("hidden")))
    CLASS_REGISTER_MUTEX_LOCK,
    FILE_REGISTER_MUTEX_LOCK,
    TS_PREDEFINED_MUTEX_LOCK;

RegisterCollection
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
  pthread_mutex_lock(&FILE_REGISTER_MUTEX_LOCK);

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

  pthread_mutex_unlock(&FILE_REGISTER_MUTEX_LOCK);
  return entry;
}

void
TS_register_remove_file(
    TSFile *file
) {
  if (file == NULL) return;

  pthread_mutex_lock(&FILE_REGISTER_MUTEX_LOCK);

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

  pthread_mutex_unlock(&FILE_REGISTER_MUTEX_LOCK);
}

TSFile *
TS_find_file(
    const char *path,
    FILE *stream
) {
  pthread_mutex_lock(&FILE_REGISTER_MUTEX_LOCK);
  char full_path[4096];
  memset(full_path, 0, sizeof(char) * 4096);
  if (strcmp(path, TS_CODE_EVAL) != 0) {
    realpath(path, full_path);
  } else {
    strcpy(full_path, path);
  }
  wchar_t buffer[4096];
  memset(buffer, 0, sizeof(wchar_t) * 4096);
  mbstowcs(buffer, full_path, 4096);

  TSFile *tsFile = NULL;
  RegisterCollection files = TS_REGISTER;
  for (u_long index = 0; index < TS_REGISTER_SIZE; index++) {
    TSRegisterEntry *entry = files[0];
    if (wcscmp(entry->tsFile->file, buffer) == 0) {
      tsFile = entry->tsFile;
      break;
    }
    files += 1;
  }
  pthread_mutex_unlock(&FILE_REGISTER_MUTEX_LOCK);

  if (tsFile == NULL) {
    if (stream == NULL) stream = fopen(path, "r");

    tsFile = calloc(sizeof(TSFile), 1);
    tsFile->tokens = NULL;
    tsFile->tokensSize = 0;
    tsFile->stream = stream;
    tsFile->file = TS_clone_string(buffer);

    if (stream) {
      tsFile->sanity = TS_FILE_VALID;
      tsFile->errorReason = NULL;
    } else {
      tsFile->sanity = TS_FILE_NOT_FOUND;
      wchar_t *msg = (wchar_t *) L"File not found!";
      tsFile->errorReason = calloc(sizeof(wchar_t), wcslen(msg) + 1);
      wcscpy(tsFile->errorReason, msg);
      fprintf(stderr, "OS error: %s\n", strerror(errno));
    }
    TS_register_file(tsFile);
  }

  return tsFile;
}

void
TS_register_type(
    TSFile *file,
    TSParserToken *token
) {
  pthread_mutex_lock(&CLASS_REGISTER_MUTEX_LOCK);

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

  pthread_mutex_unlock(&CLASS_REGISTER_MUTEX_LOCK);
}

TSParserToken *
TS_find_type(
    const wchar_t *file,
    const wchar_t *name
) {
  if (name == NULL) {
    return NULL;
  }
  TSParserToken *result = NULL;

  pthread_mutex_lock(&CLASS_REGISTER_MUTEX_LOCK);
  pthread_mutex_lock(&FILE_REGISTER_MUTEX_LOCK);

  for (u_long entryIndex = 0; entryIndex < TS_REGISTER_SIZE; entryIndex++) {
    TSRegisterEntry *entry = TS_REGISTER[entryIndex];
    if (file != NULL && wcscmp(entry->tsFile->file, file) == 0) {
      for (u_long clsIndex = 0; clsIndex < entry->listSize; clsIndex++) {
        TSParserToken *cls = entry->classList[clsIndex];
        if (wcscmp(cls->name, name) == 0) {
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
      if (wcscmp(cls->name, name) == 0) {
        result = cls;
        goto exit_find_class;
      }
    }
  }

  exit_find_class:
  pthread_mutex_unlock(&CLASS_REGISTER_MUTEX_LOCK);
  pthread_mutex_unlock(&FILE_REGISTER_MUTEX_LOCK);

  return result;
}

void
TS_destroy_register(
    void
) {
  pthread_mutex_lock(&FILE_REGISTER_MUTEX_LOCK);

  for (u_long i = 0; i < TS_REGISTER_SIZE; i++) {
    TSRegisterEntry *entry = TS_REGISTER[i];
    free(entry->classList);
    free(entry);
  }
  free(TS_REGISTER);
  TS_REGISTER = NULL;

  pthread_mutex_unlock(&FILE_REGISTER_MUTEX_LOCK);
}

static void
__attribute__((visibility("hidden")))
__TS_setup_predefined(void) {
  if (TS_PREDEFINED_REGISTER == NULL) {
    TS_PREDEFINED_REGISTER = calloc(sizeof(TSRegisterEntry *), 1);

    swap(u_long, TS_PREDEFINED_REGISTER_SIZE, TS_REGISTER_SIZE);
    swap(RegisterCollection, TS_PREDEFINED_REGISTER, TS_REGISTER);
    registers_swapped = TRUE;

    TSFile *tsFile = TS_PREDEFINED_FILE = TS_find_file(TS_PREDEFINED_PATH, NULL);

    TSParseData data;
    data.line = 0;
    data.character = 0;
    data.token = 0;
    data.stream = 0;
    data.parentTSToken = 0;
    TSParserToken *type, *extends;

    // any
    type = TS_ANY_TYPE = TS_build_parser_token(TS_CLASS, &data);
    type->name = TS_clone_string((const wchar_t *) L"any");
    TS_register_type(tsFile, type);
    data.line = 0;
    data.character = 0;
    data.parentTSToken = 0;
    // undefined
    type = TS_UNDEFINED_TYPE = TS_build_parser_token(TS_INTERFACE, &data);
    type->name = TS_clone_string((const wchar_t *) L"undefined");
    extends = TS_build_parser_token(TS_EXTENDS, &data);
    TS_push_child(extends, TS_ANY_TYPE);
    TS_push_child(type, extends);
    TS_register_type(tsFile, type);
    data.line = 0;
    data.character = 0;
    data.parentTSToken = 0;
    // null
    type = TS_NULL_TYPE = TS_build_parser_token(TS_INTERFACE, &data);
    type->name = TS_clone_string((const wchar_t *) L"null");
    extends = TS_build_parser_token(TS_EXTENDS, &data);
    TS_push_child(extends, TS_ANY_TYPE);
    TS_push_child(type, extends);
    TS_register_type(tsFile, type);
    data.line = 0;
    data.character = 0;
    data.parentTSToken = 0;
    // void
    type = TS_build_parser_token(TS_FUNCTION, &data);
    type->name = TS_clone_string((const wchar_t *) L"void");
    extends = TS_build_parser_token(TS_EXTENDS, &data);
    TS_push_child(extends, TS_ANY_TYPE);
    TS_push_child(type, extends);
    TS_register_type(tsFile, type);
    data.line = 0;
    data.character = 0;
    data.parentTSToken = 0;

    TS_parse_stream(TS_PREDEFINED_PATH, tsFile->stream);

    if (TS_PREDEFINED_FILE->sanity != TS_FILE_VALID) {
      pthread_mutex_unlock(&TS_PREDEFINED_MUTEX_LOCK);
      registers_swapped = FALSE;
      TS_free_tsFile(TS_PREDEFINED_FILE);
      TS_destroy_register();

      fprintf(stderr, "Predefined classes file is malformed! Exiting...\n");
      exit(5);
    }

    swap(u_long, TS_PREDEFINED_REGISTER_SIZE, TS_REGISTER_SIZE);
    swap(RegisterCollection, TS_PREDEFINED_REGISTER, TS_REGISTER);
    registers_swapped = FALSE;
  }
}

void
TS_setup_predefined(
    void
) {
  pthread_mutex_lock(&TS_PREDEFINED_MUTEX_LOCK);
  TS_suppress_logging(__TS_setup_predefined);
  pthread_mutex_unlock(&TS_PREDEFINED_MUTEX_LOCK);
}

void
TS_remove_predefined() {
  pthread_mutex_lock(&TS_PREDEFINED_MUTEX_LOCK);
  if (TS_PREDEFINED_REGISTER != NULL) {
    swap(u_long, TS_PREDEFINED_REGISTER_SIZE, TS_REGISTER_SIZE);
    swap(RegisterCollection, TS_PREDEFINED_REGISTER, TS_REGISTER);
    registers_swapped = TRUE;

    TS_free_tsFile(TS_PREDEFINED_FILE);
    TS_destroy_register();

    swap(u_long, TS_PREDEFINED_REGISTER_SIZE, TS_REGISTER_SIZE);
    swap(RegisterCollection, TS_PREDEFINED_REGISTER, TS_REGISTER);
    registers_swapped = FALSE;

  }
  pthread_mutex_unlock(&TS_PREDEFINED_MUTEX_LOCK);
}

unsigned char TS_is_predefined(TSParserToken *token) {
  TSRegisterEntry *registered = registers_swapped ? TS_REGISTER[0] : TS_PREDEFINED_REGISTER[0];
  TSParserToken *child = NULL;
  for (u_long index = 0; index < registered->listSize; index++) {
    child = registered->classList[index];
    if (child == token) return TRUE;
  }
  return FALSE;
}

unsigned char TS_is_type(TSParserToken *token) {
  if (token->tokenType != TS_CLASS)
    return TRUE;
  if (token->tokenType != TS_INTERFACE)
    return TRUE;
  return FALSE;
}
