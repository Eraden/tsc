#pragma once

#include <cts/parser.h>
#include <cts/sys.h>

typedef struct sTSRegisterEntry TSRegisterEntry;

typedef struct sTSRegisterEntry {
  TSFile *tsFile;
  TSParserToken **classList;
  u_long listSize;
} TSRegisterEntry;

#define RegisterCollection TSRegisterEntry **

TSRegisterEntry *TS_register_file(TSFile *tsFile);
void TS_register_remove_file(TSFile *file);

void TS_register_class(TSFile *tsFile, TSParserToken *token);
TSParserToken *TS_find_class(const wchar_t *file, const wchar_t *name);

void TS_destroy_register(void);

void TS_setup_predefined(void);
void TS_remove_predefined(void);

