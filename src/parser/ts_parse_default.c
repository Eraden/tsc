#include <cts/parser.h>

TSParserToken *
TS_parse_default(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("default");
  TSParserToken *token = TS_build_parser_token(TS_DEFAULT, tsParseData);

  if (token->parent == NULL) {
    ts_token_syntax_error(
        (const wchar_t *) L"Unexpected `default` without `switch` or `export`",
        tsFile,
        token
    );
  }

  const wchar_t *tok = NULL;
  volatile unsigned char proceed = TRUE;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile);

    tok = (const wchar_t *) TS_getToken(tsFile->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "default");
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
      case L':': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        proceed = FALSE;
        break;
      }
      default: {
        TS_MOVE_BY(tsParseData, tok);
        if (token->parent->tokenType == TS_SWITCH) {
          TS_UNEXPECTED_TOKEN(tsFile, token, tok, "switch default. Default cannot have any condition!");
        } else if (token->parent->tokenType == TS_EXPORT) {
          tsParseData->token = tok;
          TSParserToken *child = TS_parse_ts_token(tsFile, tsParseData);
          TS_push_child(token, child);
        }
        free((void *) tok);
        proceed = FALSE;
        break;
      }
    }
  }

  proceed = (volatile unsigned char) (token->parent->tokenType == TS_SWITCH);
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile);

    tok = (const wchar_t *) TS_getToken(tsFile->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "default");
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
        free((void *) tok);
        proceed = FALSE;
        break;
      }
      default: {
        TS_MOVE_BY(tsParseData, tok);
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
