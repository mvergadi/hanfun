// =============================================================================
/*!
 * @file       inc/hanfun/core/device_information.h
 *
 * This file contains the definition for the core Device Information interface
 * of the HAN-FUN protocol.
 *
 * @version    1.5.3
 *
 * @copyright  Copyright &copy; &nbsp; 2014 ULE Alliance
 *
 * For licensing information, please see the file 'LICENSE' in the root folder.
 *
 * Initial development by Bithium S.A. [http://www.bithium.com]
 */
// =============================================================================
#ifndef HF_DEVICE_INFORMATION_H
#define HF_DEVICE_INFORMATION_H

#include "hanfun/common.h"

#include "hanfun/interface.h"

#include "hanfun/core.h"

// =============================================================================
// Defines
// =============================================================================

#ifndef HF_DEVICE_MANUFACTURER_CODE
   #warning "HF_DEVICE_MANUFACTURER_CODE not defined ! Using default value '0x0000'"
//! Device Electronic Manufacturer Code
   #define HF_DEVICE_MANUFACTURER_CODE   0x0000
#endif

#ifndef HF_APPLICATION_VERSION
   #warning "HF_APPLICATION_VERSION not defined ! Using default empty value !"
//! Application version.
   #define HF_APPLICATION_VERSION        ""
#endif

#ifndef HF_HARDWARE_VERSION
   #warning "HF_HARDWARE_VERSION not defined ! Using default empty value !"
//! Hardware Version
   #define HF_HARDWARE_VERSION           ""
#endif

#ifndef HF_MANUFACTURER_NAME
   #warning "HF_MANUFACTURER_NAME not defined ! Using default empty value !"
//! Manufacturer name.
   #define HF_MANUFACTURER_NAME          ""
#endif

// =============================================================================
// API
// =============================================================================
namespace HF
{
   namespace Core
   {
      // Forward declaration.
      namespace DeviceInformation
      {
         struct Server;
      }  // namespace DeviceInformation

      /*!
       * @ingroup dev_info
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
      HF::Attributes::IAttribute *create_attribute(HF::Core::DeviceInformation::Server *server,
                                                   uint8_t uid);

      /*!
       * This namespace contains the classes that implement the Device Information service.
       *
       * @todo Add support for Minimum Sleep Time attribute.
       * @todo Add support for Actual %Response Time [Paging Interval] attribute.
       * @todo Add support for %RFPI/%IPUI attribute.
       * @todo Add support for Location attribute.
       * @todo Add support for %Device Enable attribute.
       * @todo Add support for Friendly Name attribute.
       * @todo Add support for %Device Serial Number attribute.
       */
      namespace DeviceInformation
      {
         /*!
          * @addtogroup dev_info Device Information
          * @ingroup core
          *
          * This module contains the classes that implement the %Device Information service.
          *
          * @{
          */
         constexpr static uint8_t CORE_VERSION      = HF::CORE_VERSION;             //!< HAN-FUN Core version.
         constexpr static uint8_t PROFILE_VERSION   = HF::PROFILES_VERSION;         //!< HAN-FUN Profile version.
         constexpr static uint8_t INTERFACE_VERSION = HF::INTERFACES_VERSION;       //!< HAN-FUN Interface version.

         constexpr static uint16_t EMC              = HF_DEVICE_MANUFACTURER_CODE;  //!< Electronic Manufacture Code.

#if HF_CORE_DEV_INFO_APP_VERSION_ATTR
         constexpr static _const char *APPLICATION_VERSION = HF_APPLICATION_VERSION; //!< Application Version.
#endif
#if HF_CORE_DEV_INFO_HW_VERSION_ATTR
         constexpr static _const char *HARDWARE_VERSION = HF_HARDWARE_VERSION;       //!< Hardware Version.
#endif
#if HF_CORE_DEV_INFO_MANUFACTURER_NAME_ATTR
         constexpr static _const char *MANUFACTURER_NAME = HF_MANUFACTURER_NAME;     //!< Manufacturer Name.
#endif

