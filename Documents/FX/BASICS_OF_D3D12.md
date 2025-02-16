# Basics of D3D12 - Amélie Heinrich

## What is a Graphics API?

A **Graphics API** (Application Programming Interface) is a set of tools, protocols, and conventions that allows developers to interact with a computer's graphics hardware (the GPU) in a controlled and efficient manner. It provides the necessary interfaces to create and render visual content, such as 3D models, textures, and animations. In simpler terms, it acts as a bridge between the software (such as video games or graphics applications) and the underlying hardware (the GPU or graphics card), enabling developers to manipulate visual elements and display them on the screen.

Graphics APIs abstract the complexity of direct hardware interactions, providing higher-level operations for rendering tasks like transforming 3D objects, applying textures, and running shaders.

## What Makes D3D12 Special?

**Direct3D 12 (D3D12)** is a modern, low-level graphics API that offers enhanced control over the GPU compared to its predecessor, **Direct3D 11 (D3D11)**. This increased control comes with a higher degree of complexity, but it provides developers with numerous performance and efficiency benefits, especially in graphics-intensive applications like video games and high-end simulations.

Key features that make D3D12 special include:
- **Low-Level Control**: Direct access to hardware resources enables developers to fine-tune performance.
- **Multithreading**: D3D12 provides better support for multithreading, allowing developers to efficiently use multi-core CPUs.
- **Explicit Resource Management**: Unlike D3D11, where the driver handles resource management, D3D12 gives developers full control over memory allocation, synchronization, and resource states.
- **Advanced Graphics Features**: D3D12 supports modern graphical techniques such as ray tracing, mesh shaders, and variable rate shading, enabling cutting-edge effects and performance.
- **Performance Improvements**: The increased control and ability to optimize at the hardware level lead to significant performance improvements, especially in demanding applications.

D3D12 supports a range of features, including GPU video encoding/decoding, ray tracing, mesh shaders, variable rate shading, and work graphs, making it suitable for the latest rendering techniques in real-time graphics.

## Core Types in D3D12

Below are the essential D3D12 types that you’ll frequently interact with when using the API:

### 1. **Device** (`ID3D12Device`)
   - Represents the **GPU** and its capabilities.
   - It is the entry point for creating resources such as buffers, textures, and shaders. It also manages command queues and synchronization.
   - This object acts as the main interface for interacting with the GPU.

### 2. **Command Queue** (`ID3D12CommandQueue`)
   - A queue used to submit **commands** (such as drawing commands) to the GPU.
   - Different command queues are used for different types of tasks (e.g., **graphics**, **compute**, **copy**).
   - **Multiple command queues** can be used for concurrent GPU workloads, which improves efficiency and parallelism.

### 3. **Command List** (`ID3D12GraphicsCommandList`)
   - A **command list** records GPU commands before they are executed.
   - Commands in the list can include rendering commands, state changes, and resource bindings.
   - Once the command list is recorded, it is submitted to the **command queue** for execution.

### 4. **Fence** (`ID3D12Fence`)
   - A synchronization object used to track the completion of GPU tasks.
   - Fences are used to ensure that the GPU has finished executing a command before the CPU continues its execution, preventing race conditions and incorrect behavior.
   - A **GPU fence** can be used to synchronize between CPU and GPU operations.

### 5. **Descriptor Heap** (`ID3D12DescriptorHeap`)
   - A collection of **descriptors** that describe GPU resources such as buffers, textures, and samplers.
   - Descriptors are used to bind resources to shaders.
   - There are various types of descriptor heaps, such as those for **Constant Buffers (CBVs)**, **Shader Resource Views (SRVs)**, and **Unordered Access Views (UAVs)**.
   - Descriptor heaps are essential for resource binding, allowing efficient access to resources during rendering.

### 6. **Pipeline State Object (PSO)** (`ID3D12PipelineState`)
   - Represents the configuration of the rendering pipeline, which includes shaders, rasterizer state, blend state, and input layout.
   - PSOs define how data flows through the GPU and how resources are processed.
   - Once created, a PSO is used to configure the pipeline before rendering commands are issued, ensuring optimal performance.

### 7. **Resource** (`ID3D12Resource`)
   - A **resource** represents any data stored on the GPU, such as buffers for vertex data, textures for images, and other data structures.
   - Resources are fundamental for rendering tasks and can be either **static** (e.g., assets) or **dynamic** (e.g., render targets, compute buffers).
   - Resources can also be shared across different command lists and queues.

### 8. **Descriptor** (`D3D12_CPU_DESCRIPTOR_HANDLE` / `D3D12_GPU_DESCRIPTOR_HANDLE`)
   - A **handle** that points to a specific entry in a descriptor heap, representing a resource or data bound to a shader.
   - **CPU handles** are used by the CPU to interact with descriptor heaps, while **GPU handles** are used by the GPU during rendering.

### 9. **Resource Barrier** (`D3D12_RESOURCE_BARRIER`)
   - Specifies **state transitions** for a resource (e.g., changing a resource from **copying** to **rendering**).
   - Barriers are used to synchronize resource access, ensuring that resources are in the correct state when the GPU reads from or writes to them.

### 10. **Root Signature** (`ID3D12RootSignature`)
   - Describes how resources like buffers, textures, and samplers are bound to shaders.
   - The **root signature** specifies the layout of root parameters (such as constants or resource bindings), allowing shaders to efficiently access data.
   - Root signatures are associated with pipeline state objects, enabling efficient resource binding during rendering.

### 11. **Shader** (`ID3D12Shader`)
   - A **shader** is a program that runs on the GPU to process data (e.g., vertices, pixels).
   - Shaders are typically written in **HLSL** (High-Level Shader Language) and compiled to GPU-specific code.
   - Common types of shaders include **vertex shaders**, **pixel shaders**, **compute shaders**, etc., each responsible for different stages of the rendering pipeline.

### 12. **Swap Chain** (`IDXGISwapChain`)
   - A **swap chain** manages the presentation of rendered images to the screen.
   - It represents a **frame buffer** and supports **double or triple buffering** to reduce screen tearing.
   - The swap chain allows for **flipping** between back buffers, ensuring smooth transitions between frames.

### 13. **Heap** (`ID3D12Heap`)
   - A **heap** is a memory block on the GPU where resources can be allocated.
   - This low-level feature is useful for advanced memory management, such as dynamic resources or large allocations, and provides more flexibility in managing GPU memory.

### 14. **Query** (`ID3D12QueryHeap`)
   - A **query heap** is used to query GPU information, such as **occlusion queries** (whether an object is visible) or **timestamp queries** (measuring time taken by operations).
   - Queries are useful for performance analysis and for controlling complex rendering effects.

### 15. **Root Constants** (`D3D12_ROOT_PARAMETER_TYPE`)
   - **Root constants** are small constant values passed directly to the shader via the root signature.
   - These constants allow shaders to access essential data, like transformation matrices or other frequently used values, without the overhead of using buffers.
   - Root constants are ideal for small, immutable data.

---

## Conclusion

Direct3D 12 is a powerful, low-level graphics API that offers great flexibility and performance for high-end graphics applications. By providing developers with more control over GPU resources and execution, D3D12 enables more efficient rendering pipelines, better use of modern hardware, and access to cutting-edge features such as ray tracing and mesh shaders. Understanding the core types and how they interact is essential for leveraging the full potential of D3D12 in your applications.
