#include <algorithm>
#include <iostream>
#include <unordered_set>
#include "Source/time.hpp"
using namespace std::string_literals;

struct Item
{
    int id;
    std::string name;
    double value;

    Item(int const id, std::string const& name, double const value)
        :
    id(id), name(name), value(value)
    {}

    bool operator==(const Item& other) const
    {
        return id == other.id && name == other.name && value == other.value;
    }

    // Overload the '<<' operator for Item
    friend std::ostream& operator<<(std::ostream& os, const Item& item) {
        os << "ID: " << item.id << ", Name: " << item.name << ", Value: " << item.value;
        return os;
    }
};

namespace std
{
    template<>
    struct hash<Item>
    {
        typedef Item argument_type;
        typedef size_t result_type;

        result_type operator()(argument_type const& item) const
        {
            result_type hash_val = 17;
            hash_val = 31 * hash_val + std::hash<int>()(item.id);
            hash_val = 31 * hash_val + std::hash<std::string>()(item.name);
            hash_val = 31 * hash_val + std::hash<double>()(item.value);

            return hash_val;
        }
    };
}


int main(int argc, char* argv[])
{
    std::unordered_set<Item> set{
        {1, "one", 1.0},
        {2, "two", 2.0},
        {3, "three", 3.0},
    };

    for (const Item& item : set) {
        std::cout << item << std::endl;
    }

    return 0;
}