         //! Attributes.
         typedef enum _Attributes
         {
            CORE_VERSION_ATTR      = 0x01,   //!< HF Core version attribute.              (M)
            PROFILE_VERSION_ATTR   = 0x02,   //!< HF Profile version attribute.           (M)
            INTERFACE_VERSION_ATTR = 0x03,   //!< HF Interface version attribute.         (M)
            EXTRA_CAP_ATTR         = 0x04,   //!< Extra capabilities attribute.           (M)
            MIN_SLEEP_TIME_ATTR    = 0x05,   //!< Minimum sleep time attribute.           (O)
            ACTUAL_RESP_TIME_ATTR  = 0x06,   //!< Actual response time attribute.         (O)
            APP_VERSION_ATTR       = 0x07,   //!< Hardware version attribute.             (O)
            HW_VERSION_ATTR        = 0x08,   //!< Hardware version attribute.             (O)
            EMC_ATTR               = 0x09,   //!< Electronic Manufacture Code attribute.  (O)
            DECT_ID_ATTR           = 0x0A,   //!< RFPI / IPUI                             (0)
            MANUFACTURER_NAME_ATTR = 0x0B,   //!< Manufacture's name attribute.           (O)
            LOCATION_ATTR          = 0x0C,   //!< Location attribute.                     (O)
            ENABLED_ATTR           = 0x0D,   //!< Device enabled attribute.               (O)
            FRIENDLY_NAME_ATTR     = 0x0E,   //!< Device friendly name attribute.         (O)
            UID_ATTR               = 0x0F,   //!< Device UID attribute.                   (O)
            SERIAL_NUMBER_ATTR     = 0x10,   //!< Serial number attribute.                (O)
            __LAST_ATTR__          = SERIAL_NUMBER_ATTR,
         } Attributes;

         /*!
          * @copybrief HF::Core::create_attribute (HF::Core::DeviceInformation::Server *,uint8_t)
          *
          * @see HF::Core::create_attribute (HF::Core::DeviceInformation::Server *,uint8_t)
          *
          * @param [in] uid   attribute %UID to create the attribute object for.
          *
          * @retval  pointer to an attribute object
          * @retval  <tt>nullptr</tt> if the attribute UID does not exist.
          */
         HF::Attributes::IAttribute *create_attribute(uint8_t uid);

         /*!
          * Data type to contain the Friendly name attribute.
          */
         struct FriendlyName
         {
            /*!
             * Data type representing a unit0's friendly name.
             */
            struct Unit
            {
               uint8_t     id;
               std::string name;

               typedef HF::Common::SerializableHelper<std::string> string_helper;

               //! Minimum pack/unpack required data size.
               static constexpr uint16_t min_size = sizeof(uint8_t) + string_helper::min_size;

               //! @copydoc HF::Common::Serializable::size
               uint16_t size() const;

               //! @copydoc HF::Common::Serializable::pack
               uint16_t pack(HF::Common::ByteArray &array, uint16_t offset = 0) const;

               //! @copydoc HF::Common::Serializable::unpack
               uint16_t unpack(const HF::Common::ByteArray &array, uint16_t offset = 0);
            };

            //! Device unit's friendly names.
            std::vector<Unit> units;

            //! Minimum pack/unpack required data size.
            static constexpr uint16_t min_size = sizeof(uint8_t);

            //! @copydoc HF::Common::Serializable::size
            uint16_t size() const;

            //! @copydoc HF::Common::Serializable::pack
            uint16_t pack(HF::Common::ByteArray &array, uint16_t offset = 0) const;

            //! @copydoc HF::Common::Serializable::unpack
            uint16_t unpack(const HF::Common::ByteArray &array, uint16_t offset = 0);

            //! @copydoc HF::Attributes::IAttribute::changed
            float changed(const FriendlyName &other) const
            {
               UNUSED(other);
               return 0.0;
            }

            //! @copydoc HF::Attributes::IAttribute::compare
            int compare(const FriendlyName &other) const;
         };

         /*!
          * Parent class for the Device Information interface implementation.
          */
         class Base: public Service<HF::Interface::DEVICE_INFORMATION>
         {
            protected:

            /*!
             * Constructor.
             *
             * @param [in] unit  reference to the unit containing this service.
             */
            Base(Unit0 &unit): Service<HF::Interface::DEVICE_INFORMATION>(unit) {}
         };

         /*!
          * Device Information interface : Server side.
          */
         struct Server: public ServiceRole<Base, HF::Interface::SERVER_ROLE>
         {
            HF::UID::UID device_uid;   //! Device UID.

