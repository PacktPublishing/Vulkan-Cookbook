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
// Common

#ifndef ALL_HEADERS
#define ALL_HEADERS

#include "01 Instance and Devices/03 Connecting with a Vulkan Loader library.h"
#include "01 Instance and Devices/05 Loading function exported from a Vulkan Loader library.h"
#include "01 Instance and Devices/06 Loading global-level functions.h"
#include "01 Instance and Devices/07 Checking available Instance extensions.h"
#include "01 Instance and Devices/08 Creating a Vulkan Instance.h"
#include "01 Instance and Devices/09 Loading instance-level functions.h"
#include "01 Instance and Devices/10 Enumerating available physical devices.h"
#include "01 Instance and Devices/11 Checking available device extensions.h"
#include "01 Instance and Devices/12 Getting features and properties of a physical device.h"
#include "01 Instance and Devices/13 Checking available queue families and their properties.h"
#include "01 Instance and Devices/14 Selecting index of a queue family with desired capabilities.h"
#include "01 Instance and Devices/15 Creating a logical device.h"
#include "01 Instance and Devices/16 Loading device-level functions.h"
#include "01 Instance and Devices/17 Getting a device queue.h"
#include "01 Instance and Devices/18 Creating a logical device with geometry shaders and graphics queue.h"
#include "01 Instance and Devices/19 Destroying a logical device.h"
#include "01 Instance and Devices/20 Destroying a Vulkan Instance.h"
#include "01 Instance and Devices/21 Releasing a Vulkan Loader library.h"

#include "02 Image Presentation/01 Creating a Vulkan Instance with WSI extensions enabled.h"
#include "02 Image Presentation/02 Creating a presentation surface.h"
#include "02 Image Presentation/03 Selecting a queue family that supports presentation to a given surface.h"
#include "02 Image Presentation/04 Creating a logical device with WSI extensions enabled.h"
#include "02 Image Presentation/05 Selecting a desired presentation mode.h"
#include "02 Image Presentation/06 Getting capabilities of a presentation surface.h"
#include "02 Image Presentation/07 Selecting a number of swapchain images.h"
#include "02 Image Presentation/08 Choosing a size of swapchain images.h"
#include "02 Image Presentation/09 Selecting desired usage scenarios of swapchain images.h"
#include "02 Image Presentation/10 Selecting a transformation of swapchain images.h"
#include "02 Image Presentation/11 Selecting a format of swapchain images.h"
#include "02 Image Presentation/12 Creating a swapchain.h"
#include "02 Image Presentation/13 Getting handles of swapchain images.h"
#include "02 Image Presentation/14 Creating a swapchain with R8G8B8A8 format and a MAILBOX present mode.h"
#include "02 Image Presentation/15 Acquiring a swapchain image.h"
#include "02 Image Presentation/16 Presenting an image.h"
#include "02 Image Presentation/17 Destroying a swapchain.h"
#include "02 Image Presentation/18 Destroying a presentation surface.h"

#include "03 Command Buffers and Synchronization/01 Creating a command pool.h"
#include "03 Command Buffers and Synchronization/02 Allocating command buffers.h"
#include "03 Command Buffers and Synchronization/03 Beginning a command buffer recording operation.h"
#include "03 Command Buffers and Synchronization/04 Ending a command buffer recording operation.h"
#include "03 Command Buffers and Synchronization/05 Resetting a command buffer.h"
#include "03 Command Buffers and Synchronization/06 Resetting a command pool.h"
#include "03 Command Buffers and Synchronization/07 Creating a semaphore.h"
#include "03 Command Buffers and Synchronization/08 Creating a fence.h"
#include "03 Command Buffers and Synchronization/09 Waiting for fences.h"
#include "03 Command Buffers and Synchronization/10 Resetting fences.h"
#include "03 Command Buffers and Synchronization/11 Submitting command buffers to the queue.h"
#include "03 Command Buffers and Synchronization/12 Synchronizing two command buffers.h"
#include "03 Command Buffers and Synchronization/13 Checking if processing of a submitted command buffer has finished.h"
#include "03 Command Buffers and Synchronization/14 Waiting until all commands submitted to a queue are finished.h"
#include "03 Command Buffers and Synchronization/15 Waiting for all submitted commands to be finished.h"
#include "03 Command Buffers and Synchronization/16 Destroying a fence.h"
#include "03 Command Buffers and Synchronization/17 Destroying a semaphore.h"
#include "03 Command Buffers and Synchronization/18 Freeing command buffers.h"
#include "03 Command Buffers and Synchronization/19 Destroying a command pool.h"

