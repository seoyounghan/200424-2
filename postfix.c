#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum {
   lparen = 0,  /* ( 왼쪽 괄호 */
   rparen = 9,  /* ) 오른쪽 괄호*/
   times = 7,   /* * 곱셈 */
   divide = 6,  /* / 나눗셈 */
   plus = 5,    /* + 덧셈 */
   minus = 4,   /* - 뺄셈 */
   operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];      /* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];   /* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];   /* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];      /* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;      /* evalStack용 top */

int evalResult = 0;      /* 계산 결과를 저장 */

void postfixPush(char x)
{
   postfixStack[++postfixStackTop] = x;
}

char postfixPop()
{
   char x;
   if (postfixStackTop == -1)
      return '\0';
   else {
      x = postfixStack[postfixStackTop--];
   }
   return x;
}


void evalPush(int x)
{
   evalStack[++evalStackTop] = x;
}

int evalPop()
{
   if (evalStackTop == -1)
      return -1;
   else
      return evalStack[evalStackTop--];
}


/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()
{
   printf("Type the expression >>> ");
   scanf("%s", infixExp);
}

precedence getToken(char symbol)
{
   switch (symbol) {
   case '(': return lparen;
   case ')': return rparen;
   case '+': return plus;
   case '-': return minus;
   case '/': return divide;
   case '*': return times;
   default: return operand;
   }

}

precedence getPriority(char x)
{
   return getToken(x);
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{
   if (postfixExp == '\0')
      strncpy(postfixExp, c, 1);
   else
      strncat(postfixExp, c, 1);
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
   /* infixExp의 문자 하나씩을 읽기위한 포인터 */
   char* exp = infixExp;
   char x; /* 문자하나를 임시로 저장하기 위한 변수 */
   char temp;
   int n = 0;

   /* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
   while (*exp != '\0')
   {
      /* 필요한 로직 완성 */
      x = getToken(*exp);      // x에 exp가 가리키는 infixExp 원소의 우선순위를 저장한다.
      if (x == operand)   // x가 피연산자라면
         charCat(exp);   // 그 값을 postfixExp에 저장한다.
      else if (x == rparen)   // x가 오른쪽 괄호라면
      {
         /* 왼쪽 괄호가 나올 때까지 토큰들을 제거해서 출력시킴 */
         while (getToken(postfixStack[postfixStackTop]) != lparen)
         {
            temp = postfixPop();   // temp에 postfixStack의 top에 있는 연산자 저장하고
            charCat(&temp);         // 그 연산자를 postfixExp에 저장한다.
         }
         postfixPop();      // 왼쪽 괄호를 제거한다.
      }
      else if (postfixStackTop == -1)      // x가 오른쪽 괄호가 아닌 연산자이고 postfixStack이 비어있다면
      {
         postfixPush(*exp);      // 그 연산자를 postfixStack에 push한다.
      }
      else if (x > getToken(postfixStack[postfixStackTop]))      // x가 postfixStack의 top에 있는 연산자보다 우선순위가 높으면
      {
         if (x == 7 && getToken(postfixStack[postfixStackTop]) == 6)   // *와 /의 우선순위는 원래 같으므로
         {
            temp = postfixPop();   // temp에 postfixStack의 top에 있는 연산자를 저장하고
            postfixPush(*exp);      // postfixStack에 exp가 가리키는 infixExp의 연산자를 push하고
            charCat(&temp);      // temp의 연산자를 postfixExp에 저장한다.
         }
         else if (x == 5 && getToken(postfixStack[postfixStackTop]) == 4)   // +와 -의 우선순위는 원래 같으므로
         {
            temp = postfixPop();   // temp에 postfixStack의 top에 있는 연산자를 저장하고
            postfixPush(*exp);      // postfixStack에 exp가 가리키는 infixExp의 연산자를 push하고
            charCat(&temp);      // temp의 연산자를 postfixExp에 저장한다.
         }
         else
         {
            postfixPush(*exp);   // 우선순위가 높으면 스택에 push.
         }
      }
      else if (x == getToken(postfixStack[postfixStackTop]))   // x와 postfixStack의 top에 있는 연산자와 우선순위가 같다면
      {
         temp = postfixPop();   // temp에 postfixStack의 top에 있는 연산자를 저장하고
         postfixPush(*exp);      // postfixStack에 exp가 가리키는 infixExp의 연산자를 push하고
         charCat(&temp);      // temp의 연산자를 postfixExp에 저장한다.
      }
      else if (x < getToken(postfixStack[postfixStackTop]))   // x가 postfixStack의 top에 있는 연산자보다 우선순위가 낮다면
      {
         while (postfixStackTop != -1 && x != lparen && x < getToken(postfixStack[postfixStackTop]))
         {   // postfixStack이 비어있지 않고 x가 왼쪽 괄호가 아니며 x가 postfixStack의 top에 있는 연산자보다 우선순위가 낮으면 반복한다.
            temp = postfixPop();   // temp에 postfixStack의 top에 있는 연산자를 저장하고
            charCat(&temp);      // temp의 연산자를 postfixExp에 저장한다.
         }
         postfixPush(*exp);      // x보다 높은 우선순위의 연산자가 없으면 postfixStack에 push한다.
      }
      exp++;   // infixExp의 다음 문자로 포인터를 이동한다.
   }

   /* 필요한 로직 완성 */
   while (postfixStackTop != -1)   // 중위 표기에서 후위 표기로의 변환이 끝나고 남은 연산자들을 postfixExp에 저장한다.
   {
      temp = postfixPop();   // temp에 postfixStack의 top에 있는 연산자를 저장하고
      charCat(&temp);      // temp의 연산자를 postfixExp에 저장한다.
   }   // postfixStack이 빌 때까지 반복한다.
}
void debug()
{
   printf("\n---DEBUG\n");
   printf("infixExp =  %s\n", infixExp);
   printf("postExp =  %s\n", postfixExp);
   printf("eval result = %d\n", evalResult);

   printf("postfixStack : ");
   for (int i = 0; i < MAX_STACK_SIZE; i++)
      printf("%c  ", postfixStack[i]);

   printf("\n");

}

