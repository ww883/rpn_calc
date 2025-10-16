#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <stdexcept>
#include <map>
#include <iomanip>

class RPNCalculator {
private:
    std::stack<double> operands;
    std::vector<std::string> history;
    bool debugMode;

    // 检查是否为运算符
    bool isOperator(const std::string& token) {
        return token == "+" || token == "-" || token == "*" || token == "/" ||
            token == "sqrt" || token == "^" || token == "fib" || token == "pascal";
    }

    // 检查是否为数字
    bool isNumber(const std::string& token) {
        std::istringstream iss(token);
        double value;
        iss >> value;
        return !iss.fail() && iss.eof();
    }

    // 处理四则运算
    void handleBasicOperation(const std::string& op) {
        if (operands.size() < 2) {
            throw std::runtime_error("错误: 栈中操作数不足");
        }

        double right = operands.top(); operands.pop();
        double left = operands.top(); operands.pop();
        double result;

        if (op == "+") result = left + right;
        else if (op == "-") result = left - right;
        else if (op == "*") result = left * right;
        else if (op == "/") {
            if (right == 0) throw std::runtime_error("错误: 除零错误");
            result = left / right;
        }

        operands.push(result);
        if (debugMode) {
            std::cout << "计算: " << left << " " << op << " " << right << " = " << result << std::endl;
        }
    }

    // 计算斐波那契数列
    double fibonacci(int n) {
        if (n < 0) throw std::runtime_error("错误: 斐波那契数列参数必须为非负整数");
        if (n == 0) return 0;
        if (n == 1) return 1;

        double a = 0, b = 1;
        for (int i = 2; i <= n; i++) {
            double temp = b;
            b = a + b;
            a = temp;
        }
        return b;
    }

    // 计算杨辉三角指定行和
    double pascalTriangle(int n) {
        if (n < 0) throw std::runtime_error("错误: 杨辉三角行数必须为非负整数");

        double sum = 0;
        double value = 1;
        for (int k = 0; k <= n; k++) {
            sum += value;
            value = value * (n - k) / (k + 1);
        }
        return sum;
    }

public:
    RPNCalculator() : debugMode(false) {}

    // 设置调试模式
    void setDebugMode(bool mode) {
        debugMode = mode;
    }

    // 清空栈
    void clearStack() {
        while (!operands.empty()) {
            operands.pop();
        }
    }

    // 显示当前栈内容
    void displayStack() {
        if (operands.empty()) {
            std::cout << "栈为空" << std::endl;
            return;
        }

        std::stack<double> temp = operands;
        std::vector<double> elements;

        std::cout << "当前栈内容 (从顶到底): ";
        while (!temp.empty()) {
            elements.push_back(temp.top());
            temp.pop();
        }

        for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }

    // 执行单个RPN表达式
    double evaluateExpression(const std::string& expression) {
        std::istringstream iss(expression);
        std::string token;
        std::vector<std::string> tokens;

        // 分割表达式为标记
        while (iss >> token) {
            tokens.push_back(token);
        }

        for (const auto& t : tokens) {
            if (isNumber(t)) {
                double value = std::stod(t);
                operands.push(value);
                if (debugMode) {
                    std::cout << "压入数字: " << value << std::endl;
                }
            }
            else if (isOperator(t)) {
                if (t == "sqrt") {
                    if (operands.empty()) throw std::runtime_error("错误: 栈为空");
                    double value = operands.top(); operands.pop();
                    if (value < 0) throw std::runtime_error("错误: 不能对负数开平方");
                    double result = sqrt(value);
                    operands.push(result);
                    if (debugMode) {
                        std::cout << "计算: sqrt(" << value << ") = " << result << std::endl;
                    }
                }
                else if (t == "^") {
                    if (operands.size() < 2) throw std::runtime_error("错误: 栈中操作数不足");
                    double exponent = operands.top(); operands.pop();
                    double base = operands.top(); operands.pop();
                    double result = pow(base, exponent);
                    operands.push(result);
                    if (debugMode) {
                        std::cout << "计算: " << base << " ^ " << exponent << " = " << result << std::endl;
                    }
                }
                else if (t == "fib") {
                    if (operands.empty()) throw std::runtime_error("错误: 栈为空");
                    double n = operands.top(); operands.pop();
                    if (n != (int)n || n < 0) {
                        throw std::runtime_error("错误: 斐波那契数列参数必须为非负整数");
                    }
                    double result = fibonacci((int)n);
                    operands.push(result);
                    if (debugMode) {
                        std::cout << "计算: fib(" << n << ") = " << result << std::endl;
                    }
                }
                else if (t == "pascal") {
                    if (operands.empty()) throw std::runtime_error("错误: 栈为空");
                    double n = operands.top(); operands.pop();
                    if (n != (int)n || n < 0) {
                        throw std::runtime_error("错误: 杨辉三角行数必须为非负整数");
                    }
                    double result = pascalTriangle((int)n);
                    operands.push(result);
                    if (debugMode) {
                        std::cout << "计算: pascal(" << n << ") = " << result << std::endl;
                    }
                }
                else {
                    handleBasicOperation(t);
                }
            }
            else {
                throw std::runtime_error("错误: 未知的标记 '" + t + "'");
            }
        }

        if (operands.size() != 1) {
            throw std::runtime_error("错误: 表达式不完整或有多余操作数");
        }

        double result = operands.top();
        history.push_back(expression + " = " + std::to_string(result));
        return result;
    }

