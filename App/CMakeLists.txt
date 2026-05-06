set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories("${CMAKE_SOURCE_DIR}/App/Include" "${CMAKE_SOURCE_DIR}/App/External"
)

file(GLOB_RECURSE APP_SRC "${CMAKE_SOURCE_DIR}/App/src/*.c")

add_executable(App ${APP_SRC})

target_compile_options(App PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wconversion
    -Wsign-conversion
    -Wstrict-prototypes
    -Werror
)