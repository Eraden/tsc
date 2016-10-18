#include <cts/register.h>

// Predefined TS types
TSParserToken *__attribute__((__used__)) TS_ANY_TYPE = NULL;
TSParserToken *__attribute__((__used__)) TS_UNDEFINED_TYPE = NULL;
TSParserToken *__attribute__((__used__)) TS_NULL_TYPE = NULL;

// Arithmetic Operators
TSParserToken *__attribute__((__used__)) TS_PLUS = NULL;
TSParserToken *__attribute__((__used__)) TS_MINUS = NULL;
TSParserToken *__attribute__((__used__)) TS_MULTIPLY = NULL;
TSParserToken *__attribute__((__used__)) TS_DIVIDE = NULL;
TSParserToken *__attribute__((__used__)) TS_MODULO = NULL;
TSParserToken *__attribute__((__used__)) TS_INCREMENT = NULL;
TSParserToken *__attribute__((__used__)) TS_DECREMENT = NULL;

// Relational Operators
TSParserToken *__attribute__((__used__)) TS_EQUAL = NULL;
TSParserToken *__attribute__((__used__)) TS_SAME = NULL;
TSParserToken *__attribute__((__used__)) TS_NOT_EQUAL = NULL;
TSParserToken *__attribute__((__used__)) TS_GREATER = NULL;
TSParserToken *__attribute__((__used__)) TS_GREATER_OR_EQUAL = NULL;
TSParserToken *__attribute__((__used__)) TS_LESS = NULL;
TSParserToken *__attribute__((__used__)) TS_LESS_OR_EQUAL = NULL;

// Logical Operators
TSParserToken *__attribute__((__used__)) TS_LOGICAL_AND = NULL;
TSParserToken *__attribute__((__used__)) TS_LOGICAL_OR = NULL;
TSParserToken *__attribute__((__used__)) TS_LOGICAL_NOT = NULL;

// Bitwise Operators
TSParserToken *__attribute__((__used__)) TS_BITWISE_AND = NULL;
TSParserToken *__attribute__((__used__)) TS_BITWISE_OR = NULL;
TSParserToken *__attribute__((__used__)) TS_BITWISE_XOR = NULL;

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

TSParserToken **TS_OPERATORS = NULL;
const unsigned short int TS_OPERATORS_COUNT = 20;

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

static TSParseData
TS_build_parse_data(void) {
  TSParseData data;
  data.line = 0;
  data.character = 0;
  data.token = 0;
  data.stream = 0;
  data.parentTSToken = 0;
  return data;
}

static TSParserToken *
TS_build_operator(wchar_t *operator, unsigned short int index) {
  TSParseData data = TS_build_parse_data();
  data.token = operator;

  TSParserToken *token = TS_parse_operator(NULL, &data);
  TS_OPERATORS[index] = token;
  return token;
}

static void
__TS_setup_operators(void) {
  TS_OPERATORS = calloc(sizeof(TSParserToken *), TS_OPERATORS_COUNT);

  unsigned short int index = 0;
  // Arithmetic Operators
  TS_PLUS = TS_build_operator((wchar_t *) L"+", index++);
  TS_MINUS = TS_build_operator((wchar_t *) L"-", index++);
  TS_MULTIPLY = TS_build_operator((wchar_t *) L"*", index++);
  TS_DIVIDE = TS_build_operator((wchar_t *) L"/", index++);
  TS_MODULO = TS_build_operator((wchar_t *) L"%", index++);
  TS_INCREMENT = TS_build_operator((wchar_t *) L"++", index++);
  TS_DECREMENT = TS_build_operator((wchar_t *) L"--", index++);

  // Relational Operators
  TS_EQUAL = TS_build_operator((wchar_t *) L"==", index++);
  TS_SAME = TS_build_operator((wchar_t *) L"===", index++);
  TS_NOT_EQUAL = TS_build_operator((wchar_t *) L"!=", index++);
  TS_GREATER = TS_build_operator((wchar_t *) L">", index++);
  TS_GREATER_OR_EQUAL = TS_build_operator((wchar_t *) L">=", index++);
  TS_LESS = TS_build_operator((wchar_t *) L"<", index++);
  TS_LESS_OR_EQUAL = TS_build_operator((wchar_t *) L"<=", index++);

  // Logical Operators
  TS_LOGICAL_AND = TS_build_operator((wchar_t *) L"&&", index++);
  TS_LOGICAL_OR = TS_build_operator((wchar_t *) L"||", index++);
  TS_LOGICAL_NOT = TS_build_operator((wchar_t *) L"!", index++);

  // Bitwise Operators
  TS_BITWISE_AND = TS_build_operator((wchar_t *) L"&", index++);
  TS_BITWISE_OR = TS_build_operator((wchar_t *) L"|", index++);
  TS_BITWISE_XOR = TS_build_operator((wchar_t *) L"^", index);
}

