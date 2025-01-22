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
    string expression;  // 存储用户输入的表达式
    bool check_if_available();  // 检查表达式是否合法
public:
    Calculate(string input_ = "No input") : expression(input_) {
        // 删除空白字符
        expression.erase(std::remove_if(expression.begin(), expression.end(), ::isspace), expression.end());
    }
    double calculate_result();  // 计算表达式的结果
};

// 检查字符是否是运算符
inline bool check_op(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^';
}

// 检查字符是否是括号
inline bool check_Parentheses(char ch) {
    return ch == '(' || ch == ')';
}

// 根据运算符计算两个操作数的结果
double cal(char op, double a, double b) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return pow(a, b);
        default: return 0;  // 默认返回 0
    }
}

// 执行栈操作：弹出运算符和操作数，计算结果并压入栈中
void Stack_operation(stack<double>& numstack, stack<char>& opstack) {
    char op = opstack.top();  // 弹出栈顶运算符
    opstack.pop();
    double num1 = numstack.top(); numstack.pop();  // 弹出第一个操作数
    double num2 = numstack.top(); numstack.pop();  // 弹出第二个操作数
    numstack.push(cal(op, num2, num1));  // 计算结果并压入栈中
}

// 检查表达式是否合法
bool Calculate::check_if_available() {
    if (expression == "No input") {
        cout << "Please input a string!" << endl;
        return false;
    }

    int size = expression.length();
    stack<char> parentheses;  // 用于检查括号匹配

    for (int i = 0; i < size; i++) {
        char ch = expression[i];

        // 检查括号
        if (check_Parentheses(ch)) {
            if (ch == '(') {
                parentheses.push('(');  // 左括号入栈
            } else {
                if (parentheses.empty() || parentheses.top() != '(') {
                    cout << "Parentheses match fault!" << endl;
                    return false;
                }
                parentheses.pop();  // 右括号匹配成功，弹出左括号
            }
        }

        // 检查运算符
        else if (check_op(ch)) {
            if (i == 0 || i == size - 1) {
                cout << "Operation Error!" << endl;
                return false;
            }

            char prev = expression[i - 1];  // 运算符前一个字符
            char next = expression[i + 1];  // 运算符后一个字符
            if (!((isdigit(prev) || prev == ')') && (isdigit(next) || next == '('))) {
                cout << "Operation Error!" << endl;
                return false;
            }
        }

        // 检查非法字符
        else if (!isdigit(ch) && ch != '.') {
            cout << "INVALID LETTERS" << endl;
            return false;
        }
    }

    // 检查括号是否全部匹配
    if (!parentheses.empty()) {
        cout << "Parentheses match fault!" << endl;
        return false;
    }

    return true;
}

// 计算表达式的结果
double Calculate::calculate_result() {
    if (!this->check_if_available()) {
        cout << "Wrong!" << endl;
        exit(1);
    }

    stack<char> opstack;  // 运算符栈
    stack<double> numstack;  // 操作数栈
    int size = expression.length();

    for (int i = 0; i < size; i++) {
        char ch = expression[i];

        // 处理数字和小数点
        if (isdigit(ch) || ch == '.') {
            stringstream ss;
            while (i < size && (isdigit(expression[i]) || expression[i] == '.')) {
                ss << expression[i++];  // 将数字和小数点拼接到字符串流中
            }
            i--;  // 回退一步，因为外层循环会再增加一次 i
            double num;
            ss >> num;  // 将字符串流转换为浮点数
            numstack.push(num);  // 将数字压入操作数栈
        }

        // 处理运算符和括号
        else {
            switch (ch) {
                case '(':
                    opstack.push(ch);  // 左括号直接入栈
                    break;

                case ')':
                    // 弹出栈顶运算符并计算，直到遇到左括号
                    while (!opstack.empty() && opstack.top() != '(') {
                        Stack_operation(numstack, opstack);
                    }
                    if (!opstack.empty() && opstack.top() == '(') {
                        opstack.pop();  // 弹出左括号
                    } else {
                        cout << "Parentheses match fault!" << endl;
                        exit(1);
                    }
                    break;

                case '^':
                    // 乘方运算符优先级最高，直接入栈
                    opstack.push(ch);
                    break;

                case '*':
                case '/':
                    // 处理乘法和除法运算符
                    while (!opstack.empty() && opstack.top() != '(' &&
                        (opstack.top() == '^' || opstack.top() == '*' || opstack.top() == '/')) {
                        Stack_operation(numstack, opstack);
                    }
                    opstack.push(ch);  // 当前运算符入栈
                    break;

                case '+':
                case '-':
                    // 处理加法和减法运算符
                    while (!opstack.empty() && opstack.top() != '(' &&
                        (opstack.top() == '^' || opstack.top() == '*' || opstack.top() == '/' ||
                            opstack.top() == '+' || opstack.top() == '-')) {
                        Stack_operation(numstack, opstack);
                    }
                    opstack.push(ch);  // 当前运算符入栈
                    break;

                default:
                    // 非法字符（已经在 check_if_available 中检查过，这里不会执行）
                    break;
            }
        }
    }

    // 清空栈中剩余的运算符
    while (!opstack.empty()) {
        Stack_operation(numstack, opstack);
    }

    // 返回最终结果
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

