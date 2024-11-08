#include "Perturbation.h"
#include <imagina/output_info_helper.h>
#include <imagina/pixel_management.h>

namespace Perturbation {
	const PixelDataInfo *PerturbationEvaluator::GetOutputInfo() {
		IM_GET_OUTPUT_INFO_IMPL(Output, Value);
	}

	void PerturbationEvaluator::Prepare(const real_hp &x, const real_hp &y, real_hr radius, const StandardEvaluationParameters &parameters) {
		this->parameters = parameters;
		delete[] reference;
		reference = new complex[parameters.Iterations + 1];
		complex_hp C = complex_hp(x, y);
		complex_hp Z = C;

		reference[0] = 0.0;
		reference[1] = complex(Z);

		size_t i = 1;
		while (i < parameters.Iterations) {
			Z = Z * Z + C;
			i++;

			complex z = complex(Z);
			reference[i] = z;

			if (norm(z) > 16.0) break;
		}

		referenceLength = i;
	}

	void PerturbationEvaluator::Evaluate(IRasterizer rasterizer) {
		real_hr x, y;
		while (rasterizer.GetPixel(x, y)) {
			complex dc = { real(x), real(y) };
			complex Z = 0.0, z = 0.0, dz = 0.0;

			uint_iter i = 0, j = 0;
			while (i < parameters.Iterations) {
				dz = dz * (Z + z) + dc;
				i++; j++;

				Z = reference[j];
				z = Z + dz;

				if (norm(z) > 4096.0) break;

				if (j == referenceLength || norm(z) < norm(dz)) {
					Z = real(0.0);
					dz = z;
					j = 0;
				}
			}

			Output output;
			output.Value = i;

			rasterizer.WriteResults(&output);
		}
	}
}