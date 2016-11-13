#include <cts/register.h>

static TSParserToken *
TS_parse_interface_field(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TSParserToken *token = TS_parse_var(tsFile, tsParseData);
  token->tokenType = TS_CLASS_FIELD;
  return token;
}

static TSParserToken *
TS_parse_interface_method(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_CLASS_METHOD, tsParseData);
    const wchar_t *tok = NULL;
    volatile unsigned char proceed = TRUE;
    // arguments, returnType
    token->childrenSize = 2;
    token->children = calloc(sizeof(TSParserToken *), token->childrenSize);
    token->children[TS_INTERFACE_METHOD_ARGUMENTS_INDEX] = NULL;
    tsParseData->token = (const wchar_t *) L"";
    TSParserToken *returnType = token->children[TS_INTERFACE_METHOD_RETURN_TYPE_INDEX] = TS_build_parser_token(
        TS_FUNCTION_RETURN_TYPE, tsParseData);
    TS_push_child(returnType, TS_create_borrow(TS_ANY_TYPE, tsParseData));

    TSClassParseFlag flag = TS_PARSE_CLASS_MEMBER_NAME;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (const wchar_t *) TS_getToken(tsParseData->stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "interface method");
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
          proceed = FALSE;
          free((void *) tok);
          break;
        }
        case L'(': {
          tsParseData->token = tok;
          TSParserToken *callArgs = TS_parse_call_arguments(tsFile, tsParseData);
          token->children[TS_INTERFACE_METHOD_ARGUMENTS_INDEX] = callArgs;
          free((void *) tok);
          break;
        }
        case L';': {
          TS_MOVE_BY(tsParseData, tok);
          proceed = FALSE;
          free((void *) tok);
          break;
        }
        case L':': {
          TS_MOVE_BY(tsParseData, tok);
          flag = TS_PARSE_CLASS_MEMBER_METHOD_RETURN_TYPE;
          free((void *) tok);
          break;
        }
        default: {
          switch (flag) {
            case TS_PARSE_CLASS_MEMBER_METHOD_RETURN_TYPE: {
              TS_MOVE_BY(tsParseData, tok);
              TSParserToken *type = TS_find_type(tsFile->file, tok);
              TS_free_borrow(returnType->children[TS_VARIABLE_TYPE_INDEX]);
              returnType->children[TS_VARIABLE_TYPE_INDEX] = TS_create_borrow(type, tsParseData);
              flag = TS_PARSE_CLASS_MEMBER_NAME;
              free((void *) tok);
              break;
            }
            default: {
              TS_MOVE_BY(tsParseData, tok);
              if (token->content == NULL) {
                token->content = TS_clone_string(tok);
              } else {
                TS_UNEXPECTED_TOKEN(tsFile, token, tok, "interface method");
              }
              free((void *) tok);
              break;
            }
          }
          break;
        }
      }
    }
  TS_TOKEN_END(TS_CLASS_METHOD)
}

TSParserToken *
TS_parse_interface_body(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_INTERFACE_BODY, tsParseData);
    const wchar_t *tok = NULL;
    volatile unsigned char proceed = TRUE;

    wchar_t *buffer = NULL;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (const wchar_t *) TS_getToken(tsParseData->stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "interface body");
        break;
      }

      switch (tok[0]) {
        case L'\n': {
          TS_NEW_LINE(tsParseData, tok);
          if (buffer) {
            TS_put_back(tsFile->stream, buffer);
            TS_put_back(tsFile->stream, tok);
            TSParserToken *field = TS_parse_interface_field(tsFile, tsParseData);
            if (field) TS_push_child(token, field);
            free(buffer);
            buffer = NULL;
          }
          free((void *) tok);
          break;
        }
        case L'}': {
          TS_MOVE_BY(tsParseData, tok);
          if (buffer) {
            TS_put_back(tsFile->stream, buffer);
            TS_put_back(tsFile->stream, tok);
            TSParserToken *field = TS_parse_interface_field(tsFile, tsParseData);
            if (field) TS_push_child(token, field);
            free(buffer);
            buffer = NULL;
          }
          proceed = FALSE;
          free((void *) tok);
          break;
        }
        case L';': {
          TS_MOVE_BY(tsParseData, tok);
          if (buffer) {
            TS_put_back(tsFile->stream, buffer);
            TS_put_back(tsFile->stream, tok);
            TSParserToken *field = TS_parse_interface_field(tsFile, tsParseData);
            if (field) TS_push_child(token, field);
            free(buffer);
            buffer = NULL;
          }
          free((void *) tok);
          break;
        }
        case L'(': {
          if (buffer) TS_put_back(tsFile->stream, buffer);
          TS_put_back(tsFile->stream, tok);
          TSParserToken *method = TS_parse_interface_method(tsFile, tsParseData);
          TS_push_child(token, method);
          free((void *) tok);
          free(buffer);
          buffer = NULL;
          break;
        }
        default: {
          wchar_t *tmp = TS_join_strings(buffer, tok);
          free(buffer);
          buffer = tmp;
          free((void *) tok);
          break;
        }
      }
    }

  TS_TOKEN_END(TS_INTERFACE_BODY);
}

TSParserToken *
TS_parse_interface(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_INTERFACE, tsParseData)
    if (token->parent != NULL) {
      ts_token_syntax_error((const wchar_t *) L"Interface must be in global scope!", tsFile, token);
    }

    const wchar_t *tok = NULL;
    volatile unsigned char proceed;

    proceed = TRUE;

    // name and goto {
    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (const wchar_t *) TS_getToken(tsParseData->stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "interface");
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
          if (token->name == NULL) {
            TS_MISSING_NAME(tsFile, token, L"interface");
          }
          proceed = FALSE;
          free((void *) tok);
          break;
        }
        default: {
          if (wcscmp(tok, (const wchar_t *) L"extends") == 0) {
            if (token->name == NULL) {
              TS_MISSING_NAME(tsFile, token, L"interface");
            } else {
              tsParseData->token = tok;
              TS_parse_extends(tsFile, tsParseData);
            }
          } else if (TS_is_keyword(tok)) {
            TS_UNEXPECTED_TOKEN(tsFile, token, tok, "interface");
          } else if (token->name == NULL && TS_name_is_valid(tok) == FALSE) {
            ts_token_syntax_error((const wchar_t *) L"Not suitable interface name", tsFile, token);
          } else if (token->name == NULL) {
            TS_MOVE_BY(tsParseData, tok);
            token->name = TS_clone_string(tok);
          }
          free((void *) tok);
          break;
        }
      }
    }

    TSParserToken *body = TS_parse_interface_body(tsFile, tsParseData);
    TS_push_child(token, body);

    TS_register_type(tsFile, token);
  TS_TOKEN_END(TS_INTERFACE)
}

void
TS_free_interface(
    const TSParserToken *token
) {
  TS_free_children(token);
  if (token->name) free(token->name);
  free((void *) token);
}

void
TS_free_interface_body(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}