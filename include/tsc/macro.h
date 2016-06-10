#pragma once

#define LOG(msg, ...) \
  fprintf(stdout, msg, __VA_ARGS__);
#define FLOG(msg, ...) \
  log_to_file(msg, __VA_ARGS__);
#define ERR(msg, ...) \
  log_error(msg, __VA_ARGS__);
#define PANIC(context, code) \
  log_error("Failed at %s:%llu:%llu:%llu\n", context->file, context->line, context->character, context->position); \
  exit(code)
#define ALLOC_FAILED \
  exit(1008);
#define CHECK_ALLOC_FAILED(obj, class) \
  if (obj == NULL) { fprintf(stderr, "Could not allocate "#class"\n"); ALLOC_FAILED; }
#define POS(type, context) \
  log_to_file(#type" at %s:%llu:%llu:%llu\n", context->file, context->line, context->character, context->position);
#define PUSH_TS(class) class **push##class (class **coll, unsigned long int size, class *el) { \
  class **new = (class **) malloc(sizeof(class *) * (size + 1)); \
  if (coll != NULL) for (unsigned long int i = 0; i < size + 1; i++) new[i] = coll[i]; \
  new[size] = el; \
  return new; \
}

#define POP_TS(class) class **pop##class (class **, unsigned long int size) {\
  class **new = (class **) malloc(sizeof(class *) * (size - 1)); \
  if (coll != NULL) for (unsigned long int i = 0; i < size - 1; i++) new[i] = coll[i]; \
  return new; \
}

#define TS_ASSERT(context, condition, failure_msg) \
  if (!(condition)) { \
    ERR("%s\n", failure_msg); \
    PANIC(context, 1); \
  }