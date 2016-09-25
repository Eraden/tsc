#include <cts/parser.h>

TSTokenType
TS_resolve_for_token_type(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_SECTION_BEGIN("for type check");
  volatile wchar_t *buffer = NULL;
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
        volatile wchar_t *newPointer = calloc(sizeof(wchar_t), size + TS_STRING_END);
        if (buffer) {
          wcscpy((wchar_t *) newPointer, (const wchar_t *) buffer);
          free((void *) buffer);
        }
        wcscat((wchar_t *) newPointer, tok);
        buffer = newPointer;
        free((void *) tok);
        break;
      }
      case L'=': {
        size += wcslen(tok);
        wchar_t *newPointer = calloc(sizeof(wchar_t), size + TS_STRING_END);
        if (buffer) {
          wcscpy(newPointer, (const wchar_t *) buffer);
          free((void *) buffer);
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
          wcscpy(newPointer, (const wchar_t *) buffer);
          free((void *) buffer);
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
          wcscpy(newPointer, (const wchar_t *) buffer);
          free((void *) buffer);
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
  if (buffer) {
    TS_put_back(tsFile->stream, buffer);
    free((void *) buffer);
  }
  TS_TOKEN_SECTION_END("for type check");

  return type;
}

static TSParserToken *
__attribute__((__visibility__("hidden")))
TS_parse_for_head(
    TSFile *tsFile,
    TSParseData *tsParseData
)
{
  TSTokenType headType = TS_resolve_for_token_type(tsFile, tsParseData);
  TSParserToken *head = TS_build_parser_token(headType, tsParseData);
  const wchar_t *tok;

  unsigned char proceed = 1;
  while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile);
      tok = (const wchar_t *) TS_getToken(tsFile->stream);
      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, head, "for head");
        break;
      }
      switch (tok[0]) {
        case L'\n': {
          u_long movedBy = wcslen(tok);
          tsParseData->position += movedBy;
          tsParseData->line += 1;
          tsParseData->character = 0;
          free((void *) tok);
          break;
        }
        case L' ': {
          u_long len = wcslen(tok);
          tsParseData->character += len;
          tsParseData->position += len;
          free((void *) tok);
          break;
        }
        case L')': {
          free((void *) tok);
          proceed = 0;
          break;
        }
        default: {
          tsParseData->token = tok;
          TSParserToken *current = TS_parse_ts_token(tsFile, tsParseData);
          free((void *) tok);
          TS_push_child(head, current);
          break;
        }
      }
    }
  tsParseData->parentTSToken = head->parent;
  return head;
}

TSParserToken *
__attribute__((__visibility__("hidden")))
TS_parse_for_body(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  const wchar_t *tok = NULL;
  unsigned char proceed = 1;
  unsigned char buildScope = 1;
  TSParserToken *body = NULL;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile);

    tok = (const wchar_t *) TS_getToken(tsFile->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, tsParseData->parentTSToken, "for body");
      break;
    }
    switch (tok[0]) {
      case L'\n': {
        u_long movedBy = wcslen(tok);
        tsParseData->position += movedBy;
        tsParseData->line += 1;
        tsParseData->character = 0;
        free((void *) tok);
        break;
      }
      case L' ': {
        u_long len = wcslen(tok);
        tsParseData->character += len;
        tsParseData->position += len;
        free((void *) tok);
        break;
      }
      case L';': {
        u_long len = wcslen(tok);
        tsParseData->character += len;
        tsParseData->position += len;
        free((void *) tok);
        proceed = 0;
        buildScope = 0;
        break;
      }
      case L'{': {
        tsParseData->token = TS_clone_string(tok);
        free((void *) tok);
        proceed = 0;
        break;
      }
      default: {
        TS_UNEXPECTED_TOKEN(tsFile, tsParseData->parentTSToken, tok, "for");
        proceed = 0;
        break;
      }
    }
  }

  if (tsFile->sanity != TS_FILE_VALID) buildScope = 0;
  if (buildScope) {
    body = TS_parse_scope(tsFile, tsParseData);
  }

  return body;
}

TSParserToken *
TS_parse_for(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN("for");

  TSParserToken *token = TS_build_parser_token(TS_FOR, tsParseData);

  const wchar_t *tok = NULL;
  unsigned char proceed = 1;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile);
    tok = (const wchar_t *) TS_getToken(tsFile->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "for");
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
        TS_UNEXPECTED_TOKEN(tsFile, token, tok, "for");
        free((void *) tok);
        proceed = 0;
        break;
      }
    }
  }

  TSParserToken *head = TS_parse_for_head(tsFile, tsParseData);
  TS_push_child(token, head);

  TSParserToken *body = TS_parse_for_body(tsFile, tsParseData);
  if (body) TS_push_child(token, body);

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