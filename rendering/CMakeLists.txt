add_library(renderer Shader.cpp Texture.cpp ResourceManager.cpp SpriteRenderer.cpp GameHandler.cpp)

target_link_libraries(renderer Game)

target_include_directories(renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
