# Vulkan Cookbook
This is the code repository for [Vulkan Cookbook](https://www.packtpub.com/game-development/vulkan-cookbook?utm_source=github&utm_medium=repository&utm_campaign=9781786468154), published by [Packt](https://www.packtpub.com/). All the example workflows that are mentioned in the book are present in the package.

## About the Book
Vulkan is the next generation graphics API released by the Khronos group. It is expected to be the successor to OpenGL and OpenGL ES, which it shares some similarities with such as its cross-platform capabilities, programmable pipeline stages, or nomenclature. Vulkan is a low-level API that gives developers much more control over the hardware, but also adds new responsibilities such as explicit memory and resources management. With it, though, Vulkan is expected to be much faster.

### Related Books

* [Vulkan Programming [Video]](https://www.packtpub.com/application-development/vulkan-programming-video?utm_source=github&utm_medium=repository&utm_campaign=9781786460714)

* [Learning Vulkan](https://www.packtpub.com/application-development/learning-vulkan?utm_source=github&utm_medium=repository&utm_campaign=9781786469809)

* [Building an Unreal RTS Game: The Basics [Video]](https://www.packtpub.com/application-development/building-unreal-rts-game-basics-video?utm_source=github&utm_medium=repository&utm_campaign=9781787285279)

### Suggestions and Feedback
 [Click here](https://docs.google.com/forms/d/e/1FAIpQLSe5qwunkGf6PUvzPirPDtuy1Du5Rlzew23UBp2S-P3wB-GcwQ/viewform) if you have any feedback or suggestions.

<hr>

## Credits
### Special thanks for authors and developers of the following projects and resources:
* [**tinyobjloader**](https://github.com/syoyo/tinyobjloader) - A single-header library for loading Wavefront OBJ files.
* [**stb image**](https://github.com/nothings/stb) - A single-header library for loading image files (other libraries are also available).
* [**Humus**](http://www.humus.name/index.php?page=Textures) - A large collection of cubemaps (and other resources).

<hr>

## Please note!
### Currently only Windows operating system is supported. Linux version is being prepared and should be ready soon.

<hr>

# [Samples](./Samples/Source%20Files/)

## [Chapter 11 - Lighting](./Samples/Source%20Files/11%20Lighting/)

<img src="./Samples/Source%20Files/11%20Lighting/01-Rendering_a_geometry_with_vertex_diffuse_lighting/screenshot.png" width="150px" align="right">

* ### [01 - Rendering a geometry with vertex diffuse lighting](./Samples/Source%20Files/11%20Lighting/01-Rendering_a_geometry_with_vertex_diffuse_lighting/main.cpp)

Sample showing how to implement a diffuse lighting algorithm calculated only at geometry's verices using vertex shaders.<br>
<b>Left mouse button:</b> rotate the scene

<img src="./Samples/Source%20Files/11%20Lighting/02-Rendering_a_geometry_with_fragment_specular_lighting/screenshot.png" width="150px" align="right">

* ### [02 - Rendering a geometry with fragment specular lighting](./Samples/Source%20Files/11%20Lighting/02-Rendering_a_geometry_with_fragment_specular_lighting/main.cpp)

This sample present the Phong specular lighting algorithm implemented on vertex and fragment shaders.<br>
<b>Left mouse button:</b> rotate the scene

<img src="./Samples/Source%20Files/11%20Lighting/03-Rendering_a_normal_mapped_geometry/screenshot.png" width="150px" align="right">

* ### [03 - Rendering a normal mapped geometry](./Samples/Source%20Files/11%20Lighting/03-Rendering_a_normal_mapped_geometry/main.cpp)

Here a normal mapping technique is presented and the model is lit using the specular lighting algorithm.<br>
<b>Left mouse button:</b> rotate the scene

<img src="./Samples/Source%20Files/11%20Lighting/04-Rendering_a_reflective_and_refractive_geometry_using_cubemaps/screenshot.png" width="150px" align="right">

* ### [04 - Rendering a reflective and refractive geometry using cubemaps](./Samples/Source%20Files/11%20Lighting/04-Rendering_a_reflective_and_refractive_geometry_using_cubemaps/main.cpp)

Sample presenting how to use cubemaps to render a transparent geometry that both reflects and refracts environment.<br>
<b>Left mouse button:</b> rotate the scene

<img src="./Samples/Source%20Files/11%20Lighting/05-Adding_shadows_to_the_scene/screenshot.png" width="150px" align="right">

* ### [05 - Adding shadows to the scene](./Samples/Source%20Files/11%20Lighting/05-Adding_shadows_to_the_scene/main.cpp)

In this sample a basic shadow mapping algorithm is shown. In the first render pass a shadow map is generated. In the second render pass a scene is rendered and the data from the shadow map is used to check, whether the geometry is lit or covered in shadow.<br>
<b>Left mouse button:</b> rotate the scene<br>
<b>Right mouse button:</b> move the light

## [Chapter 12 - Advanced Rendering Techniques](./Samples/Source%20Files/12%20Advanced%20Rendering%20Techniques/)

<img src="./Samples/Source%20Files/12%20Advanced%20Rendering%20Techniques/01-Drawing_a_skybox/screenshot.png" width="150px" align="right">

* ### [01 - Drawing a skybox](./Samples/Source%20Files/12%20Advanced%20Rendering%20Techniques/01-Drawing_a_skybox/main.cpp)

Here it is shown how to draw a skybox, which simulates background - objects seen in a distance and/or a sky.<br>
<b>Left mouse button:</b> look around

<img src="./Samples/Source%20Files/12%20Advanced%20Rendering%20Techniques/02-Drawing_bilboards_using_geometry_shaders/screenshot.png" width="150px" align="right">

* ### [02 - Drawing bilboards using geometry shaders](./Samples/Source%20Files/12%20Advanced%20Rendering%20Techniques/02-Drawing_bilboards_using_geometry_shaders/main.cpp)

This sample presents a way of drawing sprites or bilboards - flat, textured quads that are always facing the camera.<br>
<b>Left mouse button:</b> rotate the scene

<img src="./Samples/Source%20Files/12%20Advanced%20Rendering%20Techniques/03-Drawing_particles_using_compute_and_graphics_pipelines/screenshot.png" width="150px" align="right">

* ### [03 - Drawing particles using compute and graphics pipelines](./Samples/Source%20Files/12%20Advanced%20Rendering%20Techniques/03-Drawing_particles_using_compute_and_graphics_pipelines/main.cpp)

Here an example of rendering particles is shown. Compute shaders are used to calculate positions of all particles in the system. Particles are rendered as flat bilboards (sprites).<br>
<b>Left mouse button:</b> rotate the scene

<img src="./Samples/Source%20Files/12%20Advanced%20Rendering%20Techniques/04-Rendering_a_tesselated_terrain/screenshot.png" width="150px" align="right">

* ### [04 - Rendering a tesselated terrain](./Samples/Source%20Files/12%20Advanced%20Rendering%20Techniques/04-Rendering_a_tesselated_terrain/main.cpp)

This code sample shows one of the ways to draw a terrain. A complete graphics pipeline with all five programmable stages is used that tessellates the terrain near the camera to improve its complexity, with level of details faiding away with increasing distance from the camera, and with a flat shading lighting algorithm.<br>
<b>Left mouse button:</b> rotate the scene<br>
<b>Mouse wheel:</b> zoom in / zoom out

<img src="./Samples/Source%20Files/12%20Advanced%20Rendering%20Techniques/05-Rendering_a_fullscreen_quad_for_postprocessing/screenshot.png" width="150px" align="right">

* ### [05 - Rendering a fullscreen quad for postprocessing](./Samples/Source%20Files/12%20Advanced%20Rendering%20Techniques/05-Rendering_a_fullscreen_quad_for_postprocessing/main.cpp)

Sample presenting a fast and easy way to prepare an image postprocessing phase in a graphics pipeline - by using a fullscreen quad drawn already in a clip space. An edge detection algorithm is shown as on of the examples of postprocessing techniques.

<img src="./Samples/Source%20Files/12%20Advanced%20Rendering%20Techniques/06-Using_input_attachment_for_color_correction_postprocess_effect/screenshot.png" width="150px" align="right">

* ### [06 - Using an input attachment for color correction postprocess effect](./Samples/Source%20Files/12%20Advanced%20Rendering%20Techniques/06-Using_input_attachment_for_color_correction_postprocess_effect/main.cpp)

In this code another postprocessing technique is shown that uses one of the Vulkan's specific features - input attachments, which allow reading data from render targets (attachments) in the same render pass.<br>
<b>Left mouse button:</b> rotate the scene

## [Other](./Samples/Source%20Files/Other/)

<img src="./Samples/Source%20Files/Other/01-Creating_Logical_Device/screenshot.png" width="150px" align="right">

* ### [01 - Creating a logical device](./Samples/Source%20Files/Other/01-Creating_Logical_Device/main.cpp)

Code sample that shows basic Vulkan setup - instance creation, physical device enumeration and logical device creation.

<img src="./Samples/Source%20Files/Other/02-Creating_Swapchain/screenshot.png" width="150px" align="right">

* ### [02 - Creating a swapchain](./Samples/Source%20Files/Other/02-Creating_Swapchain/main.cpp)

Here a swapchain object is created, which allows us to render a scene directly to an application's window.

<img src="./Samples/Source%20Files/Other/03-Using_Render_Passes/screenshot.png" width="150px" align="right">

* ### [03 - Using render passes](./Samples/Source%20Files/Other/03-Using_Render_Passes/main.cpp)

This example shows how to preapre a basic render pass - a description of attachments (render targets) needed to render a geometry.

<img src="./Samples/Source%20Files/Other/04-Using_Graphics_Pipeline/screenshot.png" width="150px" align="right">

* ### [04 - Using a graphics pipeline](./Samples/Source%20Files/Other/04-Using_Graphics_Pipeline/main.cpp)

Sample showing how to create a graphics pipeline, setup its multiple parameters and use it to draw a scene.

<img src="./Samples/Source%20Files/Other/05-Using_Combined_Image_Samplers/screenshot.png" width="150px" align="right">

* ### [05 - Using combined image samplers](./Samples/Source%20Files/Other/05-Using_Combined_Image_Samplers/main.cpp)

Here descriptor sets are introduced. They are required to setup an interface between application and a pipeline and to provide images (textures) to shaders.

<img src="./Samples/Source%20Files/Other/06-Using_Uniform_Buffers/screenshot.png" width="150px" align="right">

* ### [06 - Using uniform buffers](./Samples/Source%20Files/Other/06-Using_Uniform_Buffers/main.cpp)

Another example of using descriptor sets, but this time it presented how to prepare transformation matrices and provide them to shaders.

<img src="./Samples/Source%20Files/Other/07-Using_Push_Constants/screenshot.png" width="150px" align="right">

* ### [07 - Using push constants](./Samples/Source%20Files/Other/07-Using_Push_Constants/main.cpp)

This code sample presents a very fast and easy way to provide data to shaders - push constants. Though the provided data may not be too big, they are ideal for performing frequent updates.

<img src="./Samples/Source%20Files/Other/08-Using_Tessellation_Shaders/screenshot.png" width="150px" align="right">

* ### [08 - Using tessellation shaders](./Samples/Source%20Files/Other/08-Using_Tessellation_Shaders/main.cpp)

Here we can see how to create a graphics pipeline with tessellation control and evaluation shaders enabled responsible for increasing the complexity of a rendered geometry.

<img src="./Samples/Source%20Files/Other/09-Using_Geometry_Shaders/screenshot.png" width="150px" align="right">

* ### [09 - Using geometry shaders](./Samples/Source%20Files/Other/09-Using_Geometry_Shaders/main.cpp)

Sample presenting how to use geometry shaders and generate new primitives instead of those drawn in an application.

<img src="./Samples/Source%20Files/Other/10-Using_Compute_Shaders/screenshot.png" width="150px" align="right">

* ### [10 - Using compute shaders](./Samples/Source%20Files/Other/10-Using_Compute_Shaders/main.cpp)

This code sample shows how to create a compute pipeline - the second type of pipelines supported in the Vulkan API. It allows us to perform mathematical computations.

<img src="./Samples/Source%20Files/Other/11-Drawing_Vertex_Normals/screenshot.png" width="150px" align="right">

* ### [11 - Drawing vertex normals](./Samples/Source%20Files/Other/11-Drawing_Vertex_Normals/main.cpp)

Here a commonly used debugging technique is presented that uses geometry shaders to display normal vectors provided by the application.

<img src="./Samples/Source%20Files/Other/12-Using_Depth_Attachments/screenshot.png" width="150px" align="right">

* ### [12 - Using depth attachments](./Samples/Source%20Files/Other/12-Using_Depth_Attachments/main.cpp)

In this example we can see how to setup a render pass, framebufer and a graphics pipeline to use depth attachment and enable depth test during drawing.

<img src="./Samples/Source%20Files/Other/13-Enabling_Alpha_Blending/screenshot.png" width="150px" align="right">

* ### [13 - Enabling alpha blending](./Samples/Source%20Files/Other/13-Enabling_Alpha_Blending/main.cpp)

This code sample shows how to enable alpha blending (transparency) in a graphics pipeline.<br>
<b>Left mouse button:</b> disable/enable blending<br>

<img src="./Samples/Source%20Files/Other/14-Drawing_Single_Fullscreen_Triangle_For_Postprocessing/screenshot.png" width="150px" align="right">

* ### [14 Drawing Single Fullscreen Triangle For Postprocessing](./Samples/Source%20Files/Other/14-Drawing_Single_Fullscreen_Triangle_For_Postprocessing/main.cpp)

This sample shows an alternative for performing a postprocessing with a quad (two triangles). Here a single triangle covering the whole screen is used to apply a grayscale effect.<br>

<hr>

# [Recipes Library](./Library/Source%20Files/)

## [Chapter 01 - Instance and Devices](./Library/Source%20Files/01%20Instance%20and%20Devices/)

* [03 - Connecting with a Vulkan Loader library](./Library/Source%20Files/01%20Instance%20and%20Devices/03%20Connecting%20with%20a%20Vulkan%20Loader%20library.cpp)

* [05 - Loading function exported from a Vulkan Loader library](./Library/Source%20Files/01%20Instance%20and%20Devices/05%20Loading%20function%20exported%20from%20a%20Vulkan%20Loader%20library.cpp)

* [06 - Loading global-level functions](./Library/Source%20Files/01%20Instance%20and%20Devices/06%20Loading%20global-level%20functions.cpp)

* [07 - Checking available Instance extensions](./Library/Source%20Files/01%20Instance%20and%20Devices/07%20Checking%20available%20Instance%20extensions.cpp)

* [08 - Creating a Vulkan Instance](./Library/Source%20Files/01%20Instance%20and%20Devices/08%20Creating%20a%20Vulkan%20Instance.cpp)

* [09 - Loading instance-level functions](./Library/Source%20Files/01%20Instance%20and%20Devices/09%20Loading%20instance-level%20functions.cpp)

* [10 - Enumerating available physical devices](./Library/Source%20Files/01%20Instance%20and%20Devices/10%20Enumerating%20available%20physical%20devices.cpp)

* [11 - Checking available device extensions](./Library/Source%20Files/01%20Instance%20and%20Devices/11%20Checking%20available%20device%20extensions.cpp)

* [12 - Getting features and properties of a physical device](./Library/Source%20Files/01%20Instance%20and%20Devices/12%20Getting%20features%20and%20properties%20of%20a%20physical%20device.cpp)

* [13 - Checking available queue families and their properties](./Library/Source%20Files/01%20Instance%20and%20Devices/13%20Checking%20available%20queue%20families%20and%20their%20properties.cpp)

* [14 - Selecting index of a queue family with desired capabilities](./Library/Source%20Files/01%20Instance%20and%20Devices/14%20Selecting%20index%20of%20a%20queue%20family%20with%20desired%20capabilities.cpp)

* [15 - Creating a logical device](./Library/Source%20Files/01%20Instance%20and%20Devices/15%20Creating%20a%20logical%20device.cpp)

* [16 - Loading device-level functions](./Library/Source%20Files/01%20Instance%20and%20Devices/16%20Loading%20device-level%20functions.cpp)

* [17 - Getting a device queue](./Library/Source%20Files/01%20Instance%20and%20Devices/17%20Getting%20a%20device%20queue.cpp)

* [18 - Creating a logical device with geometry shaders and graphics queue](./Library/Source%20Files/01%20Instance%20and%20Devices/18%20Creating%20a%20logical%20device%20with%20geometry%20shaders%20and%20graphics%20queue.cpp)

* [19 - Destroying a logical device](./Library/Source%20Files/01%20Instance%20and%20Devices/19%20Destroying%20a%20logical%20device.cpp)

* [20 - Destroying a Vulkan Instance](./Library/Source%20Files/01%20Instance%20and%20Devices/20%20Destroying%20a%20Vulkan%20Instance.cpp)

* [21 - Releasing a Vulkan Loader library](./Library/Source%20Files/01%20Instance%20and%20Devices/21%20Releasing%20a%20Vulkan%20Loader%20library.cpp)

## [Chapter 02 - Image Presentation](./Library/Source%20Files/02%20Image%20Presentation/)

* [01 - Creating a Vulkan Instance with WSI extensions enabled](./Library/Source%20Files/02%20Image%20Presentation/01%20Creating%20a%20Vulkan%20Instance%20with%20WSI%20extensions%20enabled.cpp)

* [02 - Creating a presentation surface](./Library/Source%20Files/02%20Image%20Presentation/02%20Creating%20a%20presentation%20surface.cpp)

* [03 - Selecting a queue family that supports presentation to a given surface](./Library/Source%20Files/02%20Image%20Presentation/03%20Selecting%20a%20queue%20family%20that%20supports%20presentation%20to%20a%20given%20surface.cpp)

* [04 - Creating a logical device with WSI extensions enabled](./Library/Source%20Files/02%20Image%20Presentation/04%20Creating%20a%20logical%20device%20with%20WSI%20extensions%20enabled.cpp)

* [05 - Selecting a desired presentation mode](./Library/Source%20Files/02%20Image%20Presentation/05%20Selecting%20a%20desired%20presentation%20mode.cpp)

* [06 - Getting capabilities of a presentation surface](./Library/Source%20Files/02%20Image%20Presentation/06%20Getting%20capabilities%20of%20a%20presentation%20surface.cpp)

* [07 - Selecting a number of swapchain images](./Library/Source%20Files/02%20Image%20Presentation/07%20Selecting%20a%20number%20of%20swapchain%20images.cpp)

* [08 - Choosing a size of swapchain images](./Library/Source%20Files/02%20Image%20Presentation/08%20Choosing%20a%20size%20of%20swapchain%20images.cpp)

* [09 - Selecting desired usage scenarios of swapchain images](./Library/Source%20Files/02%20Image%20Presentation/09%20Selecting%20desired%20usage%20scenarios%20of%20swapchain%20images.cpp)

* [10 - Selecting a transformation of swapchain images](./Library/Source%20Files/02%20Image%20Presentation/10%20Selecting%20a%20transformation%20of%20swapchain%20images.cpp)

* [11 - Selecting a format of swapchain images](./Library/Source%20Files/02%20Image%20Presentation/11%20Selecting%20a%20format%20of%20swapchain%20images.cpp)

* [12 - Creating a swapchain](./Library/Source%20Files/02%20Image%20Presentation/12%20Creating%20a%20swapchain.cpp)

* [13 - Getting handles of swapchain images](./Library/Source%20Files/02%20Image%20Presentation/13%20Getting%20handles%20of%20swapchain%20images.cpp)

* [14 - Creating a swapchain with R8G8B8A8 format and a MAILBOX present mode](./Library/Source%20Files/02%20Image%20Presentation/14%20Creating%20a%20swapchain%20with%20R8G8B8A8%20format%20and%20a%20MAILBOX%20present%20mode.cpp)

* [15 - Acquiring a swapchain image](./Library/Source%20Files/02%20Image%20Presentation/15%20Acquiring%20a%20swapchain%20image.cpp)

* [16 - Presenting an image](./Library/Source%20Files/02%20Image%20Presentation/16%20Presenting%20an%20image.cpp)

* [17 - Destroying a swapchain](./Library/Source%20Files/02%20Image%20Presentation/17%20Destroying%20a%20swapchain.cpp)

* [18 - Destroying a presentation surface](./Library/Source%20Files/02%20Image%20Presentation/18%20Destroying%20a%20presentation%20surface.cpp)

## [Chapter 03 - Command Buffers and Synchronization](./Library/Source%20Files/03%20Command%20Buffers%20and%20Synchronization/)

* [01 - Creating a command pool](./Library/Source%20Files/03%20Command%20Buffers%20and%20Synchronization/01%20Creating%20a%20command%20pool.cpp)

* [02 - Allocating command buffers](./Library/Source%20Files/03%20Command%20Buffers%20and%20Synchronization/02%20Allocating%20command%20buffers.cpp)

* [03 - Beginning a command buffer recording operation](./Library/Source%20Files/03%20Command%20Buffers%20and%20Synchronization/03%20Beginning%20a%20command%20buffer%20recording%20operation.cpp)

* [04 - Ending a command buffer recording operation](./Library/Source%20Files/03%20Command%20Buffers%20and%20Synchronization/04%20Ending%20a%20command%20buffer%20recording%20operation.cpp)

* [05 - Resetting a command buffer](./Library/Source%20Files/03%20Command%20Buffers%20and%20Synchronization/05%20Resetting%20a%20command%20buffer.cpp)

* [06 - Resetting a command pool](./Library/Source%20Files/03%20Command%20Buffers%20and%20Synchronization/06%20Resetting%20a%20command%20pool.cpp)

* [07 - Creating a semaphore](./Library/Source%20Files/03%20Command%20Buffers%20and%20Synchronization/07%20Creating%20a%20semaphore.cpp)

* [08 - Creating a fence](./Library/Source%20Files/03%20Command%20Buffers%20and%20Synchronization/08%20Creating%20a%20fence.cpp)

* [09 - Waiting for fences](./Library/Source%20Files/03%20Command%20Buffers%20and%20Synchronization/09%20Waiting%20for%20fences.cpp)

* [10 - Resetting fences](./Library/Source%20Files/03%20Command%20Buffers%20and%20Synchronization/10%20Resetting%20fences.cpp)

* [11 - Submitting command buffers to the queue](./Library/Source%20Files/03%20Command%20Buffers%20and%20Synchronization/11%20Submitting%20command%20buffers%20to%20the%20queue.cpp)

* [12 - Synchronizing two command buffers](./Library/Source%20Files/03%20Command%20Buffers%20and%20Synchronization/12%20Synchronizing%20two%20command%20buffers.cpp)

* [13 - Checking if processing of a submitted command buffer has finished](./Library/Source%20Files/03%20Command%20Buffers%20and%20Synchronization/13%20Checking%20if%20processing%20of%20a%20submitted%20command%20buffer%20has%20finished.cpp)

* [14 - Waiting until all commands submitted to a queue are finished](./Library/Source%20Files/03%20Command%20Buffers%20and%20Synchronization/14%20Waiting%20until%20all%20commands%20submitted%20to%20a%20queue%20are%20finished.cpp)

* [15 - Waiting for all submitted commands to be finished](./Library/Source%20Files/03%20Command%20Buffers%20and%20Synchronization/15%20Waiting%20for%20all%20submitted%20commands%20to%20be%20finished.cpp)

* [16 - Destroying a fence](./Library/Source%20Files/03%20Command%20Buffers%20and%20Synchronization/16%20Destroying%20a%20fence.cpp)

* [17 - Destroying a semaphore](./Library/Source%20Files/03%20Command%20Buffers%20and%20Synchronization/17%20Destroying%20a%20semaphore.cpp)

* [18 - Freeing command buffers](./Library/Source%20Files/03%20Command%20Buffers%20and%20Synchronization/18%20Freeing%20command%20buffers.cpp)

* [19 - Destroying a command pool](./Library/Source%20Files/03%20Command%20Buffers%20and%20Synchronization/19%20Destroying%20a%20command%20pool.cpp)

## [Chapter 04 - Resources and Memory](./Library/Source%20Files/04%20Resources%20and%20Memory/)

* [01 - Creating a buffer](./Library/Source%20Files/04%20Resources%20and%20Memory/01%20Creating%20a%20buffer.cpp)

* [02 - Allocating and binding memory object to a buffer](./Library/Source%20Files/04%20Resources%20and%20Memory/02%20Allocating%20and%20binding%20memory%20object%20to%20a%20buffer.cpp)

* [03 - Setting a buffer memory barrier](./Library/Source%20Files/04%20Resources%20and%20Memory/03%20Setting%20a%20buffer%20memory%20barrier.cpp)

* [04 - Creating a buffer view](./Library/Source%20Files/04%20Resources%20and%20Memory/04%20Creating%20a%20buffer%20view.cpp)

* [05 - Creating an image](./Library/Source%20Files/04%20Resources%20and%20Memory/05%20Creating%20an%20image.cpp)

* [06 - Allocating and binding memory object to an image](./Library/Source%20Files/04%20Resources%20and%20Memory/06%20Allocating%20and%20binding%20memory%20object%20to%20an%20image.cpp)

* [07 - Setting an image memory barrier](./Library/Source%20Files/04%20Resources%20and%20Memory/07%20Setting%20an%20image%20memory%20barrier.cpp)

* [08 - Creating an image view](./Library/Source%20Files/04%20Resources%20and%20Memory/08%20Creating%20an%20image%20view.cpp)

* [09 - Creating a 2D image and view](./Library/Source%20Files/04%20Resources%20and%20Memory/09%20Creating%20a%202D%20image%20and%20view.cpp)

* [10 - Creating a layered 2D image with a CUBEMAP view](./Library/Source%20Files/04%20Resources%20and%20Memory/10%20Creating%20a%20layered%202D%20image%20with%20a%20CUBEMAP%20view.cpp)

* [11 - Mapping, updating and unmapping host-visible memory](./Library/Source%20Files/04%20Resources%20and%20Memory/11%20Mapping,%20updating%20and%20unmapping%20host-visible%20memory.cpp)

* [12 - Copying data between buffers](./Library/Source%20Files/04%20Resources%20and%20Memory/12%20Copying%20data%20between%20buffers.cpp)

* [13 - Copying data from a buffer to an image](./Library/Source%20Files/04%20Resources%20and%20Memory/13%20Copying%20data%20from%20a%20buffer%20to%20an%20image.cpp)

* [14 - Copying data from an image to a buffer](./Library/Source%20Files/04%20Resources%20and%20Memory/14%20Copying%20data%20from%20an%20image%20to%20a%20buffer.cpp)

* [15 - Using staging buffer to update a buffer with a device-local memory bound](./Library/Source%20Files/04%20Resources%20and%20Memory/15%20Using%20staging%20buffer%20to%20update%20a%20buffer%20with%20a%20device-local%20memory%20bound.cpp)

* [16 - Using staging buffer to update an image with a device-local memory bound](./Library/Source%20Files/04%20Resources%20and%20Memory/16%20Using%20staging%20buffer%20to%20update%20an%20image%20with%20a%20device-local%20memory%20bound.cpp)

* [17 - Destroying an image view](./Library/Source%20Files/04%20Resources%20and%20Memory/17%20Destroying%20an%20image%20view.cpp)

* [18 - Destroying an image](./Library/Source%20Files/04%20Resources%20and%20Memory/18%20Destroying%20an%20image.cpp)

* [19 - Destroying a buffer view](./Library/Source%20Files/04%20Resources%20and%20Memory/19%20Destroying%20a%20buffer%20view.cpp)

* [20 - Freeing a memory object](./Library/Source%20Files/04%20Resources%20and%20Memory/20%20Freeing%20a%20memory%20object.cpp)

* [21 - Destroying a buffer](./Library/Source%20Files/04%20Resources%20and%20Memory/21%20Destroying%20a%20buffer.cpp)

## [Chapter 05 - Descriptor Sets](./Library/Source%20Files/05%20Descriptor%20Sets/)

* [01 - Creating a sampler](./Library/Source%20Files/05%20Descriptor%20Sets/01%20Creating%20a%20sampler.cpp)

* [02 - Creating a sampled image](./Library/Source%20Files/05%20Descriptor%20Sets/02%20Creating%20a%20sampled%20image.cpp)

* [03 - Creating a combined image sampler](./Library/Source%20Files/05%20Descriptor%20Sets/03%20Creating%20a%20combined%20image%20sampler.cpp)

* [04 - Creating a storage image](./Library/Source%20Files/05%20Descriptor%20Sets/04%20Creating%20a%20storage%20image.cpp)

* [05 - Creating a uniform texel buffer](./Library/Source%20Files/05%20Descriptor%20Sets/05%20Creating%20a%20uniform%20texel%20buffer.cpp)

* [06 - Creating a storage texel buffer](./Library/Source%20Files/05%20Descriptor%20Sets/06%20Creating%20a%20storage%20texel%20buffer.cpp)

* [07 - Creating a uniform buffer](./Library/Source%20Files/05%20Descriptor%20Sets/07%20Creating%20a%20uniform%20buffer.cpp)

* [08 - Creating a storage buffer](./Library/Source%20Files/05%20Descriptor%20Sets/08%20Creating%20a%20storage%20buffer.cpp)

* [09 - Creating an input attachment](./Library/Source%20Files/05%20Descriptor%20Sets/09%20Creating%20an%20input%20attachment.cpp)

* [10 - Creating a descriptor set layout](./Library/Source%20Files/05%20Descriptor%20Sets/10%20Creating%20a%20descriptor%20set%20layout.cpp)

* [11 - Creating a descriptor pool](./Library/Source%20Files/05%20Descriptor%20Sets/11%20Creating%20a%20descriptor%20pool.cpp)

* [12 - Allocating descriptor sets](./Library/Source%20Files/05%20Descriptor%20Sets/12%20Allocating%20descriptor%20sets.cpp)

* [13 - Updating descriptor sets](./Library/Source%20Files/05%20Descriptor%20Sets/13%20Updating%20descriptor%20sets.cpp)

* [14 - Binding descriptor sets](./Library/Source%20Files/05%20Descriptor%20Sets/14%20Binding%20descriptor%20sets.cpp)

* [15 - Creating descriptors with a texture and a uniform buffer](./Library/Source%20Files/05%20Descriptor%20Sets/15%20Creating%20descriptors%20with%20a%20texture%20and%20a%20uniform%20buffer.cpp)

* [16 - Freeing descriptor sets](./Library/Source%20Files/05%20Descriptor%20Sets/16%20Freeing%20descriptor%20sets.cpp)

* [17 - Resetting a descriptor pool](./Library/Source%20Files/05%20Descriptor%20Sets/17%20Resetting%20a%20descriptor%20pool.cpp)

* [18 - Destroying a descriptor pool](./Library/Source%20Files/05%20Descriptor%20Sets/18%20Destroying%20a%20descriptor%20pool.cpp)

* [19 - Destroying a descriptor set layout](./Library/Source%20Files/05%20Descriptor%20Sets/19%20Destroying%20a%20descriptor%20set%20layout.cpp)

* [20 - Destroying a sampler](./Library/Source%20Files/05%20Descriptor%20Sets/20%20Destroying%20a%20sampler.cpp)

## [Chapter 06 - Render Passes and Framebuffers](./Library/Source%20Files/06%20Render%20Passes%20and%20Framebuffers/)

* [01 - Specifying attachments descriptions](./Library/Source%20Files/06%20Render%20Passes%20and%20Framebuffers/01%20Specifying%20attachments%20descriptions.cpp)

* [02 - Specifying subpass descriptions](./Library/Source%20Files/06%20Render%20Passes%20and%20Framebuffers/02%20Specifying%20subpass%20descriptions.cpp)

* [03 - Specifying dependencies between subpasses](./Library/Source%20Files/06%20Render%20Passes%20and%20Framebuffers/03%20Specifying%20dependencies%20between%20subpasses.cpp)

* [04 - Creating a render pass](./Library/Source%20Files/06%20Render%20Passes%20and%20Framebuffers/04%20Creating%20a%20render%20pass.cpp)

* [05 - Creating a framebuffer](./Library/Source%20Files/06%20Render%20Passes%20and%20Framebuffers/05%20Creating%20a%20framebuffer.cpp)

* [06 - Preparing a render pass for geometry rendering and postprocess subpasses](./Library/Source%20Files/06%20Render%20Passes%20and%20Framebuffers/06%20Preparing%20a%20render%20pass%20for%20geometry%20rendering%20and%20postprocess%20subpasses.cpp)

* [07 - Preparing a render pass and a framebuffer with color and depth attachments](./Library/Source%20Files/06%20Render%20Passes%20and%20Framebuffers/07%20Preparing%20a%20render%20pass%20and%20a%20framebuffer%20with%20color%20and%20depth%20attachments.cpp)

* [08 - Beginning a render pass](./Library/Source%20Files/06%20Render%20Passes%20and%20Framebuffers/08%20Beginning%20a%20render%20pass.cpp)

* [09 - Progressing to the next subpass](./Library/Source%20Files/06%20Render%20Passes%20and%20Framebuffers/09%20Progressing%20to%20the%20next%20subpass.cpp)

* [10 - Ending a render pass](./Library/Source%20Files/06%20Render%20Passes%20and%20Framebuffers/10%20Ending%20a%20render%20pass.cpp)

* [11 - Destroying a framebuffer](./Library/Source%20Files/06%20Render%20Passes%20and%20Framebuffers/11%20Destroying%20a%20framebuffer.cpp)

* [12 - Destroying a render pass](./Library/Source%20Files/06%20Render%20Passes%20and%20Framebuffers/12%20Destroying%20a%20render%20pass.cpp)

## [Chapter 08 - Graphics and Compute Pipelines](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/)

* [01 - Creating a shader module](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/01%20Creating%20a%20shader%20module.cpp)

* [02 - Specifying pipeline shader stages](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/02%20Specifying%20pipeline%20shader%20stages.cpp)

* [03 - Specifying pipeline vertex input state](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/03%20Specifying%20pipeline%20vertex%20input%20state.cpp)

* [04 - Specifying pipeline input assembly state](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/04%20Specifying%20pipeline%20input%20assembly%20state.cpp)

* [05 - Specifying pipeline tessellation state](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/05%20Specifying%20pipeline%20tessellation%20state.cpp)

* [06 - Specifying pipeline viewport and scissor test state](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/06%20Specifying%20pipeline%20viewport%20and%20scissor%20test%20state.cpp)

* [07 - Specifying pipeline rasterization state](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/07%20Specifying%20pipeline%20rasterization%20state.cpp)

* [08 - Specifying pipeline multisample state](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/08%20Specifying%20pipeline%20multisample%20state.cpp)

* [09 - Specifying pipeline depth and stencil state](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/09%20Specifying%20pipeline%20depth%20and%20stencil%20state.cpp)

* [10 - Specifying pipeline blend state](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/10%20Specifying%20pipeline%20blend%20state.cpp)

* [11 - Specifying pipeline dynamic states](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/11%20Specifying%20pipeline%20dynamic%20states.cpp)

* [12 - Creating a pipeline layout](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/12%20Creating%20a%20pipeline%20layout.cpp)

* [13 - Specifying graphics pipeline creation parameters](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/13%20Specifying%20graphics%20pipeline%20creation%20parameters.cpp)

* [14 - Creating a pipeline cache object](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/14%20Creating%20a%20pipeline%20cache%20object.cpp)

* [15 - Retrieving data from a pipeline cache](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/15%20Retrieving%20data%20from%20a%20pipeline%20cache.cpp)

* [16 - Merging multiple pipeline cache objects](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/16%20Merging%20multiple%20pipeline%20cache%20objects.cpp)

* [17 - Creating graphics pipelines](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/17%20Creating%20graphics%20pipelines.cpp)

* [18 - Creating a compute pipeline](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/18%20Creating%20a%20compute%20pipeline.cpp)

* [19 - Binding a pipeline object](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/19%20Binding%20a%20pipeline%20object.cpp)

* [20 - Creating a pipeline layout with a combined image sampler, a buffer and push constant ranges](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/20%20Creating%20a%20pipeline%20layout%20with%20a%20combined%20image%20sampler,%20a%20buffer%20and%20push%20constant%20ranges.cpp)

* [21 - Creating a graphics pipeline with vertex and fragment shaders, depth test enabled, and with dynamic viewport and scissor tests](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/21%20Creating%20a%20graphics%20pipeline%20with%20vertex%20and%20fragment%20shaders,%20depth%20test%20enabled,%20and%20with%20dynamic%20viewport%20and%20scissor%20tests.cpp)

* [22 - Creating multiple graphics pipelines on multiple threads](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/22%20Creating%20multiple%20graphics%20pipelines%20on%20multiple%20threads.cpp)

* [23 - Destroying a pipeline](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/23%20Destroying%20a%20pipeline.cpp)

* [24 - Destroying a pipeline cache](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/24%20Destroying%20a%20pipeline%20cache.cpp)

* [25 - Destroying a pipeline layout](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/25%20Destroying%20a%20pipeline%20layout.cpp)

* [26 - Destroying a shader module](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/26%20Destroying%20a%20shader%20module.cpp)

## [Chapter 09 - Command Recording and Drawing](./Library/Source%20Files/09%20Command%20Recording%20and%20Drawing/)

* [01 - Clearing a color image](./Library/Source%20Files/09%20Command%20Recording%20and%20Drawing/01%20Clearing%20a%20color%20image.cpp)

* [02 - Clearing a depth-stencil image](./Library/Source%20Files/09%20Command%20Recording%20and%20Drawing/02%20Clearing%20a%20depth-stencil%20image.cpp)

* [03 - Clearing render pass attachments](./Library/Source%20Files/09%20Command%20Recording%20and%20Drawing/03%20Clearing%20render%20pass%20attachments.cpp)

* [04 - Binding vertex buffers](./Library/Source%20Files/09%20Command%20Recording%20and%20Drawing/04%20Binding%20vertex%20buffers.cpp)

* [05 - Binding an index buffer](./Library/Source%20Files/09%20Command%20Recording%20and%20Drawing/05%20Binding%20an%20index%20buffer.cpp)

* [06 - Providing data to shaders through push constants](./Library/Source%20Files/09%20Command%20Recording%20and%20Drawing/06%20Providing%20data%20to%20shaders%20through%20push%20constants.cpp)

* [07 - Setting viewport state dynamically](./Library/Source%20Files/09%20Command%20Recording%20and%20Drawing/07%20Setting%20viewport%20state%20dynamically.cpp)

* [08 - Setting scissor state dynamically](./Library/Source%20Files/09%20Command%20Recording%20and%20Drawing/08%20Setting%20scissor%20state%20dynamically.cpp)

* [09 - Setting line width state dynamically](./Library/Source%20Files/09%20Command%20Recording%20and%20Drawing/09%20Setting%20line%20width%20state%20dynamically.cpp)

* [10 - Setting depth bias state dynamically](./Library/Source%20Files/09%20Command%20Recording%20and%20Drawing/10%20Setting%20depth%20bias%20state%20dynamically.cpp)

* [11 - Setting blend constants state dynamically](./Library/Source%20Files/09%20Command%20Recording%20and%20Drawing/11%20Setting%20blend%20constants%20state%20dynamically.cpp)

* [12 - Drawing a geometry](./Library/Source%20Files/09%20Command%20Recording%20and%20Drawing/12%20Drawing%20a%20geometry.cpp)

* [13 - Drawing an indexed geometry](./Library/Source%20Files/09%20Command%20Recording%20and%20Drawing/13%20Drawing%20an%20indexed%20geometry.cpp)

* [14 - Dispatching compute work](./Library/Source%20Files/09%20Command%20Recording%20and%20Drawing/14%20Dispatching%20compute%20work.cpp)

* [15 - Executing secondary command buffer inside a primary command buffer](./Library/Source%20Files/09%20Command%20Recording%20and%20Drawing/15%20Executing%20secondary%20command%20buffer%20inside%20a%20primary%20command%20buffer.cpp)

* [16 - Recording a command buffer that draws a geometry with dynamic viewport and scissor states](./Library/Source%20Files/09%20Command%20Recording%20and%20Drawing/16%20Recording%20a%20command%20buffer%20that%20draws%20a%20geometry%20with%20dynamic%20viewport%20and%20scissor%20states.cpp)

* [17 - Recording command buffers on multiple threads](./Library/Source%20Files/09%20Command%20Recording%20and%20Drawing/17%20Recording%20command%20buffers%20on%20multiple%20threads.cpp)

* [18 - Preparing a single frame of animation](./Library/Source%20Files/09%20Command%20Recording%20and%20Drawing/18%20Preparing%20a%20single%20frame%20of%20animation.cpp)

* [19 - Increasing the performance through increasing the number of separately rendered frames](./Library/Source%20Files/09%20Command%20Recording%20and%20Drawing/19%20Increasing%20the%20performance%20through%20increasing%20the%20number%20of%20separately%20rendered%20frames.cpp)

## [Chapter 10 - Helper Recipes](./Library/Source%20Files/10%20Helper%20Recipes/)

* [01 - Preparing a translation matrix](./Library/Source%20Files/10%20Helper%20Recipes/01%20Preparing%20a%20translation%20matrix.cpp)

* [02 - Preparing a rotation matrix](./Library/Source%20Files/10%20Helper%20Recipes/02%20Preparing%20a%20rotation%20matrix.cpp)

* [03 - Preparing a scaling matrix](./Library/Source%20Files/10%20Helper%20Recipes/03%20Preparing%20a%20scaling%20matrix.cpp)

* [04 - Preparing a perspective projection matrix](./Library/Source%20Files/10%20Helper%20Recipes/04%20Preparing%20a%20perspective%20projection%20matrix.cpp)

* [05 - Preparing an orthographic projection matrix](./Library/Source%20Files/10%20Helper%20Recipes/05%20Preparing%20an%20orthographic%20projection%20matrix.cpp)

* [06 - Loading texture data from a file](./Library/Source%20Files/10%20Helper%20Recipes/06%20Loading%20texture%20data%20from%20a%20file.cpp)

* [07 - Loading a 3D model from an OBJ file](./Library/Source%20Files/10%20Helper%20Recipes/07%20Loading%20a%203D%20model%20from%20an%20OBJ%20file.cpp)
### Download a free PDF

 <i>If you have already purchased a print or Kindle version of this book, you can get a DRM-free PDF version at no cost.<br>Simply click on the link to claim your free PDF.</i>
<p align="center"> <a href="https://packt.link/free-ebook/9781786468154">https://packt.link/free-ebook/9781786468154 </a> </p>