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
// Recipe:  07 Loading a 3D model from an OBJ file

#include "10 Helper Recipes/07 Loading a 3D model from an OBJ file.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace VulkanCookbook {

  namespace {

    void GenerateTangentSpaceVectors( Mesh & mesh );

  }

  bool Load3DModelFromObjFile( char const * filename,
                               bool         load_normals,
                               bool         load_texcoords,
                               bool         generate_tangent_space_vectors,
                               bool         unify,
                               Mesh       & mesh,
                               uint32_t   * vertex_stride ) {
    // Load model
    tinyobj::attrib_t                attribs;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;
    std::string                      error;

    bool result = tinyobj::LoadObj( &attribs, &shapes, &materials, &error, filename );
    if( !result ) {
      std::cout << "Could not open the '" << filename << "' file.";
      if( 0 < error.size() ) {
        std::cout << " " << error;
      }
      std::cout << std::endl;
      return false;
    }

    // Normal vectors and texture coordinates are required to generate tangent and bitangent vectors
    if( !load_normals || !load_texcoords ) {
      generate_tangent_space_vectors = false;
    }

    // Load model data and unify (normalize) its size and position
    float min_x = attribs.vertices[0];
    float max_x = attribs.vertices[0];
    float min_y = attribs.vertices[1];
    float max_y = attribs.vertices[1];
    float min_z = attribs.vertices[2];
    float max_z = attribs.vertices[2];

    mesh = {};
    uint32_t offset = 0;
    for( auto & shape : shapes ) {
      uint32_t part_offset = offset;

      for( auto & index : shape.mesh.indices ) {
        mesh.Data.emplace_back( attribs.vertices[3 * index.vertex_index + 0] );
        mesh.Data.emplace_back( attribs.vertices[3 * index.vertex_index + 1] );
        mesh.Data.emplace_back( attribs.vertices[3 * index.vertex_index + 2] );
        ++offset;

        if( load_normals ) {
          if( attribs.normals.size() == 0 ) {
            std::cout << "Could not load normal vectors data in the '" << filename << "' file.";
            return false;
          } else {
            mesh.Data.emplace_back( attribs.normals[3 * index.normal_index + 0] );
            mesh.Data.emplace_back( attribs.normals[3 * index.normal_index + 1] );
            mesh.Data.emplace_back( attribs.normals[3 * index.normal_index + 2] );
          }
        }

        if( load_texcoords ) {
          if( attribs.texcoords.size() == 0 ) {
            std::cout << "Could not load texture coordinates data in the '" << filename << "' file.";
            return false;
          } else {
            mesh.Data.emplace_back( attribs.texcoords[2 * index.texcoord_index + 0] );
            mesh.Data.emplace_back( attribs.texcoords[2 * index.texcoord_index + 1] );
          }
        }

        if( generate_tangent_space_vectors ) {
          // Insert temporary tangent space vectors data
          for( int i = 0; i < 6; ++i ) {
            mesh.Data.emplace_back( 0.0f );
          }
        }

        if( unify ) {
          if( attribs.vertices[3 * index.vertex_index + 0] < min_x ) {
            min_x = attribs.vertices[3 * index.vertex_index + 0];
          }
          if( attribs.vertices[3 * index.vertex_index + 0] > max_x ) {
            max_x = attribs.vertices[3 * index.vertex_index + 0];
          }
          if( attribs.vertices[3 * index.vertex_index + 1] < min_y ) {
            min_y = attribs.vertices[3 * index.vertex_index + 1];
          }
          if( attribs.vertices[3 * index.vertex_index + 1] > max_y ) {
            max_y = attribs.vertices[3 * index.vertex_index + 1];
          }
          if( attribs.vertices[3 * index.vertex_index + 2] < min_z ) {
            min_z = attribs.vertices[3 * index.vertex_index + 2];
          }
          if( attribs.vertices[3 * index.vertex_index + 2] > max_z ) {
            max_z = attribs.vertices[3 * index.vertex_index + 2];
          }
        }
      }

      uint32_t part_vertex_count = offset - part_offset;
      if( 0 < part_vertex_count ) {
        mesh.Parts.push_back( { part_offset, part_vertex_count } );
      }
    }

    uint32_t stride = 3 + (load_normals ? 3 : 0) + (load_texcoords ? 2 : 0) + (generate_tangent_space_vectors ? 6 : 0);
    if( vertex_stride ) {
      *vertex_stride = stride * sizeof( float );
    }

    if( generate_tangent_space_vectors ) {
      GenerateTangentSpaceVectors( mesh );
    }

    if( unify ) {
      float offset_x = 0.5f * (min_x + max_x);
      float offset_y = 0.5f * (min_y + max_y);
      float offset_z = 0.5f * (min_z + max_z);
      float scale_x = abs( min_x - offset_x ) > abs( max_x - offset_x ) ? abs( min_x - offset_x ) : abs( max_x - offset_x );
      float scale_y = abs( min_y - offset_y ) > abs( max_y - offset_y ) ? abs( min_y - offset_y ) : abs( max_y - offset_y );
      float scale_z = abs( min_z - offset_z ) > abs( max_z - offset_z ) ? abs( min_z - offset_z ) : abs( max_z - offset_z );
      float scale = scale_x > scale_y ? scale_x : scale_y;
      scale = scale_z > scale ? 1.0f / scale_z : 1.0f / scale;

      for( size_t i = 0; i < mesh.Data.size() - 2; i += stride ) {
        mesh.Data[i + 0] = scale * (mesh.Data[i + 0] - offset_x);
        mesh.Data[i + 1] = scale * (mesh.Data[i + 1] - offset_y);
        mesh.Data[i + 2] = scale * (mesh.Data[i + 2] - offset_z);
      }
    }

    return true;
  }

  namespace {

    // Based on:
    // Lengyel, Eric. "Computing Tangent Space Basis Vectors for an Arbitrary Mesh". Terathon Software 3D Graphics Library, 2001.
    // http://www.terathon.com/code/tangent.html

    void CalculateTangentAndBitangent( float const   * normal_data,
                                       Vector3 const & face_tangent,
                                       Vector3 const & face_bitangent,
                                       float         * tangent_data,
                                       float         * bitangent_data ) {
      // Gram-Schmidt orthogonalize
      Vector3 const normal = { normal_data[0], normal_data[1], normal_data[2] };
      Vector3 const tangent = Normalize( face_tangent - normal * Dot( normal, face_tangent ) );

      // Calculate handedness
      float handedness = (Dot( Cross( normal, tangent ), face_bitangent ) < 0.0f) ? -1.0f : 1.0f;

      Vector3 const bitangent = handedness * Cross( normal, tangent );

      tangent_data[0] = tangent[0];
      tangent_data[1] = tangent[1];
      tangent_data[2] = tangent[2];

      bitangent_data[0] = bitangent[0];
      bitangent_data[1] = bitangent[1];
      bitangent_data[2] = bitangent[2];
    }

    void GenerateTangentSpaceVectors( Mesh & mesh ) {
      size_t const normal_offset = 3;
      size_t const texcoord_offset = 6;
      size_t const tangent_offset = 8;
      size_t const bitangent_offset = 11;
      size_t const stride = bitangent_offset + 3;

      for( auto & part : mesh.Parts ) {
        for( size_t i = 0; i < mesh.Data.size(); i += stride * 3 ) {
          size_t i1 = i;
          size_t i2 = i1 + stride;
          size_t i3 = i2 + stride;
          Vector3 const v1 = { mesh.Data[i1], mesh.Data[i1 + 1], mesh.Data[i1 + 2] };
          Vector3 const v2 = { mesh.Data[i2], mesh.Data[i2 + 1], mesh.Data[i2 + 2] };
          Vector3 const v3 = { mesh.Data[i3], mesh.Data[i3 + 1], mesh.Data[i3 + 2] };

          std::array<float, 2> const w1 = { mesh.Data[i1 + texcoord_offset], mesh.Data[i1 + texcoord_offset + 1] };
          std::array<float, 2> const w2 = { mesh.Data[i2 + texcoord_offset], mesh.Data[i2 + texcoord_offset + 1] };
          std::array<float, 2> const w3 = { mesh.Data[i3 + texcoord_offset], mesh.Data[i3 + texcoord_offset + 1] };

          float x1 = v2[0] - v1[0];
          float x2 = v3[0] - v1[0];
          float y1 = v2[1] - v1[1];
          float y2 = v3[1] - v1[1];
          float z1 = v2[2] - v1[2];
          float z2 = v3[2] - v1[2];

          float s1 = w2[0] - w1[0];
          float s2 = w3[0] - w1[0];
          float t1 = w2[1] - w1[1];
          float t2 = w3[1] - w1[1];

          float r = 1.0f / (s1 * t2 - s2 * t1);
          Vector3 face_tangent = { (t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r };
          Vector3 face_bitangent = { (s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r };

          CalculateTangentAndBitangent( &mesh.Data[i1 + normal_offset], face_tangent, face_bitangent, &mesh.Data[i1 + tangent_offset], &mesh.Data[i1 + bitangent_offset] );
          CalculateTangentAndBitangent( &mesh.Data[i2 + normal_offset], face_tangent, face_bitangent, &mesh.Data[i2 + tangent_offset], &mesh.Data[i2 + bitangent_offset] );
          CalculateTangentAndBitangent( &mesh.Data[i3 + normal_offset], face_tangent, face_bitangent, &mesh.Data[i3 + tangent_offset], &mesh.Data[i3 + bitangent_offset] );
        }
      }
    }
  }

} // namespace VulkanCookbook
