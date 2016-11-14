#include <cts/parser.h>

static wchar_t *
__attribute__((section("parse-decorator")))
TS_parse_decorator_name(
    TSFile *tsFile,
    TSParseData *tsParseData,
    TSParserToken *token
) {
  const wchar_t *tok = NULL;
  volatile unsigned char proceed = TRUE;
  wchar_t *name = NULL;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (const wchar_t *) TS_get_token(tsFile->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "decorator call");
      free((void *) tok);
      break;
    }

    switch (tok[0]) {
      case L'(': {
        TS_MOVE_BY(tsParseData, tok);
        if (name == NULL) {
          TS_token_syntax_error((const wchar_t *) L"Missing decorator name", tsFile, token);
        } else {
          tsParseData->token = tok;
          TSParserToken *callArgs = TS_parse_call_arguments(tsFile, tsParseData);
          TS_push_child(token, callArgs);
        }
        free((void *) tok);
        proceed = FALSE;
        break;
      }
      case '\n':
      case '\t':
      case '\r':
      case L' ': {
        TS_MOVE_BY(tsParseData, tok);
        free((void *) tok);
        if (!name) {
          TS_token_syntax_error((const wchar_t *) L"Unexpected white character in decorator call function name", tsFile,
                                token);
        }
        proceed = FALSE;
        break;
      }
      default: {
        if (TS_name_is_valid(tok) && !TS_is_keyword(tok)) {
          wchar_t *newPointer = TS_join_strings(name, tok);
          if (name) free(name);
          name = newPointer;
          TS_MOVE_BY(tsParseData, tok);
        } else {
          TS_UNEXPECTED_TOKEN(tsFile, token, tok, "call decorator");
        }
        free((void *) tok);
        break;
      }
    }
  }

  if (tsFile->sanity == TS_FILE_VALID) {
    if (name == NULL) {
      TS_token_syntax_error(
          (const wchar_t *) L"Missing decorator call function name",
          tsFile,
          token
      );
    } else if (TS_name_is_valid(name) != TRUE) {
      TS_token_syntax_error(
          (const wchar_t *) L"Invalid characters in decorator call",
          tsFile,
          token
      );
    }
  }

  return name;
}

TSParserToken *
__attribute__((section("parse-decorator")))
TS_parse_decorator(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_DECORATOR, tsParseData)

    wchar_t *name = TS_parse_decorator_name(tsFile, tsParseData, token);

    if (name) {
      token->name = TS_clone_string(name);
      free((void *) name);
    }

    if (tsFile->sanity == TS_FILE_VALID) {
      tsParseData->token = (const wchar_t *) L"(";
      TSParserToken *args = TS_parse_call_arguments(tsFile, tsParseData);
      TS_push_child(token, args);
    }

  TS_TOKEN_END(TS_DECORATOR)
}

void
TS_free_decorator(
    const TSParserToken *token
) {
  TS_free_children(token);
  if (token->name) free(token->name);
  free((void *) token);
}
