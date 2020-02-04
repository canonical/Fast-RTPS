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
 * @file EventLogLevel.h
 */
#ifndef _FASTDDS_RTPS_SECURITY_LOGGING_EVENTLOGLEVEL_H_
#define _FASTDDS_RTPS_SECURITY_LOGGING_EVENTLOGLEVEL_H_

namespace eprosima {
namespace fastrtps {
namespace rtps {
namespace security {

/**
 * @brief The EventLogLevel enum
 *
 * @note Definition in DDS-Sec v1.1 8.6.2.1.1
 */
enum struct EventLogLevel : long
{
  FATAL_LEVEL = 0,  // security error causing a shutdown or failure of the Domain Participant
  SEVERE_LEVEL,     // major security error or fault
  ERROR_LEVEL,      // minor security error or fault
  WARNING_LEVEL,    // undesirable or unexpected behavior
  NOTICE_LEVEL,     // important security event
  INFO_LEVEL,       // interesting security event
  DEBUG_LEVEL,      // detailed information on the flow of the security events
  TRACE_LEVEL       // even more detailed information
};

} //namespace security
} //namespace rtps
} //namespace fastrtps
} //namespace eprosima

#endif // _FASTDDS_RTPS_SECURITY_LOGGING_EVENTLOGLEVEL_H_
