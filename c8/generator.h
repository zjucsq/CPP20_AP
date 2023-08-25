#include <coroutine>
#include <utility>

struct Generator {
  struct promise_type {
    Generator get_return_object() { return Generator{this}; }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_always final_suspend() { return {}; }
    void return_void() {}
    std::suspend_always yield_value(int value) {
      cur_value_ = value;
      return {};
    }
    int cur_value_;
  };
  using Handle = std::coroutine_handle<promise_type>;
  Handle mCoroHdl{};

  explicit Generator(promise_type *p) : mCoroHdl(Handle::from_promise(*p)) {}
  Generator(Generator &&rhs) : mCoroHdl(std::exchange(rhs.mCoroHdl, nullptr)) {}
  ~Generator() {
    if (mCoroHdl)
      mCoroHdl.destroy();
  }

  void next() { return mCoroHdl.resume(); }
  bool done() { return mCoroHdl.done(); }
  int cur_value() { return mCoroHdl.promise().cur_value_; }
};