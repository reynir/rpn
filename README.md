This is a proof of concept implementation of a simple reverse polish notation
calculator that uses the call stack as operand stack.

[Dan Søndergaard](https://github.com/dansondergaard) gave me the idea. It is
full undefined behaviour, but as long as one gives correct input it should give
correct output.

Currently the supported operators are '+' and '-'. A calculation is terminated by EOF.

Sample usage:

    $ echo 3 1 2 + - | ./rpn
    0
