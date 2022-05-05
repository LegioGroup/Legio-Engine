cmake_minimum_required(VERSION 3.15)
project(Legio VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

#Add external Libs
add_subdirectory(external/glm)
add_subdirectory(external/glfw)
add_subdirectory(external/spdlog)

#Find Packages
find_package(Vulkan REQUIRED)

#Not very effient way to add files if the project gets too big
file(GLOB SOURCES "src/*.cpp" "src/*/*.cpp" "src/*/*/*.cpp")

add_library(${PROJECT_NAME} 
    ${SOURCES})
    
 add_custom_command(TARGET ${PROJECT_NAME}
                     POST_BUILD
                     COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/src/rendering/vulkan/shaders/compile.bat)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        glm
        spdlog/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${Vulkan_INCLUDE_DIRS}
        glfw
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        glm
        spdlog
    PRIVATE
        glfw
        ${Vulkan_LIBRARIES}
)