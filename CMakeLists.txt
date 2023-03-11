cmake_minimum_required(VERSION 3.16)

project(Kredo-lighting
    VERSION 0.1
    DESCRIPTION "OpenGL lighting sandbox with wxWidgets"
    LANGUAGES CXX
)

configure_file(Kredo-lighting-Config.h.in Kredo-lighting-Config.h)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/.opt/${PROJECT_NAME}")
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
unset("QT_QMAKE_EXECUTABLE")


# Sources and includes
set(KREDO_SOURCES_DIR "${PROJECT_SOURCE_DIR}/src")
set(KREDO_VENDOR_DIR "${PROJECT_SOURCE_DIR}/vendor")
set(KREDO_RESOURCES_DIR "${PROJECT_SOURCE_DIR}/resources")

include_directories(
    ${KREDO_SOURCES_DIR}
    "${KREDO_SOURCES_DIR}/core"
    "${KREDO_SOURCES_DIR}/log"
    "${KREDO_SOURCES_DIR}/opengl"
    "${KREDO_SOURCES_DIR}/entities"
    "${KREDO_SOURCES_DIR}/test"
    ${PROJECT_BINARY_DIR}
)

file(GLOB KREDO_SOURCES
    src/core/*.h
    src/core/*.cpp
    src/log/*.h
    src/log/*.cpp
    src/opengl/*.h
    src/opengl/*.cpp
    src/entities/*.h
    src/entities/*.cpp
    src/test/*.h
    src/test/*.cpp
)


# Target
add_executable(${PROJECT_NAME} ${KREDO_SOURCES})
target_compile_definitions(${PROJECT_NAME} PRIVATE KREDO_RESOURCES_DIR="${KREDO_RESOURCES_DIR}")
target_compile_definitions(${PROJECT_NAME} PRIVATE KREDO_CONFIG_DIR="$ENV{HOME}/.local/share/${PROJECT_NAME}")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/locale" "${CMAKE_CURRENT_BINARY_DIR}/locale"
)


# Installation
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/locale" DESTINATION bin)


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
if (NOT EXISTS "${KREDO_VENDOR_DIR}/yaml-cpp/CMakeLists.txt")
    message(FATAL_ERROR "yaml-cpp submodule was not downloaded! GIT_SUBMODULE was turned off or failed. Update submodules and try again.")
endif()
if (NOT EXISTS "${KREDO_VENDOR_DIR}/entt/CMakeLists.txt")
    message(FATAL_ERROR "entt submodule was not downloaded! GIT_SUBMODULE was turned off or failed. Update submodules and try again.")
endif()

option(ASSIMP_BUILD_TESTS OFF)
option(ASSIMP_INSTALL OFF)
option(YAML_CPP_BUILD_TOOLS OFF)

add_subdirectory(vendor/Glad)
add_subdirectory(vendor/assimp)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/yaml-cpp)
add_subdirectory(vendor/entt)

find_package(wxWidgets REQUIRED COMPONENTS core gl)
include(${wxWidgets_USE_FILE})


# Dependencies
target_include_directories(${PROJECT_NAME}
    PRIVATE "${KREDO_VENDOR_DIR}/Glad/include"
    PRIVATE "${KREDO_VENDOR_DIR}/assimp/include"
    PRIVATE "${KREDO_VENDOR_DIR}/glm"
    PRIVATE "${KREDO_VENDOR_DIR}/yaml-cpp/include"
    PRIVATE "${KREDO_VENDOR_DIR}/entt/include"
)

target_link_directories(${PROJECT_NAME}
    PRIVATE "${KREDO_VENDOR_DIR}/vendor/Glad/src"
    PRIVATE "${KREDO_VENDOR_DIR}/vendor/assimp/lib"
    PRIVATE "${KREDO_VENDOR_DIR}/vendor/yaml-cpp/src"
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${wxWidgets_LIBRARIES}
    PRIVATE glad
    PRIVATE assimp
    PRIVATE yaml-cpp
    PRIVATE EGL
    PRIVATE dl
)
