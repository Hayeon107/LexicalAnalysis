#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <map>
using namespace std;
// Global variable declarations
map<string,int> finalMap;
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
int id_cnt;
int const_cnt;
int op_cnt;

ifstream file("front.txt");
// Function declarations
void addChar();
void getChar();
void getNonBlank();
int lex();
void term();
void expr();
void factor();
int error();
void statement();
void statements();


// Character types
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

// Token codes
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define SEMI_COLON 27

int error() { 
    cout << "Error" << endl;
    return -1; }

int main() {
    // Open the input data file and process its contents
    string line;
    id_cnt = 0;
    const_cnt = 0;
    op_cnt = 0;
    while (!file.eof())
    {
        getline(file, line);
        getChar();
        do {
            lex();
            statements();
            //            if (nextToken != EOF) {           }
        } while (nextToken != EOF);
        cout << line << endl;
    }
    /*
    if (file.is_open()) {
        string line;
        getline(file, line);
        getChar();
        do {
            lex();
            statements();
//            if (nextToken != EOF) {           }
        } while (nextToken != EOF);
        cout << "ID: " << id_cnt << "; CONST: " << const_cnt << ";OP: " << op_cnt << endl;
        file.close();
    else {
        cout << "ERROR - cannot open front.txt" << endl;
    }
    }*/
    return 0;
}


int lookup(char ch) {
    switch (ch) {
    case '(':
        addChar();
        nextToken = LEFT_PAREN;
        break;
    case ')':
        addChar();
        nextToken = RIGHT_PAREN;
        break;
    case '+':
        addChar();
        nextToken = ADD_OP;
        break;
    case '-':
        addChar();
        nextToken = SUB_OP;
        break;
    case '*':
        addChar();
        nextToken = MULT_OP;
        break;
    case '/':
        addChar();
        nextToken = DIV_OP;
        break;
        /*
    case ':':
        addChar();
        if (nextChar == '=') {
            getChar();
            addChar();
            nextToken = ASSIGN_OP;
        }
        else {
            nextToken = UNKNOWN;
        }
        break; */
    case '=':
        addChar();
        nextToken = ASSIGN_OP;
        break;
    case ';':
        addChar();
        nextToken = SEMI_COLON;
        break;
    default:
        addChar();
        nextToken = EOF;
        break;
    }
    return nextToken;
}

void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    }
    else {
        cout << "Error - lexeme is too long" << endl;
    }
}

void getChar() { // charClass 가져오기
    if ((nextChar = file.get()) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else
            charClass = UNKNOWN;
    }
    else {
        charClass = EOF;
    }
}

void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}

int lex() {
    lexLen = 0;
    getNonBlank();
    switch (charClass) {
    case LETTER:
        addChar();
        getChar();
        while (charClass == LETTER || charClass == DIGIT) {
            addChar();
            getChar();
        }
        nextToken = IDENT;
        id_cnt += 1;
        break;
    case DIGIT:
        addChar();
        getChar();
        while (charClass == DIGIT) {
            addChar();
            getChar();
        }
        nextToken = INT_LIT;
        const_cnt += 1;
        break;
    case UNKNOWN:
        lookup(nextChar);
        getChar();
        if (nextToken == ADD_OP || nextToken == MULT_OP || nextToken == DIV_OP || nextToken == SUB_OP) {
            op_cnt += 1;
        }
        break;
    case EOF:
        nextToken = EOF;
        lexeme[0] = 'E';
        lexeme[1] = 'O';
        lexeme[2] = 'F';
        lexeme[3] = 0;
        break;
    }

    cout << "Next token is " << nextToken << ", Next lexeme is " << lexeme << endl;

    return nextToken;
}

void statements() {
    cout << "<statements>" << endl;
    statement();
    if (nextToken == SEMI_COLON) {
        lex();
        statements();
    }
}

void statement() {
    cout << "<statement>" << endl;
    if (nextToken == IDENT) {
        lex();
        if (nextToken == ASSIGN_OP) {
            lex();
            expr();
        }
        
        else {
            cout << "error at statement1" << endl;
        }
    }
    else {
        cout << "error at statement2" << endl;
//        error();
    }
}
void expr() {
    cout << "<expr>" << endl;
    term();
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        lex();
        term();
    };
}
void term() {
    cout << "<term>" << endl;
    factor();
    while (nextToken == MULT_OP || nextToken == DIV_OP) {
        lex();
        factor();
    };
}

void factor() {
    cout << "<factor>" << endl;
    if (nextToken == LEFT_PAREN) {
        lex();
        expr();
        if (nextToken == RIGHT_PAREN) {
            lex();
        }
        else {
            cout << "error at factor" << endl;
            //            error();
        }
    }
    else if (nextToken == IDENT || nextToken == INT_LIT) {
        lex();
    }
    else {
        cout << "error at factor 2" << endl;
        //error();
    }
}


