project(TestProject)
message(STATUS "Running resource packer...")
execute_process(COMMAND "${ResourcePackerDir}/GamedevResourcePacker${CMAKE_EXECUTABLE_SUFFIX}"
        "${CMAKE_SOURCE_DIR}/Config" "${CMAKE_SOURCE_DIR}/Assets"
        "${CMAKE_BINARY_DIR}/bin/Assets" "${CMAKE_BINARY_DIR}/Generated/ResourceSubsystem"
        WORKING_DIRECTORY "${ResourcePackerDir}"
        RESULT_VARIABLE RPResult
        OUTPUT_FILE "${CMAKE_BINARY_DIR}/ResourcePacker.log")

if (RPResult)
    message(FATAL_ERROR "Resource packer failed with code ${RPResult}. See ResourcePacker.log.")
else ()
    message(STATUS "Resource packer executed!")
endif ()

include_directories(${CMAKE_SOURCE_DIR}
        "${CMAKE_BINARY_DIR}/Generated"
        "${CMAKE_BINARY_DIR}/Generated/ResourceSubsystem")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_subdirectory(${CMAKE_BINARY_DIR}/Generated/ResourceSubsystem)
add_subdirectory(Game)
