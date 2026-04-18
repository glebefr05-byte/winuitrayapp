cmake_minimum_required(VERSION 3.10)
project(ServiceRpc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_program(MIDL_COMPILER midl)
if(NOT MIDL_COMPILER)
    message(FATAL_ERROR "MIDL compiler not found. Please install Windows SDK.")
endif()

set(GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GENERATED_DIR})

add_custom_command(
    OUTPUT 
        ${GENERATED_DIR}/ServiceInterface_h.h
        ${GENERATED_DIR}/ServiceInterface_i.c
        ${GENERATED_DIR}/ServiceInterface_c.c
        ${GENERATED_DIR}/ServiceInterface_s.c
    COMMAND ${MIDL_COMPILER}
        /out ${GENERATED_DIR}
        /header ServiceInterface_h.h
        /iid ServiceInterface_i.c
        /client stub
        /server stub
        ${CMAKE_CURRENT_SOURCE_DIR}/ServiceInterface.idl
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/ServiceInterface.idl
    COMMENT "Generating RPC stub files"
)

add_custom_target(GenerateRpcFiles DEPENDS 
    ${GENERATED_DIR}/ServiceInterface_h.h
    ${GENERATED_DIR}/ServiceInterface_i.c
    ${GENERATED_DIR}/ServiceInterface_c.c
    ${GENERATED_DIR}/ServiceInterface_s.c
)

add_executable(RpcService
    RpcServer.cpp
    ${GENERATED_DIR}/ServiceInterface_s.c
    ${GENERATED_DIR}/ServiceInterface_c.c
    ${GENERATED_DIR}/ServiceInterface_i.c
)

target_include_directories(RpcService PRIVATE ${GENERATED_DIR})
target_link_libraries(RpcService rpcrt4)
add_dependencies(RpcService GenerateRpcFiles)

add_library(RpcClient STATIC
    ${GENERATED_DIR}/ServiceInterface_c.c
    ${GENERATED_DIR}/ServiceInterface_i.c
)

target_include_directories(RpcClient PUBLIC ${GENERATED_DIR})
target_link_libraries(RpcClient rpcrt4)
add_dependencies(RpcClient GenerateRpcFiles)

add_executable(TrayApp WIN32
    TrayApp.cpp
    MainWindow.cpp
    TrayIcon.cpp
)

target_link_libraries(TrayApp RpcClient user32 shell32 comctl32)
target_include_directories(TrayApp PRIVATE ${GENERATED_DIR})