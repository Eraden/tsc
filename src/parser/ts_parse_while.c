#include <cts/register.h>

static void
TS_parse_while_conditions(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  wchar_t *tok = NULL;
  unsigned char proceed = TRUE;

  while (proceed) {
    tok = (wchar_t *) TS_getToken(tsFile->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, tsParseData->parentTSToken, "while conditions");
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
      case L'(': {
        tsParseData->token = tok;
        TSParserToken *condition = TS_parse_condition(tsFile, tsParseData);
        if (condition) {
          TS_push_child(tsParseData->parentTSToken, condition);
        } else {
          // todo error
        }
        proceed = FALSE;
        free((void *) tok);
        break;
      }
      default: {
        TS_UNEXPECTED_TOKEN(tsFile, tsParseData->parentTSToken, tok, "while conditions");
        break;
      }
    }
  }
}

static void
TS_parse_while_scope(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  wchar_t *tok = NULL;
  unsigned char proceed = TRUE;

  while (proceed) {
    tok = (wchar_t *) TS_getToken(tsFile->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, tsParseData->parentTSToken, "while body");
      break;
    }

    switch (tok[0]) {
      case L' ': {
        TS_MOVE_BY(tsParseData, tok);
        break;
      }
      case L'\n': {
        TS_NEW_LINE(tsParseData, tok);
        break;
      }
      case L'{': {
        tsParseData->token = tok;
        TSParserToken *condition = TS_parse_scope(tsFile, tsParseData);
        if (condition) {
          TS_push_child(tsParseData->parentTSToken, condition);
        } else {
          // todo error
        }
        proceed = FALSE;
        break;
      }
      default: {
        TS_UNEXPECTED_TOKEN(tsFile, tsParseData->parentTSToken, tok, "while body");
        break;
      }
    }
    free((void *) tok);
  }
}

TSParserToken *
TS_parse_while(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_WHILE, tsParseData)

    TS_parse_while_conditions(tsFile, tsParseData);
    if (token->parent == NULL || token->parent->tokenType != TS_DO) {
      TS_parse_while_scope(tsFile, tsParseData);
    }

  TS_TOKEN_END(TS_WHILE)
}

void
TS_free_while(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}