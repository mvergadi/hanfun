// =============================================================================
/*!
 * @file       inc/hanfun/core/event_scheduling.h
 *
 * This file contains the definitions for the %Event %Scheduling service.
 *
 * @version    1.5.3
 *
 * @copyright  Copyright &copy; &nbsp; 2017 ULE Alliance
 *
 * For licensing information, please see the file 'LICENSE' in the root folder.
 *
 * Initial development by Bithium S.A. [http://www.bithium.com]
 */
// =============================================================================
#ifndef HF_CORE_EVENT_SCHEDULING_H
#define HF_CORE_EVENT_SCHEDULING_H

#include "hanfun/protocol.h"
#include "hanfun/core.h"

#include "hanfun/core/scheduling.h"

namespace HF
{
   namespace Core
   {
      namespace Scheduling
      {
         // Forward declaration
         namespace Event
         {
            struct IServer;
         }
      }

      /*!
       * @ingroup event_scheduling
       *
       * Create an attribute object that can hold the attribute with the given @c uid.
       *
       * If @c server is not equal to @c nullptr then initialize it with the current
       * value.
       *
       * @param [in] server   pointer to the object to read the current value from.
       * @param [in] uid      attribute's %UID to create the attribute object for.
       *
       * @return  pointer to an attribute object or @c nullptr if the attribute UID does not
       *          exist.
       */
      HF::Attributes::IAttribute *create_attribute(Scheduling::Event::IServer *server, uint8_t uid);

      namespace Scheduling
      {
         /*!
          * This namespace contains the implementation of the %Event %Scheduling service.
          */
         namespace Event
         {
            /*!
             * @addtogroup event_scheduling  Event Scheduling
             * @ingroup core
             *
             * This module contains the classes that implement the %Event %Scheduling service.
             *
             * @{
             */

            //! %Event %Scheduler specific part for the @c HF::Scheduling::Entry.
            struct Interval
            {
               uint32_t start;          //!< Start Date.
               uint32_t end;            //!< End Date.
               uint32_t repeat;         //!< Repeat interval (in seconds).

               /*!
                * Constructor
                *
                * @param [in] _start   timestamp for the start of the event in seconds.
                * @param [in] _end     timestamp for the end of the event in seconds..
                * @param [in] _repeat  repetition interval in seconds.
                */
               Interval(uint32_t _start, uint32_t _end, uint32_t _repeat):
                  start(_start), end(_end), repeat(_repeat)
               {}

               Interval() = default;

               /*!
                * Get the initial value for the next_run entry attribute.
                *
                * @return  the timestamp for the first run.
                */
               uint32_t first() const
               {
                  return start;
               }

               /*!
                * Get the step between executions.
                *
                * @return  the step time.
                */
               uint32_t step() const
               {
                  return repeat;
               }

               /*!
                * Check if entry is active.
                *
                * @param [in] _time    current time.
                *
                * @retval true   if the entry is active.
                * @retval false  otherwise.
                */
               bool active(uint32_t _time) const
               {
                  return (start <= _time && _time <= end);
               }

               //! Minimum pack/unpack required data size.
               static constexpr uint16_t min_size = sizeof(uint32_t)    // Start Date.
                                                    + sizeof(uint32_t)  // End Date.
                                                    + sizeof(uint32_t); // Repeat interval.

               //! @copydoc HF::Common::Serializable::size
               uint16_t size() const
               {
                  return min_size;
               }

               //! @copydoc HF::Common::Serializable::pack
               uint16_t pack(Common::ByteArray &array, uint16_t offset = 0) const;

               //! @copydoc HF::Common::Serializable::unpack
               uint16_t unpack(const Common::ByteArray &array, uint16_t offset = 0);
            };

            //! Specific part for the %Event Scheduler of the @c HF::Scheduling::Entry.
            typedef Scheduling::Entry<Interval> Entry;

            //! Specific part for the %Event Scheduler of the @c HF::Scheduling::GetEntryResponse.
            typedef Scheduling::GetEntryResponse<Interval> GetEntryResponse;

            /*!
             * Helper class to handle the Maximum Number Of %Entries attribute for the
             * %Event %Scheduling service.
             */
            struct MaximumNumberOfEntries: public Scheduling::MaximumNumberOfEntries
            {
               MaximumNumberOfEntries(uint8_t value = 0, HF::Interface *owner = nullptr):
                  Scheduling::MaximumNumberOfEntries(HF::Interface::EVENT_SCHEDULING,
                                                     value, owner)
               {}
            };

            /*!
             * Helper class to handle the Number Of %Entries attribute for the
             * %Event %Scheduling service.
             */
            struct NumberOfEntries: public Scheduling::NumberOfEntries
            {
               NumberOfEntries(uint8_t value = 0, HF::Interface *owner = nullptr):
                  Scheduling::NumberOfEntries(HF::Interface::EVENT_SCHEDULING, value, owner)
               {}
            };

