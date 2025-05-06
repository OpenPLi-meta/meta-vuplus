SECTION = "base"
LICENSE = "CLOSED"
#require conf/license/license-close.inc

PROVIDES += "vuplus-bluetooth-util"
RPROVIDES:${PN} += "vuplus-bluetooth-util"

PR="${SRCDATE}.${SRCDATE_PR}"

SRC_URI = " \
    http://downloads.openpli.org/archive/vuplus/vuplus-bluetooth-util-${MACHINE}-${PR}.tar.gz \
    file://vuplus-bluetooth.conf \
"

FILESEXTRAPATHS:prepend := "${THISDIR}/vuplus-bluetooth-util:"

S="${WORKDIR}/vuplus-bluetooth-util-${MACHINE}"

do_install() {
	install -d ${D}${bindir}
	install -m 0755 ${S}/* ${D}${bindir}
}

do_package_qa() {
}

PACKAGE_ARCH = "${MACHINE_ARCH}"
FILES:${PN}="/"

do_install() {
	install -d ${D}${bindir}
	install -m 0755 ${S}/bsa_server ${D}${bindir}

	install -d ${D}/usr/local/modules
	install -m 0644 ${S}/btusb.ko ${D}/usr/local/modules/btusb.ko
	install -m 0644 ${S}/bthid.ko ${D}/usr/local/modules/bthid.ko

	if [ -n "${FIRMWARE_FILE}" ]; then
		install -d ${D}/lib/firmware/${FIRMWARE_PATH}
		install -m 0644 ${S}/${FIRMWARE_FILE} ${D}/lib/firmware/${FIRMWARE_PATH}
	fi

	install -d ${D}${sysconfdir}/modprobe.d
	install -m 0664 ${S}/../vuplus-bluetooth.conf ${D}${sysconfdir}/modprobe.d
}

INHIBIT_PACKAGE_STRIP = "1"
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
INSANE_SKIP:${PN} = "already-stripped"
