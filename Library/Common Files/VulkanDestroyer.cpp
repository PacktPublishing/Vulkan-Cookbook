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
// VulkanDestroyer

#include "VulkanDestroyer.h"

namespace VulkanCookbook {

#define VK_DESTROYER_SPECIALIZATION( type, deleter )  \
  template<>                                          \
  void VkDestroyer<type>::Destroy() {                 \
    if( nullptr != deleter ) {                        \
      deleter( Device, Object, nullptr );             \
    }                                                 \
  }

  void VkDestroyer<VkInstance>::Destroy() {
    if( nullptr != vkDestroyInstance ) {
      vkDestroyInstance( Instance, nullptr );
    }
  }

  void VkDestroyer<VkDevice>::Destroy() {
    if( nullptr != vkDestroyDevice ) {
      vkDestroyDevice( LogicalDevice, nullptr );
    }
  }

  void VkDestroyer<VkSurfaceKHR>::Destroy() {
    if( nullptr != vkDestroySurfaceKHR ) {
      vkDestroySurfaceKHR( Instance, Object, nullptr );
    }
  }

  VK_DESTROYER_SPECIALIZATION( VkSemaphore, vkDestroySemaphore )
  // VK_DESTROYER_SPECIALIZATION( VkCommandBuffer, vkFreeCommandBuffers ) <- command buffers are freed along with the pool
  VK_DESTROYER_SPECIALIZATION( VkFence, vkDestroyFence )
  VK_DESTROYER_SPECIALIZATION( VkDeviceMemory, vkFreeMemory )
  VK_DESTROYER_SPECIALIZATION( VkBuffer, vkDestroyBuffer )
  VK_DESTROYER_SPECIALIZATION( VkImage, vkDestroyImage )
  VK_DESTROYER_SPECIALIZATION( VkEvent, vkDestroyEvent )
  VK_DESTROYER_SPECIALIZATION( VkQueryPool, vkDestroyQueryPool )
  VK_DESTROYER_SPECIALIZATION( VkBufferView, vkDestroyBufferView )
  VK_DESTROYER_SPECIALIZATION( VkImageView, vkDestroyImageView )
  VK_DESTROYER_SPECIALIZATION( VkShaderModule, vkDestroyShaderModule )
  VK_DESTROYER_SPECIALIZATION( VkPipelineCache, vkDestroyPipelineCache )
  VK_DESTROYER_SPECIALIZATION( VkPipelineLayout, vkDestroyPipelineLayout )
  VK_DESTROYER_SPECIALIZATION( VkRenderPass, vkDestroyRenderPass )
  VK_DESTROYER_SPECIALIZATION( VkPipeline, vkDestroyPipeline )
  VK_DESTROYER_SPECIALIZATION( VkDescriptorSetLayout, vkDestroyDescriptorSetLayout )
  VK_DESTROYER_SPECIALIZATION( VkSampler, vkDestroySampler )
  VK_DESTROYER_SPECIALIZATION( VkDescriptorPool, vkDestroyDescriptorPool )
  // VK_DESTROYER_SPECIALIZATION( VkDescriptorSet, vkFreeDescriptorSets ) <- descriptor sets are freed along with the pool
  VK_DESTROYER_SPECIALIZATION( VkFramebuffer, vkDestroyFramebuffer )
  VK_DESTROYER_SPECIALIZATION( VkCommandPool, vkDestroyCommandPool )
  VK_DESTROYER_SPECIALIZATION( VkSwapchainKHR, vkDestroySwapchainKHR )

} // namespace VulkanCookbook
