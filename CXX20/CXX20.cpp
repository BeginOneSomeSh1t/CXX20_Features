#include "Source/dist.hpp"

int main(int argc, char* argv[])
{
  std::random_device rd{};
  auto mtgen{std::mt19937{rd()}};
  auto ud {std::uniform_int_distribution<>{1, 6}};
  generate_and_print([&mtgen, &ud](){return ud(mtgen);});
}
