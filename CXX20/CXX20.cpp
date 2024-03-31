#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

inline void print_time()
{
    auto now{ std::chrono::system_clock::now() };
    auto stime{ std::chrono::system_clock::to_time_t(now) };
    auto ltime{ std::localtime(&stime) };

    std::cout << std::put_time(ltime, "%c") << '\n';
}

std::mutex g_mutex;

void thread_func()
{
    using namespace std::chrono_literals;
    {
        std::lock_guard lock{g_mutex};
        std::cout << "Running thread " << std::this_thread::get_id() << '\n';
    }

    std::this_thread::yield();
    std::this_thread::sleep_for(2s);

    {
        std::lock_guard lock{g_mutex};
        std::cout << "Done in thread " << std::this_thread::get_id() << '\n';
    }
}

template<typename T>
struct container
{
    std::mutex     mutex;
    std::vector<T> data;
};

template<typename T>
void move_between(container<T>& c1, container<T>& c2, T const value)
{
    std::lock(c1.mutex, c2.mutex);

    std::lock_guard l1{c1.mutex, std::adopt_lock};
    std::lock_guard l2{c2.mutex, std::adopt_lock};

    c1.data.erase(
        std::remove(c1.data.begin(), c1.data.end(), value),
        c1.data.end()
    );
    c2.data.push_back(value);
}

template<typename R>
void print_range(R&& r)
{
    for (auto&& e : r)
    {
        std::cout << e << ' ';
    }
    std::cout << '\n';
}



int main(int argc, char* argv[])
{
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i)
    {
        threads.emplace_back(thread_func);
    }

    for (auto& thread : threads)
    {
        thread.join();
    }


    container<int> c1;
    c1.data.push_back(1);
    c1.data.push_back(2);
    c1.data.push_back(3);
    c1.data.push_back(4);

    std::cout << "c1 before: \n";
    print_range(c1.data);

    container<int> c2;
    c2.data.push_back(5);
    c2.data.push_back(6);
    c2.data.push_back(7);
    c2.data.push_back(8);

    std::cout << "c2 before: \n";
    print_range(c2.data);


    std::cout << "Moving between...." << '\n';

    std::thread t1{move_between<int>, std::ref(c1), std::ref(c2), 3};
    std::thread t2{move_between<int>, std::ref(c2), std::ref(c1), 6};

    t1.join();
    t2.join();

    std::cout << "c1 after: \n";
    print_range(c1.data);
    
    std::cout << "c2 after: \n";
    print_range(c2.data);
}

