add_requires("opencv")

target("cv")
    set_kind("binary")
    add_files("src/*.cpp")
    add_includedirs("include")
    add_packages("opencv")