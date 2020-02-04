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
 * @file LogOptions.h
 */
#ifndef _FASTDDS_RTPS_SECURITY_LOGGING_LOGOPTIONS_H_
#define _FASTDDS_RTPS_SECURITY_LOGGING_LOGOPTIONS_H_

#include <string>

#include "fastdds/rtps/security/logging/EventLogLevel.h"

namespace eprosima {
namespace fastrtps {
namespace rtps {
namespace security {

/**
 * @brief The LogOptions struct
 */
struct LogOptions
{
  //! Whether the log events should be distributed over DDS
  bool distribute;

  //! Level at which log messages will be logged.
  //! Messages at or below the log_level are logged.
  EventLogLevel event_log_level;

  //! Full path to a local file
  std::string log_file;
};

} //namespace security
} //namespace rtps
} //namespace fastrtps
} //namespace eprosima

#endif // _FASTDDS_RTPS_SECURITY_LOGGING_LOGOPTIONS_H_
