add_library(parser parser.cpp)
add_library(compiler compiler.cpp)
target_link_libraries(parser fmt)
