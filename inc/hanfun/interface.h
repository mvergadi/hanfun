// =============================================================================
/*!
 * @file       inc/hanfun/interface.h
 *
 * This file contains the definitions common to all interfaces.
 *
 * @version    1.4.3
 *
 * @copyright  Copyright &copy; &nbsp; 2014 ULE Alliance
 *
 * For licensing information, please see the file 'LICENSE' in the root folder.
 *
 * Initial development by Bithium S.A. [http://www.bithium.com]
 */
// =============================================================================
#ifndef HF_INTERFACE_H
#define HF_INTERFACE_H

#include "hanfun/common.h"
#include "hanfun/attributes.h"

#include "hanfun/protocol.h"

namespace HF
{
   /*!
    * @defgroup interfaces Interfaces
    *
    * This module contains the classes that define and implement the %Interfaces API.
    */

   /*!
    * @addtogroup common_itf Common
    * @ingroup interfaces
    *
    * This module contains the common classes for the %Interfaces API implementation.
    * @{
    */

   /*!
    * Common interface for all Interfaces.
    */
   struct Interface
   {
      /*!
       * Interface roles.
       */
      typedef enum _Role
      {
         CLIENT_ROLE = 0,   //!< Client Role.
         SERVER_ROLE = 1,   //!< Server Role.
      } Role;

      /*!
       * Interfaces Unique Identifiers (UID).
       */
      typedef enum _UID
      {
         /* Core Services */
         DEVICE_MANAGEMENT       = 0x0001, //!< %Device Management interface %UID.
         BIND_MANAGEMENT         = 0x0002, //!< Bind Management interface %UID.
         GROUP_MANGEMENT         = 0x0003, //!< Group Management interface %UID. __Not implemented__
         IDENTIFY                = 0x0004, //!< Identify interface %UID. __Not implemented__
         DEVICE_INFORMATION      = 0x0005, //!< %Device information interface UID.
         ATTRIBUTE_REPORTING     = 0x0006, //!< %Attribute Reporting interface UID.
         BATCH_PROGRAM_MANGEMENT = 0x0007, //!< Batch Program Management interface UID. __Not implemented__
         EVENT_SCHEDULING        = 0x0008, //!< Event Scheduling interface UID. __Not implemented__
         WEEKLY_SCHEDULING       = 0x0009, //!< Weekly Scheduling interface UID. __Not implemented__
         TAMPER_ALERT            = 0x0101, //!< Tamper %Alert interface UID. __Not implemented__
         TIME                    = 0x0102, //!< %Time interface UID. __Not implemented__
         POWER                   = 0x0110, //!< Power interface UID. __Not implemented__
         KEEP_ALIVE              = 0x0115, //!< Keep Alive interface UID. __Not implemented__
         RSSI                    = 0x0111, //!< %RSSI interface UID.
         SUOTA                   = 0x0400, //!< %SUOTA interface UID.

         /* Functional Interfaces. */
         ALERT                 = 0x0100, //!< Alert interface UID
         ON_OFF                = 0x0200, //!< ON-OFF interface UID
         LEVEL_CONTROL         = 0x0201, //!< Level Control interface UID
         SIMPLE_POWER_METER    = 0x0300, //!< Simple Power Meter interface UID
         SIMPLE_TEMPERATURE    = 0x0301, //!< Simple Temperature interface UID
         SIMPLE_HUMIDITY       = 0x0302, //!< Simple Humidity interface UID
         SIMPLE_THERMOSTAT     = 0x0303, //!< Simple Thermostat interface UID
         SIMPLE_BUTTON         = 0x0304, //!< Simple Button interface UID.
         SIMPLE_VISUAL_EFFECTS = 0x0305, //!< Simple Visual Effects interface UID.
         SIMPLE_AIR_PRESSURE   = 0x0306, //!< Simple Air Pressure interface UID.
         COLOUR_CONTROL        = 0x0202, //!< FIXME Colour Control interface UID.

         /* Reserved */
         RESERVED = 0x7F00,              //!< Proprietary interfaces.
         MAX_UID  = 0x7FFE,              //!< Max interface UID value.
         ANY_UID  = 0x7FFF,              //!< Any interface UID value.
      } UID;

      struct Any: public Common::Interface
      {
         Any(): Common::Interface(ANY_UID)
         {}
      };

      //! Maximum value for command IDs in interfaces.
      static constexpr uint8_t MAX_CMD_ID = 0xFF;

      virtual ~Interface() {}

      // =============================================================================
      // API
      // =============================================================================

