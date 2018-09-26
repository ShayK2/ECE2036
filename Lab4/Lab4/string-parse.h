#ifndef _STRING_PARSE_H
#define _STRING_PARSE_H

// Subroutine read_inputs
// Two parameters:
// 1) Array (size 2) of "char*" to return the parsed operand substrings.
// 2) Array (size 2) of "char" to return the operator/delimiter characters
// This function reads an input string from the standard input, parses it into
// alternating operand substrings and operator/delimiter characters, and returns
// the number of operand/delimiter pairs. Operands starting with an '(' will be
// parsed into operand substrings which contain the opening parenthesis as well
// as all intermediate characters up to and including the trailing ')'.

int read_inputs(char* operands[2], char delim[2]);

#endif
