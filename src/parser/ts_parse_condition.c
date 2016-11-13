#include <cts/parser.h>
#include <cts/register.h>

TSParserToken *
TS_parse_condition(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_CONDITION, tsParseData)
    if (token->parent == NULL) {
      ts_token_syntax_error((const wchar_t *) L"Invalid condition parent", tsFile, token);
    } else {
      switch (token->parent->tokenType) {
        case TS_SWITCH:
        case TS_IF:
        case TS_CASE: {
          break;
        }
        default: {
          ts_token_syntax_error((const wchar_t *) L"Invalid condition parent", tsFile, token);
          break;
        }
      }
    }

    const wchar_t *tok = NULL;
    volatile unsigned char proceed = TRUE;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (const wchar_t *) TS_getToken(tsParseData->stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "condition");
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
        case L')': {
          if (token->childrenSize == 0) {
            ts_token_syntax_error((const wchar_t *) L"Empty condition", tsFile, token);
            free((void *) tok);
            break;
          }
          switch (token->parent->tokenType) {
            case TS_SWITCH: {
              TS_MOVE_BY(tsParseData, tok);
              proceed = FALSE;
              break;
            }
            case TS_IF: {
              TS_MOVE_BY(tsParseData, tok);
              proceed = FALSE;
              break;
            }
            default: {
              ts_token_syntax_error((const wchar_t *) L"Invalid condition parent", tsFile, token);
              break;
            }
          }
          free((void *) tok);
          break;
        }
        case L':': {
          if (token->childrenSize == 0) {
            ts_token_syntax_error((const wchar_t *) L"Empty condition", tsFile, token);
            free((void *) tok);
            break;
          }
          switch (token->parent->tokenType) {
            case TS_CASE: {
              TS_MOVE_BY(tsParseData, tok);
              proceed = FALSE;
              break;
            }
            default: {
              ts_token_syntax_error((const wchar_t *) L"Invalid condition parent", tsFile, token);
              break;
            }
          }
          free((void *) tok);
          break;
        }
        default: {
          TS_MOVE_BY(tsParseData, tok);

          tsParseData->token = tok;
          TSParserToken *child = TS_parse_ts_token(tsFile, tsParseData);

          if (!child) {
            free((void *) tok);
            // todo error
            break;
          }

          // Operators will override this value
          if (token->childrenSize > 0) {
            TS_UNEXPECTED_TOKEN(tsFile, child, child->name, "condition");
            TS_free_tsToken(child);
            free((void *) tok);
            break;
          }

          TSParserToken *resolved = NULL;
          switch (child->tokenType) {
            case TS_UNKNOWN: {
              TSParserToken *unresolved = child;
              resolved = TS_search_token(unresolved);
              if (!resolved) resolved = TS_find_type(tsFile->file, unresolved->name);
              if (!resolved) {
                TS_UNEXPECTED_TOKEN(tsFile, unresolved, unresolved->name, "condition");
                TS_free_tsToken(unresolved);
              } else {
                resolved = TS_create_borrow(resolved, tsParseData);
                TS_free_tsToken(unresolved);
              }
              break;
            }
            default: {
              resolved = child;
              break;
            }
          }
          if (resolved) TS_push_child(token, resolved);

          free((void *) tok);
          break;
        }
      }
    }

  TS_TOKEN_END(TS_CONDITION)
}

void TS_free_condition(const TSParserToken *token) {
  TS_free_children(token);
  free((void *) token);
}

