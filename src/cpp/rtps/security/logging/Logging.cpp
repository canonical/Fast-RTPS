#include "fastdds/rtps/security/logging/Logging.h"

namespace eprosima {
namespace fastrtps {
namespace rtps {
namespace security {

Logging::Logging()
  : listener_(nullptr)
  , logging_enabled_(false)
  , options_set_(false)
{
  //
}

bool Logging::get_log_options(LogOptions& log_options, SecurityException& exception) const
{
  if (!options_set_)
  {
    exception = SecurityException("LogOptions not set!");
    return false;
  }

  log_options = log_options_;
  return true;
}

bool Logging::enable_logging(SecurityException& exception)
{
  if (options_set_)
  {
    logging_enabled_ = true;
  }
  else
  {
    exception = SecurityException("Cannot enable logging before logging options are set!");
  }

  return logging_enabled_;
}

bool Logging::set_listener(LoggerListener* /*listener*/, SecurityException& exception)
{
  exception = SecurityException("Not implemented yet.");

  return false;
}

void Logging::log(const EventLogLevel event_log_level,
                  const std::string& message,
                  const std::string& category,
                  SecurityException& exception)
{
  if (logging_enabled_)
  {
    if (event_log_level <= log_options_.event_log_level)
    {
      log_impl(message, category, exception);
    }
  }
  //TODO(artivis): throw if not enabled?
}

void Logging::log_impl(const std::string& /*message*/,
                       const std::string& /*category*/,
                       SecurityException& exception)
{
  exception = SecurityException("Not implemented yet.");
}

} //namespace security
} //namespace rtps
} //namespace fastrtps
} //namespace eprosima
