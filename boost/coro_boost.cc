#include <boost/coroutine/all.hpp>
#include <iostream>
#include <chrono>
#include <thread>


using namespace boost::coroutines;

void cooperative(coroutine<void>::push_type &sink)
{
  std::cout << "Hello\n";
  sink();
  std::cout << "world";
  std::this_thread::sleep_for(std::chrono::seconds(3));
}

int main()
{
  coroutine<void>::pull_type source{cooperative};
  std::cout << ", ";
  source();
  std::cout << "!\n";
}
