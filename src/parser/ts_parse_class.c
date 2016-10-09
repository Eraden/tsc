#include <cts/parser.h>
#include <cts/register.h>

static wchar_t *
__attribute__((visibility("hidden")))
TS_class_name_from_parent(
    TSParseData *tsParseData,
    TSParserToken *token,
    volatile const wchar_t *tok
) {
  TSParserToken *parent = token->parent;
  if (token->parent == NULL) {
    return NULL;
  }
  switch (parent->tokenType) {
    case TS_VAR:
    case TS_CONST:
    case TS_LET: {
      break;
    }
    default: {
      return NULL;
    }
  }
  if (tok) {
    TS_put_back(tsParseData->stream, (const wchar_t *) tok);
  }
  return TS_clone_string(parent->name);
}

static void
__attribute__((visibility("hidden")))
TS_parse_class_head(
    TSFile *tsFile,
    TSParseData *tsParseData,
    TSParserToken *token
) {
  TSParserToken *extends = NULL;
  volatile unsigned char proceed = TRUE;
  const wchar_t *tok;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "class header");
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
      case L'{': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        proceed = FALSE;
        break;
      }
      default: {
        tsParseData->token = tok;
        TSParserToken *child = TS_parse_ts_token(tsFile, tsParseData);
        free((void *) tok);

        switch (child->tokenType) {
          case TS_EXTENDS: {
            if (extends == NULL) {
              extends = child;
              TS_push_child(token, child);
            } else {
              TS_free_tsToken(child);
              ts_token_syntax_error((wchar_t *) L"Unexpected parent name. Class can have only one parent", tsFile, token);
              proceed = FALSE;
            }
            break;
          }
          case TS_IMPLEMENTS: {
            TS_push_child(token, child);
            break;
          }
          case TS_UNKNOWN: {
            TS_free_unknown(child);
            break;
          }
          default: {
            TS_free_tsToken(child);
            break;
          }
        }
        break;
      }
    }
  }
}

static void
__attribute__((visibility("hidden")))
TS_parse_class_method(
    TSFile *tsFile,
    TSParseData *tsParseData,
    TSParserToken *bodyToken
) {
  volatile unsigned char proceed = TRUE;
  const wchar_t *tok;

  TSClassParseFlag parseFlag = TS_PARSE_CLASS_MEMBER_METHOD_ARGUMENTS;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, bodyToken, "class header");
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
      case L'{':
      case L')': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        parseFlag = TS_PARSE_CLASS_MEMBER_METHOD_BODY;
        break;
      }
      case L':': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        parseFlag = TS_PARSE_CLASS_MEMBER_METHOD_RETURN_TYPE;
        break;
      }
      case L';': {
        TS_MOVE_BY(tsParseData, tok);
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
        if (parseFlag == TS_PARSE_CLASS_MEMBER_METHOD_ARGUMENTS) {
          TS_put_back(tsFile->stream, tok);
          tsParseData->token = tok;
          TSParserToken *argument = TS_parse_call_arguments(tsFile, tsParseData);
          TS_push_child(bodyToken, argument);

          free((void *) tok);
        } else if (parseFlag == TS_PARSE_CLASS_MEMBER_METHOD_BODY) {
          TS_put_back(tsFile->stream, tok);
          tsParseData->token = tok;

          TSParserToken *child = TS_parse_scope(tsFile, tsParseData);
          TS_push_child(bodyToken, child);
          proceed = FALSE;
          free((void *) tok);

        } else /*if (parseFlag == TS_PARSE_CLASS_MEMBER_METHOD_RETURN_TYPE)*/ {
          TSParserToken *classToken = TS_find_type(tsFile->file, tok);
          if (classToken) {
            TSParserToken *returnType = TS_build_parser_token(TS_FUNCTION_RETURN_TYPE, tsParseData);
            TS_push_child(returnType, classToken);
            TS_push_child(bodyToken, returnType);
            tsParseData->parentTSToken = returnType->parent;
          } else {
            TS_UNKNOWN_CLASS(tsFile, bodyToken, tok);
          }

          TS_MOVE_BY(tsParseData, tok);
          free((void *) tok);
        }
        break;
      }
    }
  }
}

