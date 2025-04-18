add_rules("mode.release", "mode.debug")
set_languages("cxx20")

add_requires("asio", "catch2", "raylib")


target("Common")
    set_kind("static")
    add_files("Common/*.cpp")
    add_includedirs("Common/include")

target("Server")
    set_symbols("debug")
    set_kind("binary")
    add_files("Server/**.cpp")
    add_includedirs("Network/include")
    add_includedirs("Common/include")
    add_includedirs("Server/include")
    add_packages("asio", "raylib")
    add_deps("Common")


target("Client")
    add_includedirs("Common/include")
    add_includedirs("Network/include")
    add_includedirs("Client/include")
    add_files("Client/*.cpp")
    add_deps("Common")
    add_packages("asio", "raylib")

target("UnitTest")
    set_kind("binary")
    add_files("UnitTest/**.cpp")
    add_includedirs("Network/include")
    add_includedirs("Common/include")
    add_includedirs("Server/include")
    add_packages("catch2")
    add_deps("Common")
    
task("test")
    set_menu({usage="xmake test", description="Server & Client test", options={}})
    on_run(function(target)
        import("core.base.task")
        task.run("build", {}, "all")

        task.run("run", {detach=false}, "Server")
        task.run("run", {detach=true}, "Client")
    end)
