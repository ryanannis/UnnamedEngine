cmake_minimum_required(VERSION 3.0)
project(UnnamedEngine)

# Compile a lib with everything EXCEPT main.cpp
# and an executable containing everything else

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(UnnamedEngine/Vendor/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(UnnamedEngine/Vendor/assimp)

option(BUILD_BULLET2_DEMOS OFF)
option(BUILD_CPU_DEMOS OFF)
option(BUILD_EXTRAS OFF)
option(BUILD_OPENGL3_DEMOS OFF)
option(BUILD_UNIT_TESTS OFF)
add_subdirectory(UnnamedEngine/Vendor/bullet)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /std:c++17 /Zi")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++17")
endif()

include_directories(UnnamedEngine/Sources/
                    UnnamedEngine/Vendor/assimp/include/
					UnnamedEngine/Vendor/catch/
                    UnnamedEngine/Vendor/bullet/src/
                    UnnamedEngine/Vendor/glfw/include/
                    UnnamedEngine/Vendor/glm/
                    UnnamedEngine/Vendor/stb/)

file(GLOB_RECURSE PROJECT_HEADERS UnnamedEngine/Sources/*.h)
file(GLOB_RECURSE PROJECT_SOURCES UnnamedEngine/Sources/Engine/*.cpp UnnamedEngine/Sources/Game/*.cpp)
file(GLOB_RECURSE PROJECT_SHADERS UnnamedEngine/Shaders/*.comp
                          UnnamedEngine/Shaders/*.frag
                          UnnamedEngine/Shaders/*.geom
                          UnnamedEngine/Shaders/*.vert)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Vendors" FILES ${VENDORS_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE
                "-DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\""
				"-DCONTENT_DIR=\"${PROJECT_SOURCE_DIR}/Content/\"")

add_library(ProjectLib ${PROJECT_SOURCES} ${PROJECT_HEADERS}
										  ${PROJECT_SHADERS} 
										  ${PROJECT_CONFIGS}
										  ${VENDORS_SOURCES})

# Compile main executable

add_executable(${PROJECT_NAME} UnnamedEngine/Sources/main.cpp)
target_link_libraries(${PROJECT_NAME} ProjectLib assimp glfw
						${GLFW_LIBRARIES} 
						BulletDynamics BulletCollision LinearMath)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})


# Compile unit testing libs 

enable_testing()

file(GLOB_RECURSE TESTING_SOURCES UnnamedEngine/Test/*.cpp)
file(GLOB_RECURSE TESTING_HEADERS UnnamedEngine/Test/*.h)

add_executable(runUnitTests ${TESTING_SOURCES} ${TESTING_HEADERS})

target_link_libraries(runUnitTests ProjectLib assimp glfw
						${GLFW_LIBRARIES} BulletDynamics BulletCollision LinearMath)
