cmake_minimum_required(VERSION 3.12)
project("sdl-minesweeper" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(SDL_X86_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL3/x86_64-w64-mingw32")
set(SDL_I686_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL3/i686-w64-mingw32")
    
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    target_include_directories(${PROJECT_NAME}
        PRIVATE
            "${SDL_X86_DIR}/include"
    )
    
    target_link_directories(${PROJECT_NAME}
        PRIVATE
            "${SDL_X86_DIR}/lib"
    )
    
    file(COPY "${SDL_X86_DIR}/bin/SDL3.dll" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/build")
else()
    target_include_directories(${PROJECT_NAME}
        PRIVATE
            "${SDL_I686_DIR}/include"
    )
    
    target_link_directories(${PROJECT_NAME}
        PRIVATE
            "${SDL_I686_DIR}/lib"
    )

    file(COPY "${SDL_I686_DIR}/bin/SDL3.dll" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/build")
endif()

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        SDL3
        stdc++exp
)