#include <imagina/module.h>
#include "Perturbation.h"
#include "PTWithCompression.h"
#include "HInfLA.h"
#include "MipLA.h"

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