################################################################################
#
# display-mgr
#
################################################################################

DISPLAY_MGR_SITE = $(DISPLAY_MGR_PKGDIR)/src
DISPLAY_MGR_SITE_METHOD = local

DISPLAY_MGR_DEPENDENCIES = lvgl

define DISPLAY_MGR_BUILD_CMDS
	$(TARGET_CC) $(@D)/main.c -o $(@D)/display-mgr \
		$(TARGET_CFLAGS) \
		-I$(STAGING_DIR)/usr/include/lvgl \
		-L$(STAGING_DIR)/usr/lib \
		-llvgl
endef

define DISPLAY_MGR_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/display-mgr \
		$(TARGET_DIR)/usr/bin/display-mgr
endef

define DISPLAY_MGR_INSTALL_INIT_SYSV
	$(INSTALL) -D -m 0755 $(DISPLAY_MGR_PKGDIR)/S99display-mgr \
		$(TARGET_DIR)/etc/init.d/S99display-mgr
endef

$(eval $(generic-package))