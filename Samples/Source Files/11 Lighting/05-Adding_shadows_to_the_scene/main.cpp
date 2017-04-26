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
// Chapter: 11 Lighting
// Recipe:  05 Adding shadows to the scene

#include "CookbookSampleFramework.h"
#include "OrbitingCamera.h"

using namespace VulkanCookbook;

class Sample : public VulkanCookbookSample {
  std::array<Mesh, 2>                     Scene;
  VkDestroyer<VkBuffer>                   VertexBuffer;
  VkDestroyer<VkDeviceMemory>             VertexBufferMemory;

  struct ShadowMapParameters {
    VkDestroyer<VkImage>                    Image;
    VkDestroyer<VkDeviceMemory>             Memory;
    VkDestroyer<VkImageView>                View;
    VkDestroyer<VkFramebuffer>              Framebuffer;
  }                                       ShadowMap;
  VkDestroyer<VkSampler>                  ShadowMapSampler;

  VkDestroyer<VkDescriptorSetLayout>      DescriptorSetLayout;
  VkDestroyer<VkDescriptorPool>           DescriptorPool;
  std::vector<VkDescriptorSet>            DescriptorSets;

  VkDestroyer<VkPipelineLayout>           PipelineLayout;

  VkDestroyer<VkRenderPass>               ShadowMapRenderPass;
  VkDestroyer<VkPipeline>                 ShadowMapPipeline;
  VkDestroyer<VkRenderPass>               SceneRenderPass;
  VkDestroyer<VkPipeline>                 ScenePipeline;

  VkDestroyer<VkBuffer>                   StagingBuffer;
  VkDestroyer<VkDeviceMemory>             StagingBufferMemory;
  bool                                    UpdateUniformBuffer;
  VkDestroyer<VkBuffer>                   UniformBuffer;
  VkDestroyer<VkDeviceMemory>             UniformBufferMemory;

  OrbitingCamera                          LightSource;
  OrbitingCamera                          Camera;

