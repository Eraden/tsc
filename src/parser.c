#include <cts/register.h>

static void
TS_append_ts_parser_token(
    TSFile *tsFile,
    TSParserToken *token
);

static void
TS_next_line(
    TSParseData *data
);

wchar_t *
TS_clone_string(
    const wchar_t *string
) {
  wchar_t *clone = NULL;
  if (string) {
    clone = calloc(sizeof(wchar_t), wcslen(string) + TS_STRING_END);
    wcscpy(clone, string);
  }
  return clone;
}

TSParserToken *
TS_build_parser_token(
    TSTokenType tokenType,
    TSParseData *tsParseData
) {
  TSParserToken *token = TS_NEW_TOKEN;
  token->tokenType = tokenType;
  token->position = tsParseData->position;
  token->character = tsParseData->character;
  token->line = tsParseData->line;
  token->modifiers = TS_MODIFIER_SCOPE;
  token->children = NULL;
  token->childrenSize = 0;
  token->parent = tsParseData->parentTSToken;
  token->data = NULL;
  tsParseData->parentTSToken = token;
  return token;
}

static const TSKeyword TS_KEYWORDS[KEYWORDS_SIZE] = {
    {(wchar_t *) L"var",        TS_parse_var},
    {(wchar_t *) L"let",        TS_parse_let},
    {(wchar_t *) L"const",      TS_parse_const},
    {(wchar_t *) L"class",      TS_parse_class},
    {(wchar_t *) L"function",   TS_parse_function},
    {(wchar_t *) L"=>",         TS_parse_arrow},
    {(wchar_t *) L"if",         TS_parse_if},
    {(wchar_t *) L"else",       TS_parse_else},
    {(wchar_t *) L"return",     TS_parse_return},
    {(wchar_t *) L"@",          TS_parse_decorator},
    {(wchar_t *) L"import",     TS_parse_import},
    {(wchar_t *) L"export",     TS_parse_export},
    {(wchar_t *) L"default",    TS_parse_default},
    {(wchar_t *) L"{",          TS_parse_scope_or_json},
    {(wchar_t *) L"extends",    TS_parse_extends},
    {(wchar_t *) L"implements", TS_parse_implements},
    {(wchar_t *) L"interface",  TS_parse_interface},
    {(wchar_t *) L"new",        TS_parse_new},
    {(wchar_t *) L"//",         TS_parse_inline_comment},
    {(wchar_t *) L"/*",         TS_parse_multiline_comment},
    {(wchar_t *) L"switch",     TS_parse_switch},
    {(wchar_t *) L"case",       TS_parse_case},
    {(wchar_t *) L"break",      TS_parse_break},
    {(wchar_t *) L"for",        TS_parse_for},
    {(wchar_t *) L"of",         TS_parse_of},
    {(wchar_t *) L"in",         TS_parse_in},
    {(wchar_t *) L"[",          TS_parse_array},
    {(wchar_t *) L"\"",         TS_parse_string},
    {(wchar_t *) L"\'",         TS_parse_string},
};

unsigned char TS_is_keyword(const wchar_t *str) {
  for (unsigned short int i = 0; i < KEYWORDS_SIZE; i++) {
    TSKeyword kw = TS_KEYWORDS[i];
    if (wcscmp(kw.str, str) == 0) return 1;
  }
  return 0;
}

void TS_put_back(FILE *stream, volatile const wchar_t *value) {
  for (long i = wcslen((const wchar_t *) value) - 1; i >= 0; --i) {
    ungetwc((wint_t) value[i], stream);
  }
}

unsigned char
TS_name_is_valid(
    const wchar_t *name
) {
  if (name == NULL) return 0;
  wchar_t c = 0;
  const u_long len = wcslen(name);
  for (u_long i = 0; i < len; i++) {
    c = name[i];
    switch (c) {
      case L'@':
      case L'\'':
      case L'"':
      case L'{':
      case L'}':
      case L'(':
      case L')':
      case L'[':
      case L']':
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
        return FALSE;
      }
      default:
        break;
    }
  }
  return TRUE;
}

void
TS_push_child(
    TSParserToken *token,
    TSParserToken *child
) {
  TS_log_to_file((wchar_t *) L"%s\n", "Pushing new TSParserToken child to TSParserToken parent");
  TSParserToken **newPointer = (TSParserToken **) calloc(sizeof(TSParserToken *), token->childrenSize + 1);
  if (token->children != NULL) memcpy(newPointer, token->children, sizeof(TSParserToken *) * token->childrenSize);
  if (token->children != NULL) free(token->children);
  token->children = newPointer;
  token->children[token->childrenSize] = child;
  token->childrenSize += 1;
  TS_log_to_file((wchar_t *) L"    size increased to: %lu\n", token->childrenSize);
}

