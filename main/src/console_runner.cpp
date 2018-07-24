//    Copyright The asap Project Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include <csignal>  // for signal handling
#include <chrono>   // for sleep timeout
#include <thread>   // for access to this thread

#include <console_runner.h>


namespace {
volatile std::sig_atomic_t gSignalInterrupt_;
}  // unnamed namespace

void SignalHandler(int signal) {
  gSignalInterrupt_ = signal;
}


namespace asap {

ConsoleRunner::ConsoleRunner(AbstractApplication &app, RunnerBase::shutdown_function_type func)
    : RunnerBase(app, std::move(func)) {
}
ConsoleRunner::~ConsoleRunner() {
}
void ConsoleRunner::Run() {
  // Register to handle the signals that indicate when the server should exit.
  // It is safe to register for the same signal multiple times in a program,
  // provided all registration for the specified signal is made through Asio.
  std::signal(SIGINT, SignalHandler);
  std::signal(SIGTERM, SignalHandler);

  // Initialize the app
  app_.Init();

  // Main loop
  bool sleep_when_inactive = true;
  while (gSignalInterrupt_ == 0) {
    if (sleep_when_inactive) {
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    // Draw the Application
    sleep_when_inactive = app_.Run();
  }
  ASLOG(info, "signal ({}) caught", gSignalInterrupt_);
  // Invoke the app shutdown
  app_.ShutDown();
  // Call the main shutdown function
  shutdown_function_();
}

}  // namespace asap
