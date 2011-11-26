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
  /* This represents the top of the stack */
  operation top;
  /* This is the command received from a recursive call */
  operation command;

  top = read_op();
  while (1) {
    switch (top.type) {
      /* "Put" the new value on top of the stack and call recursively */
      case val:
        command = loop();
        switch (command.type) {
          /* We received an operator, return a "partial application" */
          case op:
            command.value = top.value;
            command.type = apply;
            return command;
            break;
          /* We received a partial application (operator+value). Compute the
           * result and store it on the top of the stack */
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
          /* End of file received! return the top of the stack. Note that the
           * inital caller will only see the bottom value of the stack */
          case eof:
            top.type = eof;
            return top;
            break;
        }
        break;
      /* We read an operator. Return it so someone else can handle it */
      case op:
        return top;
        break;
      /* We read the end of file. Return it so we can return the result */
      case eof:
        return top;
        break;
    }
  }
}

/* Dirty hack to skip to next newline */
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
    while ((c = getchar()) != EOF && isdigit(c)) {
      i = i*10 + c - '0';
    }
    res.type = val;
    res.value = i;
    if (c != '\n')
      next_line();
  }

  return res;
}
