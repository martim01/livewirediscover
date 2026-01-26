cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(livewireserver LANGUAGES CXX VERSION 1.1.1)

include(addexternal.cmake)

execute_process(COMMAND ${CMAKE_COMMAND} -DNAMESPACE=${PROJECT_NAME} -DMAJOR=${PROJECT_VERSION_MAJOR} -DMINOR=${PROJECT_VERSION_MINOR} -DPATCH=${PROJECT_VERSION_PATCH} -DOUTPUT=${CMAKE_BINARY_DIR} -P ${CMAKE_SOURCE_DIR}/version.cmake)

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
add_external_library(log ${DIR_LOG} "https://github.com/martim01/log.git" "" FALSE)
add_external_library(asio ${DIR_ASIO} "https://github.com/chriskohlhoff/asio" "" FALSE)


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

target_link_directories(livewireserver PRIVATE ${CMAKE_SOURCE_DIR}/lib)

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
