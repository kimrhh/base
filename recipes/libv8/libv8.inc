DESCRIPTION = "V8 is Google's open source high-performance JavaScript engine, written in C++ and used in Google Chrome, the open source browser from Google."
LICENSE = "BSD"
DEPENDS = "native:gyp libpthread libdl librt"
RECIPE_TYPES = "machine native sdk"

inherit make c++ library

SRC_URI = " \
           git://chromium.googlesource.com/v8/v8;protocol=https;${SRC_REV} \
           file://0001-avoid-thin-static-archives.patch \
"

DEPENDS_${PN} = "libpthread libdl librt libgcc libm libstdc++"
RDEPENDS_${PN} = "libpthread libdl librt libgcc libm libstdc++"

PYTHON_PATH = "${BUILD_SYSROOT}/lib/python"

V8_TARGET_ARCH = ""
do_configure[prefuncs] += "do_configure_map_v8_arch"
def do_configure_map_v8_arch(d):
    import re
    a = d.getVar('TARGET_ARCH', True)
    if re.match('i.86', a):              a = 'ia32'
    elif re.match('x86_64', a):            a = 'x64'
    elif re.match('arm', a):                a = 'arm'
    d.set('V8_TARGET_ARCH', a)
    a = "{}.release".format(a)
    d.set('V8_TARGET', a)

UNPACK_CMD_tar_gz = "tar zx --no-same-owner --transform='s!^!${PN}-${PV}/!' -f %s"

do_configure() {
    export PYTHONPATH=${PYTHON_PATH}
    ./build/gyp_v8 \
                   --depth=. \
                   -f make \
                   --generator-output=out \
                   -Dwerror= \
                   -Dv8_target_arch=${V8_TARGET_ARCH} \
                   -Dcomponent=shared_library \
                   -Dsoname_version=${PV} \
                   -Dconsole=readline \
                   -Dclang=0 \
                   -Dhost_clang=0 \
                   -Dv8_enable_i18n_support=no \
                   -Dlinux_use_gold_flags=0 \
                   -Dv8_use_snapshot=0
}

do_compile() {
    oe_runmake -C out builddir=out/Release V=1 BUILDTYPE=Release libv8.so.${PV}
}

do_install() {
    # install -libv8 shared object
    install -d ${D}${libdir}

    # install -debug debug object
    install -m0755 ${S}/out/out/Release/lib.target/libv8.so.${PV} \
        ${D}${libdir}/
    ln -s libv8.so.${PV} ${D}${libdir}/libv8.so

    # install -dev header files
    install -d ${D}/${includedir}
    install -m0644 ${S}/include/*.h ${D}/${includedir}/

    # install -dev static libs
    install -m0644 ${S}/out/out/Release/obj.target/tools/gyp/libv8*.a \
        ${D}${libdir}/
}
