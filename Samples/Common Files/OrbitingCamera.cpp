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

#include "OrbitingCamera.h"
#include "10 Helper Recipes/02 Preparing a rotation matrix.h"

namespace VulkanCookbook {

  Vector3 OrbitingCamera::GetTarget() const {
    return Target;
  }

  float OrbitingCamera::GetDistance() const {
    return Distance;
  }

  float OrbitingCamera::GetHorizontalAngle() const {
    return HorizontalAngle;
  }

  float OrbitingCamera::GetVerticalAngle() const {
    return VerticalAngle;
  }

  void OrbitingCamera::ChangeDistance( float distance_delta ) {
    Distance -= distance_delta;
    if( Distance < 0.0f ) {
      Distance = 0.0f;
    }
    Position = Target - Distance * ForwardVector;
    Dirty = true;
  }

  void OrbitingCamera::RotateHorizontally( float angle_delta ) {
    Matrix4x4 const rotation = PrepareRotationMatrix( angle_delta, { 0.0f, -1.0f, 0.0f } );

    // Comment by Anastazja:
    //
    // Mama i tata, i brat, i znow brat, i babcia, i dziadek, i wujek, i babcia prabacia, i ciocia.
    // Napisalam to ja z tata.

    HorizontalAngle += angle_delta;
    ForwardVector = ForwardVector * rotation;
    UpVector = UpVector * rotation;
    RightVector = RightVector * rotation;
    Position = Target - Distance * ForwardVector;
    Dirty = true;
  }

  void OrbitingCamera::RotateVertically( float angle_delta ) {
    float const old_angle = VerticalAngle;
    float tmp_delta = angle_delta;

    VerticalAngle += angle_delta;
    if( VerticalAngle > 90.0f ) {
      VerticalAngle = 90.0f;
      tmp_delta = 90.0f - old_angle;
    } else if( VerticalAngle < -90.0f ) {
      VerticalAngle = -90.0f;
      tmp_delta = -90.0f - old_angle;
    }

    Matrix4x4 const rotation = PrepareRotationMatrix( tmp_delta, RightVector );

    ForwardVector = ForwardVector * rotation;
    UpVector = UpVector * rotation;
    Position = Target - Distance * ForwardVector;
    Dirty = true;
  }

  OrbitingCamera::OrbitingCamera() :
    OrbitingCamera( { 0.0f, 0.0f, 0.0f }, 1.0f, 0.0f, 0.0f ) {
  }

  OrbitingCamera::OrbitingCamera( Vector3 const & target,
                                  float           distance,
                                  float           horizontal_angle,
                                  float           vertical_angle ) :
    Camera( target - distance * Vector3{ 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } ),
    Target( target ),
    Distance( distance ),
    HorizontalAngle( 0.0f ),
    VerticalAngle( 0.0f ) {
    RotateHorizontally( horizontal_angle );
    RotateVertically( vertical_angle );
  }

  OrbitingCamera::OrbitingCamera( OrbitingCamera const & camera ) {
    *this = camera;
  }

  OrbitingCamera::~OrbitingCamera() {
  }

  OrbitingCamera& OrbitingCamera::operator=( OrbitingCamera const & camera ) {
    if( this != &camera ) {
      Camera::operator=( camera );
      Target = camera.Target;
      Distance = camera.Distance;
      HorizontalAngle = camera.HorizontalAngle;
      VerticalAngle = camera.VerticalAngle;
    }
    return *this;
  }

} // namespace VulkanCookbook