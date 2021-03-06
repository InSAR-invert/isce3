set(TESTFILES
core/constants.py
core/datetime_.py
core/ellipsoid.py
core/interp1d.py
core/linspace.py
core/LUT1d.py
core/LUT2d.py
core/orbit.py
core/quaternion.py
core/timedelta.py
focus/backproject.py
focus/chirp.py
focus/rangecomp.py
geocode/geocodeSlc.py
geometry/bbox.py
geometry/dem.py
geometry/geocode.py
geometry/geo2rdr.py
geometry/rdr2geo.py
image/resamp.py
io/gdal/dataset.py
io/gdal/raster.py
io/raster.py
signal/covariance.py
product/radargridparameters.py
product/swath.py
)

if(WITH_CUDA)
    list(APPEND TESTFILES
        cuda/core/compute_capability.py
        cuda/core/device.py
        cuda/focus/backproject.py
        cuda/geometry/geo2rdr.py
        cuda/geometry/rdr2geo.py
        cuda/image/resamp.py
        )
endif()

foreach(TESTFILE ${TESTFILES})
    get_filename_component(NAMEWE ${TESTFILE} NAME_WE)
    get_filename_component(DIR ${TESTFILE} DIRECTORY)
    string(REPLACE "/" "." TESTNAME test.python.pybind.${DIR}.${NAMEWE})

    add_test(${TESTNAME} ${PYTEST_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/${TESTFILE})
    set_property(TEST ${TESTNAME} PROPERTY ENVIRONMENT
        LD_LIBRARY_PATH=${PYTEST_LD_LIBRARY_PATH}
        PYTHONPATH=${PYTEST_PYTHONPATH})
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${DIR})
    set_property(TEST ${TESTNAME} PROPERTY WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${DIR})
endforeach()

set_tests_properties(test.python.pybind.geometry.geo2rdr PROPERTIES
             DEPENDS test.python.pybind.geometry.rdr2geo)
if(WITH_CUDA)
    set_tests_properties(test.python.pybind.cuda.geometry.geo2rdr PROPERTIES
                 DEPENDS test.python.pybind.cuda.geometry.rdr2geo)
endif()