      /*!
       * This method returns the interface UID.
       *
       * Subclasses MUST override this method to provide a valid value
       * for the interface being implemented.
       *
       * This method returns a @c uint16_t and not a Interface::UID,
       * to provide support for proprietary interfaces.
       *
       * @return  the UID for the interface.
       */
      virtual uint16_t uid() const = 0;

      /*!
       * Return the Interface::Role this interface implements.
       *
       * @return  the Interface::Role implemented by the interface.
       */
      virtual Interface::Role role() const = 0;

      /*!
       * Handle incoming messages from the network.
       *
       * @param [in]    packet   the packet receive from the network.
       *
       * @param [in]    payload  the byte array containing the data received from the
       *                         network.
       *
       * @param [in]    offset   the offset the payload start at in the byte array.
       *
       * @return        the result of the message processing.
       */
      virtual Common::Result handle(Protocol::Packet &packet, Common::ByteArray &payload,
                                    uint16_t offset) = 0;

      /*!
       * Handle periodic processing.
       *
       * @param [in] time    current system clock value in seconds.
       */
      virtual void periodic(uint32_t time) = 0;

      /*!
       * Return a pointer to the interface attribute with the given @c uid.
       *
       * @param [in] uid   identifier of the attribute in the interface.
       *
       * @return     a pointer to the attribute if it exists,
       *             @c nullptr otherwise.
       */
      virtual HF::Attributes::IAttribute *attribute(uint8_t uid) = 0;

      /*!
       * Return a vector containing the attribute UIDs, for the given pack ID.
       *
       * @param [in] pack_id  the Attribute pack ID to get the attributes UIDs for.
       *
       * @return  vector containing the attributes UIDs.
       */
      virtual HF::Attributes::UIDS attributes(
         uint8_t pack_id = HF::Attributes::Pack::MANDATORY) const = 0;
   };

   /*! @} */

   /*!
    * This is the top-level namespace for the implemented %Interfaces.
    */
   namespace Interfaces
   {
      /*!
       * @addtogroup common_itf
       * @{
       */

      /*!
       * Top-level parent class for all implemented interfaces.
       *
       * This class provides the implementation of the common functionality present in
       * all interfaces implementations.
       */
      struct AbstractInterface: virtual public HF::Interface
      {
         virtual ~AbstractInterface() {}

         Common::Result handle(Protocol::Packet &packet, Common::ByteArray &payload,
                               uint16_t offset);

         void periodic(uint32_t time)
         {
            UNUSED(time);
         }

         HF::Attributes::IAttribute *attribute(uint8_t uid)
         {
            UNUSED(uid);
            return nullptr;
         }

         virtual HF::Attributes::UIDS attributes(
            uint8_t pack_id = HF::Attributes::Pack::MANDATORY) const
         {
            UNUSED(pack_id);
            return HF::Attributes::UIDS();
         }

         bool operator==(AbstractInterface &other)
         {
            return uid() == other.uid();
         }

         bool operator!=(AbstractInterface &other)
         {
            return !(*this == other);
         }

         protected:

         /*!
          * Send message @c msg to the network address given by @c addr.
          *
          * @param [in] addr        HF network address.
          * @param [in] message     pointer to the message to be sent to the network.
          */
         virtual void send(const Protocol::Address &addr, Protocol::Message &message) = 0;

         /*!
          * Notify that an attribute value as changed.
          *
          * @param [in] old_value   attribute's old value.
          * @param [in] new_value   attribute's new value.
          */
         virtual void notify(const HF::Attributes::IAttribute &old_value,
                             const HF::Attributes::IAttribute &new_value) const
         {
            UNUSED(old_value);
            UNUSED(new_value);
         }

         /*!
          * Check if message has correct attributes to be processed by the interface.
          *
          * The attributes checked are :
          * - Interface UID;
          * - Interface Role;
          *
          * @see HF::Interface::handle
          *
          * @param [in] message  incoming message.
          * @param [in] payload  message payload.
          * @param [in] offset   offset in payload array to start reading.
          *
          * @retval Common::Result::OK       if message is for this interface;
          * @retval Common::Result::FAIL_ARG otherwise.
          */
         Common::Result check(Protocol::Message &message, Common::ByteArray &payload,
                              uint16_t offset);

         /*!
          * Check if @c payload data size if sufficient for processing the @c message.
          *
          * @see HF::Interface::handle
          *
          * @param [in] message  incoming message.
          * @param [in] payload  message payload.
          * @param [in] offset   offset in payload array to start reading.
          *
          * @retval Common::Result::OK       if message is for this interface;
          * @retval Common::Result::FAIL_ARG otherwise.
          */
         Common::Result check_payload_size(Protocol::Message &message, Common::ByteArray &payload,
                                           uint16_t offset);

