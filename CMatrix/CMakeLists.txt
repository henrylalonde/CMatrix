# CMakeList.txt : CMake project for CMatrix, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (CMatrix "CMatrix.cpp" "CMatrix.h" "help.h" "operations.h" "operations.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMatrix PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
find_package(Eigen3 REQUIRED NO_MODULE)

target_link_libraries(CMatrix PRIVATE Eigen3::Eigen)