static void
TS_append_ts_parser_token(
    TSFile *tsFile,
    TSParserToken *token
) {
  TSParserToken **newPointer = (TSParserToken **) calloc(sizeof(TSParserToken *), tsFile->tokensSize + 1);
  if (tsFile->tokens != NULL) memcpy(newPointer, tsFile->tokens, sizeof(TSParserToken *) * tsFile->tokensSize);
  if (tsFile->tokens != NULL) free(tsFile->tokens);
  tsFile->tokens = newPointer;
  tsFile->tokens[tsFile->tokensSize] = token;
  tsFile->tokensSize += 1;
}

TSParserToken *
TS_parse_ts_token(
    TSFile *tsFile,
    TSParseData *data
) {
  if (tsFile->sanity == TS_FILE_VALID) {
    for (u_short i = 0; i < KEYWORDS_SIZE; i++) {
      TSKeyword k = TS_KEYWORDS[i];
      if (wcscmp(data->token, k.str) == 0) {
        TS_log_to_file((wchar_t *) L"  -  data->token = \"%ls\"\n  -  k.str = \"%ls\"\n", data->token, k.str);
        TSParserToken *token = k.callback(tsFile, data);
        return token;
      }
    }
  }

  TSParserToken *t = calloc(sizeof(TSParserToken), 1);
  t->content = (void *) TS_clone_string(data->token);
  t->children = NULL;
  t->childrenSize = 0;
  t->line = data->line;
  t->character = data->character;
  t->position = data->position;
  t->tokenType = TS_UNKNOWN;
  t->parent = data->parentTSToken;

  if (data->parentTSToken == NULL && data->token != NULL) {
    switch (data->token[0]) {
      case L' ':
      case L'\n':
      case L';': {
        break;
      }
      default: {
        ts_token_syntax_error(
            (const wchar_t *) L"Unknown token in global scope!",
            tsFile,
            t
        );
        fwprintf(stderr, (const wchar_t *) L"      invalid token: '%ls'\n", t->content);
        break;
      }
    }
  }

  return t;
}

static void
TS_next_line(
    TSParseData *data
) {
  u_long len = wcslen(data->token);
  data->position += len;
  data->line += 1;
  data->character = 0;
}

