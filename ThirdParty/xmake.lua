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
