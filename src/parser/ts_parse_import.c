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
TS_parse_import_from(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_IMPORT_FROM, tsFile);

    unsigned char proceed = TRUE;
    wchar_t *tok = NULL;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (wchar_t *) TS_get_token(tsFile->input.stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "import from");
        break;
      }

      switch (tok[0]) {
        case L' ': {
          TS_MOVE_BY(tsFile, tok);
          if (token->childrenSize) proceed = FALSE;
          free((void *) tok);
          break;
        }
        case L'\n': {
          TS_NEW_LINE(tsFile, tok);
          if (token->childrenSize) proceed = FALSE;
          free((void *) tok);
          break;
        }
        case L';': {
          TS_MOVE_BY(tsFile, tok);
          if (token->children == NULL) {
            TS_token_syntax_error((const wchar_t *) L"Unexpected `;` while parsing `import from`", tsFile, token);
          }
          free((void *) tok);
          proceed = FALSE;
          break;
        }
        case L'\'':
        case L'"': {
          tsFile->parse.token = tok;
          TSParserToken *string = TS_parse_string(tsFile);
          if (string) {
            wchar_t *directory = TS_resolve_directory(tsFile->input.file);
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
              TS_token_syntax_error(
                  (const wchar_t *) L"Could not resolve relative path for imported file",
                  tsFile, token
              );
              TS_token_syntax_error_info(tsFile, (const wchar_t *) L"%ls", string->content);
            }
            TS_push_child(token, string);
          } else {
            TS_token_syntax_error(
                (const wchar_t *) L"Expecting string while parsing `import from` but nothing was found", tsFile, token);
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
TS_parse_import_from_file(TSFile *tsFile) {
  TSParserToken *token = tsFile->parse.parentTSToken;
  TSParserToken *fromToken = NULL;
  unsigned char proceed = TRUE;
  wchar_t *tok = NULL;

  while (proceed) {
    TS_LOOP_SANITY_CHECK(tsFile)

    tok = (wchar_t *) TS_get_token(tsFile->input.stream);

    if (tok == NULL) {
      TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "import");
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
        if (wcscmp(tok, (const wchar_t *) L"from") == 0) {
          tsFile->parse.token = tok;
          fromToken = TS_parse_import_from(tsFile);
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
TS_parse_import_imported_tokens(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_IMPORTED_TOKENS, tsFile);

    unsigned char proceed = TRUE, IMPORT_STARTED = FALSE;
    wchar_t *tok = NULL;

    while (proceed) {
      TS_LOOP_SANITY_CHECK(tsFile)

      tok = (wchar_t *) TS_get_token(tsFile->input.stream);

      if (tok == NULL) {
        TS_UNEXPECTED_END_OF_STREAM(tsFile, token, "import tokens");
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
          IMPORT_STARTED = TRUE;
          TS_MOVE_BY(tsFile, tok);
          free((void *) tok);
          break;
        }
        case L'}': {
          TS_MOVE_BY(tsFile, tok);
          proceed = FALSE;
          free((void *) tok);
          break;
        }
        case L',': {
          TS_MOVE_BY(tsFile, tok);
          free((void *) tok);
          break;
        }
        default: {
          if (TS_is_keyword(tok)) {
            TS_UNEXPECTED_TOKEN(tsFile, token, tok, "import");
          } else if (!TS_name_is_valid(tok)) {
            TS_UNEXPECTED_TOKEN(tsFile, token, tok, "import");
          } else if (IMPORT_STARTED) {
            tsFile->parse.token = tok;
            TSParserToken *unknown = TS_parse_ts_token(tsFile);
            if (unknown) {
              TS_push_child(token, unknown);
            } else {
              TS_token_syntax_error((const wchar_t *) L"Expecting token while parsing `import` but nothing was found",
                                    tsFile, token);
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
TS_parse_import(TSFile *tsFile) {
  TS_TOKEN_BEGIN(TS_IMPORT, tsFile)

    tsFile->parse.token = (const wchar_t *) L"";
    TSParserToken *imports = TS_parse_import_imported_tokens(tsFile);
    if (imports) {
      TS_push_child(token, imports);
    }
    TSParserToken *fromToken = TS_parse_import_from_file(tsFile);
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
  if (token->data) TS_free_ts_file(token->data);
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
  free((void *) token);
}