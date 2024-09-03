#pragma once
#include <imagina/evaluator.h>
#include <vector>

namespace MipLA {
	using namespace Imagina;

	inline real_sr magnitude(const SRComplex &z) {
		//return std::abs(z);
		return chebyshev_norm(z);
	}

	struct LAStep {
		using real = real_sr;
		using complex = SRComplex;

		static constexpr real ValidRadiusScale = 0x1.0p-24;
		static constexpr real InitialValidRadius = std::numeric_limits<real>::infinity();

		complex A, B;
		real ValidRadius, ValidRadiusC;

		LAStep() = default;
		LAStep(const LAStep &) = default;
		LAStep(LAStep &&) = default;

		explicit LAStep(complex z) : A(2.0 * z), B(1.0), ValidRadius(magnitude(z) * ValidRadiusScale), ValidRadiusC(InitialValidRadius) {}

		LAStep &operator=(const LAStep &) = default;
		LAStep &operator=(LAStep &&) = default;

		LAStep Composite(const LAStep &step) const {
			LAStep result;

			result.ValidRadius = std::min(ValidRadius, step.ValidRadius / magnitude(A));
			result.ValidRadiusC = std::min({ ValidRadiusC, step.ValidRadiusC, step.ValidRadius / magnitude(B) });
			result.A = A * step.A;
			result.B = B * step.A + step.B;

			return result;
		}
	};

	class MipLAEvaluator {
		using real = real_sr;
		using complex = SRComplex;
		struct Output {
			double Value;
		};

		StandardEvaluationParameters parameters;

		uint64_t referenceLength;
		complex *reference = nullptr;
		std::vector<std::vector<LAStep>> LAData;

		void ComputeOrbit(const real_hp &x, const real_hp &y, real_hr radius);
		void CreateLAFromOrbit();
		bool CreateNewLALevel();
		std::pair<LAStep *, size_t> Lookup(size_t i, real norm_dz, real norm_dc);

	public:
		const PixelDataInfo *GetOutputInfo();

		void Prepare(const real_hp &x, const real_hp &y, real_hr radius, const StandardEvaluationParameters &parameters);
		void Evaluate(IRasterizingInterface rasterizingInterface);
	};
}

IMPLEMENT_INTERFACE(MipLA::MipLAEvaluator, Imagina::IEvaluator);
