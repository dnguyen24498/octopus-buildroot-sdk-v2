POWER_MGR_VERSION = 1.0
POWER_MGR_SITE = package/koala/power-mgr
POWER_MGR_SITE_METHOD = local

define POWER_MGR_BUILD_CMDS
	cp $(POWER_MGR_SITE)/power-mgr.c $(@D)/
	$(TARGET_CC) $(TARGET_CFLAGS) $(@D)/power-mgr.c -o $(@D)/power-mgr
endef

define POWER_MGR_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/power-mgr \
		$(TARGET_DIR)/usr/bin/power-mgr
endef

$(eval $(generic-package))