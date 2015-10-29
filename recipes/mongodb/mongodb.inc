DESCRIPTION = "MongoDB is the next-generation database that lets you create applications never before possible."
LICENSE = "AGPL-3.0 Apache-2.0"

inherit scons c++ auto-package-utils sysvinit

DEPENDS = " \
           libv8 \
           libv8-dev \
           openssl \
           libpthread \
           libpcre \
           pcre-libpcrecpp \
           libpcap \
           libz \
           libsnappy \
           libsnappy-dev \
           libstemmer \
           libstemmer-dev \
           libyaml-cpp \
           libyaml-cpp-dev \
           gperftools-dev \
           gperftools-libtcmalloc \
           gperftools-libtcmalloc-dev \
           boost-dev \
           libprogram-options \
           libsystem \
           libfilesystem \
           libchrono \
           libthread \
           asio-dev \
"

SRC_URI = " \
    https://fastdl.mongodb.org/src/mongodb-src-r${PV}.tar.gz \
    file://init \
    file://0001-fix-compilation-errors-with-v8-3.25.patch \
    file://0001-fix-missing-search-for-system-asio-libs.patch \
    file://0001-propagate-execution-environment-into-SCons.patch \
    file://0001-fix-missing-boost-library-in-third-party-src.patch \
    file://0001-remove-char-arguments-to-boost-mutex-constructors.patch \
    file://0001-include-operation-context-to-allow-dereferencing.patch \
    file://0001-remove-debug-flag-from-build.patch \
"

S = "${SRCDIR}/mongodb-src-r${PV}/"

EXTRA_OESCONS = "--ssl \
                 --wiredtiger=off \
                 --use-system-all \
                 --js-engine=v8-3.25 \
                 --disable-warnings-as-errors \
                 --release \
                 TARGET_ARCH=arm \
                 TARGET_OS=linux \
                 LINKFLAGS=-L${MACHINE_SYSROOT}/usr/lib \
                 all \
"

AUTO_PACKAGE_UTILS = "mongo mongod mongoperf mongos mongosniff"

RECIPE_FLAGS += "mongodb_db_path mongodb_sysvinit_start mongodb_sysvinit_stop"
SYSVINIT_SCRIPT_mongodb            = "mongod"
DEFAULT_USE_mongodb_sysvinit_start = "70"
DEFAULT_USE_mongodb_sysvinit_stop  = "0"
DEFAULT_USE_mongodb_db_path        = ""

do_install[postfuncs] =+ "do_install_init"
do_install_init[expand] = 3 # Expand empty use flag
do_install_init() {
    sysvinit_install_script ${SRCDIR}/init ${SYSVINIT_SCRIPT_mongodb}

    if [ -n "${USE_mongodb_db_path}" ]; then
        install -d ${D}${sysconfdir}/default
        echo MONGOD_DB_PATH=${USE_mongodb_db_path} >> ${D}${sysconfdir}/default/mongod
    fi
}

FILES_${PN}-mongod += "${sysconfdir}/"

RDEPENDS_COMMON = " \
                   libpcre \
                   libpcrecpp \
                   libprogram-options \
                   libfilesystem \
                   libthread \
                   libsystem \
                   libchrono \
                   libyaml-cpp \
                   libv8 \
                   gperftools-libtcmalloc \
                   openssl \
                   librt \
                   libdl \
                   libstdc++ \
                   libm \
                   libgcc \
                   libpthread \
                   libc \
"

DEPENDS_${PN}-mongo       = "${RDEPENDS_COMMON}"
DEPENDS_${PN}-mongod      = "${RDEPENDS_COMMON} libsnappy"
DEPENDS_${PN}-mongoperf   = "${RDEPENDS_COMMON} libsnappy"
DEPENDS_${PN}-mongos      = "${RDEPENDS_COMMON} libsnappy"
DEPENDS_${PN}-mongosniff  = "${RDEPENDS_COMMON} libpcap libsnappy"
RDEPENDS_${PN}-mongo      = "${RDEPENDS_COMMON}"
RDEPENDS_${PN}-mongod     = "${RDEPENDS_COMMON} libsnappy"
RDEPENDS_${PN}-mongoperf  = "${RDEPENDS_COMMON} libsnappy"
RDEPENDS_${PN}-mongos     = "${RDEPENDS_COMMON} libsnappy"
RDEPENDS_${PN}-mongosniff = "${RDEPENDS_COMMON} libpcap libsnappy"
