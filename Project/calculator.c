#include <stdio.h>	
#include <stdlib.h>
#include <string.h>
#include <math.h>  
#define MAX_STACK_SIZE 100  // 스택 최대 크기


typedef struct {
	int data[MAX_STACK_SIZE];
	int top;
}Stacktype;

void init_stack(Stacktype* s) {
	s->top = -1;
}

int is_empty(Stacktype* s) {
	return s->top == -1;
}

int is_full(Stacktype* s) {
	return (s->top == (MAX_STACK_SIZE - 1));
}

void push(Stacktype* s, int item) {
	if (is_full(s)) {
		fprintf(stderr, "스택 포화 에러\n");
		return;
	}
	s->data[++(s->top)] = item;
}

int pop(Stacktype* s) {
	if (is_empty(s)) {
		fprintf(stderr, "스택 공백 에러\n");
		exit(1);
	}
	else return s->data[s->top--];
}

int peek(Stacktype* s) {
	if (is_empty(s)) {
		fprintf(stderr, "스택 공백 에러\n");
		exit(1);
	}
	else return s->data[s->top];
}
/* -----스택기본골격---- */



// 연산자 우선순위 반환
int prec(char op)
{
	switch (op) {
	case '(': case ')': return 0;
	case '+': case '-': return 1;
	case '*': case '/': return 2;
	case '^': return 3;

	}return -1;

}

// 수식을 입력받아 반환
void get_exp(char* exp)
{
	printf("수식을 입력해주세요: ");
	scanf("%[^\n]s", exp);
}

// 수식에서 한 글자씩(토큰) 반환
char get_token(char* exp, int i)
{
	return exp[i];
}

// 스택을 이용하여 괄호 오류 확인
int check_exp1(char* exp)
{
	char token, open_token;
	int i = 0;
	Stacktype s;
	init_stack(&s);

	for (int i = 0; i < strlen(exp); i++)
	{
		token = exp[i];
		switch (token)
		{
			// 토큰이 열린 괄호일때는 push
		case '(': case'[':case '{':
			push(&s, token);
			break;

			// 닫힌 괄호 라면
		case ')': case']':case '}':
			// 스택이 비어있을 때 -> 오류
			if (is_empty(&s)) {
				return 0;
			}
			else {	// pop한 괄호와 짝이 안맞을 때 -> 오류
				open_token = pop(&s);
				if ((open_token == '(' && token != ')') ||
					(open_token == '[' && token != ']') ||
					(open_token == '{' && token != '}'))
					return 0;
				break;
			}
			// 괄호종류가 아니라면 패스
		default:
			break;
		}

	}
	// 짝이 없는 괄호가 스택에 남아있을 때 -> 오류
	if (!is_empty(&s))
		return 0;
	return 1;

}

// 사용할 수 없는 문자가 포함 되어있는지 확인
int check_exp2(char* exp)
{
	char token;
	for (int i = 0; i < strlen(exp); i++)
	{
		token = exp[i];
		// 숫자가 아니라면 -> 오류
		if (!(token >= '0' && token <= '9'))
			// 연산자가 아니라면 -> 오류
			if (!(token == '+' || token == '-' || token == '*' || token == '/' || token == '^'))
				// 괄호, 공백이 아니라면 -> 오류
				if (!(token == '(' || token == ')' || token == '[' ||
					token == ']' || token == '{' || token == '}' || token == ' '))
					return 0;
	}
}


