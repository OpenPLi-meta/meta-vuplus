DESCRIPTION = "Hardware drivers for ${MACHINE}"
SECTION = "base"
PRIORITY = "required"
LICENSE = "CLOSED"

PV = "${KV}+${SRCDATE}"

SRC_URI = "http://downloads.openpli.org/archive/vuplus/vuplus-dvb-modules-${DOWNLOADMACHINE}-${KV}-${SRCDATE}.tar.gz"

S = "${WORKDIR}"

INHIBIT_PACKAGE_STRIP = "1"

inherit module-base

FILES:${PN} = "/"

do_install() {
	install -d ${D}/lib/modules/${KV}/extra
	for f in *.ko; do
		install -m 0644 ${WORKDIR}/$f ${D}/lib/modules/${KV}/extra/$f;
	done
	install -d ${D}/${sysconfdir}/modules-load.d
	for i in `ls | grep \\.ko | sed -e 's/.ko//g'`; do
		echo $i >> ${D}/${sysconfdir}/modules-load.d/_vuplus.conf
	done
}

pkg_postinst:${PN} () {
        if [ -d /proc/stb ]; then
		depmod -a
        fi
        true
}

FILESEXTRAPATHS:prepend := "${THISDIR}/vuplus-dvb-modules:"