#include "04 Resources and Memory/01 Creating a buffer.h"
#include "04 Resources and Memory/02 Allocating and binding memory object to a buffer.h"
#include "04 Resources and Memory/03 Setting a buffer memory barrier.h"
#include "04 Resources and Memory/04 Creating a buffer view.h"
#include "04 Resources and Memory/05 Creating an image.h"
#include "04 Resources and Memory/06 Allocating and binding memory object to an image.h"
#include "04 Resources and Memory/07 Setting an image memory barrier.h"
#include "04 Resources and Memory/08 Creating an image view.h"
#include "04 Resources and Memory/09 Creating a 2D image and view.h"
#include "04 Resources and Memory/10 Creating a layered 2D image with a CUBEMAP view.h"
#include "04 Resources and Memory/11 Mapping, updating and unmapping host-visible memory.h"
#include "04 Resources and Memory/12 Copying data between buffers.h"
#include "04 Resources and Memory/13 Copying data from a buffer to an image.h"
#include "04 Resources and Memory/14 Copying data from an image to a buffer.h"
#include "04 Resources and Memory/15 Using staging buffer to update a buffer with a device-local memory bound.h"
#include "04 Resources and Memory/16 Using staging buffer to update an image with a device-local memory bound.h"
#include "04 Resources and Memory/17 Destroying an image view.h"
#include "04 Resources and Memory/18 Destroying an image.h"
#include "04 Resources and Memory/19 Destroying a buffer view.h"
#include "04 Resources and Memory/20 Freeing a memory object.h"
#include "04 Resources and Memory/21 Destroying a buffer.h"

#include "05 Descriptor Sets/01 Creating a sampler.h"
#include "05 Descriptor Sets/02 Creating a sampled image.h"
#include "05 Descriptor Sets/03 Creating a combined image sampler.h"
#include "05 Descriptor Sets/04 Creating a storage image.h"
#include "05 Descriptor Sets/05 Creating a uniform texel buffer.h"
#include "05 Descriptor Sets/06 Creating a storage texel buffer.h"
#include "05 Descriptor Sets/07 Creating a uniform buffer.h"
#include "05 Descriptor Sets/08 Creating a storage buffer.h"
#include "05 Descriptor Sets/09 Creating an input attachment.h"
#include "05 Descriptor Sets/10 Creating a descriptor set layout.h"
#include "05 Descriptor Sets/11 Creating a descriptor pool.h"
#include "05 Descriptor Sets/12 Allocating descriptor sets.h"
#include "05 Descriptor Sets/13 Updating descriptor sets.h"
#include "05 Descriptor Sets/14 Binding descriptor sets.h"
#include "05 Descriptor Sets/15 Creating descriptors with a texture and a uniform buffer.h"
#include "05 Descriptor Sets/16 Freeing descriptor sets.h"
#include "05 Descriptor Sets/17 Resetting a descriptor pool.h"
#include "05 Descriptor Sets/18 Destroying a descriptor pool.h"
#include "05 Descriptor Sets/19 Destroying a descriptor set layout.h"
#include "05 Descriptor Sets/20 Destroying a sampler.h"

#include "06 Render Passes and Framebuffers/01 Specifying attachments descriptions.h"
#include "06 Render Passes and Framebuffers/02 Specifying subpass descriptions.h"
#include "06 Render Passes and Framebuffers/03 Specifying dependencies between subpasses.h"
#include "06 Render Passes and Framebuffers/04 Creating a render pass.h"
#include "06 Render Passes and Framebuffers/05 Creating a framebuffer.h"
#include "06 Render Passes and Framebuffers/06 Preparing a render pass for geometry rendering and postprocess subpasses.h"
#include "06 Render Passes and Framebuffers/07 Preparing a render pass and a framebuffer with color and depth attachments.h"
#include "06 Render Passes and Framebuffers/08 Beginning a render pass.h"
#include "06 Render Passes and Framebuffers/09 Progressing to the next subpass.h"
#include "06 Render Passes and Framebuffers/10 Ending a render pass.h"
#include "06 Render Passes and Framebuffers/11 Destroying a framebuffer.h"
#include "06 Render Passes and Framebuffers/12 Destroying a render pass.h"

