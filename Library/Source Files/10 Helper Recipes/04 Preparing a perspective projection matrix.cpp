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
// Recipe:  04 Preparing a perspective projection matrix

#include "10 Helper Recipes/04 Preparing a perspective projection matrix.h"

namespace VulkanCookbook {

  Matrix4x4 PreparePerspectiveProjectionMatrix( float aspect_ratio,
                                                float field_of_view,
                                                float near_plane,
                                                float far_plane ) {
    float f = 1.0f / tan( Deg2Rad( 0.5f * field_of_view ) );

    Matrix4x4 perspective_projection_matrix = {
      f / aspect_ratio,
      0.0f,
      0.0f,
      0.0f,

      0.0f,
      -f,
      0.0f,
      0.0f,

      0.0f,
      0.0f,
      far_plane / (near_plane - far_plane),
      -1.0f,
      
      0.0f,
      0.0f,
      (near_plane * far_plane) / (near_plane - far_plane),
      0.0f
    };
    return perspective_projection_matrix;
  }

} // namespace VulkanCookbook
