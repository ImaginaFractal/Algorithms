#pragma once
#include <imagina/evaluator.h>

namespace Perturbation {
	using namespace Imagina;

	class PerturbationEvaluator {
		using real = real_sr;
		using complex = complex_sr;
		struct Output {
			double Value;
		};

		StandardEvaluationParameters parameters;

		uint64_t referenceLength;
		complex *reference = nullptr;

	public:
		const PixelDataInfo *GetOutputInfo();

		void Prepare(const real_hp &x, const real_hp &y, real_hr radius, const StandardEvaluationParameters &parameters);
		void Evaluate(IRasterizingInterface rasterizingInterface);
	};
}

IMPLEMENT_INTERFACE(Perturbation::PerturbationEvaluator, Imagina::IEvaluator);
