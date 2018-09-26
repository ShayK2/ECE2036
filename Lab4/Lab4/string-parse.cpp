#include <ctype.h>                     //for isspace()
#include <stdio.h>                     //for fgets()
#include "string-parse.h"

// Subroutine "IsDelim" takes two parameters:
// 1) A single character to test
// 2) A string of desired delimiters
// Returns "true" if the character is a delimiter
// Returns "false" otherwise.

bool IsDelim(char ch, const char* delim) {
  if (ch==0) return true;               //End of string is always delimiter
  while(*delim)
    if (*delim++==ch) return true;      //Found, so it is a delimiter
  return false;                         //Not found, so not a delimiter
}

// Subroutine StringParse
// Five parameters:
// 1) The string to parse
// 2) Array of "char*" to return the parsed operand substrings.
// 3) Array of "char" to return the delimiter characters
// 4) Length of the two output arrays.
// 5) String containing the delimiter set
// Returns the number of substrings found
// If a string starts with a '(', the parser returns everything
// up to and including the trailing ')' as a single substring.

int StringParse(char* st, char** subSt, char* term, int len,const char* delim) {

  // First change all whitespace characters to spaces

  char* st1 = st;
  while(*st1) {
    if (isspace(*st1)) *st1 = ' ';      //isspace() function found in <ctype.h>
    st1++;
  }

  // Skip any leading spaces
  
  while(*st == ' ') st++;
  if (*st == 0) return 0;               //Empty string, no substrings

  //Parse

  int i = 0;

  while(i < len) {
    char* start = st;
    bool  parenthesis = (*start == '(');
    if (parenthesis) st++;              //Skip the open parenthesis

    while(1) {
      bool foundDelim = false;
      if (parenthesis) {
        if( IsDelim(*st, ")")) parenthesis = false;
      } else {
        foundDelim = IsDelim(*st, delim);
      }

      if (foundDelim) {                 //Found end of substring
        subSt[i] = start;               //Start of this substring
        term[i] = *st;                  //Save terminator character
        *st++ = 0;                      //Terminate the substring

        // If the delimiter was a space, look ahead for non-space delimiter
        if (term[i]==' ') {
          while(*st==' ') st++;         //Skip any following spaces
          if (IsDelim(*st, delim)) {    //First non-space delimiter, use it
            term[i] = *st;
            *st++ = 0;
          }
        }

        while (*st ==' ') st++;         //Skip any remaining spaces
        if (term[i]==0)                 //End of input reached
          return i+(*subSt[i]!=0);
        i++;                            //Index for next substring and delimiter
        break;                          //Return to outer loop
      }
      else st++;                        // Next character
    }
  }
  return i;  // Out of room in the return array, just return those found
}

int read_inputs(char** operands, char* delim) {
  static char input[101];
  if (fgets(input,sizeof(input),stdin) == NULL) return 0;    //End of input

  //Parse up to 2 operand/delim pairs with StringParse (returns number of pairs)
  return StringParse(input, operands, delim, 2, "+-*/^ ");
}
