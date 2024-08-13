#include "Perturbation"
#include <Imagina/output_info_helper>
#include <Imagina/pixel_management>

namespace Perturbation {
	const PixelDataInfo *PerturbationEvaluator::GetOutputInfo() {
		IM_GET_OUTPUT_INFO_IMPL(Output, Value);
	}

	void PerturbationEvaluator::Precompute() {
		delete[] reference;
		reference = new complex[parameters.Iterations + 1];
		HPComplex C = HPComplex(x, y);
		HPComplex Z = C;

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

	void PerturbationEvaluator::Evaluate(IRasterizingInterface rasterizingInterface) {
		HRReal x, y;
		while (rasterizingInterface.GetPixel(x, y)) {
			complex dc = { real(x), real(y) };
			complex Z = 0.0, z = 0.0, dz = 0.0;

			ITUInt i = 0, j = 0;
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

			rasterizingInterface.WriteResults(&output);
		}
	}
}