require vuplus-dvb-proxy.inc

KV = "3.14.28"

SRCDATE = "20250706"
SRCDATE_PR = "r1"

pkg_postinst_${PN}_append () {
	if [ ! -f $D/lib/modules/${KERNEL_VERSION}/extra/dvb-bcm.ko ]; then
		ln -s /lib/modules/${KERNEL_VERSION}/extra/dvb-bcm7252s.ko $D/lib/modules/${KERNEL_VERSION}/extra/dvb-bcm.ko
	fi
}

SRC_URI[md5sum] = "3a8adc60eacb0e02ee45c4619333876d"
SRC_URI[sha256sum] = "d64fc179c1a8c2b33cc34775e50b95405ccfc6f7353b1492a5e320735f106eca"
