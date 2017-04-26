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
// Recipe:  13 Updating descriptor sets

#ifndef UPDATING_DESCRIPTOR_SETS
#define UPDATING_DESCRIPTOR_SETS

#include "Common.h"

namespace VulkanCookbook {

  struct ImageDescriptorInfo {
    VkDescriptorSet                     TargetDescriptorSet;
    uint32_t                            TargetDescriptorBinding;
    uint32_t                            TargetArrayElement;
    VkDescriptorType                    TargetDescriptorType;
    std::vector<VkDescriptorImageInfo>  ImageInfos;
  };

  struct BufferDescriptorInfo {
    VkDescriptorSet                     TargetDescriptorSet;
    uint32_t                            TargetDescriptorBinding;
    uint32_t                            TargetArrayElement;
    VkDescriptorType                    TargetDescriptorType;
    std::vector<VkDescriptorBufferInfo> BufferInfos;
  };

  struct TexelBufferDescriptorInfo {
    VkDescriptorSet                     TargetDescriptorSet;
    uint32_t                            TargetDescriptorBinding;
    uint32_t                            TargetArrayElement;
    VkDescriptorType                    TargetDescriptorType;
    std::vector<VkBufferView>           TexelBufferViews;
  };

  struct CopyDescriptorInfo {
    VkDescriptorSet     TargetDescriptorSet;
    uint32_t            TargetDescriptorBinding;
    uint32_t            TargetArrayElement;
    VkDescriptorSet     SourceDescriptorSet;
    uint32_t            SourceDescriptorBinding;
    uint32_t            SourceArrayElement;
    uint32_t            DescriptorCount;
  };

  void UpdateDescriptorSets( VkDevice                                       logical_device,
                             std::vector<ImageDescriptorInfo> const       & image_descriptor_infos,
                             std::vector<BufferDescriptorInfo> const      & buffer_descriptor_infos,
                             std::vector<TexelBufferDescriptorInfo> const & texel_buffer_descriptor_infos,
                             std::vector<CopyDescriptorInfo> const        & copy_descriptor_infos );

} // namespace VulkanCookbook

#endif // UPDATING_DESCRIPTOR_SETS