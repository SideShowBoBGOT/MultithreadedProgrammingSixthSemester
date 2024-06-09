message(STATUS "Conan: Using CMakeDeps conandeps_legacy.cmake aggregator via include()")
message(STATUS "Conan: It is recommended to use explicit find_package() per dependency instead")

find_package(CLI11)
find_package(tl-optional)
find_package(magic_enum)
find_package(fmt)

set(CONANDEPS_LEGACY  CLI11::CLI11  tl::optional  magic_enum::magic_enum  fmt::fmt )