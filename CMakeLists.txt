cmake_minimum_required (VERSION 3.8)

project ("Algorithms")

if (NOT TARGET ImaginaCore)
	add_subdirectory ("../Core" Core)
endif ()

set(SourceFiles "module.cpp" "Perturbation.cpp" "HInfLA.cpp" "MipLA.cpp" "PTWithCompression.cpp")

add_library (Algorithms MODULE ${SourceFiles})
set_target_properties (Algorithms PROPERTIES PREFIX "")
target_link_libraries (Algorithms ImaginaCore)
target_link_libraries (Algorithms ImaginaCommon)

set_target_properties (Algorithms PROPERTIES CXX_STANDARD 20)