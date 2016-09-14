#include <tsc/log.h>

static FILE *TS_error_file = NULL;
#define TS_error_stream TS_error_file

void
TS_set_error_output(FILE *file) {
  TS_error_file = file;
}

void
io_panic(
    wchar_t *msg
) {
  fprintf(stderr, "%ls\n%s\n", msg, strerror(errno));
  exit(TS_FILE_NOT_FOUND_CODE);
}

static void
create_log_directory() {
  const int perm = S_IRWXU | S_IRWXG | S_IROTH;

  const int result = mkdir("./log", perm);
  switch (result) {
    case EACCES:
    {
      fprintf(TS_error_stream, "User can't create log directory, permission denied!");
      exit(result);
    }
    case EEXIST:
    {
      // OK
      break;
    }
    case EMLINK:
    {
      fprintf(TS_error_stream, "Directory has too many links!");
      exit(result);
    }
    case ENOSPC:
    {
      fprintf(TS_error_stream, "Not enough space!");
      exit(result);
    }
    case EROFS:
    {
      fprintf(TS_error_stream, "Read only!");
      exit(result);
    }
    default:break;
  }
}

void init_log(void) {
  TS_error_file = stderr;

  if (TS_check_log_level(TS_VERBOSITY_OFF) == 0) return;

  create_log_directory();
  FILE *file;
  file = fopen("./log/info.log", "r");
  if (file != NULL) {
    fclose(file);
    log_to_file((wchar_t *) L"\n\n");
  } else {
    file = fopen("./log/info.log", "w");
    if (file == NULL) {
      io_panic((wchar_t *) L"Failed to open tsc.log for writing!\n");
    }
    time_t t = time(NULL);
    char *str = ctime(&t);
    fprintf(file, "%s\n", str);
    fclose(file);
    file = fopen("./log/error.log", "w");
    if (file == NULL) {
      io_panic((wchar_t *) L"Failed to open error.log for writing!\n");
    }
    fprintf(file, "%s\n", str);
    fclose(file);
  }
}

void
log_to_file(
    wchar_t *msg, ...
) {
  if (TS_check_log_level(TS_VERBOSITY_DEBUG) == 0) return;

  FILE *file = fopen("./log/info.log", "a");
  if (file == NULL) {
    io_panic((wchar_t *) L"Failed to open info.log for appending!\n");
  }
  va_list ap_file;
  va_start(ap_file, msg);
  vfwprintf(file, msg, ap_file);
  va_end(ap_file);
  fclose(file);

  if (TS_check_log_level(TS_VERBOSITY_LOG) == 0) return;
  file = stdout;
  va_list ap_stdout;
  va_start(ap_stdout, msg);
  vfwprintf(file, msg, ap_stdout);
  va_end(ap_stdout);
}

void
log_error(
    wchar_t *msg, ...
) {
  if (TS_check_log_level(TS_VERBOSITY_ERROR) == 0) return;

  FILE *file = fopen("./log/error.log", "a");
  if (file == NULL) {
    io_panic((wchar_t *) L"Failed to open error.log for appending!\n");
  }
  va_list ap;
  va_start(ap, msg);
  vfwprintf(file, msg, ap);
  va_end(ap);

  va_start(ap, msg);
  vfwprintf(TS_error_stream, msg, ap);
  va_end(ap);
  fclose(file);
}

