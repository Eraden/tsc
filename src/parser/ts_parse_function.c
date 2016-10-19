#include <cts/parser.h>
#include <cts/register.h>

static void
TS_parse_function_arguments(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TSParserToken *token = tsParseData->parentTSToken;
  const wchar_t *tok;
  volatile unsigned char proceed = TRUE;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "function argument");
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
        tsParseData->token = tok;
        TSParserToken *callArguments = TS_parse_call_arguments(tsFile, tsParseData);
        TS_push_child(token, callArguments);
        proceed = FALSE;
        free((void *) tok);
        break;
      }

      default: {
        TS_UNEXPECTED_TOKEN(tsFile, token, tok, "function arguments");
        free((void *) tok);
        break;
      }
    }
  }
}

static void
TS_parse_function_lookup_return_type(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TSParserToken *token = tsParseData->parentTSToken;
  const wchar_t *tok;
  volatile unsigned char proceed = TRUE;
  volatile unsigned char foundColon = FALSE;
  volatile unsigned char foundType = FALSE;
  TSParserToken *type = TS_find_type(tsFile->file, (const wchar_t *) L"any");
  TSParserToken *returnType = TS_build_parser_token(TS_FUNCTION_RETURN_TYPE, tsParseData);
  tsParseData->parentTSToken = returnType->parent;
  TS_push_child(returnType, type);
  TS_push_child(token, returnType);

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "function return type");
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
      case L':': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        proceed = FALSE;
        foundColon = TRUE;
        break;
      }
      case L'{': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        proceed = FALSE;
        break;
      }
      default: {
        TS_UNEXPECTED_TOKEN(tsFile, token, tok, "function type");
        proceed = FALSE;
      }
    }
  }

  proceed = foundColon;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_getToken(tsParseData->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "function return type");
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
        if (!foundType) {
          ts_token_syntax_error(
              (const wchar_t *) L"Found colon but type wasn't declared while parsing function!",
              tsFile,
              token
          );
          free((void *) tok);
        } else {
          TS_MOVE_BY(tsParseData, tok);
          free((void *) tok);
        }
        proceed = FALSE;
        break;
      }
      default: {
        if (foundType) {
          free((void *) tok);
          ts_token_syntax_error(
              (wchar_t *) L"Unexpected token while parsing function return type. Return type was already defined!",
              tsFile,
              token
          );
          proceed = FALSE;
        } else if (!TS_name_is_valid(tok)) {
          free((void *) tok);
          ts_token_syntax_error((wchar_t *) L"Invalid type name for function return type!", tsFile, token);
          proceed = FALSE;
        } else {
          foundType = TRUE;
          type = TS_find_type(tsFile->file, tok);
          if (type) {
            returnType->children[0] = type;
          } else {
            TS_UNKNOWN_TYPE(tsFile, token, tok);
          }
          TS_MOVE_BY(tsParseData, tok);
          free((void *) tok);
        }
        break;
      }
    }
  }
}

TSParserToken *
TS_parse_function(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_FUNCTION, tsParseData)
    const wchar_t *tok;
    volatile unsigned char proceed = TRUE;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (const wchar_t *) TS_getToken(tsParseData->stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "function");
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
          if (token->name) {
            proceed = FALSE;
            TS_put_back(tsFile->stream, tok);
            free((void *) tok);
            break;
          } else if (token->parent) {
            proceed = FALSE;
            TS_put_back(tsFile->stream, tok);
            free((void *) tok);

            switch (token->parent->tokenType) {
              case TS_RETURN: {
                break;
              }
              case TS_CONST:
              case TS_LET:
              case TS_VAR: {
                token->name = TS_clone_string(token->parent->name);
                break;
              }
              default: {
                ts_token_syntax_error((const wchar_t *) L"Missing function name", tsFile, token);
                proceed = FALSE;
              }
            }
          } else {
            TS_MISSING_NAME(tsFile, token, L"function");
            proceed = FALSE;
            free((void *) tok);
          }
          break;
        }
        default: {
          if (TS_name_is_valid(tok)) {
            token->name = TS_clone_string(tok);
            free((void *) tok);
          } else {
            free((void *) tok);
            ts_token_syntax_error((wchar_t *) L"Invalid function name", tsFile, token);
          }
          proceed = FALSE;
          break;
        }
      }
    }

    TS_parse_function_arguments(tsFile, tsParseData);
    TS_parse_function_lookup_return_type(tsFile, tsParseData);
    tsParseData->token = (const wchar_t *) L"{";
    TSParserToken *body = TS_parse_scope(tsFile, tsParseData);
    TS_push_child(token, body);

  TS_TOKEN_END(TS_FUNCTION);
}

void
TS_free_function(
    const TSParserToken *token
) {
  TS_free_children(token);

  if (token->name != NULL) free((void *) token->name);
  free((void *) token);
}

void
TS_free_function_return_type(
    const TSParserToken *token
) {
  TS_free_children_from(token, 1);
  free((void *) token);
}