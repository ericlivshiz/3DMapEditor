#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread> // Include this header for std::thread
#include <mutex>  // Include this header for std::mutex
#include <queue>  // Include this header for std::queue
#include <functional> // Include this header for std::function

class ThreadPool {
public:
    ThreadPool(size_t numThreads) : stop(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            threads.emplace_back([this] {
                while (true) {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        condition.wait(lock, [this] { return stop || !tasks.empty(); });

                        if (stop && tasks.empty())
                            return;

                        task = std::move(tasks.front());
                        tasks.pop();
                    }

                    task();
                }
                });
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();

        for (std::thread& thread : threads) {
            thread.join();
        }
    }

    template <class Func>
    void enqueue(Func&& func) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.emplace([func]() { func; });
        }
        condition.notify_one();
    }
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;
};