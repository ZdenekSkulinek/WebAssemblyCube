WebAssembly cube - The WebGL example
====================================

For a longer time, I was unable fo found openGL example with **WebAssembly** technology. So I just wite it.
Please consider is as a small "Hello world" example with graphics.

![Cube example](cube_md.png)

Project contains two subprojects, one is web page, second is standalone openGL native application.

Look at demon on [ZdenekSkulinek.cz](http://test.zdenekskulinek.cz/example)

Prereqisities
-------------

- OpenGl properly installed, if uoy want use standalone application
- Emscripten installed. Recently, I am using version 2.0.29.
- CMake. I have targetted project for version 3.0.
- CLion IDE is optional. Its only one proprietal software that I am using. It's not neccesary, you run project from commaned line, of cource.
- Python 3. version is not important, take latest.

Run in web browser
------------------

I have use Python http server. Cmake copies all files into root of project, so you can run:

```bash
python3 -m http.server 8000
```

in root and and navigate to, surprisingly, **http://localhost:8000**.

Building webassembly project
----------------------------

You need to find toolchain file inside your emsdk directory. For example, if I have emsdk indtalled on /home/zdenek/dev/emsdk.
```bash
mkdir build-wasm
cd build-wasm
cmake -D CMAKE_BUILD_TYPE=debug -D CMAKE_TOOLCHAIN_FILE=/home/zdenek/dev/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake ../project-wasm/
make
```

and run...

```bash
cd ..
python3 -m http.server 8000
```

Building standalone project
---------------------------

```bash
cd project-stand
cmake ../project-stand/
make
```

and run...

```bash
./cube-stand
```