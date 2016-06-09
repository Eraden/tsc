#include <tsc/log.h>

static void io_panic(const char *msg) {
  fprintf(stderr, "%s\n", msg);
  switch (errno) {
    case EPERM:
      {
        fprintf(stderr, "Operation not permitted\n");
        exit(errno);
        break;
      }
    case ENOENT:
      {
        fprintf(stderr, "No such file or directory\n");
        exit(errno);
        break;
      }
    case ESRCH:
      {
        fprintf(stderr, "No such process\n");
        exit(errno);
        break;
      }
    case EINTR:
      {
        fprintf(stderr, "Interrupted system call\n");
        exit(errno);
        break;
      }
    case EIO:
      {
        fprintf(stderr, "I/O error\n");
        exit(errno);
        break;
      }
    case ENXIO:
      {
        fprintf(stderr, "No such device or address\n");
        exit(errno);
        break;
      }
    case E2BIG:
      {
        fprintf(stderr, "Argument list too long\n");
        exit(errno);
        break;
      }
    case ENOEXEC:
      {
        fprintf(stderr, "Exec format error\n");
        exit(errno);
        break;
      }
    case EBADF:
      {
        fprintf(stderr, "Bad file number\n");
        exit(errno);
        break;
      }
    case ECHILD:
      {
        fprintf(stderr, "No child processes\n");
        exit(errno);
        break;
      }
    case EAGAIN:
      {
        fprintf(stderr, "Try again\n");
        exit(errno);
        break;
      }
    case ENOMEM:
      {
        fprintf(stderr, "Out of memory\n");
        exit(errno);
        break;
      }
    case EACCES:
      {
        fprintf(stderr, "Permission denied\n");
        exit(errno);
        break;
      }
    case EFAULT:
      {
        fprintf(stderr, "Bad address\n");
        exit(errno);
        break;
      }
    case EBUSY:
      {
        fprintf(stderr, "Device or resource busy\n");
        exit(errno);
        break;
      }
    case EEXIST:
      {
        fprintf(stderr, "File exists\n");
        exit(errno);
        break;
      }
    case EXDEV:
      {
        fprintf(stderr, "Cross-device link\n");
        exit(errno);
        break;
      }
    case ENODEV:
      {
        fprintf(stderr, "No such device\n");
        exit(errno);
        break;
      }
    case ENOTDIR:
      {
        fprintf(stderr, "Not a directory\n");
        exit(errno);
        break;
      }
    case EISDIR:
      {
        fprintf(stderr, "Is a directory\n");
        exit(errno);
        break;
      }
    case EINVAL:
      {
        fprintf(stderr, "Invalid argument\n");
        exit(errno);
        break;
      }
    case ENFILE:
      {
        fprintf(stderr, "File table overflow\n");
        exit(errno);
        break;
      }
    case EMFILE:
      {
        fprintf(stderr, "Too many open files\n");
        exit(errno);
        break;
      }
    case ENOTTY:
      {
        fprintf(stderr, "Not a typewriter\n");
        exit(errno);
        break;
      }
    case EFBIG:
      {
        fprintf(stderr, "File too large\n");
        exit(errno);
        break;
      }
    case ENOSPC:
      {
        fprintf(stderr, "No space left on device\n");
        exit(errno);
        break;
      }
    case ESPIPE:
      {
        fprintf(stderr, "Illegal seek\n");
        exit(errno);
        break;
      }
    case EROFS:
      {
        fprintf(stderr, "Read-only file system\n");
        exit(errno);
        break;
      }
    case EMLINK:
      {
        fprintf(stderr, "Too many links\n");
        exit(errno);
        break;
      }
    case EPIPE:
      {
        fprintf(stderr, "Broken pipe\n");
        exit(errno);
        break;
      }
    case EDOM:
      {
        fprintf(stderr, "Math argument out of domain of func\n");
        exit(errno);
        break;
      }
    case ERANGE:
      {
        fprintf(stderr, "Math result not representable\n");
        exit(errno);
        break;
      }
  }
}

static void create_log_directory() {
  const int perm = S_IRUSR | S_IRWXU | S_IREAD | S_IWUSR | S_IWRITE | S_IRGRP | S_IWGRP | S_IROTH;

  // fprintf(stdout, "perm: %i \t\t %o\n", S_IRUSR , S_IRUSR  );
  // fprintf(stdout, "perm: %i \t\t %o\n", S_IREAD , S_IREAD  );
  // fprintf(stdout, "perm: %i \t\t %o\n", S_IWUSR , S_IWUSR  );
  // fprintf(stdout, "perm: %i \t\t %o\n", S_IWRITE, S_IWRITE );
  // fprintf(stdout, "perm: %i \t\t %o\n", S_IRGRP , S_IRGRP  );
  // fprintf(stdout, "perm: %i \t\t %o\n", S_IWGRP , S_IWGRP  );
  // fprintf(stdout, "perm: %i \t\t %o\n", S_IROTH , S_IROTH  );
  // fprintf(stdout, "Log dir perm: %o\n", perm);

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
  }
}

void init_log() {
  create_log_directory();
  FILE *file;
  file = fopen("./log/tsc.log", "r");
  if (file != NULL) {
    fclose(file);
    log_to_file("\n\n");
  } else {
    file = fopen("./log/tsc.log", "w");
    if (!file) {
      io_panic("Failed to open tsc.log for writing!\n");
    }
    time_t t = time(NULL);
    char *str = ctime(&t);
    fprintf(file, "%s\n", str);
    fclose(file);
    file = fopen("./log/tsc.error.log", "w");
    if (!file) {
      io_panic("Failed to open tsc.error.log for writing!\n");
    }
    fprintf(file, "%s\n", str);
    fclose(file);
  }
}

void log_to_file(char *msg, ...) {
  FILE *file = fopen("./log/tsc.log", "a");
  if (!file) {
    io_panic("Failed to open tsc.log for appending!\n");
  }
  va_list ap;
  va_start(ap, msg);
  vfprintf(file, msg, ap);
  va_end(ap);
  fclose(file);
}

void log_error(char *msg, ...) {
  FILE *file = fopen("./log/tsc.error.log", "a");
  if (!file) {
    io_panic("Failed to open tsc.error.log for appending!\n");
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