            /*!
             * Helper class to handle the %Status attribute for the %Event %Scheduling service.
             */
            struct Status: public Scheduling::Status
            {
               Status(uint8_t value = 0, HF::Interface *owner = nullptr):
                  Scheduling::Status(HF::Interface::EVENT_SCHEDULING, value, owner)
               {}
            };

            /*!
             * @copybrief HF::Core::create_attribute (HF::Core::Scheduling::Event::IServer *,uint8_t)
             *
             * @see HF::Core::create_attribute (HF::Core::Scheduling::Event::IServer *,uint8_t)
             *
             * @param [in] uid   attribute %UID to create the attribute object for.
             *
             * @retval  pointer to an attribute object
             * @retval  <tt>nullptr</tt> if the attribute UID does not exist.
             */
            HF::Attributes::IAttribute *create_attribute(uint8_t uid);

            typedef Scheduling::Base<HF::Interface::EVENT_SCHEDULING,
                                     Scheduling::IServer> IServerBase;

            typedef Interfaces::Interface<HF::Interface::EVENT_SCHEDULING,
                                          Scheduling::IClient> IClientBase;

            /*!
             * %Event %Scheduling %Service : %Client side implementation.
             *
             * This class provides the client side of the %Event %Scheduling interface.
             */
            struct IClient: public IClientBase
            {
               //! Constructor
               IClient(): IClientBase()
               {}

               //! Destructor
               virtual ~IClient() {}

               static constexpr HF::Interface::UID ITF = HF::Interface::EVENT_SCHEDULING;

               /*!
                * @copybrief HF::Core::Scheduling::IClient::activate_scheduler
                *
                * @param [in] addr     the network address to send the message to.
                * @param [in] enabled  enable/disable scheduler.
                */
               virtual void activate_scheduler(const Protocol::Address &addr, bool enabled)
               {
                  Scheduling::IClient::activate_scheduler(addr, ITF, enabled);
               }

               /*!
                * Send a HAN-FUN message containing a @c Scheduling::DEFINE_EVENT_CMD,
                * to the given network address.
                *
                * @param [in] addr     the network address to send the message to.
                * @param [in] id       the ID for the event entry.
                * @param [in] status   the event entry status.
                * @param [in] time     the interval to set the event entry to.
                * @param [in] pid      the batch program to call on event.
                */
               virtual void define_event(const Protocol::Address &addr, uint8_t id,
                                         uint8_t status, Interval &time, uint8_t pid);

#ifdef HF_CORE_EVENT_SCHEDULING_UPDATE_EVENT_STATUS_CMD

               /*!
                * @copybrief HF::Core::Scheduling::IClient::update_event_status
                *
                * @param [in] addr     the network address to send the message to.
                * @param [in] id       entry ID to delete.
                * @param [in] enabled  enable/disable entry.
                */
               virtual void update_event_status(const Protocol::Address &addr,
                                                uint8_t id, bool enabled)
               {
                  Scheduling::IClient::update_event_status(addr, ITF, id, enabled);
               }
#endif

#ifdef HF_CORE_EVENT_SCHEDULING_GET_EVENT_ENTRY_CMD
               /*!
                * @copybrief HF::Core::Scheduling::IClient::get_event_entry
                *
                * @param [in] addr  the network address to send the message to.
                * @param [in] id    entry ID to delete.
                */
               virtual void get_event_entry(const Protocol::Address &addr, uint8_t id)
               {
                  Scheduling::IClient::get_event_entry(addr, ITF, id);
               }
#endif

               /*!
                * @copybrief HF::Core::Scheduling::IClient::delete_event
                *
                * @param [in] addr  the network address to send the message to.
                * @param [in] id    entry ID to delete.
                */
               virtual void delete_event(const Protocol::Address &addr, uint8_t id)
               {
                  Scheduling::IClient::delete_event(addr, ITF, id);
               }

#ifdef HF_CORE_EVENT_SCHEDULING_DELETE_ALL_EVENTS_CMD
               /*!
                * @copybrief HF::Core::Scheduling::IClient::delete_all_events
                *
                * @param [in] addr  the network address to send the message to.
                */
               virtual void delete_all_events(const Protocol::Address &addr)
               {
                  Scheduling::IClient::delete_all_events(addr, ITF);
               }
#endif

               using IClientBase::send;
            };

            /*!
             * %Scheduling %Service : %Client side implementation.
             *
             * This class provides the client side of the %Scheduling interface.
             */
            struct Client: public IClient
            {
               /*!
                * Constructor.
                */
               Client(): IClient()
               {}

               virtual ~Client()
               {}
            };

            /*!
             * Scheduling %Service : %Server side implementation.
             *
             * This class provides the server side of the Scheduling interface.
             */
            struct IServer: public IServerBase
            {
               uint16_t uid() const
               {
                  return IServerBase::uid();
               }

