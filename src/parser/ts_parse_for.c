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
  volatile unsigned char proceed = TRUE;
  TSParserToken *token = tsParseData->parentTSToken;
  TSTokenType type = TS_FOR;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile);
    tok = (const wchar_t *) TS_getToken(tsFile->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "for");
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
        proceed = FALSE;
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
        proceed = FALSE;
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
          proceed = FALSE;
        } else if (wcscmp(tok, (const wchar_t *) L"in") == 0) {
          type = TS_FOR_IN;
          proceed = FALSE;
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

static void
__attribute__((__visibility__("hidden")))
TS_parse_for_head_for_of(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TSParserToken *head = tsParseData->parentTSToken;
  const wchar_t *tok;
  TSForParseFlag flag = TS_PARSE_FOR_OF_KEYWORD_LET;

  volatile unsigned char proceed = TRUE;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile);
    tok = (const wchar_t *) TS_getToken(tsFile->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, head, "for of head");
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
      case L')': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        proceed = FALSE;
        break;
      }
      default: {
        tsParseData->token = tok;
        TSParserToken *current = TS_parse_ts_token(tsFile, tsParseData);
        free((void *) tok);
        switch (flag) {
          case TS_PARSE_FOR_OF_KEYWORD_LET: {
            if (current->tokenType == TS_LET) {
              TS_push_child(head, current);
              flag = TS_PARSE_FOR_OF_KEYWORD_OF;
            } else {
              TS_free_tsToken(current);
              TS_UNEXPECTED_TOKEN(tsFile, current, tok, "for of variable name");
            }
            break;
          }
          case TS_PARSE_FOR_OF_KEYWORD_OF: {
            if (current->tokenType == TS_OF) {
              TS_push_child(head, current);
              flag = TS_PARSE_FOR_OF_COLLECTION;
            } else {
              TS_free_tsToken(current);
              TS_UNEXPECTED_TOKEN(tsFile, current, tok, "for of, expecting to get `of`");
            }
            break;
          }
          case TS_PARSE_FOR_OF_COLLECTION: {
            if (TS_is_keyword(tok) && current->tokenType != TS_ARRAY) {
              TS_free_tsToken(current);
              ts_token_syntax_error(
                  (const wchar_t *) L"Unexpected keyword while parsing `for of` collection",
                  tsFile, current
              );
            }
            else if (current->tokenType == TS_UNKNOWN || current->tokenType == TS_ARRAY) {
              TS_push_child(head, current);
            } else {
              TS_free_tsToken(current);
              TS_UNEXPECTED_TOKEN(tsFile, current, tok, "for of, expecting to collection`");
            }
            break;
          }
        }
        break;
      }
    }
  }
  tsParseData->parentTSToken = head->parent;
}

static void
__attribute__((__visibility__("hidden")))
TS_parse_for_head_for_in(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TSParserToken *head = tsParseData->parentTSToken;
  const wchar_t *tok;
  TSForParseFlag flag = TS_PARSE_FOR_IN_KEYWORD_LET;

  volatile unsigned char proceed = TRUE;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile);
    tok = (const wchar_t *) TS_getToken(tsFile->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, head, "for in head");
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
      case L')': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        proceed = FALSE;
        break;
      }
      default: {
        tsParseData->token = tok;
        TSParserToken *current = TS_parse_ts_token(tsFile, tsParseData);
        free((void *) tok);
        switch (flag) {
          case TS_PARSE_FOR_IN_KEYWORD_LET: {
            if (current->tokenType == TS_LET) {
              TS_push_child(head, current);
              flag = TS_PARSE_FOR_IN_KEYWORD_IN;
            } else {
              TS_free_tsToken(current);
              TS_UNEXPECTED_TOKEN(tsFile, current, tok, "for in variable name");
            }
            break;
          }
          case TS_PARSE_FOR_IN_KEYWORD_IN: {
            if (current->tokenType == TS_IN) {
              TS_push_child(head, current);
              flag = TS_PARSE_FOR_IN_COLLECTION;
            } else {
              TS_free_tsToken(current);
              TS_UNEXPECTED_TOKEN(tsFile, current, tok, "for in, expecting to get `in`");
            }
            break;
          }
          case TS_PARSE_FOR_IN_COLLECTION: {
            if (TS_is_keyword(tok)) {
              TS_free_tsToken(current);
              ts_token_syntax_error(
                  (const wchar_t *) L"Unexpected keyword while parsing `for in` collection",
                  tsFile, current
              );
            }
            else if (current->tokenType == TS_UNKNOWN) {
              TS_push_child(head, current);
            } else {
              TS_free_tsToken(current);
              TS_UNEXPECTED_TOKEN(tsFile, current, tok, "for in, expecting to get collection");
            }
            break;
          }
        }
        break;
      }
    }
  }
  tsParseData->parentTSToken = head->parent;
}

static void
__attribute__((__visibility__("hidden")))
TS_parse_for_head_for_let(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TSParserToken *head = tsParseData->parentTSToken;
  const wchar_t *tok;

  volatile unsigned char proceed = TRUE;
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
        proceed = FALSE;
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
}

static TSParserToken *
__attribute__((__visibility__("hidden")))
TS_parse_for_head(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TSTokenType headType = TS_resolve_for_token_type(tsFile, tsParseData);
  TSParserToken *head = TS_build_parser_token(headType, tsParseData);

  switch (head->tokenType) {
    case TS_FOR_LET: {
      TS_parse_for_head_for_let(tsFile, tsParseData);
      break;
    }
    case TS_FOR_IN: {
      TS_parse_for_head_for_in(tsFile, tsParseData);
      break;
    }
    case TS_FOR_OF: {
      TS_parse_for_head_for_of(tsFile, tsParseData);
      break;
    }
    default: {
      ts_token_syntax_error((const wchar_t *) L"Invalid for head", tsFile, head);
    }
  }
  return head;
}

TSParserToken *
__attribute__((__visibility__("hidden")))
TS_parse_for_body(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  const wchar_t *tok = NULL;
  volatile unsigned char proceed = TRUE;
  unsigned char buildScope = TRUE;
  TSParserToken *body = NULL;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile);

    tok = (const wchar_t *) TS_getToken(tsFile->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, tsParseData->parentTSToken, "for body");
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
        buildScope = FALSE;
        break;
      }
      case L'{': {
        TS_MOVE_BY(tsParseData, tok);
        tsParseData->token = TS_clone_string(tok);
        free((void *) tok);
        proceed = FALSE;
        break;
      }
      default: {
        TS_UNEXPECTED_TOKEN(tsFile, tsParseData->parentTSToken, tok, "for");
        proceed = FALSE;
        break;
      }
    }
  }

  if (tsFile->sanity != TS_FILE_VALID) buildScope = FALSE;
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
  volatile unsigned char proceed = TRUE;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile);
    tok = (const wchar_t *) TS_getToken(tsFile->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "for");
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
        proceed = FALSE;
        break;
      }
      default: {
        TS_UNEXPECTED_TOKEN(tsFile, token, tok, "for");
        free((void *) tok);
        proceed = FALSE;
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