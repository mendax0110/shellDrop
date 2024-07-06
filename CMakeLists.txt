cmake_minimum_required(VERSION 3.5)

project(shellDrop)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SHELL_SOURCES
    src/shell.cpp
    src/shellDrop.cpp
)

add_executable(shellDrop
    ${SHELL_SOURCES}
)

target_link_libraries(shellDrop ws2_32 wininet)

add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/shellDrop.exe.zip"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:shellDrop>" "${CMAKE_CURRENT_BINARY_DIR}/shellDrop.exe"
    COMMAND ${CMAKE_COMMAND} -E echo "Packing shellDrop.exe"
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/utils/packer.bat "${CMAKE_CURRENT_BINARY_DIR}/shellDrop.exe"
    DEPENDS shellDrop
    COMMENT "Packing shellDrop.exe"
)

add_custom_target(pack ALL
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/shellDrop.exe.zip"
)
