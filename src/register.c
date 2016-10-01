#include <cts/register.h>

#define TS_PREDEFINED_SIZE 8

static pthread_mutex_t
__attribute__(( visibility("hidden")))
    REGISTER_CLASS_MUTEX_LOCK, REGISTER_FILE_MUTEX_LOCK;

static RegisterCollection
__attribute__(( visibility("hidden")))
TS_REGISTER = NULL;

static u_long
__attribute__(( visibility("hidden")))
    TS_REGISTER_SIZE = 0;

static RegisterCollection
__attribute__(( visibility("hidden")))
TS_PREDEFINED_REGISTER = NULL;

static u_long
__attribute__(( visibility("hidden")))
    TS_PREDEFINED_REGISTER_SIZE = 0;

TSRegisterEntry *
TS_register_file(
    TSFile *tsFile
) {
  TSRegisterEntry *entry = calloc(sizeof(TSRegisterEntry), 1);
  entry->tsFile = tsFile;
  entry->classList = NULL;
  entry->listSize = 0;

  pthread_mutex_lock(&REGISTER_FILE_MUTEX_LOCK);

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
    memcpy(newPointer, entry->classList, entry->listSize);
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
    if (wcscmp(entry->tsFile->file, file) == 0) {
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
    if (wcscmp(entry->tsFile->file, file) == 0) {
      for (u_long clsIndex = 0; clsIndex < entry->listSize; clsIndex++) {
        TSParserToken *cls = entry->classList[clsIndex];
        if (wcscmp(cls->classData->name, name) == 0) {
          result = cls;
          goto exit_find_class;
        }
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
  for (u_long i = 0; i < TS_REGISTER_SIZE; i++) {
    TSRegisterEntry *entry = TS_REGISTER[i];
    free(entry->classList);
    free(entry);
  }
  free(TS_REGISTER);
}

static TSParserToken *
TS_build_predefined_class(
    wchar_t *name
) {
  TSParseData data;
  TSParserToken *token;
  data.stream = NULL;
  data.parentTSToken = NULL;
  data.token = NULL;
  data.character = 0;
  data.line = 0;
  data.line = 0;
  token = TS_build_parser_token(TS_CLASS, &data);
  token->name = calloc(sizeof(wchar_t), wcslen(name) + TS_STRING_END);
  wcscpy(token->name, name);
  return token;
}

void
__attribute__(( __unused__ ))
TS_initialize_register(
    void
) {
  TSFile *tsFile = calloc(sizeof(TSFile), 1);
  tsFile->errorReason = NULL;
  tsFile->file = NULL;
  tsFile->sanity = TS_FILE_VALID;
  tsFile->stream = NULL;
  tsFile->tokens = NULL;
  tsFile->tokensSize = 0;

  TSRegisterEntry *registerEntry = calloc(sizeof(TSRegisterEntry), 1);
  registerEntry->classList = calloc(sizeof(TSParserToken *), TS_PREDEFINED_SIZE);
  registerEntry->listSize = TS_PREDEFINED_SIZE;
  registerEntry->tsFile = tsFile;

  TS_PREDEFINED_REGISTER = calloc(sizeof(TSRegisterEntry *), 1);
  TS_PREDEFINED_REGISTER[0] = registerEntry;

  TSParserToken *token = NULL;

  token = TS_build_predefined_class((wchar_t *) L"Object");
  registerEntry->classList[0] = token;

  token = TS_build_predefined_class((wchar_t *) L"Function");
  registerEntry->classList[1] = token;

  token = TS_build_predefined_class((wchar_t *) L"Array");
  registerEntry->classList[2] = token;

  token = TS_build_predefined_class((wchar_t *) L"Boolean");
  registerEntry->classList[3] = token;

  token = TS_build_predefined_class((wchar_t *) L"Number");
  registerEntry->classList[4] = token;

  token = TS_build_predefined_class((wchar_t *) L"Date");
  registerEntry->classList[5] = token;

  token = TS_build_predefined_class((wchar_t *) L"String");
  registerEntry->classList[6] = token;

  token = TS_build_predefined_class((wchar_t *) L"RegExp");
  registerEntry->classList[7] = token;
}

void
__attribute__(( __unused__ ))
TS_register_remove_class(
    TSFile *tsFile,
    TSParserToken *token
) {
//  for (u_long entryIndex = 0; entryIndex < TS_REGISTER_SIZE; entryIndex++) {
//    TSRegisterEntry *entry = TS_REGISTER[entryIndex];
//    if (entry->tsFile == tsFile) {
//      for (u_long clsIndex = 0; clsIndex < entry->listSize; clsIndex++) {
//        TSParserToken *cls = entry->classList[clsIndex];
//        if (cls == token) {
//          TSParserToken **newPointer = calloc(sizeof(TSParserToken), entry->listSize - 1);
//          break;
//        }
//      }
//      break;
//    }
//  }
}

