add_rules("mode.release", "mode.debug")
set_languages("cxx20")

add_requires("asio", "catch2")

target("Server")
    set_kind("binary")
    add_files("Server/**.cpp")
    add_includedirs("Network/include")
    add_packages("asio")

target("UnitTest")
    set_kind("binary")
    add_files("UnitTest/**.cpp")
    add_includedirs("Network/include")
    add_includedirs("Common/include")
    add_includedirs("Server/include")
    add_packages("catch2")
    