
add_executable (Project1)

target_sources(Project1 PRIVATE main.cpp glad.c )

target_include_directories(Project1 PRIVATE ${CMAKE_SOURCE_DIR}/Thirdparty/glad/include)

target_link_libraries(Project1 PRIVATE glfw)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable (learn)

target_sources(learn PRIVATE learn.cpp glad.c)

target_include_directories(learn PRIVATE ${CMAKE_SOURCE_DIR}/Thirdparty/glad/include)

target_link_libraries(learn PRIVATE glfw)
