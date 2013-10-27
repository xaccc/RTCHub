#/**********************************************************\ 
# Auto-generated X11 project definition file for the
# WebrtcEngine project
#\**********************************************************/

# X11 template platform definition CMake file
# Included from ../CMakeLists.txt

# remember that the current source dir is the project root; this file is in X11/
file (GLOB PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    X11/[^.]*.cpp
    X11/[^.]*.h
    X11/[^.]*.cmake
    )

SOURCE_GROUP(X11 FILES ${PLATFORM})

# use this to add preprocessor definitions
add_definitions(
    -DPOSIX
    -DWEBRTC_LINUX
    -DIRENDER_X11
)

set (SOURCES
    ${SOURCES}
    ${PLATFORM}
    )

add_x11_plugin(${PROJECT_NAME} SOURCES)

find_package(GTK2 REQUIRED)
if (GTK2_FOUND)
    include_directories(${GTK2_INCLUDE_DIR})
    add_definitions(-DHAVE_GTK2)
    message("GTK 2.x found and used as GUI\n")
else (GTK2_FOUND)
    message(FATAL_ERROR "GTK2 is required to build this project.")
endif (GTK2_FOUND)

find_library(LIBWEBRTC libwebrtc_Release.a ../../../../third_party/libjingle/trunk/out/Release)

# add library dependencies here; leave ${PLUGIN_INTERNAL_DEPS} there unless you know what you're doing!
target_link_libraries(${PROJECT_NAME}
    ${PLUGIN_INTERNAL_DEPS}
    #${GTK2_LIBRARIES}
    ${LIBWEBRTC}
    -lX11
    -lXext
    -lXrender
    -lXcomposite
    -lGL
    -lexpat
    -ldl
    -lrt
    -lpthread
    -lssl
    -lcrypto
    )
