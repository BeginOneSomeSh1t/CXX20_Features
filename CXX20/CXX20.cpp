#include <filesystem>
#include "Source/files.hpp"
#include <iostream>
#include <chrono>


template<typename R>
void print_range(R&& r)
{
    for (auto&& e : r)
    {
        std::cout << e << ' ';
    }
    std::cout << '\n';
}

class foo
{
    int i;
    char c;
    std::string s;
public:
    foo(int const i = 0, char const c = 0, std::string s = {})
        :
    i(i), c(c), s(std::move(s))
    {}

    // constructors
    foo(foo const&) = default;
    foo& operator=(foo const&) = default;
    
    // methods
private:
    bool write(std::ofstream& ofile) const
    {
        ofile.write(reinterpret_cast<const char*>(&i), sizeof(i));
        ofile.write(&c, sizeof(c));

        auto size{ static_cast<int>(s.size()) };
        ofile.write(reinterpret_cast<char*>(&size), sizeof(size));
        ofile.write(s.data(), s.size());

        return !ofile.fail();
    }

    bool read(std::ifstream& ifile)
    {
        ifile.read(reinterpret_cast<char*>(&i), sizeof(i));
        ifile.read(&c, sizeof(c));

        auto size{ 0 };
        ifile.read(reinterpret_cast<char*>(&size), sizeof(size));
        s.resize(size);
        ifile.read(s.data(), size);

        return !ifile.fail();
    }
    
public:
    // operators
    bool operator==(const foo& rhs) const
    {
        return i == rhs.i &&
               c == rhs.c &&
               s == rhs.s;    
    }

    friend std::ofstream& operator<<(std::ofstream& ofile, foo const& v)
    {
        if(!v.write(ofile)) std::cout << "Failed to write a foo data to a file! \n";
        return ofile;
    }

    friend std::ifstream& operator >>(std::ifstream& ifile, foo& v)
    {
        if(!v.read(ifile)) std::cout << "Failed to raed a foo data from a file! \n";
        return ifile;
    }

    bool operator!=(foo const& rhs) const
    {
        return !(*this == rhs);
    }

    friend std::ostream& operator<<(std::ostream& o, foo const& v)
    {
        o << "Foo-non-pod: \n"
        << "i = " << v.i << '\n'
        << "c = " << v.c << '\n'
        << "s = " << v.s << '\n';
        return o;
    }
};

struct foopod
{
    bool a;
    char b;
    int c[2];

    friend std::ostream& operator<<(std::ostream& o, foopod const& v)
    {
        o << "Foopod: \n"
        << "a = " << v.a << '\n' 
        << "b = " << v.b << '\n';
        return o;
    }
};

bool operator==(foopod const& f1, foopod const& f2)
{
    return f1.a == f2.a && f1.b == f2.b &&
        f1.c[0] == f2.c[0] && f1.c[1] == f2.c[1];
}



int main(int argc, char* argv[])
{
    using namespace std::string_literals;
    std::vector<foopod> output{
        {true, '1', {1, 2}},
        {true, '2', {3, 4}},
        {false, '3', {5, 6}}
    };
    
    // Serialization of a pod type
    std::ofstream ofile{ "sample.bin", std::ios::binary };
    if(ofile.is_open())
    {
        for (auto const& value : output)
        {
            ofile.write(reinterpret_cast<const char*>(&value), sizeof(value));
        }
        ofile.close();
    }

    std::vector<foopod> input;
    std::ifstream ifile{ "sample.bin", std::ios::binary };
    if(ifile.is_open())
    {
        while(true)
        {
            foopod value;
            ifile.read(reinterpret_cast<char*>(&value), sizeof(value));

            if(ifile.fail() || ifile.eof()) break;
            input.push_back(value);
        }

        ifile.close();
    }
    
   print_range(input);

    std::vector<foo> nonpod_output{
        {18, '1', "Hello "s},
        {19, '2', "World!"s},
    };

    std::ofstream nonpod_ofile{ "nonpodsample.bin", std::ios::binary };
    if(nonpod_ofile.is_open())
    {
        for (auto const& value : nonpod_output)
        {
            nonpod_ofile << value;
        }
        nonpod_ofile.close();
    }

    std::vector<foo> nonpod_input;
    std::ifstream nonpod_ifile{ "nonpodsample.bin", std::ios::binary };
    if(nonpod_ifile.is_open())
    {
        while(true)
        {
            foo nonpod_foo;
            nonpod_ifile >> nonpod_foo;

            if(nonpod_ifile.fail() || nonpod_ifile.eof()) break;
            nonpod_input.push_back(nonpod_foo);
        }
        nonpod_ifile.close();
    }

    print_range(nonpod_input);
    
}

// 0,2174 with std::copy
// 0.1587 with read_data function
// ~ 0,15 with std::istreambuf_iterator
