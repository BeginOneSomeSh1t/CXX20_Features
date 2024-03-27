#pragma once

namespace binary
{
    using byte8 = unsigned char;
    using byte16 = unsigned short;
    using byte32 = unsigned int;

    namespace binary_literals
    {
        namespace binary_literals_internals
        {
            template<typename  CharT, char... bits>
            struct binary_struct;

            template<typename CharT, char... bits>
            struct binary_struct<CharT, '0', bits...>
            {
                static constexpr CharT value
                {
                    binary_struct<CharT, bits...>::value
                };
            };

            template<typename CharT, char... bits>
            struct binary_struct<CharT, '1', bits...>
            {
                static constexpr CharT value{
                    static_cast<CharT>(1 << sizeof...(bits)) |
                    binary_struct<CharT, bits...>::value
                };
            };

            template<typename CharT>
            struct binary_struct<CharT>
            {
                static constexpr CharT value{0};
            };
        }

        template<char...bits>
        constexpr byte8 operator""_b8()
        {
            static_assert(sizeof...(bits) <= 8, "Too many bits");
            return binary_literals_internals::binary_struct<byte8, bits...>::value;
        }

        template<char... bits>
        constexpr byte16 operator""_b16()
        {
            static_assert(sizeof...(bits) <= 16, "Too many bits");
            return binary_literals_internals::binary_struct<byte16, bits...>::value;
        }
        
        template<char... bits>
        constexpr byte32 operator""_b32()
        {
            static_assert(sizeof...(bits) <= 32, "Too many bits");
            return binary_literals_internals::binary_struct<byte32, bits...>::value;
        }
    }
}
