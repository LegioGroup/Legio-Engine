cmake_minimum_required(VERSION 3.15)
project(Legio VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

#Add external Libs
add_subdirectory(external/glm)
add_subdirectory(external/glfw)

#Not very effient way to add files if the project gets too big
file(GLOB SOURCES "src/*.cpp" "src/*/*.cpp" "src/*/*/*.cpp")

add_library(${PROJECT_NAME} 
    ${SOURCES})
    
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        glm
    PRIVATE
        glfw
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        glm
    PRIVATE
        glfw
)