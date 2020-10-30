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

#ifndef TNN_TEST_UNIT_TEST_COMMON_H_
#define TNN_TEST_UNIT_TEST_COMMON_H_

#include <chrono>
#include <random>
#include <string>
#include <vector>

#include "tnn/core/abstract_device.h"
#include "tnn/core/context.h"
#include "tnn/core/macro.h"
#include "tnn/interpreter/layer_resource.h"
#include "tnn/utils/random_data_utils.h"

namespace TNN_NS {

IntScaleResource* CreateIntScale(int channel);
void SetUpEnvironment(AbstractDevice** cpu, AbstractDevice** device, Context** cpu_context, Context** device_context);

std::string GenerateHeadProto(std::vector<std::vector<int>> input_dims_vec, int output_count = 1);

}  // namespace TNN_NS

#endif  // TNN_TEST_UNIT_TEST_COMMON_H_
