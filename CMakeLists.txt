cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

# CMP0074 (CMake 3.12+)
# find_package() uses <PackageName>_ROOT variables.
# This is helpful for us when using packages non-standard install locations.
if (POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()
# CMP0076 (CMake 3.13+)
# The target_sources() command converts relative paths to absolute.
# We're indifferent, so this quiets the warnings.
if (POLICY CMP0076)
  cmake_policy(SET CMP0076 NEW)
endif()

project(isce3
    VERSION   0.1.0
    LANGUAGES CXX
    )

find_program(CCACHE ccache)
if(CCACHE)
    message(STATUS "Using ccache: ${CCACHE}")
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
    set(CMAKE_CUDA_COMPILER_LAUNCHER ${CCACHE})
endif()

set(ISCE_CUDA_ARCHS "Auto" CACHE STRING
    "Select target CUDA device architecture, options are:
    - comma-separated compute capabilities (e.g. 3.5,5.0,5.2)
    - \"Auto\" to detect installed CUDA devices and target those architectures
    - \"\" (empty) to use default compilation options")

# add local .cmake directory to CMAKE_MODULE_PATH
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/.cmake)

# Set the name for the isce3 C++ library.
set(LISCE ${PROJECT_NAME})

# import some helper functions
include(IsceCudaHelper)

# Auto-detect cuda support by default
if(NOT DEFINED WITH_CUDA)
    set(WITH_CUDA Auto CACHE STRING "")
endif()
if(WITH_CUDA STREQUAL Auto)
    include(CheckLanguage)
    check_language(CUDA)
    if(CMAKE_CUDA_COMPILER)
        set(WITH_CUDA ON CACHE STRING "" FORCE)
    else()
        set(WITH_CUDA OFF CACHE STRING "" FORCE)
    endif()
endif()
if (WITH_CUDA)
    enable_language(CUDA)

    # check CUDA version
    set(CUDA_VERSION ${CMAKE_CUDA_COMPILER_VERSION})
    if (CUDA_VERSION VERSION_LESS 9)
        message(FATAL_ERROR "CUDA version must be at least 9. Detected ${CUDA_VERSION}")
    endif()

    # specify target CUDA device architecture(s)
    set_cuda_arch_flags("${ISCE_CUDA_ARCHS}")

    # Set the name for the isce3 CUDA library.
    set(LISCECUDA ${PROJECT_NAME}-cuda)
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo;Coverage" CACHE STRING "" FORCE)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()
if(NOT CMAKE_BUILD_TYPE IN_LIST CMAKE_CONFIGURATION_TYPES)
    message(FATAL_ERROR "Unsupported build type '${CMAKE_BUILD_TYPE}' "
                        "(must be one of ${CMAKE_CONFIGURATION_TYPES})")
endif()

if(CMAKE_BUILD_TYPE STREQUAL Coverage)
    message("Compiling with code coverage reporting.")
    include(CodeCoverage)
else()
    # Define a dummy function so we don't have to keep checking BUILD_TYPE
    function(SetCoverageOptions target)
    endfunction()
endif()

###Ensure tracking is own for testing
enable_testing()

###Include custom installation paths and checks for the project
include(ConfigISCE)

###Explicit check to prevent in-source builds
AssureOutOfSourceBuilds()

####Check CXX Version and Standard to C++17
CheckCXX()

# Define some utilities for creating cython modules
include(useCython)

###Layout same install directory structure as pyre
include(GNUInstallDirs)
InitInstallDirLayout()

# Dependencies

option(ISCE3_FETCH_DEPS "Fetch external dependencies at build time" ON)
include(CMakeDependentOption)
cmake_dependent_option(ISCE3_FETCH_EIGEN "Fetch Eigen at build time" ON
                       "ISCE3_FETCH_DEPS" OFF)
cmake_dependent_option(ISCE3_FETCH_GTEST "Fetch googletest at build time" ON
                       "ISCE3_FETCH_DEPS" OFF)

include(.cmake/FetchExternRepo.cmake)

add_subdirectory(extern)
getpackage_cereal()
getpackage_eigen()
getpackage_fftw()
getpackage_gdal()
getpackage_googletest()
getpackage_hdf5()
getpackage_openmp_optional()
getpackage_pyre()

# These packages required only for the python API. getpackage_python() should
# be executed first in order to ensure a sufficient version of Python is used.
getpackage_python()
getpackage_cython()
getpackage_numpy()
getpackage_pybind11()

add_subdirectory(bin)
add_subdirectory(cxx)    # Core C++ library
add_subdirectory(python) # Python bindings
add_subdirectory(tests)  # Unit tests
add_subdirectory(share)  # Examples
add_subdirectory(doc)    # Documentation

configure_file(
    doc/doxygen/Doxyfile.in
    doc/doxygen/Doxyfile
    )
configure_file(
    doc/sphinx/conf.py.in
    doc/sphinx/conf.py
    )

set(ISCE3_CMAKE_DIR "share/cmake/isce3" CACHE STRING
    "Install directory for cmake files, relative to install prefix"
    )
install(EXPORT isce3-targets
        NAMESPACE ISCE3::
        DESTINATION ${ISCE3_CMAKE_DIR}
        )
include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/.cmake/isce3-config.cmake.in
    ${PROJECT_BINARY_DIR}/isce3-config.cmake
    INSTALL_DESTINATION ${ISCE3_CMAKE_DIR})
install(FILES ${PROJECT_BINARY_DIR}/isce3-config.cmake
        DESTINATION ${ISCE3_CMAKE_DIR})
