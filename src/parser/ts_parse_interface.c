#include <cts/register.h>

TSParserToken *
TS_parse_interface(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_INTERFACE, tsParseData)
    if (token->parent != NULL) {
      ts_token_syntax_error((const wchar_t *) L"Interface must be in global scope!", tsFile, token);
    }

    const wchar_t *tok;
    volatile unsigned char proceed;

    proceed = (volatile unsigned char) (tsFile->sanity == TS_FILE_VALID);

    // name and goto {
    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (const wchar_t *) TS_getToken(tsParseData->stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "interface");
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
        case L'{': {
          TS_MOVE_BY(tsParseData, tok);
          if (token->name == NULL) {
            TS_MISSING_NAME(tsFile, token, L"interface");
          }
          proceed = FALSE;
          free((void *) tok);
          break;
        }
        default: {
          if (wcscmp(tok, (const wchar_t *) L"extends") == 0) {
            if (token->name == NULL) {
              TS_MISSING_NAME(tsFile, token, L"interface");
            } else {
              tsParseData->token = tok;
              TS_parse_extends(tsFile, tsParseData);
            }
          } else if (TS_is_keyword(tok)) {
            TS_UNEXPECTED_TOKEN(tsFile, token, tok, "interface");
          } else if (token->name == NULL && TS_name_is_valid(tok) == FALSE) {
            ts_token_syntax_error((const wchar_t *) L"Not suitable interface name", tsFile, token);
          } else if (token->name == NULL) {
            TS_MOVE_BY(tsParseData, tok);
            token->name = TS_clone_string(tok);
          }
          free((void *) tok);
          break;
        }
      }
    }
    // body
    proceed = (volatile unsigned char) (tsFile->sanity == TS_FILE_VALID);
    TSParserToken *child = NULL;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (const wchar_t *) TS_getToken(tsParseData->stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "interface");
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
        case L'}': {
          TS_MOVE_BY(tsParseData, tok);
          proceed = FALSE;
          free((void *) tok);
          break;
        }
        case L';': {
          TS_MOVE_BY(tsParseData, tok);
          child = NULL;
          free((void *) tok);
          break;
        }
        case L'(': {
          TS_MOVE_BY(tsParseData, tok);
          tsParseData->token = tok;
          child = TS_parse_call_arguments(tsFile, tsParseData);
          tsParseData->parentTSToken = child->parent;
          free((void *) tok);
          break;
        }
        default: {
          free((void *) tok);
          break;
        }
      }
    }
  TS_TOKEN_END(TS_INTERFACE)
}

void
TS_free_interface(
    const TSParserToken *token
) {
  TS_free_children(token);
  if (token->name) free(token->name);
  free((void *) token);
}
