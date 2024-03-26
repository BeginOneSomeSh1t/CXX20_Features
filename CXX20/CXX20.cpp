#include <iostream>
#include <vector>
#include <string>

namespace std
{
    template<typename T>
    pair(T&&, const char*) -> pair<T, std::string>;

    template<typename T>
    pair(const char*, T&&) -> pair<std::string, T>;

    pair(const char*, const char*) -> pair<std::string, std::string>;
}



int main(int argc, char* argv[])
{

    auto p{std::pair(42, "Hello World!")};
    
    std::cout << typeid(p).name() << std::endl;
    return 0;
}
