#include <Imagina/module>
#include "Perturbation"
#include "PTWithCompression"
#include "HInfLA"
#include "MipLA"

using namespace Imagina;

constexpr ComponentInfo Components[]{
	ComponentInfo::Evaluator<Perturbation::PerturbationEvaluator>("PerturbationEvaluator", "Perturbation"),
	ComponentInfo::Evaluator<PTWithCompression::PTWithCompressionEvaluator>("PTWithCompression", "Perturbation With Compression"),
	ComponentInfo::Evaluator<HinfLA::HInfLAEvaluator>("HInfLAEvaluator", "HInf LA"),
	ComponentInfo::Evaluator<MipLA::MipLAEvaluator>("MipLAEvaluator", "Mip LA"),
};
constexpr ModuleInfo Module("Algorithms", "Algorithms", Components);

im_api const Imagina::ModuleInfo *ImGetModuleInfo() {
	return &Module;
}