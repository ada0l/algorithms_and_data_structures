cmake_minimum_required(VERSION 2.8)

project(cards)

set(SOURCE_EXE main.c queue.c card.c random.c)

add_executable(main ${SOURCE_EXE})
