# Program interpreter for a LISP-like language

Interpreter of a LISP-like language containing definitions of variables, instructions of input/output, conditional choices and loops, limited only to operations between integer variables.

## Note

- Keywords are written in capital letters
- N→α, N→β are abbreviated as N → α | β
- ε denotes the empty string
- Italic characters indicate non-terminal symbols
- normal characters indicate symbols terminals

## Input format

*program → stmt block*
*stmt block → statement* | ( BLOCK *statement list* )
*statement list* → *statement statement list | statement*
*statement → variable def* |
*io stmt* |
*cond stmt* |
*loop stmt*
*variable def* → ( SET *variable id num expr* )
*io stmt* → ( PRINT *num expr* ) | ( INPUT *variable id* )
*cond stmt* → ( IF *bool expr stmt block stmt block* )
*loop stmt* → ( WHILE *bool expr stmt block* )
*num expr* → ( ADD *num expr num expr* )
| ( SUB *num expr num expr* )
| ( MUL *num expr num expr* )
| ( DIV *num expr num expr* )
| *number*
| *variable id*
*bool expr* | (LT *num expr num expr*)
| (GT *num expr num expr*)
| (EQ *num expr num expr*)
| (AND *bool expr bool expr*)
| (OR *bool expr bool expr*)
| (NOT *bool expr*)
| TRUE | FALSE
*variable id → alpha list*
*alpha list → alpha alpha list | alpha*
*alpha* → a | b | c | . . . | z | A | B | C | . . . | Z
*number → - posnumber* | *posnumber*
*posnumber* → 0 | *sigdigit rest*
*sigdigit* → 1 | . . . | 9
*rest → digit rest* | ε
*digit* → 0 | *sigdigit*

## Instructions

The program code is located in the Code directory. The lispInterpreter.cpp file contains the main. The program must be run from the command line passing as a parameter the .txt file containing the grammar to be interpreted. Example files are present in the Examples directory