static u_short
__attribute__((visibility("hidden")))
TS_valid_char_for_token(
    wchar_t c
) {
  switch (c) {
    case L'@':
    case L'\'':
    case L'"':
    case L'{':
    case L'}':
    case L'(':
    case L')':
    case L'[':
    case L']':
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
  if (stream == NULL) return NULL;

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
        TS_GET_TOKEN_MSG((wchar_t *) L"# '*' character, checking if it's end of multiline token...\n", tok);
        if (tok == NULL) {
          TS_GET_TOKEN_MSG((wchar_t *) L"# tok is %ls\n", tok);
          wchar_t next = (wchar_t) fgetwc(stream);
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
          TS_GET_TOKEN_MSG((wchar_t *) L"# tok isn't null, putting back and returning token\n", tok);
          return tok;
        }
      }
      case L'/': {
        TS_GET_TOKEN_MSG((wchar_t *) L"# Comment character...\n", tok);
        if (tok == NULL) {
          wchar_t next = (wchar_t) fgetwc(stream);
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
          TS_GET_TOKEN_MSG((wchar_t *) L"# Returning comment token...\n", tok);
          return tok;
        } else {
          ungetwc((wint_t) c, stream);
          TS_GET_TOKEN_MSG((wchar_t *) L"# Putting back and returning token since token is '%ls'...\n", tok);
          return tok;
        }
      }
      case L'-':
      case L'+':
      case L'|':
      case L'&': {
        TS_GET_TOKEN_MSG((wchar_t *) L"# Arithmetic character...\n", tok);
        if (tok == NULL || (tok[0] == c && prev == c && wcslen((const wchar_t *) tok) == 1)) {
          const size_t size = tok == NULL ? 1 : wcslen((const wchar_t *) tok) + 1;

          wchar_t *newPointer = calloc(sizeof(wchar_t), size + TS_STRING_END);
          if (tok != NULL) wcscpy(newPointer, (const void *) tok);
          if (tok != NULL) free((void *) tok);

          newPointer[0] = c;
          if (size == 2) newPointer[1] = c;
          prev = c;

          tok = newPointer;
          wchar_t next = (wchar_t) fgetwc(stream);

          ungetwc((wint_t) next, stream);

          if (next != c) return tok;
        } else {
          ungetwc((wint_t) c, stream);
          TS_GET_TOKEN_MSG((wchar_t *) L"# token: '%ls' [single token + else]\n", tok);
          return tok;
        }
        break;
      }
      case L'=': {
        TS_GET_TOKEN_MSG((wchar_t *) L"# '=' character building token...\n", tok)
        if (tok == NULL || (tok[0] == c && prev == c)) {
          const size_t size = tok == NULL ? 1 : wcslen((const wchar_t *) tok) + 1;

          wchar_t *newPointer = calloc(sizeof(wchar_t), size + TS_STRING_END);
          if (tok != NULL) wcscpy(newPointer, (const void *) tok);
          if (tok != NULL) free((void *) tok);

          newPointer[0] = c;
          if (size == 2) newPointer[1] = c;
          if (size == 3) newPointer[2] = c;
          prev = c;

          tok = newPointer;
          wchar_t next = (wchar_t) fgetwc(stream);

          ungetwc((wint_t) next, stream);

          if (next != c) return tok;
          if (size == 3) return tok;
        } else {
          ungetwc((wint_t) c, stream);
          TS_GET_TOKEN_MSG(
              (wchar_t *) L"# Putting back since token ('%ls') exists and contains invalid characters...\n",
              tok
          )
          return tok;
        }
        break;
      }
      case L'@':
      case L'\'':
      case L'"':
      case L'{':
      case L'}':
      case L'(':
      case L')':
      case L'[':
      case L']':
      case L',':
      case L'.':
      case L':':
      case L';':
      case L'!':
      case L'#':
      case L'\\':
      case L'%':
      case L'\n': {
        TS_GET_TOKEN_MSG((wchar_t *) L"# Special character '%lc', tok is '%ls'\n", c, tok);
        if (tok == NULL) {
          tok = (wchar_t *) calloc(sizeof(wchar_t), 1 + TS_STRING_END);
          tok[0] = c;
          return tok;
        } else {
          ungetwc((wint_t) c, stream);
          TS_GET_TOKEN_MSG((wchar_t *) L"# Putting back since token already exists!\n", tok);
          return tok;
        }
      }
      case L' ': {
        if (tok == NULL) {
          TS_GET_TOKEN_MSG((wchar_t *) L"# parse space, tok is NULL\n", tok);
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
          TS_GET_TOKEN_MSG((wchar_t *) L"# parse space, tok is empty\n", tok);
          free((void *) tok);
          return NULL;

        } else {
          ungetwc((wint_t) c, stream);
          TS_GET_TOKEN_MSG((wchar_t *) L"# parse space, tok exists and isn't empty, token: '%ls'\n", tok);
          return tok;
        }
      }
      default: {
        if (TS_valid_char_for_token(prev)) {
          TS_GET_TOKEN_MSG(
              (wchar_t *) L"-- parse default, prev was '%lc' and is valid for token, token is: '%ls'\n", prev, tok
          );
          const u_long size = tok ? wcslen((const wchar_t *) tok) : 0;
          volatile wchar_t *newPointerForDefault = (wchar_t *) calloc(sizeof(wchar_t), size + 1 + TS_STRING_END);
          if (tok != NULL) {
            wcscat((wchar_t *) newPointerForDefault, (const wchar_t *) tok);
          }
          if (tok != NULL) {
            free((void *) tok);
          }
          tok = newPointerForDefault;
          tok[size] = c;

        } else {
          ungetwc((wint_t) c, stream);
          TS_GET_TOKEN_MSG(
              (wchar_t *) L"# previous character isn't valid for token, putting current char back and returning token '%ls'\n",
              tok);
          return tok;
        }
      }
    }
  }
  if (tok != NULL && tok[0] == -1) {
    free((void *) tok);
    return NULL;
  }
  TS_GET_TOKEN_MSG((wchar_t *) L"# token: '%ls' [done return]\n", tok);
  return tok;
}

TSFile *
TS_parse_file(const char *fileName) {
  FILE *stream = fopen(fileName, "r");

  if (stream == NULL) {
    fprintf(stderr, "Could not open file '%s' to read!\n", fileName);
    fprintf(stderr, "  failure message: '%s'\n", strerror(errno));
  }

  return TS_parse_stream(fileName, stream);
}

