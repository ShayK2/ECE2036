#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>

using namespace std;

const string operators = "+-/*^";
const string numberChars = "0123456789.";

// Akshay Karthik
// ECE 2036 A
// Lab 1 - 2/2/18

class ComputeLine {
public:
    
    // Constructor for ComputeLine object, setting private data to default (empty) values
    
    ComputeLine() {
        operand1 = "";
        operand2 = "";
        numValidOperand = 0;
        operatorType = ' ';
        divideByZeroFlag = false;
        ans = 0;
    }
    
    // Getters and setters for private data
    
    void setOperand1(string op) {
        operand1 = op;
    }
    
    string getOperand1() {
        return operand1;
    }
    
    void setOperand2(string op) {
        operand2 = op;
    }
    
    string getOperand2() {
        return operand2;
    }
    
    void setNumValidOperand(int num) {
        numValidOperand = num;
    }
    
    int getNumValidOperand() {
        return numValidOperand;
    }
    
    void setOperatorType(char opType) {
        operatorType = opType;
    }
    
    char getOperatorType() {
        return operatorType;
    }
    
    void setDivideByZeroFlag(bool flag) {
        divideByZeroFlag = flag;
    }
    
    bool getDivideByZeroFlag() {
        return divideByZeroFlag;
    }
    
    void setAns(float answer) {
        ans = answer;
    }
    
    float getAns() {
        return ans;
    }
    
    // Calculates the output based on the number of valid operands and the given operation.
    
    void calculateAns() {
        float val1 = atof(operand1.c_str());
        float val2 = atof(operand2.c_str());
        
        if (numValidOperand == 0) {
            cout << "Invalid expression" << endl;

        // If there is one operand, we change the value of ans based on the operation, or set
        // ans to that value if there is no operand.
            
        } else if (numValidOperand == 1) {
            if (operand1.empty()) {
                 switch(operatorType) {
                     case '+':
                         ans = ans + val2;
                         cout << setw(8);
                         cout << "= " << ans << endl;
                         break;
            
                     case '-':
                         ans = ans - val2;
                         cout << setw(8);
                         cout << "= " << ans << endl;
                         break;
            
                     case '*':
                         ans = ans * val2;
                         cout << setw(8);
                         cout << "= " << ans << endl;
                         break;
            
                     case '/':
                         if (val2 == 0) {
                             divideByZeroFlag = true;
                             cout << setw(8);
                             cout << "Divide by zero error" << endl;
                         } else {
                             ans = ans / val2;
                             cout << setw(8);
                             cout << "= " << ans << endl;
                         }
                         break;
            
                     case '^':
                         int newNum = ans;
                         for (int i = 1; i < val2; i++) {
                             newNum = newNum * ans;
                         }
                         ans = newNum;
                         cout << setw(8);
                         cout << "= " << ans << endl;
                         break;
                 }
             } else {
                 ans = val1;
                 cout << setw(8);
                 cout << "= " << ans << endl;
             }
            
        // If there are two operands, we perform the operation on them and output the result
            
        } else if (numValidOperand == 2) {
            switch(operatorType) {
                case '+':
                    ans = val1 + val2;
                    cout << setw(8);
                    cout << "= " << ans << endl;
                    break;
                    
                case '-':
                    ans = val1 - val2;
                    cout << setw(8);
                    cout << "= " << ans << endl;
                    break;
                    
                case '*':
                    ans = val1 * val2;
                    cout << setw(8);
                    cout << "= " << ans << endl;
                    break;
                    
                case '/':
                    if (val2 == 0) {
                        divideByZeroFlag = true;
                        cout << setw(8);
                        cout << "Divide by zero error" << endl;
                    } else {
                        ans = val1 / val2;
                        cout << setw(8);
                        cout << "= " << ans << endl;
                    }
                    break;
                    
                case '^':
                    ans = 1;
                    for (int i = 0; i < val2; i++) {
                        ans = ans * val1;
                    }
                    cout << setw(8);
                    cout << "= " << ans << endl;
                    break;
            }
        }
    }
    
    // Reads in user input
    
    bool readInputs() {
        string input;
        getline(cin, input);
        return (StringParse(input));
    }
    
    // Parses the user input into operands and operations
    
    bool StringParse(string input) {
        int posOp = input.find_first_of(operators);
        int posNum = input.find_first_of(numberChars);
        int exitProgram = false;
        
        //clear all operands and reset flags
        operand1.clear();
        operand2.clear();
        numValidOperand = 0;
        divideByZeroFlag = false;
        
        operatorType = input[posOp]; //if posOp is npos this should return null character
        
        if ((posOp == string::npos)&&(posNum == string::npos)) { //if there is no operator AND no numbers
            operatorType = 0;
            numValidOperand = 0;
            exitProgram = true;
        } else if ((posOp == string::npos)&&(posNum != string::npos)) { //if no operator and only numbers
            operatorType = 0;  //just making sure it is null
            operand1 = input.substr(posNum, string::npos);
            numValidOperand = 1;
        } else if ((posOp != string::npos)&&(posNum == string::npos)) { //if operator and no numbers
            numValidOperand = 0; //this should trigger an invalid expression
        } else if (posOp < posNum) { //then operator is before the posNum I don't check for two numbers
            operand2 = input.substr(posNum, string::npos);
            numValidOperand = 1;
        } else if (posOp > posNum) { //then there is at least one number BEFORE operator
            operand1 = input.substr(posNum, posOp-posNum);
            operand2 = input.substr(posOp+1, string::npos); //there may or may not be a number here
            numValidOperand = 2;
            
            if (operand2.find_first_of(numberChars) == string::npos) { //if no numbers after then invalid
                operand2.clear(); //then make an emptry string
                operatorType = 0;
                numValidOperand = 0;
            }
        }
        return exitProgram;
    }
    
private:
    string operand1;
    string operand2;
    int numValidOperand;
    char operatorType;
    bool divideByZeroFlag;
    float ans;
};

int main() {
    ComputeLine CurrentLine;
    bool GETOUT = false;
    
    // As long as the user continues to provide valid operations, the calculator continues
    
    while(!GETOUT) {
        if (!(GETOUT = CurrentLine.readInputs())) {
            CurrentLine.calculateAns();
        }
    }
    return 0;
}
