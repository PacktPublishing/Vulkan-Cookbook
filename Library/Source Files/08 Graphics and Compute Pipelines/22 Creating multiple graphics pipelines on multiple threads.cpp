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
// Recipe:  22 Creating multiple graphics pipelines on multiple threads

#include "08 Graphics and Compute Pipelines/14 Creating a pipeline cache object.h"
#include "08 Graphics and Compute Pipelines/15 Retrieving data from a pipeline cache.h"
#include "08 Graphics and Compute Pipelines/16 Merging multiple pipeline cache objects.h"
#include "08 Graphics and Compute Pipelines/17 Creating graphics pipelines.h"
#include "08 Graphics and Compute Pipelines/22 Creating multiple graphics pipelines on multiple threads.h"
#include "Tools.h"

namespace VulkanCookbook {

  bool CreateMultipleGraphicsPipelinesOnMultipleThreads( VkDevice                                                       logical_device,
                                                         std::string const                                            & pipeline_cache_filename,
                                                         std::vector<std::vector<VkGraphicsPipelineCreateInfo>> const & graphics_pipelines_create_infos,
                                                         std::vector<std::vector<VkPipeline>>                         & graphics_pipelines ) {
    // Load cache from file (if available)
    std::vector<unsigned char> cache_data;
    GetBinaryFileContents( pipeline_cache_filename, cache_data );

    // Create cache for each thread, initialize its contents with data loaded from file
    std::vector<VkDestroyer<VkPipelineCache>> pipeline_caches( graphics_pipelines_create_infos.size() );
    for( size_t i = 0; i < graphics_pipelines_create_infos.size(); ++i ) {
      pipeline_caches[i] = VkDestroyer< VkPipelineCache >( logical_device );
      if( !CreatePipelineCacheObject( logical_device, cache_data, *pipeline_caches[i] ) ) {
        return false;
      }
    }

    // Create multiple threads, where each thread creates multiple pipelines using its own cache
    std::vector<std::thread> threads( graphics_pipelines_create_infos.size() );
    for( size_t i = 0; i < graphics_pipelines_create_infos.size(); ++i ) {
      graphics_pipelines[i].resize( graphics_pipelines_create_infos[i].size() );
      threads[i] = std::thread( CreateGraphicsPipelines, logical_device, std::ref( graphics_pipelines_create_infos[i] ), *pipeline_caches[i], std::ref( graphics_pipelines[i] ) );
    }

    // Wait for all threads to finish
    for( size_t i = 0; i < graphics_pipelines_create_infos.size(); ++i ) {
      threads[i].join();
    }

    // Merge all the caches into one, retrieve its contents and store them in the file
    VkPipelineCache target_cache = *pipeline_caches.back();
    std::vector<VkPipelineCache> source_caches( pipeline_caches.size() - 1);
    for( size_t i = 0; i < pipeline_caches.size() - 1; ++i ) {
      source_caches[i] = *pipeline_caches[i];
    }

    if( !MergeMultiplePipelineCacheObjects( logical_device, target_cache, source_caches ) ) {
      return false;
    }

    if( !RetrieveDataFromPipelineCache( logical_device, target_cache, cache_data ) ) {
      return false;
    }

    //if( !SaveBinaryFile( pipeline_cache_filename, cache_data ) ) {
    //  return false;
    //}

    return true;
  }

} // namespace VulkanCookbook
