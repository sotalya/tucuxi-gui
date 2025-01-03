set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

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

if(WIN32)
target_link_libraries(guitest
                PRIVATE
                    xerces
                    apputils
                    admin
                    core
                    "C:\Program Files\googletest-distribution\lib\gtest.lib"
                    "C:\Program Files\Spix\lib\Spix.lib"
                    ${QT_COMPONENTS}
)
else()
target_link_libraries(guitest
                PRIVATE
                    xerces
                    apputils
                    admin
                    core
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

