#include <tsc/parser.h>

static void TS_append_ts_parser_token(TSFile *tsFile, TSParserToken token);

static void TS_next_line(TSParseData *data);

const wchar_t *
__attribute__((__malloc__))
TS_clone_string(
    const wchar_t *string
) {
  wchar_t *clone = calloc(sizeof(wchar_t), wcslen(string) + TS_STRING_END);
  wcscpy(clone, string);
  return clone;
}

static const TSKeyword TS_KEYWORDS[KEYWORDS_SIZE] = {
    TS_VAR,               (wchar_t *) L"var",         TS_parse_var,
    TS_LET,               (wchar_t *) L"let",         TS_parse_let,
    TS_CONST,             (wchar_t *) L"const",       TS_parse_const,
    TS_CLASS,             (wchar_t *) L"class",       TS_parse_class,
    TS_FUNCTION,          (wchar_t *) L"function",    TS_parse_function,
    TS_ARROW,             (wchar_t *) L"=>",          TS_parse_arrow,
    TS_IF,                (wchar_t *) L"if",          TS_parse_if,
    TS_ELSE,              (wchar_t *) L"else",        TS_parse_else,
    TS_RETURN,            (wchar_t *) L"return",      TS_parse_return,
    TS_DECORATOR,         (wchar_t *) L"@",           TS_parse_decorator,
    TS_IMPORT,            (wchar_t *) L"import",      TS_parse_import,
    TS_EXPORT,            (wchar_t *) L"export",      TS_parse_export,
    TS_DEFAULT,           (wchar_t *) L"default",     TS_parse_default,
    TS_SCOPE,             (wchar_t *) L"{",           TS_parse_scope,
    TS_EXTENDS,           (wchar_t *) L"extends",     TS_parse_extends,
    TS_IMPLEMENTS,        (wchar_t *) L"implements",  TS_parse_implements,
    TS_NEW,               (wchar_t *) L"new",         TS_parse_new,
    TS_INLINE_COMMENT,    (wchar_t *) L"//",          TS_parse_inline_comment,
    TS_MULTILINE_COMMENT, (wchar_t *) L"/*",          TS_parse_multiline_comment,
};

void TS_put_back(FILE *stream, const wchar_t *value) {
  for (long i = wcslen(value) - 1; i >= 0; --i) {
    ungetwc((wint_t) value[i], stream);
  }
}

unsigned char
TS_name_is_valid(
    const wchar_t *name
) {
  wchar_t c;
  for (u_long i = 0, l = wcslen(name); i < l; i++) {
    c = name[i];
    switch (c) {
      case L'@':
      case L'\'':
      case L'"':
      case L'{':
      case L'}':
      case L'(':
      case L')':
      case L',':
      case L'.':
      case L'=':
      case L':':
      case L';':
      case L'+':
      case L'-':
      case L'/':
      case L'*':
      case L'\\':
      case L'%':
      case L'\n':
      case L' ': {
        return 0;
      }
      default:
        break;
    }
  }
  return 1;
}

void
TS_push_child(
    TSParserToken *token,
    TSParserToken child
) {
  log_to_file((wchar_t *) L"%s\n", "Pushing new TSParserToken child to TSParserToken parent");
  TSParserToken *newPointer = (TSParserToken *) calloc(sizeof(TSParserToken), token->childrenSize + 1);
  if (token->children != NULL) memcpy(newPointer, token->children, sizeof(TSParserToken) * token->childrenSize);
  if (token->children != NULL) free(token->children);
  token->children = newPointer;
  token->children[token->childrenSize] = child;
  token->childrenSize += 1;
  log_to_file((wchar_t *) L"    size increased to: %lu\n", token->childrenSize);
}

static void
TS_append_ts_parser_token(
    TSFile *tsFile,
    TSParserToken token
) {
  TSParserToken *newPointer = (TSParserToken *) calloc(sizeof(TSParserToken), tsFile->tokensSize + 1);
  if (tsFile->tokens != NULL) memcpy(newPointer, tsFile->tokens, sizeof(TSParserToken) * tsFile->tokensSize);
  if (tsFile->tokens != NULL) free(tsFile->tokens);
  tsFile->tokens = newPointer;
  tsFile->tokens[tsFile->tokensSize] = token;
  tsFile->tokensSize += 1;
}

