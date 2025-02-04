//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-04 10:37:18
//

#pragma once

#include <RHI/RHI.hpp>

/// @brief A ring buffer type for holding a fixed number of `Buffer::Ref` objects, used for frame management.
/// 
/// This is a typedef that creates a ring buffer with a size defined by `FRAMES_IN_FLIGHT`. It 
/// is typically used for managing buffers in a circular manner, enabling efficient resource usage
/// across multiple frames.
typedef Array<Buffer::Ref, FRAMES_IN_FLIGHT> RingBuffer;

/// @brief Enum representing different types of resources in a render pass.
/// 
/// This enum class defines the various types of resources that can be associated with a render pass, 
/// including textures, buffers, samplers, and ring buffers. Each type corresponds to a specific kind 
/// of GPU resource used during rendering.
enum class RenderPassResourceType
{
    SharedTexture,   ///< A shared texture resource
    SharedRingBuffer,///< A shared ring buffer resource
    SharedBuffer,    ///< A shared buffer resource
    SharedSampler    ///< A shared sampler resource
};

/// @brief A class representing a resource in a render pass, such as textures, buffers, and samplers.
/// 
/// This class defines a resource that can be used during a render pass. The resource can be of different 
/// types (texture, buffer, ring buffer, or sampler) and can be accessed by views. The resource is created, 
/// managed, and utilized during the rendering process.
class RenderPassResource
{
public:
    RenderPassResourceType Type;    ///< The type of resource (e.g., texture, buffer)
    Texture::Ref Texture;           ///< The associated texture reference (if applicable)
    RingBuffer RBuffer;             ///< The associated ring buffer (if applicable)
    Buffer::Ref Buffer;             ///< The associated buffer reference (if applicable)
    Sampler::Ref Sampler;           ///< The associated sampler reference (if applicable)

    /// @brief Default constructor for a render pass resource.
    RenderPassResource() = default;

    /// @brief Default destructor for a render pass resource.
    ~RenderPassResource() = default;

    /// @brief Adds a view to the render pass resource.
    /// 
    /// This method allows you to add a new view for the resource, specifying the view's type, 
    /// dimension, and format. A view represents how the resource will be accessed during rendering.
    /// 
    /// @param type The type of the view (e.g., texture, buffer)
    /// @param dimension The dimension of the view (default is `Texture`)
    /// @param format The format of the texture (default is `TextureFormat::Unknown`)
    /// @return A reference to the newly added view.
    View::Ref AddView(ViewType type, ViewDimension dimension = ViewDimension::Texture, TextureFormat format = TextureFormat::Unknown);

    /// @brief Retrieves a specific view associated with this resource.
    /// 
    /// This method retrieves a view based on its type. If the resource has multiple views, 
    /// this function will return the corresponding one.
    /// 
    /// @param type The type of view to retrieve.
    /// @return A reference to the requested view.
    View::Ref GetView(ViewType type);

    /// @brief Returns the descriptor index for the resource's view.
    /// 
    /// This function retrieves the descriptor index for the view, which can be used for 
    /// binding the resource to shaders or other parts of the pipeline.
    /// 
    /// @param type The type of view to get the descriptor for (default is `ViewType::None`).
    /// @param frameIndex The index of the frame (default is 0).
    /// @return The descriptor index of the specified view.
    int Descriptor(ViewType type = ViewType::None, int frameIndex = 0);

private:
    RHI::Ref ParentRHI;            ///< The parent RHI used to manage the resource
    Vector<View::Ref> Views;      ///< A vector of views associated with the resource

    friend class RendererTools;    ///< Friend class for managing the creation of render pass resources
};

/// @brief A utility class for managing render pass resources.
/// 
/// The `RendererTools` class provides methods for creating, retrieving, and managing shared resources 
/// used in render passes. These resources include textures, buffers, samplers, and ring buffers. The tools 
/// facilitate the management and reuse of resources across multiple render passes in the rendering pipeline.
class RendererTools
{
public:
    /// @brief Initializes the renderer tools with a specified rendering hardware interface (RHI).
    /// 
    /// This method sets up the necessary environment for managing render pass resources.
    /// It should be called before using any resource management functions.
    /// 
    /// @param rhi The RHI reference to be used for managing resources.
    static void Init(RHI::Ref rhi);

    /// @brief Creates a shared texture resource for use in a render pass.
    /// 
    /// This method creates a texture resource that can be shared across multiple render passes.
    /// 
    /// @param name The name of the texture resource.
    /// @param desc The description of the texture, including properties such as size and format.
    /// @return A reference to the created render pass resource.
    static Ref<RenderPassResource> CreateSharedTexture(const String& name, TextureDesc desc);

    /// @brief Creates a shared ring buffer resource for use in a render pass.
    /// 
    /// This method creates a ring buffer resource that can be used in a render pass for storing 
    /// temporary data.
    /// 
    /// @param name The name of the ring buffer resource.
    /// @param size The size of the ring buffer in bytes.
    /// @return A reference to the created render pass resource.
    static Ref<RenderPassResource> CreateSharedRingBuffer(const String& name, UInt64 size);

    /// @brief Creates a shared read-write buffer resource for use in a render pass.
    /// 
    /// This method creates a read-write buffer resource that can be used for writing and reading 
    /// data during the render pass.
    /// 
    /// @param name The name of the buffer resource.
    /// @param size The size of the buffer in bytes.
    /// @param stride The stride (or element size) of the buffer.
    /// @return A reference to the created render pass resource.
    static Ref<RenderPassResource> CreateSharedRWBuffer(const String& name, UInt64 size, UInt64 stride);

    /// @brief Creates a shared sampler resource for use in a render pass.
    /// 
    /// This method creates a sampler resource that can be used for texture sampling during rendering.
    /// 
    /// @param name The name of the sampler resource.
    /// @param filter The filter mode to use for sampling the texture.
    /// @param address The address mode to use for texture coordinates outside the texture bounds.
    /// @param mips Whether to enable mipmap filtering (default is false).
    /// @return A reference to the created render pass resource.
    static Ref<RenderPassResource> CreateSharedSampler(const String& name, SamplerFilter filter, SamplerAddress address, bool mips = false);

    /// @brief Retrieves an existing render pass resource by name.
    /// 
    /// This method looks up a previously created resource by its name. It is useful for reusing 
    /// existing resources across different parts of the rendering pipeline.
    /// 
    /// @param name The name of the resource to retrieve.
    /// @return A reference to the requested render pass resource.
    static Ref<RenderPassResource> Get(const String& name);

private:
    /// @brief A structure holding data for managing renderer tools, including the RHI and resources.
    struct RendererToolsData
    {
        RHI::Ref RHI;  ///< The rendering hardware interface used by the tools.
        UnorderedMap<String, Ref<RenderPassResource>> Resources; ///< A map of resources by their name.
    };

    static RendererToolsData sData;  ///< Static data used by the `RendererTools` class.
};

