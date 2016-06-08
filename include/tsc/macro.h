#pragma once

#define LOG(msg, ...) fprintf(stdout, msg, __VA_ARGS__);
#define FLOG(msg, ...) log_to_file(msg, __VA_ARGS__);
#define ERR(msg, ...) log_error(msg, __VA_ARGS__);
#define PANIC(context, code) \
  log_error("Failed at %s:%llu:%llu:%llu\n", context->file, context->line, context->character, context->position); \
  exit(code)
#define ALLOC_FAILED exit(1008);
#define CHECK_ALLOC_FAILED(obj, class) if (obj == NULL) { fprintf(stderr, "Could not allocate "#class"\n"); ALLOC_FAILED; }
#define POS(type, context) log_to_file(#type" at %s:%llu:%llu:%llu\n", context->file, context->line, context->character, context->position);
#define PUSH_TS(class) class **push##class (class **coll, unsigned long int size, class *el) { \
  FLOG("%s\n", "> Pushing "#class" to collection...");\
  FLOG(">   current collection size is: %lu\n", size);\
  class **new = (class **) malloc(sizeof(class *) * (size + 1)); \
  if (coll != NULL) for (unsigned long int i = 0; i < size + 1; i++) new[i] = coll[i]; \
  FLOG(">   pushed at position %lu new entry to collection, new size is %lu\n", size, size + 1);\
  FLOG(">   new coll address: %p\n", new); \
  new[size] = el; \
  return new; \
}

#define POP_TS(class) class **pop##class (class **, unsigned long int size) {\
  FLOG("%s\n", "> Pop "##class" from collection...")\
  FLOG(">   current collection size is: %lu\n", size);\
  class **new = (class **) malloc(sizeof(class *) * (size - 1)); \
  if (coll != NULL) for (unsigned long int i = 0; i < size - 1; i++) new[i] = coll[i]; \
  FLOG(">   new size is %lu\n", size - 1);\
  FLOG(">   new coll address: %p\n", new); \
  return new; \
}
