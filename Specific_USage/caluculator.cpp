#include <iostream>
#include <cstring>
#include <stack>
#include <cctype>
#include <algorithm>
#include <sstream>
#include <cmath>
using namespace std;

class Calculate {
private:
    string expression;  // Store the user input expression
    bool check_if_available();  // Check if the expression is valid
public:
    Calculate(string input_ = "No input") : expression(input_) {
        // Remove whitespace characters
        expression.erase(std::remove_if(expression.begin(), expression.end(), ::isspace), expression.end());
    }
    double calculate_result();  // Calculate the result of the expression
};

// Check if a character is an operator
inline bool check_op(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^';
}

// Check if a character is a parenthesis
inline bool check_Parentheses(char ch) {
    return ch == '(' || ch == ')';
}

// Calculate the result of two operands based on the operator
double cal(char op, double a, double b) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return pow(a, b);
        default: return 0;  // Default return 0
    }
}

// Perform stack operation: Pop operator and operands, calculate the result, and push it back to the stack
void Stack_operation(stack<double>& numstack, stack<char>& opstack) {
    char op = opstack.top();  // Pop the top operator
    opstack.pop();
    double num1 = numstack.top(); numstack.pop();  // Pop the first operand
    double num2 = numstack.top(); numstack.pop();  // Pop the second operand
    numstack.push(cal(op, num2, num1));  // Calculate the result and push it back to the stack
}

// Check if the expression is valid
bool Calculate::check_if_available() {
    if (expression == "No input") {
        cout << "Please input a string!" << endl;
        return false;
    }

    int size = expression.length();
    stack<char> parentheses;  // Used to check parenthesis matching

    for (int i = 0; i < size; i++) {
        char ch = expression[i];

        // Check parentheses
        if (check_Parentheses(ch)) {
            if (ch == '(') {
                parentheses.push('(');  // Push left parenthesis onto the stack
            } else {
                if (parentheses.empty() || parentheses.top() != '(') {
                    cout << "Parentheses match fault!" << endl;
                    return false;
                }
                parentheses.pop();  // Right parenthesis matched successfully, pop the left parenthesis
            }
        }

        // Check operators
        else if (check_op(ch)) {
            if (i == 0 || i == size - 1) {
                cout << "Operation Error!" << endl;
                return false;
            }

            char prev = expression[i - 1];  // Character before the operator
            char next = expression[i + 1];  // Character after the operator
            if (!((isdigit(prev) || prev == ')') && (isdigit(next) || next == '('))) {
                cout << "Operation Error!" << endl;
                return false;
            }
        }

        // Check for invalid characters
        else if (!isdigit(ch) && ch != '.') {
            cout << "INVALID LETTERS" << endl;
            return false;
        }
    }

    // Check if all parentheses are matched
    if (!parentheses.empty()) {
        cout << "Parentheses match fault!" << endl;
        return false;
    }

    return true;
}

// Calculate the result of the expression
double Calculate::calculate_result() {
    if (!this->check_if_available()) {
        cout << "Wrong!" << endl;
        exit(1);
    }

    stack<char> opstack;  // Operator stack
    stack<double> numstack;  // Operand stack
    int size = expression.length();

    for (int i = 0; i < size; i++) {
        char ch = expression[i];

        // Handle digits and decimal points
        if (isdigit(ch) || ch == '.') {
            stringstream ss;
            while (i < size && (isdigit(expression[i]) || expression[i] == '.')) {
                ss << expression[i++];  // Concatenate digits and decimal points into the string stream
            }
            i--;  // Step back once because the outer loop will increment i again
            double num;
            ss >> num;  // Convert the string stream to a floating-point number
            numstack.push(num);  // Push the number onto the operand stack
        }

        // Handle operators and parentheses
        else {
            switch (ch) {
                case '(':
                    opstack.push(ch);  // Push left parenthesis directly onto the stack
                    break;

                case ')':
                    // Pop the top operator and calculate until a left parenthesis is encountered
                    while (!opstack.empty() && opstack.top() != '(') {
                        Stack_operation(numstack, opstack);
                    }
                    if (!opstack.empty() && opstack.top() == '(') {
                        opstack.pop();  // Pop the left parenthesis
                    } else {
                        cout << "Parentheses match fault!" << endl;
                        exit(1);
                    }
                    break;

                case '^':
                    // Exponentiation operator has the highest priority, push directly onto the stack
                    opstack.push(ch);
                    break;

                case '*':
                case '/':
                    // Handle multiplication and division operators
                    while (!opstack.empty() && opstack.top() != '(' &&
                        (opstack.top() == '^' || opstack.top() == '*' || opstack.top() == '/')) {
                        Stack_operation(numstack, opstack);
                    }
                    opstack.push(ch);  // Push the current operator onto the stack
                    break;

                case '+':
                case '-':
                    // Handle addition and subtraction operators
                    while (!opstack.empty() && opstack.top() != '(' &&
                        (opstack.top() == '^' || opstack.top() == '*' || opstack.top() == '/' ||
                            opstack.top() == '+' || opstack.top() == '-')) {
                        Stack_operation(numstack, opstack);
                    }
                    opstack.push(ch);  // Push the current operator onto the stack
                    break;

                default:
                    // Invalid character (already checked in check_if_available, won't execute here)
                    break;
            }
        }
    }

    // Clear remaining operators in the stack
    while (!opstack.empty()) {
        Stack_operation(numstack, opstack);
    }

    // Return the final result
    return numstack.top();
}

int main() {
    string inputstr;
    while (true) {
        cout << "Enter the expression (or 'q' to quit): " << endl;
        getline(cin, inputstr);
        if (inputstr == "q") break;
        Calculate test(inputstr);
        cout << "Result: " << test.calculate_result() << endl;
    }
    return 0;
}