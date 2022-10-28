project(demonsinvasion)
add_executable(${PROJECT_NAME} main.cpp window.cpp demon.cpp shot.cpp ship.cpp)
enable_abcg(${PROJECT_NAME})