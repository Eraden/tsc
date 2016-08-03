#pragma once

#include <tsc/sys.h>
#include <tsc/parser.h>

struct sTSRegisterEntry;

typedef struct sTSRegisterEntry {
  const TSFile *tsFile;
  TSParserToken classToken;
  struct sTSRegisterEntry *next;
  struct sTSRegisterEntry *prev;
} TSRegisterEntry;

void TS_register_class(const TSFile *tsFile, const TSParserToken token);
const TSParserToken TS_find_class(const wchar_t *file, const wchar_t *name);