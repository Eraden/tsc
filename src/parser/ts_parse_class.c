#include <cts/file.h>
#include <cts/parser.h>
#include <cts/register.h>

static wchar_t *TS_class_method_move_to_type(TSFile *tsFile) {
  wchar_t *tok = NULL;
  while (1) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (wchar_t *) TS_get_token(tsFile->input.stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, tsFile->parse.parentTSToken, "class method return type");
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
        return tok;
      }
    }
  }
  return NULL;
}

static wchar_t *
TS_class_name_from_parent(TSFile *tsFile, TSParserToken *token, volatile const wchar_t *tok) {
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
    TS_put_back(tsFile->input.stream, (const wchar_t *) tok);
  }
  return TS_clone_string(parent->name);
}

static void
TS_parse_class_head(TSFile *tsFile, TSParserToken *token) {
  TSParserToken *extends = NULL;
  volatile unsigned char proceed = TRUE;
  const wchar_t *tok;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_get_token(tsFile->input.stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "class header");
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
      case L'{': {
        TS_MOVE_BY(tsFile, tok);
        free((void *) tok);
        proceed = FALSE;
        break;
      }
      default: {
        tsFile->parse.token = tok;
        TSParserToken *child = TS_parse_ts_token(tsFile);

        switch (child->tokenType) {
          case TS_EXTENDS: {
            if (extends == NULL) {
              extends = child;
              TS_push_child(token, child);
            } else {
              TS_token_syntax_error((wchar_t *) L"Unexpected parent name. Class can have only one parent", tsFile,
                                    token);
              proceed = FALSE;
              TS_free_ts_token(child);
            }
            break;
          }
          case TS_IMPLEMENTS: {
            TS_push_child(token, child);
            break;
          }
          case TS_UNKNOWN:
          default: {
            TS_UNEXPECTED_TOKEN(tsFile, child, tok, "class head");
            TS_free_ts_token(child);
            break;
          }
        }
        free((void *) tok);
        break;
      }
    }
  }
}

static void
TS_parse_class_method(TSFile *tsFile, TSParserToken *bodyToken) {
  volatile unsigned char proceed = TRUE;
  const wchar_t *tok = NULL;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_get_token(tsFile->input.stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, bodyToken, "class header");
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
      case L'{': {
        TS_MOVE_BY(tsFile, tok);
        tsFile->parse.token = tok;

        TSParserToken *scope = TS_parse_scope(tsFile);
        bodyToken->children[TS_CLASS_METHOD_BODY_INDEX] = scope;
        free((void *) tok);
        proceed = FALSE;
        break;
      }
      case L':': {
        TS_MOVE_BY(tsFile, tok);
        free((void *) tok);

        tok = TS_class_method_move_to_type(tsFile);
        if (tok) {
          TSParserToken *classToken = TS_find_type(tsFile->input.file, tok);
          if (classToken) {
            TSParserToken *returnType = bodyToken->children[TS_CLASS_METHOD_RETURN_TYPE_INDEX];
            TS_free_borrow(returnType->children[0]);
            returnType->children[0] = TS_create_borrow(classToken, tsFile);
          } else {
            TS_UNKNOWN_TYPE(tsFile, bodyToken, tok);
          }
          TS_MOVE_BY(tsFile, tok);
          free((void *) tok);
        }
        break;
      }
      case L'(': {
        tsFile->parse.token = tok;
        TSParserToken *arguments = TS_parse_call_arguments(tsFile);
        if (arguments) {
          bodyToken->children[TS_CLASS_METHOD_CALL_ARGS_INDEX] = arguments;
        } else {
          // TODO error
        }

        free((void *) tok);
        break;
      }
      default: {
        TS_UNEXPECTED_TOKEN(tsFile, bodyToken, tok, "class method");
        free((void *) tok);
      }
    }
  }
}

