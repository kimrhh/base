DESCRIPTION = "libuv is a multi-platform support library with a focus on asynchronous I/O."
LICENSE = "MIT BSD Artistic-2.0"
RECIPE_TYPES = "machine sdk native"

inherit c autotools library

SRC_URI = "http://dist.libuv.org/dist/v${PV}/libuv-v${PV}.tar.gz"
S = "${SRCDIR}/${PN}-v${PV}"

LIBRARY_VERSION = "11"
DEPENDS_HOST_OS = "libdl librt"
DEPENDS_HOST_OS:HOST_LIBC_mingw = ""
DEPENDS = "${DEPENDS_HOST_OS}"
DEPENDS_${PN} = "libc ${DEPENDS_HOST_OS}"
RDEPENDS_${PN} = "libc ${DEPENDS_HOST_OS}"

EXPORT_LIBTOOLIZE = ""
EXPORT_LIBTOOLIZE:native = "export _lt_pkgdatadir=${BUILD_SYSROOT}/share/libtool"

do_configure[prefuncs] += "do_configure_autogen"
do_configure_autogen() {
    ${EXPORT_LIBTOOLIZE}
    sh autogen.sh
}
