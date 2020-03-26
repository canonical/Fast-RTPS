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
#ifndef _FASTDDS_RTPS_SECURITY_LOGGING_LOGGING_H_
#define _FASTDDS_RTPS_SECURITY_LOGGING_LOGGING_H_

#include <limits>

#include "fastdds/rtps/security/logging/LogOptions.h"
#include "fastdds/rtps/security/logging/BuiltinLoggingType.h"
#include "fastdds/rtps/security/exceptions/SecurityException.h"
#include "fastdds/rtps/common/Guid.h"

namespace eprosima {
namespace fastrtps {

class Publisher;

namespace rtps {
namespace security {

/**
 * @brief The LoggerListener class
 */
class LoggerListener
{
  LoggerListener() = default;
  ~LoggerListener() = default;
};

/**
 * @brief Base class for all security logging plugins.
 */
class Logging
{
public:

  Logging();
  virtual ~Logging() = default;

  /**
   * @brief set_log_options
   * @param log_options
   * @return TRUE if successful
   */
  bool set_log_options(const LogOptions& log_options, SecurityException& exception);

  /**
   * @brief get_log_options
   * @param log_options
   * @param exception
   * @return
   */
  bool get_log_options(LogOptions& log_options, SecurityException& exception) const;

  /**
   * @brief enable_logging
   */
  bool enable_logging(SecurityException& exception);

  /**
   * @brief set_listener
   * @param listener
   * @return
   */
  bool set_listener(LoggerListener* listener, SecurityException& exception);

  /**
   * @brief log
   * @param log_level
   * @param message
   * @param category
   */
  void log(const EventLogLevel event_log_level,
           const std::string& message,
           const std::string& category,
           SecurityException& exception) const;

  /**
   * @brief Whether the options are set or not.
   * @return True if the options are set.
   */
  bool options_set() const { return options_set_; }

  /**
   * @brief Whether the logging is enabled or not.
   * @return True if the logging is enabled.
   */
  bool enabled() const { return logging_enabled_; }

  /**
   * @brief Return the LoggerListener.
   * @return A pointer to the (const) LoggerListener.
   */
  LoggerListener const* get_listener() const { return listener_; }

  bool set_guid(const GUID_t& guid, SecurityException& exception);

  bool set_domain_id(const uint32_t id, SecurityException& exception);

protected:

  /**
   * @brief enable_logging_impl
   * @return
   */
  virtual bool enable_logging_impl(SecurityException& /*exception*/) { return true; }

  /**
   * @brief convert
   * @param msg
   * @return
   */
  virtual bool convert(const EventLogLevel event_log_level,
                       const std::string& message,
                       const std::string& category,
                       BuiltinLoggingType& builtin_msg,
                       SecurityException& exception) const;

  /**
   * @brief log_impl
   * @param message
   * @param category
   * @param exception
   */
  virtual void log_impl(const BuiltinLoggingType& message,
                        SecurityException& exception) const = 0;

  Publisher* get_publisher() { return publisher_; }

private:

  LoggerListener* listener_;

  bool logging_enabled_ = false;
  bool options_set_ = false;

  LogOptions log_options_;
  GUID_t guid_;
  std::string guid_str_;
  uint32_t domain_id_ = std::numeric_limits<uint32_t>::max();
  std::string domain_id_str_;

  // DomainParticipant::create_publisher(...)
  Publisher* publisher_;
};

} //namespace security
} //namespace rtps
} //namespace fastrtps
} //namespace eprosima

#endif // _FASTDDS_RTPS_SECURITY_LOGGING_LOGGING_H_
