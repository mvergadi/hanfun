// =============================================================================
/*!
 * @file       inc/hanfun/core/group_table.h
 *
 * This file contains the definitions for the Group Table service.
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

#ifndef HF_CORE_GROUP_TABLE_H
#define HF_CORE_GROUP_TABLE_H

#include "hanfun/protocol.h"
#include "hanfun/core.h"

namespace HF
{
   namespace Core
   {
      // Forward declaration.
      namespace GroupTable
      {
         class Server;
      }

      /*!
       * @ingroup group_table_itf
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
      HF::Attributes::IAttribute *create_attribute(GroupTable::Server *server, uint8_t uid);

      /*!
       * This namespace contains the implementation of the Group Table service.
       */
      namespace GroupTable
      {
         /*!
          * @addtogroup group_table_itf  Group Table service
          * @ingroup interfaces
          *
          * This module contains the classes that define and implement the Group Table service API.
          * @{
          */
         //! Command IDs.
         typedef enum _CMD
         {
            ADD_CMD          = 0x01, //!< Add command UID.
            REMOVE_CMD       = 0x02, //!< Remove command UID.
            REMOVE_ALL_CMD   = 0x03, //!< Remove All command UID.
            READ_ENTRIES_CMD = 0x04, //!< Read Entries command UID.
            __LAST_CMD__     = READ_ENTRIES_CMD
         } CMD;

         //! Attributes
         typedef enum _Attributes
         {
            NUMBER_OF_ENTRIES_ATTR     = 0x01, //!< Number Of Entries attribute UID.
            NUMBER_OF_MAX_ENTRIES_ATTR = 0x02, //!< Number Of Max Entries attribute UID.
            __LAST_ATTR__              = NUMBER_OF_MAX_ENTRIES_ATTR
         } Attributes;

         // =============================================================================
         // Attribute Helper classes
         // =============================================================================

         /*!
          * Helper class to handle the Number Of Entries attribute for the Group Table service.
          */
         struct NumberOfEntries: public HF::Attributes::Attribute<uint8_t>
         {
            static constexpr uint8_t ID       = NUMBER_OF_ENTRIES_ATTR; //!< Attribute UID.
            static constexpr bool    WRITABLE = false;                  //!< Attribute Read/Write

            NumberOfEntries(uint8_t value = 0, HF::Interface *owner = nullptr):
               Attribute<uint8_t>(HF::Interface::GROUP_TABLE, ID, owner, value, WRITABLE)
            {}
         };

         /*!
          * Helper class to handle the Number Of Max Entries attribute for the Group Table service.
          */
         struct NumberOfMaxEntries: public HF::Attributes::Attribute<uint8_t>
         {
            static constexpr uint8_t ID       = NUMBER_OF_MAX_ENTRIES_ATTR; //!< Attribute UID.
            static constexpr bool    WRITABLE = false;                      //!< Attribute Read/Write

            NumberOfMaxEntries(uint8_t value = 0, HF::Interface *owner = nullptr):
               Attribute<uint8_t>(HF::Interface::GROUP_TABLE, ID, owner, value, WRITABLE)
            {}
         };

         /*!
          * @copybrief HF::Core::create_attribute (HF::Interfaces::GroupTable::Server *,uint8_t)
          *
          * @see HF::Core::create_attribute (HF::Core::GroupTable::Server *,uint8_t)
          *
          * @param [in] uid   attribute %UID to create the attribute object for.
          *
          * @retval  pointer to an attribute object
          * @retval  <tt>nullptr</tt> if the attribute UID does not exist.
          */
         HF::Attributes::IAttribute *create_attribute(uint8_t uid);

         /*!
          * Group Table %Service : Parent.
          *
          * This is the parent class for the Group Table service implementation.
          */
         struct Base: public Service<HF::Interface::GROUP_TABLE>
         {
            protected:

            //! Constructor
            Base(Unit0 &unit): Service<HF::Interface::GROUP_TABLE>(unit) {}
         };

         /*!
          * Group Table %Service : %Server side implementation.
          *
          * This class provides the server side of the Group Table interface.
          */
         class Server: public ServiceRole<GroupTable::Base, HF::Interface::SERVER_ROLE>
         {
            protected:

            uint8_t _number_of_entries;     //!< Number Of Entries
            uint8_t _number_of_max_entries; //!< Number Of Max Entries

            public:

            //! Constructor
            Server(Unit0 &unit): ServiceRole<GroupTable::Base, HF::Interface::SERVER_ROLE>(unit) {}

            //! Destructor
            virtual ~Server() {}

            // ======================================================================
            // Events
            // ======================================================================
            //! @name Events
            //! @{

            /*!
             * Callback that is called when a @c GroupTable::ADD_CMD,
             * is received.
             *
             * @param [in] addr       the network address to send the message to.
             */
            virtual void add(const Protocol::Address &addr);

            /*!
             * Callback that is called when a @c GroupTable::REMOVE_CMD,
             * is received.
             *
             * @param [in] addr       the network address to send the message to.
             */
            virtual void remove(const Protocol::Address &addr);

            /*!
             * Callback that is called when a @c GroupTable::REMOVE_ALL_CMD,
             * is received.
             *
             * @param [in] addr       the network address to send the message to.
             */
            virtual void remove_all(const Protocol::Address &addr);

            /*!
             * Callback that is called when a @c GroupTable::READ_ENTRIES_CMD,
             * is received.
             *
             * @param [in] addr       the network address to send the message to.
             */
            virtual void read_entries(const Protocol::Address &addr);

            //! @}
            // ======================================================================

            // =============================================================================
            // Get/Set API.
            // =============================================================================

            /*!
             * Get the Number Of Entries for the Group Table server.
             *
             * @return  the current Number Of Entries.
             */
            uint8_t number_of_entries() const;

            /*!
             * Set the Number Of Entries for the Group Table server.
             *
             * @param [in] __value the  Number Of Entries value to set the server to.
             */
            void number_of_entries(uint8_t __value);

            /*!
             * Get the Number Of Max Entries for the Group Table server.
             *
             * @return  the current Number Of Max Entries.
             */
            uint8_t number_of_max_entries() const;

            /*!
             * Set the Number Of Max Entries for the Group Table server.
             *
             * @param [in] __value the  Number Of Max Entries value to set the server to.
             */
            void number_of_max_entries(uint8_t __value);

            // =============================================================================
            // Attribute API.
            // =============================================================================

            HF::Attributes::IAttribute *attribute(uint8_t uid);

            HF::Attributes::UIDS attributes(uint8_t pack_id =
                                               HF::Attributes::Pack::MANDATORY) const;

            protected:

            Common::Result handle_command(Protocol::Packet &packet, Common::ByteArray &payload,
                                          uint16_t offset);
         };

         /*!
          * Group Table %Service : %Client side implementation.
          *
          * This class provides the client side of the Group Table interface.
          */
         struct Client: public ServiceRole<GroupTable::Base, HF::Interface::CLIENT_ROLE>
         {
            Client(Unit0 &unit): ServiceRole<GroupTable::Base, HF::Interface::CLIENT_ROLE>(unit) {}

            virtual ~Client() {}

            // ======================================================================
            // Commands
            // ======================================================================
            //! @name Commands
            //! @{

            /*!
             * Send a HAN-FUN message containing a @c GroupTable::ADD_CMD, to the given
             * network address.
             *
             * @param [in] addr       the network address to send the message to.
             */
            void add(const Protocol::Address &addr);

            /*!
             * Send a HAN-FUN message containing a @c GroupTable::ADD_CMD,
             * to the D:0/U:0 network address.
             */
            void add()
            {
               Protocol::Address addr(0, 0);
               add(addr);
            }

            /*!
             * Send a HAN-FUN message containing a @c GroupTable::REMOVE_CMD, to the given
             * network address.
             *
             * @param [in] addr       the network address to send the message to.
             */
            void remove(const Protocol::Address &addr);

            /*!
             * Send a HAN-FUN message containing a @c GroupTable::REMOVE_CMD,
             * to the D:0/U:0 network address.
             */
            void remove()
            {
               Protocol::Address addr(0, 0);
               remove(addr);
            }

            /*!
             * Send a HAN-FUN message containing a @c GroupTable::REMOVE_ALL_CMD, to the given
             * network address.
             *
             * @param [in] addr       the network address to send the message to.
             */
            void remove_all(const Protocol::Address &addr);

            /*!
             * Send a HAN-FUN message containing a @c GroupTable::REMOVE_ALL_CMD,
             * to the D:0/U:0 network address.
             */
            void remove_all()
            {
               Protocol::Address addr(0, 0);
               remove_all(addr);
            }

            /*!
             * Send a HAN-FUN message containing a @c GroupTable::READ_ENTRIES_CMD, to the given
             * network address.
             *
             * @param [in] addr       the network address to send the message to.
             */
            void read_entries(const Protocol::Address &addr);

            /*!
             * Send a HAN-FUN message containing a @c GroupTable::READ_ENTRIES_CMD,
             * to the D:0/U:0 network address.
             */
            void read_entries()
            {
               Protocol::Address addr(0, 0);
               read_entries(addr);
            }

            //! @}
            // ======================================================================
         };

         /*! @} */

      }  // namespace GroupTable

   }  // namespace Core

}   // namespace HF

/*!
 * @addtogroup group_table_itf
 * @{
 */

// =============================================================================
// Stream Helpers
// =============================================================================

/*!
 * Convert the given @c command into a string and write it to the given @c stream.
 *
 * @param [in] stream   out stream to write the string to.
 * @param [in] command  role value to convert to a string.
 *
 * @return   <tt>stream</tt>
 */
std::ostream &operator<<(std::ostream &stream, const HF::Core::GroupTable::CMD command);

/*!
 * Convert the given @c attribute into a string and write it to the given @c stream.
 *
 * @param [in] stream      out stream to write the string to.
 * @param [in] attribute   attribute value to convert to a string.
 *
 * @return   <tt>stream</tt>
 */
std::ostream &operator<<(std::ostream &stream,
                         const HF::Core::GroupTable::Attributes attribute);
/*! @} */

#endif /* HF_CORE_GROUP_TABLE_H */
