cmake_minimum_required(VERSION 3.10)
project(LabEncryption)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CRYPTOPP REQUIRED libcrypto++)

add_executable(main
    src/main.cpp
    src/FileSearcher.cpp
    src/Crypto.cpp
)

target_include_directories(main PRIVATE ${CRYPTOPP_INCLUDE_DIRS})
target_link_libraries(main PRIVATE ${CRYPTOPP_LIBRARIES} pthread)
