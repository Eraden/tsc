#include <cts/parser.h>

TSParserToken *
TS_parse_caller(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_CALLER, tsParseData)

    token->modifiers = TS_MODIFIER_SCOPE;
    token->name = (void *) TS_clone_string(tsParseData->token);

    volatile unsigned char proceed = TRUE;
    const wchar_t *tok = NULL;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (const wchar_t *) TS_get_token(tsParseData->stream);

      if (tok == NULL) {
        if (token->data == NULL) {
          TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "caller arguments");
          break;

        } else {
          break;
        }
      }

      switch (tok[0]) {
        case L' ': {
          TS_MOVE_BY(tsParseData, tok);
          free((void *) tok);

          break;
        }
        case L'\n': {
          TS_put_back(tsParseData->stream, tok);
          free((void *) tok);
          break;
        }
        case L';': {
          TS_put_back(tsParseData->stream, tok);
          free((void *) tok);
          proceed = FALSE;
          break;
        }
        case L',':
        case L'(': {
          TS_MOVE_BY(tsParseData, tok);
          tsParseData->token = tok;
          TSParserToken *callArguments = TS_parse_call_arguments(tsFile, tsParseData);
          TS_push_child(token, callArguments);
          free((void *) tok);
          break;
        }
        case L')': {
          TS_MOVE_BY(tsParseData, tok);
          proceed = FALSE;
          free((void *) tok);
          break;
        }
        default: {
          TS_UNEXPECTED_TOKEN(tsFile, token, tok, "caller");
          free((void *) tok);
          break;
        }
      }
    }

  TS_TOKEN_END(TS_CALLER)
}

void
TS_free_caller(
    const TSParserToken *token
) {
  TS_free_children(token);
  if (token->data) free(token->data);
  free((void *) token);
}