static TSParserToken *
TS_parse_class_member(TSFile *tsFile) {
  volatile unsigned char proceed = TRUE;
  const wchar_t *tok;
  TSClassParseFlag parseFlag = TS_PARSE_CLASS_MEMBER_NAME;

  TSParserToken *bodyToken = TS_build_parser_token(TS_UNKNOWN, tsFile);
  tsFile->parse.parentTSToken = bodyToken->parent;
  bodyToken->modifiers = TS_MODIFIER_PRIVATE;

  wchar_t *name = NULL;
  wchar_t *type = NULL;
  wchar_t *value = NULL;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_get_token(tsFile->input.stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, bodyToken, "class member");
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
      case L'(': {
        if (name == NULL) {
          TS_token_syntax_error((wchar_t *) L"Missing class method name", tsFile, bodyToken);
          proceed = FALSE;
        } else {
          bodyToken->tokenType = TS_CLASS_METHOD;
          tsFile->parse.parentTSToken = bodyToken;

          bodyToken->name = TS_clone_string(name);

          bodyToken->childrenSize = 3;
          bodyToken->children = calloc(sizeof(TSParserToken *), bodyToken->childrenSize);
          TSParserToken *returnType = TS_build_parser_token(TS_FUNCTION_RETURN_TYPE, tsFile);
          TS_push_child(returnType, TS_create_borrow(TS_ANY_TYPE, tsFile));
          bodyToken->children[TS_CLASS_METHOD_RETURN_TYPE_INDEX] = returnType;
          tsFile->parse.parentTSToken = bodyToken;

          if (type) {
            TSParserToken *classToken = TS_find_type(tsFile->input.file, type);
            if (classToken) {
              TS_free_borrow(returnType->children[0]);
              returnType->children[0] = TS_create_borrow(classToken, tsFile);
            } else {
              TS_UNKNOWN_TYPE(tsFile, bodyToken, type);
            }
          }

          TS_put_back(tsFile->input.stream, tok);
          TS_parse_class_method(tsFile, bodyToken);
          tsFile->parse.parentTSToken = bodyToken->parent;

          proceed = FALSE;
          free((void *) tok);
        }
        break;
      }
      case L'=': {
        TS_MOVE_BY(tsFile, tok);
        free((void *) tok);
        parseFlag = TS_PARSE_CLASS_MEMBER_VALUE;
        break;
      }
      case L':': {
        TS_MOVE_BY(tsFile, tok);
        free((void *) tok);
        parseFlag = TS_PARSE_CLASS_MEMBER_TYPE;
        break;
      }
      case L';': {
        bodyToken->tokenType = TS_CLASS_FIELD;
        bodyToken->name = TS_clone_string(name);
        TSParserToken *tokenType = NULL;
        if (type) {
          tokenType = TS_find_type(tsFile->input.file, type);
        }
        if (tokenType == NULL) {
          tokenType = TS_find_type(tsFile->input.file, (const wchar_t *) L"any");
        }
        TS_push_child(bodyToken, TS_create_borrow(tokenType, tsFile));

        TSParserToken *tokenValue = TS_build_parser_token(TS_UNKNOWN, tsFile);
        tsFile->parse.parentTSToken = tokenValue->parent;
        tokenValue->content = TS_clone_string(value);
        TS_push_child(bodyToken, tokenValue);

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

        TS_MOVE_BY(tsFile, tok);
        free((void *) tok);
        break;
      }
    }
  }

  if (name) free((void *) name);
  if (type) free((void *) type);
  if (value) free((void *) value);

  tsFile->parse.parentTSToken = bodyToken->parent;

  return bodyToken;
}

static void
TS_parse_class_body(TSFile *tsFile, TSParserToken *token) {
  volatile unsigned char proceed = TRUE;
  const wchar_t *tok = NULL;
  TSParserToken *child = NULL;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_get_token(tsFile->input.stream);
    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "class body member");
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
      case L'}': {
        TS_MOVE_BY(tsFile, tok);
        free((void *) tok);
        proceed = FALSE;
        break;
      }
      case L'{': {
        TS_MOVE_BY(tsFile, tok);
        free((void *) tok);
        break;
      }
      default: {
        tsFile->parse.token = tok;
        TS_MOVE_BY(tsFile, tok);
        child = TS_parse_ts_token(tsFile);

        if (child->tokenType == TS_UNKNOWN) {
          TS_put_back(tsFile->input.stream, child->content);
          TS_free_ts_token(child);

          child = TS_parse_class_member(tsFile);
        }

        TS_push_child(token, child);

        free((void *) tok);
        break;
      }
    }
  }
}

TSParserToken *
TS_parse_class(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_CLASS, tsFile)

    const wchar_t *tok;
    volatile unsigned char proceed = TRUE;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile);

      tok = (const wchar_t *) TS_get_token(tsFile->input.stream);
      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "class name");
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
        case L'{': {
          if (token->parent) {
            token->name = TS_class_name_from_parent(tsFile, token, tok);
            if (token->name == NULL) {
              free((void *) tok);
              TS_token_syntax_error((wchar_t *) L"Missing class name", tsFile, token);
              proceed = FALSE;
              break;
            }
            TS_put_back(tsFile->input.stream, tok);
          } else {
            free((void *) tok);
            TS_token_syntax_error((wchar_t *) L"Missing class name", tsFile, token);
            proceed = FALSE;
            break;
          }
          proceed = FALSE;
          free((void *) tok);

          break;
        }
        default: {
          if (wcscmp((const wchar_t *) L"extends", tok) == 0 || wcscmp((const wchar_t *) L"implements", tok) == 0) {
            TS_put_back(tsFile->input.stream, tok);
            token->name = TS_class_name_from_parent(tsFile, token, tok);
            if (token->name == NULL) {
              free((void *) tok);
              TS_MISSING_NAME(tsFile, token, "class");
            }
          } else if (TS_is_keyword(tok) == TRUE) {
            TS_UNEXPECTED_TOKEN(tsFile, token, tok, "class name");
            free((void *) tok);

          } else if (TS_name_is_valid(tok) == FALSE) {
            TS_token_syntax_error((wchar_t *) L"Invalid class name", tsFile, token);
            free((void *) tok);

          } else {
            token->name = TS_clone_string(tok);
            TS_MOVE_BY(tsFile, tok);
            free((void *) tok);
          }
          proceed = FALSE;
          break;
        }
      }
    }

    TS_register_type(tsFile, token);

    TS_parse_class_head(tsFile, token);
    TS_parse_class_body(tsFile, token);

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
