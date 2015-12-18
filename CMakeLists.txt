cmake_minimum_required(VERSION 3.3)
project(sokoban)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(SOURCE_FILES
    shaders/frag.glsl
    shaders/vert.glsl
    image_reader/stb_image.h
    maths/maths.c
    maths/maths.h
    objects/cube.c
    objects/cube.h
    utils/gl_utils.c
    utils/gl_utils.h
    level_reader.c
    level_reader.h
    main.c
    sokoban.h
    warehouse.c
    warehouse.h)

include_directories(image_reader maths objects textures utils)

add_executable(sokoban ${SOURCE_FILES})

target_link_libraries(sokoban GL GLEW glfw m)
