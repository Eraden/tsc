#include <cts/parser.h>

TSParserToken *
TS_parse_export(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_EXPORT, tsFile)

    if (token->parent != NULL && !TS_is_embedded_in(token, TS_NAMESPACE)) {
      TS_token_syntax_error(
          (const wchar_t *) L"Unexpected parent for export. Export can be declared only in global scope.",
          tsFile, token
      );
    }

    volatile unsigned char proceed = TRUE;
    const wchar_t *tok = NULL;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (const wchar_t *) TS_get_token(tsFile->input.stream);

      if (tok == NULL) {
        if (token->childrenSize == 1)
          break;
        else {
          TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "`export` keyword");
          break;
        }
      }

      switch (tok[0]) {
        case L' ': {
          TS_MOVE_BY(tsFile, tok);
          free((void *) tok);
          break;
        }
        case L'\n': {
          if (token->childrenSize == 0) {
            free((void *) tok);
            TS_token_syntax_error(
                (wchar_t *) L"Expecting expression after `export` keyword. Found new line.",
                tsFile,
                token
            );
            break;
          } else {
            proceed = FALSE;
            TS_put_back(tsFile->input.stream, tok);
            free((void *) tok);
            break;
          }
        }
        case L';': {
          proceed = FALSE;
          TS_put_back(tsFile->input.stream, tok);
          free((void *) tok);
          break;
        }
        default: {
          if (token->childrenSize == 1) {
            TS_put_back(tsFile->input.stream, tok);
            proceed = FALSE;
          } else {
            TS_MOVE_BY(tsFile, tok);

            tsFile->parse.token = tok;
            TSParserToken *child = TS_parse_ts_token(tsFile);

            TS_push_child(token, child);
          }
          free((void *) tok);
          break;
        }
      }
    }

  TS_TOKEN_END(TS_EXPORT)
}

void
TS_free_export(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}
