--
-- > Notice: Amélie Heinrich @ 2025
-- > Create Time: 2025-02-03 09:57:56
--

add_rules("mode.debug", "mode.release", "mode.releasedbg")

includes("ThirdParty")

target("Mnemen")
    set_kind("static")
    set_languages("c++20")
    set_encodings("utf-8")
    add_deps("spdlog",
             "ImGui",
             "ImGuizmo",
             "STB",
             "meshopt",
             "miniaudio",
             "Jolt",
             "Recast",
             "Detour",
             "DetourCrowd",
             "DetourDebugUtils",
             "Wren",
             "Wren++")
    
    add_files("Engine/**.cpp") -- Might need to change this for multi-platform or multi-API
    add_headerfiles("Engine/**.hpp")
    add_includedirs("Engine/Mnemen",
                    "ThirdParty/SDL3/include",
                    "ThirdParty/spdlog/include",
                    "ThirdParty/glm",
                    "ThirdParty/DirectX/include",
                    "ThirdParty/DXC/Include",
                    "ThirdParty/ImGui/",
                    "ThirdParty/ImGui/backends",
                    "ThirdParty/PIX/include",
                    "ThirdParty/",
                    "ThirdParty/nvtt/",
                    "ThirdParty/meshopt/src",
                    "ThirdParty/Jolt",
                    "ThirdParty/miniaudio",
                    "ThirdParty/Recast/Recast/Include",
                    "ThirdParty/Recast/Detour/Include",
                    "ThirdParty/Recast/DetourCrowd/Include",
                    "ThirdParty/Recast/DetourTileCache/Include",
                    "ThirdParty/Recast/DebugUtils/Include",
                    "ThirdParty/Wren/src/include",
                    "ThirdParty/JSON/single_include")
    add_linkdirs("ThirdParty/SDL3/lib")
    add_defines("GLM_ENABLE_EXPERIMENTAL", "USE_PIX", "GLM_FORCE_DEPTH_ZERO_TO_ONE")

    if is_plat("windows") then
        add_syslinks("user32",
                     "gdi32",
                     "kernel32",
                     "d3d12",
                     "dxgi",
                     "SDL3.lib",
                     "Comdlg32.lib",
                     "Shlwapi.lib",
                     "ThirdParty/PIX/lib/WinPixEventRuntime.lib",
                     "ThirdParty/DXC/lib/dxcompiler.lib",
                     "ThirdParty/nvtt/lib64/nvtt30205.lib",
                     "ThirdParty/Assimp/lib/assimp-vc143-mtd.lib")
    end    

    if is_mode("debug") then
        set_symbols("debug")
        set_optimize("none")
        add_defines("MNEMEN_DEBUG")
    end
    if is_mode("release") then
        set_symbols("hidden")
        set_optimize("fastest")
        set_strip("all")
    end
    if is_mode("releasedbg") then
        set_symbols("debug")
        set_optimize("fastest")
        set_strip("all")
    end

    -- Copy DLLs in build folder
    before_link(function (target)
        if not os.exists("$(buildir)/$(plat)/$(arch)/$(mode)/Assets/") then
            os.mkdir("$(buildir)/$(plat)/$(arch)/$(mode)/Assets/")
        end
        if not os.exists("$(buildir)/$(plat)/$(arch)/$(mode)/.cache/") then
            os.mkdir("$(buildir)/$(plat)/$(arch)/$(mode)/.cache/")
        end
        os.cp("Binaries/*", "$(buildir)/$(plat)/$(arch)/$(mode)/")
        os.cp("Assets/*", "$(buildir)/$(plat)/$(arch)/$(mode)/Assets/")
        os.cp(".cache/*", "$(buildir)/$(plat)/$(arch)/$(mode)/.cache/")
    end)

target("Editor")
    set_kind("binary")
    set_languages("c++17")
    set_rundir(".")
    set_encodings("utf-8")

    add_files("Editor/*.cpp")
    add_headerfiles("Editor/**.hpp")
    add_includedirs("Engine",
                    "Engine/Mnemen",
                    "Editor",
                    "ThirdParty/SDL3/include",
                    "ThirdParty/spdlog/include",
                    "ThirdParty/glm",
                    "ThirdParty/ImGui/",
                    "ThirdParty/DirectX/include",
                    "ThirdParty/",
                    "ThirdParty/nvtt/",
                    "ThirdParty/Jolt",
                    "ThirdParty/miniaudio",
                    "ThirdParty/Recast/Recast/Include",
                    "ThirdParty/Recast/Detour/Include",
                    "ThirdParty/Recast/DetourCrowd/Include",
                    "ThirdParty/Recast/DetourTileCache/Include",
                    "ThirdParty/Recast/DebugUtils/Include",
                    "ThirdParty/Wren/src/include",
                    "ThirdParty/JSON/single_include")
    add_deps("Mnemen")
    add_defines("GLM_ENABLE_EXPERIMENTAL")

    if is_mode("debug") then
        set_symbols("debug")
        set_optimize("none")
        add_defines("EDITOR_DEBUG")
    end
    if is_mode("release") then
        set_symbols("hidden")
        set_optimize("fastest")
        set_strip("all")
    end
    if is_mode("releasedbg") then
        set_symbols("debug")
        set_optimize("fastest")
        set_strip("all")
    end

target("Runtime")
    set_kind("binary")
    set_languages("c++17")
    set_rundir(".")
    set_encodings("utf-8")

    add_files("Runtime/*.cpp")
    add_headerfiles("Runtime/**.hpp")
    add_includedirs("Engine",
                    "Engine/Mnemen",
                    "Runtime",
                    "ThirdParty/SDL3/include",
                    "ThirdParty/spdlog/include",
                    "ThirdParty/glm",
                    "ThirdParty/ImGui/",
                    "ThirdParty/DirectX/include",
                    "ThirdParty/",
                    "ThirdParty/nvtt/",
                    "ThirdParty/Jolt",
                    "ThirdParty/miniaudio",
                    "ThirdParty/Recast/Recast/Include",
                    "ThirdParty/Recast/Detour/Include",
                    "ThirdParty/Recast/DetourCrowd/Include",
                    "ThirdParty/Recast/DetourTileCache/Include",
                    "ThirdParty/Recast/DebugUtils/Include",
                    "ThirdParty/Wren/src/include",
                    "ThirdParty/JSON/single_include")
    add_deps("Mnemen")
    add_defines("GLM_ENABLE_EXPERIMENTAL")

    if is_mode("debug") then
        set_symbols("debug")
        set_optimize("none")
        add_defines("RUNTIME_DEBUG")
    end
    if is_mode("release") then
        set_symbols("hidden")
        set_optimize("fastest")
        set_strip("all")
    end
    if is_mode("releasedbg") then
        set_symbols("debug")
        set_optimize("fastest")
        set_strip("all")
    end

