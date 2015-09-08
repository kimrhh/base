DESCRIPTION = "JavaScript package manager"
HOMEPAGE = "https://www.npmjs.com/"
LICENSE = "Artistic-2.0"
RECIPE_TYPES = "native sdk-cross canadian-cross"

inherit make

DEPENDS = "native:nodejs"
SRC_URI = "https://github.com/npm/npm/archive/v${PV}.zip"
FILES_${PN} = "${bindir}/npm ${bindir}/npm-cross ${libdir}/node_modules"
RDEPENDS_${PN} = "sdk:nodejs target:nodejs-source"
RDEPENDS_${PN}:native = "nodejs"

do_configure() {
    ./configure --prefix=${D}
}

do_install_wrapper_sdk = "do_install_wrapper"
do_install_wrapper_sdk:native = ""
do_install[postfuncs] += "${do_install_wrapper_sdk}"
do_install_wrapper () {
    # Using real tabs below, since they are stripped by bash/cat/EOF
	cat <<-'EOF' > ${D}/${bindir}/npm-cross
		#!/bin/bash

		SCRIPT_SRC=$(dirname $BASH_SOURCE)
		SCRIPT_DIR=$(readlink -fn $SCRIPT_SRC)
		NODE_DIR=$SCRIPT_DIR/../${TARGET_ARCH}/sysroot/${target_datadir}/src/nodejs

		AR=${TARGET_PREFIX}ar \
		CC=${TARGET_PREFIX}cc \
		CXX=${TARGET_PREFIX}g++ \
		LINK=${TARGET_PREFIX}g++ \
		npm_config_arch=${TARGET_CPU} \
		npm_config_nodedir=$NODE_DIR \
		npm_config_userconfig=$SCRIPT_DIR/../.npmrc \
		npm_config_prefix=$SCRIPT_DIR/../ \
		npm $@
	EOF
	chmod +x ${D}/${bindir}/npm-cross
}
