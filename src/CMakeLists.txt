# =============================================================================
#
#   @file src/CMakeLists.txt
#
#   HAN-FUN Library CMakeList.
#
#   @version 1.5.3
#
#   @copyright  Copyright (c) 2014  ULE Alliance
#
#   For licensing information, please see the file 'LICENSE' in the root folder.
#
#   Initial development by Bithium S.A. [http://www.bithium.com]
#
# =============================================================================

# ==============================================================================
# Configuration
# ==============================================================================

# ==============================================================================
# Build
# ==============================================================================

## Common ##
set(HF_COMMON_SRCS common.cpp attributes.cpp devices.cpp profiles.cpp uids.cpp debug.cpp)

if(HF_BUILD_TESTS)
   list(APPEND HF_COMMON_SRCS ${PROJECT_SOURCE_DIR}/tests/helpers/assert.cpp)
endif()

## Protocol ##
set(HF_PROTOCOL_SRCS protocol.cpp)

## Interfaces ##
set(HF_INTERFACES_SRCS interfaces.cpp)

macro(add_interface __name)
   if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/interfaces/${__name}.cpp")
     list(APPEND HF_CORE_SRCS "interfaces/${__name}.cpp")
   endif ()
   if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/interfaces/${__name}_server.cpp")
     list(APPEND HF_CORE_SRCS "interfaces/${__name}_server.cpp")
   endif ()
   if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/interfaces/${__name}_client.cpp")
     list(APPEND HF_CORE_SRCS "interfaces/${__name}_client.cpp")
   endif ()
endmacro()

add_interface("alert")
add_interface("on_off")
add_interface("level_control")
add_interface("simple_power_meter")
add_interface("simple_temperature")
add_interface("simple_humidity")
add_interface("simple_thermostat")
add_interface("simple_button")
add_interface("simple_visual_effects")
add_interface("simple_air_pressure")
add_interface("simple_light_sensor")
add_interface("colour_control")
add_interface("simple_keypad")

## Core Services & Interfaces
macro(add_core __name)
   if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/core/${__name}.cpp")
     list(APPEND HF_CORE_SRCS "core/${__name}.cpp")
   endif ()
   if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/core/${__name}_server.cpp")
     list(APPEND HF_CORE_SRCS "core/${__name}_server.cpp")
   endif ()
   if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/core/${__name}_client.cpp")
     list(APPEND HF_CORE_SRCS "core/${__name}_client.cpp")
   endif ()
endmacro()

add_core("device_management")
add_core("bind_management")
add_core("device_information")
add_core("attribute_reporting")
add_core("group_table")
add_core("session_management")
add_core("rssi")

if(${HF_GROUP_SUPPORT})
   add_core("group_management")
endif()

if(${HF_CORE_SUOTA_SUPPORT})
   add_core("suota")
endif()

if(${HF_TIME_SUPPORT})
   add_core("time")
endif()

if(${HF_BATCH_PROGRAM_SUPPORT})
   add_core("batch_program_management")
endif()

if(${HF_EVENT_SCHEDULING_SUPPORT} OR ${HF_WEEKLY_SCHEDULING_SUPPORT})
   add_core("scheduling")
endif()

if(${HF_EVENT_SCHEDULING_SUPPORT})
   add_core("event_scheduling")
endif()

if(${HF_WEEKLY_SCHEDULING_SUPPORT})
   add_core("weekly_scheduling")
endif()

## Units ##
set(HF_UNITS_SRCS units.cpp)

## Library ##
add_library(hanfun  ${HF_COMMON_SRCS} ${HF_PROTOCOL_SRCS}
                    ${HF_INTERFACES_SRCS} ${HF_CORE_SRCS}
                    ${HF_UNITS_SRCS})

install(TARGETS hanfun EXPORT ${PACKAGE_NAME}-targets
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}" COMPONENT dev)

set(HF_TARGETS ${HF_TARGETS} hanfun PARENT_SCOPE)
