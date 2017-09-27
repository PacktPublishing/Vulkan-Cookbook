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
// Vulkan Destroyer

#ifndef VULKAN_DESTROYER
#define VULKAN_DESTROYER

#include <functional>
#include "VulkanFunctions.h"

namespace VulkanCookbook {

  // VkDestroyer<> - wrapper for automatic object destruction

  // Deleter functions

  template<class VkType>
  void DestroyVulkanObject( VkType object );

  template<>
  inline void DestroyVulkanObject<VkInstance>( VkInstance object ) {
    vkDestroyInstance( object, nullptr );
  }

  template<>
  inline void DestroyVulkanObject<VkDevice>( VkDevice object ) {
    vkDestroyDevice( object, nullptr );
  }

  template<class VkParent, class VkChild>
  void DestroyVulkanObject( VkParent parent, VkChild object );

  template<>
  inline void DestroyVulkanObject<VkInstance, VkSurfaceKHR>( VkInstance instance, VkSurfaceKHR surface ) {
    vkDestroySurfaceKHR( instance, surface, nullptr );
  }

#define VK_DESTROYER_SPECIALIZATION( VkChild, VkDeleter )                                 \
  template<>                                                                              \
  inline void DestroyVulkanObject<VkDevice, VkChild>( VkDevice device, VkChild object ) { \
    VkDeleter( device, object, nullptr );                                                 \
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

  // Class definition

  template<class VkType>
  class VkDestroyer {
  public:
    VkDestroyer() :
      Object( VK_NULL_HANDLE ),
      DestroyerFunction( nullptr ) {
    }

    VkDestroyer( std::function<void( VkType )> destroyer_function ) :
      Object( VK_NULL_HANDLE ),
      DestroyerFunction( destroyer_function ) {
    }

    VkDestroyer( VkType object, std::function<void( VkType )> destroyer_function ) :
      Object( object ),
      DestroyerFunction( destroyer_function ) {
    }

    ~VkDestroyer() {
      if( DestroyerFunction && Object ) {
        DestroyerFunction( Object );
      }
    }

    VkDestroyer( VkDestroyer<VkType> && other ) :
      Object( other.Object ),
      DestroyerFunction( other.DestroyerFunction ) {
      other.Object = VK_NULL_HANDLE;
      other.DestroyerFunction = nullptr;
    }

    VkDestroyer& operator=(VkDestroyer<VkType> && other) {
      if( this != &other ) {
        VkType object = Object;
        std::function<void( VkType )> destroyer_function = DestroyerFunction;

        Object = other.Object;
        DestroyerFunction = other.DestroyerFunction;

        other.Object = object;
        other.DestroyerFunction = destroyer_function;
      }
      return *this;
    }

    VkType & operator*() {
      return Object;
    }

    VkType const & operator*() const {
      return Object;
    }

    bool operator!() const {
      return Object == VK_NULL_HANDLE;
    }

    operator bool() const {
      return Object != VK_NULL_HANDLE;
    }

    VkDestroyer( VkDestroyer<VkType> const & ) = delete;
    VkDestroyer& operator=( VkDestroyer<VkType> const & ) = delete;

  private:
    VkType Object;
    std::function<void( VkType )> DestroyerFunction;
  };

  // Helper functions

  inline void InitVkDestroyer( VkDestroyer<VkInstance> & destroyer ) {
    destroyer = VkDestroyer<VkInstance>( std::bind( DestroyVulkanObject<VkInstance>, std::placeholders::_1 ) );
  }

  inline void InitVkDestroyer( VkDestroyer<VkDevice> & destroyer ) {
    destroyer = VkDestroyer<VkDevice>( std::bind( DestroyVulkanObject<VkDevice>, std::placeholders::_1 ) );
  }

  template<class VkParent, class VkType>
  inline void InitVkDestroyer( VkParent const & parent, VkDestroyer<VkType> & destroyer ) {
    destroyer = VkDestroyer<VkType>( std::bind( DestroyVulkanObject<VkParent, VkType>, parent, std::placeholders::_1 ) );
  }

  template<class VkParent, class VkType>
  inline void InitVkDestroyer( VkDestroyer<VkParent> const & parent, VkDestroyer<VkType> & destroyer ) {
    destroyer = VkDestroyer<VkType>( std::bind( DestroyVulkanObject<VkParent, VkType>, *parent, std::placeholders::_1 ) );
  }

} // namespace VulkanCookbook

#endif // VULKAN_DESTROYER
