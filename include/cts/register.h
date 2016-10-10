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

TSParserToken *TS_ANY_TYPE;
TSParserToken *TS_UNDEFINED_TYPE;
TSParserToken *TS_NULL_TYPE;

TSRegisterEntry *TS_register_file(TSFile *tsFile);
void TS_register_remove_file(TSFile *file);
TSFile *TS_find_file(const char *path, FILE *stream);

void TS_register_type(TSFile *tsFile, TSParserToken *token);
TSParserToken *TS_find_type(const wchar_t *file, const wchar_t *name);

void TS_destroy_register(void);

void TS_setup_predefined(void);
void TS_remove_predefined(void);

unsigned char TS_is_predefined(TSParserToken *token);