            /*!
             * Constructor.
             *
             * @param [in] unit  reference to the unit containing this service.
             */
            Server(HF::Core::Unit0 &unit):
               ServiceRole<Base, HF::Interface::SERVER_ROLE>(unit), _capabilities(0)
            {}

            virtual ~Server() {}

            // =============================================================================
            // Interface Attribute API.
            // =============================================================================

            HF::Attributes::IAttribute *attribute(uint8_t uid);

            HF::Attributes::UIDS attributes(uint8_t pack_id =
                                               HF::Attributes::Pack::MANDATORY) const;

            /*!
             * Set extra capabilities attribute paging bit to given @c value.
             *
             * @param [in] value    paging bit value to place in extra capabilities attribute.
             */
            void paging(bool value);

            /*!
             * Get extra capabilities attribute paging bit.
             *
             * @retval  true the broadcast bit is 1.
             * @retval  false the broadcast bit is 0.
             */
            bool has_paging()  const;

            /*!
             * Set extra capabilities attribute broadcast bit to given @c value.
             *
             * @param [in] value    broadcast bit value to place in extra capabilities attribute.
             */
            void broadcast(bool value);

            /*!
             * Get extra capabilities attribute broadcast bit.
             *
             * @retval  true the broadcast bit is 1.
             * @retval  false the broadcast bit is 0.
             */
            bool has_broadcast() const;

            /*!
             * Setter for the extra capabilities attribute.
             *
             * @param [in] value    bitmask value for the extra capabilities attribute.
             */
            void capabilities(uint8_t value);

            /*!
             * Getter for the extra capabilities attribute bitmask.
             *
             * @return  the bitmask value of the extra capabilities attribute.
             */
            uint8_t capabilities();

#if HF_CORE_DEV_INFO_APP_VERSION_ATTR
            /*!
             * Getter for the @c APP_VERSION_ATTR attribute.
             *
             * @return  the application version string.
             */
            const std::string application_version() const
            {
               return std::string(APPLICATION_VERSION);
            }
#endif
#if HF_CORE_DEV_INFO_HW_VERSION_ATTR
            /*!
             * Getter for the @c HW_VERSION_ATTR attribute.
             *
             * @return  the hardware version string.
             */
            virtual const std::string hardware_version() const
            {
               return std::string(HARDWARE_VERSION);
            }
#endif
#if HF_CORE_DEV_INFO_MANUFACTURER_NAME_ATTR
            /*!
             * Getter for the @c MANUFACTURE_NAME_ATTR attribute.
             *
             * @return  the manufacturer name string.
             */
            const std::string manufacturer_name() const
            {
               return std::string(MANUFACTURER_NAME);
            }
#endif
            protected:

            uint8_t _capabilities;
         };

         /*!
          * Create a message that can be used to retrieve the mandatory attributes
          * on a remote device.
          *
          * @return    pointer to a message to retrieve the mandatory attributes.
          */
         Protocol::Message *mandatory();

         /*!
          * Create a message that can be used to retrieve all the attributes
          * on a remote device.
          *
          * @return    pointer to a message to retrieve all the attributes.
          */
         Protocol::Message *all();

         /*!
          * Create a message that can be used to retrieve the attributes with the given @c uids of the
          * device information interface on a remote device.
          *
          * @param [in] uids array containing the attribute uids to retrive from the remote device.
          *
          * @return    pointer to a message to retrieve the attributes with the given uid's.
          */
         Protocol::Message *get(HF::Attributes::UIDS &uids);

         /*!
          * Create a message that can be used to retrieve the attribute with the given @c uid.
          *
          * @param [in] uid  attribute's uid to retrieve.
          *
          * @return    pointer to a message to retrieve the attribute with the given uid.
          */
         Protocol::Message *get(uint8_t uid);

         /*! @} */

      }  // namespace DeviceInformation

   }  // namespace Core

}  // namespace HF

/*!
 * @addtogroup dev_info
 * @{
 */

// =============================================================================
// Stream Helpers
// =============================================================================

/*!
 * Convert the given @c attribute into a string and write it to the given @c stream.
 *
 * @param [in] stream      out stream to write the string to.
 * @param [in] attribute   attribute value to convert to a string.
 *
 * @return   <tt>stream</tt>
 */
std::ostream &operator<<(std::ostream &stream,
                         const HF::Core::DeviceInformation::Attributes attribute);

/*! @} */

#endif /* HF_DEVICE_INFORMATION_H */
