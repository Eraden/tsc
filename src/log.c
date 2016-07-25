#include <tsc/log.h>

void io_panic(const char *msg) {
  fprintf(stderr, "%s\n", msg);
  switch (errno) {
    case EPERM:
      {
        fprintf(stderr, "Operation not permitted\n");
        exit(errno);
      }
    case ENOENT:
      {
        fprintf(stderr, "No such file or directory\n");
        exit(errno);
      }
    case ESRCH:
      {
        fprintf(stderr, "No such process\n");
        exit(errno);
      }
    case EINTR:
      {
        fprintf(stderr, "Interrupted system call\n");
        exit(errno);
      }
    case EIO:
      {
        fprintf(stderr, "I/O error\n");
        exit(errno);
      }
    case ENXIO:
      {
        fprintf(stderr, "No such device or address\n");
        exit(errno);
      }
    case E2BIG:
      {
        fprintf(stderr, "Argument list too long\n");
        exit(errno);
      }
    case ENOEXEC:
      {
        fprintf(stderr, "Exec format error\n");
        exit(errno);
      }
    case EBADF:
      {
        fprintf(stderr, "Bad file number\n");
        exit(errno);
      }
    case ECHILD:
      {
        fprintf(stderr, "No child processes\n");
        exit(errno);
      }
    case EAGAIN:
      {
        fprintf(stderr, "Try again\n");
        exit(errno);
      }
    case ENOMEM:
      {
        fprintf(stderr, "Out of memory\n");
        exit(errno);
      }
    case EACCES:
      {
        fprintf(stderr, "Permission denied\n");
        exit(errno);
      }
    case EFAULT:
      {
        fprintf(stderr, "Bad address\n");
        exit(errno);
      }
    case EBUSY:
      {
        fprintf(stderr, "Device or resource busy\n");
        exit(errno);
      }
    case EEXIST:
      {
        fprintf(stderr, "File exists\n");
        exit(errno);
      }
    case EXDEV:
      {
        fprintf(stderr, "Cross-device link\n");
        exit(errno);
      }
    case ENODEV:
      {
        fprintf(stderr, "No such device\n");
        exit(errno);
      }
    case ENOTDIR:
      {
        fprintf(stderr, "Not a directory\n");
        exit(errno);
      }
    case EISDIR:
      {
        fprintf(stderr, "Is a directory\n");
        exit(errno);
      }
    case EINVAL:
      {
        fprintf(stderr, "Invalid argument\n");
        exit(errno);
      }
    case ENFILE:
      {
        fprintf(stderr, "File table overflow\n");
        exit(errno);
      }
    case EMFILE:
      {
        fprintf(stderr, "Too many open files\n");
        exit(errno);
      }
    case ENOTTY:
      {
        fprintf(stderr, "Not a typewriter\n");
        exit(errno);
      }
    case EFBIG:
      {
        fprintf(stderr, "File too large\n");
        exit(errno);
      }
    case ENOSPC:
      {
        fprintf(stderr, "No space left on device\n");
        exit(errno);
      }
    case ESPIPE:
      {
        fprintf(stderr, "Illegal seek\n");
        exit(errno);
      }
    case EROFS:
      {
        fprintf(stderr, "Read-only file system\n");
        exit(errno);
      }
    case EMLINK:
      {
        fprintf(stderr, "Too many links\n");
        exit(errno);
      }
    case EPIPE:
      {
        fprintf(stderr, "Broken pipe\n");
        exit(errno);
      }
    case EDOM:
      {
        fprintf(stderr, "Math argument out of domain of func\n");
        exit(errno);
      }
    case ERANGE:
      {
        fprintf(stderr, "Math result not representable\n");
        exit(errno);
      }
    default:break;
  }
}

static void create_log_directory() {
  const int perm = S_IRWXU | S_IRWXG | S_IROTH;

//  fprintf(stdout, "Creating log directory...\n");
  const int result = mkdir("./log", perm);
  switch (result) {
    case EACCES:
      {
        fprintf(stderr, "User can't create log directory, permission denied!");
        exit(result);
      }
    case EEXIST:
      {
        // OK
        break;
      }
    case EMLINK:
      {
        fprintf(stderr, "Directory has too many links!");
        exit(result);
      }
    case ENOSPC:
      {
        fprintf(stderr, "Not enough space!");
        exit(result);
      }
    case EROFS:
      {
        fprintf(stderr, "Read only!");
        exit(result);
      }
    default:break;
  }
}

void init_log() {
  create_log_directory();
  FILE *file;
  file = fopen("./log/info.log", "r");
  if (file != NULL) {
    fclose(file);
    log_to_file("\n\n");
  } else {
    file = fopen("./log/info.log", "w");
    if (!file) {
      io_panic("Failed to open tsc.log for writing!\n");
    }
    time_t t = time(NULL);
    char *str = ctime(&t);
    fprintf(file, "%s\n", str);
    fclose(file);
    file = fopen("./log/error.log", "w");
    if (!file) {
      io_panic("Failed to open error.log for writing!\n");
    }
    fprintf(file, "%s\n", str);
    fclose(file);
  }
}

void log_to_file(char *msg, ...) {
  FILE *file = fopen("./log/info.log", "a");
  if (!file) {
    io_panic("Failed to open info.log for appending!\n");
  }
  va_list ap_file;
  va_start(ap_file, msg);
  vfprintf(file, msg, ap_file);
  va_end(ap_file);
  fclose(file);

#ifdef TS_DEBUG
  file = stdout;
  va_list ap_stdout;
  va_start(ap_stdout, msg);
  vfprintf(file, msg, ap_stdout);
  va_end(ap_stdout);
#endif
}

void log_error(char *msg, ...) {
  FILE *file = fopen("./log/error.log", "a");
  if (!file) {
    io_panic("Failed to open error.log for appending!\n");
  }
  va_list ap;
  va_start(ap, msg);
  vfprintf(file, msg, ap);
  va_end(ap);
  va_start(ap, msg);
  vfprintf(stderr, msg, ap);
  va_end(ap);
  fclose(file);
}

