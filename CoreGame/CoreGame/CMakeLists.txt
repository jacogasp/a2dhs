project(CoreGame CXX)
set(targetname "CoreGame")
add_library(
        ${targetname} SHARED
        src/GodotLibrary.cpp
        include/Controller.h src/Controller.cpp
        include/Player.h src/Player.cpp
        include/Enemy.h src/Enemy.cpp
        include/Mob.h src/Mob.cpp
        include/HUD.h src/HUD.cpp
        include/Main.h src/Main.cpp
        include/Map.h src/Map.cpp
        include/CoreMath.h
        include/VisibilityPolygon.h src/VisibilityPolygon.cpp
)

target_link_libraries(CoreGame PUBLIC godot-cpp)

message(STATUS "Build dir: ${BUILD_DIR}")
set_target_properties(${targetname} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG "${BUILD_DIR}")
set_target_properties(${targetname} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE "${BUILD_DIR}")
set_target_properties(${targetname} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BUILD_DIR}")
set_target_properties(${targetname} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BUILD_DIR}")
target_compile_options(${targetname} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall>
        )

target_include_directories(
        ${targetname} PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

export(TARGETS ${targetname} godot-cpp NAMESPACE CoreGame:: FILE /usr/local/lib/CoreGameConfig.cmake)
install(TARGETS ${targetname} godot-cpp)