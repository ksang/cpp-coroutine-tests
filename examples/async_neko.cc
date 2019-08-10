#include <iostream>
#include <experimental/coroutine>

using namespace std::experimental;
using namespace std;

class Neko
{
public:
    struct promise_type;

private:
    coroutine_handle<promise_type> _coroutine;

public:
    bool resume();
    int return_val();

    explicit Neko(coroutine_handle<promise_type> coroutine)
        : _coroutine(coroutine)
    {
    }
    ~Neko()
    {
        if (_coroutine) {
            _coroutine.destroy();
        }
    }
    Neko() = default;
    Neko(Neko const&) = delete;
    Neko& operator=(Neko const&) = delete;

    Neko(Neko&& other)
        : _coroutine(other._coroutine) {
        other._coroutine = nullptr;
    }

    Neko& operator=(Neko&& other) {
        if (&other != this) {
            _coroutine = other._coroutine;
            other._coroutine = nullptr;
        }
        return *this;
    }
};

struct Neko::promise_type {
    int _value = 0;
    Neko get_return_object()
    {
        return Neko(coroutine_handle<promise_type>::from_promise(*this));
    }
    auto initial_suspend() { return suspend_never{}; }
    auto final_suspend() { return suspend_always{}; }

    void return_value(const int v)
    {
        _value = v;
    }

    void unhandled_exception()
    {
        std::terminate();
    }
};

int Neko::return_val()
{
    return _coroutine.promise()._value;
}

bool Neko::resume()
{
    if (not _coroutine.done())
        _coroutine.resume();
    return not _coroutine.done();
}

Neko set_value(std::string name, int value)
{
    cout << "Neko(" << name << ") was called\n";
    co_await suspend_always{};
    cout << "counter(" << name << ") resumed with value " << value << '\n';
    co_return value;
}

int main()
{
    Neko neko_a = set_value("a", 10);
    Neko neko_b = set_value("b", 20);
    while(neko_a.resume());
    while(neko_b.resume());
    cout << "neko_a value: " << neko_a.return_val() << '\n';
    cout << "neko_b value: " << neko_b.return_val() << '\n';
}