static void
__attribute__((visibility("hidden")))
__TS_setup_predefined(void) {
  if (TS_PREDEFINED_REGISTER == NULL) {
    __TS_setup_operators();
    TS_PREDEFINED_REGISTER = calloc(sizeof(TSRegisterEntry *), 1);

    swap(u_long, TS_PREDEFINED_REGISTER_SIZE, TS_REGISTER_SIZE);
    swap(RegisterCollection, TS_PREDEFINED_REGISTER, TS_REGISTER);
    registers_swapped = TRUE;

    const char *path = TS_getUserLibraryPath();
    TSFile *tsFile = TS_PREDEFINED_FILE = TS_find_file(path, NULL);

    TSParseData data;
    TSParserToken *type, *extends;

    // any
    data = TS_build_parse_data();
    type = TS_ANY_TYPE = TS_build_parser_token(TS_CLASS, &data);
    type->name = TS_clone_string((const wchar_t *) L"any");
    TS_register_type(tsFile, type);
    // undefined
    data = TS_build_parse_data();
    type = TS_UNDEFINED_TYPE = TS_build_parser_token(TS_INTERFACE, &data);
    type->name = TS_clone_string((const wchar_t *) L"undefined");
    extends = TS_build_parser_token(TS_EXTENDS, &data);
    TS_push_child(extends, TS_ANY_TYPE);
    TS_push_child(type, extends);
    TS_register_type(tsFile, type);
    // null
    data = TS_build_parse_data();
    type = TS_NULL_TYPE = TS_build_parser_token(TS_INTERFACE, &data);
    type->name = TS_clone_string((const wchar_t *) L"null");
    extends = TS_build_parser_token(TS_EXTENDS, &data);
    TS_push_child(extends, TS_ANY_TYPE);
    TS_push_child(type, extends);
    TS_register_type(tsFile, type);
    // void
    data = TS_build_parse_data();
    type = TS_build_parser_token(TS_FUNCTION, &data);
    type->name = TS_clone_string((const wchar_t *) L"void");
    extends = TS_build_parser_token(TS_EXTENDS, &data);
    TS_push_child(extends, TS_ANY_TYPE);
    TS_push_child(type, extends);
    TS_register_type(tsFile, type);

    TS_parse_stream(path, tsFile->stream);
    free((void *) path);

    if (TS_PREDEFINED_FILE->sanity != TS_FILE_VALID) {
      pthread_mutex_unlock(&TS_PREDEFINED_MUTEX_LOCK);
      registers_swapped = FALSE;
      TS_free_tsFile(TS_PREDEFINED_FILE);
      TS_destroy_register();
      TS_PREDEFINED_FILE = NULL;

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

    TSParserToken **operators = TS_OPERATORS;
    for (unsigned short int size = TS_OPERATORS_COUNT; size;) {
      TS_free_operator(operators[0]);
      size -= 1;
      operators += 1;
    }
    free(TS_OPERATORS);

    swap(u_long, TS_PREDEFINED_REGISTER_SIZE, TS_REGISTER_SIZE);
    swap(RegisterCollection, TS_PREDEFINED_REGISTER, TS_REGISTER);
    registers_swapped = FALSE;

  }
  pthread_mutex_unlock(&TS_PREDEFINED_MUTEX_LOCK);
}

unsigned char TS_is_predefined(TSParserToken *token) {
  if ((registers_swapped ? TS_REGISTER : TS_PREDEFINED_REGISTER) == NULL)
    return FALSE;
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

//TSParserToken *
//TS_search_in(
//    wchar_t *name,
//    TSParserToken *scope,
//    TSFile *tsFile
//) {
//  if (name == NULL) return NULL;
//  if (scope == NULL && tsFile == NULL) return NULL;
//  if (scope == NULL) {
//    TSParserToken **tokens = tsFile->tokens;
//    TSParserToken *token;
//    for (u_long i = 0, len = tsFile->tokensSize; i < len; i++) {
//      token = tokens[0];
//      if (token->name && wcscmp(token->name, name) == 0) {
//        return token;
//      }
//      tokens += 1;
//    }
//  }
//  return NULL;
//}

TSParserToken *
TS_search_token(
    TSParserToken *token
) {
  if (token->tokenType != TS_UNKNOWN) return token;
  const wchar_t *name = token->content;
  volatile TSParserToken *current = token->parent;
  volatile unsigned char proceed = TRUE;
  TSParserToken **children = NULL;
  TSParserToken *child = NULL;
  u_int len = 0, index = 0;

  while (current && proceed) {
    switch (current->tokenType) {
      case TS_SCOPE: {
        len = current->childrenSize;
        children = current->children;
        for (index = 0; index < len; index++) {
          child = children[0];
          if (child->name && wcscmp(name, child->name) == 0) {
            proceed = FALSE;
            current = child;
          }
          children += 1;
        }
        break;
      }
      case TS_FOR: {
        break;
      }
      default: {
        //
      }
    }
    if (proceed) current = current->parent;
  }
  return (TSParserToken *) current;
}
