--
-- > Notice: Amélie Heinrich @ 2024
-- > Create Time: 2025-02-03 09:57:56
--

add_rules("mode.debug", "mode.release", "mode.releasedbg")

includes("ThirdParty")

target("Mnemen")
    set_kind("static")
    set_languages("c++20")
    set_encodings("utf-8")
    add_deps("spdlog")
    
    add_files("Engine/**.cpp") -- Might need to change this for multi-platform or multi-API
    add_includedirs("Engine/Mnemen", "ThirdParty/SDL3/include", "ThirdParty/spdlog/include", { public = true })
    add_linkdirs("ThirdParty/SDL3/lib")
    add_defines("GLM_ENABLE_EXPERIMENTAL", "USE_PIX", "GLM_FORCE_DEPTH_ZERO_TO_ONE")

    if is_plat("windows") then
        add_syslinks("user32",
                     "gdi32",
                     "kernel32",
                     "d3d12",
                     "dxgi",
                     "SDL3.lib")
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
        os.cp("Binaries/*", "$(buildir)/$(plat)/$(arch)/$(mode)/")
    end)

target("Editor")
    set_kind("binary")
    set_languages("c++17")
    set_rundir(".")
    set_encodings("utf-8")

    add_files("Editor/*.cpp")
    add_includedirs("Engine", "Editor", "ThirdParty/SDL3/include", "ThirdParty/spdlog/include")
    add_deps("Mnemen")

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
    add_includedirs("Engine", "Runtime", "ThirdParty/SDL3/include", "ThirdParty/spdlog/include")
    add_deps("Mnemen")

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

