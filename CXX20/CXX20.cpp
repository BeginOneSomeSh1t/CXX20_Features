#include <algorithm>
#include <string>
#include <chrono>
#include <variant>
#include <iostream>

#include "Source/string_helpers.hpp"


enum class genre{drama, action, sf, comedy};

struct movie
{
    std::string title;
    std::chrono::minutes length;
    std::vector<genre> genres;
};

struct track
{
    std::string title;
    std::chrono::minutes length;
};

struct music
{
    std::string title;
    std::string artist;
    std::vector<track> tracks;
};

struct software
{
    std::string title;
    std::string vendor;
};

struct visitor_functor
{
    void operator()(movie const& arg) const
    {
        std::cout << "Movie:" << std::endl;
        std::cout << arg.title << std::endl;
        std::cout << arg.length.count() << std::endl;
    }
    void operator()(music const& arg) const
    {
        std::cout << "Music:" << std::endl;
        std::cout << arg.title << std::endl;
        std::cout << arg.artist << std::endl;
        std::cout << "Tracks:" << std::endl;
        for (auto const& t : arg.tracks)
        {
            std::cout << t.title << std::endl;
            std::cout << t.length.count() << std::endl;
        }
    }

    void operator()(software const& arg) const
    {
        std::cout << "Soft:" << std::endl;
        std::cout << arg.title << std::endl;
        std::cout << arg.vendor << std::endl;
    }
};

using dvd = std::variant<movie, music, software>;

int main(int argc, char* argv[])
{
   std::vector dvds
    {
       dvd{music{"Back In Black", "AC/DC", {track{"Hells Bells", std::chrono::minutes{3}}, track{"Back In Black", std::chrono::minutes{3}}}}},
       dvd{movie{"Interstellar", std::chrono::minutes{120}, {genre::action, genre::sf}}},
       dvd{software{"OpenAI", "OpenAI, Inc."}}
   };

   for (auto& dvd : dvds)
   {
       std::visit([](auto&& arg){std::cout << arg.title << std::endl;}, dvd);
   }

   for (auto const& d : dvds)
   {
       auto result =
           std::visit([](auto&& arg) -> dvd
           {
               auto cpy{arg};
               cpy.title = string_helpers::to_upper(cpy.title);
               return cpy;
           }, d);

       std::visit([](auto&& arg)
       {
           std::cout << arg.title << std::endl;
       }, result);
   }

   /*for (auto const& d : dvds)
   {
       std::visit(visitor_functor(), d);
   }*/

   for (auto const& d : dvds)
   {
       std::visit([](auto&& arg)
       {
           using T = std::decay_t<decltype(arg)>;

           if constexpr (std::is_same_v<T, movie>)
           {
               std::cout << "Movie:" << std::endl;
               std::cout << arg.title << std::endl;
               std::cout << arg.length.count() << std::endl;
           }
           else if constexpr (std::is_same_v<T, music>)
           {
                std::cout << "Music:" << std::endl;
                std::cout << arg.title << std::endl;
                std::cout << arg.artist << std::endl;
                std::cout << "Tracks:" << std::endl;
                for (auto const& t : arg.tracks)
                {
                    std::cout << t.title << std::endl;
                    std::cout << t.length.count() << std::endl;
                }
           }
           else if constexpr (std::is_same_v<T, software>)
           {
               std::cout << "Soft:" << std::endl;
               std::cout << arg.title << std::endl;
               std::cout << arg.vendor << std::endl;
           }
       }, d);
   }
}

