add_executable(test_emucpu test_emucpu.cpp)
target_link_libraries(test_emucpu gtest)
add_test(Emucpu-Test test_emucpu)
