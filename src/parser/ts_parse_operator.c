#include <cts/register.h>

static unsigned char
TS_parse_operator_isPossibleNumber(const wchar_t *tok) {
  if (wcscmp(tok, (const wchar_t *) L"+") == 0) return TRUE;
  if (wcscmp(tok, (const wchar_t *) L"-") == 0) return TRUE;
  return FALSE;
}

static wchar_t *
TS_parse_operator_right_operand(TSFile *tsFile) {
  wchar_t *tok = NULL;
  unsigned char proceed = TRUE;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile);

    tok = (wchar_t *) TS_get_token(tsFile->input.stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, tsFile->parse.parentTSToken, "operator right operand");
      break;
    }

    switch (tok[0]) {
      case L' ': {
        TS_MOVE_BY(tsFile, tok);
        free((void *) tok);
        break;
      }
      case L'\n': {
        TS_NEW_LINE(tsFile, tok);
        free((void *) tok);
        break;
      }
      default: {
        proceed = FALSE;
        if (!TS_is_keyword(tok) && TS_name_is_valid(tok)) {
          proceed = FALSE;
        } else {
          TS_UNEXPECTED_TOKEN(tsFile, tsFile->parse.parentTSToken, tok, "operator right operand");
          free((void *) tok);
          tok = NULL;
        }
        break;
      }
    }
  }
  return tok;
}

static void
TS_parse_operator_resolvePrev(TSFile *tsFile, TSParseOperatorData *data) {
  if (tsFile->sanity != TS_FILE_VALID)
    return;

  if (data->prev) return;

  TSParserToken *unresolved = data->parent->childrenSize > 0 ? data->parent->children[data->lastIndex] : NULL;
  if (!unresolved) {
    data->prev = NULL;
    return;
  }
  if (unresolved->tokenType == TS_UNKNOWN) {
    TSParserToken *resolved = NULL;
    resolved = TS_search_token(unresolved);
    if (resolved == NULL) {
      resolved = TS_find_type(tsFile->input.file, unresolved->name);
    }
    if (resolved != NULL) {
      data->prev = TS_create_borrow(resolved, tsFile);
      TS_free_ts_token(unresolved);
    } else {
      TS_UNEXPECTED_TOKEN(tsFile, unresolved, unresolved->name, "operator left operand");
    }
  } else {
    data->prev = unresolved;
  }
}

static void
TS_parse_operator_resolveNext(TSFile *tsFile, TSParseOperatorData *data) {
  if (tsFile->sanity != TS_FILE_VALID)
    return;

  if (data->next) return;

  wchar_t *tok = TS_parse_operator_right_operand(tsFile);
  if (tok) {
    tsFile->parse.token = tok;
    TSParserToken *unresolved = TS_parse_ts_token(tsFile);
    if (unresolved->tokenType == TS_NUMBER) {
      data->next = unresolved;
    } else {
      TSParserToken *resolved = NULL;
      resolved = TS_search_token(unresolved);
      if (resolved == NULL) {
        resolved = TS_find_type(tsFile->input.file, unresolved->name);
      }
      if (resolved) {
        data->next = TS_create_borrow(resolved, tsFile);
        TS_free_ts_token(unresolved);
      } else {
        TS_UNEXPECTED_TOKEN(tsFile, unresolved, tok, "right operand");
        TS_free_ts_token(unresolved);
        data->done = TRUE;
      }
    }
    free((void *) tok);
  } else {
    TS_UNEXPECTED_END_OF_STREAM(tsFile, data->token, "operator right operand");
    data->done = TRUE;
    data->prev = NULL;
  }
}

// a op b
// a + b;
// a - b;
// a * b;
// a / b;
// a % b;
// a == b;
// a += b;
// a -= b;
// a |= b;
// a &= b;
// a === b;
// a != b;
// a > b;
// a >= b;
// a < b;
// a <= b;
// a && b;
// a || b;
// a & b;
// a | b;
// a ^ b;
static void TS_parse_operator_a_op_b(TSFile *tsFile, TSParseOperatorData *data) {
  if (data->prev == NULL) {
    TS_token_syntax_error((const wchar_t *) L"Missing previous token for operator a op b", tsFile, data->token);
    return;
  }
  TS_parse_operator_resolveNext(tsFile, data);

  if (tsFile->sanity == TS_FILE_VALID) {
    switch (data->prev->tokenType) {
      case TS_GROUP:
      case TS_BORROW:
      case TS_NUMBER:
      case TS_OPERATOR: {
        TS_push_child(data->token, data->prev);
        break;
      }
      default: {
        TS_UNEXPECTED_TOKEN(tsFile, data->prev, data->tok, "left operand");
        break;
      }
    }
  }

  if (tsFile->sanity == TS_FILE_VALID) {
    switch (data->next->tokenType) {
      case TS_GROUP:
      case TS_BORROW:
      case TS_NUMBER:
      case TS_OPERATOR:  {
        data->parent->childrenSize -= 1;
        TS_push_child(data->token, data->next);
        break;
      }
      default: {
        TS_UNEXPECTED_TOKEN(tsFile, data->prev, data->tok, "right operand");
        if (data->next) TS_free_ts_token(data->next);
        break;
      }
    }
  }
}

