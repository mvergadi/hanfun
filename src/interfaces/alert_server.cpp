// =============================================================================
/*!
 * \file       src/interfaces/alert_server.cpp
 *
 * This file contains the implementation of the Alert interface : Server role.
 *
 * \author     Filipe Alves <filipe.alves@bithium.com>
 *
 * \version    x.x.x
 *
 * \copyright  Copyright &copy; &nbsp; 2013 Bithium S.A.
 */
// =============================================================================

#include "hanfun/interfaces/alert.h"

using namespace HF;
using namespace HF::Protocol;
using namespace HF::Interfaces;

// =============================================================================
// Alert Interface : Server Role
// =============================================================================

//! Constructor
AlertServer::AlertServer()
{
   _state   = 0x0;
   _enabled = UINT32_MAX;
}

//! Destructor
AlertServer::~AlertServer()
{}

// =============================================================================
// AlertServer::state
// =============================================================================
/*!
 *
 */
// =============================================================================
uint32_t AlertServer::state ()
{
   return this->_state;
}

// =============================================================================
// AlertServer::state
// =============================================================================
/*!
 *
 */
// =============================================================================
bool AlertServer::state (uint8_t index, bool state)
{
   if (index >= STATE_SIZE_BITS)
   {
      return false;
   }

   bool res = enabled (index);

   if (res)
   {
      uint32_t mask = 1L << index;

      if (state)
      {
         this->_state |= mask;
      }
      else
      {
         this->_state &= ~mask;
      }
   }

   return res;
}

// =============================================================================
// AlertServer::state
// =============================================================================
/*!
 *
 */
// =============================================================================
bool AlertServer::state (uint8_t index)
{
   if (index >= STATE_SIZE_BITS)
   {
      return false;
   }

   return (this->_state & (1L << index)) != 0;
}

// =============================================================================
// AlertServer::clear
// =============================================================================
/*!
 *
 */
// =============================================================================
void AlertServer::clear ()
{
   this->_state = 0;
}

// =============================================================================
// AlertServer::enable
// =============================================================================
/*!
 *
 */
// =============================================================================
void AlertServer::enable (uint8_t index)
{
   if (index > STATE_SIZE_BITS)
   {
      return;
   }

   this->_enabled |= (1L << index);
}

// =============================================================================
// AlertServer::enableAll
// =============================================================================
/*!
 *
 */
// =============================================================================
void AlertServer::enableAll ()
{
   this->_enabled = UINT32_MAX;
}

// =============================================================================
// AlertServer::enabled
// =============================================================================
/*!
 *
 */
// =============================================================================
uint32_t AlertServer::enabled ()
{
   return this->_enabled;
}

// =============================================================================
// AlertServer::enabled
// =============================================================================
/*!
 *
 */
// =============================================================================
bool AlertServer::enabled (uint8_t index)
{
   if (index > STATE_SIZE_BITS)
   {
      return false;
   }

   return (this->_enabled & (1L << index)) != 0;
}

// =============================================================================
// AlertServer::disable
// =============================================================================
/*!
 *
 */
// =============================================================================
void AlertServer::disable (uint8_t index)
{
   if (index > STATE_SIZE_BITS)
   {
      return;
   }

   this->_enabled &= (~(1L << index));
}

// =============================================================================
// AlertServer::disableAll
// =============================================================================
/*!
 *
 */
// =============================================================================
void AlertServer::disableAll ()
{
   this->_enabled = 0;
}

// =============================================================================
// AlertServer::disabled
// =============================================================================
/*!
 *
 */
// =============================================================================
bool AlertServer::disabled (uint8_t index)
{
   if (index > STATE_SIZE_BITS)
   {
      return true;
   }

   return (this->_enabled & (1L << index)) == 0;
}

// =============================================================================
// AlertServer::status_cmd
// =============================================================================
/*!
 *
 */
// =============================================================================
Alert::Message *AlertServer::status (uint16_t profile_uid)
{
   return new (nothrow) Alert::Message (profile_uid, this->_state);
}

// =============================================================================
// AlertServer::status
// =============================================================================
/*!
 *
 */
// =============================================================================
void AlertServer::status (Protocol::Address &addr, uint16_t profile_uid)
{
   Protocol::Message::Interface itf (CLIENT_ROLE, HF::Interface::ALERT, STATUS_CMD);

   Alert::Message *alert_msg = status (profile_uid);

   Protocol::Message message (Protocol::Message::COMMAND_REQ, itf, alert_msg);

   sendMessage (addr, message);
}

// =============================================================================
// AlertServer::attribute
// =============================================================================
/*!
 *
 */
// =============================================================================
IAttribute *AlertServer::attribute (uint8_t uid)
{
   Attributes attr = static_cast <Attributes>(uid);

   switch (attr)
   {
      case STATE_ATTR:
         return new Attribute<uint32_t &>( this->uid(), attr, _state, State::WRITABBLE );

      case ENABLE_ATTR:
         return new Attribute<uint32_t &>( this->uid(), attr, _enabled, Enable::WRITABBLE );

      default:
         return nullptr;
   }
}
