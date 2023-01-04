
/*
#include <iostream>
#include <functional>
#include <boost/context/all.hpp>


class Coroutine {
 public:
 
  template<typename F>
  Coroutine(F&& f) :
      f_(std::forward<F>(f)),
      ctx_(std::bind(&Coroutine::Run, this)) {}

 
  void Suspend() { ctx_ = ctx_.resume(); }

  
  void Resume() { ctx_ = ctx_.resume(); }

  bool Done() const { return done_; }

 private:
  std::function<void()> f_;
  boost::context::continuation ctx_;
  bool done_ = false;

  void Run() {
    f_();
    done_ = true;
  }
};

*/

/*
int main() {
  Coroutine co([&] {
    std::cout << "Hello ";
    co.Suspend();
    std::cout << "world!" << std::endl;
  });

  std::cout << "Started ";
  co.Resume();
  std::cout << "coroutine." << std::endl;
  co.Resume();

  return 0;
}*/
