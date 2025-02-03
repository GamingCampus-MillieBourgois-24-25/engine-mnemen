# Rendering

- API: DirectX12 (because it gives access to mesh shading)
- Pipeline: Mesh Shading (makes culling and geometry generation easier)
- Lighting: PBR, CSM, SSAO
- PostFX: Deferred, TAA/FXAA, Color Grading, Bloom, DOF
- GPU driven: Frustum/Occlusion/Cone culling
- Other: Custom shader passes? Shader graph?

# Engine

- Windowing, input: SDL3 (cross platform and easy to use)
- Asset loading, compressing: NVTT + Assimp (cross format and allows BC7 compression)
- ECS: entt (very easy to use)
- Scene Format: json (easier to use too)
- Profiler: Optick (I have more experience with Optick and it's easier to integrate)

# Physics

- Library: Jolt (used in AAA and very easy to use)
- Features: Rigidbodies, characters, vehicles (?)

# Audio

- Library: TBD (FMOD? MiniAudio? XAudio2?)
- Audio sources, audio listeners, spatialized audio
- Filters?

# Scripting

- Language: TBD

# AI

- Library: Recast Navigation
