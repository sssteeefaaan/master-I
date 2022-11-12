cmake_minimum_required(VERSION 3.5)
project(MatrixMultiplication)

find_package(MPI REQUIRED)
find_package(OpenMP REQUIRED)
find_package(HDF5)

# additional options
option(ENABLE_HDF5 "Enable HDF5 support." ON)

set(CMAKE_C_STANDARD 11)
set(SOURCE_FILES main.c)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MPI_C_COMPILE_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MPI_C_LINK_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

add_executable(MatrixMultiplication ${SOURCE_FILES})
include_directories(${MPI_C_INCLUDE_PATH})
target_link_libraries(MatrixMultiplication ${MPI_C_LIBRARIES})

if(ENABLE_HDF5)
    if(HDF5_FOUND)
        include_directories(${HDF5_INCLUDE_DIR})
        target_link_libraries(MatrixMultiplication ${HDF5_C_LIBRARIES})
        set(HDF5_SOURCE_FILES utils/h5defs.h utils/h5_matrix_utils.c utils/h5_matrix_utils.h)
        target_sources(MatrixMultiplication PUBLIC ${HDF5_SOURCE_FILES})
    else()
        message(FATAL_ERROR "HDF5 support has been requested, but no HDF5 library was found!")
    endif()
else()
    add_definitions(-DDISABLE_HDF5)
    message(STATUS "HDF5 support disabled.")
endif()