         /*!
          * Return the minimal payload size that should be present for the given
          * message.
          *
          * @param [in] message message that was received.
          *
          * @return  the minimum size in bytes that the packet payload should hold.
          */
         virtual uint16_t payload_size(Protocol::Message &message) const;

         /*!
          * Return the minimal payload size that a message should hold when
          * addressed at the given interface.
          *
          * @param [in] itf   the interface being address.
          *
          * @return  the minimum number of bytes for the message for the interface.
          */
         virtual uint16_t payload_size(Protocol::Message::Interface &itf) const
         {
            UNUSED(itf);
            return 0;
         }

         /*!
          * Helper function template to retrieve minimum size required for
          * serializing/deserializing the class given.
          *
          * @tparam _Message  message class to get the minimum size to of.
          *
          * @return  the minimum size required to serializing/deserializing given class.
          */
         template<typename _Message>
         uint16_t payload_size_helper() const
         {
            _Message message;
            return message.size();
         }

         /*!
          * @copydoc HF::Interface::handle
          *
          * Handle command request/response messages, i.e. :
          *  - Protocol::Message:Type::COMMAND_REQ;
          *  - Protocol::Message:Type::COMMAND_RESP_REQ;
          *  - Protocol::Message:Type::COMMAND_RES;
          */
         virtual Common::Result handle_command(Protocol::Packet &packet,
                                               Common::ByteArray &payload, uint16_t offset);

         /*!
          * @copydoc HF::Interface::handle
          *
          * Handle attributes request/response messages, i.e. :
          *  - Protocol::Message:Type::GET_ATTR_REQ;
          *  - Protocol::Message:Type::SET_ATTR_REQ;
          *  - Protocol::Message:Type::SET_ATTR_RESP_REQ;
          *  - Protocol::Message:Type::GET_ATTR_PACK_REQ;
          *  - Protocol::Message:Type::SET_ATTR_PACK_REQ;
          *  - Protocol::Message:Type::SET_ATTR_PACK_RESP_REQ;
          */
         virtual Common::Result handle_attribute(Protocol::Packet &packet,
                                                 Common::ByteArray &payload, uint16_t offset);

         /*!
          * Check if the given UID matches the interface UID.
          *
          * @param [in] uid   the UID value to check against the interface UID.
          *
          * @retval  true     the UIDs match,
          * @retval  false    otherwise.
          */
         virtual bool check_uid(uint16_t uid) const = 0;
      };

      /*!
       * Helper class template for parent class implementation of the interfaces.
       *
       * @tparam _uid   interface UID to be used by the interface.
       */
      template<uint16_t _uid>
      struct Interface: public AbstractInterface
      {
         //! @copydoc HF::Interface::uid
         uint16_t uid() const
         {
            return _uid;
         }

         protected:

         bool check_uid(uint16_t uid) const
         {
            return Interface::uid() == uid;
         }
      };

      /* *INDENT-OFF* */
      /*!
       * @copydoc HF::Interfaces::Interface
       *
       * @deprecated This template class has been deprecated please use HF::Interfaces::Interface
       */
      template<uint16_t _uid>
      struct __attribute__((deprecated)) Base: public Interface<_uid>{};
      /* *INDENT-ON* */

      /*!
       * Helper class template for implementing a given interface role.
       *
       * @tparam Itf    parent interface class.
       * @tparam _role  interface role implemented.
       */
      template<typename Itf, HF::Interface::Role _role>
      struct InterfaceRole: public Itf
      {
         //! @copydoc HF::Interface::role
         HF::Interface::Role role() const
         {
            return _role;
         }
      };

      /*!
       * Helper template class to allow interfaces implementation to be
       * added as fields to other classes.
       *
       * @tparam  _Interface  base parent interface class.
       * @tparam  _Proxy      class providing the required methods for proper interface
       *                      usage.
       */
      template<typename _Interface, typename _Proxy>
      struct Proxy: public _Interface
      {
         static_assert(std::is_base_of<Interfaces::AbstractInterface, _Interface>::value,
                       "Interface MUST be of type HF::Interfaces::AbstractInterface !");

         typedef _Interface base;

         Proxy(_Proxy &_proxy): proxy(_proxy)
         {}

         //! @copydoc HF::Interfaces::AbstractInterface::send
         void send(const Protocol::Address &addr, Protocol::Message &message)
         {
            proxy.send(addr, message);
         }

