cmake_minimum_required(VERSION 3.21)

# Fishing2026.dll - the SKSE plugin of Fishing 2026.
# This is the "older" build (CharmedBaryon's CommonLibSSE-NG: Skyrim VR and SE/AE 1.6.1130 and older); the "new" build is made by E:/WorkSpace/ng-build.
project(Fishing2026 VERSION 1.0.0 LANGUAGES CXX)

find_package(CommonLibSSE CONFIG REQUIRED)

add_commonlibsse_plugin(${PROJECT_NAME} NAME "Fishing2026" AUTHOR "CageTV" SOURCES
    src/main.cpp
    src/Settings.cpp
    src/HandFishing.cpp
    src/WaterCheck.cpp
    src/FancySettings.cpp
    src/Menu.cpp
    src/Preview.cpp
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_include_directories(${PROJECT_NAME} PRIVATE src include)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/PCH.h)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:/Z7>)
target_link_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:/DEBUG /OPT:REF /OPT:ICF>)
