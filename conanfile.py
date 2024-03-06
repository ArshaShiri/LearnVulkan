from conan import ConanFile
from conan.tools.cmake import cmake_layout
from conan.tools.cmake import CMakeToolchain

class VulkanTutorial(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"

    def requirements(self):
        self.requires("glfw/3.3.8")
        self.requires("glm/cci.20230113")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        
        # From https://github.com/cpp-best-practices
        tc.extra_cxxflags = ["-Wall"]
        tc.extra_cxxflags.append("-Wextra") # reasonable and standard
        tc.extra_cxxflags.append("-Wshadow") # warn the user if a variable declaration shadows one from a parent context
        tc.extra_cxxflags.append("-Wnon-virtual-dtor") # warn the user if a class with virtual functions has a non-virtual destructor. This helps catch hard to track down memory errors
        tc.extra_cxxflags.append("-Wold-style-cast") # warn for c-style casts
        tc.extra_cxxflags.append("-Wcast-align") # warn for potential performance problem casts
        tc.extra_cxxflags.append("-Wunused") # warn on anything being unused
        tc.extra_cxxflags.append("-Woverloaded-virtual") # warn if you overload (not override) a virtual function
        tc.extra_cxxflags.append("-Wpedantic") # warn if non-standard C++ is used
        tc.extra_cxxflags.append("-Wconversion") # warn on type conversions that may lose data
        tc.extra_cxxflags.append("-Wsign-conversion") # warn on sign conversions
        tc.extra_cxxflags.append("-Wnull-dereference") # warn if a null dereference is detected
        tc.extra_cxxflags.append("-Wdouble-promotion") # warn if float is implicit promoted to double
        tc.extra_cxxflags.append("-Wformat=2") # warn on security issues around functions that format output (ie printf)
        tc.extra_cxxflags.append("-Wimplicit-fallthrough") # warn on statements that fallthrough without an explicit annotation

        #tc.extra_cxxflags.append("-Werror")

        tc.generate()
