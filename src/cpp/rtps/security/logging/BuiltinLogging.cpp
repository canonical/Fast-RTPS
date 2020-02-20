#include "fastdds/rtps/security/logging/BuiltinLogging.h"

namespace eprosima {
namespace fastrtps {
namespace rtps {
namespace security {

BuiltinLogging::BuiltinLogging()
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

BuiltinLogging::~BuiltinLogging()
{
  stop();
  queue_.push(std::move(MessagePtr(nullptr)));
  if (thread_.joinable())
    thread_.join();
}

void BuiltinLogging::log_impl(const std::string& message,
                              const std::string& category,
                              SecurityException& /*exception*/)
{
  queue_.push(std::move(
    MessagePtr(new Message(message, category)
  )));
}

BuiltinLoggingType BuiltinLogging::convert(Message& /*msg*/)
{
  return BuiltinLoggingType{};
}

void BuiltinLogging::publish(BuiltinLoggingType& /*msg*/)
{
  //
}

} //namespace security
} //namespace rtps
} //namespace fastrtps
} //namespace eprosima
