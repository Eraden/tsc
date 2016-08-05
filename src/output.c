#include <tsc/output.h>

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
    const TSFile *tsFile,
    TSParserToken tsParserToken,
    TSOutputSettings outputSettings
) {
  switch (tsParserToken.tokenType) {
    case TS_VAR:
      TS_print_from_var(tsFile, tsParserToken, outputSettings);
      break;
    case TS_LET:
      TS_print_from_var(tsFile, tsParserToken, outputSettings);
      break;
    case TS_CONST:
      TS_print_from_var(tsFile, tsParserToken, outputSettings);
      break;
    case TS_CLASS: {
      TS_print_for_class(tsFile, tsParserToken, outputSettings);
      break;
    }
    case TS_FUNCTION:
      TS_print_from_function(tsFile, tsParserToken, outputSettings);
      break;
    case TS_ARROW: {
      break;
    }
    case TS_IF: {
      TS_print_for_if(tsFile, tsParserToken, outputSettings);
      break;
    }
    case TS_ELSE: {
      TS_print_for_else(tsFile, tsParserToken, outputSettings);
      break;
    }
    case TS_RETURN: {
      ts_print_for_return(tsFile, tsParserToken, outputSettings);
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
    case TS_CONDITION:break;
    case TS_ARGUMENT:break;
  }
}

// STRING

const wchar_t *
TS_string_for_token(
    const TSFile *tsFile,
    TSParserToken tsParserToken,
    TSOutputSettings outputSettings
) {
  switch (tsParserToken.tokenType) {
    case TS_VAR:
      return TS_string_from_var(tsFile, tsParserToken, outputSettings);
    case TS_LET:
      return TS_string_from_var(tsFile, tsParserToken, outputSettings);
    case TS_CONST:
      return TS_string_from_var(tsFile, tsParserToken, outputSettings);
    case TS_CLASS: {
      return TS_string_for_class(tsFile, tsParserToken, outputSettings);
    }
    case TS_FUNCTION:
      return TS_string_from_function(tsFile, tsParserToken, outputSettings);
    case TS_ARROW: {
      wchar_t *tag = (wchar_t *) L"=>";
      wchar_t *s = (wchar_t *) calloc(sizeof(wchar_t), sizeof(tag) + 1);
      wcscat(s, tag);
      return s;
    }
    case TS_IF: {
      return TS_string_for_if(tsFile, tsParserToken, outputSettings);
    }
    case TS_ELSE: {
      return TS_string_for_else(tsFile, tsParserToken, outputSettings);
    }
    case TS_RETURN: {
      return ts_string_for_return(tsFile, tsParserToken, outputSettings);
    }
    case TS_DECORATOR: {
      wchar_t *tag = (wchar_t *) L"component";
      wchar_t *s = (wchar_t *) calloc(sizeof(wchar_t), sizeof(tag) + 1);
      wcscat(s, tag);
      return s;
    }
    case TS_IMPORT: {
      wchar_t *tag = (wchar_t *) L"import";
      wchar_t *s = (wchar_t *) calloc(sizeof(wchar_t), sizeof(tag) + 1);
      wcscat(s, tag);
      return s;
    }
    case TS_EXPORT: {
      wchar_t *tag = (wchar_t *) L"export";
      wchar_t *s = (wchar_t *) calloc(sizeof(wchar_t), sizeof(tag) + 1);
      wcscat(s, tag);
      return s;
    }
    case TS_DEFAULT: {
      wchar_t *tag = (wchar_t *) L"default";
      wchar_t *s = (wchar_t *) calloc(sizeof(wchar_t), sizeof(tag) + 1);
      wcscat(s, tag);
      return s;
    }
    case TS_SCOPE: {
      wchar_t *tag = (wchar_t *) L"unnamed scope";
      wchar_t *s = (wchar_t *) calloc(sizeof(wchar_t), sizeof(tag) + 1);
      wcscat(s, tag);
      return s;
    }
    case TS_EXTENDS: {
      wchar_t *tag = (wchar_t *) L"extends";
      wchar_t *s = (wchar_t *) calloc(sizeof(wchar_t), sizeof(tag) + 1);
      wcscat(s, tag);
      return s;
    }
    case TS_IMPLEMENTS: {
      wchar_t *tag = (wchar_t *) L"implements";
      wchar_t *s = (wchar_t *) calloc(sizeof(wchar_t), sizeof(tag) + 1);
      wcscat(s, tag);
      return s;
    }
    case TS_CLASS_FIELD:
      break;
    case TS_CLASS_METHOD:
      break;
    case TS_NEW:
      break;
    case TS_UNKNOWN:
      break;
    case TS_INLINE_COMMENT:
      break;
    case TS_MULTILINE_COMMENT:
      break;
    case TS_CONDITION:break;
    case TS_ARGUMENT:break;
  }
  return NULL;
}

void
TS_print_stream(
    const TSFile *tsFile,
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
    const TSFile *tsFile
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