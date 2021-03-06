/*Copyright (c) 2018 PaddlePaddle Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#pragma once

#include <string>
#include <vector>
#include "ngraph/ngraph.hpp"
#include "paddle/fluid/platform/ngraph_helper.h"

namespace paddle {
namespace operators {
namespace ngraphs {

void BuildFillConstantNode(
    const std::shared_ptr<paddle::framework::OperatorBase>& op,
    std::shared_ptr<
        std::unordered_map<std::string, std::shared_ptr<ngraph::Node>>>
        ngb_node_map) {
  auto op_attrs = paddle::framework::AttrReader(op->Attrs());
  auto vsp = op_attrs.Get<std::vector<int64_t>>("shape");
  ngraph::Shape shape;
  for (auto& sp : vsp) {
    shape.push_back(sp);
  }
  float value = op_attrs.Get<float>("value");
  ngraph::element::Type ng_dtype;
  auto data_type = static_cast<paddle::framework::proto::VarType::Type>(
      op_attrs.Get<int>("dtype"));
  if (data_type == paddle::framework::proto::VarType::FP32) {
    ng_dtype = ngraph::element::f32;
  } else if (data_type == paddle::framework::proto::VarType::FP64) {
    ng_dtype = ngraph::element::f64;
  } else if (data_type == paddle::framework::proto::VarType::INT64) {
    ng_dtype = ngraph::element::i64;
  } else if (data_type == paddle::framework::proto::VarType::INT32) {
    ng_dtype = ngraph::element::i32;
  } else {
    PADDLE_THROW("unsupported data type: %s", data_type);
  }
  auto out = ngraph::op::Constant::create(ng_dtype, shape, {value});
  paddle::platform::SetOutputNode(op, "Out", out, ngb_node_map);
}
}  // namespace ngraphs
}  // namespace operators
}  // namespace paddle
