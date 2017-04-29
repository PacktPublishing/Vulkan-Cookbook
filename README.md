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

<hr>

# [Recipes Library](./Library/Source%20Files/)

## [Chapter 01 - Instance and Devices](./Library/Source%20Files/01%20Instance%20and%20Devices/)

## [Chapter 02 - Image Presentation](./Library/Source%20Files/02%20Image%20Presentation/)

## [Chapter 03 - Command Buffers and Synchronization](./Library/Source%20Files/03%20Command%20Buffers%20and%20Synchronization/)

## [Chapter 04 - Resources and Memory](./Library/Source%20Files/04%20Resources%20and%20Memory/)

## [Chapter 05 - Descriptor Sets](./Library/Source%20Files/05%20Descriptor%20Sets/)

## [Chapter 06 - Render Passes and Framebuffers](./Library/Source%20Files/06%20Render%20Passes%20and%20Framebuffers/)

## [Chapter 08 - Graphics and Compute Pipelines](./Library/Source%20Files/08%20Graphics%20and%20Compute%20Pipelines/)

## [Chapter 09 - Command Recording and Drawing](./Library/Source%20Files/09%20Command%20Recording%20and%20Drawing/)

## [Chapter 10 - Helper Recipes](./Library/Source%20Files/10%20Helper%20Recipes/)
