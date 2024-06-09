import os

from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain

class CompressorRecipe(ConanFile):
    name = "exam_mpi"
    version = "1.0.0"
    user = "user"
    channel = "stable"
    settings = ("os", "build_type", "arch", "compiler")

    def layout(self):
        self.folders.build = "conan_external"
        self.folders.generators = os.path.join(self.folders.build, "generators")

    def generate(self):
        deps = CMakeDeps(self)
        deps.check_components_exist = True
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def requirements(self):
        self.test_requires("cli11/2.4.2")
        self.test_requires("tl-optional/1.1.0")
        self.test_requires("magic_enum/0.9.5")
        self.test_requires("fmt/10.2.1")
