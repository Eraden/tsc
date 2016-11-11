#include <cts/register.h>

TSParserToken *
TS_parse_call_arguments(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_CALL_ARGUMENTS, tsParseData)
    volatile unsigned char proceed = TRUE;
    const wchar_t *tok = NULL;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (const wchar_t *) TS_getToken(tsParseData->stream);

      if (tok == NULL) {
        if (token->data == NULL) {
          ts_token_syntax_error((wchar_t *) L"Unexpected end of stream while parsing `new` keyword.", tsFile, token);
          break;
        } else break;
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
          proceed = FALSE;
          break;
        }
        case L')': {
          TS_MOVE_BY(tsParseData, tok);
          free((void *) tok);
          proceed = FALSE;
          break;
        }
        default: {
          TS_put_back(tsFile->stream, tok);
          tsParseData->token = tok;
          TSParserToken *child = TS_parse_argument(tsFile, tsParseData);
          if (child == NULL) {
            ts_token_syntax_error((const wchar_t *) L"Expecting call argument but nothing was found", tsFile, token);
          } else if (child->tokenType != TS_ARGUMENT) {
            TS_UNEXPECTED_TOKEN(tsFile, child, tok, "call arguments");
            TS_free_tsToken(child);
          } else {
            TS_push_child(token, child);
          }
          free((void *) tok);
          break;
        }
      }
    }
  TS_TOKEN_END(TS_CALL_ARGUMENTS)
}

void
TS_free_call_arguments(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}