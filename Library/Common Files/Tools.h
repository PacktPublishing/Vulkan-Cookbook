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
// Tools

#ifndef TOOLS
#define TOOLS

#include "Common.h"

namespace VulkanCookbook {

  using Vector3   = std::array<float, 3>;
  using Matrix4x4 = std::array<float, 16>;

  bool GetBinaryFileContents( std::string const          & filename,
                              std::vector<unsigned char> & contents );

  float Deg2Rad( float value );

  float Dot( Vector3 const & left,
             Vector3 const & right );

  Vector3 Cross( Vector3 const & left,
                 Vector3 const & right );

  Vector3 Normalize( Vector3 const & vector );

  Vector3 operator+ ( Vector3 const & left,
                      Vector3 const & right );

  Vector3 operator- ( Vector3 const & left,
                      Vector3 const & right );

  Vector3 operator+ ( float const   & left,
                      Vector3 const & right );

  Vector3 operator- ( float const   & left,
                      Vector3 const & right );

  Vector3 operator+ ( Vector3 const & left,
                      float const   & right );

  Vector3 operator- ( Vector3 const & left,
                      float const   & right );

  Vector3 operator* ( float           left,
                      Vector3 const & right );

  Vector3 operator* ( Vector3 const & left,
                      float           right );

  Vector3 operator* ( Vector3 const   & left,
                      Matrix4x4 const & right );

  Vector3 operator- ( Vector3 const & vector );

  bool operator== ( Vector3 const & left,
                    Vector3 const & right );

  Matrix4x4 operator* ( Matrix4x4 const & left,
                        Matrix4x4 const & right );

} // namespace VulkanCookbook

#endif // TOOLS