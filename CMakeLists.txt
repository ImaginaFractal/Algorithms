cmake_minimum_required (VERSION 3.8)

project ("SampleEvaluators")

if (NOT TARGET ImaginaCore)
	add_subdirectory ("../Core" Core)
endif ()

set(SourceFiles "module.cpp" "Perturbation.cpp" "HInfLA.cpp")

add_library (SampleEvaluators MODULE ${SourceFiles})
set_target_properties (SampleEvaluators PROPERTIES PREFIX "")
target_link_libraries (SampleEvaluators ImaginaCore)
target_link_libraries (SampleEvaluators ImaginaCommon)

set_target_properties (SampleEvaluators PROPERTIES CXX_STANDARD 20)