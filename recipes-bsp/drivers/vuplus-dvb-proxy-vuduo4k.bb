require vuplus-dvb-proxy.inc

KV = "4.1.45"

SRCDATE = "20250706"
SRCDATE_PR = "r1"

pkg_postinst_${PN}_append () {
	if [ ! -f $D/lib/modules/${KERNEL_VERSION}/extra/dvb-bcm.ko ]; then
		ln -s /lib/modules/${KERNEL_VERSION}/extra/dvb-bcm7278.ko $D/lib/modules/${KERNEL_VERSION}/extra/dvb-bcm.ko
	fi
}

SRC_URI[md5sum] = "4ac3cd137d81f3aa8bbcb372ba5ca411"
SRC_URI[sha256sum] = "c31ab1560810d331ea23eb1a104ffe3ab4ba8e959213ef4f66041ace19d4c797"
