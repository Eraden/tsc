#include <cts/register.h>

static unsigned char
__attribute__((visibility("hidden")))
TS_parse_argument_done(
    TSFile *tsFile,
    TSParseData *tsParseData,
    TSVariableParseFlag parseFlag
) {
  TSParserToken *token = tsParseData->parentTSToken;
  switch (parseFlag) {
    case TS_PARSE_VARIABLE_NAME: {
      ts_token_syntax_error((wchar_t *) L"Missing argument name", tsFile, token);
      break;
    }
    case TS_PARSE_VARIABLE_TYPE: {
      ts_token_syntax_error((wchar_t *) L"Expect argument type but none provided", tsFile, token);
      break;
    }
    case TS_PARSE_VARIABLE_VALUE: {
      ts_token_syntax_error((wchar_t *) L"Expect argument default value but none provided", tsFile, token);
      break;
    }
    case TS_PARSE_VARIABLE_NONE: {
    }
    default: {
      return TRUE;
    }
  }
  return FALSE;
}

TSParserToken *
TS_parse_argument(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_ARGUMENT, tsParseData)

    const wchar_t *tok = NULL;

    token->name = NULL;
    token->children = calloc(sizeof(TSParserToken *), 2);
    token->childrenSize = 2;
    TSParserToken *value = NULL;
    TSParserToken *type = NULL;
    token->children[TS_VARIABLE_TYPE] = TS_ANY_TYPE;
    token->children[TS_VARIABLE_VALUE] = TS_UNDEFINED_TYPE;

    volatile unsigned char proceed = TRUE;
    TSVariableParseFlag parseFlag = TS_PARSE_VARIABLE_NAME;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (const wchar_t *) TS_getToken(tsParseData->stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "argument");
        break;
      }

      switch (tok[0]) {
        case L' ': {
          TS_MOVE_BY(tsParseData, tok);
          free((void *) tok);
          break;
        }
        case L'\n': {
          TS_NEW_LINE(tsParseData, tok);
          free((void *) tok);
          break;
        }
        case L')': {
          if (TS_parse_argument_done(tsFile, tsParseData, parseFlag)) {
            TS_put_back(tsParseData->stream, tok);
          }
          free((void *) tok);

          proceed = FALSE;
          break;
        }
        case L'=': {
          TS_MOVE_BY(tsParseData, tok);
          free((void *) tok);

          if (token->name == NULL) {
            ts_token_syntax_error((wchar_t *) L"Assigning to argument without name", tsFile, token);
            proceed = FALSE;
          } else if (parseFlag == TS_PARSE_VARIABLE_TYPE) {
            ts_token_syntax_error((const wchar_t *) L"Assigning to argument when expect type", tsFile, token);
            proceed = FALSE;
          } else {
            parseFlag = TS_PARSE_VARIABLE_VALUE;
          }
          break;
        }
        case L',': {
          TS_MOVE_BY(tsParseData, tok);
          free((void *) tok);
          TS_parse_argument_done(tsFile, tsParseData, parseFlag);

          proceed = FALSE;

          break;
        }
        case L':': {
          if (token->name == NULL) {
            ts_token_syntax_error(
                (wchar_t *) L"Unexpected argument type definition. Argument has no name",
                tsFile, token
            );
            proceed = FALSE;

          } else if (parseFlag >= TS_PARSE_VARIABLE_TYPE) {
            ts_token_syntax_error(
                (wchar_t *) L"Unexpected argument type definition. Type was already declared",
                tsFile, token
            );
            proceed = FALSE;
          }
          TS_MOVE_BY(tsParseData, tok);
          parseFlag = TS_PARSE_VARIABLE_TYPE;
          free((void *) tok);

          break;
        }
        default: {
          if (wcscmp(tok, (wchar_t *) L"private") == 0) {
            token->modifiers = TS_MODIFIER_PRIVATE;

          } else if (wcscmp(tok, (wchar_t *) L"protected") == 0) {
            token->modifiers = TS_MODIFIER_PROTECTED;

          } else if (wcscmp(tok, (wchar_t *) L"public") == 0) {
            token->modifiers = TS_MODIFIER_PUBLIC;

          } else if (parseFlag == TS_PARSE_VARIABLE_NAME) {
            wchar_t *newPointer = TS_join_strings(token->name, tok);
            if (token->name) free((void *) token->name);
            token->name = newPointer;
            TS_MOVE_BY(tsParseData, tok);

            parseFlag = TS_PARSE_VARIABLE_NONE;

          } else if (parseFlag == TS_PARSE_VARIABLE_VALUE) {
            tsParseData->token = tok;
            value = TS_find_type(tsFile->file, tok);
            if (!value) {
              value = TS_parse_ts_token(tsFile, tsParseData);
              tsParseData->parentTSToken = value->parent;
            }
            token->children[TS_VARIABLE_VALUE] = value;
            parseFlag = TS_PARSE_VARIABLE_NONE;

          } else if (parseFlag == TS_PARSE_VARIABLE_TYPE) {
            TS_MOVE_BY(tsParseData, tok);
            if (tok[0] != L'(') {
              type = TS_find_type(tsFile->file, tok);
              if (type) {
                token->children[TS_VARIABLE_TYPE] = type;
              } else {
                TS_UNKNOWN_TYPE(tsFile, token, tok);
              }
            } else {
              // TODO fat arrow
            }
            parseFlag = TS_PARSE_VARIABLE_NONE;
          } else {
            TS_MOVE_BY(tsParseData, tok);
          }

          free((void *) tok);
          break;
        }
      }
    }

  TS_TOKEN_END(TS_ARGUMENT)
}

void TS_free_argument(const TSParserToken *token) {
  TS_free_children_from(token, 1);

  if (token->name) free((void *) token->name);
  free((void *) token);
}