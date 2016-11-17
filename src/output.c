#include <cts/output.h>
#include <cts/file.h>

void TS_write(TSFile *tsFile, const wchar_t *buffer) {
  switch (tsFile->output.type) {
    case TS_OUTPUT_STREAM: {
      fwprintf(tsFile->output.stream, L"%ls", buffer);
      fflush(tsFile->output.stream);
      break;
    }
    case TS_OUTPUT_STRING: {
      size_t len = wcslen(tsFile->output.buffer);
      if (tsFile->output.buffer == NULL) {
        tsFile->output.buffer = malloc(len + 1);
        wcscpy(tsFile->output.buffer, buffer);
        tsFile->output.buffer[len] = 0;
      } else {
        size_t currentLen = wcslen(tsFile->output.buffer);
        tsFile->output.buffer = realloc(tsFile->output.buffer, sizeof(wchar_t) * (len + currentLen + 1));
        wcscat(tsFile->output.buffer, buffer);
        tsFile->output.buffer[len + currentLen] = 0;
      }
      break;
    }
    case TS_OUTPUT_UNSET: {
      fprintf(stderr, "No output given!\n");
      exit(6);
    }
    case TS_OUTPUT_FILE: {
      FILE *stream = fopen(tsFile->output.outputFileName, "a+");
      fprintf(stream, "%ls", buffer);
      fflush(stream);
      fclose(stream);
      break;
    }
  }
}

void TS_clear_output(TSFile *tsFile) {
  switch (tsFile->output.type) {
    case TS_OUTPUT_UNSET: {
      tsFile->output.buffer = NULL;
      break;
    }
    case TS_OUTPUT_STREAM: {
      tsFile->output.stream = NULL;
      break;
    }
    case TS_OUTPUT_STRING: {
      free(tsFile->output.buffer);
      tsFile->output.buffer = NULL;
      break;
    }
    case TS_OUTPUT_FILE: {
      free(tsFile->output.outputFileName);
      tsFile->output.outputFileName = NULL;
      break;
    }
  }
}

void TS_set_output_stream(TSFile *tsFile, FILE *stream) {
  TS_clear_output(tsFile);
  tsFile->output.type = TS_OUTPUT_STREAM;
  tsFile->output.stream = stream;
}

void TS_set_output_string(TSFile *tsFile, wchar_t *str) {
  TS_clear_output(tsFile);
  tsFile->output.type = TS_OUTPUT_STRING;
  TS_write(tsFile, str);
}

void TS_set_output_file(TSFile *tsFile, char *fileName) {
  TS_clear_output(tsFile);
  tsFile->output.type = TS_OUTPUT_FILE;
  size_t len = strlen(fileName);
  tsFile->output.outputFileName = malloc(sizeof(char) * (len + 1));
  strcpy(tsFile->output.outputFileName, fileName);
  tsFile->output.outputFileName[len] = 0;
}
