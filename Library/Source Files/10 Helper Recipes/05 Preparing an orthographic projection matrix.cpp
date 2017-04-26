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
// Recipe:  05 Preparing an orthographic projection matrix

#include "10 Helper Recipes/05 Preparing an orthographic projection matrix.h"

namespace VulkanCookbook {

  Matrix4x4 PrepareOrthographicProjectionMatrix( float left_plane,
                                                 float right_plane,
                                                 float bottom_plane,
                                                 float top_plane,
                                                 float near_plane,
                                                 float far_plane ) {
    Matrix4x4 orthographic_projection_matrix = {
      2.0f / (right_plane - left_plane),
      0.0f,
      0.0f,
      0.0f,

      0.0f,
      2.0f / (bottom_plane - top_plane),
      0.0f,
      0.0f,

      0.0f,
      0.0f,
      1.0f / (near_plane - far_plane),
      0.0f,

      -(right_plane + left_plane) / (right_plane - left_plane),
      -(bottom_plane + top_plane) / (bottom_plane - top_plane),
      near_plane / (near_plane - far_plane),
      1.0f
    };
    return orthographic_projection_matrix;
  }

} // namespace VulkanCookbook