  virtual bool Initialize( WindowParameters WindowParameters ) override {
    if( !InitializeVulkan( WindowParameters ) ) {
      return false;
    }

    Camera = OrbitingCamera( Vector3{ 0.0f, 0.0f, 0.0f }, 4.0f );

    LightSource = OrbitingCamera( Vector3{ 0.0f, 0.0f, 0.0f }, 4.0f, 0.0f, -80.0f );

    // Vertex data

    if( !Load3DModelFromObjFile( "Data/Models/knot.obj", true, false, false, true, Scene[0] ) ) {
      return false;
    }

    if( !Load3DModelFromObjFile( "Data/Models/plane.obj", true, false, false, false, Scene[1] ) ) {
      return false;
    }

    std::vector<float> vertex_data( Scene[0].Data );
    vertex_data.insert( vertex_data.end(), Scene[1].Data.begin(), Scene[1].Data.end() );

    InitVkDestroyer( LogicalDevice, VertexBuffer );
    if( !CreateBuffer( *LogicalDevice, sizeof( vertex_data[0] ) * vertex_data.size(),
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, *VertexBuffer ) ) {
      return false;
    }

    InitVkDestroyer( LogicalDevice, VertexBufferMemory );
    if( !AllocateAndBindMemoryObjectToBuffer( PhysicalDevice, *LogicalDevice, *VertexBuffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *VertexBufferMemory ) ) {
      return false;
    }

    if( !UseStagingBufferToUpdateBufferWithDeviceLocalMemoryBound( PhysicalDevice, *LogicalDevice, sizeof( vertex_data[0] ) * vertex_data.size(),
      &vertex_data[0], *VertexBuffer, 0, 0, VK_ACCESS_TRANSFER_WRITE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
      GraphicsQueue.Handle, FrameResources.front().CommandBuffer, {} ) ) {
      return false;
    }

    // Staging buffer
    InitVkDestroyer( LogicalDevice, StagingBuffer );
    if( !CreateBuffer( *LogicalDevice, 3 * 16 * sizeof(float), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, *StagingBuffer ) ) {
      return false;
    }
    InitVkDestroyer( LogicalDevice, StagingBufferMemory );
    if( !AllocateAndBindMemoryObjectToBuffer( PhysicalDevice, *LogicalDevice, *StagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, *StagingBufferMemory ) ) {
      return false;
    }

    // Uniform buffer
    InitVkDestroyer( LogicalDevice, UniformBuffer );
    InitVkDestroyer( LogicalDevice, UniformBufferMemory );
    if( !CreateUniformBuffer( PhysicalDevice, *LogicalDevice, 3 * 16 * sizeof( float ), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
      *UniformBuffer, *UniformBufferMemory ) ) {
      return false;
    }

    if( !UpdateStagingBuffer( true ) ) {
      return false;
    }

    // Image in which shadow map will be stored

    InitVkDestroyer( LogicalDevice, ShadowMap.Image );
    InitVkDestroyer( LogicalDevice, ShadowMap.Memory );
    InitVkDestroyer( LogicalDevice, ShadowMap.View );
    InitVkDestroyer( LogicalDevice, ShadowMapSampler );
    if( !CreateCombinedImageSampler( PhysicalDevice, *LogicalDevice, VK_IMAGE_TYPE_2D, DepthFormat, { 512, 512, 1 }, 1, 1,
      VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_DEPTH_BIT, VK_FILTER_LINEAR,
      VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
      VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, 0.0f, false, 1.0f, false, VK_COMPARE_OP_ALWAYS, 0.0f, 1.0f, VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK,
      false, *ShadowMapSampler, *ShadowMap.Image, *ShadowMap.Memory, *ShadowMap.View ) ) {
      return false;
    }

    // Descriptor set

    std::vector<VkDescriptorSetLayoutBinding> descriptor_set_layout_bindings = {
      {
        0,                                          // uint32_t             binding
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,          // VkDescriptorType     descriptorType
        1,                                          // uint32_t             descriptorCount
        VK_SHADER_STAGE_VERTEX_BIT,                 // VkShaderStageFlags   stageFlags
        nullptr                                     // const VkSampler    * pImmutableSamplers
      },
      {
        1,                                          // uint32_t             binding
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,  // VkDescriptorType     descriptorType
        1,                                          // uint32_t             descriptorCount
        VK_SHADER_STAGE_FRAGMENT_BIT,               // VkShaderStageFlags   stageFlags
        nullptr                                     // const VkSampler    * pImmutableSamplers
      }
    };
    InitVkDestroyer( LogicalDevice, DescriptorSetLayout );
    if( !CreateDescriptorSetLayout( *LogicalDevice, descriptor_set_layout_bindings, *DescriptorSetLayout ) ) {
      return false;
    }

    std::vector<VkDescriptorPoolSize> descriptor_pool_sizes = {
      {
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,          // VkDescriptorType     type
        1                                           // uint32_t             descriptorCount
      },
      {
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,  // VkDescriptorType     type
        1                                           // uint32_t             descriptorCount
      }
    };
    InitVkDestroyer( LogicalDevice, DescriptorPool );
    if( !CreateDescriptorPool( *LogicalDevice, false, 1, descriptor_pool_sizes, *DescriptorPool ) ) {
      return false;
    }

    if( !AllocateDescriptorSets( *LogicalDevice, *DescriptorPool, { *DescriptorSetLayout }, DescriptorSets ) ) {
      return false;
    }

    BufferDescriptorInfo buffer_descriptor_update = {
      DescriptorSets[0],                                // VkDescriptorSet                      TargetDescriptorSet
      0,                                                // uint32_t                             TargetDescriptorBinding
      0,                                                // uint32_t                             TargetArrayElement
      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,                // VkDescriptorType                     TargetDescriptorType
      {                                                 // std::vector<VkDescriptorBufferInfo>  BufferInfos
        {
          *UniformBuffer,                                 // VkBuffer                             buffer
          0,                                              // VkDeviceSize                         offset
          VK_WHOLE_SIZE                                   // VkDeviceSize                         range
        }
      }
    };

    ImageDescriptorInfo image_descriptor_update = {
      DescriptorSets[0],                                // VkDescriptorSet                      TargetDescriptorSet
      1,                                                // uint32_t                             TargetDescriptorBinding
      0,                                                // uint32_t                             TargetArrayElement
      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,        // VkDescriptorType                     TargetDescriptorType
      {                                                 // std::vector<VkDescriptorImageInfo>   ImageInfos
        {
          *ShadowMapSampler,                              // VkSampler                            sampler
          *ShadowMap.View,                                // VkImageView                          imageView
          VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL // VkImageLayout                        imageLayout
        }
      }
    };

    UpdateDescriptorSets( *LogicalDevice, { image_descriptor_update }, { buffer_descriptor_update }, {}, {} );

    // Shadow map render pass - for rendering into depth attachment

    std::vector<VkAttachmentDescription> shadow_map_attachment_descriptions = {
      {
        0,                                                // VkAttachmentDescriptionFlags     flags
        DepthFormat,                                      // VkFormat                         format
        VK_SAMPLE_COUNT_1_BIT,                            // VkSampleCountFlagBits            samples
        VK_ATTACHMENT_LOAD_OP_CLEAR,                      // VkAttachmentLoadOp               loadOp
        VK_ATTACHMENT_STORE_OP_STORE,                     // VkAttachmentStoreOp              storeOp
        VK_ATTACHMENT_LOAD_OP_DONT_CARE,                  // VkAttachmentLoadOp               stencilLoadOp
        VK_ATTACHMENT_STORE_OP_DONT_CARE,                 // VkAttachmentStoreOp              stencilStoreOp
        VK_IMAGE_LAYOUT_UNDEFINED,                        // VkImageLayout                    initialLayout
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL   // VkImageLayout                    finalLayout
      }
    };

    VkAttachmentReference shadow_map_depth_attachment = {
      0,                                                // uint32_t                             attachment
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL  // VkImageLayout                        layout
    };

    std::vector<SubpassParameters> shadow_map_subpass_parameters = {
      {
        VK_PIPELINE_BIND_POINT_GRAPHICS,              // VkPipelineBindPoint                  PipelineType
        {},                                           // std::vector<VkAttachmentReference>   InputAttachments
        {},                                           // std::vector<VkAttachmentReference>   ColorAttachments
        {},                                           // std::vector<VkAttachmentReference>   ResolveAttachments
        &shadow_map_depth_attachment,                 // VkAttachmentReference const        * DepthStencilAttachment
        {}                                            // std::vector<uint32_t>                PreserveAttachments
      }
    };

    std::vector<VkSubpassDependency> shadow_map_subpass_dependencies = {
      {
        VK_SUBPASS_EXTERNAL,                            // uint32_t                   srcSubpass
        0,                                              // uint32_t                   dstSubpass
        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,          // VkPipelineStageFlags       srcStageMask
        VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,     // VkPipelineStageFlags       dstStageMask
        VK_ACCESS_SHADER_READ_BIT,                      // VkAccessFlags              srcAccessMask
        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,   // VkAccessFlags              dstAccessMask
        VK_DEPENDENCY_BY_REGION_BIT                     // VkDependencyFlags          dependencyFlags
      },
      {
        0,                                              // uint32_t                   srcSubpass
        VK_SUBPASS_EXTERNAL,                            // uint32_t                   dstSubpass
        VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,      // VkPipelineStageFlags       srcStageMask
        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,          // VkPipelineStageFlags       dstStageMask
        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,   // VkAccessFlags              srcAccessMask
        VK_ACCESS_SHADER_READ_BIT,                      // VkAccessFlags              dstAccessMask
        VK_DEPENDENCY_BY_REGION_BIT                     // VkDependencyFlags          dependencyFlags
      }
    };

    InitVkDestroyer( LogicalDevice, ShadowMapRenderPass );
    if( !CreateRenderPass( *LogicalDevice, shadow_map_attachment_descriptions, shadow_map_subpass_parameters, shadow_map_subpass_dependencies,
      *ShadowMapRenderPass ) ) {
      return false;
    }

    InitVkDestroyer( LogicalDevice, ShadowMap.Framebuffer );
    if( !CreateFramebuffer( *LogicalDevice, *ShadowMapRenderPass, { *ShadowMap.View }, 512, 512, 1, *ShadowMap.Framebuffer ) ) {
      return false;
    }

    // Scene render pass - for rendering final scene

    std::vector<VkAttachmentDescription> attachment_descriptions = {
      {
        0,                                                // VkAttachmentDescriptionFlags     flags
        Swapchain.Format,                                 // VkFormat                         format
        VK_SAMPLE_COUNT_1_BIT,                            // VkSampleCountFlagBits            samples
        VK_ATTACHMENT_LOAD_OP_CLEAR,                      // VkAttachmentLoadOp               loadOp
        VK_ATTACHMENT_STORE_OP_STORE,                     // VkAttachmentStoreOp              storeOp
        VK_ATTACHMENT_LOAD_OP_DONT_CARE,                  // VkAttachmentLoadOp               stencilLoadOp
        VK_ATTACHMENT_STORE_OP_DONT_CARE,                 // VkAttachmentStoreOp              stencilStoreOp
        VK_IMAGE_LAYOUT_UNDEFINED,                        // VkImageLayout                    initialLayout
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR                   // VkImageLayout                    finalLayout
      },
      {
        0,                                                // VkAttachmentDescriptionFlags     flags
        DepthFormat,                                      // VkFormat                         format
        VK_SAMPLE_COUNT_1_BIT,                            // VkSampleCountFlagBits            samples
        VK_ATTACHMENT_LOAD_OP_CLEAR,                      // VkAttachmentLoadOp               loadOp
        VK_ATTACHMENT_STORE_OP_DONT_CARE,                 // VkAttachmentStoreOp              storeOp
        VK_ATTACHMENT_LOAD_OP_DONT_CARE,                  // VkAttachmentLoadOp               stencilLoadOp
        VK_ATTACHMENT_STORE_OP_DONT_CARE,                 // VkAttachmentStoreOp              stencilStoreOp
        VK_IMAGE_LAYOUT_UNDEFINED,                        // VkImageLayout                    initialLayout
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL  // VkImageLayout                    finalLayout
      }
    };

    VkAttachmentReference depth_attachment = {
      1,                                                // uint32_t                             attachment
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL  // VkImageLayout                        layout;
    };

    std::vector<SubpassParameters> subpass_parameters = {
      {
        VK_PIPELINE_BIND_POINT_GRAPHICS,              // VkPipelineBindPoint                  PipelineType
        {},                                           // std::vector<VkAttachmentReference>   InputAttachments
        {                                             // std::vector<VkAttachmentReference>   ColorAttachments
          {
            0,                                          // uint32_t                             attachment
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,   // VkImageLayout                        layout
          }
        },
        {},                                           // std::vector<VkAttachmentReference>   ResolveAttachments
        &depth_attachment,                            // VkAttachmentReference const        * DepthStencilAttachment
        {}                                            // std::vector<uint32_t>                PreserveAttachments
      }
    };

    std::vector<VkSubpassDependency> subpass_dependencies = {
      {
        VK_SUBPASS_EXTERNAL,                            // uint32_t                   srcSubpass
        0,                                              // uint32_t                   dstSubpass
        VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,      // VkPipelineStageFlags       srcStageMask
        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,          // VkPipelineStageFlags       dstStageMask
        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,   // VkAccessFlags              srcAccessMask
        VK_ACCESS_SHADER_READ_BIT,                      // VkAccessFlags              dstAccessMask
        VK_DEPENDENCY_BY_REGION_BIT                     // VkDependencyFlags          dependencyFlags
      },
      {
        0,                                              // uint32_t                   srcSubpass
        VK_SUBPASS_EXTERNAL,                            // uint32_t                   dstSubpass
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,  // VkPipelineStageFlags       srcStageMask
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,              // VkPipelineStageFlags       dstStageMask
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,           // VkAccessFlags              srcAccessMask
        VK_ACCESS_MEMORY_READ_BIT,                      // VkAccessFlags              dstAccessMask
        VK_DEPENDENCY_BY_REGION_BIT                     // VkDependencyFlags          dependencyFlags
      }
    };

    InitVkDestroyer( LogicalDevice, SceneRenderPass );
    if( !CreateRenderPass( *LogicalDevice, attachment_descriptions, subpass_parameters, subpass_dependencies, *SceneRenderPass ) ) {
      return false;
    }

    // Graphics pipeline

    std::vector<VkPushConstantRange> push_constant_ranges = {
      {
        VK_SHADER_STAGE_VERTEX_BIT,     // VkShaderStageFlags     stageFlags
        0,                              // uint32_t               offset
        sizeof( float ) * 4             // uint32_t               size
      }
    };
    InitVkDestroyer( LogicalDevice, PipelineLayout );
    if( !CreatePipelineLayout( *LogicalDevice, { *DescriptorSetLayout }, push_constant_ranges, *PipelineLayout ) ) {
      return false;
    }

    // Scene

    std::vector<unsigned char> scene_vertex_shader_spirv;
    if( !GetBinaryFileContents( "Data/Shaders/11 Lighting/05 Adding shadows to the scene/scene.vert.spv", scene_vertex_shader_spirv ) ) {
      return false;
    }

    VkDestroyer<VkShaderModule> scene_vertex_shader_module( LogicalDevice );
    if( !CreateShaderModule( *LogicalDevice, scene_vertex_shader_spirv, *scene_vertex_shader_module ) ) {
      return false;
    }

    std::vector<unsigned char> scene_fragment_shader_spirv;
    if( !GetBinaryFileContents( "Data/Shaders/11 Lighting/05 Adding shadows to the scene/scene.frag.spv", scene_fragment_shader_spirv ) ) {
      return false;
    }
    VkDestroyer<VkShaderModule> scene_fragment_shader_module( LogicalDevice );
    if( !CreateShaderModule( *LogicalDevice, scene_fragment_shader_spirv, *scene_fragment_shader_module ) ) {
      return false;
    }

    std::vector<ShaderStageParameters> scene_shader_stage_params = {
      {
        VK_SHADER_STAGE_VERTEX_BIT,       // VkShaderStageFlagBits        ShaderStage
        *scene_vertex_shader_module,      // VkShaderModule               ShaderModule
        "main",                           // char const                 * EntryPointName;
        nullptr                           // VkSpecializationInfo const * SpecializationInfo;
      },
      {
        VK_SHADER_STAGE_FRAGMENT_BIT,     // VkShaderStageFlagBits        ShaderStage
        *scene_fragment_shader_module,    // VkShaderModule               ShaderModule
        "main",                           // char const                 * EntryPointName
        nullptr                           // VkSpecializationInfo const * SpecializationInfo
      }
    };

    std::vector<VkPipelineShaderStageCreateInfo> scene_shader_stage_create_infos;
    SpecifyPipelineShaderStages( scene_shader_stage_params, scene_shader_stage_create_infos );

    std::vector<VkVertexInputBindingDescription> scene_vertex_input_binding_descriptions = {
      {
        0,                            // uint32_t                     binding
        6 * sizeof( float ),          // uint32_t                     stride
        VK_VERTEX_INPUT_RATE_VERTEX   // VkVertexInputRate            inputRate
      }
    };

    std::vector<VkVertexInputAttributeDescription> scene_vertex_attribute_descriptions = {
      {
        0,                                                                        // uint32_t   location
        0,                                                                        // uint32_t   binding
        VK_FORMAT_R32G32B32_SFLOAT,                                               // VkFormat   format
        0                                                                         // uint32_t   offset
      },
      {
        1,                                                                        // uint32_t   location
        0,                                                                        // uint32_t   binding
        VK_FORMAT_R32G32B32_SFLOAT,                                               // VkFormat   format
        3 * sizeof( float )                                                       // uint32_t   offset
      }
    };

    VkPipelineVertexInputStateCreateInfo scene_vertex_input_state_create_info;
    SpecifyPipelineVertexInputState( scene_vertex_input_binding_descriptions, scene_vertex_attribute_descriptions, scene_vertex_input_state_create_info );

    // Shadow

    std::vector<unsigned char> shadow_vertex_shader_spirv;
    if( !GetBinaryFileContents( "Data/Shaders/11 Lighting/05 Adding shadows to the scene/shadow.vert.spv", shadow_vertex_shader_spirv ) ) {
      return false;
    }

    VkDestroyer<VkShaderModule> shadow_vertex_shader_module( LogicalDevice );
    if( !CreateShaderModule( *LogicalDevice, shadow_vertex_shader_spirv, *shadow_vertex_shader_module ) ) {
      return false;
    }

    std::vector<ShaderStageParameters> shadow_shader_stage_params = {
      {
        VK_SHADER_STAGE_VERTEX_BIT,       // VkShaderStageFlagBits        ShaderStage
        *shadow_vertex_shader_module,     // VkShaderModule               ShaderModule
        "main",                           // char const                 * EntryPointName;
        nullptr                           // VkSpecializationInfo const * SpecializationInfo;
      }
    };

    std::vector<VkPipelineShaderStageCreateInfo> shadow_shader_stage_create_infos;
    SpecifyPipelineShaderStages( shadow_shader_stage_params, shadow_shader_stage_create_infos );

    std::vector<VkVertexInputBindingDescription> shadow_vertex_input_binding_descriptions = {
      {
        0,                            // uint32_t                     binding
        6 * sizeof( float ),          // uint32_t                     stride
        VK_VERTEX_INPUT_RATE_VERTEX   // VkVertexInputRate            inputRate
      }
    };

    std::vector<VkVertexInputAttributeDescription> shadow_vertex_attribute_descriptions = {
      {
        0,                                                                        // uint32_t   location
        0,                                                                        // uint32_t   binding
        VK_FORMAT_R32G32B32_SFLOAT,                                               // VkFormat   format
        0                                                                         // uint32_t   offset
      },
    };

    VkPipelineVertexInputStateCreateInfo shadow_vertex_input_state_create_info;
    SpecifyPipelineVertexInputState( shadow_vertex_input_binding_descriptions, shadow_vertex_attribute_descriptions, shadow_vertex_input_state_create_info );

    // Common

    VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info;
    SpecifyPipelineInputAssemblyState( VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, false, input_assembly_state_create_info );

    ViewportInfo viewport_infos = {
      {
        {                   // VkViewport     Viewport
          0.0f,               // float          x
          0.0f,               // float          y
          512.0f,             // float          width
          512.0f,             // float          height
          0.0f,               // float          minDepth
          1.0f                // float          maxDepth
        },
      },
      {
        {                   // VkRect2D       Scissor
          {                   // VkOffset2D     offset
            0,                  // int32_t        x
            0                   // int32_t        y
          },
          {                   // VkExtent2D     extent
            512,                // uint32_t       width
            512                 // uint32_t       height
          }
        }
      }
    };
    VkPipelineViewportStateCreateInfo viewport_state_create_info;
    SpecifyPipelineViewportAndScissorTestState( viewport_infos, viewport_state_create_info );

    VkPipelineRasterizationStateCreateInfo rasterization_state_create_info;
    SpecifyPipelineRasterizationState( false, false, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE, false, 0.0f, 1.0f, 0.0f, 1.0f, rasterization_state_create_info );

    VkPipelineMultisampleStateCreateInfo multisample_state_create_info;
    SpecifyPipelineMultisampleState( VK_SAMPLE_COUNT_1_BIT, false, 0.0f, nullptr, false, false, multisample_state_create_info );

    VkPipelineDepthStencilStateCreateInfo depth_stencil_state_create_info;
    SpecifyPipelineDepthAndStencilState( true, true, VK_COMPARE_OP_LESS_OR_EQUAL, false, 0.0f, 1.0f, false, {}, {}, depth_stencil_state_create_info );

    std::vector<VkPipelineColorBlendAttachmentState> attachment_blend_states = {
      {
        false,                          // VkBool32                 blendEnable
        VK_BLEND_FACTOR_ONE,            // VkBlendFactor            srcColorBlendFactor
        VK_BLEND_FACTOR_ONE,            // VkBlendFactor            dstColorBlendFactor
        VK_BLEND_OP_ADD,                // VkBlendOp                colorBlendOp
        VK_BLEND_FACTOR_ONE,            // VkBlendFactor            srcAlphaBlendFactor
        VK_BLEND_FACTOR_ONE,            // VkBlendFactor            dstAlphaBlendFactor
        VK_BLEND_OP_ADD,                // VkBlendOp                alphaBlendOp
        VK_COLOR_COMPONENT_R_BIT |      // VkColorComponentFlags    colorWriteMask
        VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT
      }
    };
    VkPipelineColorBlendStateCreateInfo blend_state_create_info;
    SpecifyPipelineBlendState( false, VK_LOGIC_OP_COPY, attachment_blend_states, { 1.0f, 1.0f, 1.0f, 1.0f }, blend_state_create_info );

    // Scene

    std::vector<VkDynamicState> dynamic_states = {
      VK_DYNAMIC_STATE_VIEWPORT,
      VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamic_state_create_info;
    SpecifyPipelineDynamicStates( dynamic_states, dynamic_state_create_info );

    VkGraphicsPipelineCreateInfo scene_pipeline_create_info;
    SpecifyGraphicsPipelineCreationParameters( 0, scene_shader_stage_create_infos, scene_vertex_input_state_create_info, input_assembly_state_create_info,
      nullptr, &viewport_state_create_info, rasterization_state_create_info, &multisample_state_create_info, &depth_stencil_state_create_info, &blend_state_create_info,
      &dynamic_state_create_info, *PipelineLayout, *SceneRenderPass, 0, VK_NULL_HANDLE, -1, scene_pipeline_create_info );

    std::vector<VkPipeline> scene_pipeline;
    if( !CreateGraphicsPipelines( *LogicalDevice, { scene_pipeline_create_info }, VK_NULL_HANDLE, scene_pipeline ) ) {
      return false;
    }
    InitVkDestroyer( LogicalDevice, scene_pipeline[0], ScenePipeline );

    // Shadow

    VkGraphicsPipelineCreateInfo shadow_pipeline_create_info;
    SpecifyGraphicsPipelineCreationParameters( 0, shadow_shader_stage_create_infos, shadow_vertex_input_state_create_info, input_assembly_state_create_info,
      nullptr, &viewport_state_create_info, rasterization_state_create_info, &multisample_state_create_info, &depth_stencil_state_create_info, nullptr,
      nullptr, *PipelineLayout, *ShadowMapRenderPass, 0, VK_NULL_HANDLE, -1, shadow_pipeline_create_info );

    std::vector<VkPipeline> shadow_pipeline;
    if( !CreateGraphicsPipelines( *LogicalDevice, { shadow_pipeline_create_info }, VK_NULL_HANDLE, shadow_pipeline ) ) {
      return false;
    }
    InitVkDestroyer( LogicalDevice, shadow_pipeline[0], ShadowMapPipeline );

    return true;
  }

  virtual bool Draw() override {
    auto prepare_frame = [&]( VkCommandBuffer command_buffer, uint32_t swapchain_image_index, VkFramebuffer framebuffer ) {
      if( !BeginCommandBufferRecordingOperation( command_buffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, nullptr ) ) {
        return false;
      }

      if( UpdateUniformBuffer ) {
        UpdateUniformBuffer = false;

        BufferTransition pre_transfer_transition = {
          *UniformBuffer,               // VkBuffer         Buffer
          VK_ACCESS_UNIFORM_READ_BIT,   // VkAccessFlags    CurrentAccess
          VK_ACCESS_TRANSFER_WRITE_BIT, // VkAccessFlags    NewAccess
          VK_QUEUE_FAMILY_IGNORED,      // uint32_t         CurrentQueueFamily
          VK_QUEUE_FAMILY_IGNORED       // uint32_t         NewQueueFamily
        };
        SetBufferMemoryBarrier( command_buffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, { pre_transfer_transition } );

        std::vector<VkBufferCopy> regions = {
          {
            0,                        // VkDeviceSize     srcOffset
            0,                        // VkDeviceSize     dstOffset
            3 * 16 * sizeof( float )  // VkDeviceSize     size
          }
        };
        CopyDataBetweenBuffers( command_buffer, *StagingBuffer, *UniformBuffer, regions );

        BufferTransition post_transfer_transition = {
          *UniformBuffer,               // VkBuffer         Buffer
          VK_ACCESS_TRANSFER_WRITE_BIT, // VkAccessFlags    CurrentAccess
          VK_ACCESS_UNIFORM_READ_BIT,   // VkAccessFlags    NewAccess
          VK_QUEUE_FAMILY_IGNORED,      // uint32_t         CurrentQueueFamily
          VK_QUEUE_FAMILY_IGNORED       // uint32_t         NewQueueFamily
        };
        SetBufferMemoryBarrier( command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT, { post_transfer_transition } );
      }

      // Shadow map generation

      BeginRenderPass( command_buffer, *ShadowMapRenderPass, *ShadowMap.Framebuffer, { { 0, 0, }, { 512, 512 } }, { { 1.0f, 0 } }, VK_SUBPASS_CONTENTS_INLINE );

      BindVertexBuffers( command_buffer, 0, { { *VertexBuffer, 0 } } );

      BindDescriptorSets( command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *PipelineLayout, 0, DescriptorSets, {} );

      BindPipelineObject( command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *ShadowMapPipeline );

      DrawGeometry( command_buffer, Scene[0].Parts[0].VertexCount + Scene[1].Parts[0].VertexCount, 1, 0, 0 );

      EndRenderPass( command_buffer );

      if( PresentQueue.FamilyIndex != GraphicsQueue.FamilyIndex ) {
        ImageTransition image_transition_before_drawing = {
          Swapchain.Images[swapchain_image_index],  // VkImage              Image
          VK_ACCESS_MEMORY_READ_BIT,                // VkAccessFlags        CurrentAccess
          VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,     // VkAccessFlags        NewAccess
          VK_IMAGE_LAYOUT_UNDEFINED,                // VkImageLayout        CurrentLayout
          VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, // VkImageLayout        NewLayout
          PresentQueue.FamilyIndex,                 // uint32_t             CurrentQueueFamily
          GraphicsQueue.FamilyIndex,                // uint32_t             NewQueueFamily
          VK_IMAGE_ASPECT_COLOR_BIT                 // VkImageAspectFlags   Aspect
        };
        SetImageMemoryBarrier( command_buffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, { image_transition_before_drawing } );
      }

      // Drawing
      BeginRenderPass( command_buffer, *SceneRenderPass, framebuffer, { { 0, 0 }, Swapchain.Size }, { { 0.1f, 0.2f, 0.3f, 1.0f }, { 1.0f, 0 } }, VK_SUBPASS_CONTENTS_INLINE );

      VkViewport viewport = {
        0.0f,                                       // float    x
        0.0f,                                       // float    y
        static_cast<float>(Swapchain.Size.width),   // float    width
        static_cast<float>(Swapchain.Size.height),  // float    height
        0.0f,                                       // float    minDepth
        1.0f,                                       // float    maxDepth
      };
      SetViewportStateDynamically( command_buffer, 0, { viewport } );

      VkRect2D scissor = {
        {                                           // VkOffset2D     offset
          0,                                          // int32_t        x
          0                                           // int32_t        y
        },
        {                                           // VkExtent2D     extent
          Swapchain.Size.width,                       // uint32_t       width
          Swapchain.Size.height                       // uint32_t       height
        }
      };
      SetScissorStateDynamically( command_buffer, 0, { scissor } );

      BindPipelineObject( command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *ScenePipeline );

      ProvideDataToShadersThroughPushConstants( command_buffer, *PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof( float ) * 4, &LightSource.GetPosition()[0] );

      DrawGeometry( command_buffer, Scene[0].Parts[0].VertexCount + Scene[1].Parts[0].VertexCount, 1, 0, 0 );

      EndRenderPass( command_buffer );

      if( PresentQueue.FamilyIndex != GraphicsQueue.FamilyIndex ) {
        ImageTransition image_transition_before_present = {
          Swapchain.Images[swapchain_image_index],  // VkImage              Image
          VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,     // VkAccessFlags        CurrentAccess
          VK_ACCESS_MEMORY_READ_BIT,                // VkAccessFlags        NewAccess
          VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,          // VkImageLayout        CurrentLayout
          VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,          // VkImageLayout        NewLayout
          GraphicsQueue.FamilyIndex,                // uint32_t             CurrentQueueFamily
          PresentQueue.FamilyIndex,                 // uint32_t             NewQueueFamily
          VK_IMAGE_ASPECT_COLOR_BIT                 // VkImageAspectFlags   Aspect
        };
        SetImageMemoryBarrier( command_buffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, { image_transition_before_present } );
      }

      if( !EndCommandBufferRecordingOperation( command_buffer ) ) {
        return false;
      }
      return true;
    };

    WaitForAllSubmittedCommandsToBeFinished( *LogicalDevice );

    return IncreasePerformanceThroughIncreasingTheNumberOfSeparatelyRenderedFrames( *LogicalDevice, GraphicsQueue.Handle, PresentQueue.Handle,
      *Swapchain.Handle, Swapchain.Size, Swapchain.ImageViewsRaw, *SceneRenderPass, {}, prepare_frame, FrameResources );
  }

  void OnMouseEvent() {
    UpdateStagingBuffer( false );
  }

  bool UpdateStagingBuffer( bool force ) {
    if( MouseState.Buttons[0].IsPressed ) {
      Camera.RotateHorizontally( 0.5f * MouseState.Position.Delta.X );
      Camera.RotateVertically( -0.5f * MouseState.Position.Delta.Y );
      force = true;
    }

    if( MouseState.Buttons[1].IsPressed ) {
      LightSource.RotateHorizontally( 0.5f * MouseState.Position.Delta.X );
      LightSource.RotateVertically( -0.5f * MouseState.Position.Delta.Y );
      force = true;
    }

    if( force ) {
      UpdateUniformBuffer = true;

      Matrix4x4 light_view_matrix = LightSource.GetMatrix();
      Matrix4x4 scene_view_matrix = Camera.GetMatrix();
      Matrix4x4 perspective_matrix = PreparePerspectiveProjectionMatrix( static_cast<float>(Swapchain.Size.width) / static_cast<float>(Swapchain.Size.height), 50.0f, 0.5f, 10.0f );

      std::vector<float> data;
      data.insert( data.end(), &light_view_matrix[0], &light_view_matrix[0] + 16 );
      data.insert( data.end(), &scene_view_matrix[0], &scene_view_matrix[0] + 16 );
      data.insert( data.end(), &perspective_matrix[0], &perspective_matrix[0] + 16 );

      if( !MapUpdateAndUnmapHostVisibleMemory( *LogicalDevice, *StagingBufferMemory, 0, sizeof( data[0] ) * data.size(), &data[0], true, nullptr ) ) {
        return false;
      }
    }
    return true;
  }

  virtual bool Resize() override {
    if( !CreateSwapchain() ) {
      return false;
    }

    if( IsReady() ) {
      if( !UpdateStagingBuffer( true ) ) {
        return false;
      }
    }
    return true;
  }

};

VULKAN_COOKBOOK_SAMPLE_FRAMEWORK( "11/05 - Adding shadows to the scene", 50, 25, 1280, 800, Sample )
