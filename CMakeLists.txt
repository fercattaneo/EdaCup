
cmake_minimum_required(VERSION 3.0)

project("EDACUP")

set (CMAKE_CXX_STANDARD 11)

# From "Working with CMake" documentation:
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # AddressSanitizer (ASan)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif()
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # UndefinedBehaviorSanitizer (UBSan)
    add_compile_options(-fsanitize=undefined)
    add_link_options(-fsanitize=undefined)
endif()

add_executable(main main.cpp MQTTClient2.cpp Players.cpp GameModel.cpp data.cpp)

# Packages
find_package(raylib REQUIRED)
find_package(mosquitto REQUIRED)
find_package(OpenSSL REQUIRED)

target_include_directories(main PRIVATE ${RAYLIB_INCLUDE_DIRS} ${MOSQUITTO_INCLUDE_DIRS})
target_link_libraries(main PRIVATE ${RAYLIB_LIBRARIES} ${MOSQUITTO_LIBRARIES} OpenSSL::SSL 
        OpenSSL::Crypto)

##enable_testing()

#TODO SEPARA LAS FUNCIONES EN ARCHIVOS Y PONERLAS EN ESTE PARENTESIS
##add_executable(main_test main_test.cpp logic.cpp text.cpp "text.h")

##add_test(NAME test1 COMMAND main_test)
