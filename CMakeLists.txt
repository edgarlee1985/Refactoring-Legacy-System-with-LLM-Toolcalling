cmake_minimum_required(VERSION 3.5) # CMake install : https://cmake.org/download/
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


set(srcs
    ./src/main.cpp
    ./src/QtLegacyProject.cpp
    ./src/GlobalData.cpp
    ./src/DeviceDialog.cpp
    ./src/UserDialog.cpp
    ./src/OperationDialog.cpp
    ./src/DeviceDialogController.cpp
)

set(headers
    ./src/QtLegacyProject.h
    ./src/GlobalData.h
    ./src/DeviceDialog.h
    ./src/UserDialog.h
    ./src/OperationDialog.h
    ./src/DeviceDialogController.h
)

# 【新增】明確列出 UI 檔
set(uis
    ./src/QtLegacyProject.ui
)

# Specify MSVC UTF-8 encoding
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(${PROJECT_NAME}
    WIN32 # If you need a terminal for debug, please comment this statement
    ${srcs}      # 加入所有 .cpp
    ${headers}   # 加入所有 .h
    ${uis}       # 加入所有 .ui
) 

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)