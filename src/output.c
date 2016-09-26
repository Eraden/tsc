#include <cts/output.h>

// PRINT

void TS_print_indent(FILE *stream, const u_long indent) {
  for (u_long indentIndex = 0; indentIndex < indent; indentIndex++) {
    fprintf(stream, "%s", "  ");
    fflush(stream);
  }
}

void TS_push_indent_string(wchar_t *string, const u_long indent) {
  for (u_long indentIndex = 0; indentIndex < indent; indentIndex++)
    wcscat(string, (const wchar_t *) L"  ");
}

void
TS_print_for_token(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
  switch (tsParserToken->tokenType) {
    case TS_VAR: {
      break;
    }
    case TS_LET: {
      break;
    }
    case TS_CONST: {
      break;
    }
    case TS_CLASS: {
      break;
    }
    case TS_FUNCTION:
      break;
    case TS_ARROW: {
      break;
    }
    case TS_IF: {
      break;
    }
    case TS_ELSE: {
      break;
    }
    case TS_RETURN: {
      break;
    }
    case TS_DECORATOR: {
      break;
    }
    case TS_IMPORT: {
      break;
    }
    case TS_EXPORT: {
      break;
    }
    case TS_DEFAULT: {
      break;
    }
    case TS_SCOPE: {
      break;
    }
    case TS_EXTENDS: {
      break;
    }
    case TS_IMPLEMENTS: {
      break;
    }
    case TS_CLASS_FIELD: {
      break;
    }
    case TS_CLASS_METHOD: {
      break;
    }
    case TS_NEW: {
      break;
    }
    case TS_UNKNOWN: {
      break;
    }
    case TS_INLINE_COMMENT: {
      break;
    }
    case TS_MULTILINE_COMMENT: {
      break;
    }
    case TS_CONDITION: {
      break;
    }
    case TS_ARGUMENT: {
      break;
    }
    case TS_CALLER: {
      break;
    }
    case TS_SWITCH: {
      break;
    }
    case TS_CASE: {
      break;
    }
    case TS_BREAK: {
      break;
    }
    case TS_FOR: {
      break;
    }
    case TS_FOR_LET: {
      break;
    }
    case TS_FOR_IN: {
      break;
    }
    case TS_FOR_OF: {
      break;
    }
  }
}

// STRING

const wchar_t *
TS_string_for_token(
    TSFile *tsFile,
    TSParserToken *tsParserToken,
    TSOutputSettings outputSettings
) {
  switch (tsParserToken->tokenType) {
    case TS_VAR: {
      break;
    }
    case TS_LET: {
      break;
    }
    case TS_CONST: {
      break;
    }
    case TS_CLASS: {
      break;
    }
    case TS_FUNCTION: {
      break;
    }
    case TS_ARROW: {
      break;
    }
    case TS_IF:  {
      break;
    }
    case TS_ELSE: {
      break;
    }
    case TS_RETURN: {
      break;
    }
    case TS_DECORATOR: {
      break;
    }
    case TS_IMPORT: {
      break;
    }
    case TS_EXPORT: {
      break;
    }
    case TS_DEFAULT: {
      break;
    }
    case TS_SCOPE: {
      break;
    }
    case TS_EXTENDS: {
      break;
    }
    case TS_IMPLEMENTS: {
      break;
    }
    case TS_CLASS_FIELD: {
      break;
    }
    case TS_CLASS_METHOD: {
      break;
    }
    case TS_NEW: {
      break;
    }
    case TS_UNKNOWN: {
      break;
    }
    case TS_INLINE_COMMENT: {
      break;
    }
    case TS_MULTILINE_COMMENT: {
      break;
    }
    case TS_CONDITION: {
      break;
    }
    case TS_ARGUMENT: {
      break;
    }
    case TS_CALLER: {
      break;
    }
    case TS_SWITCH: {
      break;
    }
    case TS_CASE: {
      break;
    }
    case TS_BREAK: {
      break;
    }
    case TS_FOR: {
      break;
    }
    case TS_FOR_LET: {
      break;
    }
    case TS_FOR_IN: {
      break;
    }
    case TS_FOR_OF: {
      break;
    }
  }
  return NULL;
}

void
TS_print_stream(
    TSFile *tsFile,
    FILE *stream
) {

  if (tsFile->tokens == NULL) {
    return;
  }

  TSOutputSettings outputSettings;
  outputSettings.indent = 0;
  outputSettings.stream = stream;

  const wchar_t *header = TS_output_header();
  fprintf(stream, "%ls", header);
  fflush(stream);
  free((void *) header);

  const u_long size = tsFile->tokensSize;
  for (u_long i = 0; i < size; i++) {
    TS_print_for_token(tsFile, tsFile->tokens[i], outputSettings);
  }

  const wchar_t *footer = TS_output_footer();
  fprintf(stream, "%ls", footer);
  fflush(stream);
  free((void *) footer);
}

const wchar_t *
__attribute__((__unused__))
TS_generate_string_from_file(
    TSFile *tsFile
) {
  const u_long size = tsFile->tokensSize;
  TSOutputSettings outputSettings;
  outputSettings.indent = 0;
  outputSettings.stream = NULL;
  wchar_t *string = NULL;

  if (tsFile->tokens == NULL) {
    return NULL;
  }

  const wchar_t *header = TS_output_header();

  {
    wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), wcslen(header) + 1);
    wcscpy(newPointer, header);
    free((void *) header);
  }
  for (u_long i = 0; i < size; i++) {
    const wchar_t *s = TS_string_for_token(tsFile, tsFile->tokens[i], outputSettings);
    if (s != NULL) {
      wchar_t *newPointer = calloc(sizeof(wchar_t), (string ? wcslen(string) : 0) + wcslen(s) + 1);
      if (string != NULL) wcscpy(newPointer, string);
      if (string != NULL) free(string);
      wcscat(newPointer, s);
      string = newPointer;
      free((void *) s);
    }
  }

  {
    const wchar_t *footer = TS_output_footer();
    wchar_t *newPointer = (wchar_t *) calloc(sizeof(wchar_t), wcslen(string) + wcslen(footer) + 1);
    wcscpy(newPointer, string);
    wcscat(newPointer, footer);
    free((void *) string);
    free((void *) footer);
    string = newPointer;
  }

  return string;
}