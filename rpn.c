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
operation read(void);

int idx = 0;
operation ops[10];

int main(void)
{
  operation res;
  ops[0].type = val;
  ops[0].value = 2;
  ops[0].operation = bogus;

  ops[1].type = val;
  ops[1].value = 2;
  ops[1].operation = bogus;

  ops[2].type = op;
  ops[2].value = 0;
  ops[2].operation = add;

  ops[3].type = eof;
  ops[3].value = 0;
  ops[3].operation = bogus;

  res = loop();

  if (res.type == eof)
    printf("Result is: %d\n", res.value);

  return 0;
}

operation loop(void)
{
  operation top;
  operation command;

  top = read();
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

operation read(void)
{
  return ops[idx++];
}
