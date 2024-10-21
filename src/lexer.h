#ifndef DEEZHTTP_LEXER_H_
#define DEEZHTTP_LEXER_H_

#include <stddef.h>

typedef struct {
    char ch;
    size_t position;
    size_t peek_position;
    const char* string;
    size_t string_len;
} dh_lexer;

dh_lexer dh_create_lexer(const char* string, size_t string_len);
void dh_lexer_next_ch(dh_lexer* lexer);

void dh_lexer_destroy(dh_lexer* lexer);

#endif // DEEZHTTP_LEXER_H_
