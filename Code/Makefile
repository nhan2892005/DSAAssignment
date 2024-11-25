project(assignment)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -std=c++17 -DTEST_LAYER")

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/tensor)
include_directories(${PROJECT_SOURCE_DIR}/include/sformat)
include_directories(${PROJECT_SOURCE_DIR}/include/ann)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/test)

FILE(GLOB_RECURSE SRC ./src/*.cpp)

add_executable(main
    ${SRC}
    /home/kdm/Documents/UNI/241/DSA/TEMPATE_27_10/test/unit_test/layer/unit_test_softmax.cpp
    main.cpp
)