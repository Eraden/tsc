#include <cts/parser.h>

TSParserToken *
TS_parse_json_entry(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_JSON_ENTRY, tsFile)

    unsigned char proceed = TRUE;
    wchar_t *tok = NULL;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (wchar_t *) TS_get_token(tsFile->input.stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "json");
        break;
      }

      switch (tok[0]) {
        case L' ': {
          TS_MOVE_BY(tsFile, tok);
          free((void *) tok);
          break;
        }
        case L'\n': {
          TS_NEW_LINE(tsFile, tok);
          free((void *) tok);
          break;
        }
        case L',': {
          if (token->childrenSize == 2) {
            proceed = FALSE;
          } else {
            TS_UNEXPECTED_TOKEN(tsFile, token, tok, "json entry");
          }
          tsFile->parse.parentTSToken = token;
          free(tok);
          break;
        }
        case L':': {
          TS_MOVE_BY(tsFile, tok);
          free((void *) tok);
          break;
        }
        case L'}': {
          TS_MOVE_BY(tsFile, tok);
          TS_put_back(tsFile->input.stream, tok);
          free((void *) tok);
          proceed = FALSE;
          break;
        }
        default: {
          switch (token->childrenSize) {
            case 0: {
              tsFile->parse.token = tok;
              TSParserToken *key = TS_parse_ts_token(tsFile);
              if (key && key->tokenType == TS_STRING) {
                TS_push_child(token, key);
              } else if (key) {
                TS_UNEXPECTED_TOKEN(tsFile, key, tok, "json entry key");
                TS_free_ts_token(key);
              } else {
                TS_token_syntax_error((const wchar_t *) L"Parse json key failed. No token returned!", tsFile, token);
              }
              break;
            }
            case 1: {
              tsFile->parse.token = tok;
              TSParserToken *value = TS_parse_ts_token(tsFile);
              TS_push_child(token, value);
              break;
            }
            default: {
              TS_UNEXPECTED_TOKEN(tsFile, token, tok, "json entry");
              break;
            }
          }
          TS_MOVE_BY(tsFile, tok);
          free((void *) tok);
          break;
        }
      }
    }

  TS_TOKEN_END(TS_JSON_ENTRY)
}

TSParserToken *
TS_parse_json(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_JSON, tsFile)

    unsigned char proceed = TRUE;
    wchar_t *tok = NULL;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (wchar_t *) TS_get_token(tsFile->input.stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "json");
        break;
      }

      switch (tok[0]) {
        case L' ': {
          TS_MOVE_BY(tsFile, tok);
          free((void *) tok);
          break;
        }
        case L'\n': {
          TS_NEW_LINE(tsFile, tok);
          free((void *) tok);
          break;
        }
        case L';': {
          TS_MOVE_BY(tsFile, tok);
          TS_put_back(tsFile->input.stream, tok);
          free((void *) tok);
          proceed = FALSE;
          break;
        }
        case L'}': {
          TS_MOVE_BY(tsFile, tok);
          free((void *) tok);
          proceed = FALSE;
          break;
        }
        default: {
          TS_put_back(tsFile->input.stream, tok);
          tsFile->parse.token = tok;
          TSParserToken *child = TS_parse_json_entry(tsFile);
          if (child) {
            TS_push_child(token, child);
          } else {
            TS_UNEXPECTED_TOKEN(tsFile, child, tok, "json key");
          }
          TS_MOVE_BY(tsFile, tok);
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
