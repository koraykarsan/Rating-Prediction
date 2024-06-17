project(TermProject)
find_package(Threads REQUIRED)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -mcmodel=medium")

add_executable(TermProject main.cpp DataFrame.cpp Array2D.cpp)
target_link_libraries(TermProject PRIVATE Threads::Threads)