static TSParserToken *
__attribute__((visibility("hidden")))
TS_parse_class_member(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  volatile unsigned char proceed = TRUE;
  const wchar_t *tok;
  TSClassParseFlag parseFlag = TS_PARSE_CLASS_MEMBER_NAME;

  TSParserToken *bodyToken = TS_build_parser_token(TS_UNKNOWN, tsParseData);
  bodyToken->modifiers = TS_MODIFIER_PRIVATE;

  wchar_t *name = NULL;
  wchar_t *type = NULL;
  wchar_t *value = NULL;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, bodyToken, "class member");
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
        if (name == NULL) {
          ts_token_syntax_error((wchar_t *) L"Missing class method name", tsFile, bodyToken);
          proceed = FALSE;
        } else {
          bodyToken->tokenType = TS_CLASS_METHOD;

          bodyToken->name = TS_clone_string(name);
          if (type) {
            TSParserToken *classToken = TS_find_type(tsFile->file, type);
            if (classToken) {
              TSParserToken *returnType = TS_build_parser_token(TS_FUNCTION_RETURN_TYPE, tsParseData);
              TS_push_child(returnType, classToken);
              TS_push_child(bodyToken, returnType);
              tsParseData->parentTSToken = returnType->parent;
            } else {
              TS_UNKNOWN_CLASS(tsFile, bodyToken, type);
            }
          }

          TS_parse_class_method(tsFile, tsParseData, bodyToken);

          proceed = FALSE;
          free((void *) tok);
        }
        break;
      }
      case L'=': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        parseFlag = TS_PARSE_CLASS_MEMBER_VALUE;
        break;
      }
      case L':': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        parseFlag = TS_PARSE_CLASS_MEMBER_TYPE;
        break;
      }
      case L';': {
        bodyToken->tokenType = TS_CLASS_FIELD;
        bodyToken->name = TS_clone_string(name);
        TSParserToken *tokenType = NULL;
        if (type) {
          tokenType = TS_find_type(tsFile->file, type);
        }
        if (tokenType == NULL) {
          tokenType = TS_find_type(tsFile->file, (const wchar_t *) L"any");
        }
        TS_push_child(bodyToken, tokenType);

        TSParserToken *tokenValue = TS_build_parser_token(TS_UNKNOWN, tsParseData);
        tokenValue->content = TS_clone_string(value);
        TS_push_child(bodyToken, tokenValue);
        tsParseData->parentTSToken = tokenValue->parent;

        proceed = FALSE;
        free((void *) tok);
        break;
      }
      default: {
        if (wcscmp(tok, (const wchar_t *) L"public") == 0) {
          bodyToken->modifiers = TS_MODIFIER_PUBLIC;

        } else if (wcscmp(tok, (const wchar_t *) L"protected") == 0) {
          bodyToken->modifiers = TS_MODIFIER_PROTECTED;

        } else if (wcscmp(tok, (const wchar_t *) L"private") == 0) {
          bodyToken->modifiers = TS_MODIFIER_PRIVATE;

        } else if (wcscmp(tok, (const wchar_t *) L"static") == 0) {
          bodyToken->modifiers = bodyToken->modifiers | TS_MODIFIER_STATIC;

        } else if (wcscmp(tok, (const wchar_t *) L"get") == 0) {
          bodyToken->modifiers = bodyToken->modifiers | TS_MODIFIER_GETTER;

        } else if (wcscmp(tok, (const wchar_t *) L"set") == 0) {
          bodyToken->modifiers = bodyToken->modifiers | TS_MODIFIER_SETTER;

        } else if (parseFlag == TS_PARSE_CLASS_MEMBER_NAME) {
          name = TS_clone_string(tok);
          parseFlag = TS_PARSE_CLASS_MEMBER_TYPE;

        } else if (parseFlag == TS_PARSE_CLASS_MEMBER_TYPE) {
          wchar_t *newPointer = TS_join_strings(type, tok);
          if (type) free((void *) type);
          type = newPointer;

        } else /*if (parseFlag == TS_PARSE_CLASS_MEMBER_VALUE)*/ {
          wchar_t *newPointer = TS_join_strings(value, tok);
          if (value != NULL) free((void *) value);
          value = newPointer;
        }

        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        break;
      }
    }
  }

  if (name) free((void *) name);
  if (type) free((void *) type);
  if (value) free((void *) value);

  tsParseData->parentTSToken = bodyToken->parent;

  return bodyToken;
}

