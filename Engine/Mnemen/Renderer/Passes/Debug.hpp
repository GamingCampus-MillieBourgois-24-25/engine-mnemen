//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-11 11:31:06
//

#pragma once

#include <Renderer/RenderPass.hpp>

/// @class Debug
/// @brief A class that handles debug rendering for visualizing geometries and scenes.
///
/// This class provides static methods for drawing basic geometric shapes such as lines, 
/// triangles, boxes, spheres, and frustums, which are used for debugging purposes in 3D scenes.
/// It extends the `RenderPass` class and allows for the baking and rendering of debug visuals.
class Debug : public RenderPass
{
public:
    /// @struct Line
    /// @brief A structure to represent a line in 3D space.
    ///
    /// This structure contains information about the start point, end point, and color
    /// of a line to be rendered.
    struct Line
    {
        glm::vec3 From; ///< The starting point of the line.
        glm::vec3 To; ///< The ending point of the line.
        glm::vec3 Color; ///< The color of the line.
    };

    /// @brief Constructs a Debug object.
    ///
    /// This constructor initializes the `Debug` object with the provided render hardware interface (RHI).
    ///
    /// @param rhi The render hardware interface reference.
    Debug(RHI::Ref rhi);

    /// @brief Destructor for the Debug class.
    ~Debug() = default;

    /// @brief Prepares the scene for baking.
    ///
    /// This function is intended to bake or process the scene data. It is currently empty and can
    /// be customized further if necessary.
    ///
    /// @param scene The scene to be baked.
    void Bake(Scene& scene) {}

    /// @brief Renders the debug visuals for the given frame and scene.
    ///
    /// This function renders the debug visuals (lines, shapes, etc.) for the provided frame and scene.
    ///
    /// @param frame The frame to render for.
    /// @param scene The scene to render for.
    void Render(const Frame& frame, ::Ref<Scene> scene) override;

    /// @brief Draws a line from one point to another with a given color.
    ///
    /// This static function draws a line between two 3D points.
    ///
    /// @param from The starting point of the line.
    /// @param to The ending point of the line.
    /// @param color The color of the line (default is white).
    static void DrawLine(glm::vec3 from, glm::vec3 to, glm::vec3 color = glm::vec3(1.0f));

