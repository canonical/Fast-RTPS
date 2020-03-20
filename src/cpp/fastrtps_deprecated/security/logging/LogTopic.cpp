#include "fastrtps_deprecated/security/logging/LogTopic.h"

#include "fastrtps/publisher/Publisher.h"
#include "fastrtps/log/Log.h"

namespace eprosima {
namespace fastrtps {
namespace rtps {
namespace security {

LogTopic::LogTopic()
  : stop_(false)
  , thread_([this]() {
      for (;;)
      {
        // Put the thread asleep until there is
        // something to process
        auto p = queue_.wait_pop();

        if (!p)
        {
          if (stop_)
          {
            return;
          }
          continue;
        }

        publish(*p);
      }
    })
{
  //
}

LogTopic::~LogTopic()
{
  stop();
  queue_.push(std::move(BuiltinLoggingTypePtr(nullptr)));
  if (thread_.joinable())
  {
    thread_.join();
  }
}

void LogTopic::log_impl(const BuiltinLoggingType& message,
                        SecurityException& /*exception*/) const
{
  queue_.push(std::move(
    BuiltinLoggingTypePtr(new BuiltinLoggingType(message)
  )));
}

void LogTopic::publish(BuiltinLoggingType& msg)
{
  if (!get_publisher()->write((void*)&msg))
  {
    logError(BUILTINLOGGING, "Could not log BuiltinLoggingType message.");
  }
}

} //namespace security
} //namespace rtps
} //namespace fastrtps
} //namespace eprosima
