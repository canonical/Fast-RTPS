/*************************************************************************
 * Copyright (c) 2014 eProsima. All rights reserved.
 *
 * This copy of eProsima Fast RTPS is licensed to you under the terms described in the
 * FASTRTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/

/**
 * @file WriterProxy.h
 */

#ifndef WRITERPROXY_H_
#define WRITERPROXY_H_
#ifndef DOXYGEN_SHOULD_SKIP_THIS_PUBLIC




#include "../common/Types.h"
#include "../common/Locator.h"
#include "../common/CacheChange.h"
#include "../attributes/ReaderAttributes.h"

#include<set>

namespace boost
{
    class recursive_mutex;
}


namespace eprosima
{
    namespace fastrtps
    {
        namespace rtps
        {

            class StatefulReader;
            class HeartbeatResponseDelay;
            class WriterProxyLiveliness;

            /**
             * Class WriterProxy that contains the state of each matched writer for a specific reader.
             * @ingroup READER_MODULE
             */
            class WriterProxy
            {
                public:
                    ~WriterProxy();

                    /**
                     * Constructor.
                     * @param watt RemoteWriterAttributes.
                     * @param heartbeatResponse Time the Reader should wait to respond to a heartbeat.
                     * @param SR Pointer to the StatefulReader.
                     */
                    WriterProxy(RemoteWriterAttributes& watt,Duration_t heartbeatResponse,StatefulReader* SR);

                    /**
                     * Get the maximum sequenceNumber received from this Writer.
                     * @param[out] seqNum Pointer to the sequenceNumber
                     * @return True if correct.
                     */
                    const SequenceNumber_t available_changes_max() const;

                    /**
                     * Update the missing changes up to the provided sequenceNumber.
                     * All changes with status UNKNOWN with seqNum <= input seqNum are marked MISSING.
                     * @param[in] seqNum Pointer to the SequenceNumber.
                     */
                    void missing_changes_update(const SequenceNumber_t& seqNum);

                    /**
                     * Update the lost changes up to the provided sequenceNumber.
                     * All changes with status UNKNOWN or MISSING with seqNum < input seqNum are marked LOST.
                     * @param[in] seqNum Pointer to the SequenceNumber.
                     */
                    void lost_changes_update(const SequenceNumber_t& seqNum);

                    /**
                     * The provided change is marked as RECEIVED.
                     * @param[in] change Pointer to the change
                     * @return True if correct.
                     */
                    bool received_change_set(CacheChange_t* change);

                    /**
                     * Set a change as RECEIVED and NOT RELEVANT.
                     * @param seqNum Sequence number of the change
                     * @return true on success
                     */
                    bool irrelevant_change_set(const SequenceNumber_t& seqNum);

                    void setNotValid(const CacheChange_t *change);

                    bool areThereMissing();

                    /**
                     * The method returns a vector containing all missing changes.
                     * @param missing Pointer to vector of pointers to ChangeFromWriter_t structure.
                     * @return True if correct.
                     */
                    std::vector<const ChangeFromWriter_t*>  missing_changes();

                    //! Pointer to associated StatefulReader.
                    StatefulReader* mp_SFR;
                    //! Parameters of the WriterProxy
                    RemoteWriterAttributes m_att;
                    //! Acknack Count
                    uint32_t m_acknackCount;
                    //! LAst HEartbeatcount.
                    uint32_t m_lastHeartbeatCount;
                    //!Timed event to postpone the heartbeatResponse.
                    HeartbeatResponseDelay* mp_heartbeatResponse;
                    //!TO check the liveliness Status periodically.
                    WriterProxyLiveliness* mp_writerProxyLiveliness;
                    //!Indicates if the heartbeat has the final flag set.
                    bool m_heartbeatFinalFlag;

                    /**
                     * Get a specific change by its sequence number
                     *
                     * @param seq Sequence number of the change
                     * @param change Pointer to pointer to CacheChange_t, to store the change
                     * @return true on success
                     */
                    CacheChange_t* get_change(SequenceNumber_t& seq);

                    /**
                     * Check if the writer is alive
                     * @return true if the writer is alive
                     */
                    inline bool isAlive(){return m_isAlive;};

                    /**
                     * Set the writer as alive
                     */
                    void assertLiveliness();
                    /**
                     * Set the writer as not alive
                     * @return
                     */
                    inline void setNotAlive(){m_isAlive = false;};

                    /**
                     * Get the mutex
                     * @return Associated mutex
                     */
                    inline boost::recursive_mutex* getMutex(){return mp_mutex;};

                private:

                    /*!
                     * @brief Add ChangeFromWriter_t up to the sequenceNumber passed, but not including this.
                     * Ex: If you have seqNums 1,2,3 and you receive seqNum 6, you need to add 4 and 5.
                     * @param sequence_number
                     * @param default_status ChangeFromWriter_t added will be created with this ChangeFromWriterStatus_t.
                     * @return True if sequence_number will be the next after last element in the m_changesFromW container.
                     * @remarks No thread-safe.
                     */
                    bool maybe_add_changes_from_writer_up_to(const SequenceNumber_t& sequence_number, const ChangeFromWriterStatus_t default_status = ChangeFromWriterStatus_t::UNKNOWN);

                    void cleanup();

                    //!Is the writer alive
                    bool m_isAlive;
                    //Print Method for log purposes
                    void print_changes_fromWriter_test2();

                    //!Mutex Pointer
                    boost::recursive_mutex* mp_mutex;

                    //!Vector containing the ChangeFromWriter_t objects.
                    std::set<ChangeFromWriter_t, ChangeFromWriterCmp> m_changesFromW;
                    SequenceNumber_t changesFromWLowMark_;

                    void for_each_set_status_from(decltype(m_changesFromW)::iterator first,
                            decltype(m_changesFromW)::iterator last,
                            ChangeFromWriterStatus_t status,
                            ChangeFromWriterStatus_t new_status);

                    void for_each_set_status_from_and_maybe_remove(decltype(m_changesFromW)::iterator first,
                            decltype(m_changesFromW)::iterator last,
                            ChangeFromWriterStatus_t status,
                            ChangeFromWriterStatus_t orstatus,
                            ChangeFromWriterStatus_t new_status);
            };

        } /* namespace rtps */
    } /* namespace fastrtps */
} /* namespace eprosima */
#endif
#endif /* WRITERPROXY_H_ */
