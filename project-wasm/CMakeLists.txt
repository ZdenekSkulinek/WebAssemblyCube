cmake_minimum_required(VERSION 3.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(cube-wasm)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}../build-wasm")
set(RUNTIME_OUTPUT_DIRECTORY "${RUNTIME_OUTPUT_DIRECTORY}../build-wasm")

set(ASSETS
        "Robot.bmp"
        )

set(listAssets "")
FOREACH(f IN LISTS ASSETS)
  get_filename_component(_dir ${f} DIRECTORY)
  string(CONCAT listAssets "${listAssets}" "--preload-file ")
  string(CONCAT listAssets "${listAssets}" "${PROJECT_SOURCE_DIR}"  )
  string(CONCAT listAssets "${listAssets}" "/../"  )
  string(CONCAT listAssets "${listAssets}" "${f}"  )
  string(CONCAT listAssets "${listAssets}" "@")
  string(CONCAT listAssets "${listAssets}" "${_dir}" )
  string(CONCAT listAssets "${listAssets}" "/ " )
ENDFOREACH(f)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 -D__EMSCRIPTEN__ -g -gsource-map --source-map-base='./'  ")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} \
        -s TOTAL_MEMORY=50331648 \
        --use-preload-plugins \
        ${listAssets} "
        )

add_executable(cube-wasm ../main.cpp ../window.cpp  ../Cube.h ../Cube.cpp ../CubeTextured.cpp ../CubeTextured.h)
set_property(TARGET cube-wasm PROPERTY CXX_STANDARD 11)
add_custom_command(TARGET cube-wasm POST_BUILD COMMAND cp -f ../build-wasm/cube-wasm.wasm ../cube-wasm.wasm)
add_custom_command(TARGET cube-wasm POST_BUILD COMMAND cp -f ../build-wasm/cube-wasm.wasm.map ../cube-wasm.wasm.map)
add_custom_command(TARGET cube-wasm POST_BUILD COMMAND cp -f ../build-wasm/cube-wasm.js ../cube-wasm.js)
add_custom_command(TARGET cube-wasm POST_BUILD COMMAND cp -f ../build-wasm/cube-wasm.data ../cube-wasm.data)