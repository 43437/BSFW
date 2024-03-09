if exist build ( rd build /S /Q )
md build
cd build
cmake -G"Visual Studio 15 2017" -DCMAKE_CONFIGURATION_TYPES="Debug" --build ..