#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <exception>
#include <condition_variable>
#include <queue>
#include <random>

inline void print_time()
{
    auto now{ std::chrono::system_clock::now() };
    auto stime{ std::chrono::system_clock::to_time_t(now) };
    auto ltime{ std::localtime(&stime) };

    std::cout << std::put_time(ltime, "%c") << '\n';
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

std::mutex g_lockprint;
std::mutex g_lockqueue;
std::condition_variable g_queuecheck;
std::queue<int> g_buffer;
bool g_done;

void producer(
     int const                           id,
     std::mt19937&                       generator,
     std::uniform_int_distribution<int>& dsleep,
     std::uniform_int_distribution<int>& dcode
)
{
    for (int i = 0; i < 5; ++i)
    {
        // simulate work
        std::this_thread::sleep_for(
        std::chrono::seconds(dsleep(generator))
        );
    }

    // generate data
    {
        std::unique_lock lock{ g_lockqueue };
        const int value{ id * 100 + dcode(generator) };
        g_buffer.push(value);

        {
            std::unique_lock io_lock{ g_lockprint };
            std::cout << std::format("[produced({})]: {} \n", id, value) << std::flush;
        }
    }

    // notify consumers
    g_queuecheck.notify_one();
}

void consumer()
{
    // loop untill end is signaled
    while(!g_done)
    {
        std::unique_lock lock{ g_lockqueue };

        g_queuecheck.wait_for(
            lock,
            std::chrono::seconds{ 1 },
            [&]{ return !g_buffer.empty(); }
        );

        while(!g_done && !g_buffer.empty())
        {
            std::unique_lock print_lock{ g_lockprint };
            std::cout << std::format("[consumed]: {} \n", g_buffer.front()) << std::flush;
            g_buffer.pop();
        }
    }
}

int main(int argc, char* argv[])
{
    std::array<int, std::mt19937::state_size> seed_data;
    std::random_device rd;
    std::generate(seed_data.begin(), seed_data.end(), std::ref(rd));

    std::seed_seq seq{ seed_data.begin(), seed_data.end() };
    std::mt19937 generator{ seq };
    std::uniform_int_distribution<> dsleep{ 1, 5 };
    std::uniform_int_distribution<> dcode{ 1, 99 };

    std::cout << "Start producing and consuming..." << std::flush << '\n';

    std::thread consumer_thread{ consumer };
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i)
    {
        threads.emplace_back(
                producer,
                i + 1,
                std::ref(generator),
                std::ref(dsleep),
                std::ref(dcode)
        );
    }

    // work for the workers to finish
    for (auto& t : threads)
    {
        t.join();
    }

    // notify the looger to finish and wait for it
    g_done = true;
    consumer_thread.join();

    std::cout << "done producing and consuming...";
}

