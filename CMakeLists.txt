cmake_minimum_required(VERSION 3.10.0)
project(zGL VERSION 0.1.0 LANGUAGES C CXX)

set(GLFW_INCLUDE_DIR "~/src/zGL/zGL/Libraries/include/GLFW")
set(GLFW_LIBRARY "~/src/zGL/zGL/Libraries/lib/libglfw3.a")
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)


include_directories(${zGL_SOURCE_DIR}/Libraries/include ${GLFW_INCLUDE_DIR})


add_executable(zGL camera.cpp contentManager.cpp glad.c graphicsDevice.cpp main.cpp material.cpp mesh.cpp model.cpp presentationParameters.cpp primitiveBuilder.cpp stb_image.cpp texture2d.cpp timer.cpp tiny_obj_loader.cpp shader.cpp zviewport.cpp)

target_link_libraries(zGL PRIVATE ${GLFW_LIBRARY} OpenGL::GL X11 Xxf86vm Xrandr pthread Xi dl)
