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
// Chapter: 05 Descriptor Sets
// Recipe:  15 Creating descriptors with a texture and a uniform buffer

#include "05 Descriptor Sets/03 Creating a combined image sampler.h"
#include "05 Descriptor Sets/07 Creating a uniform buffer.h"
#include "05 Descriptor Sets/10 Creating a descriptor set layout.h"
#include "05 Descriptor Sets/11 Creating a descriptor pool.h"
#include "05 Descriptor Sets/12 Allocating descriptor sets.h"
#include "05 Descriptor Sets/13 Updating descriptor sets.h"
#include "05 Descriptor Sets/15 Creating descriptors with a texture and a uniform buffer.h"

namespace VulkanCookbook {

  bool CreateDescriptorsWithTextureAndUniformBuffer( VkPhysicalDevice               physical_device,
                                                     VkDevice                       logical_device,
                                                     VkExtent3D                     sampled_image_size,
                                                     uint32_t                       uniform_buffer_size,
                                                     VkSampler                    & sampler,
                                                     VkImage                      & sampled_image,
                                                     VkDeviceMemory               & sampled_image_memory_object,
                                                     VkImageView                  & sampled_image_view,
                                                     VkBuffer                     & uniform_buffer,
                                                     VkDeviceMemory               & uniform_buffer_memory_object,
                                                     VkDescriptorSetLayout        & descriptor_set_layout,
                                                     VkDescriptorPool             & descriptor_pool,
                                                     std::vector<VkDescriptorSet> & descriptor_sets ) {

    if( !CreateCombinedImageSampler( physical_device, logical_device, VK_IMAGE_TYPE_2D, VK_FORMAT_R8G8B8A8_UNORM, sampled_image_size, 1, 1, VK_IMAGE_USAGE_TRANSFER_DST_BIT,
      false, VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT, VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT,
      VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_ADDRESS_MODE_REPEAT, 0.0f, false, 1.0f, false, VK_COMPARE_OP_ALWAYS, 0.0f, 0.0f, VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK, false,
      sampler, sampled_image, sampled_image_memory_object, sampled_image_view ) ) {
      return false;
    }

    if( !CreateUniformBuffer( physical_device, logical_device, uniform_buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT, uniform_buffer, uniform_buffer_memory_object ) ) {
      return false;
    }

    std::vector<VkDescriptorSetLayoutBinding> bindings = {
      {
        0,                                                          // uint32_t             binding
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,                  // VkDescriptorType     descriptorType
        1,                                                          // uint32_t             descriptorCount
        VK_SHADER_STAGE_FRAGMENT_BIT,                               // VkShaderStageFlags   stageFlags
        nullptr                                                     // const VkSampler    * pImmutableSamplers
      },
      {
        1,                                                          // uint32_t             binding
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,                          // VkDescriptorType     descriptorType
        1,                                                          // uint32_t             descriptorCount
        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,  // VkShaderStageFlags   stageFlags
        nullptr                                                     // const VkSampler    * pImmutableSamplers
      }
    };
    if( !CreateDescriptorSetLayout( logical_device, bindings, descriptor_set_layout ) ) {
      return false;
    }

    std::vector<VkDescriptorPoolSize> descriptor_types = {
      {
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,                  // VkDescriptorType     type
        1                                                           // uint32_t             descriptorCount
      },
      {
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,                          // VkDescriptorType     type
        1                                                           // uint32_t             descriptorCount
      }
    };
    if( !CreateDescriptorPool( logical_device, false, 1, descriptor_types, descriptor_pool ) ) {
      return false;
    }

    if( !AllocateDescriptorSets( logical_device, descriptor_pool, { descriptor_set_layout }, descriptor_sets ) ) {
      return false;
    }

    std::vector<ImageDescriptorInfo> image_descriptor_infos = {
      {
        descriptor_sets[0],                         // VkDescriptorSet                      TargetDescriptorSet
        0,                                          // uint32_t                             TargetDescriptorBinding
        0,                                          // uint32_t                             TargetArrayElement
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,  // VkDescriptorType                     TargetDescriptorType
        {                                           // std::vector<VkDescriptorImageInfo>   ImageInfos
          {
            sampler,                                  // VkSampler                            sampler
            sampled_image_view,                       // VkImageView                          imageView
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL  // VkImageLayout                        imageLayout
          }
        }
      }
    };

    std::vector<BufferDescriptorInfo> buffer_descriptor_infos = {
      {
        descriptor_sets[0],                         // VkDescriptorSet                      TargetDescriptorSet
        1,                                          // uint32_t                             TargetDescriptorBinding
        0,                                          // uint32_t                             TargetArrayElement
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,          // VkDescriptorType                     TargetDescriptorType
        {                                           // std::vector<VkDescriptorBufferInfo>  BufferInfos
          {
            uniform_buffer,                           // VkBuffer                             buffer
            0,                                        // VkDeviceSize                         offset
            VK_WHOLE_SIZE                             // VkDeviceSize                         range
          }
        }
      }
    };

    UpdateDescriptorSets( logical_device, image_descriptor_infos, buffer_descriptor_infos, {}, {} );
    return true;
  }

} // namespace VulkanCookbook
