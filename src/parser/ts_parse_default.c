#include <tsc/parser.h>

/**
 * TODO implement
 */
TSParserToken *
TS_parse_default(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("default");
  TSParserToken *token = TS_build_parser_token(TS_DEFAULT, tsParseData);

  const wchar_t *tok = NULL;
  volatile unsigned char proceed = 1;

  while (proceed) {
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
        proceed = 0;
        break;
      }
      default: {
        u_long movedBy = wcslen(tok);
        tsParseData->position += movedBy;
        tsParseData->character += movedBy;
        free((void *) tok);

        ts_token_syntax_error(
            (const wchar_t *) L"Unexpected token while parsing default. Default cannot ave any condition!",
            tsFile,
            token
        );
        proceed = 0;
        break;
      }
    }
  }

  proceed = 1;
  while (proceed) {
    tok = (const wchar_t *) TS_getToken(tsFile->stream);

    if (tok == NULL) {
      ts_token_syntax_error(
          (const wchar_t *) L"Unexpected end of stream while parsing default",
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
      case L';': {
        u_long movedBy = wcslen(tok);
        tsParseData->position += movedBy;
        tsParseData->character += movedBy;
        free((void *) tok);
        proceed = 0;
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

  tsParseData->parentTSToken = token->parent;
  TS_TOKEN_END("default");
  return token;
}

void
TS_free_default(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}
