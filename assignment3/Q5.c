#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void *xmalloc(size_t n) {
  void *p = malloc(n);
  if (!p) {
    fprintf(stderr, "Out of memory: malloc(%zu) failed\n", n);
    exit(EXIT_FAILURE);
  }
  return p;
}

static void *xrealloc(void *ptr, size_t n) {
  void *p = realloc(ptr, n);
  if (!p && n != 0) {
    fprintf(stderr, "Out of memory: realloc(%zu) failed\n", n);
    exit(EXIT_FAILURE);
  }
  return p;
}

typedef struct {
  char **lines;
  size_t size;
  size_t capacity;
} Editor;

#define INITIAL_CAPACITY 4

char *read_line_stream(FILE *stream) {
  size_t bufcap = 128;
  size_t len = 0;
  char *buf = xmalloc(bufcap);
  int c;

  while ((c = fgetc(stream)) != EOF) {
    if (c == '\r')
      continue;
    if (c == '\n')
      break;
    if (len + 1 >= bufcap) {
      bufcap *= 2;
      buf = xrealloc(buf, bufcap);
    }
    buf[len++] = (char)c;
  }

  if (len == 0 && c == EOF) {
    free(buf);
    return NULL;
  }

  buf[len] = '\0';
  return xrealloc(buf, len + 1);
}

char *read_line_stdin(const char *prompt) {
  if (prompt) {
    printf("%s", prompt);
    fflush(stdout);
  }
  return read_line_stream(stdin);
}

void initEditor(Editor *ed) {
  ed->size = 0;
  ed->capacity = INITIAL_CAPACITY;
  ed->lines = xmalloc(ed->capacity * sizeof(char *));
}

void freeAll(Editor *ed) {
  if (!ed)
    return;
  for (size_t i = 0; i < ed->size; i++) {
    free(ed->lines[i]);
  }
  free(ed->lines);
  ed->lines = NULL;
  ed->size = 0;
  ed->capacity = 0;
}

void ensureCapacity(Editor *ed, size_t min_capacity) {
  if (ed->capacity >= min_capacity)
    return;
  size_t newcap = ed->capacity ? ed->capacity : 1;
  while (newcap < min_capacity)
    newcap *= 2;
  ed->lines = xrealloc(ed->lines, newcap * sizeof(char *));
  ed->capacity = newcap;
}

void shrinkToFit(Editor *ed) {
  if (ed->size == ed->capacity)
    return;
  if (ed->size == 0) {
    free(ed->lines);
    ed->lines = xmalloc(INITIAL_CAPACITY * sizeof(char *));
    ed->capacity = INITIAL_CAPACITY;
    return;
  }
  ed->lines = xrealloc(ed->lines, ed->size * sizeof(char *));
  ed->capacity = ed->size;
}

void insertLine(Editor *ed, size_t index, const char *text) {
  if (index > ed->size) {
    fprintf(stderr, "insertLine: index out of range\n");
    return;
  }

  ensureCapacity(ed, ed->size + 1);

  if (index < ed->size) {
    memmove(&ed->lines[index + 1], &ed->lines[index],
            (ed->size - index) * sizeof(char *));
  }

  char *copy = xmalloc(strlen(text) + 1);
  strcpy(copy, text);

  ed->lines[index] = copy;
  ed->size++;
}

void deleteLine(Editor *ed, size_t index) {
  if (ed->size == 0) {
    fprintf(stderr, "deleteLine: buffer empty\n");
    return;
  }
  if (index >= ed->size) {
    fprintf(stderr, "deleteLine: index out of range\n");
    return;
  }

  free(ed->lines[index]);

  if (index + 1 < ed->size) {
    memmove(&ed->lines[index], &ed->lines[index + 1],
            (ed->size - index - 1) * sizeof(char *));
  }

  ed->size--;
  ed->lines[ed->size] = NULL;
}

void appendLine(Editor *ed, const char *text) {
  insertLine(ed, ed->size, text);
}

void printAllLines(const Editor *ed) {
  printf("---- Buffer: %zu line(s) ----\n", ed->size);
  for (size_t i = 0; i < ed->size; i++) {
    printf("%4zu: %s\n", i + 1, ed->lines[i]);
  }
  printf("---- end ----\n");
}

