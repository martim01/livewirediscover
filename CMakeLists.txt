cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(livewireserver LANGUAGES CXX VERSION 1.1.1)

execute_process(COMMAND ${CMAKE_COMMAND} -DNAMESPACE=livewireserver -DMAJOR=${PROJECT_VERSION_MAJOR} -DMINOR=${PROJECT_VERSION_MINOR} -DPATCH=${PROJECT_VERSION_PATCH} -P ${CMAKE_CURRENT_SOURCE_DIR}/version.cmake)

SET(DIR_BASE $ENV{HOME} CACHE STRING "base location for libraries")
SET(DIR_LOG ${DIR_BASE}/log CACHE STRING "location of pml log")
SET(DIR_ASIO ${DIR_BASE}/asio CACHE STRING "location of asio")

if(NOT DEFINED BUILD_LOG)
        SET(BUILD_LOG ON)
endif()

add_library(livewireserver SHARED "src/parser.cpp" "src/receiver.cpp" "src/livewirehandler.cpp" "src/livewireserver.cpp" "src/livewireserverimpl.cpp" ${CMAKE_BINARY_DIR}/src/livewireserver_version.cpp)

target_include_directories(livewireserver PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(livewireserver PUBLIC ${CMAKE_BINARY_DIR}/include)

#Get log
find_path(SP_TEMP_DIR7 NAMES "CMakeLists.txt" PATHS ${DIR_LOG} NO_CACHE)
if((NOT SP_TEMP_DIR7) OR (NOT EXISTS ${SP_TEMP_DIR7}))
	message(STATUS "log not found - clone from GitHub")
	execute_process(COMMAND git clone https://github.com/martim01/log/ ${DIR_LOG})
endif()
unset(SP_TEMP_DIR7)

find_path(SP_TEMP_DIR8 NAMES "CMakeLists.txt" PATHS ${DIR_LOG} NO_CACHE)
if((NOT SP_TEMP_DIR8) OR (NOT EXISTS ${SP_TEMP_DIR8}))
	message(FATAL_ERROR "Failed to clone pml::Log")
endif()
unset(SP_TEMP_DIR8)

if(${BUILD_LOG})
	add_subdirectory(${DIR_LOG} ${CMAKE_SOURCE_DIR}/build/log)
else()
	message(STATUS "Restgoose - pml::log being build elsewhere")
endif()


# ASIO
message(STATUS "Look for asio in ${DIR_ASIO}")
find_path(SP_TEMP_DIR NAMES "asio.hpp" PATHS ${DIR_ASIO}/asio/include NO_CACHE)
if((NOT SP_TEMP_DIR) OR (NOT EXISTS ${SP_TEMP_DIR}))
	message(STATUS "asio not found - clone from GitHub")
	execute_process(COMMAND git clone https://github.com/chriskohlhoff/asio ${DIR_ASIO})
endif()

find_path(SP_TEMP_DIR2 NAMES "asio.hpp" PATHS ${DIR_ASIO}/asio/include NO_CACHE)
if((NOT SP_TEMP_DIR2) OR (NOT EXISTS ${SP_TEMP_DIR2}))
	message(FATAL_ERROR "Failed to clone asio")
else()
	message(STATUS "asio found")
endif()
unset(SP_TEMP_DIR2)

target_include_directories(livewireserver PUBLIC ${PROJECT_SOURCE_DIR}/include)
include_directories(${DIR_LOG}/include)
target_include_directories(livewireserver PUBLIC ${DIR_ASIO}/asio/include)



list(APPEND flags "-fPIC" "-Wall" "-fpermissive"  "-std=c++17")

if(CMAKE_BUILD_TYPE MATCHES Release)
   list(APPEND flags "-O3")
   target_compile_definitions(livewireserver PUBLIC NDEBUG DLL_EXPORTS _MSL_STDINT_H)
else()
   list(APPEND flags "-g")
   target_compile_definitions(livewireserver PUBLIC DEBUG DLL_EXPORTS _MSL_STDINT_H)
endif()

target_compile_options(livewireserver PRIVATE ${flags})
target_compile_definitions(livewireserver PUBLIC ASIO_STANDALONE)

target_link_libraries(livewireserver optimized pml_log debug pml_logd)
target_compile_options(livewireserver PRIVATE ${flags})



#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_compile_definitions(livewireserver PRIVATE __GNU__)
else()
	target_compile_definitions(livewireserver PRIVATE __WIN32__ LIVEWIRE_DLL)
endif()

set_target_properties(livewireserver PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(livewireserver PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/)


#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS livewireserver LIBRARY DESTINATION /usr/local/lib)
endif()
