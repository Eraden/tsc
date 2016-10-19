#include <cts/register.h>

static void
TS_parse_import_resolve_imports(
    TSParserToken *importsToken,
    TSFile *importedFile
) {
  TSParserToken *token, *exported, *imported;
  TSParserToken **tokens, **imports = importsToken->children;
  for (u_long importIndex = 0; importIndex < importsToken->childrenSize; importIndex++) {
    imported = imports[0];
    tokens = importedFile->tokens;
    for (u_long exportIndex = 0; exportIndex < importedFile->tokensSize; exportIndex++) {
      token = tokens[0];
      if (token->tokenType == TS_EXPORT && token->childrenSize) {
        exported = token->children[0];
        if (wcscmp(exported->name, imported->name) == 0) {
          // TODO
        }
      }
      tokens += 1;
    }
  }
}

static TSParserToken *
TS_parse_import_from(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_IMPORT_FROM, tsParseData);

    unsigned char proceed = TRUE;
    wchar_t *tok = NULL;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (wchar_t *) TS_getToken(tsFile->stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "import from");
        break;
      }

      switch (tok[0]) {
        case L' ': {
          TS_MOVE_BY(tsParseData, tok);
          if (token->childrenSize) proceed = FALSE;
          free((void *) tok);
          break;
        }
        case L'\n': {
          TS_NEW_LINE(tsParseData, tok);
          if (token->childrenSize) proceed = FALSE;
          free((void *) tok);
          break;
        }
        case L';': {
          TS_MOVE_BY(tsParseData, tok);
          if (token->children == NULL) {
            ts_token_syntax_error((const wchar_t *) L"Unexpected `;` while parsing `import from`", tsFile, token);
          }
          free((void *) tok);
          proceed = FALSE;
          break;
        }
        case L'\'':
        case L'"': {
          tsParseData->token = tok;
          TSParserToken *string = TS_parse_string(tsFile, tsParseData);
          if (string) {
            wchar_t *directory = TS_resolve_directory(tsFile->file);
            wchar_t *resolved = TS_resolve_path(directory, string->content);
            free(directory);
            if (resolved) {
              free(string->content);
              string->content = resolved;
              u_long size = 2 * wcslen(resolved) + 1;
              char *path = calloc(sizeof(char), size);
              wcstombs(path, resolved, size);
              token->data = TS_parse_file(path);
              free(path);
            } else {
              ts_token_syntax_error(
                  (const wchar_t *) L"Could not resolve relative path for imported file",
                  tsFile, token
              );
              ts_token_syntax_error_info(tsFile, (const wchar_t *) L"%ls", string->content);
            }
            TS_push_child(token, string);
          } else {
            ts_token_syntax_error((const wchar_t *) L"Expecting string while parsing `import from` but nothing was found", tsFile, token);
          }
          proceed = FALSE;
          free(tok);
          break;
        }
        default: {
          TS_UNEXPECTED_TOKEN(tsFile, token, tok, "import from");
          free((void *) tok);
          break;
        }
      }
    }

  TS_TOKEN_END(TS_IMPORT_FROM)
}

static TSParserToken *
TS_parse_import_from_file(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TSParserToken *token = tsParseData->parentTSToken;
  TSParserToken *fromToken = NULL;
  unsigned char proceed = TRUE;
  wchar_t *tok = NULL;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (wchar_t *) TS_getToken(tsFile->stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "import");
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
      default: {
        if (wcscmp(tok, (const wchar_t *) L"from") == 0) {
          tsParseData->token = tok;
          fromToken = TS_parse_import_from(tsFile, tsParseData);
        } else {
          TS_UNEXPECTED_TOKEN(tsFile, token, tok, "import, expect `from`");
        }
        proceed = FALSE;
        free((void *) tok);
        break;
      }
    }
  }
  return fromToken;
}

static TSParserToken *
TS_parse_import_imported_tokens(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_IMPORTED_TOKENS, tsParseData);

    unsigned char proceed = TRUE, IMPORT_STARTED = FALSE;
    wchar_t *tok = NULL;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (wchar_t *) TS_getToken(tsFile->stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "import tokens");
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
          IMPORT_STARTED = TRUE;
          TS_MOVE_BY(tsParseData, tok);
          free((void *) tok);
          break;
        }
        case L'}': {
          TS_MOVE_BY(tsParseData, tok);
          proceed = FALSE;
          free((void *) tok);
          break;
        }
        case L',': {
          TS_MOVE_BY(tsParseData, tok);
          free((void *) tok);
          break;
        }
        default: {
          if (TS_is_keyword(tok)) {
            TS_UNEXPECTED_TOKEN(tsFile, token, tok, "import");
          } else if (!TS_name_is_valid(tok)) {
            TS_UNEXPECTED_TOKEN(tsFile, token, tok, "import");
          } else if (IMPORT_STARTED) {
            tsParseData->token = tok;
            TSParserToken *unknown = TS_parse_ts_token(tsFile, tsParseData);
            if (unknown) {
              TS_push_child(token, unknown);
            } else {
              ts_token_syntax_error((const wchar_t *) L"Expecting token while parsing `import` but nothing was found", tsFile, token);
            }
          } else {
            TS_UNEXPECTED_TOKEN(tsFile, token, tok, "import");
          }
          free((void *) tok);
          break;
        }
      }
    }

  TS_TOKEN_END(TS_IMPORTED_TOKENS)
}

TSParserToken *
TS_parse_import(
    TSFile *tsFile,
    TSParseData *tsParseData
) {
  TS_TOKEN_BEGIN(TS_IMPORT, tsParseData)

    tsParseData->token = (const wchar_t *) L"";
    TSParserToken *imports = TS_parse_import_imported_tokens(tsFile, tsParseData);
    if (imports) {
      TS_push_child(token, imports);
    }
    TSParserToken *fromToken = TS_parse_import_from_file(tsFile, tsParseData);
    if (fromToken) {
      TS_push_child(token, fromToken);
    }

    if (imports && fromToken && fromToken->data) {
      TS_parse_import_resolve_imports(imports, fromToken->data);
    }

  TS_TOKEN_END(TS_IMPORT)
}

void
TS_free_import_from(
    const TSParserToken *token
) {
  TS_free_children(token);
  if (token->data) TS_free_tsFile(token->data);
  free((void *) token);
}

void
TS_free_import(
    const TSParserToken *token
) {
  TS_free_children(token);
  free((void *) token);
}

void
TS_free_imported_tokens(
    const TSParserToken *token
) {
  // TODO find proper tokens in imported file
  TS_free_children(token);
//  if (token->children) free(token->children);
  free((void *) token);
}