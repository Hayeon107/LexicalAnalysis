#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

int charClass = 0;
vector<char> lexeme;
char nextChar = ' ';
int lexLen = 0;
int nextToken = 0;
string tokenString = "";

int ID_cnt = 0;
int CONST_cnt = 0;
int OP_cnt = 0;

enum CharClass { LETTER, DIGIT, UNKNOWN, EOF_CHAR };
enum TokenCode { CONST = 10, IDENT = 11, ASSIGNMENT_OP = 20, ADD_OP = 21, SUB_OP = 22, MULT_OP = 23, DIV_OP = 24, LEFT_PAREN = 25, RIGHT_PAREN = 26, SEMI_COLON = 59 };

map<string, string> identResult;

vector<string> readLine;
bool isOk = true;
vector<string> errorString;

void addChar();
void getchar(ifstream& file);
void lexical(ifstream& file);
void program(ifstream& file);
void statements(ifstream& file);
void statement(ifstream& file);
int expression(ifstream& file);
int term(ifstream& file);
int termTail(ifstream& file, int t);
int factorTail(ifstream& file, int fa);
int factor(ifstream& file);

ifstream inputFile;
int main() {
    inputFile.open("front.txt");

    if (!inputFile.is_open()) {
        cerr << "Failed to open input file." << endl;
        return 1;
    }

    getchar(inputFile);
    while (nextToken != EOF_CHAR) {
        lexical(inputFile);
        program(inputFile);
    }

    inputFile.close();

    cout << "Result ==> ";
    for (const auto& entry : identResult) {
        cout << entry.first << ": " << entry.second;
        if (entry != *identResult.rbegin()) {
            cout << "; ";
        }
    }

    return 0;
}

void lookup(char ch) {
    if (ch == '(') {
        addChar();
        nextToken = LEFT_PAREN;
    }
    else if (ch == ')') {
        addChar();
        nextToken = RIGHT_PAREN;
    }
    else if (ch == '+') {
        addChar();
        nextToken = ADD_OP;
        OP_cnt++;
    }
    else if (ch == '-') {
        addChar();
        nextToken = SUB_OP;
        OP_cnt++;
    }
    else if (ch == '*') {
        addChar();
        nextToken = MULT_OP;
        OP_cnt++;
    }
    else if (ch == '/') {
        addChar();
        nextToken = DIV_OP;
        OP_cnt++;
    }
    else if (ch == ';') {
        addChar();
        nextToken = SEMI_COLON;
        readLine.push_back(";");
    }
    else if (nextChar == ':') {
        addChar();
        getchar(inputFile);
        if (nextChar == '=') {
            addChar();
            nextToken = ASSIGNMENT_OP;
            readLine.push_back(":=");
        }
    }
    else {
        addChar();
        nextToken = EOF_CHAR;
    }
}

void addChar() {
    if (lexLen <= 98) {
        lexeme.push_back(nextChar);
        lexLen++;
    }
    else {
        errorString.push_back("Error - lexeme is too long");
        isOk = false;
    }
}

void getchar(ifstream& file) {
    if (file.get(nextChar)) {
        if (isalpha(nextChar)) {
            charClass = LETTER;
        }
        else if (isdigit(nextChar)) {
            charClass = DIGIT;
        }
        else {
            charClass = UNKNOWN;
        }
    }
    else {
        charClass = EOF_CHAR;
    }
}

void getNonBlank(ifstream& file) {
    while (isspace(nextChar)) {
        getchar(file);
    }
}

void lexical(ifstream& file) {
    lexeme.clear();
    lexLen = 0;
    getNonBlank(file);

    if (charClass == LETTER) {
        addChar();
        getchar(file);
        while (charClass == LETTER || charClass == DIGIT) {
            addChar();
            getchar(file);
        }
        nextToken = IDENT;
        ID_cnt++;
        tokenString = string(lexeme.begin(), lexeme.end());
        readLine.push_back(tokenString);
    }
    else if (charClass == DIGIT) {
        addChar();
        getchar(file);
        while (charClass == DIGIT) {
            addChar();
            getchar(file);
        }
        nextToken = CONST;
        CONST_cnt++;
        tokenString = string(lexeme.begin(), lexeme.end());
        readLine.push_back(tokenString);
    }
    else if (charClass == UNKNOWN) {
        lookup(nextChar);
        getchar(file);
    }
    else if (charClass == EOF_CHAR) {
        nextToken = EOF_CHAR;
    }
    tokenString = string(lexeme.begin(), lexeme.end());

    if (nextChar == '\n' || file.eof()) {
        if (!readLine.empty()) {
            string readLineString = "";
            for (const string& s : readLine) {
                readLineString += s;
            }
            cout << readLineString << endl;
            cout << "ID: " << ID_cnt << "; CONST: " << CONST_cnt << "; OP: " << OP_cnt << ";" << endl;
            if (isOk) {
                cout << "(OK)" << endl;
            }
            else {
                for (const string& error : errorString) {
                    cout << error << endl;
                }
                errorString.clear();
            }
            cout << endl;
            ID_cnt = 0;
            CONST_cnt = 0;
            OP_cnt = 0;
            isOk = true;
            readLine.clear();
        }
    }
}


