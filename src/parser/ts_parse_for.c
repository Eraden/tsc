#include <cts/parser.h>

TSTokenType
TS_resolve_for_token_type(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_SECTION_BEGIN("for type check");
  volatile wchar_t *buffer = NULL;
  volatile u_long size = 0;
  volatile unsigned char proceed = TRUE;
  const wchar_t *tok;
  TSParserToken *token = tsParseData->parentTSToken;
  TSTokenType type = TS_FOR;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile);

    tok = (const wchar_t *) TS_get_token(tsFile->stream);

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
        type = TS_FOR_WITH_CONDITION;
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
          type = TS_FOR_WITH_CONDITION;
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
TS_parse_for_head_for_of(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TSParserToken *head = tsParseData->parentTSToken;
  const wchar_t *tok;
  TSForIterationParseFlag flag = TS_PARSE_FOR_VARIABLE;

  volatile unsigned char proceed = TRUE;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile);

    tok = (const wchar_t *) TS_get_token(tsFile->stream);

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
          case TS_PARSE_FOR_VARIABLE: {
            if (current->tokenType == TS_LET) {
              TS_push_child(head, current);
              flag = TS_PARSE_FOR_ITERATION_KEYWORD;
            } else {
              TS_free_ts_token(current);
              TS_UNEXPECTED_TOKEN(tsFile, current, tok, "for of variable name");
            }
            break;
          }
          case TS_PARSE_FOR_ITERATION_KEYWORD: {
            if (current->tokenType == TS_OF) {
              TS_push_child(head, current);
              flag = TS_PARSE_FOR_COLLECTION;
            } else {
              TS_free_ts_token(current);
              TS_UNEXPECTED_TOKEN(tsFile, current, tok, "for of, expecting to get `of`");
            }
            break;
          }
          case TS_PARSE_FOR_COLLECTION: {
            if (TS_is_keyword(tok) && current->tokenType != TS_ARRAY) {
              TS_free_ts_token(current);
              TS_token_syntax_error(
                  (const wchar_t *) L"Unexpected keyword while parsing `for of` collection",
                  tsFile, current
              );
            } else if (current->tokenType == TS_UNKNOWN || current->tokenType == TS_ARRAY) {
              TS_push_child(head, current);
            } else {
              TS_free_ts_token(current);
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
TS_parse_for_head_for_in(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TSParserToken *head = tsParseData->parentTSToken;
  const wchar_t *tok;
  TSForIterationParseFlag flag = TS_PARSE_FOR_VARIABLE;

  volatile unsigned char proceed = TRUE;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile);

    tok = (const wchar_t *) TS_get_token(tsFile->stream);

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
        TSParserToken *child = TS_parse_ts_token(tsFile, tsParseData);
        free((void *) tok);
        switch (flag) {
          case TS_PARSE_FOR_VARIABLE: {
            if (child->tokenType == TS_LET) {
              TS_push_child(head, child);
              flag = TS_PARSE_FOR_ITERATION_KEYWORD;
            } else {
              TS_free_ts_token(child);
              TS_UNEXPECTED_TOKEN(tsFile, child, tok, "for in variable name");
            }
            break;
          }
          case TS_PARSE_FOR_ITERATION_KEYWORD: {
            if (child->tokenType == TS_IN) {
              TS_push_child(head, child);
              flag = TS_PARSE_FOR_COLLECTION;
            } else {
              TS_free_ts_token(child);
              TS_UNEXPECTED_TOKEN(tsFile, child, tok, "for in, expecting to get `in`");
            }
            break;
          }
          case TS_PARSE_FOR_COLLECTION: {
            if (TS_is_keyword(tok)) {
              TS_free_ts_token(child);
              TS_token_syntax_error((const wchar_t *) L"Unexpected keyword while parsing `for in` collection", tsFile,
                                    child);
            } else if (child->tokenType == TS_UNKNOWN || child->tokenType == TS_COLON) {
              TS_push_child(head, child);
            } else {
              TS_UNEXPECTED_TOKEN(tsFile, child, tok, "for in, expecting to get collection");
              TS_free_ts_token(child);
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
TS_parse_for_head_for_with_condition(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TSParserToken *head = tsParseData->parentTSToken;
  const wchar_t *tok;
  TSForWithConditionParseFlag flag = TS_PARSE_FOR_VARIABLES;
  TSParserToken *current = NULL;
  TSParserToken *variablesSection = NULL, *conditionSection = NULL, *changeSection = NULL;

  variablesSection = TS_build_parser_token(TS_LOOP_VARIABLES_SECTION, tsParseData);
  current = variablesSection;
  TS_push_child(head, current);

  volatile unsigned char proceed = TRUE;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile);

    tok = (const wchar_t *) TS_get_token(tsFile->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, head, "for head");
      break;
    }
    switch (tok[0]) {
      case L'\n': {
        TS_NEW_LINE(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L' ': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        break;
      }
      case L')': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        proceed = FALSE;
        break;
      }
      case L';': {
        TS_MOVE_BY(tsParseData, tok);
        switch (flag) {
          case TS_PARSE_FOR_VARIABLES: {
            flag = TS_PARSE_FOR_CONDITION;
            tsParseData->parentTSToken = variablesSection->parent;
            conditionSection = TS_build_parser_token(TS_LOOP_CONDITION_SECTION, tsParseData);
            current = conditionSection;
            TS_push_child(head, current);
            break;
          }
          case TS_PARSE_FOR_CONDITION: {
            flag = TS_PARSE_FOR_CHANGE;
            tsParseData->parentTSToken = conditionSection->parent;
            changeSection = TS_build_parser_token(TS_LOOP_CHANGE_SECTION, tsParseData);
            current = changeSection;
            TS_push_child(head, current);
            break;
          }
          case TS_PARSE_FOR_CHANGE: {
            TS_token_syntax_error((const wchar_t *) L"Unexpected semicolon while parsing for with condition head",
                                  tsFile, current);
            break;
          }
        }
        free((void *) tok);
        break;
      }
      default: {
        tsParseData->token = tok;
        TSParserToken *child = TS_parse_ts_token(tsFile, tsParseData);
        switch (flag) {
          case TS_PARSE_FOR_VARIABLES: {
            switch (child->tokenType) {
              case TS_LET:
              case TS_VAR: {
                TS_push_child(current, child);
                break;
              }
              case TS_CONST:
              default: {
                TS_free_ts_token(child);
                TS_UNEXPECTED_TOKEN(tsFile, child, tok, "for variables section");
                break;
              }
            }
            break;
          }
          default: {
            TS_push_child(current, child);
            break;
          }
        }
        free((void *) tok);
        break;
      }
    }
  }
  if (flag != TS_PARSE_FOR_CHANGE) {
    TS_token_syntax_error((const wchar_t *) L"Unexpected end of for head", tsFile, head);
  }
  tsParseData->parentTSToken = head->parent;
}

static TSParserToken *
TS_parse_for_head(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TSTokenType headType = TS_resolve_for_token_type(tsFile, tsParseData);
  TSParserToken *head = TS_build_parser_token(headType, tsParseData);

  switch (head->tokenType) {
    case TS_FOR_WITH_CONDITION: {
      TS_parse_for_head_for_with_condition(tsFile, tsParseData);
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
      TS_token_syntax_error((const wchar_t *) L"Invalid for head", tsFile, head);
    }
  }
  return head;
}

TSParserToken *
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

    tok = (const wchar_t *) TS_get_token(tsFile->stream);

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
    tsParseData->token = (const wchar_t *) L"{";
    body = TS_parse_scope(tsFile, tsParseData);
  }

  return body;
}

TSParserToken *
TS_parse_for(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_FOR, tsParseData)

    const wchar_t *tok = NULL;
    volatile unsigned char proceed = TRUE;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile);

      tok = (const wchar_t *) TS_get_token(tsFile->stream);

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

  TS_TOKEN_END(TS_FOR)
}

void TS_free_for(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}