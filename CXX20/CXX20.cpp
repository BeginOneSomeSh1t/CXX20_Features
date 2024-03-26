#include <iomanip>
#include <iostream>
#include <string>
#include <limits>
#include <sstream>

template<typename T>
auto serialize(T&& value)
{
    std::ostringstream oss;
    oss << std::setprecision(std::numeric_limits<T>::digits10) << value;
    return oss.str();
}


int main(int argc, char* argv[])
{
    auto value {1.23456789012345};
    std::cout << std::setprecision(std::numeric_limits<decltype(value)>::digits10) << value << '\n'; // make sure we show the double at full

    auto value2{0};
    std::cin >> value2;

    if(std::cin.fail() || std::to_string(value2).length() > std::numeric_limits<decltype(value2)>::digits10)
    {
        std::cout << "Invalid input" << std::endl;
        return 0;
    }

    std::cout << std::setprecision(std::numeric_limits<decltype(value2)>::digits10) << value2 << '\n';

    std::cout << serialize(4.56f) << '\n';

    
}
