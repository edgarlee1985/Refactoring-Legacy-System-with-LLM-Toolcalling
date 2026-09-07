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

set(srcs
    ./src/main.cpp
    ./src/QtLegacyProject.cpp
    ./src/GlobalData.cpp
    ./src/DeviceDialog.cpp
    ./src/UserDialog.cpp
    ./src/OperationDialog.cpp
    ./src/DeviceDialogController.cpp
    ./src/UserDialogController.cpp
    ./src/OperationDialogController.cpp
    ./test/TestDeviceOperation.cpp
    ./test/OldOperationDialogController.cpp
    ./test/ApprovalTestDeviceOperation.cpp
)

set(headers
    ./src/QtLegacyProject.h
    ./src/GlobalData.h
    ./src/DeviceDialog.h
    ./src/UserDialog.h
    ./src/OperationDialog.h
    ./src/DeviceDialogController.h
    ./src/UserDialogController.h
    ./src/OperationDialogController.h
    ./test/TestDeviceOperation.h
    ./test/OldOperationDialogController.h
)

# 【新增】明確列出 UI 檔
set(uis
    ./src/QtLegacyProject.ui
)

# Specify MSVC UTF-8 encoding
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# 產品模式 (保留 WIN32，編譯為純 GUI)
add_executable(${PROJECT_NAME} WIN32
    ${srcs} ${headers} ${uis}
) 
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)

# 測試模式 (移除 WIN32，編譯為 Console 應用程式)
add_executable(${PROJECT_NAME}_Test
    ${srcs} ${headers} ${uis}
) 
target_link_libraries(${PROJECT_NAME}_Test PRIVATE Qt5::Widgets)
# 關鍵：為測試版注入編譯巨集
target_compile_definitions(${PROJECT_NAME}_Test PRIVATE TEST_MODE)

# ==========================================
# Google Test + Approval 測試模式
# ==========================================
add_executable(${PROJECT_NAME}_Approval_Test
    ${srcs} ${headers} ${uis}
) 
# 連結 Qt、GTest 與 ApprovalTests
target_link_libraries(${PROJECT_NAME}_Approval_Test 
    PRIVATE 
    Qt5::Widgets
    gtest
)
# 注入全新的巨集
target_compile_definitions(${PROJECT_NAME}_Approval_Test PRIVATE APPROVAL_TEST_MODE)