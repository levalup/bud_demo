cmake_minimum_required(VERSION 3.3)
project(Linking)

# 声明一些编译参数
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

# 添加bud的头文件和静态库
include_directories(../bud)
link_directories(../bud/build/lib)

# 查找源文件
aux_source_directory(${PROJECT_SOURCE_DIR} SOURCE_FILES)
#aux_source_directory(${PROJECT_SOURCE_DIR}/other OTHER_SOURCE_FILES)

# 合并不同目录下的源文件
list(APPEND SOURCE_FILES ${OTHER_SOURCE_FILES})

# 编译目标
add_executable(Linking ${SOURCE_FILES})
target_link_libraries(Linking bud SDL2 SDL2_image SDL2_ttf SDL2_mixer)

# 移动可执行文件
set(LOCAL_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/build)
set_target_properties(Linking PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${LOCAL_OUTPUT_DIR})