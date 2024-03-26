#include <iostream>
#include "Source/dummy_array.hpp"

int main(int argc, char* argv[])
{
  dummy_array<int, 3> arr;
    arr.set_at(0, 6);
    arr.set_at(1, 7);
    arr.set_at(2, 8);
    for (auto i : arr) {
        std::cout << i << std::endl;
    }
}
