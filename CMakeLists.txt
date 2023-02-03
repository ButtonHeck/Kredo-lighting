cmake_minimum_required(VERSION 3.16)

project(Kredo-lighting
    VERSION 0.1
    DESCRIPTION "OpenGL lighting sandbox with wxWidgets"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
unset("QT_QMAKE_EXECUTABLE")


# Sources and includes
set(KREDO_SOURCES_DIR "${PROJECT_SOURCE_DIR}/src")
set(KREDO_VENDOR_DIR "${PROJECT_SOURCE_DIR}/vendor")

include_directories(
    ${KREDO_SOURCES_DIR}
)

set(HEADER_FILES
    "${KREDO_SOURCES_DIR}/core/application.h"
    "${KREDO_SOURCES_DIR}/core/main_window.h"
    "${KREDO_SOURCES_DIR}/core/logger_controller.h"
    "${KREDO_SOURCES_DIR}/opengl/opengl_window.h"
    "${KREDO_SOURCES_DIR}/opengl/opengl_canvas.h"
    "${KREDO_SOURCES_DIR}/opengl/opengl_manager.h"
)

set(SOURCE_FILES
    "${KREDO_SOURCES_DIR}/core/application.cpp"
    "${KREDO_SOURCES_DIR}/core/main_window.cpp"
    "${KREDO_SOURCES_DIR}/core/logger_controller.cpp"
    "${KREDO_SOURCES_DIR}/opengl/opengl_window.cpp"
    "${KREDO_SOURCES_DIR}/opengl/opengl_canvas.cpp"
    "${KREDO_SOURCES_DIR}/opengl/opengl_manager.cpp"
)


# Target
add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
    ${HEADER_FILES}
)


# Submodules
find_package(Git QUIET)
if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/../.git")
    # update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" OFF)
    if (GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if (NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()


# check all the submodules
if (NOT EXISTS "${KREDO_VENDOR_DIR}/assimp/CMakeLists.txt")
    message(FATAL_ERROR "assimp submodule was not downloaded! GIT_SUBMODULE was turned off or failed. Update submodules and try again.")
endif()
if (NOT EXISTS "${KREDO_VENDOR_DIR}/glm/CMakeLists.txt")
    message(FATAL_ERROR "glm submodule was not downloaded! GIT_SUBMODULE was turned off or failed. Update submodules and try again.")
endif()

option(ASSIMP_BUILD_TESTS OFF)
option(ASSIMP_INSTALL OFF)

add_subdirectory(vendor/Glad)
add_subdirectory(vendor/assimp)
add_subdirectory(vendor/glm)

find_package(wxWidgets REQUIRED COMPONENTS core gl)
include(${wxWidgets_USE_FILE})


# Dependencies
target_include_directories(${PROJECT_NAME}
    PRIVATE "${KREDO_VENDOR_DIR}/Glad/include"
    PRIVATE "${KREDO_VENDOR_DIR}/assimp/include"
    PRIVATE "${KREDO_VENDOR_DIR}/glm"
)

target_link_directories(${PROJECT_NAME}
    PRIVATE "${KREDO_VENDOR_DIR}/vendor/Glad/src"
    PRIVATE "${KREDO_VENDOR_DIR}/vendor/assimp/lib"
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${wxWidgets_LIBRARIES}
    PRIVATE glad
    PRIVATE assimp
    PRIVATE dl
)
