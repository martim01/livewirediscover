cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(livewireservertest LANGUAGES CXX VERSION 1.1.1)

if(NOT DEFINED BUILD_LOG)
        SET(BUILD_LOG ON)
endif()

SET(DIR_LOG ${DIR_BASE}/log CACHE STRING "location of pml log")

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

add_executable(livewireservertest "../src/parser.cpp" "main.cpp")

target_include_directories(livewireservertest PUBLIC ${PROJECT_SOURCE_DIR}/../include)
target_include_directories(livewireservertest PUBLIC ${CMAKE_BINARY_DIR}/..include)
target_include_directories(livewireservertest PUBLIC ${DIR_LOG}/include)

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" )

if(CMAKE_BUILD_TYPE MATCHES Release)
   list(APPEND flags "-O3")
   target_compile_definitions(livewireservertest PUBLIC NDEBUG DLL_EXPORTS _MSL_STDINT_H)
else()
   list(APPEND flags "-g")
   target_compile_definitions(livewireservertest PUBLIC DEBUG DLL_EXPORTS _MSL_STDINT_H)
endif()

target_compile_options(livewireservertest PRIVATE ${flags})
target_compile_definitions(livewireservertest PUBLIC ASIO_STANDALONE)

target_link_libraries(livewireservertest optimized pml_log debug pml_logd)
target_compile_options(livewireservertest PRIVATE ${flags})



#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_compile_definitions(livewireservertest PRIVATE __GNU__)
endif()

set_target_properties(livewireservertest PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(livewireservertest PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)

