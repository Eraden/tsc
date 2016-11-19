#include <cts/file.h>
#include <cts/parser.h>
#include <cts/output.h>
#include <cts/register.h>

TSFile *TS_build_file() {
  TSFile *tsFile = NULL;
  tsFile = calloc(sizeof(TSFile), 1);
  // content
  tsFile->tokens = NULL;
  tsFile->tokensSize = 0;
  // input
  tsFile->input.stream = NULL;
  tsFile->input.file = NULL;
  // output
  tsFile->output.currentToken = NULL;
  tsFile->output.stream = NULL;
  tsFile->output.indent = 0;
  tsFile->output.type = TS_OUTPUT_UNSET;
  // settings
  tsFile->settings.preserveComments = TRUE;
  tsFile->settings.highlight = TRUE;
  tsFile->settings.indentSize = 2;
  return tsFile;
}

void
TS_free_ts_file(
    TSFile *tsFile
) {
  for (; tsFile->tokensSize; ) {
    tsFile->tokensSize -= 1;
    TS_free_ts_token(tsFile->tokens[tsFile->tokensSize]);
  }
  if (tsFile->tokens != NULL) {
    free(tsFile->tokens);
  }
  if (tsFile->input.file) {
    free(tsFile->input.file);
  }
  if (tsFile->errorReason) {
    free(tsFile->errorReason);
  }

  switch (tsFile->output.type) {
    case TS_OUTPUT_UNSET: {
      break;
    }
    case TS_OUTPUT_STREAM: {
      break;
    }
    case TS_OUTPUT_STRING: {
      free(tsFile->output.buffer);
      break;
    }
    case TS_OUTPUT_FILE: {
      free(tsFile->output.outputFileName);
      break;
    }
  }
  tsFile->output.type = TS_OUTPUT_UNSET;

  TS_register_remove_file(tsFile);

  free((void *) tsFile);
}

