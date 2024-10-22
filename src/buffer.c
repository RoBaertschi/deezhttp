#include "buffer.h"

#include "error.h"

#define DH_MAX_THRESHOLD 1.8
#define DH_STREAM_GROWTH_FACTOR 1.5
#define DH_STREAM_MIN_THRESHOLD 1.2
#define DH_STREAM_INITIAL_CAPACITY 1024 * DH_STREAM_GROWTH_FACTOR
#define DH_BYTE_SIZE sizeof(char)

dh_buffer dh_stream_new(size_t capacity) {
  dh_buffer buffer;
  if (capacity == 0) {
    capacity = DH_STREAM_INITIAL_CAPACITY * DH_BYTE_SIZE;
  }
  buffer.buffer = malloc(capacity);
  CHECK_MALLOC(buffer.buffer);
  buffer.size = 0;
  buffer.capacity = capacity;
  return buffer;
}

void dh_stream_shrink(dh_buffer* buffer) {
  float ratio = (float)buffer->size / (float)buffer->capacity;

  if (ratio > DH_MAX_THRESHOLD) {
    size_t new_capacity = (size_t)(buffer->size * DH_STREAM_GROWTH_FACTOR);
    dh_stream_resize(buffer, new_capacity);
  } else if (ratio < DH_STREAM_MIN_THRESHOLD) {
    size_t new_capacity = (size_t)(buffer->size * DH_STREAM_GROWTH_FACTOR);
    if (new_capacity < buffer->size) {
      new_capacity = buffer->size;
    }
    dh_stream_resize(buffer, new_capacity);
  }
}

void dh_stream_resize(dh_buffer* buffer, size_t new_capacity) {
  buffer->buffer = realloc(buffer->buffer, new_capacity);
  CHECK_MALLOC(buffer->buffer);
  buffer->capacity = new_capacity;
}

void dh_stream_push(dh_buffer* stream, char* data, size_t data_size) {
  if (stream->size + data_size > stream->capacity) {
    size_t new_capacity =
      (size_t)((stream->size + data_size) * DH_STREAM_GROWTH_FACTOR);
    dh_stream_resize(stream, new_capacity);
  }

  memcpy(stream->buffer + stream->size, data, data_size);
  stream->size += data_size;

  dh_stream_shrink(stream);
}

void dh_stream_free(dh_buffer* stream) {
  CHECK_FREED(stream->buffer);
  stream->size = 0;
  stream->capacity = 0;
  stream->buffer = NULL;
}

char* dh_stream_pop(dh_buffer* stream, size_t data_size) {
  char* data = malloc(data_size);
  CHECK_MALLOC(data);
  memcpy(data, stream->buffer, data_size);
  stream->size -= data_size;
  memmove(stream->buffer, stream->buffer + data_size, stream->size);
  return data;
}