// op a
// !a;
// +a;
// ++a;
static void TS_parse_operator_op_a(TSFile *tsFile, TSParseOperatorData *data) {
  TS_parse_operator_resolveNext(tsFile, data);

  switch (data->next->tokenType) {
    case TS_GROUP:
    case TS_BORROW:
    case TS_OPERATOR: {
      TS_push_child(data->token, data->next);
      break;
    }
    default: {
      TS_UNEXPECTED_TOKEN(tsFile, data->next, data->tok, "invalid right operand");
      TS_free_ts_token(data->next);
      break;
    }
  }
}

// a op
// a++;
// a--;
static void TS_parse_operator_a_op(TSFile *tsFile, TSParseOperatorData *data) {
  if (data->prev == NULL) {
    TS_token_syntax_error((const wchar_t *) L"Missing previous token for operator a op b", tsFile, data->token);
    return;
  }

  switch (data->prev->tokenType) {
    case TS_GROUP:
    case TS_BORROW:
    case TS_OPERATOR: {
      data->parent->childrenSize -= 1;
      TS_push_child(data->token, data->prev);
      data->done = TRUE;
      break;
    }
    default: {
      break;
    }
  }
}

static const wchar_t *A_OP_B[17] = {
    (const wchar_t *) L"*",
    (const wchar_t *) L"/",
    (const wchar_t *) L"%",
    (const wchar_t *) L"==",
    (const wchar_t *) L"+=",
    (const wchar_t *) L"-=",
    (const wchar_t *) L"===",
    (const wchar_t *) L"!=",
    (const wchar_t *) L">",
    (const wchar_t *) L">=",
    (const wchar_t *) L"<",
    (const wchar_t *) L"<=",
    (const wchar_t *) L"&&",
    (const wchar_t *) L"||",
    (const wchar_t *) L"&",
    (const wchar_t *) L"|",
    (const wchar_t *) L"^"
};

TSParserToken *
TS_parse_operator_advanced(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_OPERATOR, tsFile)
    token->name = TS_clone_string(tsFile->parse.token);

    TSParseOperatorData data;
    data.done = FALSE;
    data.lastIndex = 0;
    data.prev = NULL;
    data.next = NULL;
    data.parent = token->parent;
    data.token = token;
    data.tok = tsFile->parse.token;

    if (data.parent) {
      data.lastIndex = data.parent->childrenSize > 0 ? data.parent->childrenSize - 1 : 0;
      TS_parse_operator_resolvePrev(tsFile, &data);
    }

    for (unsigned int i = 0; i < 17; i++) {
      TS_LOOP_SANITY_CHECK(tsFile);

      if (wcscmp(A_OP_B[i], tsFile->parse.token) == 0) {
        data.done = TRUE;
        TS_parse_operator_a_op_b(tsFile, &data);
        break;
      }
    }

    if (TS_parse_operator_isPossibleNumber(data.token->name)) {
      TS_parse_operator_resolveNext(tsFile, &data);

      if (!data.done && tsFile->sanity == TS_FILE_VALID) {
        switch (data.next->tokenType) {
          case TS_NUMBER: {
            wchar_t *name = TS_join_strings(data.token->name, data.next->content);
            TS_free_ts_token(data.next);
            data.next = NULL;
            free(data.token->name);
            data.token->name = name;
            data.token->tokenType = TS_NUMBER;
            break;
          }
          default: {
            TS_parse_operator_a_op_b(tsFile, &data);
            break;
          }
        }
        data.done = TRUE;
      }
    }

    if (!data.done && tsFile->sanity == TS_FILE_VALID) {
      TS_parse_operator_a_op(tsFile, &data);
    }
    if (!data.done && tsFile->sanity == TS_FILE_VALID) {
      TS_parse_operator_op_a(tsFile, &data);
    }

  TS_TOKEN_END(TS_OPERATOR)
}
