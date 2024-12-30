cmake_minimum_required(VERSION 3.20)
project(tucuxi LANGUAGES CXX)

### Set output directory
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}")


### Language
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)

# In core.cpp, to set the folder for ephemeraldb. Could be modified in the future
add_definitions(-DTUCU_CMAKE_COMPILE)

### Configuration
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
# Config flags default OFF, If is already set as a normal or cache variable then this command does nothing
option(config_demo OFF)
option(config_guitest OFF)
option(config_externalreport OFF)
option(config_connected OFF)
option(config_rest OFF)
option(config_view_preproc OFF)
option(config_disablepercs OFF)

add_definitions(-DDRUGSPATH=${DRUGSPATH})
add_definitions(-DNOLICENSE=${NOLICENSE})
add_definitions(-DNOSTARTUPSCREEN=${NOSTARTUPSCREEN})

### Qt set configs and find components

if (config_guitest)
    find_package(Qt6 6.5 REQUIRED COMPONENTS Core Core5Compat Xml Network Svg Qml Core5Compat Widgets LinguistTools Gui PrintSupport Quick QuickWidgets PrintSupport Test WebChannel WebSockets WebEngineCore)
set(QT_COMPONENTS Qt::Core
                  Qt::Core5Compat
                  Qt::Network
                  Qt::Qml
                  Qt::Svg
                  Qt::Widgets
                  Qt::Xml
                  Qt::Gui
                  Qt::PrintSupport
                  Qt::Quick
                  Qt::QuickWidgets
                  Qt::PrintSupport
                  Qt::Test
                  Qt::WebChannel
                  Qt::WebSockets
                  Qt::WebEngineCore
)
else()
    find_package(Qt6 6.5 REQUIRED COMPONENTS Core Core5Compat Xml Network Svg Qml Core5Compat Widgets LinguistTools Gui PrintSupport Quick QuickWidgets PrintSupport WebChannel WebSockets WebEngineCore)
    set(QT_COMPONENTS Qt::Core
                      Qt::Core5Compat
                      Qt::Network
                      Qt::Qml
                      Qt::Svg
                      Qt::Widgets
                      Qt::Xml
                      Qt::Gui
                      Qt::PrintSupport
                      Qt::Quick
                      Qt::QuickWidgets
                      Qt::PrintSupport
                      Qt::WebChannel
                      Qt::WebSockets
                      Qt::WebEngineCore
    )
endif()
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

### Create a set of compiler and linker flags
set(TUCUXI_CXXFLAGS ${TUCUXI_CXXFLAGS})
# To solve issues with namespaces
#set(TUCUXI_CXXFLAGS ${TUCUXI_CXXFLAGS} -fpermissive)

if(WIN32)
set(TUCUXI_LFLAGS ${TUCUXI_LFLAGS} -Wl --no-as-needed -ldl)
elseif(UNIX)
set(TUCUXI_LFLAGS ${TUCUXI_LFLAGS} -ldl)
endif(WIN32)

### Add specific CXX or Linker flags
if(WIN32)
    set(TUCUXI_CXXFLAGS ${TUCUXI_CXXFLAGS})
    set(TUCUXI_LFLAGS ${TUCUXI_LFLAGS})
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
elseif(UNIX)
    set(TUCUXI_CXXFLAGS ${TUCUXI_CXXFLAGS} -Wno-reorder -Wall -Wconversion -Wunreachable-code -Wuninitialized
                                           -Wold-style-cast -Wwrite-strings -fexceptions)
    set(TUCUXI_LFLAGS ${TUCUXI_LFLAGS})
endif(WIN32)

### Add defines
# Get git revision
execute_process(
      COMMAND git rev-parse --short HEAD
      RESULT_VARIABLE GIT_RET_REVISION
)
add_compile_definitions(GIT_REVISION=${GIT_RET_REVISION})

# Builds with Tucucore and use it as processing engine
add_compile_definitions(COMPILE_WITH_TUCUCORE)

if(config_nobotan)
    add_compile_definitions(CONFIG_NOBOTAN)
endif(config_nobotan)

if(config_sign)
    add_compile_definitions(CONFIG_SIGN)
endif(config_sign)

if(config_guitest)
    add_compile_definitions(CONFIG_GUITEST)
endif(config_guitest)

if(config_disablepercs)
    add_compile_definitions(CONFIG_DISABLEPERCS)
endif(config_disablepercs)

# Customized project configurations
if(config_demo)
    add_compile_definitions(CONFIG_DEMO)
    # Force configs
    set(config_rest TRUE)
endif(config_demo)

if(NOT config_demo)
    add_compile_definitions(CONFIG_CONNECTED)
    # Force configs
    set(config_connected TRUE)
    set(config_rest TRUE)
endif(NOT config_demo)

if(config_externalreport)
    add_compile_definitions(CONFIG_EXTERNALREPORT)
endif(config_externalreport)

# saves preprocessor output to .ii files in build dir
if(config_view_preproc)
    set(TUCUXI_CXXFLAGS ${TUCUXI_CXXFLAGS} -save-temps)
endif(config_view_preproc)

add_compile_definitions(tucuxi PUBLIC QT_MESSAGELOGCONTEXT)

if(APPLE)
    add_compile_definitions(tucuxi PUBLIC MACOS)
endif(APPLE)

### Dependencies
add_subdirectory(libs/)

### Build
add_subdirectory(src/)

### Tests
if(config_guitest)
    ## TODO Add test libraries
    enable_testing(true)
    add_subdirectory(test/gui)
endif(config_guitest)
