#ifndef __EMUCPU_PARSER_H
#define __EMUCPU_PARSER_H

#include <span>
#include <string>
#include <vector>

class Parser {

public:
  std::vector<std::string> parse(std::span<std::string> lines);
};

#endif