    // 批量计算表达式
    void batchEvaluate(const std::vector<std::string>& expressions) {
        std::cout << "开始批量计算 " << expressions.size() << " 个表达式:" << std::endl;
        std::cout << "----------------------------------------" << std::endl;

        for (size_t i = 0; i < expressions.size(); i++) {
            try {
                // 保存当前栈状态
                std::stack<double> backup = operands;
                clearStack();

                double result = evaluateExpression(expressions[i]);
                std::cout << "表达式 " << (i + 1) << ": " << expressions[i] << " = " << result << std::endl;

                // 恢复栈状态
                operands = backup;
            }
            catch (const std::exception& e) {
                std::cout << "表达式 " << (i + 1) << ": " << expressions[i] << " -> 错误: " << e.what() << std::endl;
            }
        }
        std::cout << "----------------------------------------" << std::endl;
    }

    // 显示计算历史
    void showHistory() {
        if (history.empty()) {
            std::cout << "计算历史为空" << std::endl;
            return;
        }

        std::cout << "计算历史:" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        for (size_t i = 0; i < history.size(); i++) {
            std::cout << i + 1 << ". " << history[i] << std::endl;
        }
        std::cout << "----------------------------------------" << std::endl;
    }

    // 清空历史记录
    void clearHistory() {
        history.clear();
        std::cout << "历史记录已清空" << std::endl;
    }

    // 交互式模式
    void interactiveMode() {
        std::cout << "=== RPN计算器交互模式 ===" << std::endl;
        std::cout << "支持命令: clear, show, history, clearhist, debug, quit" << std::endl;
        std::cout << "支持运算符: +, -, *, /, sqrt, ^, fib, pascal" << std::endl;
        std::cout << "输入RPN表达式进行计算，例如: '5 2 + 3 *'" << std::endl;
        std::cout << "----------------------------------------" << std::endl;

        std::string input;
        while (true) {
            std::cout << "> ";
            std::getline(std::cin, input);

            if (input == "quit" || input == "exit") {
                break;
            }
            else if (input == "clear") {
                clearStack();
                std::cout << "栈已清空" << std::endl;
            }
            else if (input == "show") {
                displayStack();
            }
            else if (input == "history") {
                showHistory();
            }
            else if (input == "clearhist") {
                clearHistory();
            }
            else if (input == "debug on") {
                setDebugMode(true);
                std::cout << "调试模式已开启" << std::endl;
            }
            else if (input == "debug off") {
                setDebugMode(false);
                std::cout << "调试模式已关闭" << std::endl;
            }
            else if (!input.empty()) {
                try {
                    double result = evaluateExpression(input);
                    std::cout << "结果: " << result << std::endl;
                }
                catch (const std::exception& e) {
                    std::cout << e.what() << std::endl;
                }
            }
        }
    }
};

// 演示函数
void demonstrateCalculator() {
    RPNCalculator calc;

    std::cout << "=== RPN计算器演示 ===" << std::endl;

    // 演示基本运算
    std::cout << "\n1. 基本四则运算演示:" << std::endl;
    try {
        double result = calc.evaluateExpression("5 2 + 3 *");
        std::cout << "5 2 + 3 * = " << result << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "错误: " << e.what() << std::endl;
    }

    // 演示高级功能
    std::cout << "\n2. 高级功能演示:" << std::endl;
    try {
        double result = calc.evaluateExpression("16 sqrt");
        std::cout << "16 sqrt = " << result << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "错误: " << e.what() << std::endl;
    }

    // 演示斐波那契数列
    std::cout << "\n3. 斐波那契数列演示:" << std::endl;
    try {
        double result = calc.evaluateExpression("10 fib");
        std::cout << "10 fib = " << result << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "错误: " << e.what() << std::endl;
    }

    // 演示批量计算
    std::cout << "\n4. 批量计算演示:" << std::endl;
    std::vector<std::string> expressions = {
        "3 4 + 5 *",
        "20 5 / 3 +",
        "9 sqrt 2 *",
        "5 fib 2 +"
    };
    calc.batchEvaluate(expressions);
}

int main() {
    std::cout << "RPN计算器 - C++实现" << std::endl;
    std::cout << "====================" << std::endl;

    int choice;
    std::cout << "选择模式:\n1. 演示模式\n2. 交互模式\n选择: ";
    std::cin >> choice;
    std::cin.ignore(); // 清除输入缓冲区

    if (choice == 1) {
        demonstrateCalculator();
    }
    else if (choice == 2) {
        RPNCalculator calc;
        calc.interactiveMode();
    }
    else {
        std::cout << "无效选择，使用演示模式" << std::endl;
        demonstrateCalculator();
    }

    std::cout << "感谢使用RPN计算器!" << std::endl;
    return 0;
}