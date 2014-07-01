// =============================================================================
/*!
 * \file       apps/node_app.cpp
 *
 * This file contains the implementation of the example application for a
 * HAN-FUN Node.
 *
 * \author     Filipe Alves <filipe.alves@bithium.com>
 *
 * \version    0.3.0
 *
 * \copyright  Copyright &copy; &nbsp; 2014 Bithium S.A.
 */
// =============================================================================
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

#include <unistd.h>

#include <forward_list>
#include <algorithm>

#include "node.h"

#include "common.h"

#include "application.h"

// =============================================================================
// Global Variables.
// =============================================================================

static Node node;

static SimpleLight *simple_light   = nullptr;

static SimpleSwitch *simple_switch = nullptr;

// =============================================================================
// Commands
// =============================================================================

COMMAND (Register, "r", "r:register device")
{
   UNUSED (args);
   LOG (TRACE) << __PRETTY_FUNCTION__ << NL;

   node.unit0.device_management ()->register_device ();
}

COMMAND (Address, "a", "a:device address")
{
   UNUSED (args);
   LOG (TRACE) << __PRETTY_FUNCTION__ << NL;

   std::cout << "Device Address : " << node.address () << std::endl;
}

COMMAND (SimpleLight, "sl", "sl:set device as a simple light")
{
   UNUSED (args);
   LOG (TRACE) << __PRETTY_FUNCTION__ << NL;

   if (simple_switch != nullptr)
   {
      delete simple_switch;
   }

   if (simple_light == nullptr)
   {
      simple_light = new SimpleLight (1, node);
   }
}

COMMAND (SimpleSwitch, "ss", "sl:set device as a simple switch")
{
   UNUSED (args);
   LOG (TRACE) << __PRETTY_FUNCTION__ << NL;

   if (simple_light != nullptr)
   {
      delete simple_light;
   }

   if (simple_switch == nullptr)
   {
      simple_switch = new SimpleSwitch (1, node);
   }
}

COMMAND (On, "on", "on:On Command")
{
   UNUSED (args);
   LOG (TRACE) << __PRETTY_FUNCTION__ << NL;

   if (simple_switch != nullptr)
   {
      simple_switch->on ();
   }
   else
   {
      LOG (ERROR) << "Simple Switch not configured !" << NL;
   }
}

COMMAND (Off, "off", "off:Off Command")
{
   UNUSED (args);
   LOG (TRACE) << __PRETTY_FUNCTION__ << NL;

   if (simple_switch != nullptr)
   {
      simple_switch->off ();
   }
   else
   {
      LOG (ERROR) << "Simple Switch not configured !" << NL;
   }
}

COMMAND (Toggle, "toggle", "toggle:Toggle Command")
{
   UNUSED (args);
   LOG (TRACE) << __PRETTY_FUNCTION__ << NL;

   if (simple_switch != nullptr)
   {
      simple_switch->toggle ();
   }
   else
   {
      LOG (ERROR) << "Simple Switch not configured !" << NL;
   }
}

// =============================================================================
// API
// =============================================================================

// =============================================================================
// HF::Application::Initialize
// =============================================================================
/*!
 *
 */
// =============================================================================
void HF::Application::Initialize (HF::Transport::Layer &transport)
{
   LOG (TRACE) << __PRETTY_FUNCTION__ << NL;

   transport.initialize ();

   transport.add (&node);
}
