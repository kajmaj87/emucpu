#include "parser.h"
#include <cctype>
#include <fmt/core.h>
#include <iostream>
#include <stdio.h>
#include <string>

std::string line;
uint8_t pos;
char current;
char next;

void expected(const std::string &error_msg) {
  throw std::runtime_error(error_msg);
}

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
  std::string str = "MOVE R0 ";
  str += get_num();
  emit_ln(str);
}

void add() {
  match('+');
  term();
  emit_ln("ADD R0 R0 R1");
}

void subtract() {
  match('-');
  term();
  emit_ln("SUB R0 R1 R0");
}

void expression() {
  term();
  while (next == '-' || next == '+') {
    emit_ln("MOVE R1 R0");
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
}

void init_for_next_line(std::string next_line) {
  line = next_line;
  pos = -1;
  next = line[0];
  current = NULL;
}

std::vector<std::string> Parser::parse(std::span<std::string> lines) {
  for (auto current_line : lines) {
    init_for_next_line(current_line);
    expression();
  }
}
