#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
// 전역변수 선언들
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE* in_fp; 
FILE *file;
//함수 선언들
void addChar();
void getChar();
void getNonBlank();
int lex();
void term();
void expr();
void factor();
int error();

//문자 유형들
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

//토큰 코드들
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
/**********************************/
/*main 구동기*/
main() {
	//입력 데이터 파일을 열고 그 내용을 처리
//	if ((in_fp = fopen("C:\CppStudy\LexicalAnalysis\front.in", "r")) == NULL)
	if ((in_fp = fopen("front.in", "r")) == NULL)
		printf("ERROR - cannot open front2.in \n");
	else {
		getChar();
		do {
			lex();
		} while (nextToken != EOF);
	/*
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		printf("Current directory: %s\n", cwd);
	}
	else {
		perror("getcwd() error");
	}
	return 0;
	*/
	}
}

// lookup
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
	default:
		addChar();
		nextToken = EOF;
		break;
	}
	return nextToken;
}
/******************************/
// addChar - nextChar을 lexeme에 추가하는 함수
void addChar() {
	if (lexLen <= 98) {
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = 0;
	}
	else
		printf("Error - lexeme is too long \n");
}

/******************************************/
void getChar() {
	if ((nextChar = getc(in_fp)) != EOF) {
		if (isalpha(nextChar))
			charClass = LETTER;
		else if (isdigit(nextChar))
			charClass = DIGIT;
		else charClass = UNKNOWN;
	}
	else
		charClass = EOF;
}
/******************************************/
void getNonBlank() {
	while (isspace(nextChar))
		getChar();
}
/********************************************/
//lex - 산술식을 위한 단순 어휘 분석기
int lex() {
	lexLen = 0;
	getNonBlank();
	switch (charClass) {
			//식별자를 파싱한다
	case LETTER:
		addChar();
		getChar();
		while (charClass == LETTER || charClass == DIGIT) {
			addChar();
			getChar();
		}
		nextToken = IDENT;
		break;
	//정수리터럴 파싱
	case DIGIT:
		addChar();
		getChar();
		while (charClass == DIGIT) {
			addChar();
			getChar();
		}
		nextToken = INT_LIT;
		break;
	//괄호와 연산지
	case UNKNOWN:
		lookup(nextChar);
		getChar();
		break;
		//EOF
	case EOF:
		nextToken = EOF;
		lexeme[0] = 'E';
		lexeme[1] = 'O';
		lexeme[2] = 'F';
		lexeme[3] = 0;
		break;
	}

	printf("Next token is %d, Next lexeme is %s\n", nextToken, lexeme);
	return nextToken;
}

void expr() {
	printf("Enter <expr>\n");
	//첫번째 term을 파싱
	term();
	//다음 토큰이 + 또는 - 이면 다음번째 토큰을 가져오고 다음 term을 파싱
	while (nextToken == ADD_OP || nextToken == SUB_OP) {
		lex();
		term();
	}
}

void term() {
	printf("Enter <term>\n");
	factor();
	//다음번째 토큰이 *또는 /이면 lex를 호출하여 다음번째 토큰을 가져오고 다음번째 factor을 파싱
	while (nextToken == MULT_OP || nextToken == DIV_OP) {
		lex();
		factor();
	}
	printf("Exit <term>\n");
}

void factor() {
	printf("Enter <factor>\n");
	//어느 RHS를 파싱할것인지 결정
	if (nextToken == IDENT || nextToken == INT_LIT) {
		lex(); // 다음토큰 가져오기
	}
	else {
		if (nextToken == LEFT_PAREN) {
			lex();
			expr();
			if (nextToken == RIGHT_PAREN) {
				lex();
			}
			else {
				error();
			}
		}
		else {
			error();
		}
		printf("Exit <facor>\n");
	}
}

int error() { return -1; }