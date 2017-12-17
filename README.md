# Brainfuck Translator

This translator is inspired by a [2kyu problem][1] in Codewars.

## Feature

- Translate brainfuck code to C from a file.

- Translate brainfuck code in command line mode.

- Execute brainfuck code in a file.

- Execute brainfuck code in command line mode.

## Usage

1. Execute `make.sh` to make a executable binary file.

2. Enjoy it as the following instruction!

Usage:  ./brainfuck
  or:   ./brainfuck [OPTION]... FILE
Execute or translate Brainfuck code. With no arguments,
code shoule be enter in Shell Mode.

OPTION:
  -e, -execute      execute the Brainfuck code in FILE (default)
  -t, -translate    translate the code in FILE into C
  -h, -help         display this help and exit
  -v, -version      display the version of the program and exit

## Shell Mode

Usage: OPRATE CODE

OPARATE:
  exec    execute Brainfuck code
  trans   translate Brainfuck code into C
  help    display this help
  exit    exit Shell (followed by nothing)

## WARNING

This version takes a memory leak! The comment at src/shell.c:102 points this problem.

[1]:https://www.codewars.com/kata/58924f2ca8c628f21a0001a1
