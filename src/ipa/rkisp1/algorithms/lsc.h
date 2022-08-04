/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2021-2022, Ideas On Board
 *
 * lsc.h - RkISP1 Lens Shading Correction control
 */

#pragma once

#include "algorithm.h"

namespace libcamera {

namespace ipa::rkisp1::algorithms {

class LensShadingCorrection : public Algorithm
{
public:
	LensShadingCorrection();
	~LensShadingCorrection() = default;

	int init(IPAContext &context, const YamlObject &tuningData) override;
	void prepare(IPAContext &context, unsigned int frame,
		     RKISP1FrameContext &frameContext,
		     rkisp1_params_cfg *params) override;

private:
	bool initialized_;

	std::vector<uint16_t> rData_;
	std::vector<uint16_t> grData_;
	std::vector<uint16_t> gbData_;
	std::vector<uint16_t> bData_;

	std::vector<double> xSize_;
	std::vector<double> ySize_;
};

} /* namespace ipa::rkisp1::algorithms */
} /* namespace libcamera */
