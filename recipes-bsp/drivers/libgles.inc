SECTION = "base"
LICENSE = "CLOSED"
PACKAGE_ARCH = "${MACHINE_ARCH}"

PROVIDES = "virtual/libgles2 virtual/egl"
RPROVIDES:${PN} = "libEGL.so libGLESv2.so libdvb_base.so libdvb_client.so libnxpl.so libv3ddriver.so"

PV="15.1"
PR="${SRCDATE}.${SRCDATE_PR}"

S = "${WORKDIR}/libgles-${MACHINE}"

# Closed source
SRC_URI = "http://downloads.openpli.org/archive/vuplus/libgles-${MACHINE}-${PV}-${PR}.tar.gz"

SRC_URI += "git://github.com/KhronosGroup/EGL-Registry;branch=main;name=eglh;subpath=api;destsuffix=${S}/egl;protocol=https"
SRCREV_eglh = "90b78b0662e2f0548cfd1926fb77bf628933541b"

SRC_URI += "git://github.com/KhronosGroup/OpenGL-Registry;branch=main;name=openglh;subpath=api;destsuffix=${S}/opengl;protocol=https"
SRCREV_openglh = "8261133c2645aaeef880c5f5a6e327f399bc9d04"

SRCREV_FORMAT = "libgles"

# BEGL patch for eglplatform.h
SRC_URI:append:vuduo2 = " file://vuxxo2-BEGL.patch"
SRC_URI:append:vusolo2 = " file://vuxxo2-BEGL.patch"
SRC_URI:append:vusolose = " file://vuxxo2-BEGL.patch"

INHIBIT_PACKAGE_STRIP = "1"
PRIVATE_LIBS:${PN} = "libEGL.so \
libv3ddriver.so \
libGLESv2.so"

do_install() {
	install -d ${D}${libdir}
	install -m 0755 ${S}/lib/*.so ${D}${libdir}
	ln -s libv3ddriver.so ${D}${libdir}/libEGL.so
	ln -s libv3ddriver.so ${D}${libdir}/libGLESv2.so
	install -d ${D}${includedir}
	cp -a --no-preserve=ownership ${S}/include/* ${D}${includedir}/
}

do_package_qa() {
}

FILES:${PN} = "/usr/lib/*"
FILES:${PN}-dev = "/usr/include/*"

# skip the check: we purposedly have two symlinks in the main package
INSANE_SKIP:${PN} += "already-stripped dev-so ldflags"

FILESEXTRAPATHS:prepend := "${THISDIR}/vuplus-libgles:"
