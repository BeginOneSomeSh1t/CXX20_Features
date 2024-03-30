#include <iostream>
#include <string>

#include "Source/compose.hpp"


int main(int argc, char* argv[])
{


    using namespace compose;
    
    auto n =
    ([](int const n){return std::to_string(n);} *
    [](int const n) {return n*n;} *
    [](int const n) {return n + n;} *
    [](int const n) {return std::abs(n);});

    std::cout << n(3);
}

