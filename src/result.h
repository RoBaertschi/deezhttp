#ifndef DEEZHTTP_RESULT_H_
#define DEEZHTTP_RESULT_H_
#include <stdbool.h>

#ifdef __GNUC__
#define UNUSED_FUNCTION __attribute__((unused))
#define PUSH_IGNORE_UNUSED       \
  _Pragma("GCC diagnostic push") \
    _Pragma("GCC diagnostic ignored \"-Wunused-function\"")
#define POP_DIAGNOSTICS _Pragma("GCC diagnostic pop")
#else
#define UNUSED_FUNCTION
#define PUSH_IGNORE_UNUSED
#define POP_DIAGNOSTICS
#endif

#define RESULT(N, T, E)                                                      \
  typedef struct {                                                           \
    bool is_ok;                                                              \
    union {                                                                  \
      T ok;                                                                  \
      E err;                                                                 \
    };                                                                       \
  } N;                                                                       \
  static inline N N##_ok(T ok) { return (N){.is_ok = true, .ok = ok}; }      \
  static inline N N##_err(E err) { return (N){.is_ok = false, .err = err}; } \
  UNUSED_FUNCTION static T N##_unwrap(N result) {                            \
    if (result.is_ok) {                                                      \
      return result.ok;                                                      \
    }                                                                        \
    fprintf(stderr, "called unwrap on an error result\n");                   \
    abort();                                                                 \
  }                                                                          \
  UNUSED_FUNCTION static E N##_unwrap_err(N result) {                        \
    if (!result.is_ok) {                                                     \
      return result.err;                                                     \
    }                                                                        \
    fprintf(stderr, "called unwrap_err on an ok result\n");                  \
    abort();                                                                 \
  }                                                                          \
  static inline bool N##_is_ok(N result) { return result.is_ok; }            \
  static inline bool N##_is_err(N result) { return !result.is_ok; }

#endif  // DEEZHTTP_RESULT_H_
