libs = {"opencv", "asio", "spdlog", "yaml-cpp", "nlohmann_json"}
set_languages("c++23")
add_requires(unpack(libs))

target("cv")
    set_kind("binary")
    add_files("src/**.cpp")
    add_includedirs("include")
    add_packages(unpack(libs))
    set_configdir("$(buildir)/$(plat)/$(arch)/$(mode)")
    add_configfiles("config/*.yaml", {onlycopy = true})