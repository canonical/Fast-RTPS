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
      BuiltinLoggingType msg;
      for (;;)
      {
        // Put the thread asleep until there is
        // something to process
        MessagePtr p = queue_.wait_pop();

        if (!p)
        {
          if (stop_)
          {
            return;
          }
          continue;
        }

        msg = convert(*p);

        publish(msg);
      }
    })
{
  //
}

LogTopic::~LogTopic()
{
  stop();
  queue_.push(std::move(MessagePtr(nullptr)));
  if (thread_.joinable())
  {
    thread_.join();
  }
}

void LogTopic::log_impl(const std::string& message,
                              const std::string& category,
                              SecurityException& /*exception*/) const
{
  queue_.push(std::move(
    MessagePtr(new Message(message, category)
  )));
}

BuiltinLoggingType LogTopic::convert(Message& /*msg*/)
{
  return BuiltinLoggingType{};
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
