DESCRIPTION = "yaml-cpp is a YAML parser and emitter in C++ matching the YAML 1.2 spec."
LICENSE = "MIT"

inherit cmake c++ library

DEPENDS = "libpthread boost-dev"
SRC_URI = "https://github.com/jbeder/yaml-cpp/archive/release-${PV}.tar.gz"
S = "${SRCDIR}/yaml-cpp-release-${PV}"
EXTRA_OE_CMAKE_CONF = "-DBUILD_SHARED_LIBS=ON"

LIBRARY_VERSION = "0"
DEPENDS_${PN} = "libpthread libgcc-s libm libstdc++"
RDEPENDS_${PN} = "libpthread libgcc-s libm libstdc++"
