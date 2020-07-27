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

#include "tflite_op_converter.h"

namespace TNN_CONVERTER {
DECLARE_OP_CONVERTER(Concat);

std::string TFLiteConcatConverter::TNNOpType(bool quantized_model) {
    return "Concat";
}

TNN_NS::Status TFLiteConcatConverter::exec(tnn::NetStructure &net_structure, tnn::NetResource &net_resource, const std::unique_ptr<tflite::OperatorT> &tf_lite_operator, const std::vector<std::unique_ptr<tflite::TensorT> > &tf_lite_tensors, const std::vector<std::unique_ptr<tflite::BufferT> > &tf_lite_model_buffer, const std::vector<std::unique_ptr<tflite::OperatorCodeT> > &tf_lite_op_set, bool quantized_model) {
    auto param = new TNN_NS::ConcatLayerParam;
    auto cur_layer =  net_structure.layers.back();
    cur_layer->param = std::shared_ptr<TNN_NS::LayerParam>(param);
    auto tf_lite_op_type             = tf_lite_op_set[tf_lite_operator->opcode_index]->builtin_code;
    const auto& reshape_option       = tf_lite_operator->builtin_options.AsReshapeOptions();
    param->name      = cur_layer->name;
    param->type      = cur_layer->type_str;
    param->quantized = false;
    auto option = tf_lite_operator->builtin_options.AsConcatenationOptions();
    if (quantized_model) {
        // TODO
    } else {
        param->axis = option->axis;
    }
    return TNN_NS::TNN_CONVERT_OK;
}


using namespace tflite;
REGISTER_CONVERTER(Concat, BuiltinOperator_CONCATENATION);

}