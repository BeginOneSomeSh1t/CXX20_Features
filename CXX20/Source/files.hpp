#pragma once
#include <fstream>
#include <iostream>
#include <functional>

namespace files
{
    template<typename T>
    bool write_raw_data(std::string_view filename, T const data, size_t const size)
    {
        auto success{ false };
        std::ofstream ofile{ filename.data(), std::ios::binary };
        
        // Make sure file is opened
        if(ofile.is_open())
        {
            try
            {
                ofile.write(data, size);
                success = true;
            }
            catch (std::ios_base::failure&)
            {
                std::cout << "Failed writing " << filename;
            }
            ofile.close();
        }
        return success;
    }

    template<typename T>
    size_t read_raw_data(std::string_view filename,
        std::function<T*(size_t const)> allocator)
    {
        size_t readbytes = 0;
        std::ifstream ifile{ filename.data(), std::ios::binary | std::ios::ate };

        if(ifile.is_open())
        {
            const auto length{ static_cast<size_t>(ifile.tellg()) };
            ifile.seekg(0, std::ios::beg);

            auto buffer{ allocator(length) };

            try
            {
                ifile.read(buffer, length);
                readbytes = static_cast<size_t>(ifile.gcount());
            }
            catch (std::ios::failure&)
            {
                std::cout << "Failed reading " << filename;
            }

            ifile.close();
        }
        return readbytes;
    }

}
