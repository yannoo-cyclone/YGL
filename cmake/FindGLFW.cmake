# - Find GLFW library
find_path(GLFW3_INCLUDE_DIR GLFW/glfw3.h
    ${GLFW3_ROOT}/include /usr/local/include /usr/include /opt/local/include
    ${CMAKE_SOURCE_DIR}/extern/glfw/include
)
find_library(GLFW3_LIBRARY NAMES glfw glfw3 glfw3d
    PATHS ${GLFW3_ROOT}/lib ${GLFW3_ROOT}/lib64 /usr/local/lib /usr/lib /usr/lib64
    ${CMAKE_SOURCE_DIR}/extern/glfw/lib ${CMAKE_SOURCE_DIR}/extern/glfw/lib/cmake/glfw3
)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLFW3 DEFAULT_MSG GLFW3_LIBRARY GLFW3_INCLUDE_DIR)
if(GLFW3_FOUND)
    set(GLFW3_LIBRARIES ${GLFW3_LIBRARY})
    set(GLFW3_INCLUDE_DIRS ${GLFW3_INCLUDE_DIR})
endif()
mark_as_advanced(GLFW3_LIBRARY GLFW3_INCLUDE_DIR)
