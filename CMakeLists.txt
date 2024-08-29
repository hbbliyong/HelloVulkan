cmake_minimum_required(VERSION 3.28 FATAL_ERROR)
project(LittleVulkanEngine LANGUAGES CXX C VERSION 1.0 )
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
include($ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(PROJECT_PLATFORM "Win32")
    set(PROJECT_PLATFORM_X "X86")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(PROJECT_PLATFORM "x64")
    set(PROJECT_PLATFORM_X "X64")
endif()
##配置Vulkan路径
if(NOT DEFINED ENV{VULKAN_SDK})
    message(FATAL_ERROR "not defined environment:VULKAN_SDK")
else()
    list(APPEND CMAKE_PREFIX_PATH $ENV{VULKAN_SDK})
endif()
# list(APPEND CMAKE_PREFIX_PATH "F:\\002workcode\\littleVulkanEngine\\vcpkg_installed\\x64-windows\\share")
 set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

 message("${CMAKE_TOOLCHAIN_FILE}")
# message("$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
##源代码位置

set(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")

add_subdirectory ("source/main")