#include "08 Graphics and Compute Pipelines/01 Creating a shader module.h"
#include "08 Graphics and Compute Pipelines/02 Specifying pipeline shader stages.h"
#include "08 Graphics and Compute Pipelines/03 Specifying pipeline vertex input state.h"
#include "08 Graphics and Compute Pipelines/04 Specifying pipeline input assembly state.h"
#include "08 Graphics and Compute Pipelines/05 Specifying pipeline tessellation state.h"
#include "08 Graphics and Compute Pipelines/06 Specifying pipeline viewport and scissor test state.h"
#include "08 Graphics and Compute Pipelines/07 Specifying pipeline rasterization state.h"
#include "08 Graphics and Compute Pipelines/08 Specifying pipeline multisample state.h"
#include "08 Graphics and Compute Pipelines/09 Specifying pipeline depth and stencil state.h"
#include "08 Graphics and Compute Pipelines/10 Specifying pipeline blend state.h"
#include "08 Graphics and Compute Pipelines/11 Specifying pipeline dynamic states.h"
#include "08 Graphics and Compute Pipelines/12 Creating a pipeline layout.h"
#include "08 Graphics and Compute Pipelines/13 Specifying graphics pipeline creation parameters.h"
#include "08 Graphics and Compute Pipelines/14 Creating a pipeline cache object.h"
#include "08 Graphics and Compute Pipelines/15 Retrieving data from a pipeline cache.h"
#include "08 Graphics and Compute Pipelines/16 Merging multiple pipeline cache objects.h"
#include "08 Graphics and Compute Pipelines/17 Creating graphics pipelines.h"
#include "08 Graphics and Compute Pipelines/18 Creating a compute pipeline.h"
#include "08 Graphics and Compute Pipelines/19 Binding a pipeline object.h"
#include "08 Graphics and Compute Pipelines/20 Creating a pipeline layout with a combined image sampler, a buffer and push constant ranges.h"
#include "08 Graphics and Compute Pipelines/21 Creating a graphics pipeline with vertex and fragment shaders, depth test enabled, and with dynamic viewport and scissor tests.h"
#include "08 Graphics and Compute Pipelines/22 Creating multiple graphics pipelines on multiple threads.h"
#include "08 Graphics and Compute Pipelines/23 Destroying a pipeline.h"
#include "08 Graphics and Compute Pipelines/24 Destroying a pipeline cache.h"
#include "08 Graphics and Compute Pipelines/25 Destroying a pipeline layout.h"
#include "08 Graphics and Compute Pipelines/26 Destroying a shader module.h"

#include "09 Command Recording and Drawing/01 Clearing a color image.h"
#include "09 Command Recording and Drawing/02 Clearing a depth-stencil image.h"
#include "09 Command Recording and Drawing/03 Clearing render pass attachments.h"
#include "09 Command Recording and Drawing/04 Binding vertex buffers.h"
#include "09 Command Recording and Drawing/05 Binding an index buffer.h"
#include "09 Command Recording and Drawing/06 Providing data to shaders through push constants.h"
#include "09 Command Recording and Drawing/07 Setting viewport state dynamically.h"
#include "09 Command Recording and Drawing/08 Setting scissor state dynamically.h"
#include "09 Command Recording and Drawing/09 Setting line width state dynamically.h"
#include "09 Command Recording and Drawing/10 Setting depth bias state dynamically.h"
#include "09 Command Recording and Drawing/11 Setting blend constants state dynamically.h"
#include "09 Command Recording and Drawing/12 Drawing a geometry.h"
#include "09 Command Recording and Drawing/13 Drawing an indexed geometry.h"
#include "09 Command Recording and Drawing/14 Dispatching compute work.h"
#include "09 Command Recording and Drawing/15 Executing secondary command buffer inside a primary command buffer.h"
#include "09 Command Recording and Drawing/16 Recording a command buffer that draws a geometry with dynamic viewport and scissor states.h"
#include "09 Command Recording and Drawing/17 Recording command buffers on multiple threads.h"
#include "09 Command Recording and Drawing/18 Preparing a single frame of animation.h"
#include "09 Command Recording and Drawing/19 Increasing the performance through increasing the number of separately rendered frames.h"

#include "10 Helper Recipes/01 Preparing a translation matrix.h"
#include "10 Helper Recipes/02 Preparing a rotation matrix.h"
#include "10 Helper Recipes/03 Preparing a scaling matrix.h"
#include "10 Helper Recipes/04 Preparing a perspective projection matrix.h"
#include "10 Helper Recipes/05 Preparing an orthographic projection matrix.h"
#include "10 Helper Recipes/06 Loading texture data from a file.h"
#include "10 Helper Recipes/07 Loading a 3D model from an OBJ file.h"


#endif // ALL_HEADERS