/* program Cradle */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fns.h"

#define TAB '\t'
#define BYTE unsigned char
#define BUFSIZ 1024

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
Error(BYTE *s)
{
     BYTE error_str[BUFSIZ];
     
     strlcpy(error_str, "\n Error ", BUFSIZ);
     strlcat(error_str, s, BUFSIZ);
     strlcat(error_str, " .", BUFSIZ);

     fputs(error_str, stderr);
     
}

/* Report Error and Halt */
void
Abort(BYTE *s)
{
     Error(s);
     exit(1);
}

/* Report What Was Expected */
void
Expected(BYTE *s)
{
     BYTE error_str[BUFSIZ];
     strlcpy(error_str, s, BUFSIZ);
     strlcat(error_str, " Expected", BUFSIZ);
     Abort(error_str);
}

/* Match a Specific Input Character */
void
Match(int x)
{
     extern int Look;
     BYTE s[BUFSIZ];
     
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
Emit(BYTE *s)
{
     putchar('\t');
     fputs(s, stdout);
}

/* Output a String with Tab and LF */
void
EmitLn(BYTE *s)
{
     Emit(s);
     putchar('\n');
}

/* Parse and Translate a Math Expression */
void
Term(void)
{
     BYTE s[BUFSIZ];
     int num = GetNum();
     snprintf(s, BUFSIZ, "ldi r16, %c", num);
     EmitLn(s);
	  
}

/* Parse and Translate an Expression */
void
Expression(void)     
{
     Term();
     EmitLn("mov r17, r16");

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

/* Recognize and Translate an Add */
void
Add(void)
{
     Match('+');
     Term();
     EmitLn("add r17, r16");
}

/* Recognize and Translate a Substract */
void
Substract(void)
{
     Match('-');
     Term();
     EmitLn("sub r17, r16");
}

/* Initialize and Main Program */
int
main(void)
{
     GetChar();
     Expression();
}
