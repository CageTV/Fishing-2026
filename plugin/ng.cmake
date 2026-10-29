# Fishing2026.dll on alandtse's CommonLibSSE-NG; included by E:/WorkSpace/ng-build/CMakeLists.txt (see ng_plugin there)
ng_plugin(TARGET Fishing2026 NAME Fishing2026 VERSION 1.0.0 ROOT "${CMAKE_CURRENT_LIST_DIR}"
    SOURCES src/main.cpp src/Settings.cpp src/HandFishing.cpp src/WaterCheck.cpp src/FancySettings.cpp src/Menu.cpp src/Preview.cpp
    INCLUDES "${CMAKE_CURRENT_LIST_DIR}/src" "${CMAKE_CURRENT_LIST_DIR}/include"
    PCH "${CMAKE_CURRENT_LIST_DIR}/src/PCH.h")
