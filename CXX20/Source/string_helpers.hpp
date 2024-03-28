#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <algorithm>

// aliases for convenience
template<typename CharT>
using tstring = std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT>>;

template<typename CharT>
using tstringstream = std::basic_stringstream<CharT, std::char_traits<CharT>, std::allocator<CharT>>;

template<typename CharT>
using tstring_view = std::basic_string_view<CharT, std::char_traits<CharT>>;

namespace string_helpers
{
    template<typename CharT>
    inline tstring<CharT> to_upper(tstring<CharT> text)
    {
        std::transform(std::begin(text), std::end(text), std::begin(text), toupper);
        return text;
    }

    template<typename CharT>
    inline tstring<CharT> to_lower(tstring<CharT> text)
    {
        std::transform(std::begin(text), std::end(text), std::begin(text), tolower);
        return text;
    }

    template<typename CharT>
    inline tstring<CharT> reverse(tstring<CharT> text)
    {
        std::reverse(std::begin(text), std::end(text));
        return text;
    }

    template <typename CharT>
    inline tstring_view<CharT> trim(tstring_view<CharT> text)
    {
        auto first{text.find_first_not_of(' ')};
        auto last{ text.find_last_not_of(' ') };
        return text.substr(first, (last-first + 1));
    }

    template <typename CharT>
   inline tstring_view<CharT> trim(tstring_view<CharT> text, tstring_view<CharT> chars)
    {
        auto first{text.find_first_not_of(chars)};
        auto last{ text.find_last_not_of(chars) };
        return text.substr(first, (last-first + 1));
    }

    template <typename CharT>
    inline tstring_view<CharT> trimleft(tstring_view<CharT> text)
    {
        auto first{text.find_first_not_of(' ')};
        return text.substr(first, text.size() - first);
    }
    template <typename CharT>
    inline tstring_view<CharT> trimright(tstring_view<CharT> text)
    {
        auto last{text.find_last_not_of(' ')};
        return text.substr(0, last + 1);
    }
    template <typename CharT>
    inline tstring_view<CharT> trimleft(tstring_view<CharT> text, tstring_view<CharT> chars)
    {
        auto first{text.find_first_not_of(chars)};
        return text.substr(first, text.size() - first);
    }
    template <typename CharT>
    inline tstring_view<CharT> trimright(tstring_view<CharT> text, tstring_view<CharT> chars)
    {
        auto last{text.find_last_not_of(chars)};
        return text.substr(0, last + 1);
    }

    template <typename CharT>
    inline tstring<CharT> remove(tstring<CharT> text, CharT const ch)
    {
        auto start = std::remove_if(
            std::begin(text), std:: end(text),
            [=](CharT const c) {return c == ch;}
        );
        text.erase(start, std::end(text));
        return text;
    }

    template <typename CharT>
    inline std::vector<tstring<CharT>> split
    (tstring<CharT> text, CharT const delimeter)
    {
        auto sstr = tstringstream<CharT>{text};
        auto tokens = std::vector<tstring<CharT>>{};
        auto token = tstring<CharT>{};

        while(std::getline(sstr, token, delimeter))
        {
            if(!token.empty()) tokens.push_back(token);
        }
        return tokens;
    }











    
}
