#include <Imagina/module>
#include <iterator> // For std::size()
#include "Perturbation"

Imagina::ComponentInfo ComponentInfo[]{
	{
		.Name = "PerturbationEvaluator",
		.DisplayName = "Sample Perturbation Evaluator",
		.Create = [](const char *)->Imagina::IAny { return Imagina::IEvaluator(new Perturbation::PerturbationEvaluator); },
		.Type = Imagina::ComponentType::Evaluator,
	}
};

Imagina::ModuleInfo ModuleInfo{
	.Name = "SampleEvaluators",
	.DisplayName = "Sample Evaluators",
	.ComponentCount = std::size(ComponentInfo),
	.Components = ComponentInfo,
};

extern "C" __declspec(dllexport) const Imagina::ModuleInfo *ImGetModuleInfo() {
	return &ModuleInfo;
}