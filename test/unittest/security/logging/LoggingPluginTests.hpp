// Copyright 2020 Canonical, Ltd.
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

#ifndef _UNITTEST_SECURITY_LOGGING_LOGGINGPLUGINTESTS_HPP_
#define _UNITTEST_SECURITY_LOGGING_LOGGINGPLUGINTESTS_HPP_

#include "../../../../include/fastdds/rtps/security/logging/Logging.h"
#include "../../../../include/fastdds/rtps/attributes/PropertyPolicy.h"

#include <gtest/gtest.h>

class LoggingPluginTest : public ::testing::Test
{
protected:

  virtual void SetUp()
  {
    plugin = new eprosima::fastrtps::rtps::security::Logging();

    ASSERT_NE(nullptr, plugin);
  }

  virtual void TearDown()
  {
    delete plugin;
  }

public:
  LoggingPluginTest() = default;
  ~LoggingPluginTest() = default;

  eprosima::fastrtps::rtps::security::Logging* plugin = nullptr;
};

TEST_F(LoggingPluginTest, DefaultBehavior)
{
  eprosima::fastrtps::rtps::security::SecurityException exception;

  // Options not set

  eprosima::fastrtps::rtps::security::LogOptions log_options;
  EXPECT_FALSE(plugin->get_log_options(log_options, exception));

  EXPECT_FALSE(plugin->enable_logging(exception));

  // Options set

  EXPECT_TRUE(plugin->set_log_options(log_options, exception));

  EXPECT_TRUE(plugin->get_log_options(log_options, exception));

  // Logging enabled

  EXPECT_TRUE(plugin->enable_logging(exception));

  EXPECT_FALSE(plugin->set_log_options(log_options, exception));
}

#endif // _UNITTEST_SECURITY_LOGGING_LOGGINGPLUGINTESTS_HPP_
