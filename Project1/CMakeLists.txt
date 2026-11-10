add_executable(Project1
    main.cpp
    glad.c
)

# GLAD and GLM include directories
target_include_directories(Project1 PRIVATE
    "${CMAKE_SOURCE_DIR}/Thirdparty/glad/include"
    "${CMAKE_SOURCE_DIR}/Thirdparty/glm"
)

# Link libraries
target_link_libraries(Project1 PRIVATE
    glfw
    OpenGL::GL
)

# Copy shaders and resources beside the executable
add_custom_command(TARGET Project1 POST_BUILD

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
        "$<TARGET_FILE_DIR:Project1>/shaders"

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
        "$<TARGET_FILE_DIR:Project1>/Resources"
)