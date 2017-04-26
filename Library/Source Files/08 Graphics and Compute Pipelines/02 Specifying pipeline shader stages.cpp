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
// Recipe:  02 Specifying pipeline shader stages

#include "08 Graphics and Compute Pipelines/02 Specifying pipeline shader stages.h"

namespace VulkanCookbook {

  void SpecifyPipelineShaderStages( std::vector<ShaderStageParameters> const     & shader_stage_params,
                                    std::vector<VkPipelineShaderStageCreateInfo> & shader_stage_create_infos ) {
    shader_stage_create_infos.clear();
    for( auto & shader_stage : shader_stage_params ) {
      shader_stage_create_infos.push_back( {
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,  // VkStructureType                    sType
        nullptr,                                              // const void                       * pNext
        0,                                                    // VkPipelineShaderStageCreateFlags   flags
        shader_stage.ShaderStage,                             // VkShaderStageFlagBits              stage
        shader_stage.ShaderModule,                            // VkShaderModule                     module
        shader_stage.EntryPointName,                          // const char                       * pName
        shader_stage.SpecializationInfo                       // const VkSpecializationInfo       * pSpecializationInfo
      } );
    }
  }

} // namespace VulkanCookbook
