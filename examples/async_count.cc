#include <iostream>
#include <experimental/coroutine>

using namespace std::experimental;
using namespace std;

struct neko
{
    struct promise_type;

    coroutine_handle<promise_type> _coroutine = nullptr;

    explicit neko(coroutine_handle<promise_type> coroutine)
        : _coroutine(coroutine)
    {
    }

    bool resume()
    {
        if (not _coroutine.done())
            _coroutine.resume();
        return not _coroutine.done();
    }

    int get() { return _coroutine.promise()._value; }

    ~neko()
    {
        if (_coroutine) {
            _coroutine.destroy();
        }
    }

    struct promise_type {
        int _value = 0;
        neko get_return_object()
        {
            return neko(coroutine_handle<promise_type>::from_promise(*this));
        }
        auto initial_suspend() { return suspend_never{}; }
        auto final_suspend() { return suspend_never{}; }

        void return_void()
        {
        }

        void unhandled_exception()
        {
            std::terminate();
        }

    };

    neko() = default;
    neko(neko const&) = delete;
    neko& operator=(neko const&) = delete;

    neko(neko&& other)
        : _coroutine(other._coroutine) {
        other._coroutine = nullptr;
    }

    neko& operator=(neko&& other) {
        if (&other != this) {
            _coroutine = other._coroutine;
            other._coroutine = nullptr;
        }
        return *this;
    }
};

neko named_counter(std::string name)
{
    cout << "counter(" << name << ") was called\n";
    for (unsigned i = 1; ; ++i) {
        co_await suspend_always{};
        cout << "counter(" << name << ") resumed #" << i << '\n';
    }
}

int main()
{
    neko neko_a = named_counter("a");
    neko neko_b = named_counter("b");
    neko_a.resume();
    neko_b.resume();
    neko_b.resume();
    neko_a.resume();
}
