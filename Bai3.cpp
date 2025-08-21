#include <iostream>
#include <stack>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <iomanip>
using namespace std;

bool isOperator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int precedence(char op)
{
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

queue<string> chuyendoi(const string &expression) {
    string simplifiedExpression;
    for (size_t i = 0; i < expression.size(); ++i) {
        char c = expression[i];
        if (c == '+' || c == '-') {
            int sign = (c == '-') ? -1 : 1;
            while (i + 1 < expression.size() &&
                   (expression[i + 1] == '+' || expression[i + 1] == '-')) {
                c = expression[++i];
                if (c == '-') sign *= -1;
            }
            simplifiedExpression += (sign == 1) ? '+' : '-';
        } else {
            simplifiedExpression += c;
        }
    }

    stack<char> operators;
    queue<string> output;
    string token;
    for (size_t i = 0; i < simplifiedExpression.size(); ++i) {
        char c = simplifiedExpression[i];
        if (isdigit(c) || c == '.') {
            token += c;
            while (i + 1 < simplifiedExpression.size() &&
                  (isdigit(simplifiedExpression[i + 1]) || simplifiedExpression[i + 1] == '.')) {
                token += simplifiedExpression[++i];
            }
            output.push(token);
            token.clear();
        } else if (c == '(') {
            operators.push(c);
        } else if (c == ')') {
            while (!operators.empty() && operators.top() != '(') {
                output.push(string(1, operators.top()));
                operators.pop();
            }
            if (operators.empty()) {
                throw runtime_error("Bieu thuc khong hop le");
            }
            operators.pop();
        } else if (isOperator(c)) {
            if ((c == '-' || c == '+') &&
                (i == 0 || simplifiedExpression[i - 1] == '(' || isOperator(simplifiedExpression[i - 1]))) {
                token += c;
                while (i + 1 < simplifiedExpression.size() &&
                      (isdigit(simplifiedExpression[i + 1]) || simplifiedExpression[i + 1] == '.')) {
                    token += simplifiedExpression[++i];
                }
                output.push(token);
                token.clear();
            } else {
                while (!operators.empty() && precedence(operators.top()) >= precedence(c)) {
                    output.push(string(1, operators.top()));
                    operators.pop();
                }
                operators.push(c);
            }
        }
    }
    while (!operators.empty()) {
        if (operators.top() == '(' || operators.top() == ')') {
            throw runtime_error("Bieu thuc khong hop le");
        }
        output.push(string(1, operators.top()));
        operators.pop();
    }
    return output;
}

double performOperation(double operand1, double operand2, char op)
{
    switch (op)
    {
    case '+':
        return operand1 + operand2;
    case '-':
        return operand1 - operand2;
    case '*':
        return operand1 * operand2;
    case '/':
        if (operand2 == 0)
        {
            throw runtime_error("So bi chia phai khac 0");
        }
        return operand1 / operand2;
    default:
        throw runtime_error("Bieu thuc khong hop le");
    }
}

double evaluatePostfix(queue<string> &postfix) {
    if (postfix.empty()) {
        throw runtime_error("Bieu thuc rong");
    }

    stack<double> s;
    while (!postfix.empty()) {
        string token = postfix.front();
        postfix.pop();

        if (token == "(" || token == ")") {
            continue; 
        }

        if (isdigit(token[0]) ||
            ((token[0] == '-' || token[0] == '+') &&
             token.size() > 1 &&
             (isdigit(token[1]) || token[1] == '.'))) {
            s.push(stod(token));
        } else if (isOperator(token[0]) && token.size() == 1) {
            if (s.size() < 2) {
                throw runtime_error("Bieu thuc khong hop le");
            }
            double operand2 = s.top(); s.pop();
            double operand1 = s.top(); s.pop();
            double result = performOperation(operand1, operand2, token[0]);
            s.push(result);
        } else {
            throw runtime_error("Toan tu khong hop le: " + token);
        }
    }
    if (s.size() != 1) {
        throw runtime_error("Bieu thuc khong hop le");
    }
    return s.top();
}

string removeSpaces(const string &str)
{
    string result;
    for (char c : str)
    {
        if (!isspace(c))
        {
            result += c;
        }
    }
    return result;
}

int main()
{
    string expression;
    cout << "Nhap bieu thuc trung to: ";
    getline(cin, expression);

    expression = removeSpaces(expression);
    
    try
    {
        queue<string> postfix = chuyendoi(expression);
        double result = evaluatePostfix(postfix);
        cout << "Ket qua: " << result << fixed << setprecision(2) << endl;
    }
    catch (const exception &e)
    {
        cout << "Loi: " << e.what() << endl;
    }
    return 0;
}
