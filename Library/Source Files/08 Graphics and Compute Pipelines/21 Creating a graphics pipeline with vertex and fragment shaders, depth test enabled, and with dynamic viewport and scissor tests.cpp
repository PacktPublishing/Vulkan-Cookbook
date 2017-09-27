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
// Chapter: 08 Graphics and Compute Pipelines
// Recipe:  21 Creating a graphics pipeline with vertex and fragment shaders, depth test enabled, and with dynamic viewport and scissor tests

#include "08 Graphics and Compute Pipelines/01 Creating a shader module.h"
#include "08 Graphics and Compute Pipelines/02 Specifying pipeline shader stages.h"
#include "08 Graphics and Compute Pipelines/03 Specifying pipeline vertex input state.h"
#include "08 Graphics and Compute Pipelines/04 Specifying pipeline input assembly state.h"
#include "08 Graphics and Compute Pipelines/06 Specifying pipeline viewport and scissor test state.h"
#include "08 Graphics and Compute Pipelines/07 Specifying pipeline rasterization state.h"
#include "08 Graphics and Compute Pipelines/08 Specifying pipeline multisample state.h"
#include "08 Graphics and Compute Pipelines/09 Specifying pipeline depth and stencil state.h"
#include "08 Graphics and Compute Pipelines/10 Specifying pipeline blend state.h"
#include "08 Graphics and Compute Pipelines/11 Specifying pipeline dynamic states.h"
#include "08 Graphics and Compute Pipelines/13 Specifying graphics pipeline creation parameters.h"
#include "08 Graphics and Compute Pipelines/17 Creating graphics pipelines.h"
#include "08 Graphics and Compute Pipelines/21 Creating a graphics pipeline with vertex and fragment shaders, depth test enabled, and with dynamic viewport and scissor tests.h"
#include "Tools.h"

namespace VulkanCookbook {

