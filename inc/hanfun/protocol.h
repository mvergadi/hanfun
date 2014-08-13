// =============================================================================
/*!
 * \file       inc/hanfun/protocol.h
 *
 * This file contains the definitions for the HAN-FUN protocol messages.
 *
 * \version    0.3.2
 *
 * \copyright  Copyright &copy; &nbsp; 2014 ULE Alliance
 *
 * For licensing information, please see the file 'LICENSE' in the root folder.
 *
 * Initial development by Bithium S.A. [http://www.bithium.com]
 */
// =============================================================================
#ifndef HF_PROTOCOL_H
#define HF_PROTOCOL_H

#include <list>

#include "hanfun/common.h"

// =============================================================================
// API
// =============================================================================

namespace HF
{
   // Forward declaration.
   namespace Transport
   {
      struct Link;
   }

   /*!
    * HAN-FUN Protocol implementation.
    */
   namespace Protocol
   {
      //! HAN-FUN Broadcast - device address.
      constexpr uint16_t BROADCAST_ADDR = 0x7FFF;

      //! HAN-FUN Broadcast - unit address.
      constexpr uint8_t BROADCAST_UNIT = 0xFF;

      //! HAN-FUN Network - Maximum application payload length.
      constexpr uint16_t MAX_PAYLOAD = 0x01FF;

      // =============================================================================
      // Classes
      // =============================================================================

      /*!
       * Network Message.
       */
      struct Message
      {
         /*!
          * Message types.
          */
         typedef enum
         {
            COMMAND_REQ                   = 0x01, //!< Command request
            COMMAND_RESP_REQ              = 0x02, //!< Command request with response required.
            COMMAND_RES                   = 0x03, //!< Command response.

            GET_ATTR_REQ                  = 0x04, //!< Get attributes request.
            GET_ATTR_RES                  = 0x05, //!< Get attributes response.

            SET_ATTR_REQ                  = 0x06, //!< Set attributes request.
            SET_ATTR_RESP_REQ             = 0x07, //!< Set attributes request with response required.
            SET_ATTR_RES                  = 0x08, //!< Set attributes response.

            GET_ATTR_PACK_REQ             = 0x09, //!< Get pack attributes request.
            GET_ATTR_PACK_RES             = 0x0A, //!< Get pack attributes response.

            SET_ATTR_PACK_REQ             = 0x0B, //!< Set pack attributes request.
            SET_ATTR_PACK_RESP_REQ        = 0x0C, //!< Set pack attributes request with response required.
            SET_ATTR_PACK_RES             = 0x0D, //!< Set pack attributes response.

            ATOMIC_SET_ATTR_PACK_REQ      = 0x0E, //!< Atomic set pack attributes request.
            ATOMIC_SET_ATTR_PACK_RESP_REQ = 0x0F, //!< Atomic set pack attributes request with response required.
            ATOMIC_SET_ATTR_PACK_RES      = 0x10, //!< Atomic set pack attributes response.
         } Type;

         /*!
          * Interface Address.
          */
         struct Interface:public Common::Interface
         {
            uint8_t member;            //!< Interface destination member.

            Interface(uint16_t uid = 0, uint16_t role = 0, uint8_t member = 0):Common::Interface(uid, role), member (member) {}

            //! \see HF::Serializable::size.
            size_t size () const;

            //! \see HF::Serializable::pack.
            size_t pack (Common::ByteArray &array, size_t offset = 0) const;

            //! \see HF::Serializable::unpack.
            size_t unpack (const Common::ByteArray &array, size_t offset = 0);
         };

         // =============================================================================
         // API
         // =============================================================================

         uint8_t   reference;          //!< Application reference.
         Type      type;               //!< Message type. \see Message::Type

         Interface itf;                //!< Interface Address.

         /*!
          * Message payload.
          */
         Common::ByteArray payload;

         //! The payload length value read when unpacking the message.
         uint16_t length;

         Message(size_t size = 0, Type _type = COMMAND_REQ):
            reference (0), type (_type), payload (Common::ByteArray (size)), length (0) {}

         Message(Message &parent, size_t size);

         //! \see HF::Serializable::size.
         size_t size () const;

         //! \see HF::Serializable::pack.
         size_t pack (Common::ByteArray &array, size_t offset = 0) const;

         //! \see HF::Serializable::unpack.
         size_t unpack (const Common::ByteArray &array, size_t offset = 0);
      };

      /*!
       * Network Address.
       */
      struct Address
      {
         uint16_t mod    : 1;    //!< Address modifier.
         uint16_t device : 15;   //!< Device Address.

         uint8_t  unit;          //!< Source Unit.

         /*!
          * HAN-FUN Network Destination Address Types.
          */
         typedef enum
         {
            DEVICE = 0,   //!< Destination address is for single device.
            GROUP  = 1,   //!< Destination address is for a group of devices.
         } Type;

         /*!
          * Create a new message address.
          *
          * @param _dev    device address. Default \c HF_BROADCAST_ADDR.
          * @param _unit   unit address. Default \c HF_BROADCAST_UNIT.
          * @param _mod    address modifier. Default \c DEVICE_ADDR.
          */
         Address(uint16_t _dev = BROADCAST_ADDR, uint8_t _unit = BROADCAST_UNIT,
                 Type _mod = DEVICE)
            :mod (_mod), device (_dev), unit (_unit)
         {}

         //! \see HF::Serializable::size.
         size_t size () const;

         //! \see HF::Serializable::pack.
         size_t pack (Common::ByteArray &array, size_t offset = 0) const;

