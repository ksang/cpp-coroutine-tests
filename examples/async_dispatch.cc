#include <iostream>
#include <future>
#include <chrono>
#include <thread>

std::future<int> async_dispatch(std::string name, int value)
{
    std::cout << "Future(" << name << ") was called\n";
    auto fut = std::async([] (std::string name, int value)
    {
        std::cout << "Future(" << name << ") starting to sleep " << value << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(value));
        std::cout << "Future(" << name << ") task finished\n";
        return value;
    }, name, value
    );
    return fut;
}

int main(int, char *argv[]) {
    auto fut_a = async_dispatch("a", 5);
    auto fut_b = async_dispatch("b", 10);
    std::cout << "Main thread waiting...\n";
    std::cout << "Future b result: " << fut_b.get() << '\n';
    std::cout << "Future a result: " << fut_a.get() << '\n';
    return 0;
}