TSFile *
TS_parse_stream(
    const char *file,
    FILE *stream
) {
  TSFile *tsFile = TS_find_file(file, stream);

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

  TSParseData data;
  data.line = 0;
  data.character = 0;
  data.position = 0;
  data.stream = stream;
  data.parentTSToken = NULL;

  if (tsFile->sanity != TS_FILE_VALID)
    return tsFile;

  const wchar_t *tok = NULL;
  while (1) {
    TS_LOOP_SANITY_CHECK(tsFile);

    tok = (const wchar_t *) TS_getToken(stream);
    if (tok == NULL) break;

    data.token = tok;
    if (data.token[0] == '\n') {
      TS_next_line(&data);
    } else {
      TSParserToken *token = TS_parse_ts_token(tsFile, &data);
      if (token->tokenType != TS_UNKNOWN) {
        TS_append_ts_parser_token(tsFile, token);
      } else {
        TS_free_tsToken(token);
      }
    }

    free((void *) tok);
  }

  if (tsFile->stream) {
    fclose(tsFile->stream);
    tsFile->stream = NULL;
  }

  return tsFile;
}

void
TS_free_unknown(
    const TSParserToken *token
) {
  TS_free_children(token);

  if (token->data) free(token->data);
  free((void *) token);
}

void
TS_free_tsToken(
    const TSParserToken *token
) {
  switch (token->tokenType) {
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
    case TS_FUNCTION_RETURN_TYPE:
      TS_free_function_return_type(token);
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
    case TS_ARGUMENT:
      TS_free_argument(token);
      break;
    case TS_CONDITION:
      TS_free_condition(token);
      break;
    case TS_UNKNOWN:
      TS_free_unknown(token);
      break;
    case TS_CALLER:
      TS_free_caller(token);
      break;
    case TS_SWITCH:
      TS_free_switch(token);
      break;
    case TS_CASE:
      TS_free_case(token);
      break;
    case TS_BREAK:
      TS_free_break(token);
      break;
    case TS_FOR:
    case TS_FOR_WITH_CONDITION:
    case TS_FOR_IN:
    case TS_FOR_OF:
      TS_free_for(token);
      break;
    case TS_OF:
      TS_free_of(token);
      break;
    case TS_IN:
      TS_free_in(token);
      break;
    case TS_JSON: {
      TS_free_json(token);
      break;
    }
    case TS_ARRAY:
      TS_free_array(token);
      break;
    case TS_LOOP_VARIABLES_SECTION: {
      TS_free_unknown(token);
      break;
    }
    case TS_LOOP_CONDITION_SECTION: {
      TS_free_unknown(token);
      break;
    }
    case TS_LOOP_CHANGE_SECTION: {
      TS_free_unknown(token);
      break;
    }
    case TS_STRING_TEMPLATE:
    case TS_STRING: {
      TS_free_string(token);
      break;
    }
    case TS_CALL_ARGUMENTS: {
      TS_free_call_arguments(token);
      break;
    }
    case TS_EXPORT:
      TS_free_export(token);
      break;
    case TS_IMPORT:
      TS_free_import(token);
      break;
    case TS_IMPORT_FROM: {
      TS_free_import_from(token);
      break;
    }
    case TS_IMPORTED_TOKENS: {
      TS_free_imported_tokens(token);
      break;
    }
    case TS_INTERFACE: {
      TS_free_interface(token);
      break;
    }
  }
}

void
TS_free_children(
    const TSParserToken *token
) {
  TS_free_children_from(token, 0);
}

void
TS_free_children_from(
    const TSParserToken *token,
    u_long childIndex
) {
  TSParserToken *child = NULL;
  TSParserToken **children = token->children;
  for (; childIndex < token->childrenSize; childIndex++) {
    child = children[0];
    if (!TS_is_predefined(child)) {
      TS_free_tsToken(child);
    }
    children += 1;
  }
  if (token->children != NULL) free(token->children);
}

void
TS_free_tsFile(
    TSFile *tsFile
) {

  for (; tsFile->tokensSize;) {
    tsFile->tokensSize -= 1;
    TS_free_tsToken(tsFile->tokens[tsFile->tokensSize]);
  }
  if (tsFile->tokens != NULL) free(tsFile->tokens);
  if (tsFile->file) free(tsFile->file);
  if (tsFile->errorReason) free(tsFile->errorReason);

  TS_register_remove_file(tsFile);

  free((void *) tsFile);
}
