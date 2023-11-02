#include <iostream>
#include <stack>
#include <cctype>

// Define a struct to represent tokens
struct Token {
    enum Type { NUMBER, PLUS, MINUS, MULTIPLY, DIVIDE, LPAREN, RPAREN, END } type;
    double value;
};

// Tokenize the input expression
std::stack<Token> tokenize(std::string input) {
    std::stack<Token> tokens;
    size_t i = 0;

    while (i < input.size()) {
        if (std::isspace(input[i])) {
            ++i;
        }
        else if (std::isdigit(input[i])) {
            double value = 0.0;
            while (i < input.size() && std::isdigit(input[i])) {
                value = value * 10 + (input[i] - '0');
                ++i;
            }
            tokens.push({ Token::NUMBER, value });
        }
        else {
            switch (input[i]) {
            case '+': tokens.push({ Token::PLUS, 0.0 }); break;
            case '-': tokens.push({ Token::MINUS, 0.0 }); break;
            case '*': tokens.push({ Token::MULTIPLY, 0.0 }); break;
            case '/': tokens.push({ Token::DIVIDE, 0.0 }); break;
            case '(': tokens.push({ Token::LPAREN, 0.0 }); break;
            case ')': tokens.push({ Token::RPAREN, 0.0 }); break;
            default: throw std::runtime_error("Invalid character in input");
            }
            ++i;
        }
    }

    tokens.push({ Token::END, 0.0 });
    return tokens;
}

// Function to parse the expression
double parseExpression(std::stack<Token>& tokens);

// Function to parse a term
double parseTerm(std::stack<Token>& tokens);

// Function to parse a factor
double parseFactor(std::stack<Token>& tokens);

int main() {
    std::string input;
    std::cout << "Enter an expression: ";
    std::cin >> input;

    std::stack<Token> tokens = tokenize(input);

    try {
        double result = parseExpression(tokens);

        if (tokens.top().type == Token::END) {
            std::cout << "Result: " << result << std::endl;
        }
        else {
            std::cerr << "Error: Invalid input" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

double parseExpression(std::stack<Token>& tokens) {
    double value = parseTerm(tokens);
    while (tokens.top().type == Token::PLUS || tokens.top().type == Token::MINUS) {
        Token op = tokens.top();
        tokens.pop();
        if (op.type == Token::PLUS) {
            value += parseTerm(tokens);
        }
        else {
            value -= parseTerm(tokens);
        }
    }
    return value;
}

double parseTerm(std::stack<Token>& tokens) {
    double value = parseFactor(tokens);
    while (tokens.top().type == Token::MULTIPLY || tokens.top().type == Token::DIVIDE) {
        Token op = tokens.top();
        tokens.pop();
        if (op.type == Token::MULTIPLY) {
            value *= parseFactor(tokens);
        }
        else {
            double divisor = parseFactor(tokens);
            if (divisor == 0.0) {
                throw std::runtime_error("Division by zero");
            }
            value /= divisor;
        }
    }
    return value;
}

double parseFactor(std::stack<Token>& tokens) {
    if (tokens.top().type == Token::NUMBER) {
        double value = tokens.top().value;
        tokens.pop();
        return value;
    }
    else if (tokens.top().type == Token::LPAREN) {
        tokens.pop();
        double value = parseExpression(tokens);
        if (tokens.top().type == Token::RPAREN) {
            tokens.pop();
            return value;
        }
        else {
            throw std::runtime_error("Unmatched parentheses");
        }
    }
    else {
        throw std::runtime_error("Invalid factor");
    }
}
