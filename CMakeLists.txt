# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and / or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The below copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#
# Vulkan Cookbook
# ISBN: 9781786468154
# © Packt Publishing Limited
#
# Author:   Pawel Lapinski
# LinkedIn: https://www.linkedin.com/in/pawel-lapinski-84522329
#
# CMakeLists

cmake_minimum_required (VERSION 3.0)
project( "VulkanCookbook" )

set_property( GLOBAL PROPERTY USE_FOLDERS ON )
set( CMAKE_SUPPRESS_REGENERATION true )

###############################################################
# Environment setup                                           #
###############################################################

if( CMAKE_BUILD_TYPE STREQUAL "" )
	set( CMAKE_BUILD_TYPE "debug" )
endif()

if( NOT DEFINED VK_USE_PLATFORM )
	if( WIN32 )
		set( VK_USE_PLATFORM "WIN32" )
	elseif( UNIX )
		set( VK_USE_PLATFORM "XCB" )
	endif()
endif()

if( VK_USE_PLATFORM STREQUAL "VK_USE_PLATFORM_WIN32_KHR" )
	set( CMAKE_C_FLAGS_RELWITHDEBINFO    "${CMAKE_C_FLAGS_RELWITHDEBINFO} /MT" )
	set( CMAKE_C_FLAGS_RELEASE           "${CMAKE_C_FLAGS_RELEASE} /MT" )
	set( CMAKE_C_FLAGS_DEBUG             "${CMAKE_C_FLAGS_DEBUG} /MTd" )
	set( CMAKE_CXX_FLAGS_RELWITHDEBINFO  "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT" )
	set( CMAKE_CXX_FLAGS_RELEASE         "${CMAKE_CXX_FLAGS_RELEASE} /MT" )
	set( CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_CXX_FLAGS_DEBUG} /MTd" )
	set( PLATFORM_LIBRARY                "" )
elseif( VK_USE_PLATFORM STREQUAL "XCB" )
	add_definitions( -std=c++11 )
	set( PLATFORM_LIBRARY dl xcb )
elseif( VK_USE_PLATFORM STREQUAL "XLIB" )
	add_definitions( -std=c++11 )
	set( PLATFORM_LIBRARY dl X11 )
endif()

if( CMAKE_BUILD_TYPE STREQUAL "debug" )
	add_definitions(-D_DEBUG)
endif()

add_definitions( -DVK_NO_PROTOTYPES )
add_definitions( -DVK_USE_PLATFORM_${VK_USE_PLATFORM}_KHR )

###############################################################
# Cookbook Library                                            #
###############################################################

# External files
file( GLOB EXTERNAL_HEADER_FILES "External/*.h" )
source_group( "External" FILES ${EXTERNAL_HEADER_FILES} )

# Common files
file( GLOB LIBRARY_COMMON_HEADER_FILES "Library/Common Files/*.h" "Library/Common Files/*.inl" )
file( GLOB LIBRARY_COMMON_SOURCE_FILES "Library/Common Files/*.cpp" )
source_group( "Common Files\\Header Files" FILES ${LIBRARY_COMMON_HEADER_FILES} )
source_group( "Common Files" FILES ${LIBRARY_COMMON_SOURCE_FILES} )

# Recipes Library files
macro( list_recipes_files FILE_TYPE SUB_GROUP OUT_VARIABLE )
	file( GLOB CHAPTERS RELATIVE "${CMAKE_CURRENT_LIST_DIR}/Library/Source Files" "${CMAKE_CURRENT_LIST_DIR}/Library/Source Files/*" )
	FOREACH( CHAPTER ${CHAPTERS} )
		file( GLOB CHAPTER_FILES "Library/Source Files/${CHAPTER}/${FILE_TYPE}" )
		source_group( "Source Files\\${CHAPTER}\\${SUB_GROUP}" FILES ${CHAPTER_FILES} )
		list( APPEND ${OUT_VARIABLE} ${CHAPTER_FILES} )
	ENDFOREACH()
endmacro( list_recipes_files )
list_recipes_files( "*.h" "Header Files" LIBRARY_HEADER_FILES )
list_recipes_files( "*.cpp" "" LIBRARY_SOURCE_FILES )

# Recipes Library generation
add_library( CookbookLibrary ${EXTERNAL_HEADER_FILES} ${LIBRARY_COMMON_HEADER_FILES} ${LIBRARY_COMMON_SOURCE_FILES} ${LIBRARY_HEADER_FILES} ${LIBRARY_SOURCE_FILES} )
target_link_libraries( CookbookLibrary ${PLATFORM_LIBRARY} )
target_include_directories( CookbookLibrary PUBLIC "External" "Library/Common Files" "Library/Source Files" )

###############################################################
# Sample projects                                             #
###############################################################

# OS files
file( GLOB SAMPLES_COMMON_HEADER_FILES "Samples/Common Files/*.h" )
file( GLOB SAMPLES_COMMON_SOURCE_FILES "Samples/Common Files/*.cpp" )
source_group( "Common Files\\Header Files" FILES ${SAMPLES_COMMON_HEADER_FILES} ${LIBRARY_COMMON_HEADER_FILES} )
source_group( "Common Files" FILES ${SAMPLES_COMMON_SOURCE_FILES} )

# Sample projects generation
macro( list_samples )
	file( GLOB CHAPTERS RELATIVE "${CMAKE_CURRENT_LIST_DIR}/Samples/Source Files" "${CMAKE_CURRENT_LIST_DIR}/Samples/Source Files/*" )
	FOREACH( CHAPTER ${CHAPTERS} )
		file( GLOB SAMPLES RELATIVE "${CMAKE_CURRENT_LIST_DIR}/Samples/Source Files/${CHAPTER}" "${CMAKE_CURRENT_LIST_DIR}/Samples/Source Files/${CHAPTER}/*" )

		FOREACH( SAMPLE ${SAMPLES} )
			file( GLOB SAMPLE_SOURCE_FILES "Samples/Source Files/${CHAPTER}/${SAMPLE}/*.cpp" )
			source_group( "" FILES ${SAMPLE_SOURCE_FILES} )

			add_executable( ${SAMPLE} ${EXTERNAL_HEADER_FILES} ${LIBRARY_COMMON_HEADER_FILES} ${SAMPLES_COMMON_HEADER_FILES} ${SAMPLES_COMMON_SOURCE_FILES} ${SAMPLE_SOURCE_FILES} )
			target_link_libraries( ${SAMPLE} ${PLATFORM_LIBRARY} CookbookLibrary )
			target_include_directories( ${SAMPLE} PUBLIC "External" "Library/Common Files" "Library/Source Files" "Samples/Common Files" )
			set_property( TARGET ${SAMPLE} PROPERTY FOLDER "Samples/${CHAPTER}" )
		ENDFOREACH()
	ENDFOREACH()
endmacro()

# Sample projects generation
list_samples()

file( COPY "${CMAKE_CURRENT_LIST_DIR}/Samples/Data" DESTINATION "${CMAKE_CURRENT_LIST_DIR}/build" )
