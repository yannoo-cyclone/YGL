# - Find GLEW library
find_path(GLEW_INCLUDE_DIR GL/glew.h
    ${GLEW_ROOT}/include /usr/local/include /usr/include /opt/local/include
    ${OPENGL_INCLUDE_PATH} ${CMAKE_SOURCE_DIR}/extern/glew/include
)
find_library(GLEW_LIBRARY NAMES GLEW glew glew32 glew64
    PATHS ${GLEW_ROOT}/lib ${GLEW_ROOT}/lib64 /usr/local/lib /usr/lib /usr/lib64
    ${OPENGL_LIBRARY_PATH} ${CMAKE_SOURCE_DIR}/extern/glew/lib
)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLEW DEFAULT_MSG GLEW_LIBRARY GLEW_INCLUDE_DIR)
if(GLEW_FOUND)
    set(GLEW_LIBRARIES ${GLEW_LIBRARY})
    set(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})
endif()
mark_as_advanced(GLEW_LIBRARY GLEW_INCLUDE_DIR)
