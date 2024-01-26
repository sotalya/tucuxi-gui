set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

### Add libs
add_subdirectory(tucuxi-core/libs/)

if(WIN32)
    set(XercesC_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/xerces-c/build/lib)
    set(XercesC_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/xerces-c/build/include)
else()
    set(XercesC_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/xerces-c/build/src/)
    set(XercesC_INCLUDE_DIR
            ${CMAKE_CURRENT_SOURCE_DIR}/xerces-c/build/include
            ${CMAKE_CURRENT_SOURCE_DIR}/xerces-c/src
            ${CMAKE_CURRENT_SOURCE_DIR}/xerces-c/build/src
    )
endif(WIN32)

set(XercesC_VERSION 3.2.4)

find_package(XercesC REQUIRED)

add_library(xerces INTERFACE)
if(WIN32)
	target_link_libraries(xerces INTERFACE XercesC)
else()
	# target_link_libraries(xerces INTERFACE libxerces-c.a)
endif(WIN32)

# Set locally and for parent scope
set(LIBS_INCLUDES
        ${LIBS_INCLUDES}
        ${XercesC_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/tucuxi-core/src
)
set(LIBS_INCLUDES
        ${LIBS_INCLUDES}
        ${XercesC_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/tucuxi-core/src
    PARENT_SCOPE
)