TSParserToken
TS_parse_ts_token(
    TSFile *tsFile,
    TSParseData *data
) {
  for (u_short i = 0; i < KEYWORDS_SIZE; i++) {
    TSKeyword k = TS_KEYWORDS[i];
    if (wcscmp(data->token, k.str) == 0) {
      log_to_file((wchar_t *) L"  -  data->token = \"%ls\"\n  -  k.str = \"%ls\"\n", data->token, k.str);
      TSParserToken token = k.callback(tsFile, data);
      return token;
    }
  }
  TSParserToken t;
  t.data = (void *) TS_clone_string(data->token);
  t.children = NULL;
  t.childrenSize = 0;
  t.line = data->line;
  t.character = data->character;
  t.position = data->position;
  t.tokenType = TS_UNKNOWN;
  return t;
}

static void
TS_next_line(
    TSParseData *data
) {
  data->position += 1;
  data->line += 1;
  data->character = 0;
}

static u_short
__attribute__((visibility("hidden")))
TS_valid_char_for_token(wchar_t c) {
  switch (c) {
    case L'@':
    case L'\'':
    case L'"':
    case L'{':
    case L'}':
    case L'(':
    case L')':
    case L',':
    case L'.':
    case L'=':
    case L':':
    case L';':
    case L'+':
    case L'-':
    case L'/':
    case L'*':
    case L'!':
    case L'|':
    case L'&':
    case L'#':
    case L'\\':
    case L'%':
    case L'\n':
    case L' ':
      return 0;
    default:
      return 1;
  }
}

volatile const wchar_t *
TS_getToken(
    FILE *stream
) {
  volatile wchar_t *tok = NULL;
  volatile wchar_t prev = 0;
  volatile wchar_t c = 0;

  while (!feof(stream)) {
    c = (wchar_t) fgetwc(stream);
    switch (c) {
//      case WEOF:
      case -1:
      case 0: {
        return tok;
      }
      case L'*': {
        if (tok == NULL) {
          wchar_t next = (wchar_t) fgetc(stream);
          switch (next) {
            case L'/':
              break;
            default: {
              ungetwc((wint_t) next, stream);
              next = 0;
              break;
            }
          }
          tok = (wchar_t *) calloc(sizeof(wchar_t), (next != 0 ? 2 : 1) + TS_STRING_END);
          tok[0] = c;
          if (next != 0) tok[1] = next;
          return tok;
        } else {
          ungetwc((wint_t) c, stream);
          log_to_file((wchar_t *) L"# token: '%ls' [single token + else]\n", tok);
          return tok;
        }
      }
      case L'/': {
        if (tok == NULL) {
          wchar_t next = (wchar_t) fgetc(stream);
          switch (next) {
            case L'*':
            case L'/':
              break;
            default: {
              ungetwc((wint_t) next, stream);
              next = 0;
              break;
            }
          }
          tok = (wchar_t *) calloc(sizeof(wchar_t), (next != 0 ? 2 : 1) + TS_STRING_END);
          tok[0] = c;
          if (next != 0) tok[1] = next;
          return tok;
        } else {
          ungetwc((wint_t) c, stream);
          log_to_file((wchar_t *) L"# token: '%ls' [single token + else]\n", tok);
          return tok;
        }
      }
      case L'@':
      case L'\'':
      case L'"':
      case L'{':
      case L'}':
      case L'(':
      case L')':
      case L',':
      case L'.':
      case L'=':
      case L':':
      case L';':
      case L'+':
      case L'-':
      case L'!':
      case L'|':
      case L'&':
      case L'#':
      case L'\\':
      case L'%':
      case L'\n': {
        if (tok == NULL) {
          tok = (wchar_t *) calloc(sizeof(wchar_t), 1 + TS_STRING_END);
          tok[0] = c;
          return tok;
        } else {
          ungetwc((wint_t) c, stream);
          log_to_file((wchar_t *) L"# token: '%ls' [single token + else]\n", tok);
          return tok;
        }
      }
      case L' ': {
        if (tok == NULL) {
          tok = (wchar_t *) calloc(sizeof(wchar_t), 1 + TS_STRING_END);
          tok[0] = c;
          prev = c;
          break;

        } else if (tok[0] == ' ') {
          u_long size = wcslen((const wchar_t *) tok);
          volatile wchar_t *newPointer = calloc(sizeof(wchar_t), size + 1 + TS_STRING_END);
          wcscpy((wchar_t *) newPointer, (const wchar_t *) tok);
          free((void *) tok);
          tok = newPointer;
          tok[size] = c;
          prev = c;
          break;

        } else if (wcslen((const wchar_t *) tok) == 0) {
          free((void *) tok);
          return NULL;

        } else {
          ungetwc((wint_t) c, stream);
          log_to_file((wchar_t *) L"# token: '%ls' [white + else]\n", tok);
          return tok;
        }
      }
      default: {
        if (TS_valid_char_for_token(prev)) {
          u_long size = tok ? wcslen((const wchar_t *) tok) : 0;
          volatile wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), size + 1 + TS_STRING_END);
          if (tok != NULL) wcscpy((wchar_t *) newPointer, (const wchar_t *) tok);
          if (tok != NULL) free((void *) tok);
          tok = newPointer;
          tok[size] = c;
        } else {
          ungetwc((wint_t) c, stream);
          log_to_file((wchar_t *) L"# token: '%ls' [default + else]\n", tok);
          return tok;
        }
      }
    }
  }
  if (tok != NULL && tok[0] == -1) {
    free((void *) tok);
    return NULL;
  }
  log_to_file((wchar_t *) L"# token: '%ls' [done return]\n", tok);
  return tok;
}