// 중위식 -> 후위식 변환
void postfix(char* iexp, char* pexp)
{
	int j = 0;
	char token, top_op;

	//스택생성
	Stacktype s;
	init_stack(&s);

	for (int i = 0; i < strlen(iexp); i++) {
		// 수식에서 토큰을 하나 가져와서
		token = get_token(iexp, i);

		switch (token)
		{

		case '+': case '-': case'*':case '/': case '^': //연산자(+,-,*,/,^)이라면
			/* 스택이 비어있지 않고 스택에 들어있던 연산자의 우선순위가
			   토큰의 우선순위보다 크거나 같을때 */
			while (!is_empty(&s) && (prec(token) <= prec(peek(&s))))
			{
				// 공백삽입
				pexp[j++] = ' ';
				// 연산자 삽입
				pexp[j++] = pop(&s);

			}
			// 공백 삽입
			pexp[j++] = ' ';
			// 연산자 push
			push(&s, token);
			break;

			// 열린괄호일 때, 삽입
		case '(': case '{': case '[':
			push(&s, token);
			break;

			// 닫힌 괄호일 때, 짝인 열린괄호를 만날때까지 삽입
		case ')':
			top_op = pop(&s);

			while (top_op != '(') {
				// 공백삽입,괄호 안에 있는 연산자 삽입
				pexp[j++] = ' ';
				pexp[j++] = top_op;
				top_op = pop(&s);
			}break;

			// 닫힌괄호일 때, 짝인 열린괄호를 만날때까지 삽입
		case '}':
			top_op = pop(&s);
			while (top_op != '{') {
				pexp[j++] = ' ';
				pexp[j++] = top_op;
				top_op = pop(&s);
			}break;

			// 닫힌괄호일 때, 짝인 열린괄호를 만날때까지 삽입
		case ']':
			top_op = pop(&s);
			while (top_op != '[') {
				pexp[j++] = ' ';
				pexp[j++] = top_op;
				top_op = pop(&s);
			}break;

			// 공백일 때, 아무것도 하지 않음
		case ' ':
			break;

			// 피연산자일 때 삽입
		default:
			pexp[j++] = token;
			break;
		}

	}
	//나머지 스택에 남아있던 연산자들 삽입
	while (!is_empty(&s)) {
		pexp[j++] = ' ';
		pexp[j++] = pop(&s);

	}
}

// 후위식 계산
int eval(char* pexp)
{
	int first, second, k = 0;
	char token;
	char num[100] = { "" }; // 두 자리 이상의 수를 받기 위해 배열로 선언

	// 사용할 스택 생성
	Stacktype s;
	init_stack(&s);

	for (int i = 0; i < strlen(pexp); i++) {
		// 후위식에서 토큰을 가져옴
		token = get_token(pexp, i);

		// 피연산자일 경우 num에 삽입
		if (token >= '0' && token <= '9')
			num[k++] = token;

		// 피연산자가 아닐 경우
		else {

			// 빈칸이고 num이 비어있지 않을 때
			if (token == ' ' && num[0] != ' ') {
				// num 문자열을 숫자로 바꾸어 스택에 삽입
				push(&s, atoi(num));
				// num은 빈칸, k=0으로 다시 초기화
				for (int j = 0; j < strlen(num); j++)
					num[j] = ' ';
				k = 0;
			}

			// 연산자(+,-,*,/,^)이라면
			else if (token == '+' || token == '-' || token == '*' || token == '/' || token == '^')
			{
				// 스택에 맨위,그 다음 숫자를 pop한 뒤 변수에 저장
				second = pop(&s);
				first = pop(&s);

				// 연산자에 따라 다른 연산을 한 뒤, push
				switch (token) {
				case '+': push(&s, first + second); break;
				case '-': push(&s, first - second); break;
				case '*': push(&s, first * second); break;
				case '/': push(&s, first / second); break;
				case '^': push(&s, pow(first, second)); break;
				}
			}
		}
	}
	// 스택에 남아있는 값을 반환
	return pop(&s);
}




int main(void) {
	// 중위식, 후위식선언
	char iexp[100] = { "" };
	char pexp[100] = { "" };
	// 중위식 입력받기
	get_exp(iexp);

	if (check_exp1(iexp) == 0)
		printf("!!괄호 오류!!\n");
	else if (check_exp2(iexp) == 0)
		printf("!!사용할 수 없는 문자가 포함되어 있습니다!!\n");
	// 두 오류가 없다면 진행
	else {
		// 중위식->후위식 함수
		postfix(iexp, pexp);

		// 결과 출력
		printf("후위수식변환: ");
		puts(pexp);
		printf("결과값: %d\n", eval(pexp));
	}
	return 0;
}