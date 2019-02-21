# Specify the minimum version for CMake
# cmake -H. -Bbuild

cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Debug. 
if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    message("Debug Build")
# Release. Tanne optimoinnit ym. 
elseif(${CMAKE_BUILD_TYPE} MATCHES Release)
    message("Release Build")
endif()

#find_package(OPENGL REQUIRED)

set (graphics_dir ${src_dir}/Graphics)
set (util_dir ${src_dir}/Utils)

set(graphics_src 
      ${graphics_dir}/window.cpp
      ${graphics_dir}/shader.cpp
      ${graphics_dir}/model.cpp
      ${graphics_dir}/vertexbuffer.cpp
      ${graphics_dir}/indexbuffer.cpp
      ${graphics_dir}/vertexAttributes.cpp
      )

set(util_src 
      ${util_dir}/log.cpp
      ${util_dir}/kokeilu.cpp
      )

# Jos luodaan webassemlya.
if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
	#	message("Emscripten!!!!!!!!!!!!!!!!!!.")
	#	message("${CMAKE_C_COMPILER}.")
	#	message("${CMAKE_CXX_COMPILER}.")
	#        set(CMAKE_C_COMPILER "emcc")
	#        set(CMAKE_CXX_COMPILER "em++")
	#	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[\"png\"]' -s DEMANGLE_SUPPORT=1 --preload-file assets --bind") 
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[\"png\"]'") 
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[\"png\"]'") 
	#set(LINK_FLAGS "${LINK_FLAGS} -s DEMANGLE_SUPPORT=1 --preload-file ${CMAKE_SOURCE_DIR}/assets --bind") 
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[\"png\"]'") 
endif()
	#        set_target_properties(Engine PROPERTIES LINK_FLAGS "-o dist/engine.js -s USE_FREETYPE=1 -s DISABLE_EXCEPTION_CATCHING=0 -s DEMANGLE_SUPPORT=1 -s SAFE_HEAP=1 --bind -s WASM=1 -O2 -s LEGACY_GL_EMULATION=0  -s GL_UNSAFE_OPTS=0 --pre-js pre-module.js --post-js post-module.js -s ASSERTIONS=1 -s GL_ASSERTIONS=1 -s INVOKE_RUN=0  -std=c++11 -s USE_WEBGL2=1 -s FULL_ES3=1 -s USE_GLFW=3 -s OFFSCREENCANVAS_SUPPORT=1 --preload-file textures --preload-file shaders --preload-file fonts")

# MAKE_RUNTIME_OUTPUT_DIRECTORY or EXECUTABLE_OUTPUT_PATH. Where to create the executable. 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Library output path.
set(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/lib)

# Tanne alki-kirjastot
#add_subdirectory(lib/math)

# Alikirjaston nimi, tiedostot ja tyyppi.
#add_library(math SHARED lib/math/operations.cpp)
#add_library(math STATIC lib/math/operations.cpp)

# The following folder will be included
include_directories("${CMAKE_SOURCE_DIR}/includes")

add_executable(hello ${src_dir}/hello.cpp ${util_src} ${graphics_src})
#set_target_properties(hello PROPERTIES LINK_FLAGS "-s INVOKE_RUN=0 -s SAFE_HEAP=1 -s DEMANGLE_SUPPORT=1 -s USE_FREETYPE=1 -s DISABLE_EXCEPTION_CATCHING=0 -s USE_WEBGL2=1 -s FULL_ES3=1 -s LEGACY_GL_EMULATION=0 -s GL_UNSAFE_OPTS=0 -s ASSERTIONS=1 -s GL_ASSERTIONS=1 -s OFFSCREENCANVAS_SUPPORT=1 --preload-file ${CMAKE_SOURCE_DIR}/assets/owl.png@assets/ --bind")
set_target_properties(hello PROPERTIES LINK_FLAGS "-s DEMANGLE_SUPPORT=1 -s GL_ASSERTIONS=1 -s USE_WEBGL2=1 -s FULL_ES3=1 -s FULL_ES2=1 --bind --preload-file ${CMAKE_SOURCE_DIR}/shaders@shaders/")
#set_target_properties(hello PROPERTIES LINK_FLAGS "-s DEMANGLE_SUPPORT=1 -s USE_WEBGL2=1 -s FULL_ES3=1 -s FULL_ES3=1 --preload-file ${CMAKE_SOURCE_DIR}/assets/owl.png@assets/ --bind")
#include_directories(${OPENGL_INCLUDE_DIR})
#link_directories(${util_src})
#target_link_libraries(${util_src})

#Tanne linkitykset
#target_link_libraries(hello math)
