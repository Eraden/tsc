#include <tsc/parser.h>

static void TS_append_ts_parser_token(TSFile *tsFile, TSParserToken token);

static void TS_next_line(TSParseData *data);

const char *
__attribute__((__malloc__))
TS_clone_string(
    const char *string
) {
  char *clone = (char *) calloc(sizeof(char), sizeof(string) + 1);
  clone[sizeof(string)] = 0;
  strcpy(clone, string);
  return clone;
}

static const TSKeyword TS_KEYWORDS[KEYWORDS_SIZE] = {
    TS_VAR, "var", TS_parse_var,
    TS_LET, "let", TS_parse_let,
    TS_CONST, "const", TS_parse_const,
    TS_CLASS, "class", TS_parse_class,
    TS_FUNCTION, "function", TS_parse_function,
    TS_ARROW, "=>", TS_parse_arrow,
    TS_IF, "if", TS_parse_if,
    TS_ELSE, "else", TS_parse_else,
    TS_RETURN, "return", TS_parse_return,
    TS_COMPONENT, "@", TS_parse_decorator,
    TS_IMPORT, "import", TS_parse_import,
    TS_EXPORT, "export", TS_parse_export,
    TS_DEFAULT, "default", TS_parse_default,
    TS_SCOPE, "{", TS_parse_scope,
    TS_EXTENDS, "extends", TS_parse_extends,
    TS_IMPLEMENTS, "implements", TS_parse_implements,
};

void TS_put_back(FILE *stream, const char *value) {
  for (long i = strlen(value) - 1; i >= 0; --i) {
    ungetc(value[i], stream);
  }
}

unsigned char TS_name_is_valid(const char *name) {
  char c;
  for (u_long i = 0, l = strlen(name); i < l; i++) {
    c = name[i];
    switch (c) {
      case '@':
      case '\'':
      case '"':
      case '{':
      case '}':
      case '(':
      case ')':
      case ',':
      case '.':
      case '=':
      case ':':
      case ';':
      case '+':
      case '-':
      case '/':
      case '*':
      case '\\':
      case '%':
      case '\n':
      case ' ': {
        return 0;
      }
      default:
        break;
    }
  }
  return 1;
}

void TS_push_child(TSParserToken *token, TSParserToken child) {
  log_to_file("%s\n", "Pushing new TSParserToken child to TSParserToken parent");
  TSParserToken *newPointer = (TSParserToken *) calloc(sizeof(TSParserToken), token->childrenSize + 1);
  if (token->children != NULL) memcpy(newPointer, token->children, sizeof(TSParserToken) * token->childrenSize);
  if (token->children != NULL) free(token->children);
  token->children = newPointer;
  token->children[token->childrenSize] = child;
  token->childrenSize += 1;
  log_to_file("    size increased to: %lu\n", token->childrenSize);
}

static void TS_append_ts_parser_token(TSFile *tsFile, TSParserToken token) {
  TSParserToken *newPointer = (TSParserToken *) calloc(sizeof(TSParserToken), tsFile->tokensSize + 1);
  if (tsFile->tokens != NULL) memcpy(newPointer, tsFile->tokens, sizeof(TSParserToken) * tsFile->tokensSize);
  if (tsFile->tokens != NULL) free(tsFile->tokens);
  tsFile->tokens = newPointer;
  tsFile->tokens[tsFile->tokensSize] = token;
  tsFile->tokensSize += 1;
}

TSParserToken TS_parse_ts_token(TSFile *tsFile, TSParseData *data) {
  for (u_short i = 0; i < KEYWORDS_SIZE; i++) {
    TSKeyword k = TS_KEYWORDS[i];
    if (strcmp(data->token, k.str) == 0) {
      log_to_file("  -  data->token = \"%s\"\n  -  k.str = \"%s\"\n", data->token, k.str);
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

static void TS_next_line(TSParseData *data) {
  data->position += 1;
  data->line += 1;
  data->character = 0;
}

static u_short TS_valid_char_for_token(char c) {
  switch (c) {
    case '@':
    case '\'':
    case '"':
    case '{':
    case '}':
    case '(':
    case ')':
    case ',':
    case '.':
    case '=':
    case ':':
    case ';':
    case '+':
    case '-':
    case '/':
    case '*':
    case '!':
    case '|':
    case '&':
    case '#':
    case '\\':
    case '%':
    case '\n':
    case ' ':
      return 0;
    default:
      return 1;
  }
}

volatile const char *TS_getToken(FILE *stream) {
  volatile char *tok = NULL;
  volatile char prev = 0;
  volatile char c = 0;

  while (!feof(stream)) {
    c = (char) fgetc(stream);
    switch (c) {
      case -1:
      case 0: {
        return tok;
      }
      case '@':
      case '\'':
      case '"':
      case '{':
      case '}':
      case '(':
      case ')':
      case ',':
      case '.':
      case '=':
      case ':':
      case ';':
      case '+':
      case '-':
      case '/':
      case '*':
      case '!':
      case '|':
      case '&':
      case '#':
      case '\\':
      case '%':
      case '\n': {
        if (tok == NULL) {
          tok = (char *) calloc(sizeof(char), 1 + TS_STRING_END);
          tok[0] = c;
          return tok;
        } else {
          ungetc(c, stream);
          log_to_file("# token: '%s' [single token + else]\n", tok);
          return tok;
        }
      }
      case ' ': {
        if (tok == NULL) {
          tok = (char *) calloc(sizeof(char), 1 + TS_STRING_END);
          tok[0] = c;
          prev = c;
          break;
        } else if (tok[0] == ' ') {
          u_long size = strlen((const char *) tok);
          volatile char *newPointer = calloc(sizeof(char), size + 1 + TS_STRING_END);
          strcpy((char *) newPointer, (const char *) tok);
          free((void *) tok);
          tok = newPointer;
          tok[size] = c;
          prev = c;
          break;
        } else if (strlen((const char *) tok) == 0) {
          free((void *) tok);
          return NULL;
        } else {
          ungetc(c, stream);
          log_to_file("# token: '%s' [white + else]\n", tok);
          return tok;
        }
      }
      default: {
        if (TS_valid_char_for_token(prev)) {
          u_long size = tok ? strlen((const char *) tok) : 0;
          volatile char *newPointer = (char *) calloc(sizeof(char), size + 1 + TS_STRING_END);
          if (tok != NULL) strcpy((char *) newPointer, (const char *) tok);
          if (tok != NULL) free((void *) tok);
          tok = newPointer;
          tok[size] = c;
        } else {
          ungetc(c, stream);
          log_to_file("# token: '%s' [default + else]\n", tok);
          return tok;
        }
      }
    }
  }
  if (tok != NULL && tok[0] == -1) {
    free((void *) tok);
    return NULL;
  }
  log_to_file("# token: '%s' [done return]\n", tok);
  return tok;
}

const TSFile
TS_parse_file(
    const char *file
) {
  FILE *stream = fopen(file, "r");
  if (stream == NULL) {
    TSFile tsFile;
    tsFile.tokens = NULL;
    tsFile.tokensSize = 0;
    tsFile.file = file;
    return tsFile;
  }

  return TS_parse_stream(file, stream);
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

  const char *tok;
  while (1) {
    tok = (const char *) TS_getToken(stream);
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