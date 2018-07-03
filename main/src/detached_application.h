//    Copyright The Blocxxi Project Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#pragma once

namespace asap {

class DetachedApplication final : public AbstractApplication {
 public:
  virtual ~DetachedApplication() = default;

  virtual void Init() {};
  virtual bool Run() { return true; };
  virtual void ShutDown() {};
};

}  // namespace asap
