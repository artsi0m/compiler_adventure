/* program Cradle */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fns.h"
#include "dat.h"

int Look; /* Lookahead Character */


/* Read New Character From Input Stream */
void
GetChar(void)
{
     extern int Look;
     Look = getchar();
}

/* Report an Error */
void
Error(char *s)
{
     char error_str[BUFSIZ];

     strlcpy(error_str, "\n Error ", BUFSIZ);
     strlcat(error_str, s, BUFSIZ);
     strlcat(error_str, " .", BUFSIZ);

     fputs(error_str, stderr);

}

/* Report Error and Halt */
void
Abort(char *s)
{
     Error(s);
     exit(1);
}

/* Report What Was Expected */
void
Expected(char *s)
{
     char error_str[BUFSIZ];
     strlcpy(error_str, s, BUFSIZ);
     strlcat(error_str, " Expected", BUFSIZ);
     Abort(error_str);
}

/* Match a Specific Input Character */
void
Match(int x)
{
     extern int Look;
     char s[BUFSIZ];

     if (Look != x){
	  snprintf(s, BUFSIZ, "'''' %c ''''", x);
	  Expected(s);
     }

     GetChar();
}

/* Get an Identifier */
int
GetName(void){
     extern int Look;
     int ch;

     if (isalpha(Look) == 0)
	  Expected("Name");

     ch = toupper(Look);
     GetChar();
     return ch;
}

/* Get a Number */
int
GetNum(void)
{
     extern int Look;
     int ch;

     if (isdigit(Look) == 0)
	  Expected("Integer");

     ch = Look;
     GetChar();
     return ch;
}

/* Output a String with Tab */
void
Emit(char *s)
{
     putchar('\t');
     fputs(s, stdout);
}

/* Output a String with Tab and LF */
void
EmitLn(char *s)
{
     Emit(s);
     putchar('\n');
}

/* Parse and Translate a Math Expression */
void
Term(void)
{
     char s[BUFSIZ];
     int num = GetNum();
     snprintf(s, BUFSIZ, "ldi r16, %c", num);
     EmitLn(s);

}

/* Parse and Translate an Expression */
void
Expression(void)
{
     Term();
     while (Look == '+' || Look == '-'){
	EmitLn("push r16");

     	switch (Look) {
     	case '+':
     		  Add();
     		  break;
     	case '-':
     		  Substract();
     		  break;
     	default:
     		  Expected("Addop");
     		  break;
     	}
     }
}

/* Recognize and Translate an Add */
void
Add(void)
{
     Match('+');
     Term();
     EmitLn("pop r17");
     EmitLn("add r17, r16");
}

/* Recognize and Translate a Substract */
void
Substract(void)
{
     Match('-');
     Term();
     EmitLn("pop r17");
     EmitLn("sub r17, r16");
}

void
Factor(void)
{
     char s[BUFSIZ];
     int num = GetNum();
     
}
/* Initialize and Main Program */
int
main(void)
{
     /* EmitLn(";; Setting stack for function calls");
	 EmitLn("ldi r16, high(RAMEND)");
	 EmitLn("out SPH, r16");
	 EmitLn("ldi r16, low(RAMEND)");
	 EmitLn("out SPL, r16)"); */

     GetChar();
     Expression();
}
