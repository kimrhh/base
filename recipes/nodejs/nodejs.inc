DESCRIPTION = "nodeJS Evented I/O for V8 JavaScript"
HOMEPAGE = "http://nodejs.org"
LICENSE = "MIT BSD Artistic-2.0"

inherit make c c++

##
## NOTE: This recipe compiles for mingw32, but is not runtime tested.
##       It is only used to provide the nodejs sources in the SDK, so
##       that npm can compile modules against those.
##

DEPENDS_HOST_OS = "libdl librt"
DEPENDS_HOST_OS:HOST_LIBC_mingw = ""
DEPENDS = "openssl libz libm libcares-dev libuv-dev ${DEPENDS_HOST_OS}"
RECIPE_TYPES = "machine native sdk"

SRC_URI = " \
    http://nodejs.org/dist/v${PV}/node-v${PV}.tar.gz \
    file://0001-initialize-arm_version-variable-in-gyp.patch \
"
SRC_URI:>HOST_LIBC_mingw = " file://0001-fix-compilation-for-mingw-sdk.patch file://nameser.h"
S = "${SRCDIR}/node-v${PV}"

CONFIGURE_NAMESER = ""
CONFIGURE_NAMESER:HOST_LIBC_mingw = "do_configure_nameser"
do_configure[prefuncs] = "${CONFIGURE_NAMESER}"
do_configure_nameser() {
    cp ${SRCDIR}/nameser.h ${S}/src/nameser.h
}

# Node is way too cool to use proper autotools
NODEJS_CONF_ARCH=""
NODEJS_CONF_OS=""
do_configure[prefuncs] += "do_configure_map_nodejs_arch"
def do_configure_map_nodejs_arch(d):
    import re
    a = d.getVar('TARGET_ARCH', True)
    if   re.match('p(pc|owerpc)(|64)', a):  a = 'ppc'
    elif re.match('i.86', a):               a = 'ia32'
    elif re.match('x86_64', a):             a = 'x64'
    elif re.match('arm', a):                a = 'arm'
    d.set('NODEJS_CONF_ARCH', a)

    a = d.getVar('TARGET_OS', True)
    if   'linux' in a:    a = 'linux'
    elif 'mingw' in a:    a = 'win'
    elif 'win'   in a:    a = 'win'
    d.set('NODEJS_CONF_OS', a)

EXTRA_OECONF += "-DNODE_WANT_INTERNALS=0"
SYSROOT         = "${BUILD_SYSROOT}"
SYSROOT:machine = "${MACHINE_SYSROOT}"
do_configure () {
    # $TARGET_ARCH settings don't match --dest-cpu settings
   ./configure --prefix=${prefix} \
               --without-snapshot \
               --without-npm \
               --without-perfctr \
               --without-etw \
               --without-dtrace \
               --shared-openssl \
               --shared-openssl-includes=${SYSROOT}${includedir} \
               --shared-openssl-libpath=${SYSROOT}${libdir} \
               --shared-zlib \
               --shared-zlib-includes=${SYSROOT}${includedir} \
               --shared-zlib-libpath=${SYSROOT}${libdir} \
               --shared-cares \
               --shared-cares-includes=${SYSROOT}${includedir} \
               --shared-cares-libpath=${SYSROOT}${libdir} \
               --shared-libuv \
               --shared-libuv-includes=${SYSROOT}${includedir} \
               --shared-libuv-libpath=${SYSROOT}${libdir} \
               --dest-cpu=${NODEJS_CONF_ARCH} \
               --dest-os=${NODEJS_CONF_OS}
}

do_compile () {
    oe_runmake BUILDTYPE=Release
}

do_install () {
    oe_runmake install DESTDIR=${D} PREFIX=${prefix}
}

DEPENDS_${PN} = "libz openssl libstdc++ libm libgcc libpthread libc libcares libuv ${DEPENDS_HOST_OS}"
RDEPENDS_${PN} = "libz openssl libstdc++ libm libgcc libpthread libc libcares libuv ${DEPENDS_HOST_OS}"

PACKAGES_${PN} =+ "${PN}-dev"
FILES_${PN}-dev = "${includedir}/node ${datadir}"

PACKAGES =+ "${PN}-systemtap"
FILES_${PN}-systemtap = "${datadir}/systemtap"

PACKAGES =+ "${PN}-source"
FILES_${PN}-source = "${datadir}/src"
do_unpack[postfuncs] += "do_unpack_node_source"
do_unpack_node_source () {
    cp -r ${S} ${SRCDIR}/node-source
}

do_install[postfuncs] += "do_install_node_source"
do_install_node_source () {
    install -d ${D}${datadir}/src
    cp -r ${SRCDIR}/node-source ${D}${datadir}/src/nodejs-${PV}
    ln -s nodejs-${PV} ${D}${datadir}/src/nodejs
}
