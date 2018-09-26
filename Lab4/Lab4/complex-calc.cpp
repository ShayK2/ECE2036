#include <iostream>
#include "complex.h"
#include "string-parse.h"
using namespace std;
#include <cstdlib>

int main() {
  int count;
  char* operands[2];
  char delim[2];
  Complex ans = Complex(0, 0);
  Complex operand1;
  Complex operand2;

  while(1) {
    count = read_inputs(operands, delim);

    if (count == 1) {
        if (!operands[0]) {
            operand2 = Complex(operands[1]);
            switch(delim[0]) {
                case '+': ans = ans + operand2;
                    break;
                case '-': ans = ans - operand2;
                    break;
                case '*': ans = ans * operand2;
                    break;
                case '/': ans = ans / operand2;
                    break;
            }
        } else {
            if (delim[0]) {
                std::cout << "invalid expression\n";
                continue;
            } else {
                operand1 = Complex(operands[0]);
                ans = operand1;
            }
        }
    } else if (count == 2) {
        if (operands[0]) {
            operand2 = Complex(operands[1]);
            switch(delim[0]) {
                case '+': ans = ans + operand2;
                    break;
                case '-': ans = ans - operand2;
                    break;
                case '*': ans = ans * operand2;
                    break;
                case '/': ans = ans / operand2;
                    break;
            }
        } else {
            operand1 = Complex(operands[0]);
            operand2 = Complex(operands[1]);
            switch(delim[0]) {
                case '+': ans = operand1 + operand2;
                    break;
                case '-': ans = operand1 - operand2;
                    break;
                case '*': ans = operand1 * operand2;
                    break;
                case '/': ans = operand1 / operand2;
                    break;
              }
        }
    } else {
        break;
    }
      std::cout << ans << "\n";
  }
    return 0;
}
