//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 11:03:43
//

#pragma once

#include <RHI/Resource.hpp>
#include <RHI/DescriptorHeap.hpp>
#include <RHI/Texture.hpp>

/// @brief Enum representing the type of view.
/// @details Specifies how a resource (e.g., texture or buffer) is accessed or used in the graphics pipeline.
enum class ViewType
{
    None, ///< No view type (default value).
    RenderTarget, ///< View used as a render target.
    DepthTarget, ///< View used as a depth target.
    ShaderResource, ///< View used as a shader resource (read access).
    Storage ///< View used for storage (write access).
};

/// @brief Enum representing the dimension of a resource view.
/// @details Specifies the dimensionality of the resource being viewed. This helps interpret how the resource is used.
enum class ViewDimension
{
    Buffer, ///< View is a buffer.
    Texture, ///< View is a texture.
    TextureCube ///< View is a cube map texture.
};

/// @brief Constant representing the mip level to view all available mip levels.
/// @details Used when the view should cover all mip levels of a texture.
constexpr UInt64 VIEW_ALL_MIPS = 0xFFFFFFFF;

/// @brief Represents a view into a resource (texture or buffer) for various shader and rendering operations.
/// @details The `View` class provides the functionality to create and manage views into resources, such as textures or buffers.
///          These views can be used in shaders for reading or writing data, depending on the view type. Views can be
///          created for different dimensions (buffer, texture, or texture cube) and support various formats and mip levels.
class View
{
public:
    /// @brief Type alias for a reference to a `View` object.
    using Ref = Ref<View>;

    /// @brief Constructs a view into a resource (texture or buffer).
    /// @param device A reference to the `Device` object, used to create the view.
    /// @param heaps A reference to `DescriptorHeaps`, which manage the descriptors used by the view.
    /// @param resource A reference to the `Resource` object (buffer or texture) the view is associated with.
    /// @param type The type of the view, such as render target, depth target, shader resource, or storage.
    /// @param dimension The dimension of the resource view (e.g., buffer, texture, or texture cube). Default is `ViewDimension::Texture`.
    /// @param format The format of the texture, e.g., `TextureFormat::RGBA8`. Default is `TextureFormat::Unknown`.
    /// @param mip The mip level to view. Default is `VIEW_ALL_MIPS` (all mip levels).
    /// @param depthSlice The depth slice of the texture view. Default is 0.
    /// @details This constructor creates a new view into the specified resource with the given parameters.
    View(Device::Ref device, DescriptorHeaps heaps, ::Ref<Resource> resource, ViewType type, ViewDimension dimension = ViewDimension::Texture, TextureFormat format = TextureFormat::Unknown, UInt64 mip = VIEW_ALL_MIPS, UInt64 depthSlice = 0);

    /// @brief Destructor that cleans up the view object.
    /// @details The destructor releases any resources or descriptors associated with the view.
    ~View();

    /// @brief Gets the descriptor associated with this view.
    /// @return A `DescriptorHeap::Descriptor` object representing the descriptor for this view.
    /// @details This descriptor is used in shaders to access the resource associated with this view.
    DescriptorHeap::Descriptor GetDescriptor() { return mDescriptor; }

    /// @brief Gets the resource associated with this view.
    /// @return A reference to the `Resource` object associated with this view.
    /// @details This function returns the resource (texture or buffer) that this view is based on.
    ::Ref<Resource> GetParentResource() { return mParent; }

    /// @brief Gets the type of the view (e.g., render target, depth target, shader resource, or storage).
    /// @return The `ViewType` of the current view.
    /// @details This function returns the view type, which determines how the view will be used in the pipeline.
    ViewType GetType() { return mType; }

    /// @brief Gets the dimension of the view (buffer, texture, or texture cube).
    /// @return The `ViewDimension` of the current view.
    /// @details This function returns the dimension of the view, which dictates how the resource is interpreted.
    ViewDimension GetDimension() { return mDimension; }

private:
    /// @brief The descriptor associated with this view, used to access the resource in shaders.
    DescriptorHeap::Descriptor mDescriptor;

    /// @brief The resource (texture or buffer) that this view points to.
    ::Ref<Resource> mParent = nullptr;

    /// @brief The type of this view (render target, depth target, etc.).
    ViewType mType;

    /// @brief The dimension of the view (buffer, texture, or texture cube).
    ViewDimension mDimension;
};
