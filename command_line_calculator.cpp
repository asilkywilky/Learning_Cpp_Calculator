#include <iostream> // for input and output
#include <vector>
#include <string>
#include <cstdlib>  // for atof to convert string to double
#include <cctype>   // for isdigit to check if a character is a digit

// future functions
void print_usage();
double apply_operation(double left, double right, char operation);
int get_operator_precedence(char operation);
bool is_operator(char c);

int main(int argc, char* argv[]) {
    // check if the number of arguments is correct
    // we need at least three arguments: number operator number
    // and an odd number of total arguments because it will start and end with a number
    if (argc < 4 || (argc - 1) % 2 == 0) {
        print_usage();
        return 1;
    }

    // convert command line arguments to a vector of strings
    // this makes it easier to process
    // like a list from [1 - end]
    std::vector<std::string> tokens(argv + 1, argv + argc);

    // vectors to store numbers and operators separately
    std::vector<double> values;
    std::vector<char> operators;

    // loop through each token
    for (const auto& token : tokens) {
        // if the token is a number, add it to the values vector
        if (isdigit(token[0]) || (token[0] == '-' && token.size() > 1)) {
            values.push_back(std::atof(token.c_str()));
        }
        // if the token is an operator
        else if (is_operator(token[0])) {
            // process all operators in the stack with higher or equal order of operation
            while (!operators.empty() && get_operator_precedence(operators.back()) >= get_operator_precedence(token[0])) {
                double right = values.back(); values.pop_back();
                double left = values.back(); values.pop_back();
                char op = operators.back(); operators.pop_back();
                values.push_back(apply_operation(left, right, op));
            }
            // add the current operator to the stack
            operators.push_back(token[0]);
        } else {
            // if the token is invalid, print an error message and usage
            std::cerr << "error: invalid argument '" << token << "'" << std::endl;
            print_usage();
            return 1;
        }
    }

    // add remaining operators in the stack
    while (!operators.empty()) {
        double right = values.back(); values.pop_back();
        double left = values.back(); values.pop_back();
        char op = operators.back(); operators.pop_back();
        values.push_back(apply_operation(left, right, op));
    }

    // after all operations, there should be exactly one value left
    if (values.size() != 1) {
        std::cerr << "error: invalid expression." << std::endl;
        return 1;
    }

    // print the final result
    std::cout << "result: " << values.back() << std::endl;

    return 0;
}

// function to print usage information for errors
void print_usage() {
    std::cout << "usage: calculator <num1> <operator> <num2> [<operator> <num3> ...]" << std::endl;
    std::cout << "operators: +, -, *, /" << std::endl;
}

// function to apply an operation on two numbers
double apply_operation(double left, double right, char operation) {
    // perform the operation based on the operator
    switch (operation) {
        case '+': return left + right;
        case '-': return left - right;
        case '*': return left * right;
        case '/': 
            // handle division by zero
            if (right != 0) return left / right;
            else {
                std::cerr << "error: division by zero!" << std::endl;
                exit(1);
            }
        default:
            // handle unsupported operators
            std::cerr << "error: unsupported operator '" << operation << "'" << std::endl;
            exit(1);
    }
}

// function to get the precedence of an operator
int get_operator_precedence(char operation) {
    // define order of operations
    switch (operation) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        default: return 0;
    }
}

// function to check if a character is a valid operator
bool is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}