cmake_minimum_required(VERSION 3.14)
project(QtLegacyProject LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 如果你的 Qt 路徑不同，請確認這裡的設定
set(CMAKE_PREFIX_PATH "c:/Qt/5.15.2/mingw81_64")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED) # Qt COMPONENTS

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test/vendor/googletest
)
# 避免 GTest 覆蓋 Windows 下的 Runtime Library 設定
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) 
FetchContent_MakeAvailable(googletest)

include_directories("./test/vendor/ApprovalTests")

set(core_srcs
    ./src/main.cpp
    ./src/QtLegacyProject.cpp
    ./src/GlobalData.cpp
    ./src/DeviceDialog.cpp
    ./src/UserDialog.cpp
    ./src/OperationDialog.cpp
    ./src/DeviceDialogController.cpp
    ./src/UserDialogController.cpp
    ./src/OperationDialogController.cpp
)

set(core_headers
    ./src/QtLegacyProject.h
    ./src/GlobalData.h
    ./src/DeviceDialog.h
    ./src/UserDialog.h
    ./src/OperationDialog.h
    ./src/DeviceDialogController.h
    ./src/UserDialogController.h
    ./src/OperationDialogController.h
)

set(uis
    ./src/QtLegacyProject.ui
)

# 測試模式專用檔案
set(test_srcs
    ./test/TestDeviceOperation.cpp
    ./test/OldOperationDialogController.cpp
)

set(test_headers
    ./test/TestDeviceOperation.h
    ./test/OldOperationDialogController.h
)

# Approval Test 專用檔案
set(approval_test_srcs
    ./test/ApprovalTestDeviceOperation.cpp
)

# MCP 專用檔案
set(mcp_srcs
    ./MCPToolStrategy/AddDeviceStrategy.cpp
    ./MCPToolStrategy/AddUserInfoStrategy.cpp
    ./MCPToolStrategy/ListUsersStrategy.cpp
    ./MCPToolStrategy/DeviceComputeStrategy.cpp
    ./MCPToolStrategy/TestDeviceOperationStrategy.cpp
)

set(mcp_headers
    ./MCPToolStrategy/McpToolStrategy.h
    ./MCPToolStrategy/AddDeviceStrategy.h
    ./MCPToolStrategy/AddUserInfoStrategy.h
    ./MCPToolStrategy/ListUsersStrategy.h
    ./MCPToolStrategy/DeviceComputeStrategy.h
    ./MCPToolStrategy/TestDeviceOperationStrategy.h
)

# Specify MSVC UTF-8 encoding
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# 產品模式 (保留 WIN32，編譯為純 GUI)
add_executable(${PROJECT_NAME} WIN32
    ${core_srcs}
    ${core_headers}
    ${uis}
)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)

# 測試模式 (移除 WIN32，編譯為 Console 應用程式)
add_executable(${PROJECT_NAME}_Test
    ${core_srcs}
    ${test_srcs} 
    ${core_headers}
    ${test_headers} 
)
target_link_libraries(${PROJECT_NAME}_Test PRIVATE Qt5::Widgets)
# 關鍵：為測試版注入編譯巨集
target_compile_definitions(${PROJECT_NAME}_Test PRIVATE TEST_MODE)

# ==========================================
# Google Test + Approval 測試模式
# ==========================================
add_executable(${PROJECT_NAME}_Approval_Test
    ${core_srcs}
    ${test_srcs}
    ${approval_test_srcs}
    ${core_headers}
    ${test_headers}
)
# 連結 Qt、GTest 與 ApprovalTests
target_link_libraries(${PROJECT_NAME}_Approval_Test 
    PRIVATE 
    Qt5::Widgets
    gtest
)
# 注入全新的巨集
target_compile_definitions(${PROJECT_NAME}_Approval_Test PRIVATE APPROVAL_TEST_MODE)

# ==========================================
# LLM MCP 模式
# ==========================================
add_executable(${PROJECT_NAME}_MCP
    ${core_srcs}
    ${mcp_srcs}
    ${core_headers} 
    ${mcp_headers}
    ${test_srcs}
)
# 連結 Qt5::Widgets
target_link_libraries(${PROJECT_NAME}_MCP PRIVATE Qt5::Widgets)
# 關鍵：為 MCP 測試版注入編譯巨集
target_compile_definitions(${PROJECT_NAME}_MCP PRIVATE LLM_MCP_MODE)