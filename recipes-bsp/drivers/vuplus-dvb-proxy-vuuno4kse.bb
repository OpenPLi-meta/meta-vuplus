require vuplus-dvb-proxy.inc

KV = "4.1.20"

SRCDATE = "20250706"
SRCDATE_PR = "r1"

pkg_postinst_${PN}_append () {
	if [ ! -f $D/lib/modules/${KERNEL_VERSION}/extra/dvb-bcm.ko ]; then
		ln -s /lib/modules/${KERNEL_VERSION}/extra/dvb-bcm7252sse.ko $D/lib/modules/${KERNEL_VERSION}/extra/dvb-bcm.ko
	fi
}

SRC_URI[md5sum] = "a7b4ac6ecf943d8bd76141c6cfa8b541"
SRC_URI[sha256sum] = "4905361097b7ae8aa5a8d445747a8b1df19a74d8880aef7a7c53e366f116bc6b"
