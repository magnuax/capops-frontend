cmake_minimum_required(VERSION 3.31)

project(app
  VERSION 0.1.0
  LANGUAGES CXX
)

# -----------------------------
# Global settings
# -----------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()


# -----------------------------
# Qt setup
# -----------------------------

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

file(GLOB_RECURSE APP_SOURCES 
    src/*.cpp
    include/*.hpp
    resources/*.qrc
)

# -----------------------------
# Executable & Linking
# ------------------------------

add_executable(app ${APP_SOURCES})
target_link_libraries(app PRIVATE Qt6::Core Qt6::Widgets)
target_include_directories(app PRIVATE include)