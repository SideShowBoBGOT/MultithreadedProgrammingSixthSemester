conan create . --build=missing -sbuild_type=Debug
conan create . --build=missing -sbuild_type=Release
conan install . --build=missing -sbuild_type=Debug
conan install . --build=missing -sbuild_type=Release