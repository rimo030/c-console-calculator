#include <stdio.h>	
#include <stdlib.h>
#include <string.h>
#include <math.h>  
#define MAX_STACK_SIZE 100  // ���� �ִ� ũ��


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
		fprintf(stderr, "���� ��ȭ ����\n");
		return;
	}
	s->data[++(s->top)] = item;
}

int pop(Stacktype* s) {
	if (is_empty(s)) {
		fprintf(stderr, "���� ���� ����\n");
		exit(1);
	}
	else return s->data[s->top--];
}

int peek(Stacktype* s) {
	if (is_empty(s)) {
		fprintf(stderr, "���� ���� ����\n");
		exit(1);
	}
	else return s->data[s->top];
}
/* -----���ñ⺻���---- */



// ������ �켱���� ��ȯ
int prec(char op)
{
	switch (op) {
	case '(': case ')': return 0;
	case '+': case '-': return 1;
	case '*': case '/': return 2;
	case '^': return 3;

	}return -1;

}

// ������ �Է¹޾� ��ȯ
void get_exp(char* exp)
{
	printf("������ �Է����ּ���: ");
	scanf("%[^\n]s", exp);
}

// ���Ŀ��� �� ���ھ�(��ū) ��ȯ
char get_token(char* exp, int i)
{
	return exp[i];
}

// ������ �̿��Ͽ� ��ȣ ���� Ȯ��
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
			// ��ū�� ���� ��ȣ�϶��� push
		case '(': case'[':case '{':
			push(&s, token);
			break;

			// ���� ��ȣ ���
		case ')': case']':case '}':
			// ������ ������� �� -> ����
			if (is_empty(&s)) {
				return 0;
			}
			else {	// pop�� ��ȣ�� ¦�� �ȸ��� �� -> ����
				open_token = pop(&s);
				if ((open_token == '(' && token != ')') ||
					(open_token == '[' && token != ']') ||
					(open_token == '{' && token != '}'))
					return 0;
				break;
			}
			// ��ȣ������ �ƴ϶�� �н�
		default:
			break;
		}

	}
	// ¦�� ���� ��ȣ�� ���ÿ� �������� �� -> ����
	if (!is_empty(&s))
		return 0;
	return 1;

}

// ����� �� ���� ���ڰ� ���� �Ǿ��ִ��� Ȯ��
int check_exp2(char* exp)
{
	char token;
	for (int i = 0; i < strlen(exp); i++)
	{
		token = exp[i];
		// ���ڰ� �ƴ϶�� -> ����
		if (!(token >= '0' && token <= '9'))
			// �����ڰ� �ƴ϶�� -> ����
			if (!(token == '+' || token == '-' || token == '*' || token == '/' || token == '^'))
				// ��ȣ, ������ �ƴ϶�� -> ����
				if (!(token == '(' || token == ')' || token == '[' ||
					token == ']' || token == '{' || token == '}' || token == ' '))
					return 0;
	}
}


// ������ -> ������ ��ȯ
void postfix(char* iexp, char* pexp)
{
	int j = 0;
	char token, top_op;

	//���û���
	Stacktype s;
	init_stack(&s);

	for (int i = 0; i < strlen(iexp); i++) {
		// ���Ŀ��� ��ū�� �ϳ� �����ͼ�
		token = get_token(iexp, i);

		switch (token)
		{

		case '+': case '-': case'*':case '/': case '^': //������(+,-,*,/,^)�̶��
			/* ������ ������� �ʰ� ���ÿ� ����ִ� �������� �켱������
			   ��ū�� �켱�������� ũ�ų� ������ */
			while (!is_empty(&s) && (prec(token) <= prec(peek(&s))))
			{
				// �������
				pexp[j++] = ' ';
				// ������ ����
				pexp[j++] = pop(&s);

			}
			// ���� ����
			pexp[j++] = ' ';
			// ������ push
			push(&s, token);
			break;

			// ������ȣ�� ��, ����
		case '(': case '{': case '[':
			push(&s, token);
			break;

			// ���� ��ȣ�� ��, ¦�� ������ȣ�� ���������� ����
		case ')':
			top_op = pop(&s);

			while (top_op != '(') {
				// �������,��ȣ �ȿ� �ִ� ������ ����
				pexp[j++] = ' ';
				pexp[j++] = top_op;
				top_op = pop(&s);
			}break;

			// ������ȣ�� ��, ¦�� ������ȣ�� ���������� ����
		case '}':
			top_op = pop(&s);
			while (top_op != '{') {
				pexp[j++] = ' ';
				pexp[j++] = top_op;
				top_op = pop(&s);
			}break;

			// ������ȣ�� ��, ¦�� ������ȣ�� ���������� ����
		case ']':
			top_op = pop(&s);
			while (top_op != '[') {
				pexp[j++] = ' ';
				pexp[j++] = top_op;
				top_op = pop(&s);
			}break;

			// ������ ��, �ƹ��͵� ���� ����
		case ' ':
			break;

			// �ǿ������� �� ����
		default:
			pexp[j++] = token;
			break;
		}

	}
	//������ ���ÿ� �����ִ� �����ڵ� ����
	while (!is_empty(&s)) {
		pexp[j++] = ' ';
		pexp[j++] = pop(&s);

	}
}

// ������ ���
int eval(char* pexp)
{
	int first, second, k = 0;
	char token;
	char num[100] = { "" }; // �� �ڸ� �̻��� ���� �ޱ� ���� �迭�� ����

	// ����� ���� ����
	Stacktype s;
	init_stack(&s);

	for (int i = 0; i < strlen(pexp); i++) {
		// �����Ŀ��� ��ū�� ������
		token = get_token(pexp, i);

		// �ǿ������� ��� num�� ����
		if (token >= '0' && token <= '9')
			num[k++] = token;

		// �ǿ����ڰ� �ƴ� ���
		else {

			// ��ĭ�̰� num�� ������� ���� ��
			if (token == ' ' && num[0] != ' ') {
				// num ���ڿ��� ���ڷ� �ٲپ� ���ÿ� ����
				push(&s, atoi(num));
				// num�� ��ĭ, k=0���� �ٽ� �ʱ�ȭ
				for (int j = 0; j < strlen(num); j++)
					num[j] = ' ';
				k = 0;
			}

			// ������(+,-,*,/,^)�̶��
			else if (token == '+' || token == '-' || token == '*' || token == '/' || token == '^')
			{
				// ���ÿ� ����,�� ���� ���ڸ� pop�� �� ������ ����
				second = pop(&s);
				first = pop(&s);

				// �����ڿ� ���� �ٸ� ������ �� ��, push
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
	// ���ÿ� �����ִ� ���� ��ȯ
	return pop(&s);
}




int main(void) {
	// ������, �����ļ���
	char iexp[100] = { "" };
	char pexp[100] = { "" };
	// ������ �Է¹ޱ�
	get_exp(iexp);

	if (check_exp1(iexp) == 0)
		printf("!!��ȣ ����!!\n");
	else if (check_exp2(iexp) == 0)
		printf("!!����� �� ���� ���ڰ� ���ԵǾ� �ֽ��ϴ�!!\n");
	// �� ������ ���ٸ� ����
	else {
		// ������->������ �Լ�
		postfix(iexp, pexp);

		// ��� ���
		printf("�������ĺ�ȯ: ");
		puts(pexp);
		printf("�����: %d\n", eval(pexp));
	}
	return 0;
}