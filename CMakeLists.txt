cmake_minimum_required (VERSION 3.8)

project ("SampleEvaluators")

if (NOT TARGET ImaginaCore)
	add_subdirectory ("../Core" Core)
endif ()

set(SourceFiles "module.cpp" "Perturbation.cpp")

add_library (SampleEvaluators SHARED ${SourceFiles})
set_target_properties (SampleEvaluators PROPERTIES PREFIX "")
set_target_properties (SampleEvaluators PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/modules")
target_link_libraries (SampleEvaluators ImaginaCore)
target_link_libraries (SampleEvaluators ImaginaCommon)

set_target_properties (SampleEvaluators PROPERTIES CXX_STANDARD 20)