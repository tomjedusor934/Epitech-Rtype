set "BASEDIR=%~dp0"
pushd "%BASEDIR%"

rmdir /s /q build 2>nul
conan install . --output-folder=build --build=missing
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
