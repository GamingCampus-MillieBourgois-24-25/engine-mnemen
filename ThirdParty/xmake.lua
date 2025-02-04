--
-- > Notice: Amélie Heinrich @ 2024
-- > Create Time: 2025-02-03 11:55:51
--

target("spdlog")
    set_kind("static")
    set_languages("c++20")
    set_encodings("utf-8")
    add_defines("SPDLOG_COMPILED_LIB")
    add_files("spdlog/src/*.cpp")
    add_includedirs("spdlog/include")

target("ImGui")
    set_kind("static")
    add_includedirs("SDL3/include")
    add_syslinks("ThirdParty/SDL3/lib/SDL3.lib")
    add_files("ImGui/imgui_demo.cpp",
              "ImGui/imgui_draw.cpp",
              "ImGui/imgui_tables.cpp",
              "ImGui/imgui_widgets.cpp", 
              "ImGui/imgui.cpp", 
              "ImGui/backends/imgui_impl_sdl3.cpp",
              "ImGui/backends/imgui_impl_dx12.cpp")
    add_headerfiles("ImGui/*.h")
    add_includedirs("imgui/")

target("STB")
    set_kind("static")
    add_files("stb.c")

target("meshopt")
    set_kind("static")
    add_files("meshopt/src/*.cpp")
    add_headerfiles("meshop/src/*.h")

target("Jolt")
    set_kind("static")
    set_languages("c++17")
    add_files("Jolt/Jolt/**.cpp")
    add_headerfiles("Jolt/Jolt/**.h")
    add_includedirs("Jolt")

target("miniaudio")
    set_kind("static")
    set_languages("c17")
    add_headerfiles("miniaudio/miniaudio.h")
    add_files("miniaudio.c")

target("Detour")
    set_kind("static")
    set_languages("c++17")
    add_headerfiles("Recast/Detour/Include/*.h")
    add_files("Recast/Detour/Source/*.cpp")
    add_includedirs("Recast/Detour/Include")

target("DetourCrowd")
    set_kind("static")
    set_languages("c++17")
    add_headerfiles("Recast/DetourCrowd/Include/*.h")
    add_files("Recast/DetourCrowd/Source/*.cpp")
    add_includedirs("Recast/DetourCrowd/Include", "Recast/Detour/Include")

target("DetourTileCache")
    set_kind("static")
    set_languages("c++17")
    add_headerfiles("Recast/DetourTileCache/Include/*.h")
    add_files("Recast/DetourTileCache/Source/*.cpp")
    add_includedirs("Recast/DetourTileCache/Include", "Recast/Detour/Include")

target("Recast")
    set_kind("static")
    set_languages("c++17")
    add_headerfiles("Recast/Recast/Include/*.h")
    add_files("Recast/Recast/Source/*.cpp")
    add_includedirs("Recast/Recast/Include")

target("DetourDebugUtils")
    set_kind("static")
    set_languages("c++17")
    add_headerfiles("Recast/DebugUtils/Include/*.h")
    add_files("Recast/DebugUtils/Source/*.cpp")
    add_includedirs("Recast/DebugUtils/Include", "Recast/Detour/Include", "Recast/DetourCrowd/Include", "Recast/Recast/Include", "Recast/DetourTileCache/Include")
