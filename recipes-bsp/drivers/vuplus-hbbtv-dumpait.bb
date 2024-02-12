DESCRIPTION = "dumpait"

PRIORITY = "required"
LICENSE = "GPL-3.0-only"
LIC_FILES_CHKSUM = "file://LICENSE.GPLv3;md5=5ed852a46d22220a8b07a68e564d84c7"

DEPENDS = "libdvbsi++"

SRC_URI = "git://code.vuplus.com/git/dumpait.git;branch=master"
S = "${WORKDIR}/git"
DESTDIR = "enigma2/python/Plugins/Extensions/HbbTV"

inherit gitpkgv
SRCREV = "1adfdd7dadb760a8ff2107dce60e2f99ca74fa5d"
PV = "git${SRCPV}"
PR = "r2"
PKGV = "git${GITPKGV}"
PACKAGE_ARCH = "${MACHINE_ARCH}"

RREPLACES:${PN} = "vuplus-opera-dumpait"
RCONFLICTS:${PN} = "vuplus-opera-dumpait"

inherit autotools-brokensep pkgconfig

do_install() {
	install -d ${D}/usr/lib/${DESTDIR}
	install -m 0755 ${S}/src/dumpait ${D}/usr/lib/${DESTDIR}
}

FILES:${PN} = "${libdir}/${DESTDIR}/dumpait"
FILES:${PN}-dbg += "${libdir}/${DESTDIR}/.debug/dumpait"
