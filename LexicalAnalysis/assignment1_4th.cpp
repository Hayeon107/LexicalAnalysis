#include <fstream>
#include <cctype>
#include <string>
#include <map>
#include <iostream>
#include <typeinfo>
using namespace std;

void program();
void statements();
void statement();
void expression();
void term();



void term_tail();
void factor();
void factor_tail();


int lex();
int lookup(char);
void getNonBlank();
void getChar();
void addChar();

int charClass;
string line;
char lexeme[100];
int lexLen;
char nextChar;
int token;
int nextToken;
int id_cnt;
int const_cnt;
int op_cnt;

ifstream file;

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

int error() { return -1; }

int main() {
    id_cnt = 0;
    const_cnt = 0;
    op_cnt = 0;
    file.open("front.txt");
    while (!file.eof())
    {
        getChar();
        do {
            lex();
            program();
        } while (nextToken != EOF);
    }
    file.close();
}


void program() {
    // <program> �� <statements>
    statements();
}

void statements() {
    // <statements> �� <statement> | <statement> <semi_colon> <statements>
    statement();
    if (nextToken == SEMI_COLON) {
        lex(); // Consume the SEMI_COLON
        statements();
    }
}

void statement() {
    // <statement> �� <ident> <assignment_op> <expression>
    if (nextToken == IDENT) {
        lex(); // Consume IDENT
        if (nextToken == ASSIGN_OP) {
            lex(); // Consume ASSIGN_OP
            expression();
        }
        else {
            cout << "Error: Expected ASSIGN_OP" << endl;
        }
    }
    else {
        cout << "Error: Expected IDENT" << endl;
    }
}

void expression() {
    // <expression> �� <term> <term_tail>
    term();
    term_tail();
}

void term() {
    // <term> �� <factor> <factor_tail>
    factor();
    factor_tail();
}

void factor() {
    // <factor> �� <left_paren> <expression> <right_paren> | <ident> | <const>
    if (nextToken == LEFT_PAREN) {
        lex(); // Consume LEFT_PAREN
        expression();
        if (nextToken == RIGHT_PAREN) {
            lex(); // Consume RIGHT_PAREN
        }
        else {
            cout << "Error: Expected RIGHT_PAREN" << endl;
        }
    }
    else if (nextToken == IDENT || nextToken == INT_LIT) {
        lex(); // Consume IDENT or INT_LIT
    }
    else {
        cout << "Error: Expected LEFT_PAREN, IDENT, or INT_LIT" << endl;
    }
}

void term_tail() {
    // <term_tail> �� <add_op> <term> <term_tail> | ��
    if (nextToken == ADD_OP) {
        lex(); // Consume ADD_OP
        term();
        term_tail();
    }
    // Handle �� (epsilon) production
}

void factor_tail() {
    // <factor_tail> �� <mult_op> <factor> <factor_tail> | ��
    if (nextToken == MULT_OP) {
        lex(); // Consume MULT_OP
        factor();
        factor_tail();
    }
    // Handle �� (epsilon) production
}


//������ ��ū�� �����ϴ� ���̺�
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

//���� ������ char�� ����
void getNonBlank() {
    while (isspace(nextChar)) { //nextChar�� �����̸� ����
        cout << nextChar;
        getChar();
    }
}

void addChar() { //���ָ� �����Ѵ�
    //lexeme.insert(lexeme.begin() + 2, nextChar);
    lexeme[lexLen++] = nextChar;
    //cout << nextChar;
    lexeme[lexLen] = 0;
}

void getChar() { // charClass ��������
    if ((nextChar = file.get()) != EOF) {//nextChar�� file���� �����°Ŵ�

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

//���ڸ� ��ūȭ�Ѵ�
int lex() {
    //���ָ� �����ҰŴ�
    lexLen = 0;
    getNonBlank(); //��ĭ�� ������ getChar���� �����´�
    switch (charClass) {
    case LETTER:
        addChar(); //���������
        getChar(); //charClass��������
        while (charClass == LETTER || charClass == DIGIT) {
            addChar();
            getChar();
        }
        nextToken = IDENT;
        cout << "this is lexeme : "<<lexeme << endl;
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
        cout << "this is DIGIT : " << stoi(lexeme) << endl;
        break;
    case UNKNOWN:
        lookup(nextChar);
        getChar();
        if (nextToken == ADD_OP || nextToken == MULT_OP || nextToken == DIV_OP || nextToken == SUB_OP) {
            op_cnt += 1;
        }
        else if (nextToken == SEMI_COLON) {
            cout << "\nID : " << id_cnt << "; CONST: " << const_cnt << "; OP : " << op_cnt << ";" << endl;
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

    //cout << "Next token is " << nextToken << ", Next lexeme is " << lexeme << endl;

    return nextToken;

}
