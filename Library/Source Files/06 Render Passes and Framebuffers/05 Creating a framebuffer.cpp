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
// Chapter: 06 Render Passes and Framebuffers
// Recipe:  05 Creating a framebuffer

#include "06 Render Passes and Framebuffers/05 Creating a framebuffer.h"

namespace VulkanCookbook {

  bool CreateFramebuffer( VkDevice                         logical_device,
                          VkRenderPass                     render_pass,
                          std::vector<VkImageView> const & attachments,
                          uint32_t                         width,
                          uint32_t                         height,
                          uint32_t                         layers,
                          VkFramebuffer                  & framebuffer ) {
    VkFramebufferCreateInfo framebuffer_create_info = {
      VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,    // VkStructureType              sType
      nullptr,                                      // const void                 * pNext
      0,                                            // VkFramebufferCreateFlags     flags
      render_pass,                                  // VkRenderPass                 renderPass
      static_cast<uint32_t>(attachments.size()),    // uint32_t                     attachmentCount
      attachments.data(),                           // const VkImageView          * pAttachments
      width,                                        // uint32_t                     width
      height,                                       // uint32_t                     height
      layers                                        // uint32_t                     layers
    };

    VkResult result = vkCreateFramebuffer( logical_device, &framebuffer_create_info, nullptr, &framebuffer );
    if( VK_SUCCESS != result ) {
      std::cout << "Could not create a framebuffer." << std::endl;
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
