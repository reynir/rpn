#include <stdio.h>

/* Types used in the lexer */

typedef enum {
  TOKEN_VAL,
  TOKEN_OP,
  TOKEN_EOF
} TOKEN;

typedef enum {
  OPERATION_ADD,
  OPERATION_SUB,
} OPERATION;

typedef struct {
  TOKEN type;
  union {
    int value;
    OPERATION operation;
  } dat;
} token;

/* Types used in the RPN evaluator */

typedef enum {
  RESULT_OP,
  RESULT_PARTIAL,
  RESULT_VALUE
} RESULT;

typedef struct {
  OPERATION op;
  int v1;
} partial;

typedef struct {
  RESULT type;
  union {
    int value;
    OPERATION operation;
    partial partial;
  } dat;
} result;

result loop(void);
token read_op(void);

int main(void)
{
  result res;

  res = loop();

  if (res.type == RESULT_VALUE) {
    printf("Result is: %d\n", res.dat.value);
  } else if (res.type == RESULT_OP || res.type == RESULT_PARTIAL) {
    printf("Error: Not enough operands on the stack!\n");
  }

  return 0;
}

result loop(void)
{
  /* This will represent the top of the stack */
  token top;
  /* This is the command received from a recursive call */
  result command;

  top = read_op();
  while (1) {
    switch (top.type) {
      /* "Put" the new value on top of the stack and call recursively */
      case TOKEN_VAL:
        command = loop();
        switch (command.type) {
          /* We received an operator, return a "partial application" */
          case RESULT_OP:
            {
              OPERATION operation = command.dat.operation;
              command.dat.partial.v1 = top.dat.value;
              command.dat.partial.op = operation;
              command.type = RESULT_PARTIAL;
            }
            return command;
            break;
	  /* We received a partial application (operator and value). Compute
	   * the result and store it on the top of the stack */
          case RESULT_PARTIAL:
            switch (command.dat.partial.op) {
              case OPERATION_ADD:
                top.dat.value = top.dat.value + command.dat.partial.v1;
                break;
              case OPERATION_SUB:
                top.dat.value = top.dat.value - command.dat.partial.v1;
                break;
            }
            break;
          /* End of file received! return the top of the stack. Note that the
           * inital caller will only see the bottom value of the stack. This is
           * okay since on any well formed input the stack will have size one.
           */
          case RESULT_VALUE:
            command.dat.value = top.dat.value;
            return command;
            break;
        }
        break;
      /* We read an operator. Return it so someone else can handle it */
      case TOKEN_OP:
        command.type = RESULT_OP;
        command.dat.operation = top.dat.operation;
        return command;
        break;
      /* We read the end of file. Return it so we can return the result. Note
       * that command.dat.value is undefined. */
      case TOKEN_EOF:
        command.type = RESULT_VALUE;
        return command;
        break;
    }
  }
}

token read_op(void)
{
  int i;
  char c;
  token res;

  while (1) {
    c = getchar();
    if (c == EOF) {
      res.type = TOKEN_EOF;
    } else if (c == '+') {
      res.type = TOKEN_OP;
      res.dat.operation = OPERATION_ADD;
    } else if (c == '-') {
      res.type = TOKEN_OP;
      res.dat.operation = OPERATION_SUB;
    } else if (isdigit(c)) {
      i = c - '0';
      while ((c = getchar()) != EOF && isdigit(c)) {
        i = i*10 + c - '0';
      }
      res.type = TOKEN_VAL;
      res.dat.value = i;
    } else {
      continue;
    }
    return res;
  }
}
