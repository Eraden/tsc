#include <cts/file.h>
#include <cts/parser.h>

const unsigned char
TS_is_arrow(TSFile *tsFile ) {
  wchar_t *buffer = NULL;
  wchar_t *tok = NULL;
  unsigned char proceed = TRUE;
  unsigned char isArrow = FALSE;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile);

    tok = (wchar_t *) TS_get_token(tsFile->input.stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, tsFile->parse.parentTSToken, "arrow check");
      break;
    }

    if (wcscmp(tok, (const wchar_t *) L"=>") == 0) {
      buffer = TS_append_string(buffer, tok);
      isArrow = TRUE;
      break;
    }

    switch (*tok) {
      case L')': {
        buffer = TS_append_string(buffer, tok);
        if (tsFile->parse.parentTSToken->tokenType == TS_CALL_ARGUMENTS) {
          proceed = FALSE;
        }
        TS_join_strings(buffer, tok);
        break;
      }
      case L',': {
        buffer = TS_append_string(buffer, tok);
        if (tsFile->parse.parentTSToken->tokenType == TS_CALL_ARGUMENTS) {
          proceed = FALSE;
        }
        break;
      }
      case L';': {
        buffer = TS_append_string(buffer, tok);
        switch (tsFile->parse.parentTSToken->tokenType) {
          case TS_LET:
          case TS_CONST:
          case TS_VAR: {
            proceed = FALSE;
            break;
          }
          default:;
        }
        break;
      }
      default: {
        buffer = TS_append_string(buffer, tok);
      }
    }
    free((void *) tok);
  }
  TS_put_back(tsFile->input.stream, buffer);
  if (buffer) free(buffer);
  return isArrow;
}

TSParserToken *
TS_parse_arrow_as_variable_value(TSFile *tsFile) {
  TSParserToken *token = tsFile->parse.parentTSToken;
  TSParserToken *callArgs = TS_parse_call_arguments(tsFile);
  token->children[TS_ARROW_CALL_ARGS_INDEX] = callArgs;

  wchar_t *tok = NULL;
  unsigned char proceed = TRUE;
  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile);

    tok = (wchar_t *) TS_get_token(tsFile->input.stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "arrow function");
      break;
    }

    switch (*tok) {
      case L' ': {
        TS_MOVE_BY(tsFile, tok);
        break;
      }
      case L'\n': {
        TS_NEW_LINE(tsFile, tok);
        break;
      }
      case L'=': {
        TS_MOVE_BY(tsFile, tok);
        break;
      }
      case L'>': {
        TS_MOVE_BY(tsFile, tok);
        break;
      }
      case L'{': {
        tsFile->parse.token = tok;
        TSParserToken *body = TS_parse_scope(tsFile);
        token->children[TS_ARROW_BODY_INDEX] = body;
        proceed = FALSE;
        break;
      }
      default: {
        proceed = FALSE;
        TS_UNEXPECTED_TOKEN(tsFile, token, tok, "arrow function");
        break;
      }
    }
    free(tok);
  }

  return token;
}

TSParserToken *
TS_parse_arrow(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_ARROW, tsFile)
    token->childrenSize = 2;
    token->children = calloc(sizeof(TSParserToken *), token->childrenSize);

    switch (token->parent->tokenType) {
      case TS_CONST:
      case TS_LET:
      case TS_VAR: {
        TS_parse_arrow_as_variable_value(tsFile);
        break;
      }
      case TS_CALL_ARGUMENTS: {
        break;
      }
      default: {
        TS_UNEXPECTED_TOKEN(tsFile, token, tsFile->parse.token, "arrow function. Invalid position of arrow function");
        break;
      }
    }
  TS_TOKEN_END(TS_ARROW)
}

void
TS_free_arrow(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}