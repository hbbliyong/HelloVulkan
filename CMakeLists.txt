cmake_minimum_required(VERSION 3.28 FATAL_ERROR)
project(LittleVulkanEngine LANGUAGES CXX C VERSION 1.0 )
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")

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

 message("${CMAKE_TOOLCHAIN_FILE}")

##源代码位置
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../../build/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Bin")
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../../build/RELEASE")
set(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")

add_subdirectory(thirdParty/glfw)
add_subdirectory(thirdParty/glm)
add_subdirectory (source/main)