  bool CreateGraphicsPipelineWithVertexAndFragmentShadersDepthTestEnabledAndWithDynamicViewportAndScissorTests( VkDevice                                                 logical_device,
                                                                                                                VkPipelineCreateFlags                                    additional_options,
                                                                                                                std::string const                                      & vertex_shader_filename,
                                                                                                                std::string const                                      & fragment_shader_filename,
                                                                                                                std::vector<VkVertexInputBindingDescription> const     & vertex_input_binding_descriptions,
                                                                                                                std::vector<VkVertexInputAttributeDescription> const   & vertex_attribute_descriptions,
                                                                                                                VkPrimitiveTopology                                      primitive_topology,
                                                                                                                bool                                                     primitive_restart_enable,
                                                                                                                VkPolygonMode                                            polygon_mode,
                                                                                                                VkCullModeFlags                                          culling_mode,
                                                                                                                VkFrontFace                                              front_face,
                                                                                                                bool                                                     logic_op_enable,
                                                                                                                VkLogicOp                                                logic_op,
                                                                                                                std::vector<VkPipelineColorBlendAttachmentState> const & attachment_blend_states,
                                                                                                                std::array<float, 4> const                             & blend_constants,
                                                                                                                VkPipelineLayout                                         pipeline_layout,
                                                                                                                VkRenderPass                                             render_pass,
                                                                                                                uint32_t                                                 subpass,
                                                                                                                VkPipeline                                               base_pipeline_handle,
                                                                                                                VkPipelineCache                                          pipeline_cache,
                                                                                                                std::vector<VkPipeline>                                & graphics_pipeline ) {
    std::vector<unsigned char> vertex_shader_spirv;
    if( !GetBinaryFileContents( vertex_shader_filename, vertex_shader_spirv ) ) {
      return false;
    }

    VkDestroyer<VkShaderModule> vertex_shader_module;
    InitVkDestroyer( logical_device, vertex_shader_module );
    if( !CreateShaderModule( logical_device, vertex_shader_spirv, *vertex_shader_module ) ) {
      return false;
    }

    std::vector<unsigned char> fragment_shader_spirv;
    if( !GetBinaryFileContents( fragment_shader_filename, fragment_shader_spirv ) ) {
      return false;
    }
    VkDestroyer<VkShaderModule> fragment_shader_module;
    InitVkDestroyer( logical_device, fragment_shader_module );
    if( !CreateShaderModule( logical_device, fragment_shader_spirv, *fragment_shader_module ) ) {
      return false;
    }

    std::vector<ShaderStageParameters> shader_stage_params = {
      {
        VK_SHADER_STAGE_VERTEX_BIT,   // VkShaderStageFlagBits        ShaderStage
        *vertex_shader_module,        // VkShaderModule               ShaderModule
        "main",                       // char const                 * EntryPointName
        nullptr                       // VkSpecializationInfo const * SpecializationInfo
      },
      {
        VK_SHADER_STAGE_FRAGMENT_BIT, // VkShaderStageFlagBits        ShaderStage
        *fragment_shader_module,      // VkShaderModule               ShaderModule
        "main",                       // char const                 * EntryPointName
        nullptr                       // VkSpecializationInfo const * SpecializationInfo
      }
    };

    std::vector<VkPipelineShaderStageCreateInfo> shader_stage_create_infos;
    SpecifyPipelineShaderStages( shader_stage_params, shader_stage_create_infos );

    VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info;
    SpecifyPipelineVertexInputState( vertex_input_binding_descriptions, vertex_attribute_descriptions, vertex_input_state_create_info );

    VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info;
    SpecifyPipelineInputAssemblyState( primitive_topology, primitive_restart_enable, input_assembly_state_create_info );

    ViewportInfo viewport_infos = {
      {                     // std::vector<VkViewport>   Viewports
        {
          0.0f,               // float          x
          0.0f,               // float          y
          500.0f,             // float          width
          500.0f,             // float          height
          0.0f,               // float          minDepth
          1.0f                // float          maxDepth
        }
      },
      {                     // std::vector<VkRect2D>     Scissors
        {
          {                   // VkOffset2D     offset
            0,                  // int32_t        x
            0                   // int32_t        y
          },
          {                   // VkExtent2D     extent
            500,                // uint32_t       width
            500                 // uint32_t       height
          }
        }
      }
    };
    VkPipelineViewportStateCreateInfo viewport_state_create_info;
    SpecifyPipelineViewportAndScissorTestState( viewport_infos, viewport_state_create_info );

    VkPipelineRasterizationStateCreateInfo rasterization_state_create_info;
    SpecifyPipelineRasterizationState( false, false, polygon_mode, culling_mode, front_face, false, 0.0f, 1.0f, 0.0f, 1.0f, rasterization_state_create_info );

    VkPipelineMultisampleStateCreateInfo multisample_state_create_info;
    SpecifyPipelineMultisampleState( VK_SAMPLE_COUNT_1_BIT, false, 0.0f, nullptr, false, false, multisample_state_create_info );

    VkStencilOpState stencil_test_parameters = {
      VK_STENCIL_OP_KEEP,   // VkStencilOp    failOp
      VK_STENCIL_OP_KEEP,   // VkStencilOp    passOp
      VK_STENCIL_OP_KEEP,   // VkStencilOp    depthFailOp
      VK_COMPARE_OP_ALWAYS, // VkCompareOp    compareOp
      0,                    // uint32_t       compareMask
      0,                    // uint32_t       writeMask
      0                     // uint32_t       reference
    };
    VkPipelineDepthStencilStateCreateInfo depth_and_stencil_state_create_info;
    SpecifyPipelineDepthAndStencilState( true, true, VK_COMPARE_OP_LESS_OR_EQUAL, false, 0.0f, 1.0f, false, stencil_test_parameters, stencil_test_parameters, depth_and_stencil_state_create_info );

    VkPipelineColorBlendStateCreateInfo blend_state_create_info;
    SpecifyPipelineBlendState( logic_op_enable, logic_op, attachment_blend_states, blend_constants, blend_state_create_info );

    std::vector<VkDynamicState> dynamic_states = {
      VK_DYNAMIC_STATE_VIEWPORT,
      VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamic_state_create_info;
    SpecifyPipelineDynamicStates( dynamic_states, dynamic_state_create_info );

    VkGraphicsPipelineCreateInfo graphics_pipeline_create_info;
    SpecifyGraphicsPipelineCreationParameters( additional_options, shader_stage_create_infos, vertex_input_state_create_info, input_assembly_state_create_info, nullptr, &viewport_state_create_info,
      rasterization_state_create_info, &multisample_state_create_info, &depth_and_stencil_state_create_info, &blend_state_create_info, &dynamic_state_create_info, pipeline_layout, render_pass,
      subpass, base_pipeline_handle, -1, graphics_pipeline_create_info );

    if( !CreateGraphicsPipelines( logical_device, { graphics_pipeline_create_info }, pipeline_cache, graphics_pipeline ) ) {
      return false;
    }
    return true;
  }

} // namespace VulkanCookbook
