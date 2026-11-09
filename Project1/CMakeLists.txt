
add_executable(Project1)

target_sources(Project1 PRIVATE main.cpp glad.c )

target_include_directories(Project1 PRIVATE ${CMAKE_SOURCE_DIR}/Thirdparty/glad/include)

target_link_libraries(Project1 PRIVATE glfw)

target_include_directories(Project1 PRIVATE ${CMAKE_SOURCE_DIR}/Thirdparty/glm)

add_custom_command(TARGET Project1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/shaders" "$<TARGET_FILE_DIR:Project1>/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Resources" "$<TARGET_FILE_DIR:Project1>/Resources"
)

add_executable (learn)

target_sources(learn PRIVATE learn.cpp glad.c)

target_include_directories(learn PRIVATE ${CMAKE_SOURCE_DIR}/Thirdparty/glad/include)

target_link_libraries(learn PRIVATE glfw)
