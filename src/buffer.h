#ifndef DEEZHTTP_STREAM_H_
#define DEEZHTTP_STREAM_H_

#include <stdlib.h>

typedef struct {
  char* buffer;
  size_t size;
  size_t capacity;
} dh_buffer;

dh_buffer dh_stream_new(size_t capacity);

void dh_stream_shrink(dh_buffer* stream);
void dh_stream_resize(dh_buffer* stream, size_t new_capacity);

void dh_stream_push(dh_buffer* stream, char* data, size_t data_size);

char* dh_stream_pop(dh_buffer* stream, size_t data_size);

inline char* dh_stream_get(dh_buffer* stream) { return stream->buffer; }
inline size_t dh_stream_size(dh_buffer* stream) { return stream->size; }
inline void dh_stream_clear(dh_buffer* stream) { stream->size = 0; }

void dh_stream_free(dh_buffer* stream);

#endif  // DEEZHTTP_STREAM_H_
