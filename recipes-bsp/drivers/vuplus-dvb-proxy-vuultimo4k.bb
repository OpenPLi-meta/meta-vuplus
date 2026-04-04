require vuplus-dvb-proxy.inc

KV = "3.14.28"

SRCDATE = "20250706"
SRCDATE_PR = "r1"

pkg_postinst_${PN}_append () {
	if [ ! -f $D/lib/modules/${KERNEL_VERSION}/extra/dvb-bcm.ko ]; then
		ln -s /lib/modules/${KERNEL_VERSION}/extra/dvb-bcm7444.ko $D/lib/modules/${KERNEL_VERSION}/extra/dvb-bcm.ko
	fi
}

SRC_URI[md5sum] = "80a777b9dd55773028ffd24780445dfe"
SRC_URI[sha256sum] = "80ced172df7a61488267bff6fa545ca33659ccf01084871b18b0fb23819ed995"