         //! @copydoc HF::Interfaces::AbstractInterface::notify
         void notify(const HF::Attributes::IAttribute &old_value,
                     const HF::Attributes::IAttribute &new_value) const
         {
            proxy.notify(old_value, new_value);
         }

         protected:

         //! Referent to the class providing the required functionality.
         _Proxy &proxy;
      };

      /*!
       * Helper class to add optional interfaces to other classes.
       */
      template<typename Base, typename... Proxies>
      class Container
      {
         using interfaces_t = std::tuple<Proxies...>;

         //! Tuple containing the interfaces.
         interfaces_t _interfaces;

         public:

         /*!
          * Constructor
          *
          * @param [in] base  reference to the object containing the interfaces.
          */
         Container(Base &base): _interfaces(Proxies(base) ...)
         {}

         //! @copydoc HF::Interface::handle
         Common::Result handle(Protocol::Packet &packet, Common::ByteArray &payload,
                               uint16_t offset)
         {
            HF::Interface *itf = find<0, Proxies...>(packet.message.itf.id);

            if (itf != nullptr)
            {
               return itf->handle(packet, payload, offset);
            }
            else
            {
               return Common::Result::FAIL_SUPPORT;
            }
         }

         /*!
          * @copydoc HF::Profiles::IProfile::attributes
          *
          * @param [inout] attr_list   list to place the attributes UIDs.
          */
         void attributes(HF::Attributes::List &attr_list, Common::Interface itf, uint8_t pack_id,
                         const HF::Attributes::UIDS &uids) const
         {
            attributes_itf<0, Proxies...>(attr_list, itf, pack_id, uids);
         }

         /*!
          * Return the list of interfaces present in the wrapper.
          *
          * @return  a vector containing the UIDs for the interfaces
          *          wrapped by this class.
          */
         std::vector<Common::Interface> interfaces() const
         {
            std::vector<Common::Interface> result;
            result.reserve(sizeof ... (Proxies));

            Common::Interface temp;
            /* *INDENT-OFF* */
            for_each ([&result, &temp](HF::Interface &itf)
            {
                temp.id = itf.uid ();
                temp.role = itf.role ();
                result.push_back (temp);
            });
            /* *INDENT-ON* */

            return result;
         }

         void periodic(uint32_t time)
         {
            /* *INDENT-OFF* */
            for_each ([time](HF::Interface &itf) { itf.periodic (time);});
            /* *INDENT-ON* */
         }

         /*!
          * Retrieve a pointer to the N optional interface implemented by this unit.
          *
          * @tparam N   index of the interface to retrieve the pointer to.
          *
          * @return  a pointer to the optional implemented interface.
          */
         template<uint8_t N>
         const typename std::tuple_element<N, interfaces_t>::type::base * get() const
         {
            return &std::get<N>(_interfaces);
         }

         protected:

         /*!
          * Call the given function for all the interfaces.
          *
          * @param [in] func  function to call with each of the optional implemented interfaces.
          */
         void for_each(std::function<void(HF::Interface &)> func) const
         {
            for_each<0, Proxies...>(func);
         }

         /*!
          * Call the given function for all the interfaces.
          *
          * @param [in] func  function to call with each of the optional implemented interfaces.
          */
         void for_each(std::function<void(HF::Interface &)> func)
         {
            for_each<0, Proxies...>(func);
         }

         protected:

         /*!
          * Find the interface with the given UID.
          *
          * @param itf_uid    the interface UID to search for in the wrapped interfaces.
          *
          * @tparam  N        index in the interfaces tuple to check if UID matches.
          * @tparam  Head     class for the interface at the given index.
          * @tparam  Tail     the classes associated with the remaining optional interfaces.
          *
          * @return           a pointer to the interface or @c nullptr if
          *                   the interface is not present.
          */
         template<uint8_t N, typename Head, typename... Tail>
         HF::Interface *find(uint16_t itf_uid) const
         {
            static_assert(std::is_base_of<HF::Interface, Head>::value,
                          "Head must be of type HF::Interface");

            const Head &head = std::get<N>(_interfaces);

            if (head.uid() == itf_uid)
            {
               return const_cast<Head *>(&head);
            }
            else
            {
               return find<N + 1, Tail...>(itf_uid);
            }
         }

