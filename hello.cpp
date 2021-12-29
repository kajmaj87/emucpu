// Your First C++ Program

#include <iostream>

template<typename T>
    concept has_a_name = requires {
        { T::name }
        ->std::convertible_to<const char *>;
    };

int main() {
    std::cout << "Hello World!";
    return 0;
}
