cmake_minimum_required(VERSION 3.12)
project(skia_example)

set(CMAKE_CXX_STANDARD 14)

set(SKIA_SDK "$ENV{HOME}/git_repositories/skia")
set(SKIA_SDK_INCLUDE "${SKIA_SDK}/include")

message(${SKIA_SDK_INCLUDE})

include_directories(
        ${SKIA_SDK}/tools
        ${SKIA_SDK}/tools/timer
        /opt/X11/include
        ${SKIA_SDK}/src/utils
        ${SKIA_SDK}/src/core
        ${SKIA_SDK_INCLUDE}
        ${SKIA_SDK_INCLUDE}/android
        ${SKIA_SDK_INCLUDE}/atlastext
        ${SKIA_SDK_INCLUDE}/c
        ${SKIA_SDK_INCLUDE}/codec
        ${SKIA_SDK_INCLUDE}/config
        ${SKIA_SDK_INCLUDE}/core
        ${SKIA_SDK_INCLUDE}/effects
        ${SKIA_SDK_INCLUDE}/encode
        ${SKIA_SDK_INCLUDE}/gpu
        ${SKIA_SDK_INCLUDE}/gpu/gl
        ${SKIA_SDK_INCLUDE}/gpu/mock
        ${SKIA_SDK_INCLUDE}/gpu/mtl
        ${SKIA_SDK_INCLUDE}/gpu/vk
        ${SKIA_SDK_INCLUDE}/pathops
        ${SKIA_SDK_INCLUDE}/ports
        ${SKIA_SDK_INCLUDE}/private
        ${SKIA_SDK_INCLUDE}/svg
        ${SKIA_SDK_INCLUDE}/utils
        ${SKIA_SDK_INCLUDE}/utils/mac
        ${SKIA_SDK_INCLUDE}/views)


set(LIB_SOURCE_FILES
        main.cpp)


add_executable(skia_example ${LIB_SOURCE_FILES})

#gn gen out/Release --args="is_official_build=true skia_use_system_expat=false skia_use_system_icu=false skia_use_libjpeg_turbo=false skia_use_system_libpng=false skia_use_system_libwebp=false skia_use_system_zlib=false skia_use_libwebp=false extra_cflags_cc=[\"-frtti\"]"
target_link_libraries(skia_example ${SKIA_SDK}/out/Release/libskia.a)
target_link_libraries(skia_example ${SKIA_SDK}/out/Release/libpathkit.a)

if (APPLE)
    target_link_libraries(skia_example "-framework CoreServices")
    target_link_libraries(skia_example "-framework CoreGraphics")
    target_link_libraries(skia_example "-framework CoreText")
    target_link_libraries(skia_example "-framework CoreFoundation")
endif()

set(CMAKE_MODULE_PATH "/usr/local/Cellar/sdl2/2.0.9_1/lib/cmake")
find_package(SDL2 REQUIRED)
if (SDL2_FOUND)
    include_directories(${SDL2_INCLUDE_DIR})
    target_link_libraries(skia_example ${SDL2_LIBRARIES})
endif()
