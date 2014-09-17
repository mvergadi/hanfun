// =============================================================================
/*!
 * \file       inc/hanfun/core/device_information.h
 *
 * This file contains the definition for the core Device Information interface
 * of the HAN-FUN protocol.
 *
 * \version    1.0.0
 *
 * \copyright  Copyright &copy; &nbsp; 2014 ULE Alliance
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

#ifndef HF_DEVICE_SW_VERSION
//! Device Application Version
   #define HF_DEVICE_SW_VERSION          "0.0.0"
#endif

#ifndef HF_DEVICE_MANUFACTURER_CODE
//! Device Electronic Manufacturer Code
   #define HF_DEVICE_MANUFACTURER_CODE   0x0000
#endif

#ifndef HF_DEVICE_MANUFACTURER_NAME
//! Device Manufacturer Name
   #define HF_DEVICE_MANUFACTURER_NAME   "None"
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

      HF::Attributes::IAttribute *create_attribute (DeviceInformation::Server *server, uint8_t uid);

      namespace DeviceInformation
      {
         constexpr static uint8_t  CORE_VERSION      = HF::CORE_VERSION;       //!< HAN-FUN Core version.
         constexpr static uint8_t  PROFILE_VERSION   = HF::PROFILES_VERSION;   //!< HAN-FUN Profile version.
         constexpr static uint8_t  INTERFACE_VERSION = HF::INTERFACES_VERSION; //!< HAN-FUN Interface version.

         constexpr static uint16_t EMC               = HF_DEVICE_MANUFACTURER_CODE; //!< Electronic Manufacture Code.

         static const std::string  SW_VERSION;         //!< Application Version.
         static const std::string  MANUFACTURER;       //!< Manufacturer Name.

         //! Attributes.
         typedef enum
         {
            CORE_VERSION_ATTR      = 0x01,   //!< HF Core version attribute.              (M)
            PROFILE_VERSION_ATTR   = 0x02,   //!< HF Profile version attribute.           (M)
            INTERFACE_VERSION_ATTR = 0x03,   //!< HF Interface version attribute.         (M)
            UID_ATTR               = 0x04,   //!< Device UID attribute.                   (M)
            APP_VERSION_ATTR       = 0x05,   //!< Hardware version attribute.             (O)
            HW_VERSION_ATTR        = 0x06,   //!< Hardware version attribute.             (O)
            EMC_ATTR               = 0x07,   //!< Electronic Manufacture Code attribute.  (O)
            EXTRA_CAP_ATTR         = 0x08,   //!< Extra capabilities attribute.           (O)
            MIN_SLEEP_TIME_ATTR    = 0x09,   //!< Minimum sleep time attribute.           (O)
            ACTUAL_RESP_TIME_ATTR  = 0x0A,   //!< Actual response time attribute.         (O)
            MANUFACTURE_NAME_ATTR  = 0x0B,   //!< Manufacture's name attribute.           (O)
            SERIAL_NUMBER_ATTR     = 0x0C,   //!< Serial number attribute.                (O)
            LOCATION_ATTR          = 0x0D,   //!< Location attribute.                     (O)
            ENABLED_ATTR           = 0x0E,   //!< Device enabled attribute.               (O)
            FRIENDLY_NAME_ATTR     = 0x0F,   //!< Device friendly name attribute.         (O)
         } Attributes;

         HF::Attributes::IAttribute *create_attribute (uint8_t uid);

         /*!
          * Parent class for the Device Information interface implementation.
          */
         struct Abstract:public Service <HF::Interface::DEVICE_INFORMATION>
         {
            // =============================================================================
            // API
            // =============================================================================


            // =============================================================================

            protected:

            Abstract(Unit0 &unit):
               Service (unit)
            {}
         };

         /*!
          * Device Information interface : Server side.
          */
         struct Server:public ServiceRole <Abstract, HF::Interface::SERVER_ROLE>
         {
            HF::UID::UID device_uid;

            Server(HF::Core::Unit0 &unit):
               ServiceRole <Abstract, HF::Interface::SERVER_ROLE>(unit)
            {}

            virtual ~Server() {}

            // =============================================================================
            // Interface Attribute API.
            // =============================================================================

            //! \see Interface::attribute
            HF::Attributes::IAttribute *attribute (uint8_t uid)
            {
               return Core::create_attribute (this, uid);
            }

            protected:

            //! \see AbstractInterface::attributes
            HF::Attributes::UIDS attributes (uint8_t pack_id = HF::Attributes::Pack::MANDATORY) const;
         };

         /*!
          * Create a message that can be used to retrieve the mandatory attributes
          * on a remote device.
          *
          * @return    pointer to a message to retrieve the mandatory attributes.
          */
         Protocol::Message *mandatory ();

         /*!
          * Create a message that can be used to retrieve all the attributes
          * on a remote device.
          *
          * @return    pointer to a message to retrieve all the attributes.
          */
         Protocol::Message *all ();

         /*!
          * Create a message that can be used to retrieve the attributes with the given \c uids of the
          * device information interface on a remote device.
          *
          * @param [in] uids array containing the attribute uids to retrive from the remote device.
          *
          * @return    pointer to a message to retrieve the attributes with the given uid's.
          */
         Protocol::Message *get (HF::Attributes::UIDS &uids);

         /*!
          * Create a message that can be used to retrieve the attribute with the given \c uid.
          *
          * @param [in] uid  attribute's uid to retrieve.
          *
          * @return    pointer to a message to retrieve the attribute with the given uid.
          */
         Protocol::Message *get (uint8_t uid);

      }  // namespace DeviceInformation

   }  // namespace Core

}  // namespace HF

#endif /* HF_DEVICE_INFORMATION_H */
