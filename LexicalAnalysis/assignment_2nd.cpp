#include <iostream>
#include <cctype>
#include <string>
#include <map>

// Token types
enum TokenType {
    IDENTIFIER,
    CONSTANT,
    ASSIGN_OP,
    ADD_OP,
    MULT_OP,
    LEFT_PAREN,
    RIGHT_PAREN,
    SEMI_COLON
};

bool match(std::string& input, TokenType expectedType);
bool expression(std::string& input);
bool term(std::string& input);


// Token structure
struct Token {
    TokenType type;
    std::string value;
};

// Function to tokenize the input
Token getNextToken(std::string& input) {
    Token token;
    if (input.empty()) {
        token.type = SEMI_COLON; // End of input, simulate a semicolon
    }
    else if (isalpha(input[0])) {
        token.type = IDENTIFIER;
        token.value = input[0];
        input.erase(0, 1);
        while (!input.empty() && (isalnum(input[0]) || input[0] == '_')) {
            token.value += input[0];
            input.erase(0, 1);
        }
    }
    else if (isdigit(input[0])) {
        token.type = CONSTANT;
        token.value = input[0];
        input.erase(0, 1);
        while (!input.empty() && isdigit(input[0])) {
            token.value += input[0];
            input.erase(0, 1);
        }
    }
    else {
        switch (input[0]) {
        case ':':
            if (input.length() >= 2 && input[1] == '=') {
                token.type = ASSIGN_OP;
                token.value = ":=";
                input.erase(0, 2);
            }
            break;
        case '+':
        case '-':
            token.type = ADD_OP;
            token.value = input[0];
            input.erase(0, 1);
            break;
        case '*':
        case '/':
            token.type = MULT_OP;
            token.value = input[0];
            input.erase(0, 1);
            break;
        case '(':
            token.type = LEFT_PAREN;
            token.value = "(";
            input.erase(0, 1);
            break;
        case ')':
            token.type = RIGHT_PAREN;
            token.value = ")";
            input.erase(0, 1);
            break;
        }
    }
    return token;
}

bool match(std::string& input, TokenType expectedType) {
    Token token = getNextToken(input);
    if (token.type == expectedType) {
        return true;
    }
    else {
        std::cout << "Error: Expected " << expectedType << ", but found " << token.type << std::endl;
        return false;
    }
}

bool factor(std::string& input) {
    Token token = getNextToken(input);
    if (token.type == IDENTIFIER || token.type == CONSTANT) {
        return true;
    }
    else if (token.type == LEFT_PAREN) {
        if (expression(input) && match(input, RIGHT_PAREN)) {
            return true;
        }
    }
    return false;
}

bool factor_tail(std::string& input) {
    Token token = getNextToken(input);
    if (token.type == MULT_OP || token.type == ADD_OP) {
        if (term(input) && factor_tail(input)) {
            return true;
        }
    }
    return true; // ех (empty)
}

bool term(std::string& input) {
    if (factor(input) && factor_tail(input)) {
        return true;
    }
    return false;
}

bool term_tail(std::string& input) {
    Token token = getNextToken(input);
    if (token.type == ADD_OP) {
        if (term(input) && term_tail(input)) {
            return true;
        }
    }
    return true; // ех (empty)
}

bool expression(std::string& input) {
    if (term(input) && term_tail(input)) {
        return true;
    }
    return false;
}

bool statement(std::string& input) {
    if (match(input, IDENTIFIER) && match(input, ASSIGN_OP) && expression(input)) {
        return true;
    }
    return false;
}

bool statements(std::string& input) {
    if (statement(input) && match(input, SEMI_COLON) && statements(input)) {
        return true;
    }
    return true; // ех (empty)
}

bool program(std::string& input) {
    if (statements(input)) {
        return true;
    }
    return false;
}

int main() {
    std::string input = "operand1 := 3 ; operand2 := operand1 + 2 ; target := operand1 + operand2 * 3";

    std::map<std::string, int> variables;

    while (!input.empty()) {
        std::string identifier;
        if (match(input, IDENTIFIER)) {
            identifier = getNextToken(input).value;
            match(input, ASSIGN_OP);
            int result = 0;
            expression(input);
            for (const char& c : identifier) {
                if (isalpha(c)) {
                    result += c - 'a' + 1;
                }
            }
            variables[identifier] = result;
            std::cout << identifier << " := " << variables[identifier] << "; ";
        }
    }

    std::cout << std::endl << "Result ==> ";
    for (const auto& variable : variables) {
        std::cout << variable.first << ": " << variable.second << "; ";
    }
    std::cout << std::endl;

    return 0;
}