void reset()
{
   infixExp[0] = '\0';
   postfixExp[0] = '\0';

   for (int i = 0; i < MAX_STACK_SIZE; i++)
      postfixStack[i] = '\0';

   postfixStackTop = -1;
   evalStackTop = -1;
   evalResult = 0;
}

void evaluation()
{
   /* postfixExp, evalStack을 이용한 계산 */

   /* postfixExp의 문자 하나씩을 읽기위한 포인터 */
   char* exp = postfixExp;
   char x; /* 문자하나를 임시로 저장하기 위한 변수 */
   
   int num1, num2;      // 계산할 두 숫자를 저장할 변수

   /* exp를 증가시켜가면서, 문자를 읽고 계산 */
   while (*exp != '\0')
   {
      x = getToken(*exp);      // x에 exp가 가리키는 postfixExp 원소의 우선순위를 저장한다.
      if (x == operand)   // x가 피연산자라면
         evalPush((*exp) - '0');   // evalStack에 문자를 숫자로 바꾸어 push한다.
      else // 연산자라면
      {
         num2 = evalPop();   // 계산할 첫번째 피연산자를 evalStack에서 pop한다.
         num1 = evalPop();   // 계산할 두번째 피연산자를 evalStack에서 pop한다.
         // stack은 나중에 들어온 값이 먼저 나오므로 정상적인 계산을 위해 num2에 먼저 pop해준다.

         switch (x)   // 연산자에 따라 계산하고 값을 evalStack에 push한다.
         {
         case plus:
            evalPush(num1 + num2);   // + 연산자라면 num1 + num2를 evalStack에 push한다.
            break;
         case minus:
            evalPush(num1 - num2);   // - 연산자라면 num1 - num2를 evalStack에 push한다.
            break;
         case times:
            evalPush(num1 * num2);   // * 연산자라면 num1 * num2를 evalStack에 push한다.
            break;
         case divide:
            evalPush(num1 / num2);   // / 연산자라면 num1 / num2를 evalStack에 push한다.
            break;
         default:
            break;
         }
      }
      exp++;   // postfixExp의 다음 문자로 포인터를 이동한다.
   }

   evalResult = evalPop();      // 계산된 결과를 pop하여 저장한다.
}


int main()
{
   char command;

   do {printf("-----2019038038 한서영---------\n");
      printf("----------------------------------------------------------------\n");
      printf("               Infix to Postfix, then Evaluation               \n");
      printf("----------------------------------------------------------------\n");
      printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
      printf("----------------------------------------------------------------\n");

      printf("Command = ");
      scanf(" %c", &command);

      switch (command) {
      case 'i': case 'I':
         getInfix();
         break;
      case 'p': case 'P':
         toPostfix();
         break;
      case 'e': case 'E':
         evaluation();
         break;
      case 'd': case 'D':
         debug();
         break;
      case 'r': case 'R':
         reset();
         break;
      case 'q': case 'Q':
         break;
      default:
         printf("\n       >>>>>   Concentration!!   <<<<<     \n");
         break;
      }

   } while (command != 'q' && command != 'Q');

   return 0;


}