         /*!
          * Final template instantiation that finds the wrapped interface
          * with the given UID.
          *
          * @param itf_uid    the interface UID to search for in the wrapped interfaces.
          *
          * @tparam  N  index in the interfaces tuple to check if UID matches.
          *
          * @return  @c nullptr, i.e. the interface is not present.
          */
         template<uint8_t N>
         HF::Interface *find(uint16_t itf_uid) const
         {
            UNUSED(itf_uid);
            return nullptr;
         }

         /*!
          * Helper template function to implement the HF::Units::Unit::for_each functionality.
          *
          * @param [in] func  function to call with the reference for the wrapped interface at
          *                   index @c N.
          *
          * @tparam  N     index in the wrapped interfaces tuple to check if UID matches.
          * @tparam  Head  class for the wrapped interface at the given index.
          * @tparam  Tail  the classes associated with the remaining wrapped interfaces.
          */
         template<uint8_t N, typename Head, typename... Tail>
         void for_each(std::function<void(HF::Interface &)> func) const
         {
            const auto &head = std::get<N>(this->_interfaces);

            func(*(const_cast<HF::Interface *>(static_cast<const HF::Interface *>(&head))));

            for_each<N + 1, Tail...>(func);
         }

         /*!
          * Helper template function to implement the HF::Units::Unit::for_each functionality.
          *
          * @param [in] func  function to call with the reference for the wrapped interface at
          *                   index @c N.
          *
          * @tparam  N     index in the wrapped interfaces tuple to check if UID matches.
          */
         template<uint8_t N>
         void for_each(std::function<void(HF::Interface &)> func) const
         {
            UNUSED(func);
         }

         /*!
          * Helper function used to provide HF::Units::Unit::attributes functionality.
          *
          *
          * @param [out] attrs      attribute list to append the attributes for the interface to.
          * @param [in]  itf        interface to get the attributes for.
          * @param [in]  pack_id    attributes pack id to use when retrieving the attributes.
          * @param [in]  uids       list of attribute uids to get attributes for if @c pack_id is
          *                         HF::Attributes::Pack::DYNAMIC.
          *
          * @tparam  N     index in the wrapped interfaces tuple to check if UID matches.
          * @tparam  Head  class for the wrapped interface at the given index.
          * @tparam  Tail  the classes associated with the remaining wrapped interfaces.
          */
         template<uint8_t N, typename Head, typename... Tail>
         void attributes_itf(HF::Attributes::List &attrs, Common::Interface itf,
                             uint8_t pack_id, const HF::Attributes::UIDS &uids) const
         {
            const auto &head = std::get<N>(this->_interfaces);

            if (head.uid() == itf.id)
            {
               auto result = HF::Attributes::get(head, pack_id, uids);
               attrs.merge(result);
            }
            else
            {
               attributes_itf<N + 1, Tail...>(attrs, itf, pack_id, uids);
            }
         }

         /*!
          * Helper function used to provide HF::Units::Unit::attributes functionality.
          *
          * Template expansion end.
          *
          * @param [out] attrs      attribute list to append the attributes for the interface to.
          * @param [in]  itf        interface to get the attributes for.
          * @param [in]  pack_id    attributes pack id to use when retrieving the attributes.
          * @param [in]  uids       list of attribute uids to get attributes for if @c pack_id is
          *                         HF::Attributes::Pack::DYNAMIC.
          *
          * @tparam N    index in the wrapped interfaces tuple to check if UID matches.
          */
         template<uint8_t N>
         void attributes_itf(HF::Attributes::List &attrs, Common::Interface itf,
                             uint8_t pack_id, const HF::Attributes::UIDS &uids) const
         {
            UNUSED(attrs);
            UNUSED(itf);
            UNUSED(pack_id);
            UNUSED(uids);
         }
      };

      /*! @} */

   }  // namespace Interfaces

   /*! @} */

}  // namespace HF

/*!
 * @addtogroup common_itf
 * @{
 */

// =============================================================================
// Stream Helpers
// =============================================================================

/*!
 * Convert the given @c role into a string and write it to the given @c stream.
 *
 * @param [in] stream   out stream to write the string to.
 * @param [in] role     role value to convert to a string.
 *
 * @return   <tt>stream</tt>
 */
std::ostream &operator<<(std::ostream &stream, const HF::Interface::Role role);

/*!
 * Convert the given @c uid into a string and write it to the given @c stream.
 *
 * @param [in] stream   out stream to write the string to.
 * @param [in] uid      uid value to convert to a string.
 *
 * @return   <tt>stream</tt>
 */
std::ostream &operator<<(std::ostream &stream, const HF::Interface::UID uid);

/*! @} */

#endif /* HF_INTERFACE_H */
