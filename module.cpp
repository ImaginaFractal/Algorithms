#include <Imagina/module>
#include "Perturbation"

using namespace Imagina;

constexpr ComponentInfo Components[]{
	ComponentInfo::Evaluator<Perturbation::PerturbationEvaluator>("PerturbationEvaluator", "Sample Perturbation Evaluator"),
};
constexpr ModuleInfo Module("SampleEvaluators", "Sample Evaluators", Components);

im_api const Imagina::ModuleInfo *ImGetModuleInfo() {
	return &Module;
}