
#include <iostream>
#include <format>


struct employee
{
    int         id;
    std::string first_name;
    std::string last_name;
};

template<>
struct std::formatter<employee>
{
    bool lexicographic_order = false;

    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        auto iter = ctx.begin();
        if(iter == ctx.end()) return iter;
        
        auto get_char = [&](){return iter != ctx.end() ? *iter : 0;};

        if(get_char() == ':') ++iter;
        char c = get_char();

        switch (c)
        {
        case '}': return ++iter;
        case 'L': lexicographic_order = true; return ++iter;
        case '{': return ++iter;
        default: throw std::format_error("invalid format!");
        }
    }

    template<typename FormatContext>
    auto format(employee const& e, FormatContext& ctx)
    {
        if(lexicographic_order)
            return std::format_to(
                ctx.out(),
                "[{}] {} {}",
                e.id, e.last_name, e.first_name
            );

        return std::format_to(
            ctx.out(),
            "[{}] {} {}",
            e.id, e.first_name, e.last_name
        );
    }
};



int main(int argc, char* argv[])
{
    try
    {
        auto em = employee{ 123, "John", "Doe" };
        std::cout << std::format("{:L}", em);
    }
    catch (std::format_error const& e)
    {
        std::cout << e.what();
    }
   
}
