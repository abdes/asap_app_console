//    Copyright The Blocxxi Project Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include <iostream>

#include <boost/program_options.hpp>

#include <common/logging.h>
#include <console_runner.h>
#include <detached_application.h>

namespace bpo = boost::program_options;


using asap::ConsoleRunner;
using asap::RunnerBase;


int main(int argc, char **argv) {
  auto &logger = asap::logging::Registry::GetLogger(asap::logging::Id::MAIN);

  //asap::fs::CreateDirectories();


  try {
    // Command line arguments
    bpo::options_description desc("Allowed options");
    // clang-format off
    desc.add_options()
        ("help", "show the help message");
    // clang-format on

    bpo::variables_map bpo_vm;
    bpo::store(bpo::parse_command_line(argc, argv, desc), bpo_vm);

    if (bpo_vm.count("help")) {
      std::cout << desc << std::endl;
      return 0;
    }

    bpo::notify(bpo_vm);

    ASLOG_TO_LOGGER(logger, info, "starting in console mode...");
    //
    // TODO: Change this with the chosen application implementation
    // There are two strategies for running application logic:
    //   1- Run the logic in the same thread than the runner utilising the
    //      io_context scheduling. Implement the AbstractApplication interface
    //      and create an instance of the concrete class here.
    //   2- Run the logic in separate thread(s) from the main/runner thread. In
    //      that case, use a DetachedApplication instance to pass to the runner
    //      and handle the application logic independently. Use the shutdown
    //      function to stop the application threads when the runner must stop.
    //
    asap::DetachedApplication app;

    //
    // Start the console runner
    //
    ConsoleRunner runner(app, [&]() {
      // Shutdown
      ASLOG_TO_LOGGER(logger, info, "shutdown complete");
    });
    runner.Run();
  } catch (std::exception &e) {
    ASLOG_TO_LOGGER(logger, error, "Error: {}", e.what());
    return -1;
  } catch (...) {
    ASLOG_TO_LOGGER(logger, error, "Unknown error!");
    return -1;
  }

  return 0;
}
