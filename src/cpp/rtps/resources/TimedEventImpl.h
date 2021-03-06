// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

/**
 * @file TimedEventImpl.h
 *
 */



#ifndef _RTPS_RESOURCES_TIMEDEVENTIMPL_H_
#define _RTPS_RESOURCES_TIMEDEVENTIMPL_H_
#ifndef DOXYGEN_SHOULD_SKIP_THIS_PUBLIC 
#include <fastdds/rtps/common/Time_t.h>
#include <fastdds/rtps/resources/TimedEvent.h>

#include <asio.hpp>

#include <thread>
#include <memory>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <system_error>



namespace eprosima {
namespace fastrtps {
namespace rtps {

/*!
 * This class encapsulates an asio::steady_timer.
 * Also it manages the state of the event (INACTIVE, READY, WAITING..).
 * TimedEventImpl objects can be linked between them.
 * @ingroup MANAGEMENT_MODULE
 */
class TimedEventImpl
{
    using Callback = std::function<bool(TimedEvent::EventCode)>;

    public:

    typedef enum
    {
        INACTIVE = 0, //! The event is inactive. asio::io_service is not waiting for it.
        READY, //! The event is ready for being processed by ResourceEvent and added to asio::io_service.
        WAITING, //! The event is being waiting by asio::io_service to being triggered.
    } StateCode;

    ~TimedEventImpl();

    /*!
     * @brief Default constructor.
     * @param service asio::io_service managed by ResourceEvent.
     * @param callback Callback called when asio::steady_timer is triggered.
     * @param interval Expiration time in milliseconds of the event.
     */
    TimedEventImpl(
            asio::io_service& service,
            Callback callback,
            std::chrono::microseconds interval);

    protected:

    //! Expiration time in milliseconds of the event.
    std::chrono::microseconds m_interval_microsec;

    public:

    /*!
     * @brief Return next linked TimedEventImpl object.
     * @return Next linked TimedEventImpl object. Can be nullptr.
     */
    TimedEventImpl* next() const
    {
        return next_;
    }

    /*!
     * @brief Links the passed TimedEventImpl object.
     * @param next TimedEventImpl object to be linked.
     * @return Previous linked TimedEventImpl object. Can be nullptr.
     */
    TimedEventImpl* next(TimedEventImpl* next)
    {
        TimedEventImpl* old = next_;
        next_ = next;
        return old;
    }

    /*!
     * @brief Updates the expiration time of the event.
     *
     * When updating the interval, the timer is not restarted and the new interval will only be used the next time you
     * call restart_timer().
     * @param time New expiration time.
     * @return true on success
     */
    bool update_interval(const Duration_t& time);

    /*!
     * @brief Updates the expiration time of the event.
     *
     * When updating the interval, the timer is not restarted and the new interval will only be used the next time you
     * call restart_timer().
     * @param time_millisec New expiration time in milliseconds.
     * @return true on success
     */
    bool update_interval_millisec(double time_millisec);

    /*!
     * @brief Returns current expiration time in milliseconds
     * @return Event expiration time in milliseconds
     */
    double getIntervalMsec()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        auto total_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(m_interval_microsec);
        return static_cast<double>(total_milliseconds.count());
    }

    /*!
     * @brief Returns the remaining milliseconds for the timer to expire
     * @return Remaining milliseconds for the timer to expire
     */
    double getRemainingTimeMilliSec()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(timer_.expires_from_now()).count());
    }

    /*!
     * @brief Tries to set the event as READY.
     * To achieve it, the event has to be INACTIVE.
     * @return true on success.
     */
    bool go_ready();

    /*!
     * @brief Tries to cancel the event and set it as INACTIVE.
     * To achieve it, the event has to be WAITING.
     * @return true on success.
     */
    bool go_cancel();

    /*!
     * @brief It updates the asio::steady_timer depending of the state of TimedEventImpl object.
     * @warning This method has to be called from ResourceEvent's internal thread.
     */
    void update();

    private:

    /*!
     * @brief Callback called by asio::steady_timer when it expires.
     * @param callback_weak_ptr Weak shared pointer to the callback.
     * Used to verify the TimedEventImpl object is still alive.
     * @param ec Error code provided by asio::steady_timer.
     */
    void event(
            std::weak_ptr<Callback> callback_weak_ptr,
            const std::error_code& ec);

    asio::steady_timer timer_;

    Callback callback_;

    std::shared_ptr<Callback> callback_ptr_;

    std::atomic<StateCode> state_;

    std::atomic<bool> cancel_;

    TimedEventImpl* next_;

    std::mutex mutex_;
};

}
}
} // namespace eprosima

#endif
#endif //_RTPS_RESOURCES_TIMEDEVENTIMPL_H_
