#include <tsc/parser.h>

const TSParserToken
TS_parse_new(
    TSFile __attribute((__unused__)) *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("new");

  u_long movedBy = wcslen(tsParseData->token);

  TSParserToken token;
  token.tokenType = TS_NEW;
  token.position = tsParseData->position;
  token.character = tsParseData->character;
  token.line = tsParseData->line;
  token.visibility = TS_VISIBILITY_SCOPE;
  token.children = NULL;
  token.childrenSize = 0;
  token.data = NULL;

  volatile unsigned char proceed = 1;
  const wchar_t *tok;
  while (proceed) {
    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      if (token.data == NULL)
        ts_token_syntax_error((wchar_t *) L"Unexpected end of stream while parsing `new` keyword.", tsFile, &token);
      else break;
    }

    switch (tok[0]) {
      case L' ': {
        movedBy += wcslen(tok);
        free((void *) tok);

        break;
      }
      case L'\n': {
        if (token.data == NULL) {
          free((void *) tok);
          ts_token_syntax_error((wchar_t *) L"Expecting class after `new` keyword. Found new line.", tsFile, &token);
        } else {
          proceed = 0;
          TS_put_back(tsParseData->stream, tok);
          free((void *) tok);
        }
        break;
      }
      case L';': {
        if (token.data == NULL) {
          free((void *) tok);
          ts_token_syntax_error((wchar_t *) L"Expecting class after `new` keyword. Found `;`.", tsFile, &token);
        } else {
          proceed = 0;
          TS_put_back(tsParseData->stream, tok);

          free((void *) tok);
        }
        break;
      }
      default: {
        u_long size = wcslen(tok) + TS_STRING_END;
        if (token.data != NULL) size += wcslen(token.data);
        wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), size);
        if (token.data != NULL) wcscpy(newPointer, token.data);
        if (token.data != NULL) free(token.data);
        wcscat(newPointer, tok);
        token.data = newPointer;

        free((void *) tok);
        break;
      }
    }
  }

  tsParseData->position += movedBy;
  tsParseData->character += movedBy;

  TS_TOKEN_END("new");
  return token;
}

void TS_free_new(const TSParserToken token) {
  TS_free_children(token);

  if (token.data) free(token.data);
}
