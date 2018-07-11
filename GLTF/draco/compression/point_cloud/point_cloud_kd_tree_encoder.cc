// Copyright 2016 The Draco Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include "draco/compression/point_cloud/point_cloud_kd_tree_encoder.h"
#include "draco/compression/attributes/kd_tree_attributes_encoder.h"

namespace draco {

bool PointCloudKdTreeEncoder::EncodeGeometryData() {
  const int32_t num_points = point_cloud()->num_points();
  buffer()->Encode(num_points);
  return true;
}

bool PointCloudKdTreeEncoder::GenerateAttributesEncoder(int32_t att_id) {
  // Currently supported only for single attribute.
  if (att_id == 0) {
    // Create a new attribute encoder only for the first attribute.
    AddAttributesEncoder(std::unique_ptr<AttributesEncoder>(
        new KdTreeAttributesEncoder(att_id)));
    return true;
  }
  return false;
}

}  // namespace draco
