#include "MipLA"
#include <imagina/output_info_helper.h>
#include <imagina/pixel_management.h>

namespace MipLA {
	const PixelDataInfo *MipLAEvaluator::GetOutputInfo() {
		IM_GET_OUTPUT_INFO_IMPL(Output, Value);
	}

	void MipLAEvaluator::Prepare(const HPReal &x, const HPReal &y, HRReal radius, const StandardEvaluationParameters &parameters) {
		this->parameters = parameters;
		delete[] reference;
		LAData.clear();

		ComputeOrbit(x, y, radius);
		if (referenceLength < 8) return;

		CreateLAFromOrbit();
		while (CreateNewLALevel());
	}

	void MipLAEvaluator::ComputeOrbit(const HPReal &x, const HPReal &y, HRReal radius) {
		reference = new complex[parameters.Iterations + 1];

		HPComplex C = HPComplex(x, y);
		HPComplex Z = C;
		complex z = complex(Z), dzdc = 1.0;

		reference[0] = 0.0;
		reference[1] = z;

		size_t i = 1;
		while (i < parameters.Iterations) {
			dzdc = 2.0 * z * dzdc + 1.0;
			Z = Z * Z + C;
			z = complex(Z);
			i++;

			reference[i] = z;

			if (radius * chebyshev_norm(dzdc) * 2.0 > chebyshev_norm(z)) break;
			if (norm(z) > 16.0) break;
		};

		referenceLength = i;
	}

	void MipLAEvaluator::CreateLAFromOrbit() {
		std::vector<LAStep> &CurrentLevel = LAData.emplace_back();
		CurrentLevel.reserve(referenceLength - 1);

		for (size_t i = 1; i < referenceLength; i++) {
			CurrentLevel.emplace_back(reference[i]);
		}
	}

	bool MipLAEvaluator::CreateNewLALevel() {
		std::vector<LAStep> &CurrentLevel = LAData.emplace_back();
		std::vector<LAStep> &PreviousLevel = LAData[LAData.size() - 2];
		CurrentLevel.reserve((PreviousLevel.size() + 1) / 2);

		for (size_t i = 0; i + 1 < PreviousLevel.size(); i += 2) {
			CurrentLevel.push_back(PreviousLevel[i].Composite(PreviousLevel[i + 1]));
		}
		if (PreviousLevel.size() % 2) {
			CurrentLevel.push_back(PreviousLevel.back());
		}

		return CurrentLevel.size() > 1;
	}

	std::pair<LAStep *, size_t> MipLAEvaluator::Lookup(size_t i, real norm_dz, real norm_dc) {
		if (i == 0 || i >= referenceLength || LAData.empty()) return { nullptr, 0 };

		std::pair<LAStep *, size_t> result = { nullptr, 0 }; // { step, length }
		size_t index = i - 1, length = 1;
		for (auto &level : LAData) {
			if (norm_dz > level[index].ValidRadius || norm_dc > level[index].ValidRadiusC) break;
			result.first = &level[index];
			result.second = length;

			if (index % 2) break;
			index >>= 1;
			length <<= 1;
		}

		result.second = std::min(result.second, referenceLength - i);
		return result;
	}

	void MipLAEvaluator::Evaluate(IRasterizingInterface rasterizingInterface) {
		HRReal x, y;
		while (rasterizingInterface.GetPixel(x, y)) {
			complex dc = { real(x), real(y) };
			complex Z = 0.0, z = 0.0, dz = 0.0;
			real norm_dc = magnitude(dc);

			ITUInt i = 0, j = 0;
			while (i < parameters.Iterations) {
				dz = dz * (Z + z) + dc;
				i++; j++;

				while (i < parameters.Iterations) {
					auto [step, length] = Lookup(j, magnitude(dz), norm_dc);
					if (!step) break;

					dz = dz * step->A + dc * step->B;
					i += length;
					j += length;
				}

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