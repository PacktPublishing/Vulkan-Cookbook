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

#include <utility>
#include <vector>
#include "VulkanFunctions.h"

namespace VulkanCookbook {

  // VkDestroyer<> - wrapper for automatic object destruction

  template <class OBJ>
  class VkDestroyer {
  public:
    VkDestroyer() :
      Device( VK_NULL_HANDLE ),
      Object( VK_NULL_HANDLE ) {
    }

    VkDestroyer( VkDevice device ) :
      Device( device ),
      Object( VK_NULL_HANDLE ) {
    }

    template<class VK_DEVICE>
    VkDestroyer( VkDestroyer<VK_DEVICE> const & device ) :
      Device( *device ),
      Object( VK_NULL_HANDLE ) {
    }

    VkDestroyer( VkDevice device, OBJ object ) :
      Device( device ),
      Object( object ) {
    }

    template<class VK_DEVICE>
    VkDestroyer( VkDestroyer<VK_DEVICE> const & device, OBJ object ) :
      Device( *device ),
      Object( object ) {
    }

    ~VkDestroyer() {
      if( (VK_NULL_HANDLE != Device) &&
          (VK_NULL_HANDLE != Object) ) {
        Destroy();
      }
    }

    VkDestroyer( VkDestroyer<OBJ> && other ) :
      VkDestroyer() {
      *this = std::move( other );
    }

    VkDestroyer& operator=( VkDestroyer<OBJ> && other ) {
      if( this != &other ) {
        VkDevice  device = Device;
        OBJ       object = Object;
        Device = other.Device;
        Object = other.Object;
        other.Device = device;
        other.Object = object;
      }
      return *this;
    }

    OBJ & Get() {
      return Object;
    }

    OBJ * GetPtr() {
      return &Object;
    }

    OBJ & operator*() {
      return Object;
    }

    OBJ const & operator*() const {
      return Object;
    }

    bool operator !() const {
      return VK_NULL_HANDLE == Object;
    }

    explicit operator bool() const {
      return VK_NULL_HANDLE != Object;
    }

  private:
    VkDestroyer( VkDestroyer<OBJ> const & );
    VkDestroyer& operator=( VkDestroyer<OBJ> const & );
    void Destroy();

    VkDevice  Device;
    OBJ       Object;
  };

  // VkInstance specialization for VkDestroyer<>

  template <>
  class VkDestroyer<VkInstance> {
  public:
    VkDestroyer() :
      Instance( VK_NULL_HANDLE ) {
    }

    VkDestroyer( VkInstance object ) :
      Instance( object ) {
    }

    ~VkDestroyer() {
      if( (VK_NULL_HANDLE != Instance) ) {
        Destroy();
      }
    }

    VkDestroyer( VkDestroyer<VkInstance> && other ) {
      *this = std::move( other );
    }

    VkDestroyer& operator=( VkDestroyer<VkInstance> && other ) {
      if( this != &other ) {
        Instance = other.Instance;
        other.Instance = VK_NULL_HANDLE;
      }
      return *this;
    }

    VkInstance & Get() {
      return Instance;
    }

    VkInstance * GetPtr() {
      return &Instance;
    }

    VkInstance & operator*() {
      return Instance;
    }

    VkInstance const & operator*() const {
      return Instance;
    }

    bool operator !() const {
      return VK_NULL_HANDLE == Instance;
    }

    explicit operator bool() const {
      return VK_NULL_HANDLE != Instance;
    }

  private:
    VkDestroyer( VkDestroyer<VkInstance> const & );
    VkDestroyer& operator=( VkDestroyer<VkInstance> const & );

    void Destroy() {
      if( nullptr != vkDestroyInstance ) {
        vkDestroyInstance( Instance, nullptr );
      }
    }

    VkInstance  Instance;
  };

  // VkDevice specialization for VkDestroyer<>

  template <>
  class VkDestroyer<VkDevice> {
  public:
    VkDestroyer() :
      LogicalDevice( VK_NULL_HANDLE ) {
    }

    VkDestroyer( VkDevice object ) :
      LogicalDevice( object ) {
    }

    ~VkDestroyer() {
      if( (VK_NULL_HANDLE != LogicalDevice) ) {
        Destroy();
      }
    }

    VkDestroyer( VkDestroyer<VkDevice> && other ) {
      *this = std::move( other );
    }

