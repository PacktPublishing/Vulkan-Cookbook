// MIT License
//
// Copyright( c ) 2017 Packt
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files( the "Software" ), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Vulkan Cookbook
// ISBN: 9781786468154
// © Packt Publishing Limited
//
// Author:   Pawel Lapinski
// LinkedIn: https://www.linkedin.com/in/pawel-lapinski-84522329
//
// Camera

#include "Camera.h"

namespace VulkanCookbook {

  Matrix4x4 Camera::GetMatrix() const {
    if( Dirty ) {
      ViewMatrix = {
        RightVector[0],
        UpVector[0],
        -ForwardVector[0],
        0.0f,

        RightVector[1],
        UpVector[1],
        -ForwardVector[1],
        0.0f,

        RightVector[2],
        UpVector[2],
        -ForwardVector[2],
        0.0f,

        Dot( Position, RightVector ),
        Dot( Position, UpVector ),
        Dot( Position, ForwardVector ),
        1.0f
      };
      Dirty = false;
    }
    return ViewMatrix;
  }

  Vector3 Camera::GetPosition() const {
    return Position;
  }

  Vector3 Camera::GetRightVector() const {
    return RightVector;
  }

  Vector3 Camera::GetUpVector() const {
    return UpVector;
  }

  Vector3 Camera::GetForwardVector() const {
    return ForwardVector;
  }

  Camera::Camera() :
    Camera( { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } ) {
  }

  Camera::Camera( Vector3 const & position,
                  Vector3 const & right_vector,
                  Vector3 const & up_vector,
                  Vector3 const & forward_vector ) :
    ViewMatrix( {} ),
    Position( position ),
    RightVector( right_vector ),
    UpVector( up_vector ),
    ForwardVector( forward_vector ),
    Dirty( true ) {
  }

  Camera::Camera( Camera const & camera ) {
    *this = camera;
  }

  Camera::~Camera() {
  }

  Camera& Camera::operator= ( Camera const & camera ) {
    if( this != &camera ) {
      ViewMatrix = camera.ViewMatrix;
      Position = camera.Position;
      RightVector = camera.RightVector;
      UpVector = camera.UpVector;
      ForwardVector = camera.ForwardVector;
      Dirty = camera.Dirty;
    }
    return *this;
  }

} // namespace VulkanCookbook