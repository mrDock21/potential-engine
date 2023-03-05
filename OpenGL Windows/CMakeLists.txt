# CMakeList.txt: proyecto de CMake para OpenGL Windows, incluya el origen y defina
# la lógica específica del proyecto aquí.
#
cmake_minimum_required (VERSION 3.8)

# Add vendor headers & engine
include_directories(
	"vendor/glad/include/" 
	"vendor/glfw/include/" 
	"vendor/glm/" 
	"vendor/"
	"engine/core/"
	"engine/utils/"
)

# Add other files
add_subdirectory ("engine")

# To compile a specific exercise: replace line 21
add_executable (CMakeTarget 
	"excercises/Lights/PointLight.cpp" # Only ONE .CPP file should be here
	"vendor/glad/src/glad.c"
)
# If intellisense breaks, go to Visual studio menu (to regenerate): 
#	Proyect => Configure OpenGL Windows


# Add dynamic libs 
target_link_directories (CMakeTarget PUBLIC vendor/glfw/lib-vc2022)
# Add libs & engine library 
target_link_libraries (CMakeTarget glfw3_mt engine)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeTarget PROPERTY CXX_STANDARD 20)
endif()
