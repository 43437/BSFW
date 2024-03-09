cmake_minimum_required(VERSION 3.1.2)

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
	include(vsBuild.cmake)
else()
	include(linuxBuild.cmake)
endif()