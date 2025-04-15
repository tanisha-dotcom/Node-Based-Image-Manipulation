cmake_minimum_required(VERSION 3.10)
project(OpenCV_Mingw_Example)

set(CMAKE_CXX_STANDARD 17)

set(OpenCV_DIR "C:/OpenCV_Mingw/install") 

include_directories("C:/OpenCV_Mingw/include")

link_directories("C:/OpenCV_Mingw/install/x64/mingw/lib")  

set(IMGUI_DIR "D:/Coding/Node Assignment/libs/imgui")

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
)

include_directories(${IMGUI_DIR})


add_executable(main
    src/main.cpp

    src/graph/NodeGraph.cpp

    src/nodes/ImageInputNode.cpp
    src/nodes/OutputNode.cpp
    src/nodes/BrightnessContrastNode.cpp
    src/nodes/ColorChannelSplitterNode.cpp
    src/nodes/BlurNode.cpp
    src/nodes/ThresholdNode.cpp
    src/nodes/EdgeDetectionNode.cpp
    ${IMGUI_SOURCES} 
)


find_package(OpenCV REQUIRED)

target_link_libraries(main ${OpenCV_LIBS})
