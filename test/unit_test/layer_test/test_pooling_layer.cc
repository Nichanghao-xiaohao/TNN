// Tencent is pleased to support the open source community by making TNN available.
//
// Copyright (C) 2020 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "test/unit_test/layer_test/layer_test.h"
#include "test/unit_test/unit_test_common.h"

namespace TNN_NS {

class PoolingLayerTest : public LayerTest,
                         public ::testing::WithParamInterface<std::tuple<int, int, int, int, int, int, DataType>> {};

INSTANTIATE_TEST_SUITE_P(LayerTest, PoolingLayerTest,
                         ::testing::Combine(BASIC_BATCH_CHANNEL_SIZE,
                                            // kernel
                                            testing::Values(3, 2),
                                            // stride
                                            testing::Values(1, 2),
                                            // pool type
                                            testing::Values(0, 1),
                                            // datatype
                                            testing::Values(DATA_TYPE_FLOAT, DATA_TYPE_BFP16)));

TEST_P(PoolingLayerTest, PoolingLayer) {
    // get param
    int batch          = std::get<0>(GetParam());
    int channel        = std::get<1>(GetParam());
    int input_size     = std::get<2>(GetParam());
    int kernel         = std::get<3>(GetParam());
    int stride         = std::get<4>(GetParam());
    int pool_type      = std::get<5>(GetParam());
    DataType data_type = std::get<6>(GetParam());
    DeviceType dev     = ConvertDeviceType(FLAGS_dt);
    if (data_type == DATA_TYPE_INT8 && DEVICE_ARM != dev) {
        GTEST_SKIP();
    }

    if (data_type == DATA_TYPE_BFP16 && DEVICE_ARM != dev) {
        GTEST_SKIP();
    }

    // blob desc
    auto inputs_desc  = CreateInputBlobsDesc(batch, channel, input_size, 1, data_type);
    auto outputs_desc = CreateOutputBlobsDesc(1, data_type);

    // param
    PoolingLayerParam param;
    param.name           = "Pooling";
    param.kernels_params = {kernel, kernel};
    param.kernels        = {kernel, kernel};
    param.strides        = {stride, stride};
    if (kernel == 3)
        param.pads = {1, 1, 1, 1};
    else
        param.pads = {0, 0, 0, 0};
    param.pad_type  = -1;
    param.pool_type = pool_type;
    param.kernel_indexs.push_back(-1);
    param.kernel_indexs.push_back(-1);

    Run(LAYER_POOLING, &param, nullptr, inputs_desc, outputs_desc);
}

TEST_P(PoolingLayerTest, PoolingLayerWithProto) {
    // get param
    int batch          = std::get<0>(GetParam());
    int channel        = std::get<1>(GetParam());
    int input_size     = std::get<2>(GetParam());
    int kernel         = std::get<3>(GetParam());
    int stride         = std::get<4>(GetParam());
    int pool_type      = std::get<5>(GetParam());
    DataType data_type = std::get<6>(GetParam());
    DeviceType dev     = ConvertDeviceType(FLAGS_dt);
    if (data_type == DATA_TYPE_INT8 && DEVICE_ARM != dev) {
        GTEST_SKIP();
    }

    if (data_type == DATA_TYPE_BFP16 && DEVICE_ARM != dev) {
        GTEST_SKIP();
    }

    // param
    PoolingLayerParam param;
    param.name           = "Pooling";
    param.kernels_params = {kernel, kernel};
    param.kernels        = {kernel, kernel};
    param.strides        = {stride, stride};
    if (kernel == 3)
        param.pads = {1, 1, 1, 1};
    else
        param.pads = {0, 0, 0, 0};
    param.pad_type  = -1;
    param.pool_type = pool_type;
    param.kernel_indexs.push_back(-1);
    param.kernel_indexs.push_back(-1);

    // generate proto string
    std::vector<int> input_dims = {batch, channel, input_size, input_size};
    std::string head            = GenerateHeadProto({input_dims});
    std::ostringstream ostr;
    ostr << "\""
         << "Pooling layer_name 1 1 input output ";
    ostr << param.pool_type << " ";
    ostr << param.kernels_params[1] << " ";
    ostr << param.kernels_params[0] << " ";
    ostr << param.strides[1] << " ";
    ostr << param.strides[0] << " ";
    ostr << param.pads[2] << " ";
    ostr << param.pads[0] << " ";
    ostr << param.kernel_indexs[1] << " ";
    ostr << param.kernel_indexs[0] << " ";
    ostr << param.pad_type << " ";
    ostr << param.ceil_mode << " ";
    ostr << ",\"";

    Precision precision = PRECISION_AUTO;
    if (DATA_TYPE_BFP16 == data_type) {
        precision = PRECISION_LOW;
    }

    std::string proto = head + ostr.str();
    RunWithProto(proto, precision);
}

}  // namespace TNN_NS
