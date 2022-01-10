#ifndef __EMUCPU_PARSER_H
#define __EMUCPU_PARSER_H

#include <span>
#include <string>
#include <vector>

class Parser {

public:
  const std::vector<std::string> parse(const std::span<std::string> &lines);
};

#endif
