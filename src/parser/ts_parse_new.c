#include <cts/parser.h>
#include <cts/register.h>

TSParserToken *
TS_parse_new(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_NEW, tsParseData)

    volatile unsigned char proceed = TRUE;
    const wchar_t *tok = NULL;
    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (const wchar_t *) TS_get_token(tsParseData->stream);

      if (tok == NULL) {
        if (token->data == NULL) {
          TS_token_syntax_error((wchar_t *) L"Unexpected end of stream while parsing `new` keyword.", tsFile, token);
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
          if (token->childrenSize == 0) {
            free((void *) tok);
            TS_token_syntax_error((wchar_t *) L"Expecting class after `new` keyword. Found new line.", tsFile, token);
            proceed = FALSE;
            break;
          } else {
            proceed = FALSE;
            TS_put_back(tsParseData->stream, tok);
            free((void *) tok);
          }
          break;
        }
        case L';': {
          if (token->childrenSize == 0) {
            free((void *) tok);
            TS_token_syntax_error((wchar_t *) L"Expecting class after `new` keyword. Found `;`.", tsFile, token);
            proceed = FALSE;
            break;
          } else {
            proceed = FALSE;
            TS_put_back(tsParseData->stream, tok);

            free((void *) tok);
          }
          break;
        }
        case L'(': {
          tsParseData->token = tok;
          TSParserToken *arguments = TS_parse_call_arguments(tsFile, tsParseData);
          if (arguments) {
            TS_push_child(token, arguments);
          } else {
            TS_token_syntax_error((const wchar_t *) L"Expecting call arguments but nothing was found", tsFile, token);
          }
          free((void *) tok);
          break;
        }
        case L')': {
          TS_put_back(tsFile->stream, tok);
          proceed = FALSE;
          free((void *) tok);
          break;
        }
        default: {
          TSParserToken *classToken = TS_find_type(tsFile->file, tok);
          if (classToken) {
            TS_push_child(token, TS_create_borrow(classToken, tsParseData));
          } else {
            TS_token_syntax_error((const wchar_t *) L"Instantiation of unknown class", tsFile, token);
          }

          free((void *) tok);
          break;
        }
      }
    }

  TS_TOKEN_END(TS_NEW)
}

void
TS_free_new(
    const TSParserToken *token
) {
  TS_free_children(token);

  if (token->data) free(token->data);
  free((void *) token);
}
