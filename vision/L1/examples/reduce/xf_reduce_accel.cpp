/*
 * Copyright 2022 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "xf_reduce_accel_config.h"

static constexpr int __XF_DEPTH = (HEIGHT * WIDTH * (XF_PIXELWIDTH(IN_TYPE, NPPCX)) / 8) / (INPUT_PTR_WIDTH / 8);

void reduce_accel(ap_uint<INPUT_PTR_WIDTH>* img_in,
                  unsigned char dimension,
                  ap_uint<OUTPUT_PTR_WIDTH>* img_out,
                  int height,
                  int width) {
// clang-format off
    #pragma HLS INTERFACE m_axi      port=img_in        offset=slave  bundle=gmem0 depth=__XF_DEPTH
	#pragma HLS INTERFACE m_axi      port=img_out       offset=slave  bundle=gmem1 depth=128

    #pragma HLS INTERFACE s_axilite  port=return		      bundle=control
    // clang-format on

    xf::cv::Mat<IN_TYPE, HEIGHT, WIDTH, NPPCX, XF_CV_DEPTH_IN> imgInput(height, width);
    xf::cv::Mat<OUT_TYPE, ONE_D_HEIGHT, ONE_D_WIDTH, XF_NPPC1, XF_CV_DEPTH_OUT> imgOutput(ONE_D_HEIGHT, ONE_D_WIDTH);

#pragma HLS DATAFLOW

    // Retrieve xf::cv::Mat objects from img_in data:
    xf::cv::Array2xfMat<INPUT_PTR_WIDTH, IN_TYPE, HEIGHT, WIDTH, NPPCX, XF_CV_DEPTH_IN>(img_in, imgInput);

    // Run xfOpenCV kernel:
    xf::cv::reduce<REDUCTION_OP, IN_TYPE, OUT_TYPE, HEIGHT, WIDTH, ONE_D_HEIGHT, ONE_D_WIDTH, NPPCX, XF_CV_DEPTH_IN,
                   XF_CV_DEPTH_OUT>(imgInput, imgOutput, dimension);
    // Convert _dst xf::cv::Mat object to output array:
    xf::cv::xfMat2Array<OUTPUT_PTR_WIDTH, OUT_TYPE, ONE_D_HEIGHT, ONE_D_WIDTH, XF_NPPC1, XF_CV_DEPTH_OUT>(imgOutput,
                                                                                                          img_out);

    return;
} // End of kernel
