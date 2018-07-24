//    Copyright The asap Project Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#pragma once

#include <runner_base.h>

namespace asap {

class ConsoleRunner : public RunnerBase {
 public:
  ConsoleRunner(AbstractApplication &app, shutdown_function_type func);

  ~ConsoleRunner() override;
  ConsoleRunner(const ConsoleRunner &) = delete;
  ConsoleRunner &operator=(const ConsoleRunner &) = delete;

  void Run() override;

 private:
};

}  // namespace asap
