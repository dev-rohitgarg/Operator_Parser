Compilation:
g++ parser.cpp -o parser

Execution:
parser

2+3$4 is showing valid (need to fix)

(2+3)(4+5) is not accepted by my code, (2+3)\*(4+5) is accepted

Note: don't add space in the input string.
This is because a string like 4 4 is considered valid in my code, only the first 4 is read into the input buffer.
