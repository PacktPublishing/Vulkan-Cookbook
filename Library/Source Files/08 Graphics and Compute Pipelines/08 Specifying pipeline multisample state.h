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
// Recipe:  08 Specifying pipeline multisample state

#ifndef SPECIFYING_PIPELINE_MULTISAMPLE_STATE
#define SPECIFYING_PIPELINE_MULTISAMPLE_STATE

#include "Common.h"

namespace VulkanCookbook {

  void SpecifyPipelineMultisampleState( VkSampleCountFlagBits                  sample_count,
                                        bool                                   per_sample_shading_enable,
                                        float                                  min_sample_shading,
                                        VkSampleMask const                   * sample_masks,
                                        bool                                   alpha_to_coverage_enable,
                                        bool                                   alpha_to_one_enable,
                                        VkPipelineMultisampleStateCreateInfo & multisample_state_create_info );

} // namespace VulkanCookbook

#endif // SPECIFYING_PIPELINE_MULTISAMPLE_STATE