
/* { Variable Declarations } */

/* var Look: char;              { Lookahead Character } */
                              
/* {--------------------------------------------------------------} */
/* { Read New Character From Input Stream } */

/* procedure GetChar; */
/* begin */
/*    Read(Look); */
/* end; */

/* {--------------------------------------------------------------} */
/* { Report an Error } */

/* procedure Error(s: string); */
/* begin */
/*    WriteLn; */
/*    WriteLn(^G, 'Error: ', s, '.'); */
/* end; */


/* {--------------------------------------------------------------} */
/* { Report Error and Halt } */

/* procedure Abort(s: string); */
/* begin */
/*    Error(s); */
/*    Halt; */
/* end; */


/* {--------------------------------------------------------------} */
/* { Report What Was Expected } */

/* procedure Expected(s: string); */
/* begin */
/*    Abort(s + ' Expected'); */
/* end; */

/* {--------------------------------------------------------------} */
/* { Match a Specific Input Character } */

/* procedure Match(x: char); */
/* begin */
/*    if Look = x then GetChar */
/*    else Expected('''' + x + ''''); */
/* end; */


/* {--------------------------------------------------------------} */
/* { Recognize an Alpha Character } */

/* function IsAlpha(c: char): boolean; */
/* begin */
/*    IsAlpha := upcase(c) in ['A'..'Z']; */
/* end; */
                              

/* {--------------------------------------------------------------} */

/* { Recognize a Decimal Digit } */

/* function IsDigit(c: char): boolean; */
/* begin */
/*    IsDigit := c in ['0'..'9']; */
/* end; */


/* {--------------------------------------------------------------} */
/* { Get an Identifier } */

/* function GetName: char; */
/* begin */
/*    if not IsAlpha(Look) then Expected('Name'); */
/*    GetName := UpCase(Look); */
/*    GetChar; */
/* end; */


/* {--------------------------------------------------------------} */
/* { Get a Number } */

/* function GetNum: char; */
/* begin */
/*    if not IsDigit(Look) then Expected('Integer'); */
/*    GetNum := Look; */
/*    GetChar; */
/* end; */


/* {--------------------------------------------------------------} */
/* { Output a String with Tab } */

/* procedure Emit(s: string); */
/* begin */
/*    Write(TAB, s); */
/* end; */




/* {--------------------------------------------------------------} */
/* { Output a String with Tab and CRLF } */

/* procedure EmitLn(s: string); */
/* begin */
/*    Emit(s); */
/*    WriteLn; */
/* end; */

/* {--------------------------------------------------------------} */
/* { Initialize } */

/* procedure Init; */
/* begin */
/*    GetChar; */
/* end; */


/* {--------------------------------------------------------------} */
/* { Main Program } */

/* begin */
/*    Init; */
/* end. */
/* {--------------------------------------------------------------} */
#include <cctype>
#include <fmt/core.h>
#include <iostream>
#include <stdio.h>
#include <string>

std::string line;
uint8_t pos;
char current;
char next;

void expected(std::string error_msg) { throw std::runtime_error(error_msg); }


char get_char() {
  pos++;
  current = line[pos];
  next = line[pos + 1];
  return current;
}

char get_num() {
  if (!isdigit(next)) {
    expected("Number was expected!");
  } else {
    return get_char();
  }
}

void match(char c) {
  if (c != next) {
    expected(
        fmt::format("Expected {} char at this point but got {}!", c, next));
  } else {
    get_char();
  }
}

void emit(std::string str) { printf("\t%s", str.c_str()); }

void emit_ln(std::string str) { emit(str.append("\n")); }

void term() {
  std::string str = "LOAD R0, ";
  str += get_num();
  emit_ln(str);
}

void add() {
  match('+');
  term();
  emit_ln("ADD R0, R0, R1");
}

void subtract() {
  match('-');
  term();
  emit_ln("SUB R0, R0, R1");
}

void expression() {
  term();
  emit_ln("MOVE R1, R0");
  switch (next) {
  case '+': {
    add();
    break;
  }
  case '-': {
    subtract();
    break;
  }
  default:
    expected("Operator was expected");
  }
}

void init() {
  std::getline(std::cin, line);
  pos = -1;
  next = line[0];
  current = NULL;
}

int main() {
  char c;
  init();
  expression();
  return 0;
}
