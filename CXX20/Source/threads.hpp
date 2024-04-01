#pragma once
#include <thread>

static unsigned get_num_of_threads()
{
    return std::thread::hardware_concurrency();
}