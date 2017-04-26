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

#include <fstream>
#include <iostream>
#include <cmath>
#include "Tools.h"

namespace VulkanCookbook {

  bool GetBinaryFileContents( std::string const          & filename,
                              std::vector<unsigned char> & contents ) {
    contents.clear();

    std::ifstream file( filename, std::ios::binary );
    if( file.fail() ) {
      std::cout << "Could not open '" << filename << "' file." << std::endl;
      return false;
    }

    std::streampos begin;
    std::streampos end;
    begin = file.tellg();
    file.seekg( 0, std::ios::end );
    end = file.tellg();

    if( (end - begin) == 0 ) {
      std::cout << "The '" << filename << "' file is empty." << std::endl;
      return false;
    }
    contents.resize( static_cast<size_t>(end - begin) );
    file.seekg( 0, std::ios::beg );
    file.read( reinterpret_cast<char*>(contents.data()), end - begin );
    file.close();

    return true;
  }

  float Deg2Rad( float value ) {
    return value * 0.01745329251994329576923690768489f;
  }

  float Dot( Vector3 const & left,
             Vector3 const & right ) {
    return left[0] * right[0] + left[1] * right[1] + left[2] * right[2];
  }

  Vector3 Cross( Vector3 const & left,
                 Vector3 const & right ) {
    return {
      left[1] * right[2] - left[2] * right[1],
      left[2] * right[0] - left[0] * right[2],
      left[0] * right[1] - left[1] * right[0]
    };
  }

  Vector3 Normalize( Vector3 const & vector ) {
    float length = std::sqrt( vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2] );
    return {
      vector[0] / length,
      vector[1] / length,
      vector[2] / length
    };
  }

  Vector3 operator+ ( Vector3 const & left,
                      Vector3 const & right ) {
    return{
      left[0] + right[0],
      left[1] + right[1],
      left[2] + right[2]
    };
  }

  Vector3 operator- ( Vector3 const & left,
                      Vector3 const & right ) {
    return{
      left[0] - right[0],
      left[1] - right[1],
      left[2] - right[2]
    };
  }

  Vector3 operator+ ( float const   & left,
                      Vector3 const & right ) {
    return {
      left + right[0],
      left + right[1],
      left + right[2]
    };
  }

  Vector3 operator- ( float const   & left,
                      Vector3 const & right ) {
    return {
      left - right[0],
      left - right[1],
      left - right[2]
    };
  }

  Vector3 operator+ ( Vector3 const & left,
                      float const   & right ) {
    return{
      left[0] + right,
      left[1] + right,
      left[2] + right
    };
  }

  Vector3 operator- ( Vector3 const & left,
                      float const   & right ) {
    return {
      left[0] - right,
      left[1] - right,
      left[2] - right
    };
  }

  Vector3 operator* ( float           left,
                      Vector3 const & right ) {
    return {
      left * right[0],
      left * right[1],
      left * right[2]
    };
  }

  Vector3 operator* ( Vector3 const & left,
                      float           right ) {
    return {
      left[0] * right,
      left[1] * right,
      left[2] * right
    };
  }

  Vector3 operator* ( Vector3 const   & left,
                      Matrix4x4 const & right ) {
    return {
      left[0] * right[0] + left[1] * right[1] + left[2] * right[2],
      left[0] * right[4] + left[1] * right[5] + left[2] * right[6],
      left[0] * right[8] + left[1] * right[9] + left[2] * right[10]
    };
  }

  Vector3 operator- ( Vector3 const & vector ) {
    return {
      -vector[0],
      -vector[1],
      -vector[2]
    };
  }

  bool operator== ( Vector3 const & left,
                    Vector3 const & right ) {
    if( (std::abs( left[0] - right[0] ) > 0.00001f) ||
        (std::abs( left[1] - right[1] ) > 0.00001f) ||
        (std::abs( left[2] - right[2] ) > 0.00001f) ) {
      return false;
    } else {
      return true;
    }
  }

  Matrix4x4 operator* ( Matrix4x4 const & left,
                        Matrix4x4 const & right ) {
    return {
      left[0] * right[0] + left[4] * right[1] + left[8] * right[2] + left[12] * right[3],
      left[1] * right[0] + left[5] * right[1] + left[9] * right[2] + left[13] * right[3],
      left[2] * right[0] + left[6] * right[1] + left[10] * right[2] + left[14] * right[3],
      left[3] * right[0] + left[7] * right[1] + left[11] * right[2] + left[15] * right[3],

      left[0] * right[4] + left[4] * right[5] + left[8] * right[6] + left[12] * right[7],
      left[1] * right[4] + left[5] * right[5] + left[9] * right[6] + left[13] * right[7],
      left[2] * right[4] + left[6] * right[5] + left[10] * right[6] + left[14] * right[7],
      left[3] * right[4] + left[7] * right[5] + left[11] * right[6] + left[15] * right[7],

      left[0] * right[8] + left[4] * right[9] + left[8] * right[10] + left[12] * right[11],
      left[1] * right[8] + left[5] * right[9] + left[9] * right[10] + left[13] * right[11],
      left[2] * right[8] + left[6] * right[9] + left[10] * right[10] + left[14] * right[11],
      left[3] * right[8] + left[7] * right[9] + left[11] * right[10] + left[15] * right[11],

      left[0] * right[12] + left[4] * right[13] + left[8] * right[14] + left[12] * right[15],
      left[1] * right[12] + left[5] * right[13] + left[9] * right[14] + left[13] * right[15],
      left[2] * right[12] + left[6] * right[13] + left[10] * right[14] + left[14] * right[15],
      left[3] * right[12] + left[7] * right[13] + left[11] * right[14] + left[15] * right[15]
    };
  }

} // namespace VulkanCookbook
