#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef int BufferPosition;
typedef uint8_t ASCII;
const unsigned int DEBUG = 0;
struct GapBuffer makeGapBuffer(int n_bytes);
void shiftGapTo(struct GapBuffer buff, BufferPosition cursor);
int min(int a, int b);

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

void shiftGapTo(struct GapBuffer g, BufferPosition cursor) {
  int gap_len = g.gap_end - g.gap_start;
  int buffLength = (int)sizeof(g.buf) / (int)sizeof(ASCII);
  if (cursor == g.gap_start) {
    return;
  }
  cursor = min(cursor, buffLength - gap_len);

  if (g.gap_start < cursor) {
    // Gap is before cursor
  } else if () {
    // Gap is after cursor
  }
}

int min(int a, int b) { return (a <= b) ? a : b; }