static void
__attribute__((visibility("hidden")))
TS_parse_class_body(
    TSFile *tsFile,
    TSParseData *tsParseData,
    TSParserToken *token
) {
  volatile unsigned char proceed = TRUE;
  const wchar_t *tok = NULL;
  TSParserToken *child = NULL;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);
    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "class body member");
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
      case L'{': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        break;
      }
      default: {
        tsParseData->token = tok;
        TS_MOVE_BY(tsParseData, tok);
        child = TS_parse_ts_token(tsFile, tsParseData);

        if (child->tokenType == TS_UNKNOWN) {
          TS_put_back(tsParseData->stream, child->content);
          TS_free_tsToken(child);

          child = TS_parse_class_member(tsFile, tsParseData);

        }

        TS_push_child(token, child);

        free((void *) tok);
        break;
      }
    }
  }
}

TSParserToken *
TS_parse_class(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_CLASS, tsParseData)

    const wchar_t *tok;
    volatile unsigned char proceed = TRUE;
    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile);

      tok = (const wchar_t *) TS_getToken(tsParseData->stream);
      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "class name");
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
        case L'{': {
          if (token->parent) {
            token->name = TS_class_name_from_parent(tsParseData, token, tok);
            if (token->name == NULL) {
              free((void *) tok);
              ts_token_syntax_error((wchar_t *) L"Missing class name", tsFile, token);
              proceed = FALSE;
              break;
            }
            TS_put_back(tsParseData->stream, tok);
          } else {
            free((void *) tok);
            ts_token_syntax_error((wchar_t *) L"Missing class name", tsFile, token);
            proceed = FALSE;
            break;
          }
          proceed = FALSE;
          free((void *) tok);

          break;
        }
        default: {
          if (wcscmp((const wchar_t *) L"extends", tok) == 0 || wcscmp((const wchar_t *) L"implements", tok) == 0) {
            token->name = TS_class_name_from_parent(tsParseData, token, tok);
            if (token->name == NULL) {
              free((void *) tok);
              ts_token_syntax_error((wchar_t *) L"Missing class name", tsFile, token);
              proceed = FALSE;
              break;
            }
          } else if (TS_is_keyword(tok) == 1) {
            TS_UNEXPECTED_TOKEN(tsFile, token, tok, "class name");
            free((void *) tok);
            proceed = FALSE;
            break;

          } else {
            if (TS_name_is_valid(tok) == 0) {
              free((void *) tok);
              ts_token_syntax_error((wchar_t *) L"Invalid class name", tsFile, token);
            } else {
              token->name = TS_clone_string(tok);
              TS_MOVE_BY(tsParseData, tok);
              free((void *) tok);
            }
          }
          proceed = FALSE;
          break;
        }
      }
    }

    TS_register_type(tsFile, token);

    TS_parse_class_head(tsFile, tsParseData, token);
    TS_parse_class_body(tsFile, tsParseData, token);

  TS_TOKEN_END(TS_CLASS);
}

void
TS_free_class(
    const TSParserToken *token
) {
  TS_free_children(token);
  if (token->name) free(token->name);
  free((void *) token);
}

void
TS_free_class_field(
    const TSParserToken *token
) {
  TS_free_var(token);
}

void
TS_free_class_method(
    const TSParserToken *token
) {
  TS_free_function(token);
}
