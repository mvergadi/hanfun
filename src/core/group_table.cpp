// =============================================================================
/*!
 * @file       src/core/group_table.cpp
 *
 * This file contains the implementation of the common functionality for the
 * Group Table service.
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

#include "hanfun/core/group_table.h"

// =============================================================================
// API
// =============================================================================

using namespace HF;
using namespace HF::Core;
using namespace HF::Core::GroupTable;

// =============================================================================
// Core::create_attribute
// =============================================================================
/*!
 *
 */
// =============================================================================
HF::Attributes::IAttribute *Core::create_attribute(Server *server, uint8_t uid)
{
   if (server != nullptr)
   {
      return server->attribute(uid);
   }
   else
   {
      return GroupTable::create_attribute(uid);
   }
}

// =============================================================================
// GroupTable::create_attribute
// =============================================================================
/*!
 *
 */
// =============================================================================
HF::Attributes::IAttribute *GroupTable::create_attribute(uint8_t uid)
{
   using namespace HF::Core::GroupTable;

   GroupTable::Attributes attr = static_cast<GroupTable::Attributes>(uid);

   switch (attr)
   {
      case NUMBER_OF_ENTRIES_ATTR:
      {
         return new HF::Attributes::Attribute<uint8_t>(HF::Interface::GROUP_TABLE, attr,
                                                       NumberOfEntries::WRITABLE);
      }

      case NUMBER_OF_MAX_ENTRIES_ATTR:
      {
         return new HF::Attributes::Attribute<uint8_t>(HF::Interface::GROUP_TABLE, attr,
                                                       NumberOfMaxEntries::WRITABLE);
      }

      default:
         return nullptr;
   }
}

// =============================================================================
// GroupTable::Entry
// =============================================================================

// =============================================================================
// GroupTable::Entry::size
// =============================================================================
/*!
 *
 */
// =============================================================================
uint16_t Entry::size() const
{
   return min_size;
}

// =============================================================================
// GroupTable::Entry::pack
// =============================================================================
/*!
 *
 */
// =============================================================================
uint16_t Entry::pack(Common::ByteArray &array, uint16_t offset) const
{
   HF_SERIALIZABLE_CHECK(array, offset, min_size);

   offset += array.write(offset, group);
   offset += array.write(offset, unit);

   return min_size;
}

// =============================================================================
// GroupTable::Entry::unpack
// =============================================================================
/*!
 *
 */
// =============================================================================
uint16_t Entry::unpack(const Common::ByteArray &array, uint16_t offset)
{
   HF_SERIALIZABLE_CHECK(array, offset, min_size);

   offset += array.read(offset, group);
   offset += array.read(offset, unit);

   return min_size;
}
