#include <Imagina/module>
#include "Perturbation"
#include "HInfLA"
#include "MipLA"

using namespace Imagina;

constexpr ComponentInfo Components[]{
	ComponentInfo::Evaluator<Perturbation::PerturbationEvaluator>("PerturbationEvaluator", "Sample Perturbation Evaluator"),
	ComponentInfo::Evaluator<HinfLA::HInfLAEvaluator>("HInfLAEvaluator", "Sample HInf LA Evaluator"),
	ComponentInfo::Evaluator<MipLA::MipLAEvaluator>("MipLAEvaluator", "Sample Mip LA Evaluator"),
};
constexpr ModuleInfo Module("SampleEvaluators", "Sample Evaluators", Components);

im_api const Imagina::ModuleInfo *ImGetModuleInfo() {
	return &Module;
}