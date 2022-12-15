project(M2FNR_TEST)
cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

include(ExternalProject)

if(ANDROID)
    set(EXTERNAL_ARGS "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE};-DANDROID_ABI=${ANDROID_ABI}")
endif()

if (DEFINED TARGET_DEVICE)
    list(APPEND EXTERNAL_ARGS "-DTARGET_DEVICE=${TARGET_DEVICE}")
endif()




add_executable(test
    ${SRC_DIR}/test.cpp
    ${SRC_DIR}/spng.cpp

)
target_include_directories(test PRIVATE

    ${CMAKE_BINARY_DIR}/include
)






if( ANDROID )
    find_library(log-lib log)
    find_library(egl-lib EGL)
    target_link_libraries(test GLESv3)
    target_link_libraries(test ${log-lib} ${egl-lib}  ${zlib-lib}  )
elseif( UNIX AND NOT APPLE )
    find_library(egl-lib EGL)

    target_link_libraries(test -lpthread -ldl -lz  ${egl-lib} -lGL  )
endif()
