add_library(OpenGL frameCreator.cpp VAO.cpp VBO.cpp EBO.cpp shaderClass.cpp)

target_include_directories(OpenGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
