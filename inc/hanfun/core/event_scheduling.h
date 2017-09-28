// =============================================================================
/*!
 * @file       inc/hanfun/core/event_scheduling.h
 *
 * This file contains the definitions for the Event Scheduling service.
 *
 * @version    x.x.x
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
            struct Server;
         }
      }


      /*!
       * @ingroup event_scheduling_itf
       *
       * Create an attribute object that can hold the attribute with the given @c uid.
       *
       * If @c server is not equal to @c nullptr then initialize it with the current
       * value.
       *
       * @param [in] server   pointer to the object to read the current value from.
       * @param [in] uid      attribute's UID to create the attribute object for.
       *
       * @return  pointer to an attribute object or @c nullptr if the attribute UID does not
       *          exist.
       */
      HF::Attributes::IAttribute *create_attribute(Scheduling::Event::Server *server, uint8_t uid);

      namespace Scheduling
      {
         /*!
          * This namespace contains the implementation of the Event Scheduling service.
          */
         namespace Event
         {
            //! Specific part for the Event Scheduler of the @c HF::Scheduling::Entry.
            struct Interval
            {

               uint32_t start;          //!< Start Date.
               uint32_t end;            //!< End Date.
               uint32_t repeat;         //!< Repeat interval. (in seconds)

               Interval(uint32_t _start, uint32_t _end, uint32_t _repeat):
                  start(_start), end(_end), repeat(_repeat)
               {}

               Interval() = default;

               //! Minimum pack/unpack required data size.
               static constexpr uint16_t min_size = sizeof(uint32_t)     // Start Date.
                                                    + sizeof(uint32_t)   // End Date.
                                                    + sizeof(uint32_t);  // Repeat interval.

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

            typedef Scheduling::Entry<Interval> Entry;

            //! Specific part for the Event Scheduler of the @c HF::Scheduling::GetEntryResonse.
            typedef Scheduling::GetEntryResponse<Interval> GetEntryResponse;

            /*!
             * Helper class to handle the Maximum Number Of Entries attribute for the Event Scheduling service.
             */
            struct MaximumNumberOfEntries: public Scheduling::MaximumNumberOfEntries
            {
               MaximumNumberOfEntries(uint8_t value = 0, HF::Interface *owner = nullptr):
                  Scheduling::MaximumNumberOfEntries(HF::Interface::EVENT_SCHEDULING,
                                                     value, owner)
               {}
            };

            /*!
             * Helper class to handle the Number Of Entries attribute for the Event Scheduling service.
             */
            struct NumberOfEntries: public Scheduling::NumberOfEntries
            {
               NumberOfEntries(uint8_t value = 0, HF::Interface *owner = nullptr):
                  Scheduling::NumberOfEntries(HF::Interface::EVENT_SCHEDULING,
                                              value, owner)
               {}
            };

            /*!
             * Helper class to handle the Status attribute for the Event Scheduling service.
             */
            struct Status: public Scheduling::Status
            {
               Status(uint8_t value = 0, HF::Interface *owner = nullptr):
                  Scheduling::Status(HF::Interface::EVENT_SCHEDULING,
                                     value, owner)
               {}
            };

            /*!
             * @copybrief HF::Core::create_attribute (HF::Interfaces::Scheduling::Event::Server *,uint8_t)
             *
             * @see HF::Core::create_attribute (HF::Core::Scheduling::Event::Server *,uint8_t)
             *
             * @param [in] uid   attribute %UID to create the attribute object for.
             *
             * @retval  pointer to an attribute object
             * @retval  <tt>nullptr</tt> if the attribute UID does not exist.
             */
            HF::Attributes::IAttribute *create_attribute(uint8_t uid);


            typedef Scheduling::Base<HF::Interface::EVENT_SCHEDULING> Base;

            /*!
             * Event Scheduling %Service : %Client side implementation.
             *
             * This class provides the client side of the Event Scheduling interface.
             */
            struct IClient: public Scheduling::IClient, ServiceRole<Base,
                                                                    HF::Interface::CLIENT_ROLE>
            {
               using Client = ServiceRole<Base, HF::Interface::CLIENT_ROLE>;

               //! Constructor
               IClient(Unit0 &unit): Scheduling::IClient(), Client(unit)
               {}

               //! Destructor
               virtual ~IClient() = default;

               static constexpr HF::Interface::UID ITF = HF::Interface::EVENT_SCHEDULING;

               //! @copydoc HF::Core::Scheduling::activate_scheduler(Protocol::Address).
               virtual void activate_scheduler(const Protocol::Address &addr, uint8_t _status)
               {
                  Scheduling::IClient::activate_scheduler(ITF, addr, _status);
               }

               //! @copydoc HF::Core::Scheduling::activate_scheduler().
               void activate_scheduler(uint8_t _status)
               {
                  Scheduling::IClient::activate_scheduler(ITF, _status);
               }

               //! @copydoc HF::Core::Scheduling::define_event(Protocol::Address).
               virtual void define_event(const Protocol::Address &addr,
                                         uint8_t id,
                                         uint8_t status,
                                         Interval &time,
                                         uint8_t pid);


               //! @copydoc HF::Core::Scheduling::define_event().
               void define_event(uint8_t id,
                                 uint8_t status,
                                 Interval &time,
                                 uint8_t pid)
               {
                  Protocol::Address addr(0, 0);
                  define_event(addr, id, status, time, pid);
               }

#ifdef HF_CORE_EVENT_SCHEDULING_UPDATE_EVENT_STATUS_CMD

               //! @copydoc HF::Core::Scheduling::update_event_status(Protocol::Address).
               virtual void update_event_status(const Protocol::Address &addr)
               {
                  Scheduling::IClient::update_event_status(ITF, addr);
               }

               //! @copydoc HF::Core::Scheduling::update_event_status().
               void update_event_status()
               {
                  Scheduling::IClient::update_event_status(ITF);
               }
#endif

#ifdef HF_CORE_EVENT_SCHEDULING_GET_EVENT_ENTRY_CMD
               //! @copydoc HF::Core::Scheduling::get_event_entry(Protocol::Address).
               virtual void get_event_entry(const Protocol::Address &addr)
               {
                  Scheduling::IClient::get_event_entry(ITF, addr);
               }

               //! @copydoc HF::Core::Scheduling::get_event_entry().
               void get_event_entry()
               {
                  Scheduling::IClient::get_event_entry(ITF);
               }
#endif

               //! @copydoc HF::Core::Scheduling::delete_event(Protocol::Address).
               virtual void delete_event(const Protocol::Address &addr)
               {
                  Scheduling::IClient::delete_event(ITF, addr);
               }

               //! @copydoc HF::Core::Scheduling::delete_event().
               void delete_event()
               {
                  Scheduling::IClient::delete_event(ITF);
               }

#ifdef HF_CORE_EVENT_SCHEDULING_DELETE_ALL_EVENTS_CMD
               //! @copydoc HF::Core::Scheduling::delete_all_events(Protocol::Address).
               virtual void delete_all_events(const Protocol::Address &addr)
               {
                  Scheduling::IClient::delete_all_events(ITF, addr);
               }

               //! @copydoc HF::Core::Scheduling::delete_all_events().
               void delete_all_events()
               {
                  Scheduling::IClient::delete_all_events(ITF);
               }
#endif

               using Client::send;
            };

            /*!
             * Scheduling %Service : %Client side implementation.
             *
             * This class provides the client side of the Scheduling interface.
             */
            struct Client: public IClient
            {
               /*!
                * Constructor.
                *
                * @param [in] unit  reference to the unit containing this service.
                */
               Client(Unit0 &unit): IClient(unit)
               {}

               virtual ~Client()
               {}
            };

            /*!
             * Scheduling %Service : %Server side implementation.
             *
             * This class provides the server side of the Scheduling interface.
             */
            struct IServer: public virtual Scheduling::IServer,
               public ServiceRole<Base, HF::Interface::SERVER_ROLE>
            {
               using Server = ServiceRole<Base, HF::Interface::SERVER_ROLE>;

               uint16_t uid() const
               {
                  return Server::uid();
               }

               HF::Attributes::UIDS attributes(uint8_t uid = HF::Attributes::Pack::MANDATORY) const
               {
                  return Scheduling::IServer::attributes(uid);
               }

               HF::Attributes::IAttribute *attribute(uint8_t uid)
               {
                  return Scheduling::IServer::attribute(uid);
               }

               virtual Common::Result define_event(const Protocol::Packet &packet,
                                                   Scheduling::DefineEvent<Interval> &msg);


               //! Constructor
               IServer(Unit0 &unit): Scheduling::IServer(), Server(unit)
               {}

               //! Destructor
               virtual ~IServer() = default;

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
                * Get the Event Scheduling entry given by @c id.
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

               using Server::notify;

               // =============================================================================
               // Protected types and methods
               // =============================================================================

               protected:

               Common::Result handle_command(Protocol::Packet &packet, Common::ByteArray &payload,
                                             uint16_t offset);


               void send(const Protocol::Address &addr, Protocol::Message &message,
                         Transport::Link *link)
               {
                  Base::send(addr, message, link);
               }
            };

            /*!
             * Scheduling %Service : %Server side implementation.
             *
             * This class provides the server side of the Scheduling interface.
             */
            struct Server: public IServer
            {
               protected:

               Entries<Interval> _entries;

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
               Entries<Interval> &entries() const
               {
                  return const_cast<Entries<Interval> &>(_entries);
               }
            };


         } // namespace Event
      }    // namespace Scheduling
   }       // namespace Core
}          // namespace HF


#endif /* HF_CORE_EVENT_SCHEDULING_H */
