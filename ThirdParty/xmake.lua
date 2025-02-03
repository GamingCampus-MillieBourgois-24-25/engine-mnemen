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
