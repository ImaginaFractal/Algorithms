#include "PTWithCompression.h"
#include <imagina/output_info_helper.h>
#include <imagina/pixel_management.h>

namespace PTWithCompression {
	const PixelDataInfo *PTWithCompressionEvaluator::GetOutputInfo() {
		IM_GET_OUTPUT_INFO_IMPL(Output, Value);
	}

	void PTWithCompressionEvaluator::Prepare(const real_hp &x, const real_hp &y, real_hr radius, const StandardEvaluationParameters &parameters) {
		this->parameters = parameters;
		HPComplex C = HPComplex(x, y);
		HPComplex Z = C;
		complex z = complex(Z), dzdc = 1.0;

		ReferenceCompressor compressor(reference, complex(C));

		size_t i = 1;
		while (i < parameters.Iterations) {
			compressor.Add(z);

			dzdc = 2.0 * z * dzdc + 1.0;
			Z = Z * Z + C;
			z = complex(Z);
			i++;

			if (radius * chebyshev_norm(dzdc) * 2.0 > chebyshev_norm(z)) break;
			if (norm(z) > 16.0) break;
		};

		compressor.Finalize(z);
	}

	void PTWithCompressionEvaluator::Evaluate(IRasterizingInterface rasterizingInterface) {
		real_hr x, y;
		while (rasterizingInterface.GetPixel(x, y)) {
			complex dc = { real(x), real(y) };
			complex Z = 0.0, z = 0.0, dz = 0.0;

			ReferenceDecompressor decompressor(reference);

			uint_iter i = 0;
			while (i < parameters.Iterations) {
				dz = dz * (Z + z) + dc;
				i++;

				Z = decompressor.Next();
				z = Z + dz;

				if (norm(z) > 4096.0) break;

				if (decompressor.End() || norm(z) < norm(dz)) {
					Z = decompressor.Reset();
					dz = z;
				}
			}

			Output output;
			output.Value = i;

			rasterizingInterface.WriteResults(&output);
		}
	}
}