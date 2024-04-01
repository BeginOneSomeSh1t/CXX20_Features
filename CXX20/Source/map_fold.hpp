#pragma once
#include <algorithm>
#include <future>
#include <map>
#include <numeric>
#include <queue>
#include "threads.hpp"


namespace higher_order_functions
{

    static constexpr int concurrency_threshold{ 500'000 };
    
    inline namespace map_f
    {
        template<typename F, typename R>
       R mapf(F&& func, R range)
        {
            std::transform(
            std::begin(range), std::end(range), std::begin(range),
            std::forward<F>(func)
            );
            return range;
        }

        template<typename F, typename T, typename U>
        std::map<T, U> mapf(F&& func, std::map<T, U> const& m)
        {
            std::map<T, U> r;
            for(auto const kvp : m)
                r.insert(func(kvp));
            return r;
        }

        template<typename F, typename T>
        std::queue<T> mapf(F&& func, std::queue<T> q)
        {
            std::queue<T> r;
            r.reserve(q.size());
            while(!q.empty())
            {
                r.push(func(q.front()));
                q.pop();
            }
            return r;
        }

        template<typename Iter, typename F>
        void parallel_map(Iter begin, Iter end, F f)
        {
            const auto size{ std::distance(begin, end) };

            // If the size is less than the threshold, map in a sequential order
            if(size <= concurrency_threshold)
            {
                std::transform(begin, end, begin, std::forward<F>(f));
            }
            else
            {
                const auto num_of_threads{ get_num_of_threads() };
                const auto part{ size / num_of_threads };
                auto last{ begin };

                std::vector<std::thread> threads;
                for (unsigned i = 0; i < num_of_threads; ++i)
                {
                    if(i == num_of_threads - 1) last = end;
                    else std::advance(last, part);

                    threads.emplace_back(
                    [=, &f]{std::transform(begin, last, begin, std::forward<F>(f));}
                    );

                    begin = last;
                }

                // Join all threads
                for (auto& t : threads)
                {
                    t.join();
                }
            }
        }

        template<typename Iter, typename F>
        void async_map(Iter begin, Iter end, F f)
        {
            const auto size{ std::distance(begin, end) };
            if(size <= concurrency_threshold)
            {
                std::transform(begin, end, begin, std::forward<F>(f));
            }
            else
            {
                const auto num_of_tasks{ get_num_of_threads() };
                const auto part{ size / num_of_tasks };
                auto last{ begin };

                std::vector<std::future<void>> tasks;
                for (unsigned i = 0; i < num_of_tasks; ++i)
                {
                    if(i == num_of_tasks - 1) last = end;
                    else std::advance(last, part);

                    tasks.emplace_back(
                          std::async(
                               std::launch::async,
                               [=, &f]{ std::transform(begin, last, begin, std::forward<F>(f)); }
                          )  
                    );

                    begin = last;
                }

                for (auto& task : tasks)
                {
                    task.wait();
                }
            }
        }
    }

    inline namespace fold_f
    {
        template<typename F, typename R, typename T>
        constexpr T foldl(F&& func, R&& range, T init)
        {
            return std::accumulate(
                std::begin(range), std::end(range),
                std::move(init),
                std::forward<F>(func)
                );
        }

        template<typename F, typename R, typename T>
        constexpr T foldr(F&& func, R&& range, T init)
        {
            return std::accumulate(
                std::rbegin(range), std::rend(range),
                std::move(init),
                std::forward<F>(func)
                );
        }

        template<typename F, typename T>
        constexpr T foldl(F&& func, std::queue<T> q, T init)
        {
            while(!q.empty())
            {
                init = func(init, q.front());
                q.pop();
            }
            return init;
        }

        template<typename F, typename T1, typename T2>
        auto tfoldl(F&& func, T1 arg1, T2 arg2)
        {
            return func(arg1, arg2);
        }

        template<typename F, typename T, typename... Ts>
        auto tfoldl(F&& func, T head, Ts... rest)
        {
            return func(head, tfoldl(std::forward<F>(func), rest...));
        }

        template<typename Iter, typename R, typename F>
        auto parallel_foldl(Iter begin, Iter end, R init, F op)
        {
            const auto size{ std::distance(begin, end) };
            if(size <= concurrency_threshold)
                return std::accumulate(begin, end, init, std::forward<F>(op));
            else
            {
                const auto num_of_threads{ get_num_of_threads() };
                const auto part{ size / num_of_threads };
                auto last{ begin };

                std::vector<std::thread> threads;
                std::vector<R> values(num_of_threads);

                for (unsigned i = 0; i < num_of_threads; ++i)
                {
                    if(i == num_of_threads - 1) last = end;
                    else std::advance(last, part);

                    threads.emplace_back(
                            [=, &op](R& result)
                            {
                                result = std::accumulate(begin, last, R{}, std::forward<F>(op));
                            },
                            std::ref(values[i])
                    );

                    begin = last;
                }
                for (auto& thread : threads)
                {
                    thread.join();
                }

                return std::accumulate(std::begin(values), std::end(values), init, std::forward<F>(op));
            }
        }

        template<typename Iter, typename R, typename F>
        auto async_foldl(Iter begin, Iter end, R init, F f)
        {
            const auto size{ std::distance(begin, end) };
            if(size <= concurrency_threshold)
            {
                return std::accumulate(begin, end, init, std::forward<F>(f));
            }
            else
            {
                const auto num_of_tasks{ get_num_of_threads() };
                const auto part{ size / num_of_tasks };
                auto last{ begin };

                std::vector<std::future<R>> tasks;
                for (unsigned i = 0; i < num_of_tasks; ++i)
                {
                    if(i == num_of_tasks - 1) last = end;
                    else std::advance(last, part);

                    tasks.emplace_back( 
                          std::async(
                               std::launch::async,
                               [=, &f]{ return std::accumulate(begin, last, R{}, std::forward<F>(f)); }
                          )  
                     );

                    begin = last;
                }

                std::vector<R> values;
                values.reserve(num_of_tasks);
                for (auto& fut : tasks)
                {
                    values.push_back(fut.get());
                }

                return std::accumulate(std::begin(values), std::end(values), init, std::forward<F>(f));
            }
        }
    }

    template<typename T = double>
    struct fround
    {
        typename std::enable_if_t<std::is_floating_point_v<T>, T>
        operator()(T const& value) const
        {
            return std::round(value);
        }
    };
    
}
