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
           template<typename CharT, char d, char... bits>
            constexpr CharT binary_eval()
           {
               if constexpr(sizeof...(bits) == 0)
                   return static_cast<CharT>(d-'0');
               else if constexpr (d == '0')
                   return binary_eval<CharT, bits...>();
               else if constexpr (d == '1')
                   return static_cast<CharT>(
                    (1 << sizeof...(bits)) | binary_eval<CharT, bits...>()
                   );
           }
        }

        template<char...bits>
        constexpr byte8 operator""_b8()
        {
            static_assert(sizeof...(bits) <= 8, "Too many bits");
            return binary_literals_internals::binary_eval<byte8, bits...>();
        }

        template<char... bits>
        constexpr byte16 operator""_b16()
        {
            static_assert(sizeof...(bits) <= 16, "Too many bits");
            return binary_literals_internals::binary_eval<byte16, bits...>();
        }
        
        template<char... bits>
        constexpr byte32 operator""_b32()
        {
            static_assert(sizeof...(bits) <= 32, "Too many bits");
            return binary_literals_internals::binary_eval<byte32, bits...>();
        }
    }
}
