#include <fstream>
#include <cctype>
#include <string>
#include <map>
#include <iostream>
using namespace std;

void factor();
int lex();
int lookup(char);
void getNonBlank();
void getChar();
void addChar();
void statements();
void statement();
void expr();
void term();

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

int error(){ return -1; }

int main() {
    id_cnt = 0;
    const_cnt = 0;
    op_cnt = 0;
	file.open("front.txt");
    while (!file.eof())
    {
        //getline(file, line);
        //cout << line << endl;
        getChar();
        do {
            lex();
            statements();
        } while (nextToken != EOF);
    }
	file.close();
}

void statements() {
    //<statement><semi_colon><statement>
    cout << "<statements>" << endl;
    statement();
    if (nextToken == SEMI_COLON) {
        lex();
        statements();
    }
    else {
        cout << "statement need semicolon";
        error();
    }
}

void statement() {
    //<ident><assignment_op><expr>
    if (nextToken == IDENT) {
        lex(); //다음 charclass를 가져온다
        if (nextToken == ASSIGN_OP) {
            lex();
            expr();
        }
        else {
            cout << "statement need assignment op" << endl;
        }
    }
    else {
        cout << "No ident" << endl;
        error();
    }
}

void expr() {
    //<term><term_tail>
    term();
    if (nextToken == ADD_OP || nextToken == SUB_OP) {
        lex();
        term();
    }
    else if(nextToken == ' ') {}
    else {
        cout << "expr need term" << endl;
    }
}

void term() {
    factor();
    if (nextToken == MULT_OP || nextToken == DIV_OP) {
        lex();
        factor();
    }
    else if (nextToken == ' ') {}
    else {
        cout << "term error" << endl;
        error();
    }
}

void factor() {
    if (nextToken == LEFT_PAREN) {
        lex();
        expr();
        if (nextToken == RIGHT_PAREN) {
            lex();
        }
        else {
            cout << "need lex" << endl;
        }
    }
    else if (nextToken == IDENT || nextToken == INT_LIT) {
        lex();
    }
    else {
        cout << "factor error" << endl;
    }
}

//글자의 토큰을 구별하는 테이블
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

//공백 전까지 char을 받자
void getNonBlank() {
    while (isspace(nextChar)) { //nextChar가 공백이면 실행
        cout << nextChar;
        getChar(); 
    }
}

void addChar() { //어휘를 저장한다
    //lexeme.insert(lexeme.begin() + 2, nextChar);
    lexeme[lexLen++] = nextChar;
    cout << nextChar;
    lexeme[lexLen] = 0;
}

void getChar() { // charClass 가져오기
    if ((nextChar = file.get()) != EOF) {//nextChar은 file에서 가져온거다
        
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

//글자를 토큰화한다
int lex() {
    //어휘를 저장할거다
    lexLen = 0;
    getNonBlank(); //빈칸이 나오면 getChar에서 가져온다
    switch (charClass) {
    case LETTER:
        addChar(); //어휘저장용
        getChar(); //charClass가져오기
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
        else if (nextToken == SEMI_COLON) {
            cout << "\nID : " << id_cnt << "; CONST: " << const_cnt <<"; OP : "<<op_cnt <<";" << endl;
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
