#include <cts/parser.h>

TSParserToken *
TS_parse_case(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_CASE, tsParseData)
    const wchar_t *tok = NULL;
    volatile unsigned char proceed = TRUE;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (const wchar_t *) TS_getToken(tsFile->stream);

      if (tok == NULL) {
        ts_token_syntax_error(
            (const wchar_t *) L"Unexpected end of stream while parsing case condition",
            tsFile,
            token
        );
        break;
      }
      switch (tok[0]) {
        case L' ': {
          u_long movedBy = wcslen(tok);
          tsParseData->position += movedBy;
          tsParseData->character += movedBy;
          free((void *) tok);
          break;
        }
        case L'\n': {
          u_long movedBy = wcslen(tok);
          free((void *) tok);
          tsParseData->position += movedBy;
          tsParseData->character = 0;
          tsParseData->line += 1;
          break;
        }
        case L':': {
          u_long movedBy = wcslen(tok);
          free((void *) tok);
          tsParseData->position += movedBy;
          tsParseData->character += movedBy;
          proceed = FALSE;
          break;
        }
        default: {
          u_long movedBy = wcslen(tok);
          tsParseData->position += movedBy;
          tsParseData->character += movedBy;
          tsParseData->token = tok;

          if (token->childrenSize == 1) {
            ts_token_syntax_error(
                (const wchar_t *) L"Unexpected token while parsing case condition. Only one condition is allowed!",
                tsFile,
                token
            );
            proceed = FALSE;
            break;
          } else {
            TS_put_back(tsFile->stream, tok);

            TSParserToken *child = TS_parse_condition(tsFile, tsParseData);
            if (child) {
              TS_push_child(token, child);
            }
          }
          free((void *) tok);
          break;
        }
      }
    }

    proceed = TRUE;
    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (const wchar_t *) TS_getToken(tsFile->stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "case body");
        break;
      }

      switch (tok[0]) {
        case L' ': {
          u_long movedBy = wcslen(tok);
          tsParseData->position += movedBy;
          tsParseData->character += movedBy;
          free((void *) tok);
          break;
        }
        case L'\n': {
          u_long movedBy = wcslen(tok);
          free((void *) tok);
          tsParseData->position += movedBy;
          tsParseData->character = 0;
          tsParseData->line += 1;
          break;
        }
        case L';': {
          u_long movedBy = wcslen(tok);
          tsParseData->position += movedBy;
          tsParseData->character += movedBy;
          free((void *) tok);
          proceed = FALSE;
          break;
        }
        default: {
          u_long movedBy = wcslen(tok);
          tsParseData->position += movedBy;
          tsParseData->character += movedBy;
          tsParseData->token = tok;
          TS_put_back(tsFile->stream, tok);

          TSParserToken *child = TS_parse_ts_token(tsFile, tsParseData);
          if (child) {
            TS_push_child(token, child);
          }
          free((void *) tok);
          break;
        }
      }
    }

  TS_TOKEN_END(TS_CASE);
}

void
TS_free_case(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}
