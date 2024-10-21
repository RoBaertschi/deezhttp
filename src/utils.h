#ifndef DEEZHTTP_UTILS_H_
#define DEEZHTTP_UTILS_H_

#define ANSI_RED "\x1b[31m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_BLUE "\x1b[34m"
#define ANSI_RESET "\x1b[0m"

#ifdef DEBUG_ENABLE
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

#endif  // DEEZHTTP_UTILS_H_
