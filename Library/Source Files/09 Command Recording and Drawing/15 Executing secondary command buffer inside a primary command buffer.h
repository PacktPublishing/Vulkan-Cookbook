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
// Chapter: 09 Command Recording and Drawing
// Recipe:  15 Executing secondary command buffer inside a primary command buffer

#ifndef EXECUTING_SECONDARY_COMMAND_BUFFER_INSIDE_A_PRIMARY_COMMAND_BUFFER
#define EXECUTING_SECONDARY_COMMAND_BUFFER_INSIDE_A_PRIMARY_COMMAND_BUFFER

#include "Common.h"

namespace VulkanCookbook {

  void ExecuteSecondaryCommandBufferInsidePrimaryCommandBuffer( VkCommandBuffer                      primary_command_buffer,
                                                                std::vector<VkCommandBuffer> const & secondary_command_buffers );

} // namespace VulkanCookbook

#endif // EXECUTING_SECONDARY_COMMAND_BUFFER_INSIDE_A_PRIMARY_COMMAND_BUFFER