               HF::Attributes::UIDS attributes(uint8_t uid = HF::Attributes::Pack::MANDATORY) const
               {
                  return Scheduling::IServer::attributes(uid);
               }

               uint8_t number_of_entries() const;

               // ======================================================================
               // Events
               // ======================================================================
               //! @name Events
               //! @{

               /*!
                * Callback that is called when a @c Scheduling::DEFINE_EVENT_CMD,
                * is received.
                *
                * @param [in] packet   the packet received.
                * @param [in] msg      the define event message received.
                *
                * @retval  Common::Result::OK               if the entry was created;
                * @retval  Common::Result::FAIL_ARG         if the entry ID already exists;
                * @retval  Common::Result::FAIL_RESOURCES   if the entry could not be created;
                * @retval  Common::Result::FAIL_UNKNOWN     otherwise.
                */
               virtual Common::Result define_event(const Protocol::Packet &packet,
                                                   Scheduling::Entry<Interval> &msg);

               /*!
                * Callback that is called when a @c Scheduling::UPDATE_STATUS_CMD,
                * is received.
                *
                * @param [in] packet   the packet received.
                * @param [in] msg      the update event status message received.
                *
                * @retval  Common::Result::OK         if the entry was updated;
                * @retval  Common::Result::FAIL_ARG   if the entry does not exists.
                */
               virtual Common::Result update_event_status(const Protocol::Packet &packet,
                                                          const UpdateStatus &msg);

               /*!
                * Callback that is called when a @c Scheduling::GET_ENTRY_CMD,
                * is received.
                *
                * @param [in] packet   the packet received.
                * @param [in] msg      the get event message received.
                *
                * @retval  Common::Result::OK         if the entry exists,
                * @retval  Common::Result::FAIL_ARG   otherwise.
                */
               virtual Common::Result get_event_entry(const Protocol::Packet &packet,
                                                      const GetEntry &msg);

               /*!
                * Callback that is called when a @c Scheduling::DELETE_CMD,
                * is received.
                *
                * @param [in] packet   the packet received.
                * @param [in] msg      the delete event message received.
                *
                * @retval  Common::Result::OK         if the entry was deleted,
                * @retval  Common::Result::FAIL_ARG   otherwise.
                */
               virtual Common::Result delete_event(const Protocol::Packet &packet,
                                                   const DeleteEvent &msg);

               /*!
                * Callback that is called when a @c Scheduling::DELETE_ALL_CMD,
                * is received.
                *
                * @param [in] packet   the packet received.
                *
                * @retval Common::Result::OK
                */
               virtual Common::Result delete_all_events(const Protocol::Packet &packet);

               //! @}
               // ======================================================================

               void periodic(uint32_t time);

               //! Constructor
               IServer(Unit0 &unit): IServerBase(unit)
               {}

               //! Destructor
               virtual ~IServer() {}

               // =============================================================================
               // Entries API
               // =============================================================================

               /*!
                * Get a reference to the current object implementing the persistence API,
                * for the device information.
                *
                * @return  reference to the current object for the persistence API.
                */
               virtual IEntries<Interval> &entries() const = 0;

               /*!
                * Get the %Event %Scheduling entry given by @c id.
                *
                * @param [in] id  event id of the event to retrieve.
                *
                * @return  a pointer to the event entry if it exists,
                *          @c nullptr otherwise.
                */
               Common::Pointer<Entry> entry(const uint8_t id) const
               {
                  return entries().find(id);
               }

               /*!
                * @copydoc IEntries::next_id
                */
               uint8_t next_id() const
               {
                  return entries().next_id();
               }

               using IServerBase::notify;

               // =============================================================================
               // Protected types and methods
               // =============================================================================

               protected:

               Common::Result handle_command(Protocol::Packet &packet, Common::ByteArray &payload,
                                             uint16_t offset);

            };

            /*!
             * %Event %Scheduling %Service : %Server side implementation.
             *
             * This class provides the server side of the %Scheduling interface.
             */
            template<typename _Entries>
            struct Server: public IServer
            {
               static_assert(std::is_base_of<HF::Core::Scheduling::IEntries<Interval>,
                                             _Entries>::value,
                             "_Entries must be of type HF::Core::Scheduling::IEntries<Interval>");

               protected:

               _Entries _entries;

               public:

               /*!
                * Constructor.
                *
                * @param [in] unit  reference to the unit containing this service.
                */
               Server(Unit0 &unit): IServer(unit)
               {}

               virtual ~Server()
               {}

               //! @copydoc IServer::entries
               _Entries &entries() const
               {
                  return const_cast<_Entries &>(_entries);
               }
            };

            /*!
             * %Event %Scheduling %Service : %Server side with
             * default persistence implementation.
             */
            typedef Server<Entries<Interval>> DefaultServer;

            /*! @} */

         } // namespace Event
      }    // namespace Scheduling
   }       // namespace Core
}          // namespace HF

#endif /* HF_CORE_EVENT_SCHEDULING_H */
