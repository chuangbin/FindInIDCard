#版本
cmake_minimum_required(VERSION 3.4.1)

#导入头文件
include_directories(include)

file(GLOB my_source_path ${CMAKE_SOURCE_DIR}/*.cpp ${CMAKE_SOURCE_DIR}/*.c)

add_library(OpenCV SHARED ${my_source_path})

add_library(lib_opencv SHARED IMPORTED)

set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libopencv_java4.so)

find_library( # Sets the name of the path variable.
        log-lib
        log)

find_library(jnigraphics-lib
        jnigraphics)

target_link_libraries( # Specifies the target library.
        OpenCV
        ${log-lib}
        ${jnigraphics-lib}
        lib_opencv)