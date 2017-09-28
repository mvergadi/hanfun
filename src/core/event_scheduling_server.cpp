// =============================================================================
/*!
 * @file       src/core/event_scheduling_server.cpp
 *
 * This file contains the implementation of the Event Scheduling service : Server role.
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

#include "hanfun/core/event_scheduling.h"

// =============================================================================
// API
// =============================================================================

using namespace HF;
using namespace HF::Core;
using namespace HF::Core::Scheduling::Event;

// =============================================================================
// IServer::define_event
// =============================================================================
/*!
 *
 */
// =============================================================================
Common::Result Scheduling::Event::IServer::define_event(const Protocol::Packet &packet,
                                                        Scheduling::DefineEvent<Interval> &msg)
{
   Common::Result result = Common::Result::OK;

   uint8_t id            = Entry::AVAILABLE_ID;

   uint8_t size          = entries().size();

   if (size == maximum_number_of_entries())
   {
      result = Common::Result::FAIL_RESOURCES;
      goto _end;
   }

   if (msg.id == Entry::AVAILABLE_ID)
   {
      id = next_id();

      if (id == Entry::AVAILABLE_ID)
      {
         result = Common::Result::FAIL_RESOURCES;
         goto _end;
      }

      msg.id = id;
   }
   else
   {
      if (entry(msg.id) != nullptr)
      {
         result = Common::Result::FAIL_ARG;
         goto _end;
      }
   }

   result = entries().save(static_cast<Entry>(msg));

   if (result != Common::Result::OK)
   {
      result = Common::Result::FAIL_UNKNOWN;
      goto _end;
   }

   _end:

   if (size != entries().size())
   {
      HF_NOTIFY_HELPER(NumberOfEntries, size, entries().size());
   }

   DefineEventResponse response(result, msg.id);

   Protocol::Message message(packet.message, response.size());

   response.pack(message.payload);

   send(packet.source, message, packet.link);

   return result;
}
