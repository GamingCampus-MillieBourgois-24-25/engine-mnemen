# TECHNICAL PLANNING - Amélie Heinrich

---

## Graphics API

We have chosen **Direct3D 12** as the graphics API for a number of key reasons:

- **Mesh Shaders**: Provides more efficient and flexible geometry processing.
- **Ray Tracing**: Direct3D 12 has excellent support for real-time ray tracing, enabling high-quality lighting, shadows, and reflections.
- **Bindless Architecture**: This feature allows us to easily work with large datasets and manage resources without excessive state changes, which is beneficial for GPU-driven rendering.

We opted **against Vulkan** primarily because:

- **Vulkan’s Tiled GPU Architecture**: This is designed for high-efficiency in large, complex workloads, but does not provide any tangible benefit for our specific use case, as we are focused on a Windows-only release.
  
Thus, Direct3D 12 aligns better with our needs for GPU-driven rendering and efficient resource management.

---

## Audio API

For audio, we have selected **miniaudio**, an audio library that is:

- **Lightweight**: Easy to integrate into the engine without introducing unnecessary complexity.
- **Easy to Use**: It has a simple API that makes it convenient for handling audio playback and streaming.

This choice allows us to quickly implement the core audio functionality for the engine while maintaining a minimal footprint.

---

## Physics API

We have chosen **Jolt** as our physics library for the following reasons:

- **Lightweight**: Jolt is designed to be efficient and fast, which aligns with the performance goals of the engine.
- **Active Community**: The creator, Jorrit Rouwe, is very responsive and has cultivated a strong community, making it easier to get support and resolve issues.

Jolt provides all the necessary physics features for our project, and its active development ensures it will remain a robust option for our needs.

---

## Planned Features

### **Rendering**

- **API**: Direct3D 12 (chosen for its mesh shaders, ray tracing, and bindless architecture)
- **Pipeline**: Mesh Shading — simplifies culling and geometry generation.
- **Lighting**: Physically Based Rendering (PBR), Cascaded Shadow Maps (CSM), and Screen Space Ambient Occlusion (SSAO).
- **PostFX**: Deferred rendering, Temporal Anti-Aliasing (TAA), FXAA, Color Grading, Bloom, Depth of Field (DOF).
- **GPU Driven**: Frustum culling, Occlusion culling, and Cone culling for efficient rendering.
- **Other**: Considerations for custom shader passes and/or a shader graph.
- **Developers**: Amélie, Coralie, Axel, Alexandre.

### **Engine**

- **Windowing & Input**: SDL3 (cross-platform and easy to use).
- **Asset Loading & Compression**: NVTT (NVIDIA Texture Tools) for texture compression + Assimp for cross-format model loading.
- **ECS (Entity Component System)**: **entt**, a lightweight and easy-to-use ECS framework.
- **Scene Format**: JSON (simple to work with and widely used).
- **Profiler**: **Optick**, chosen due to previous experience and ease of integration into the project.
- **Developers**: Amélie.

### **Physics**

- **Library**: **Jolt** (used in AAA titles and very developer-friendly).
- **Core Features**: Rigidbodies, character controllers, and potentially vehicles (TBD).
- **Developers**: Bastian.

### **Audio**

- **Library**: **MiniAudio** (chosen for its lightweight nature).
- **Features**: Audio sources, listeners, and spatialized audio for 3D environments.
- **Potential Future Additions**: Audio filters (TBD).
- **Developers**: Timéo.

### **AI**

- **Library**: **Recast Navigation** (a robust pathfinding library commonly used in game AI).
- **Developers**: Jonathan.

### **Scripting**

- **Language**: Wren (C API, class based, closer to what we want to have for a game engine)
