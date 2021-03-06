# =============================================================================
#
#   @file apps/CMakeLists.txt
#
#   HAN-FUN Library Examples CMakeList
#
#   @version    1.5.3
#
#   @copyright  Copyright (c) 2014  ULE Alliance
#
#   For licensing information, please see the file 'LICENSE' in the root folder.
#
#   Initial development by Bithium S.A. [http://www.bithium.com]
#
# =============================================================================

# =============================================================================
# Libuv
# =============================================================================

if (HF_BASE_APP OR HF_NODE_APP)
    include(ExternalProject)

    set (LIBUV_PREFIX      "${CMAKE_CURRENT_BINARY_DIR}/libuv")
    set (LIBUV_INSTALL_DIR "${LIBUV_PREFIX}/src/libuv")

    find_program(MAKE_BIN "make")

    ExternalProject_Add(libuv
        PREFIX ${LIBUV_PREFIX}
        URL "http://dist.libuv.org/dist/v0.10.27/libuv-v0.10.27.tar.gz"
        URL_HASH MD5=8bc14dc254363092bd36919ea05cc9ab
        CONFIGURE_COMMAND ""
        BUILD_COMMAND CC=${CMAKE_C_COMPILER} ${MAKE_BIN}
        BUILD_IN_SOURCE 1
        INSTALL_COMMAND ""
    )

    set (LIBUV_LIBRARY "${LIBUV_PREFIX}/src/libuv/libuv.a")
    set (LIBUV_LIBRARIES ${LIBUV_LIBRARY} -lrt -ldl -lm -pthread)

    include_directories( ${LIBUV_INSTALL_DIR}/include )
endif()

if (HF_APP_EXT_REG)
    add_definitions( -DHF_APP_EXT_REG )
endif()

if (HF_BASE_LIB OR HF_NODE_LIB)
    configure_file("application.h" "${CMAKE_BINARY_DIR}/hanfun/application.h" COPYONLY)
endif()

# =============================================================================
# Base Application Example
# =============================================================================

if (HF_BASE_LIB)
    message (STATUS "Build Base example application library.")

    set(BASE_APP_SRCS   "base.h" "common.cpp" "base.cpp" "base_app.cpp" "jsoncpp.cpp")

    add_definitions(-DJSON_USE_EXCEPTION=0)

    add_library(hf_base_app ${BASE_APP_SRCS})
    target_link_libraries(hf_base_app hanfun)

    install(TARGETS hf_base_app EXPORT ${PACKAGE_NAME}-targets
            ARCHIVE DESTINATION "${INSTALL_LIB_DIR}")

    set (HF_TARGETS ${HF_TARGETS} hf_base_app)
endif()

if (HF_BASE_APP)
    message (STATUS "Build Base example application.")

    add_executable(hf_base "main.cpp" "transport.cpp")
    add_dependencies(hf_base libuv)
    set_target_properties(hf_base PROPERTIES COMPILE_DEFINITIONS "HF_BASE_APP")
    target_link_libraries(hf_base hf_base_app hanfun  ${LIBUV_LIBRARIES} )
    install(TARGETS hf_base EXPORT ${PACKAGE_NAME}-targets RUNTIME DESTINATION "${INSTALL_BIN_DIR}")
endif()

# =============================================================================
# Node Applications Example
# =============================================================================

if (HF_NODE_LIB)
    message (STATUS "Build Node example application library.")

    set(NODE_APP_SRCS   "node.h" "common.cpp" "node.cpp" "node_app.cpp")

    add_library( hf_node_app ${NODE_APP_SRCS})
    target_link_libraries(hf_node_app hanfun)

    install(TARGETS hf_node_app EXPORT ${PACKAGE_NAME}-targets
            ARCHIVE DESTINATION "${INSTALL_LIB_DIR}")

    set (HF_TARGETS ${HF_TARGETS} hf_node_app)
endif()

if (HF_NODE_APP)
    message (STATUS "Build Node example application.")

    add_executable(hf_node "main.cpp" "transport.cpp")
    add_dependencies(hf_node libuv)
    set_target_properties(hf_node PROPERTIES COMPILE_DEFINITIONS "HF_NODE_APP")
    target_link_libraries(hf_node hf_node_app hanfun ${LIBUV_LIBRARIES})

    install(TARGETS hf_node EXPORT ${PACKAGE_NAME}-targets RUNTIME DESTINATION "${INSTALL_BIN_DIR}")
endif()

set (HF_TARGETS ${HF_TARGETS} PARENT_SCOPE)

# ==============================================================================
# Examples
# ==============================================================================

macro(add_example _number)
    set(_example_name  "example_${_number}")
    add_executable(${_example_name} EXCLUDE_FROM_ALL ${_example_name}.cpp)
    target_link_libraries(${_example_name} hanfun)
    list(APPEND HF_EXAMPLES ${_example_name})
endmacro()

## Registration examples
add_example(01)

add_example(02)

## Alert interface example
add_example(03)

## On-Off interface example
add_example(04)

## Level Control interface example
add_example(05)

## Device Information service example
add_example(06)

## Attribute reporting service example
add_example(07)

## Bind service example
add_example(08)

## Build all examples
add_custom_target(examples DEPENDS ${HF_EXAMPLES})

set(HF_TESTS ${HF_TESTS} ${HF_EXAMPLES} PARENT_SCOPE)
