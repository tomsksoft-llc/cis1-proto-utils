from conans import ConanFile
from conans import CMake


class Cis1ProtoUtils(ConanFile):
    name = "cis1_proto_utils"
    version = "0.0.2"
    description = "Shared classes for cis1 protocols."
    author = "MokinIA <mia@tomsksoft.com>"
    generators = "cmake"
    settings = "os", "arch", "compiler", "build_type"
    exports = []
    exports_sources = ["CMakeLists.txt", "include/*", "src/*"]
    requires = ("boost_system/1.69.0@bincrafters/stable",
                "boost_asio/1.69.0@bincrafters/stable",
                "rapidjson/1.1.0@bincrafters/stable",
                "cis1_tpl/0.0.1@tomsksoft/cis1")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("*.h", dst="include/cis1_proto_utils", src="include")
        self.copy("libcis1_proto_utils.a", dst="lib", src="lib")
        self.copy("libcis1_proto_utils.lib", dst="lib", src="lib")

    def package_info(self):
        self.cpp_info.libs = ["cis1_proto_utils"]
