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
// Recipe:  04 Creating a render pass

#include "06 Render Passes and Framebuffers/01 Specifying attachments descriptions.h"
#include "06 Render Passes and Framebuffers/02 Specifying subpass descriptions.h"
#include "06 Render Passes and Framebuffers/03 Specifying dependencies between subpasses.h"
#include "06 Render Passes and Framebuffers/04 Creating a render pass.h"

namespace VulkanCookbook {

  bool CreateRenderPass( VkDevice                                     logical_device,
                         std::vector<VkAttachmentDescription> const & attachments_descriptions,
                         std::vector<SubpassParameters> const       & subpass_parameters,
                         std::vector<VkSubpassDependency> const     & subpass_dependencies,
                         VkRenderPass                               & render_pass ) {

    SpecifyAttachmentsDescriptions( attachments_descriptions );

    std::vector<VkSubpassDescription> subpass_descriptions;
    SpecifySubpassDescriptions( subpass_parameters, subpass_descriptions );

    SpecifyDependenciesBetweenSubpasses( subpass_dependencies );

    VkRenderPassCreateInfo render_pass_create_info = {
      VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,                // VkStructureType                    sType
      nullptr,                                                  // const void                       * pNext
      0,                                                        // VkRenderPassCreateFlags            flags
      static_cast<uint32_t>(attachments_descriptions.size()),   // uint32_t                           attachmentCount
      attachments_descriptions.data(),                          // const VkAttachmentDescription    * pAttachments
      static_cast<uint32_t>(subpass_descriptions.size()),       // uint32_t                           subpassCount
      subpass_descriptions.data(),                              // const VkSubpassDescription       * pSubpasses
      static_cast<uint32_t>(subpass_dependencies.size()),       // uint32_t                           dependencyCount
      subpass_dependencies.data()                               // const VkSubpassDependency        * pDependencies
    };

    VkResult result = vkCreateRenderPass( logical_device, &render_pass_create_info, nullptr, &render_pass );
    if( VK_SUCCESS != result ) {
      std::cout << "Could not create a render pass." << std::endl;
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