    /// @brief Draws a triangle defined by three 3D points.
    ///
    /// This static function draws a triangle between three 3D points.
    ///
    /// @param a The first vertex of the triangle.
    /// @param b The second vertex of the triangle.
    /// @param c The third vertex of the triangle.
    /// @param color The color of the triangle (default is white).
    static void DrawTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 color = glm::vec3(1.0f));

    /// @brief Draws an arrow from one point to another with a given color and size.
    ///
    /// This static function draws an arrow from a start point to an end point with an optional size.
    ///
    /// @param from The starting point of the arrow.
    /// @param to The ending point of the arrow.
    /// @param color The color of the arrow (default is white).
    /// @param size The size of the arrowhead (default is 0.1f).
    static void DrawArrow(glm::vec3 from, glm::vec3 to, glm::vec3 color = glm::vec3(1.0f), float size = 0.1f);

    /// @brief Draws a unit-sized box at a given transformation with a specified color.
    ///
    /// This static function draws a unit box (size 1) at the specified transformation matrix.
    ///
    /// @param transform The transformation matrix (position, rotation, scale).
    /// @param color The color of the box (default is white).
    static void DrawUnitBox(glm::mat4 transform, glm::vec3 color = glm::vec3(1.0f));

    /// @brief Draws a box with a specified transform and size.
    ///
    /// This static function draws a box at the specified transformation and with the given size.
    ///
    /// @param transform The transformation matrix.
    /// @param min The minimum corner of the box.
    /// @param max The maximum corner of the box.
    /// @param color The color of the box (default is white).
    static void DrawBox(glm::mat4 transform, glm::vec3 min, glm::vec3 max, glm::vec3 color = glm::vec3(1.0f));

    /// @brief Draws a camera frustum using view and projection matrices.
    ///
    /// This static function draws a camera frustum based on the provided view and projection matrices.
    ///
    /// @param view The camera's view matrix.
    /// @param projection The camera's projection matrix.
    /// @param color The color of the frustum (default is white).
    static void DrawFrustum(glm::mat4 view, glm::mat4 projection, glm::vec3 color = glm::vec3(1.0f));

    /// @brief Draws a frustum using a combined projection-view matrix.
    ///
    /// This static function draws a camera frustum based on a combined projection-view matrix.
    ///
    /// @param projview The combined projection-view matrix.
    /// @param color The color of the frustum (default is white).
    static void DrawFrustum(glm::mat4 projview, glm::vec3 color = glm::vec3(1.0f));

    /// @brief Draws a coordinate system at a given transform with a specified size.
    ///
    /// This static function draws the X, Y, and Z axes to represent a coordinate system.
    ///
    /// @param transform The transformation matrix (position, rotation, scale).
    /// @param size The size of the coordinate system axes.
    static void DrawCoordinateSystem(glm::mat4 transform, float size);

    /// @brief Draws a sphere at a given center with a specified radius and color.
    ///
    /// This static function draws a sphere with a given center and radius.
    ///
    /// @param center The center of the sphere.
    /// @param radius The radius of the sphere.
    /// @param color The color of the sphere (default is white).
    /// @param level The level of detail (default is 3).
    static void DrawSphere(glm::vec3 center, float radius, glm::vec3 color = glm::vec3(1.0f), int level = 3);

    /// @brief Draws a ring at a given center and normal with a specified radius and color.
    ///
    /// This static function draws a ring with a specified center, normal, radius, and color.
    ///
    /// @param center The center of the ring.
    /// @param normal The normal vector for the ring's orientation.
    /// @param radius The radius of the ring.
    /// @param color The color of the ring (default is white).
    /// @param level The level of detail (default is 32).
    static void DrawRing(glm::vec3 center, glm::vec3 normal, float radius, glm::vec3 color = glm::vec3(1.0f), int level = 32);

    /// @brief Draws multiple rings at a given center with a specified radius and color.
    ///
    /// This static function draws a series of rings around a center.
    ///
    /// @param center The center of the rings.
    /// @param radius The radius of the rings.
    /// @param color The color of the rings (default is white).
    /// @param level The level of detail (default is 32).
    static void DrawRings(glm::vec3 center, float radius, glm::vec3 color = glm::vec3(1.0f), int level = 32);

private:
    static constexpr UInt32 MAX_LINES = 5192 * 16; ///< The maximum number of lines that can be drawn.

    /// @brief Recursively draws a wireframe unit sphere.
    ///
    /// This static function draws a unit sphere using recursion for better detail.
    ///
    /// @param matrix The transformation matrix for the sphere.
    /// @param inColor The color of the sphere.
    /// @param inDir1 The first direction for recursion.
    /// @param inDir2 The second direction for recursion.
    /// @param inDir3 The third direction for recursion.
    /// @param inLevel The level of recursion.
    static void DrawWireUnitSphereRecursive(glm::mat4 matrix, glm::vec3 inColor, glm::vec3 inDir1, glm::vec3 inDir2, glm::vec3 inDir3, int inLevel);

    /// @struct LineVertex
    /// @brief A structure for the vertices of lines to be drawn.
    ///
    /// This structure holds the position and color of each vertex of the lines.
    struct LineVertex
    {
        glm::vec3 Position; ///< The position of the vertex.
        glm::vec3 Color; ///< The color of the vertex.
    };

    /// @struct Data
    /// @brief A structure that holds the debug drawing data.
    ///
    /// This structure contains the list of lines to be drawn, the graphics pipeline,
    /// and the buffers for transferring and storing vertex data across frames.
    static struct Data
    {
        Vector<Line> Lines; ///< A vector holding the lines to be drawn.
        GraphicsPipeline::Ref Pipeline; ///< The graphics pipeline used for rendering.
        Array<Buffer::Ref, FRAMES_IN_FLIGHT> TransferBuffer; ///< Transfer buffers for each frame.
        Array<Buffer::Ref, FRAMES_IN_FLIGHT> VertexBuffer; ///< Vertex buffers for each frame.
    } sData;

    UInt64 mLineCount = 0; ///< The current number of lines being tracked for rendering.
};
