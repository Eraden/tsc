#include <cts/parser.h>

static TSParserToken *
TS_parse_switch_body(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_SWITCH_BODY, tsParseData);
  const wchar_t *tok;
  volatile unsigned char proceed = TRUE;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_get_token(tsFile->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "switch body");
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
        free((void *) tok);
        proceed = FALSE;
        break;
      }
      default: {
        tsParseData->token = tok;
        TSParserToken *child = TS_parse_ts_token(tsFile, tsParseData);

        if (child) {
          switch (child->tokenType) {
            case TS_CASE:
            case TS_DEFAULT: {
              TS_push_child(token, child);
              break;
            }
            default: {
              TS_UNEXPECTED_TOKEN(tsFile, child, tok, "switch body");
              TS_free_ts_token(child);
              break;
            }
          }
        }

        free((void *) tok);
        break;
      } // default
    } // switch

  } // while
  TS_TOKEN_END(TS_SWITCH_BODY)
}


TSParserToken *
TS_parse_switch(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_SWITCH, tsParseData)

    unsigned char proceed = TRUE;
    const wchar_t *tok = NULL;

    TSParserToken *condition = NULL, *scope = NULL;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)
      tok = (const wchar_t *) TS_get_token(tsFile->stream);
      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "switch head");
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
          TS_MOVE_BY(tsParseData, tok);
          free((void *) tok);
          condition = TS_parse_condition(tsFile, tsParseData);
          if (condition) {
            TS_push_child(token, condition);
          }
          proceed = FALSE;
          break;
        }
        default: {
          TS_UNEXPECTED_TOKEN(tsFile, token, tok, "switch head before `(`");
          free((void *) tok);
          break;
        }
      }
    }

    proceed = TRUE;
    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)
      tok = (const wchar_t *) TS_get_token(tsFile->stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "switch body");
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
        case L'{': {
          TS_MOVE_BY(tsParseData, tok);
          tsParseData->token = tok;
          scope = TS_parse_switch_body(tsFile, tsParseData);
          if (scope == NULL) {
            // TODO error
          } else {
            TS_push_child(token, scope);
          }
          proceed = FALSE;
          free((void *) tok);
          break;
        }
        default: {
          TS_UNEXPECTED_TOKEN(tsFile, token, tok, "switch body");
          proceed = FALSE;
          free((void *) tok);
          break;
        }
      }

    }

  TS_TOKEN_END(TS_SWITCH)
}

void
TS_free_switch(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}

void
TS_free_switch_conditions(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}

void
TS_free_switch_body(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}