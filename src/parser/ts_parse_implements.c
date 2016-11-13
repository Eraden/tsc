#include <cts/parser.h>
#include <cts/register.h>

TSParserToken *
TS_parse_implements(
    TSFile *__attribute__((__unused__)) tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_IMPLEMENTS, tsParseData)
    unsigned char proceed = TRUE;
    wchar_t *tok = NULL;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (wchar_t *) TS_get_token(tsFile->stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "json");
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
          TS_put_back(tsFile->stream, tok);
          proceed = FALSE;
          free(tok);
          break;
        }
        case L',': {
          TS_MOVE_BY(tsParseData, tok);
          TS_put_back(tsFile->stream, (volatile const int *) L"implements");
          proceed = FALSE;
          free((void *) tok);
          break;
        }
        default: {
          if (TS_is_keyword(tok)) {
            TS_UNEXPECTED_TOKEN(tsFile, token, tok, "implements");
          } else if (TS_name_isValid(tok) == FALSE) {
            TS_MISSING_NAME(tsFile, token, "implements");
          } else {
            TSParserToken *definition = TS_find_type(tsFile->file, tok);
            if (definition) {
              TS_push_child(token, TS_create_borrow(definition, tsParseData));
            } else {
              TS_MISSING_NAME(tsFile, token, "implements");
            }
          }
          free(tok);
          break;
        }
      }
    }
  TS_TOKEN_END(TS_IMPLEMENTS);
}

void
TS_free_implements(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}
