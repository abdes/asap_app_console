//    Copyright The Blocxxi Project Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include <iostream>

#include <cxxopts.hpp>

#include <asap/asap-version.h>
#include <common/logging.h>
#include <console_runner.h>
#include <detached_application.h>


using asap::ConsoleRunner;
using asap::RunnerBase;


int main(int argc, char **argv) {
  auto &logger = asap::logging::Registry::GetLogger("main");

  //asap::fs::CreateDirectories();


  try {
    //
    // Handle program options
    //
    // TODO: UTF-8 to be tested on Windows
    cxxopts::Options options(ASAP_PROJECT_NAME, ASAP_PROJECT_DESCRIPTION);
    options.add_options()
      ("v,version", "Show version")
      ("h,help", "Show usage information")
      ("t,test", "Test option with value", cxxopts::value<std::string>())
      ("c,chinese", "中文帮助文本", cxxopts::value<std::string>())
      ;
    auto result = options.parse(argc, argv);

    if (result.count("help")) {
      std::cout << options.help({}) << std::endl;
      exit(0);
    }

    if (result.count("version")) {
      std::cout << ASAP_NAME_VERSION << std::endl;
      exit(0);
    }

    // Test code for the option with value
    if (result.count("test")) {
      std::cout << "test = " << result["test"].as<std::string>() << std::endl;
    }
    // End Test code


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