    VkDestroyer& operator=( VkDestroyer<VkDevice> && other ) {
      if( this != &other ) {
        LogicalDevice = other.LogicalDevice;
        other.LogicalDevice = VK_NULL_HANDLE;
      }
      return *this;
    }

    VkDevice & Get() {
      return LogicalDevice;
    }

    VkDevice * GetPtr() {
      return &LogicalDevice;
    }

    VkDevice & operator*() {
      return LogicalDevice;
    }

    VkDevice const & operator*() const {
      return LogicalDevice;
    }

    bool operator !() const {
      return VK_NULL_HANDLE == LogicalDevice;
    }

    explicit operator bool() const {
      return VK_NULL_HANDLE != LogicalDevice;
    }

  private:
    VkDestroyer( VkDestroyer<VkDevice> const & );
    VkDestroyer& operator=( VkDestroyer<VkDevice> const & );

    void Destroy() {
      if( nullptr != vkDestroyDevice ) {
        vkDestroyDevice( LogicalDevice, nullptr );
      }
    }

    VkDevice LogicalDevice;
  };

  // VkSurfaceKHR specialization for VkDestroyer<>

  template <>
  class VkDestroyer<VkSurfaceKHR> {
  public:
    VkDestroyer() :
      Instance( VK_NULL_HANDLE ),
      Object( VK_NULL_HANDLE ) {
    }

    VkDestroyer( VkInstance instance ) :
      Instance( instance ),
      Object( VK_NULL_HANDLE ) {
    }

    VkDestroyer( VkDestroyer<VkInstance> const & instance ) :
      Instance( *instance ),
      Object( VK_NULL_HANDLE ) {
    }

    VkDestroyer( VkInstance instance, VkSurfaceKHR object ) :
      Instance( instance ),
      Object( object ) {
    }

    VkDestroyer( VkDestroyer<VkInstance> const & instance, VkSurfaceKHR object ) :
      Instance( *instance ),
      Object( object ) {
    }

    ~VkDestroyer() {
      if( (VK_NULL_HANDLE != Instance) &&
          (VK_NULL_HANDLE != Object) ) {
        Destroy();
      }
    }

    VkDestroyer( VkDestroyer<VkSurfaceKHR> && other ) {
      *this = std::move( other );
    }

    VkDestroyer& operator=( VkDestroyer<VkSurfaceKHR> && other ) {
      if( this != &other ) {
        Instance = other.Instance;
        Object = other.Object;
        other.Instance = VK_NULL_HANDLE;
        other.Object = VK_NULL_HANDLE;
      }
      return *this;
    }

    VkSurfaceKHR & Get() {
      return Object;
    }

    VkSurfaceKHR * GetPtr() {
      return &Object;
    }

    VkSurfaceKHR & operator*() {
      return Object;
    }

    VkSurfaceKHR const & operator*() const {
      return Object;
    }

    bool operator !() const {
      return VK_NULL_HANDLE == Object;
    }

    explicit operator bool() const {
      return VK_NULL_HANDLE != Object;
    }

  private:
    VkDestroyer( VkDestroyer<VkSurfaceKHR> const & );
    VkDestroyer& operator=( VkDestroyer<VkSurfaceKHR> const & );

    void Destroy() {
      if( nullptr != vkDestroySurfaceKHR ) {
        vkDestroySurfaceKHR( Instance, Object, nullptr );
      }
    }

    VkInstance    Instance;
    VkSurfaceKHR  Object;
  };

  
#define VK_DESTROYER_SPECIALIZATION( type, deleter )  \
  template<>                                          \
  inline void VkDestroyer<type>::Destroy() {          \
    if( nullptr != deleter ) {                        \
      deleter( Device, Object, nullptr );             \
    }                                                 \
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

  // Helper initialization functions

  template <class PARENT, class OBJ>
  void InitVkDestroyer( PARENT const & parent, OBJ obj, VkDestroyer<OBJ> & wrapper ) {
    wrapper = VkDestroyer<OBJ>( parent, obj );
  }

  template <class PARENT, class OBJ>
  void InitVkDestroyer( PARENT const & parent, VkDestroyer<OBJ> & wrapper ) {
    wrapper = VkDestroyer<OBJ>( parent );
  }

} // namespace VulkanCookbook

#endif // VULKAN_DESTROYER
