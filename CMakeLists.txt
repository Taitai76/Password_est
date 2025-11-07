cmake_minimum_required(VERSION 3.16)
project(password_est LANGUAGES CXX)

find_package(Drogon REQUIRED)

add_executable(password_est
  src/main.cpp
  src/controllers/PasswordController.cc
  src/utils/Strength.cc
)

target_include_directories(password_est PRIVATE src)
target_link_libraries(password_est PRIVATE Drogon::Drogon)
set_property(TARGET password_est PROPERTY CXX_STANDARD 17)

configure_file(${CMAKE_SOURCE_DIR}/config.json
               ${CMAKE_BINARY_DIR}/config.json COPYONLY)

file(COPY ${CMAKE_SOURCE_DIR}/public
     DESTINATION ${CMAKE_BINARY_DIR})