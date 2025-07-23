# CMakeList.txt: proyecto de CMake para OpenGL Windows, incluya el origen y defina
# la lógica específica del proyecto aquí.
#
cmake_minimum_required (VERSION 3.8)

# Add vendor headers & engine
include_directories(
	"vendor/glad/include/" 
	"vendor/glfw/include/" 
	"vendor/glm/" 
	"vendor/imgui/" 
	"vendor/imgui/backend/"
	"vendor/" 
	"engine/core/" 
	"engine/utils/" 
)

# Add other files
add_subdirectory ("engine")

# To compile a specific exercise: replace line 21
add_executable (CMakeTarget 
	# Only ONE exercise.CPP file should be here
	"excercises/Model importing/ModelImporting_basic.cpp" 

	#vendor files
	# crosspplatform window
	"vendor/glad/src/glad.c"
	# UI...
	"vendor/imgui/imgui.cpp"
	"vendor/imgui/imgui_draw.cpp"
	"vendor/imgui/imgui_tables.cpp"
	"vendor/imgui/imgui_widgets.cpp"
	"vendor/imgui/backend/imgui_impl_glfw.cpp"
	"vendor/imgui/backend/imgui_impl_opengl3.cpp"
	"vendor/imgui/backend/imgui_stdlib.cpp"
)
# If intellisense breaks, go to Visual studio menu (to regenerate): 
#	Proyect => Configure OpenGL Windows


# Add dynamic libs 
target_link_directories (CMakeTarget PUBLIC vendor/glfw/lib-vc2022 vendor/assimp/lib)


# ------IMPORTANT------
# for Assimp, the DLL and PDB files MUST BE IN THE SAME FOLDER AS THE .EXE as well...
#	and make sure that the Lib and DLL filenames match!!

# Add libs & engine library 
target_link_libraries (CMakeTarget glfw3_mt assimp-vc143-mtd engine)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeTarget PROPERTY CXX_STANDARD 20)
endif()
