#include <tsc/parser.h>

static void TS_put_back(FILE *stream, const char *value);
static void TS_validate_name(const char *name);

static void TS_append_ts_parser_token(TSFile *tsFile, TSParserToken *token);
static const TSParserToken TS_parse_var(TSFile *tsFile, TSParseData *tsParseData);
static const TSParserToken TS_parse_let(TSFile *tsFile, TSParseData *tsParseData);
static const TSParserToken TS_parse_const(TSFile *tsFile, TSParseData *tsParseData);
static const TSParserToken TS_parse_class(TSFile *tsFile, TSParseData *tsParseData);
static const TSParserToken TS_parse_function(TSFile *tsFile, TSParseData *tsParseData);
static const TSParserToken TS_parse_arrow(TSFile *tsFile, TSParseData *tsParseData);
static void TS_parse_ts_token(TSFile *tsFile, TSParseData *data);

static const TSParserToken TS_parse_var(TSFile *tsFile, TSParseData *tsParseData) {
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_VAR;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.position = tsParseData->position;

  const char *white = TS_getToken(tsParseData->stream);
  if (white == NULL) exit(1);
  movedBy += strlen(white);
  free((void *) white);

  const char *name = TS_getToken(tsParseData->stream);
  TSLocalVariableData *varData = calloc(sizeof(TSLocalVariableData), 1);
  if (varData == NULL) exit(1);
  TS_validate_name(name);
  movedBy += strlen(name);
  varData->name = name;
  token.data = varData;

  const char *tok;
  while (1) {
    tok = TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      free((void *) tok);
      break;
    } else if (tok[0] == ' ' || tok[0] == ';') {
      movedBy += strlen(tok);
      free((void *) tok);
    } else if (tok[0] == '=') {
      movedBy += strlen(tok);
      free((void *) tok);
      while (1) {
        tok = TS_getToken(tsParseData->stream);
        if (tok == NULL) exit(1);
        if (tok[0] == '\n') exit(2);
        if (tok[0] != ' ') {
          break;
        } else {
          movedBy += strlen(tok);
          free((void *) tok);
        }
      }
      varData->value = tok;
      movedBy += strlen(tok);
      break;
    } else {
      TS_put_back(tsParseData->stream, tok);
      free((void *) tok);
      break;
    }
  }

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  return token;
}

static const TSParserToken TS_parse_let(TSFile *tsFile, TSParseData *tsParseData) {
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_LET;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  return token;
}

static const TSParserToken TS_parse_const(TSFile *tsFile, TSParseData *tsParseData) {
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_CONST;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  return token;
}

static const TSParserToken TS_parse_class(TSFile *tsFile, TSParseData *tsParseData) {
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_CLASS;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  return token;
}

static const TSParserToken TS_parse_function(TSFile *tsFile, TSParseData *tsParseData) {
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_FUNCTION;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  return token;
}

static const TSParserToken TS_parse_arrow(TSFile *tsFile, TSParseData *tsParseData) {
  u_long movedBy = strlen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_ARROW;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;
  return token;
}

static const TSKeyword TS_KEYWORDS[KEYWORDS_SIZE] =  {
    TS_VAR, "var", TS_parse_var,
    TS_LET, "let", TS_parse_let,
    TS_CONST, "const", TS_parse_const,
    TS_CLASS, "class", TS_parse_class,
    TS_FUNCTION, "function", TS_parse_function,
    TS_ARROW, "=>", TS_parse_arrow,
};

static void TS_put_back(FILE *stream, const char *value) {
  for (u_long i = 0, l = strlen(value); i < l; i++) {
    ungetc(value[i], stream);
  }
}

static void TS_validate_name(const char *name) {
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
      case ' ':
        exit(1);
      default:break;
    }
  }
}

static void TS_append_ts_parser_token(TSFile *tsFile, TSParserToken *token) {
  if (tsFile->tokens == NULL) {
    tsFile->tokens = calloc(sizeof(TSParserToken), 1);
    tsFile->tokens[0] = *token;
    tsFile->tokensSize = 1;
  } else {
    tsFile->tokens = realloc(tsFile->tokens, sizeof(TSParserToken) * (tsFile->tokensSize + 1));
    tsFile->tokens[tsFile->tokensSize] = *token;
    tsFile->tokensSize += 1;
  }
}

static void TS_parse_ts_token(TSFile *tsFile, TSParseData *data) {
  for (u_short i = 0; i < KEYWORDS_SIZE; i++) {
    TSKeyword k = TS_KEYWORDS[i];
    if (strcmp(data->token, k.str) == 0) {
      TSParserToken token = k.callback(tsFile, data);
      TS_append_ts_parser_token(tsFile, &token);
    }
  }
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
    case '\\':
    case '%':
    case '\n':
    case ' ':
      return 0;
    default:
      return 1;
  }
}

const char *TS_getToken(FILE *stream) {
  char *tok = NULL;
  char prev = 0;
  char c = 0;
  while (!feof(stream)) {
    c = (char) fgetc(stream);
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
      {
        if (tok == NULL) {
          tok = (char *) calloc(sizeof(char), 1);
          tok[0] = c;
          tok[1] = 0;
          return tok;
        } else {
          ungetc(c, stream);
          return tok;
        }
      }
      case ' ':
      {
        if (tok == NULL) {
          tok = (char *) calloc(sizeof(char), 1);
          tok[0] = c;
          tok[1] = 0;
          prev = c;
          break;
        } else if (tok[0] == ' ') {
          u_long size = strlen(tok);
          tok = (char *) realloc(tok, size + 1);
          tok[size] = c;
          tok[size + 1] = 0;
          prev = c;
          break;
        } else {
          ungetc(c, stream);
          return tok;
        }
      }
      default: {
        if (TS_valid_char_for_token(prev)) {
          if (tok == NULL) {
            tok = (char *) calloc(sizeof(char), 1);
            tok[0] = c;
            tok[1] = 0;
          } else {
            u_long size = strlen(tok);
            tok = (char *) realloc(tok, size + 1);
            tok[size] = c;
            tok[size + 1] = 0;
          }
        } else {
          ungetc(c, stream);
          return tok;
        }
      }
    }
  }
  return tok;
}

const TSFile TS_parse_file(const char *file) {
  TSFile tsFile;
  tsFile.tokens = NULL;
  tsFile.file = file;

  const char *tok;
  FILE *stream = fopen(file, "r");
  if (stream == NULL) {
    return tsFile;
  }

  TSParseData data;
  data.character = 0;
  data.line = 0;
  data.position = 0;
  data.stream = stream;

  while (1) {
    tok = TS_getToken(stream);
    if (tok == NULL) break;

    data.token = tok;
    if (data.token[0] == '\n') {
      TS_next_line(&data);
    } else {
      TS_parse_ts_token(&tsFile, &data);
    }

    free((void *) tok);
  }
  return tsFile;
}