         //! \see HF::Serializable::unpack.
         size_t unpack (const Common::ByteArray &array, size_t offset = 0);

         /*!
          * Checks if this address if for the Protocol::BROADCAST_ADDR and
          * Protocol::BROADCAST_UNIT.
          *
          * @retval true   if it is the network's broadcast address,
          * @retval false  otherwise.
          */
         bool is_broadcast ()
         {
            return device == BROADCAST_ADDR && unit == BROADCAST_UNIT;
         }

         /*!
          * Checks if the given device \c address is equal to the device address
          * present in this Protocol::Address object.
          *
          * @param [in] address  network address to match.
          *
          * @retval true   if it is the network's address are equal,
          * @retval false  otherwise.
          */
         bool is_local (uint16_t address)
         {
            return this->device == address;
         }
      };

      /*!
       * HAN-FUN Protocol Packet.
       */
      struct Packet
      {
         Address source;           //!< Source Address.
         Address destination;      //!< Destination Address.

         /*!
          * Packet message payload;
          */
         Message         message;

         Transport::Link *link;     //! Link where this packet originated from.

         Packet() {}

         Packet(Message &message):message (message) {}

         Packet(Address &dst_addr, Message &message, uint8_t unit = BROADCAST_UNIT):
            destination (dst_addr), message (message)
         {
            source.mod    = Address::DEVICE;
            source.device = BROADCAST_ADDR;
            source.unit   = unit;
         }

         Packet(Address &src_addr, Address &dst_addr, Message &message):
            source (src_addr), destination (dst_addr), message (message) {}

         //! \see HF::Serializable::size.
         size_t size () const;

         //! \see HF::Serializable::pack.
         size_t pack (Common::ByteArray &array, size_t offset = 0) const;

         //! \see HF::Serializable::unpack.
         size_t unpack (const Common::ByteArray &array, size_t offset = 0);
      };

      /*!
       * HAN-FUN Response message.
       */
      struct Response
      {
         // =============================================================================
         // API
         // =============================================================================

         Common::Result code;

         Response(Common::Result code = Common::Result::OK):code (code) {}

         //! \see HF::Serializable::size.
         size_t size () const;

         //! \see HF::Serializable::pack.
         size_t pack (Common::ByteArray &array, size_t offset = 0) const;

         //! \see HF::Serializable::unpack.
         size_t unpack (const Common::ByteArray &array, size_t offset = 0);
      };

      // =============================================================================
      // Filter classes
      // =============================================================================

      /*!
       * This namespace contains the different packet filters.
       */
      namespace Filters
      {
         /*!
          * This class provides support for detecting repeated messages received from the
          * network.
          *
          * It uses the source device address, the application reference value for the message
          * and a checksum over the entire packet received to determine if the received packet
          * is a retransmission.
          */
         class Repeated
         {
            protected:

            //! Filter database entry.
            struct Entry
            {
               uint16_t address;       //!< Source device address.
               uint8_t reference;     //!< Last message application reference.
               uint8_t ttl;           //!< Time to live - counter used to drop older entries.
               uint32_t checksum;      //!< Last message checksum.

               Entry (uint16_t _address = HF::Protocol::BROADCAST_ADDR, uint8_t _reference = 0) :
                     address(_address), reference(_reference), ttl(0), checksum(0)
               {
               }

               bool operator< (const Entry &other) const
                               {
                  return this->address < other.address;
               }
            };

            //! Filter database.
            std::list<Entry> db;

            public:

            //! Maximum number of entries that will be kept in the database.
            static constexpr size_t max_size = HF_PROTOCOL_FILTER_REPEATED_MAX_SIZE;

            /*!
             * Checks if the given \c packet, is a retransmission according to
             * the filters database data.
             *
             * The given \c packet and \c payload are used to update the filters database.
             *
             * @param [in] packet     reference to the incoming packet.
             * @param [in] payload    reference to the packet's payload.
             *
             * @retval  true     the packet is a retransmission.
             * @retval  false    the packet is a not retransmission.
             */
            bool operator() (const HF::Protocol::Packet &packet,
                             const HF::Common::ByteArray &payload);

            /*!
             * Number of entries in the filter's database.
             *
             * @return  the number of entries in the filter's database.
             */
            size_t size () const
            {
               return db.size();
            }

            protected:

            /*!
             * Create a checksum of the data that is contained in the buffer \c data.
             *
             * This method implements the Fletcher-32 checksum algorithm.
             *
             * @param data    pointer to the data to calculate the checksum for.
             * @param words   number of words in the \c data to calculate the checksum from.
             *
             * @return
             */
            uint32_t checksum (uint16_t const *data, size_t words);
         };

      }  // namespace Filters

      // =============================================================================
      // Operators
      // =============================================================================

      inline bool operator ==(Address const &lhs, Address const &rhs)
      {
         return (lhs.device == rhs.device) && (lhs.unit == rhs.unit) && (lhs.mod == rhs.mod);
      }

      inline bool operator !=(Address const &lhs, Address const &rhs)
      {
         return !(lhs == rhs);
      }

      inline bool operator <(Address const &lhs, Address const &rhs)
      {
         return (lhs.device < rhs.device) ||
                (lhs.device == rhs.device &&
                 (lhs.mod < rhs.mod || (lhs.mod == rhs.mod && lhs.unit < rhs.unit)));
      }

   }  // namespace Protocol

}  // namespace HF

#endif /* HF_PROTOCOL_H */
