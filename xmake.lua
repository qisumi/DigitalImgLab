add_requires("vcpkg::cimg")

target("main")
  set_plat(os.host())
  set_arch(os.arch())
  set_toolchains("mingw")
  on_load(function (target)
    cprint("${green}building:%s-%s",os.arch(),os.host())
  end)
  set_kind("binary")
  add_files("src/main.cpp")
  set_basename("main")
  add_packages("vcpkg::cimg")
  add_links("gdi32")
  set_targetdir("bin/")
  set_languages("cxx14")
  set_optimize("faster")
  