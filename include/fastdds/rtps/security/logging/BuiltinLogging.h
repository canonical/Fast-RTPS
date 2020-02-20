// Copyright 2020 Canonical ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * @file Logging.h
 */
#ifndef _FASTDDS_RTPS_SECURITY_LOGGING_BUILTINLOGGING_H_
#define _FASTDDS_RTPS_SECURITY_LOGGING_BUILTINLOGGING_H_

#include "fastdds/rtps/security/logging/Logging.h"
#include "fastdds/rtps/security/logging/BuiltinLoggingType.h"
#include "fastrtps/utils/concurrent_queue.h"

#include <thread>
#include <atomic>

namespace eprosima {
namespace fastrtps {
namespace rtps {
namespace security {

/**
 * @brief BuiltinLogging
 */
class BuiltinLogging final : public Logging
{
  using Message = std::pair<std::string,std::string>;
  using MessagePtr = std::unique_ptr<Message>;

public:

  BuiltinLogging();
  ~BuiltinLogging();

private:

  /**
   * @brief log_impl
   * @param message
   * @param category
   * @param exception
   */
  virtual void log_impl(const std::string& message,
                        const std::string& category,
                        SecurityException& exception) override;

  BuiltinLoggingType convert(Message& msg);
  void publish(BuiltinLoggingType& msg);

  inline void stop() { stop_ = true; }

  //TODO(artivis):figure out which to use
  // Publisher* publisher_; // publisher_ = DomainParticipant::create_publisher(...)
  // DataWriter* data_writer_;

  std::atomic_bool stop_;

  std::thread thread_;

  ConcurrentQueue<MessagePtr> queue_;
};

} //namespace security
} //namespace rtps
} //namespace fastrtps
} //namespace eprosima

#endif // _FASTDDS_RTPS_SECURITY_LOGGING_BUILTINLOGGING_H_