int saveToFile(const Editor *ed, const char *filename) {
  FILE *f = fopen(filename, "w");
  if (!f) {
    fprintf(stderr, "Failed to open %s\n", filename);
    return -1;
  }

  for (size_t i = 0; i < ed->size; i++) {
    if (fprintf(f, "%s\n", ed->lines[i]) < 0) {
      fprintf(stderr, "Write error on %s\n", filename);
      fclose(f);
      return -1;
    }
  }

  fclose(f);
  return 0;
}

int loadFromFile(Editor *ed, const char *filename) {
  FILE *f = fopen(filename, "r");
  if (!f) {
    fprintf(stderr, "Failed to open %s\n", filename);
    return -1;
  }

  for (size_t i = 0; i < ed->size; i++) {
    free(ed->lines[i]);
  }
  ed->size = 0;

  ensureCapacity(ed, 1);

  char *line;
  while ((line = read_line_stream(f)) != NULL) {
    ensureCapacity(ed, ed->size + 1);
    ed->lines[ed->size++] = line;
  }

  fclose(f);
  return 0;
}

static void print_menu(void) {
  puts("\nCommands:");
  puts("  a  - append line");
  puts("  i  - insert line");
  puts("  d  - delete line");
  puts("  p  - print all");
  puts("  s  - save to file");
  puts("  l  - load from file");
  puts("  f  - free all");
  puts("  r  - shrinkToFit");
  puts("  q  - quit");
}

size_t parse_index_input(const char *prompt, size_t max_allowed_plus_one) {
  char *line = read_line_stdin(prompt);
  if (!line)
    return SIZE_MAX;

  char *endptr;
  long v = strtol(line, &endptr, 10);
  free(line);

  if (endptr == NULL || *endptr != '\0' || v < 1) {
    fprintf(stderr, "Invalid index\n");
    return SIZE_MAX;
  }

  return (size_t)(v - 1);
}

void print_memory_summary(const Editor *ed) {
  size_t pointers_bytes = ed->capacity * sizeof(char *);
  size_t strings_bytes = 0;
  for (size_t i = 0; i < ed->size; i++)
    strings_bytes += strlen(ed->lines[i]) + 1;

  printf("Memory summary: %zu pointers (capacity %zu) = %zu bytes, "
         "strings ~%zu bytes\n",
         ed->size, ed->capacity, pointers_bytes, strings_bytes);
}

int main(void) {
  Editor ed;
  initEditor(&ed);

  puts("Minimal Text Editor (no errno)");

  for (;;) {
    print_menu();
    printf("Enter command: ");
    fflush(stdout);

    int cmd = getchar();
    if (cmd == EOF)
      break;

    while (cmd == '\n')
      cmd = getchar();
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }

    switch (cmd) {
    case 'a': {
      char *text = read_line_stdin("Enter line: ");
      if (!text)
        break;
      appendLine(&ed, text);
      free(text);
      print_memory_summary(&ed);
      break;
    }

    case 'i': {
      printf("Current lines: %zu\n", ed.size);
      size_t idx = parse_index_input("Index: ", ed.size + 1);
      if (idx == SIZE_MAX)
        break;

      char *text = read_line_stdin("Enter line: ");
      if (!text)
        break;

      insertLine(&ed, idx, text);
      free(text);
      print_memory_summary(&ed);
      break;
    }

    case 'd': {
      if (ed.size == 0) {
        puts("Buffer empty.");
        break;
      }
      size_t idx = parse_index_input("Index to delete: ", ed.size);
      if (idx == SIZE_MAX || idx >= ed.size)
        break;

      deleteLine(&ed, idx);
      print_memory_summary(&ed);
      break;
    }

    case 'p':
      printAllLines(&ed);
      break;

    case 's': {
      char *fname = read_line_stdin("Save filename: ");
      if (!fname)
        break;
      saveToFile(&ed, fname);
      free(fname);
      break;
    }

    case 'l': {
      char *fname = read_line_stdin("Load filename: ");
      if (!fname)
        break;
      loadFromFile(&ed, fname);
      free(fname);
      print_memory_summary(&ed);
      break;
    }

    case 'f':
      freeAll(&ed);
      initEditor(&ed);
      puts("Buffer cleared.");
      break;

    case 'r':
      shrinkToFit(&ed);
      puts("Shrink complete.");
      print_memory_summary(&ed);
      break;

    case 'q':
      freeAll(&ed);
      return 0;

    default:
      puts("Unknown command.");
    }
  }

  freeAll(&ed);
  return 0;
}
