#include <cts/parser.h>

TSParserToken *
TS_parse_json_entry(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_JSON_ENTRY, tsParseData)

    unsigned char proceed = TRUE;
    wchar_t *tok = NULL;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (wchar_t *) TS_getToken(tsFile->stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "json");
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
        case L',': {
          if (token->childrenSize == 2) {
            proceed = FALSE;
          } else {
            TS_UNEXPECTED_TOKEN(tsFile, token, tok, "json entry");
          }
          tsParseData->parentTSToken = token;
          free(tok);
          break;
        }
        case L':': {
          TS_MOVE_BY(tsParseData, tok);
          free((void *) tok);
          break;
        }
        case L'}': {
          TS_MOVE_BY(tsParseData, tok);
          TS_put_back(tsFile->stream, tok);
          free((void *) tok);
          proceed = FALSE;
          break;
        }
        default: {
          switch (token->childrenSize) {
            case 0: {
              tsParseData->token = tok;
              TSParserToken *key = TS_parse_ts_token(tsFile, tsParseData);
              if (key && key->tokenType == TS_STRING) {
                TS_push_child(token, key);
              } else if (key) {
                TS_UNEXPECTED_TOKEN(tsFile, key, tok, "json entry key");
                TS_free_tsToken(key);
              } else {
                ts_token_syntax_error((const wchar_t *) L"Parse json key failed. No token returned!", tsFile, token);
              }
              break;
            }
            case 1: {
              tsParseData->token = tok;
              TSParserToken *value = TS_parse_ts_token(tsFile, tsParseData);
              TS_push_child(token, value);
              break;
            }
            default: {
              TS_UNEXPECTED_TOKEN(tsFile, token, tok, "json entry");
              break;
            }
          }
          TS_MOVE_BY(tsParseData, tok);
          free((void *) tok);
          break;
        }
      }
    }

  TS_TOKEN_END(TS_JSON_ENTRY)
}

TSParserToken *
TS_parse_json(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_JSON, tsParseData)

    unsigned char proceed = TRUE;
    wchar_t *tok = NULL;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (wchar_t *) TS_getToken(tsFile->stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "json");
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
        case L';': {
          TS_MOVE_BY(tsParseData, tok);
          TS_put_back(tsFile->stream, tok);
          free((void *) tok);
          proceed = FALSE;
          break;
        }
        case L'}': {
          TS_MOVE_BY(tsParseData, tok);
          free((void *) tok);
          proceed = FALSE;
          break;
        }
        default: {
          TS_put_back(tsFile->stream, tok);
          tsParseData->token = tok;
          TSParserToken *child = TS_parse_json_entry(tsFile, tsParseData);
          if (child) {
            TS_push_child(token, child);
          } else {
            TS_UNEXPECTED_TOKEN(tsFile, child, tok, "json key");
          }
          TS_MOVE_BY(tsParseData, tok);
          free((void *) tok);
          break;
        }
      }
    }

  TS_TOKEN_END(TS_JSON)
}

void
TS_free_json(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}

void
TS_free_json_entry(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}
