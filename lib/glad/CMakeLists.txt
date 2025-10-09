# Custom GLAD Cmake

# Add Glad as a library
add_library(glad glad.c)
# Setup include libs
target_include_directories(glad PUBLIC include/)
