#include <iostream>
#include "calc.h"

int main(int argc, char** argv)
{
    if (argc <= 1) {
        std::cout << "Usage: " << argv[0] << " <expression>" << std::endl;
        return EXIT_SUCCESS;
    }
    const char* expression = argv[1];
    try {
        int result = Calculator::calc(expression);
        std::cout << "'" << expression << " = " << result << "'" << std::endl;
    }
    catch (std::exception &e) {
        std::cerr << "Couln't calculate expression '" << expression << "'" << std::endl;
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch(...) {
        std::cerr << "Couln't calculate expression '" << expression << "'" << std::endl;
        std::cerr << "Unknown error" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