const TSFile
TS_parse_file(const char *fileName) {
  FILE *stream = fopen(fileName, "r");

  if (stream == NULL) {
    fprintf(stderr, "Could not open file '%s' to read!\n", fileName);
    fprintf(stderr, "  failure message: '%s'\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  return TS_parse_stream(fileName, stream);
}

const TSFile
TS_parse_stream(
    const char *file,
    FILE *stream
) {
  TSFile tsFile;
  tsFile.tokens = NULL;
  tsFile.tokensSize = 0;
  tsFile.file = file;

  TSParseData data;
  data.line = 1;
  data.character = 0;
  data.position = 0;
  data.stream = stream;

  const wchar_t *tok;
  while (1) {
    tok = (const wchar_t *) TS_getToken(stream);
    if (tok == NULL) break;

    data.token = tok;
    if (data.token[0] == '\n') {
      TS_next_line(&data);
    } else {
      TSParserToken token = TS_parse_ts_token(&tsFile, &data);
      if (token.tokenType != TS_UNKNOWN) {
        TS_append_ts_parser_token(&tsFile, token);
      } else {
        free(token.data);
      }
    }

    free((void *) tok);
  }
  return tsFile;
}

void
TS_free_unknown(
    const TSParserToken token
) {
  TS_free_children(token);

  if (token.data) free(token.data);
}

void
TS_free_tsToken(
    const TSParserToken token
) {
  switch (token.tokenType) {
    case TS_VAR:
      TS_free_var(token);
      break;
    case TS_LET:
      TS_free_let(token);
      break;
    case TS_CONST:
      TS_free_const(token);
      break;
    case TS_CLASS:
      TS_free_class(token);
      break;
    case TS_FUNCTION:
      TS_free_function(token);
      break;
    case TS_ARROW:
      TS_free_arrow(token);
      break;
    case TS_IF:
      TS_free_if(token);
      break;
    case TS_ELSE:
      TS_free_else(token);
      break;
    case TS_RETURN:
      TS_free_return(token);
      break;
    case TS_DECORATOR:
      TS_free_decorator(token);
      break;
    case TS_IMPORT:
      TS_free_import(token);
      break;
    case TS_EXPORT:
      TS_free_export(token);
      break;
    case TS_DEFAULT:
      TS_free_default(token);
      break;
    case TS_SCOPE:
      TS_free_scope(token);
      break;
    case TS_EXTENDS:
      TS_free_extends(token);
      break;
    case TS_IMPLEMENTS:
      TS_free_implements(token);
      break;
    case TS_NEW:
      TS_free_new(token);
      break;
    case TS_CLASS_FIELD:
      TS_free_class_field(token);
      break;
    case TS_CLASS_METHOD:
      TS_free_class_method(token);
      break;
    case TS_INLINE_COMMENT:
      TS_free_inline_comment(token);
      break;
    case TS_MULTILINE_COMMENT:
      TS_free_multiline_comment(token);
      break;
    case TS_UNKNOWN:
      TS_free_unknown(token);
      break;
  }
}

void TS_free_children(const TSParserToken token) {
  for (u_long childIndex = 0; childIndex < token.childrenSize; childIndex++) {
    TS_free_tsToken(token.children[childIndex]);
  }
  if (token.childrenSize > 0) free(token.children);
}

void
TS_free_tsFile(
    const TSFile tsFile
) {
  for (u_long index = 0; index < tsFile.tokensSize; index++) {
    TS_free_tsToken(tsFile.tokens[index]);
  }
  if (tsFile.tokensSize > 0) free(tsFile.tokens);
}
