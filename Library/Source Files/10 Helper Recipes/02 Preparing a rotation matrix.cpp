// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and / or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The below copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
// Vulkan Cookbook
// ISBN: 9781786468154
// © Packt Publishing Limited
//
// Author:   Pawel Lapinski
// LinkedIn: https://www.linkedin.com/in/pawel-lapinski-84522329
//
// Chapter: 10 Helper Recipes
// Recipe:  02 Preparing a rotation matrix

#include "10 Helper Recipes/02 Preparing a rotation matrix.h"

namespace VulkanCookbook {

  Matrix4x4 PrepareRotationMatrix( float           angle,
                                   Vector3 const & axis,
                                   float           normalize_axis ) {
    float x;
    float y;
    float z;

    if( normalize_axis ) {
      Vector3 normalized = Normalize( axis );
      x = normalized[0];
      y = normalized[1];
      z = normalized[2];
    } else {
      x = axis[0];
      y = axis[1];
      z = axis[2];
    }

    const float c = cos( Deg2Rad( angle ) );
    const float _1_c = 1.0f - c;
    const float s = sin( Deg2Rad( angle ) );

    Matrix4x4 rotation_matrix = {
      x * x * _1_c + c,
      y * x * _1_c - z * s,
      z * x * _1_c + y * s,
      0.0f,

      x * y * _1_c + z * s,
      y * y * _1_c + c,
      z * y * _1_c - x * s,
      0.0f,

      x * z * _1_c - y * s,
      y * z * _1_c + x * s,
      z * z * _1_c + c,
      0.0f,

      0.0f,
      0.0f,
      0.0f,
      1.0f
    };
    return rotation_matrix;
  }

} // namespace VulkanCookbook
