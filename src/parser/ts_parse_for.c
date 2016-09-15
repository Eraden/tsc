#include <tsc/parser.h>

TSTokenType
TS_resolve_for_token_type(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  wchar_t *buffer = NULL;
  u_long size = 0;
  const wchar_t *tok;
  unsigned char proceed = 1;
  TSParserToken *token = tsParseData->parentTSToken;
  TSTokenType type = TS_FOR;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile);
    tok = (const wchar_t *) TS_getToken(tsFile->stream);

    if (tok == NULL) {
      ts_token_syntax_error(
          (const wchar_t *) L"Unexpected end of stream while parsing for",
          tsFile,
          token
      );
      break;
    }

    switch (tok[0]) {
      case L'\n':
      case L' ': {
        size += wcslen(tok);
        wchar_t *newPointer = calloc(sizeof(wchar_t), size + TS_STRING_END);
        if (buffer) {
          wcscpy(newPointer, buffer);
          free(buffer);
        }
        wcscat(newPointer, tok);
        buffer = newPointer;
        free((void *) tok);
        break;
      }
      case L'=': {
        size += wcslen(tok);
        wchar_t *newPointer = calloc(sizeof(wchar_t), size + TS_STRING_END);
        if (buffer) {
          wcscpy(newPointer, buffer);
          free(buffer);
        }
        wcscat(newPointer, tok);
        buffer = newPointer;
        free((void *) tok);
        type = TS_FOR_LET;
        proceed = 0;
        break;
      }
      case L')': {
        size += wcslen(tok);
        wchar_t *newPointer = calloc(sizeof(wchar_t), size + TS_STRING_END);
        if (buffer) {
          wcscpy(newPointer, buffer);
          free(buffer);
        }
        wcscat(newPointer, tok);
        buffer = newPointer;
        free((void *) tok);
        if (type == TS_FOR) {
          type = TS_FOR_LET;
        }
        proceed = 0;
        break;
      }
      default: {
        size += wcslen(tok);
        wchar_t *newPointer = calloc(sizeof(wchar_t), size + TS_STRING_END);
        if (buffer) {
          wcscpy(newPointer, buffer);
          free(buffer);
        }
        wcscat(newPointer, tok);
        buffer = newPointer;
        if (wcscmp(tok, (const wchar_t *) L"of") == 0) {
          type = TS_FOR_OF;
          proceed = 0;
        } else if (wcscmp(tok, (const wchar_t *) L"in") == 0) {
          type = TS_FOR_IN;
          proceed = 0;
        }
        free((void *) tok);
        break;
      }
    }
  }
  TS_put_back(tsFile->stream, buffer);

  // resolve for type
  // if
  return type;
}

TSParserToken *
TS_parse_for(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("for");

  TSParserToken *token = TS_build_parser_token(TS_FOR, tsParseData);

  const wchar_t *tok;
  unsigned char proceed = 1;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile);
    tok = (const wchar_t *) TS_getToken(tsFile->stream);

    if (tok == NULL) {
      ts_token_syntax_error(
          (const wchar_t *) L"Unexpected end of stream while parsing for",
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
        tsParseData->line += 1;
        tsParseData->character = 0;
        break;
      }
      case L'(': {
        u_long movedBy = wcslen(tok);
        tsParseData->position += movedBy;
        tsParseData->character += movedBy;
        free((void *) tok);
        proceed = 0;
        break;
      }
      default: {
        ts_token_syntax_error(
            (const wchar_t *) L"Unexpected token while parsing for",
            tsFile,
            token
        );
        proceed = 0;
        break;
      }
    }
  }
  token->tokenType = TS_resolve_for_token_type(tsFile, tsParseData);

  tsParseData->parentTSToken = token->parent;
  TS_TOKEN_END("for");
  return token;
}

void TS_free_for(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}