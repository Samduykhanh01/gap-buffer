#include <stdint.h>
#include <stdlib.h>
#include <string.h>
// #include <sys/types.h>
// #include <unistring/ustring.h>
#include <time.h>
#include <unistr.h>

typedef int BufferPosition;
typedef uint8_t ASCII;
const unsigned int DEBUG = 0;
struct GapBuffer makeGapBuffer(int n_bytes);
void shiftGapTo(struct GapBuffer buff, BufferPosition cursor);
int bufferLength(struct GapBuffer b);
void insertSlice(struct GapBuffer b, BufferPosition cursor, ASCII slice[]);
size_t lengthASCII(ASCII *array);
int min(int a, int b);
int max(int a, int b);

struct GapBuffer {
  ASCII *buf;
  BufferPosition gap_start;
  BufferPosition gap_end;
};

int main(int argc, char *argv[]) {
  makeGapBuffer(16);
  return 0;
}

struct GapBuffer makeGapBuffer(int n_bytes) {
  struct GapBuffer b;
  b.buf = (ASCII *)malloc(n_bytes * sizeof(ASCII));
  b.gap_start = 0;
  b.gap_end = n_bytes;

  /*
  b.gap_start = (struct GapBuffer *)malloc(n_bytes * sizeof(struct GapBuffer));
  b.gap_end = (struct GapBuffer *)malloc(n_bytes * sizeof(struct GapBuffer));
  */

  return b;
}

// Gap mem copy
void shiftGapTo(struct GapBuffer b, BufferPosition cursor) {
  int gap_len = b.gap_end - b.gap_start;
  // int buffLength = (int)sizeof(b.buf) / (int)sizeof(ASCII);

  cursor = min(cursor, bufferLength(b) - gap_len);
  if (cursor == b.gap_start) {
    return;
  }
  // Gap is before cursor
  if (b.gap_start < cursor) {
    // delta is the distance between the cursor and the gap_start when cursor
    // moved to late lines
    unsigned long delta = cursor - b.gap_start;
    memcpy(&b.buf[b.gap_start], &b.buf[b.gap_end], delta);
    b.gap_start += delta;
    b.gap_end += delta;

    // Gap is after cursor
  } else if (b.gap_start > cursor) {

    unsigned long delta = b.gap_start - cursor;
    memcpy(&b.buf[b.gap_end - delta], &b.buf[b.gap_start - delta], delta);
    b.gap_start -= delta;
    b.gap_end -= delta;
  }
}

// Realocation on gap too small
void checkGapSize(struct GapBuffer b, int n_required) {
  int gap_len = b.gap_end - b.gap_start;
  // int buffLength = (int)sizeof(b.buf) / (int)sizeof(ASCII);

  if (gap_len < n_required) {
    shiftGapTo(b, bufferLength(b) - gap_len);
    ASCII newBuf[max(2 * bufferLength(b),
                     n_required + bufferLength(b) - gap_len)];
    memcpy(newBuf, b.buf, bufferLength(b) * sizeof(typeof(b.buf)));
    free(b.buf);
    b.buf = newBuf;
    b.gap_end = bufferLength(b);
  }
}

void insertChar(struct GapBuffer b, BufferPosition cursor, uint8_t c) {
  checkGapSize(b, 1);
  shiftGapTo(b, cursor);
  b.buf[b.gap_start] = c;
  b.gap_start += 1;
}

void insertUnicode(struct GapBuffer b, BufferPosition cursor, uint32_t *u) {
  size_t n = (size_t)sizeof(u) / (size_t)sizeof(uint32_t);
  uint8_t *resultbuf = NULL;
  size_t *lengthp;
  uint8_t *converted = u32_to_u8(u, n, resultbuf, lengthp);

  // checkGapSize(b, lengthp);
  // shiftGapTo(b, cursor);
  insertSlice(b, cursor, converted);
  // b.buf[b.gap_start] = *resultbuf;
  // b.gap_start += 1;
}

void insertSlice(struct GapBuffer b, BufferPosition cursor, ASCII *slice) {
  checkGapSize(b, lengthASCII(slice));
  shiftGapTo(b, cursor);
  memcpy(&b.buf[b.gap_start], slice, lengthASCII(slice));
  b.gap_start += lengthASCII(slice);
}

void insertString(struct GapBuffer b, BufferPosition cursor, char *str) {
  insertSlice(b, cursor, (uint8_t *)str);
}

size_t lengthASCII(ASCII *array) {
  return (size_t)sizeof(array) / (size_t)sizeof(typeof(array));
}

int bufferLength(struct GapBuffer b) {
  return (int)sizeof(b.buf) / (int)sizeof(typeof(b.buf));
}
int min(int a, int b) { return (a <= b) ? a : b; }
int max(int a, int b) { return (a > b) ? a : b; }
