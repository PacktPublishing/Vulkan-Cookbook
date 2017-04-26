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
// Camera

#ifndef CAMERA
#define CAMERA

#include "Tools.h"

namespace VulkanCookbook {

  class Camera {
  public:
    virtual Matrix4x4   GetMatrix() const final;
    virtual Vector3     GetPosition() const final;
    virtual Vector3     GetRightVector() const final;
    virtual Vector3     GetUpVector() const final;
    virtual Vector3     GetForwardVector() const final;

  protected:
              Camera();
              Camera( Vector3 const & position,
                      Vector3 const & right_vector,
                      Vector3 const & up_vector,
                      Vector3 const & forward_vector );
              Camera( Camera const & camera );
    virtual  ~Camera() = 0;

    Camera& operator=( Camera const & camera );

    mutable Matrix4x4   ViewMatrix;
    Vector3             Position;
    Vector3             RightVector;
    Vector3             UpVector;
    Vector3             ForwardVector;
    mutable bool        Dirty;
  };

} // namespace VulkanCookbook

#endif // CAMERA