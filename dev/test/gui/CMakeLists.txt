set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

#include(FetchContent)
#FetchContent_Declare(
#  googletest
#  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
#)
## For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)

#find_package(Spix REQUIRED)

### Build
set(GUITEST_HEADER_FILES
    src/defaultdrugconfig.h
    src/guitest.h
)

set(GUITEST_SOURCE_FILES
    src/guitest.cpp
    src/addeditremovetest.cpp
    src/adjustmenttest.cpp
    src/cachetest.cpp
    src/daylightsavingtest.cpp
    src/flatrequeststest.cpp
    src/functionstester.cpp
    src/guidedstresstest.cpp
    src/issueshighlighter.cpp
    src/minimaldatatest.cpp
    src/pendingrequesttest.cpp
    src/randomtest.cpp
    src/reporttest.cpp
    src/savedtestcomparison.cpp
    src/sentencesshortcutstest.cpp
    src/signaturetest.cpp
    src/stresstest.cpp
    src/targettest.cpp
    src/validationloadfile.cpp
    src/validationtest.cpp
    src/voriconazoletest.cpp
)

add_library(guitest
                SHARED
                    ${GUITEST_HEADER_FILES}
                    ${GUITEST_SOURCE_FILES}
)

if(WIN32)
    target_include_directories(guitest
                    PUBLIC
                        ${CMAKE_CURRENT_SOURCE_DIR}
                    PRIVATE
                        ${APPUTILS_INCLUDES}
                        ${ADMIN_INCLUDES}
                        ${CORE_INCLUDES}
                        ${GUIUTILS_INCLUDES}
                        ${CMAKE_CURRENT_SOURCE_DIR}/../../src
                        ${TUCUXI_BASE_DIR_INCLUDE}
                        ${LIBS_INCLUDES}
                        "C:/Program Files (x86)/googletest-distribution/include"
                        "C:/Program Files (x86)/Spix/include"
    )
else()
    target_include_directories(guitest
                    PUBLIC
                        ${CMAKE_CURRENT_SOURCE_DIR}
                    PRIVATE
                        ${APPUTILS_INCLUDES}
                        ${ADMIN_INCLUDES}
                        ${CORE_INCLUDES}
                        ${GUIUTILS_INCLUDES}
                        ${CMAKE_CURRENT_SOURCE_DIR}/../../src
                        ${TUCUXI_BASE_DIR_INCLUDE}
                        ${LIBS_INCLUDES}
    )
endif(WIN32)



if(WIN32)
target_link_libraries(guitest
                PRIVATE
                    xerces
                    guiutils
                    apputils
                    admin
                    core
                    tucucore
                    tucucommon
                    #googletest
                    "C:/Program Files (x86)/googletest-distribution/lib/gtest.lib"
                    "C:/Program Files (x86)/Spix/lib/Spix.lib"
                    ${QT_COMPONENTS}
)
else()
target_link_libraries(guitest
                PRIVATE
                    xerces
                    guiutils
                    apputils
                    admin
                    core
                    tucucore
                    tucucommon
                    /usr/local/lib/libSpix.a
                    ${QT_COMPONENTS}
)
endif(WIN32)


# Install rules
include(GNUInstallDirs)
install(TARGETS guitest
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

