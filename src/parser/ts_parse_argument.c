#include <cts/parser.h>
#include <cts/register.h>

TSParserToken *
TS_parse_argument(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_ARGUMENT, tsParseData)

    const wchar_t *tok;

    unsigned char foundColon = FALSE;
    token->name = NULL;
    TSParserToken *value = NULL;
    TSParserToken *type = TS_find_class(tsFile->file, (const wchar_t *) L"Object");
    TS_push_child(token, type);

    volatile unsigned char proceed = TRUE;
    TSParseArgumentFlag parseFlag = TS_PARSE_ARG_NAME;

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
          if (parseFlag == TS_PARSE_ARG_VALUE && value == NULL) {
            free((void *) tok);
            ts_token_syntax_error((wchar_t *) L"Value for argument is missing", tsFile, token);
            break;
          } else if (parseFlag == TS_PARSE_ARG_TYPE && type == NULL) {
            free((void *) tok);
            ts_token_syntax_error((wchar_t *) L"Type for argument is missing", tsFile, token);
            break;
          } else {
            TS_put_back(tsParseData->stream, tok);
            free((void *) tok);
          }

          proceed = FALSE;
          break;
        }
        case L'=': {
          TS_MOVE_BY(tsParseData, tok);
          free((void *) tok);

          if (token->name == NULL) {
            ts_token_syntax_error((wchar_t *) L"Assigning to argument without name", tsFile, token);
            proceed = FALSE;
          } else {
            parseFlag = TS_PARSE_ARG_VALUE;
          }
          break;
        }
        case L',': {
          TS_MOVE_BY(tsParseData, tok);
          free((void *) tok);

          if (parseFlag == TS_PARSE_ARG_VALUE && value == NULL) {
            ts_token_syntax_error((wchar_t *) L"Value for argument is missing", tsFile, token);
            break;
          } else if (parseFlag == TS_PARSE_ARG_TYPE && type == NULL) {
            ts_token_syntax_error((wchar_t *) L"Value for argument is missing", tsFile, token);
            break;
          } else if (token->name == NULL) {
            ts_token_syntax_error((wchar_t *) L"Declared argument as next but previous has no name", tsFile, token);
            break;
          }

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

          } else if (foundColon == TRUE) {
            ts_token_syntax_error(
                (wchar_t *) L"Unexpected argument type definition. Type was already declared",
                tsFile, token
            );
            proceed = FALSE;
          }
          foundColon = TRUE;
          TS_MOVE_BY(tsParseData, tok);
          parseFlag = TS_PARSE_ARG_TYPE;
          free((void *) tok);

          break;
        }
        default: {
          if (wcscmp(tok, (wchar_t *) L"private") == 0) {
            token->visibility = TS_MODIFIER_PRIVATE;

          } else if (wcscmp(tok, (wchar_t *) L"protected") == 0) {
            token->visibility = TS_MODIFIER_PROTECTED;

          } else if (wcscmp(tok, (wchar_t *) L"public") == 0) {
            token->visibility = TS_MODIFIER_PUBLIC;

          } else if (parseFlag == TS_PARSE_ARG_NAME) {
            wchar_t *newPointer = TS_join_strings(token->name, tok);
            if (token->name) free((void *) token->name);
            token->name = newPointer;
            TS_MOVE_BY(tsParseData, tok);

          } else if (parseFlag == TS_PARSE_ARG_VALUE) {
            tsParseData->token = tok;
            value = TS_parse_ts_token(tsFile, tsParseData);
            if (value) {
              TS_push_child(token, value);
            }

          } else /*if (parseFlag == TS_PARSE_ARG_TYPE)*/ {
            TS_MOVE_BY(tsParseData, tok);
            // Find class if possible
            // TODO () => {}
          }

          free((void *) tok);
          break;
        }
      }
    }

  TS_TOKEN_END(TS_ARGUMENT)
}

void TS_free_argument(const TSParserToken *token) {
  if (token->childrenSize == 2) {
    TS_free_tsToken(token->children[1]);
  }

  if (token->name) free((void *) token->name);
  free((void *) token);
}