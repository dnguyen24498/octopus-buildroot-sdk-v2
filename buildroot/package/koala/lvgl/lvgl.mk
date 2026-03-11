LVGL_VERSION = 9.5.0
LVGL_SITE = https://github.com/lvgl/lvgl/archive/refs/tags
LVGL_SOURCE = v$(LVGL_VERSION).tar.gz

LVGL_LICENSE = MIT
LVGL_LICENSE_FILES = LICENSE
LVGL_INSTALL_STAGING = YES

define LVGL_COPY_CONFIG
	cp $(@D)/lv_conf_template.h $(@D)/lv_conf.h
endef
LVGL_POST_EXTRACT_HOOKS += LVGL_COPY_CONFIG

LVGL_CONF_OPTS += \
	-DLV_CONF_INCLUDE_SIMPLE=ON \
	-DLV_BUILD_DEMOS=OFF \
	-DLV_BUILD_EXAMPLES=OFF

$(eval $(cmake-package))