add_rules("mode.release", "mode.debug")
set_languages("cxx20")

add_requires("asio")

target("Server")
    set_kind("binary")
    add_files("Server/**.cpp")
    add_includedirs("Network/include")
    add_packages("asio")
    