void program(ifstream& file) {
    statements(file);
}

void statements(ifstream& file) {
    statement(file);
    while (nextToken == SEMI_COLON) {
        lexical(file);
        statements(file);
    }
}

void statement(ifstream& file) {
    if (nextToken == IDENT) {
        identResult[tokenString] = "Unknown";
        string tempIdentString = tokenString;
        lexical(file);
        if (nextToken == ASSIGNMENT_OP) {
            lexical(file);
            int e = expression(file);
            identResult[tempIdentString] = to_string(e);
        }
    }
}

int expression(ifstream& file) {
    int t = term(file);
    int exprResult = 0;
    t = term(file);
    if (t != 0) {
        exprResult = termTail(file, t);
    }
    else {
        exprResult = 0;
        termTail(file, t);
    }
    return exprResult;
}

int term(ifstream& file) {
    int termResult = 0;
    int fa = factor(file);
    if (fa !=  0){
        termResult = factorTail(file, fa);
    }
    else {
        termResult = 0;
        factorTail(file, fa);
    }
    return termResult;
}

int termTail(ifstream& file, int t) {
    int termResult = t;
    int t1 = term(file);
    if (nextToken == ADD_OP) {
        readLine.push_back(tokenString);
        lexical(file);
        if (nextToken == ADD_OP) {
            errorString.push_back("(Warning) 중복 연산자(+) 제거");
            OP_cnt--;
            isOk = false;
            lexical(file);
        }
        t1 = term(file);
        termTail(file, t1);
        if (t1 != 0 && t != 0) {
            termResult = t + t1;
        }
        else {
            return 0;
        }
    }
    else if (nextToken == SUB_OP) {
        readLine.push_back(tokenString);
        lexical(file);
        if (nextToken == SUB_OP) {
            errorString.push_back("(Warning) 중복 연산자(-) 제거");
            OP_cnt--;
            isOk = false;
            lexical(file);
        }
        t1 = term(file);
        termTail(file, t1);
        if (t1 != 0 || t != 0) {
            termResult = t - t1;
        }
    }
    return termResult;
}

int factorTail(ifstream& file, int fa) {
    int factorTailResult = fa;
    if (nextToken == MULT_OP) {
        readLine.push_back(tokenString);
        lexical(file);
        if (nextToken == MULT_OP) {
            errorString.push_back("(Warning) 중복 연산자(*) 제거");
            OP_cnt--;
            isOk = false;
            lexical(file);
        }
        int fa1 = factor(file);
        factorTail(file, fa1);
        if (fa1 != 0 && fa != 0) {
            factorTailResult = fa * fa1;
        }
    }
    else if (nextToken == DIV_OP) {
        readLine.push_back(tokenString);
        lexical(file);
        if (nextToken == DIV_OP) {
            errorString.push_back("(Warning) 중복 연산자(/) 제거");
            OP_cnt--;
            isOk = false;
            lexical(file);
        }
        int fa1 = factor(file);
        factorTail(file, fa1);
        if (fa1 != 0 && fa != 0) {
            factorTailResult = fa / fa1;
        }
    }
    return factorTailResult;
}

int factor(ifstream& file) {
    int factorResult = 0;
    if (nextToken == LEFT_PAREN) {
        readLine.push_back(tokenString);
        lexical(file);
        factorResult = expression(file);
        if (nextToken == RIGHT_PAREN) {
            readLine.push_back(tokenString);
            lexical(file);
        }
        else {
            errorString.push_back("(Warning): ) 누락");
            isOk = false;
        }
    }
    else if (nextToken == IDENT) {
        if (identResult.find(tokenString) == identResult.end()) {
            string s = "(Error)\"정의되지 않은 변수 (" + tokenString + ")가 참조됨\"";
            errorString.push_back(s);
            isOk = false;
            identResult[tokenString] = "Unknown";
            factorResult = 0;
        }
        else {
            factorResult = stoi(identResult[tokenString]);
            lexical(file);
        }
    }
    else if (nextToken == CONST) {
        factorResult = stoi(tokenString);
        lexical(file);
    }
    else {
        errorString.push_back("(Error) 왼쪽 괄호, ident, 또는 상수 필요");
        isOk = false;
    }
    return factorResult;
}

