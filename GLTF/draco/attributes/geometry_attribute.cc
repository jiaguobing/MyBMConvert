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
#include "draco/attributes/geometry_attribute.h"

using std::array;

namespace draco {

GeometryAttribute::GeometryAttribute()
    

void GeometryAttribute::Init(GeometryAttribute::Type attribute_type,
                             DataBuffer *buffer, int8_t num_components,
                             DataType data_type, bool normalized,
                             int64_t byte_stride, int64_t byte_offset) 

bool GeometryAttribute::CopyFrom(const GeometryAttribute &src_att) 

bool GeometryAttribute::operator==(const GeometryAttribute &va) const 

void GeometryAttribute::ResetBuffer(DataBuffer *buffer, int64_t byte_stride,
                                    int64_t byte_offset)

}  // namespace draco
