#include <iostream>
#include <functional>
#include <future>
#include <chrono>
#include <thread>

#include <experimental/coroutine>

template <typename T>
class Neko : public std::future<T>
{
    using std::future<T>::future;
    using std::future<T>::get;
    using std::future<T>::valid;
    using std::future<T>::wait;
    using std::future<T>::wait_for;

public:
	Neko(std::future<T> && f) noexcept : std::future<T>{std::move(f)} {}

    // Implement future.is_ready()
    bool is_ready() const
    {
		return wait_for(std::chrono::seconds(0)) == std::future_status::ready;
	}
    // Implement future.then()
    template <typename Work>
    auto then(Work && w) -> Neko<decltype(w())> {
        return {std::async([](Neko && fut, Work && w) {
            fut.wait();
            return w();
        }, std::move(*this), std::forward<Work>(w))};
    }

    template <typename Work>
    auto then(Work && w) -> Neko<decltype(w(get()))> {
        return {std::async([](Neko && fut, Work && w) {
            return w(fut.get());
        }, std::move(*this), std::forward<Work>(w))};
    }

    bool await_ready() const
    {
		return is_ready();
	}

	void await_suspend(std::experimental::coroutine_handle<> ch)
    {
        then([ch]() mutable
        {
            ch.resume();
        });
	}

	auto await_resume()
    {
		return get();
	}
};

template <typename T, typename... Arguments>
struct std::experimental::coroutine_traits<Neko<T>, Arguments...> {
	struct promise_type {
	private:
		std::promise<T> _promise;
	public:
		Neko<T> get_return_object() {
			return _promise.get_future();
		}

		auto initial_suspend() {
			return suspend_never{};
		}

		auto final_suspend() {
			return suspend_always{};
		}

		template <typename U>
		void return_value(U&& value)
        {
			_promise.set_value(std::forward<U>(value));
		}

		void set_exception(std::exception_ptr ex)
        {
			_promise.set_exception(std::move(ex));
		}

		void unhandled_exception()
        {
            std::terminate();
        }
	};
};

Neko<int> do_something(std::string name, int value)
{
    std::cout << "Neko(" << name << ") was called\n";
    auto result = co_await Neko(std::async([] (std::string name, int value)
    {
        std::cout << "Neko(" << name << ") starting to sleep " << value << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(value));
        std::cout << "Neko(" << name << ") task finished\n";
        return value;
    }, name, value
    ));
    co_return result;
}

int main(int, char *argv[]) {
    Neko neko_a = do_something("a", 5);
    Neko neko_b = do_something("b", 10);
    std::cout << "Main thread waiting...\n";
    std::this_thread::sleep_for(std::chrono::seconds(11));
    return 0;
}
