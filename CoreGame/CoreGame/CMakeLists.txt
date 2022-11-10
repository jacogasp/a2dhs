project(CoreGame CXX)
set(target "CoreGame")
add_library(
        ${target} SHARED
        src/GodotLibrary.cpp
        include/Controller.h src/Controller.cpp
        include/Player.h src/Player.cpp
        include/Enemy.h src/Enemy.cpp
        include/Mob.h src/Mob.cpp
        include/HUD.h src/HUD.cpp
        include/Main.h src/Main.cpp
        include/Map.h src/Map.cpp
        include/Torch.h src/Torch.cpp
        include/RedEyesSpawner.h src/RedEyesSpawner.cpp
        include/CoreMath.h
        include/VisibilityPolygon.h src/VisibilityPolygon.cpp
        include/Timer.h)

target_link_libraries(${target} PUBLIC godot-cpp)

message(STATUS "Build dir: ${BUILD_DIR}")
set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG "${BUILD_DIR}")
set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE "${BUILD_DIR}")
set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BUILD_DIR}")
set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BUILD_DIR}")
target_compile_options(
        ${target} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall>
        $<$<CONFIG:DEBUG>:-DDEBUG>
)
if (UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

target_include_directories(
        ${target} PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND APPLE)
export(TARGETS ${target} godot-cpp NAMESPACE CoreGame:: FILE /usr/local/lib/CoreGameConfig.cmake)
install(TARGETS ${target} godot-cpp)
endif()

## Tests
add_subdirectory(tests)