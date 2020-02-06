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

bool Logging::set_log_options(const LogOptions& log_options, SecurityException& exception)
{
  if (logging_enabled_)
  {
    exception = SecurityException("Cannot set option after enable_logging called.");
    return false;
  }

  //TODO(artivis): assert valid path
  if (!log_options.log_file.empty() /*&& !is_valid_path(log_options.log_file)*/)
  {
    exception = SecurityException("Invalid file path for logging.");
  }

  log_options_ = log_options;
  options_set_ = true;

  if(log_options_.distribute)
  {
    //TODO(artivis): set up DataWriter/Publisher
  }

  return true;
}

bool Logging::get_log_options(LogOptions& log_options, SecurityException& exception) const
{
  if (!options_set_)
  {
    exception = SecurityException("Logging options not set!");
    return false;
  }

  log_options = log_options_;
  return true;
}

bool Logging::enable_logging(SecurityException& exception)
{
  if (!options_set_)
  {
    exception = SecurityException("Cannot enable logging before logging options are set!");
    return false;
  }

  logging_enabled_ = true;

  return true;
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
