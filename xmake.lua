---@diagnostic disable: undefined-global, unused-local

set_project("check")
set_version("0.0.1")

add_rules("mode.debug", "mode.releasedbg", "mode.release", "mode.minsizerel")
add_rules("plugin.compile_commands.autoupdate", { outputdir = "build" })

target("check")
do
  set_kind("shared")
  set_languages("c17", "cxx20")
  set_warnings("allextra", "pedantic", "error")
  -- set_pcxxheader("$(projectdir)/src/pch.hpp")
  add_configfiles("$(projectdir)/src/config.h.in")
  add_files("$(projectdir)/src/*.c")
  add_includedirs("$(buildir)")
  -- add_packages("pacman::nlohmann-json", "dbg-macro")
end
