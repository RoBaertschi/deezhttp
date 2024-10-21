#include "lexer.h"
#include "error.h"
#include <stdlib.h>

dh_lexer dh_create_lexer(const char* string, size_t string_len) {
    const char* allocated_string = malloc(string_len / sizeof(char));
    CHECK_MALLOC(allocated_string);

    dh_lexer lexer = (dh_lexer){
        .position = 0,
        .peek_position = 0,
        .string = allocated_string,
        .string_len = string_len,
        .ch = 0,
    };

    dh_lexer_next_ch(&lexer);

    return lexer;
}

void dh_lexer_next_ch(dh_lexer* lexer) {
    if (lexer->peek_position >= lexer->string_len) {
        lexer->ch = 0;
    } else {
        lexer->ch = lexer->string[lexer->peek_position];
    }

    lexer->position = lexer->peek_position;
    lexer->peek_position += 1;
}
