//    Copyright The asap Project Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include <console_runner.h>

#include <boost/asio.hpp>

namespace asap {

ConsoleRunner::ConsoleRunner(AbstractApplication &app, RunnerBase::shutdown_function_type func)
    : RunnerBase(app, std::move(func)) {
  io_context_ = new boost::asio::io_context(1);
  signals_ = new boost::asio::signal_set(*io_context_);
  // Register to handle the signals that indicate when the server should exit.
  // It is safe to register for the same signal multiple times in a program,
  // provided all registration for the specified signal is made through Asio.
  signals_->add(SIGINT);
  signals_->add(SIGTERM);
}
ConsoleRunner::~ConsoleRunner() {
  if (!io_context_->stopped()) io_context_->stop();
  delete signals_;
  delete io_context_;
}
void ConsoleRunner::Run() {
  // Initialize the app
  app_.Init();

  // Main loop
  bool interrupted = false;
  bool sleep_when_inactive = true;
  while (!interrupted) {
    signals_->async_wait(
        [this, &interrupted](boost::system::error_code /*ec*/, int signo) {
          ASLOG(info, "signal ({}) caught", signo);
          // Invoke the shutdown function
          shutdown_function_();
          // Once all operations have finished the io_context::run() call will
          // exit.
          interrupted = true;
          io_context_->stop();
        });

    if (sleep_when_inactive) {
      io_context_->run_for(std::chrono::milliseconds(200));
    } else {
      io_context_->poll_one();
    }

    // Draw the Application
    sleep_when_inactive = app_.Run();

  }
  // Invoke the app shutdown
  app_.ShutDown();
}

}  // namespace asap