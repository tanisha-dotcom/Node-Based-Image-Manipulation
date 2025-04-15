cmake_minimum_required(VERSION 3.10)
project(ImageProcessingNodeApp)

set(CMAKE_CXX_STANDARD 17)

# -------------------- Options --------------------
# Set these paths via -D in cmake command or set them as environment variables
set(OpenCV_DIR $ENV{OpenCV_DIR} CACHE PATH "Path to OpenCV directory")
set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/libs/imgui CACHE PATH "Path to ImGui directory")
set(FASTNOISE_DIR ${PROJECT_SOURCE_DIR}/libs CACHE PATH "Path to FastNoise directory")

# -------------------- Includes --------------------
include_directories(
    ${IMGUI_DIR}
    ${FASTNOISE_DIR}
    ${PROJECT_SOURCE_DIR}/src
)

# -------------------- ImGui Sources --------------------
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
)

# -------------------- Main Executable --------------------
file(GLOB_RECURSE PROJECT_SOURCES
    src/*.cpp
)

add_executable(main
    ${PROJECT_SOURCES}
    ${IMGUI_SOURCES}
)

# -------------------- OpenCV --------------------
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(main ${OpenCV_LIBS})
