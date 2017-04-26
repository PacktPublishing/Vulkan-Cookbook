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

#ifndef ORBITING_CAMERA
#define ORBITING_CAMERA

#include "Camera.h"

namespace VulkanCookbook {

  class OrbitingCamera : public Camera {
  public:
    virtual Vector3     GetTarget() const final;
    virtual float       GetDistance() const final;
    virtual float       GetHorizontalAngle() const final;
    virtual float       GetVerticalAngle() const final;

    void                ChangeDistance( float distance_delta );
    void                RotateHorizontally( float angle_delta );
    void                RotateVertically( float angle_delta );

              OrbitingCamera();
              OrbitingCamera( Vector3 const & target,
                              float           distance,
                              float           horizontal_angle = 0.0f,
                              float           vertical_angle = 0.0f );
              OrbitingCamera( OrbitingCamera const & other );
    virtual  ~OrbitingCamera();

    OrbitingCamera& operator=( OrbitingCamera const &i_OrbitingCamera );

  private:
    Vector3             Target;
    float               Distance;
    float               HorizontalAngle;
    float               VerticalAngle;
  };

} // namespace VulkanCookbook

#endif // ORBITING_CAMERA