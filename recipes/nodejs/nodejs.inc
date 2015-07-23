DESCRIPTION = "nodeJS Evented I/O for V8 JavaScript"
HOMEPAGE = "http://nodejs.org"
LICENSE = "MIT BSD Artistic-2.0"

inherit make c c++

DEPENDS = "openssl libz librt libm libdl libv8 libv8-dev"
RECIPE_TYPES = "machine native sdk"

SRC_URI = " \
    http://nodejs.org/dist/v${PV}/node-v${PV}.tar.gz \
    file://0001-initialize-arm_version-variable-in-gyp.patch \
"
S = "${SRCDIR}/node-v${PV}"

# Node is way too cool to use proper autotools
NODEJS_CONF_ARCH=""
do_configure[prefuncs] += "do_configure_map_nodejs_arch"
def do_configure_map_nodejs_arch(d):
    import re
    a = d.getVar('TARGET_ARCH', True)
    if   re.match('p(pc|owerpc)(|64)', a):  a = 'ppc'
    elif re.match('i.86$', a):              a = 'ia32'
    elif re.match('x86_64$', a):            a = 'x64'
    elif re.match('arm', a):                a = 'arm'
    d.set('NODEJS_CONF_ARCH', a)

EXTRA_OECONF += "-DNODE_WANT_INTERNALS=0"
SYSROOT         = "${BUILD_SYSROOT}"
SYSROOT:machine = "${MACHINE_SYSROOT}"
do_configure () {
    # $TARGET_ARCH settings don't match --dest-cpu settings
   ./configure --prefix=${prefix} \
               --without-snapshot \
               --without-npm \
               --shared-openssl \
               --shared-openssl-includes=${SYSROOT}${includedir} \
               --shared-openssl-libpath=${SYSROOT}${libdir} \
               --shared-v8 \
               --shared-v8-includes=${SYSROOT}${includedir} \
               --shared-v8-libpath=${SYSROOT}${libdir} \
               --shared-zlib \
               --shared-zlib-includes=${SYSROOT}${includedir} \
               --shared-zlib-libpath=${SYSROOT}${libdir} \
               --dest-cpu=${NODEJS_CONF_ARCH} \
               --dest-os=linux
}

do_compile () {
    oe_runmake BUILDTYPE=Release
}

do_install () {
    oe_runmake install DESTDIR=${D} PREFIX=${prefix}
}

DEPENDS_${PN} = "libz libv8 openssl libdl librt libstdc++ libm libgcc libpthread libc"
RDEPENDS_${PN} = "libz libv8 openssl libdl librt libstdc++ libm libgcc libpthread libc"

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
