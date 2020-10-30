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
#include "test/unit_test/utils/network_helpers.h"
#include "tnn/utils/dims_vector_utils.h"

namespace TNN_NS {

class NormalizeLayerTest : public LayerTest,
                           public ::testing::WithParamInterface<std::tuple<int, int, int, int, int, DataType>> {};

INSTANTIATE_TEST_SUITE_P(LayerTest, NormalizeLayerTest,
                         ::testing::Combine(BASIC_BATCH_CHANNEL_SIZE,
                                            // p
                                            testing::Values(1, 2),
                                            // axis
                                            testing::Values(1),
                                            // dtype
                                            testing::Values(DATA_TYPE_FLOAT)));

TEST_P(NormalizeLayerTest, NormalizeLayer) {
    // get param
    int batch          = std::get<0>(GetParam());
    int channel        = std::get<1>(GetParam());
    int input_size     = std::get<2>(GetParam());
    int p              = std::get<3>(GetParam());
    int axis           = std::get<4>(GetParam());
    DataType data_type = std::get<5>(GetParam());
    DeviceType dev     = ConvertDeviceType(FLAGS_dt);

    if (data_type == DATA_TYPE_INT8 && DEVICE_ARM != dev) {
        GTEST_SKIP();
    }

    if (batch > 1 || axis != 1 || channel < 2) {
        GTEST_SKIP();
    }

    if (channel > 4 && channel % 4 != 0) {
        GTEST_SKIP();
    }

    // blob desc
    auto inputs_desc  = CreateInputBlobsDesc(batch, channel, input_size, 1, data_type);
    auto outputs_desc = CreateOutputBlobsDesc(1, data_type);

    // param
    NormalizeLayerParam param;
    param.name = "Normalize";
    param.p    = p;
    param.axis = axis;

    Run(LAYER_NORMALIZE, &param, nullptr, inputs_desc, outputs_desc);
}

TEST_P(NormalizeLayerTest, NormalizeLayerWithProto) {
    // get param
    int batch          = std::get<0>(GetParam());
    int channel        = std::get<1>(GetParam());
    int input_size     = std::get<2>(GetParam());
    int p              = std::get<3>(GetParam());
    int axis           = std::get<4>(GetParam());
    DataType data_type = std::get<5>(GetParam());
    DeviceType dev     = ConvertDeviceType(FLAGS_dt);

    if (data_type == DATA_TYPE_INT8 && DEVICE_ARM != dev) {
        GTEST_SKIP();
    }

    if (batch > 1 || axis != 1 || channel < 2) {
        GTEST_SKIP();
    }

    if (channel > 4 && channel % 4 != 0) {
        GTEST_SKIP();
    }

    NormalizeLayerParam param;
    param.p    = p;
    param.axis = axis;

    // generate proto string
    std::vector<int> input_dims = {batch, channel, input_size, input_size};
    std::string head            = GenerateHeadProto({input_dims});
    std::ostringstream ostr;
    ostr << "\""
         << "Normalize layer_name 1 1 input output " << param.across_spatial << " " << param.epsilon << " "
         << param.channel_shared << " " << param.axis << " " << param.p << ",\"";

    std::string proto = head + ostr.str();
    RunWithProto(proto);
}

}  // namespace TNN_NS
