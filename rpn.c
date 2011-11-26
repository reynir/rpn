#include <stdio.h>

enum TYPES {
  val,
  op,
  apply,
  eof
};

enum OPERATIONS {
  add,
  sub,
  bogus
};

typedef struct operation_ {
  enum TYPES type;
  int value;
  enum OPERATIONS operation;
} operation;

operation loop(void);
operation read_op(void);

int main(void)
{
  operation res;

  res = loop();

  if (res.type == eof) {
    printf("Result is: %d\n", res.value);
  } else if (res.type == op || res.type == apply) {
    printf("Error: Not enough operands on the stack!\n");
  }

  return 0;
}

operation loop(void)
{
  operation top;
  operation command;

  top = read_op();
  while (1) {
    switch (top.type) {
      case val:
        command = loop();
        switch (command.type) {
          case val:
            break;
          case op:
            command.value = top.value;
            command.type = apply;
            return command;
            break;
          case apply:
            switch (command.operation) {
              case add:
                top.value = top.value + command.value;
                break;
              case sub:
                top.value = top.value - command.value;
                break;
            }
            break;
          case eof:
            top.type = eof;
            return top;
            break;
        }
        break;
      case op:
        return top;
        break;
      case eof:
        return top;
        break;
    }
  }
}

#define SEP '|'

void next_line(void)
{
  while (getchar() != '\n');
}

operation read_op(void)
{
  int i;
  char c;
  operation res;

  c = getchar();
  if (c == EOF || c == '\n') {
    res.type = eof;
  } else if (c == '+') {
    res.type = op;
    res.operation = add;
    next_line();
  } else if (c == '-') {
    res.type = op;
    res.operation = sub;
    next_line();
  } else {
    i = c - '0';
    while ((c = getchar()) != EOF && c != '\n') {
      i = i*10 + c - '0';
    }
    res.type = val;
    res.value = i;
  }

  return res;
}
