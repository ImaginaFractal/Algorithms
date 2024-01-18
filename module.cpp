#include <Imagina/module>
#include "Perturbation"

using namespace Imagina;

constexpr ComponentInfo Components[]{
	ComponentInfo::Evaluator<Perturbation::PerturbationEvaluator>("PerturbationEvaluator", "Sample Perturbation Evaluator"),
};
constexpr ModuleInfo Module("SampleEvaluators", "Sample Evaluators", Components);

extern "C" __declspec(dllexport) const Imagina::ModuleInfo *ImGetModuleInfo() {
	return &Module;
}