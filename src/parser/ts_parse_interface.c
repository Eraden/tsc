#include <cts/register.h>

TSParserToken *
TS_parse_interface(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_INTERFACE, tsParseData)
    if (token->parent != NULL) {
      ts_token_syntax_error((const wchar_t *) L"Interface must be in global scope!", tsFile, token);
    }

    const wchar_t *tok;
    volatile unsigned char proceed;

    proceed = (volatile unsigned char) (tsFile->sanity == TS_FILE_VALID);

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
    // body
    proceed = (volatile unsigned char) (tsFile->sanity == TS_FILE_VALID);
    TSParserToken *child = NULL, *returnType = NULL, *callArgs = NULL, *type = NULL;
    TSClassParseFlag flag = TS_PARSE_CLASS_MEMBER_NAME;

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
          if (child) TS_push_child(token, child);
          child = NULL;
          returnType = NULL;
          free((void *) tok);
          break;
        }
        case L'}': {
          TS_MOVE_BY(tsParseData, tok);
          proceed = FALSE;
          free((void *) tok);
          break;
        }
        case L';': {
          TS_MOVE_BY(tsParseData, tok);
          child = NULL;
          free((void *) tok);
          break;
        }
        case L':': {
          TS_MOVE_BY(tsParseData, tok);
          if (child == NULL) {
            TS_UNEXPECTED_TOKEN(tsFile, token, tok, "interface");
          } else {
            TS_MOVE_BY(tsParseData, tok);
            switch (child->tokenType) {
              case TS_CLASS_METHOD:
              case TS_FUNCTION: {
                flag = TS_PARSE_CLASS_MEMBER_METHOD_RETURN_TYPE;
                break;
              }
              case TS_CONST:
              case TS_VAR:
              case TS_UNKNOWN: {
                flag = TS_PARSE_CLASS_MEMBER_TYPE;
                break;
              }
              default: {
                break;
              }
            }

          }
          free((void *) tok);
          break;
        }
        case L'(': {
          TS_MOVE_BY(tsParseData, tok);
          if (child == NULL) {
            tsParseData->token = (const wchar_t *) L"";
            child = TS_build_parser_token(TS_FUNCTION, tsParseData);
            tsParseData->token = tok;
            callArgs = TS_parse_call_arguments(tsFile, tsParseData);
            TS_push_child(child, callArgs);
            tsParseData->parentTSToken = child->parent;
            returnType = TS_build_parser_token(TS_FUNCTION_RETURN_TYPE, tsParseData);
            type = TS_find_type(tsFile->file, (const wchar_t *) L"any");
            if (type) {
              TS_push_child(returnType, type);
            } else {
              TS_UNKNOWN_TYPE(tsFile, token, L"any");
            }
            TS_push_child(child, returnType);
            tsParseData->parentTSToken = child->parent;
          } else {
            flag = TS_PARSE_CLASS_MEMBER_METHOD_ARGUMENTS;
          }
          free((void *) tok);
          break;
        }
        case L'=': {
          if (child == NULL) {
            TS_UNEXPECTED_TOKEN(tsFile, token, tok, "interface");
          } else {
            TS_MOVE_BY(tsParseData, tok);
            flag = TS_PARSE_CLASS_MEMBER_VALUE;
          }
          free((void *) tok);
          break;
        }
        case L'{': {
          TS_MOVE_BY(tsParseData, tok);
          flag = TS_PARSE_CLASS_MEMBER_METHOD_BODY;
          TS_UNEXPECTED_TOKEN(tsFile, token, tok, "interface member");
          proceed = FALSE;
          free((void *) tok);
          break;
        }
        default: {
          switch (flag) {
            case TS_PARSE_CLASS_MEMBER_NAME: {
              if (TS_is_keyword(tok)) {
                TS_UNEXPECTED_TOKEN(tsFile, token, tok, "interface member name");
              } else {
                tsParseData->token = tok;
                child = TS_parse_ts_token(tsFile, tsParseData);
              }
              break;
            }
            case TS_PARSE_CLASS_MEMBER_METHOD_RETURN_TYPE: {
              if (child == NULL) {
                TS_UNEXPECTED_TOKEN(tsFile, token, tok, "interface member return type");
              } else {
                TS_MOVE_BY(tsParseData, tok);
                type = TS_find_type(tsFile->file, tok);
                if (type) {
                  TS_push_child(child, type);
                } else {
                  TS_UNKNOWN_TYPE(tsFile, token, tok);
                }
                returnType->children[0] = type;
                child = NULL;
                flag = TS_PARSE_CLASS_MEMBER_NAME;
              }
              break;
            }
            case TS_PARSE_CLASS_MEMBER_TYPE: {
              if (child == NULL) {
                TS_UNEXPECTED_TOKEN(tsFile, token, tok, "interface member return type");
              } else {
                TS_MOVE_BY(tsParseData, tok);
                type = TS_find_type(tsFile->file, tok);
                if (type) {
                  TS_push_child(child, type);
                } else {
                  TS_UNKNOWN_TYPE(tsFile, token, tok);
                }
                TS_push_child(token, child);
                child = NULL;
                returnType = NULL;
                flag = TS_PARSE_CLASS_MEMBER_NAME;
              }
              break;
            }
            case TS_PARSE_CLASS_MEMBER_METHOD_ARGUMENTS: {
              if (child == NULL) {
                TS_UNEXPECTED_TOKEN(tsFile, token, tok, "interface member return type");
              } else {
                TS_MOVE_BY(tsParseData, tok);
                token->tokenType = TS_CLASS_METHOD;
                tsParseData->token = tok;
                callArgs = TS_parse_call_arguments(tsFile, tsParseData);
                TS_push_child(child, callArgs);
                returnType = TS_build_parser_token(TS_FUNCTION_RETURN_TYPE, tsParseData);
                type = TS_find_type(tsFile->file, (const wchar_t *) L"any");
                if (type) {
                  TS_push_child(returnType, type);
                } else {
                  TS_UNKNOWN_TYPE(tsFile, token, L"any");
                }
                TS_push_child(child, returnType);
                tsParseData->parentTSToken = child->parent;
                flag = TS_PARSE_CLASS_MEMBER_NAME;
              }
              break;
            }
            case TS_PARSE_CLASS_MEMBER_VALUE: {
              if (child == NULL) {
                TS_UNEXPECTED_TOKEN(tsFile, token, tok, "interface member return type");
              } else {
                if (child->children == NULL) {
                  type = TS_find_type(tsFile->file, (const wchar_t *) L"any");
                  if (type) {
                    TS_push_child(child, type);
                  } else {
                    TS_UNKNOWN_TYPE(tsFile, token, L"any");
                  }
                }
                tsParseData->token = tok;
                TSParserToken *value = TS_parse_ts_token(tsFile, tsParseData);
                TS_push_child(child, value);
                TS_push_child(token, child);
                child = NULL;
                returnType = NULL;
              }
              break;
            }
            case TS_PARSE_CLASS_MEMBER_METHOD_BODY:break;
          }
          free((void *) tok);
          break;
        }
      }
    }
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
