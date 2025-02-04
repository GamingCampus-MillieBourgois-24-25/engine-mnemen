# Mnemen Engine

## Graphics API

We decided to go with Direct3D 12 for a few reasons -- it gives us access to mesh shaders, raytracing, and mostly, bindless architecture which facilitates GPU driven rendering.
We didn't go for Vulkan since its very tiled GPU oriented architecture doesn't benefit us at all since we'll be shipping the engine on Windows only.

## Audio API

We decided to go with miniaudio because it's very lightweight and easy to use.

## Physics API

We decided to go with Jolt because it's also very lightweight and easy to use. Also because the creator of the library, Jorrit Rouwe, has a very active community and often answers questions about Jolt.

## Planned features

### Rendering

- API: DirectX12
- Pipeline: Mesh Shading (makes culling and geometry generation easier)
- Lighting: PBR, CSM, SSAO
- PostFX: Deferred, TAA/FXAA, Color Grading, Bloom, DOF
- GPU driven: Frustum/Occlusion/Cone culling
- Other: Custom shader passes? Shader graph?
- Developers: Amelie, Coralie, Axel, Alexandre

### Engine

- Windowing, input: SDL3 (cross platform and easy to use)
- Asset loading, compressing: NVTT + Assimp (cross format and allows BC7 compression)
- ECS: entt (very easy to use)
- Scene Format: json (easier to use too)
- Profiler: Optick (I have more experience with Optick and it's easier to integrate)
- Developers: Amelie

### Physics

- Library: Jolt (used in AAA and very easy to use)
- Features: Rigidbodies, characters, vehicles (?)
- Developrs: Bastian

### Audio

- Library: MiniAudio
- Audio sources, audio listeners, spatialized audio
- Filters?
- Developers: Timéo

### AI

- Library: Recast Navigation
- Developrs: Jonathan

### Scripting

- Language: TBD
