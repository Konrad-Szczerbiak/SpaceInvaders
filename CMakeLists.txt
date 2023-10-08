

cmake_minimum_required(VERSION 3.23)
project(SpaceInvaders C)

set(CMAKE_C_STANDARD 99)


find_package(SDL2 REQUIRED SDL2)

include_directories(inc)

add_executable(SpaceInvaders src/main.c src/SDL_Init.c src/inputs.c src/draw.c src/collison.c inc/collison.h src/entities.c src/enemies.c inc/enemies.h src/utilities.c src/player.c inc/player.h)

add_library(SDL_Image STATIC IMPORTED inc/entities.h inc/utilities.h src/enemyAI.c inc/enemyAI.h)
set_target_properties(SDL_Image PROPERTIES IMPORTED_LOCATION /usr/local/lib/libSDL2_image.a)

target_link_libraries(SpaceInvaders PRIVATE SDL2::SDL2 SDL_Image)