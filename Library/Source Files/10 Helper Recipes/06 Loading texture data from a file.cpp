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
// Recipe:  06 Loading texture data from a file

#include "10 Helper Recipes/06 Loading texture data from a file.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace VulkanCookbook {

  bool LoadTextureDataFromFile( char const                 * filename,
                                int                          num_requested_components,
                                std::vector<unsigned char> & image_data,
                                int                        * image_width,
                                int                        * image_height,
                                int                        * image_num_components,
                                int                        * image_data_size ) {
    int width = 0;
    int height = 0;
    int num_components = 0;
    std::unique_ptr<unsigned char, void(*)(void*)> stbi_data( stbi_load( filename, &width, &height, &num_components, num_requested_components ), stbi_image_free );

    if( (!stbi_data) ||
        (0 >= width) ||
        (0 >= height) ||
        (0 >= num_components) ) {
      std::cout << "Could not read image!" << std::endl;
      return false;
    }

    int data_size = width * height * (0 < num_requested_components ? num_requested_components : num_components);
    if( image_data_size ) {
      *image_data_size = data_size;
    }
    if( image_width ) {
      *image_width = width;
    }
    if( image_height ) {
      *image_height = height;
    }
    if( image_num_components ) {
      *image_num_components = num_components;
    }

    image_data.resize( data_size );
    std::memcpy( image_data.data(), stbi_data.get(), data_size );
    return true;
  }

} // namespace VulkanCookbook
