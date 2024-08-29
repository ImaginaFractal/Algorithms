#pragma once
#include <imagina/evaluator.h>

namespace Perturbation {
	using namespace Imagina;

	class PerturbationEvaluator {
		using real = SRReal;
		using complex = SRComplex;
		struct Output {
			double Value;
		};

		StandardEvaluationParameters parameters;

		uint64_t referenceLength;
		complex *reference = nullptr;

	public:
		const PixelDataInfo *GetOutputInfo();

		void Prepare(const HPReal &x, const HPReal &y, HRReal radius, const StandardEvaluationParameters &parameters);
		void Evaluate(IRasterizingInterface rasterizingInterface);
	};
}

IMPLEMENT_INTERFACE(Perturbation::PerturbationEvaluator, Imagina::IEvaluator);
