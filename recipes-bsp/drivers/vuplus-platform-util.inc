SECTION = "base"
LICENSE = "CLOSED"

PV="15.1"
PR="${SRCDATE}.${SRCDATE_PR}"

SRC_URI = " \
    http://downloads.openpli.org/archive/vuplus/platform-util-${MACHINE}-${PV}-${SRCDATE}.${SRCDATE_PR}.tar.gz \
    file://${INITSCRIPT_NAME}.sysvinit \
"

S="${WORKDIR}/platform-util-${MACHINE}"

do_install() {
	install -d ${D}${bindir}
	install -m 0755 ${S}/* ${D}${bindir}
	if [ -f ${WORKDIR}/${INITSCRIPT_NAME}.sysvinit ]; then
		install -d ${D}${INIT_D_DIR}
		install -m 0755 ${WORKDIR}/${INITSCRIPT_NAME}.sysvinit ${D}${INIT_D_DIR}/${INITSCRIPT_NAME}
	fi
}

do_package_qa[noexec] = "1"

PACKAGE_ARCH := "${MACHINE_ARCH}"
FILES_${PN}="/"

inherit update-rc.d

INITSCRIPT_PARAMS = "start 65 S . stop 90 0 ."
INITSCRIPT_NAME = "vuplus-platform-util"

INSANE_SKIP_${PN} = "already-stripped"

FILESEXTRAPATHS_prepend := "${THISDIR}/vuplus-platform-util:"

