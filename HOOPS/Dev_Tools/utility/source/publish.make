
PLATFORM=
BIN_PATH = ..\\..\\..\\bin\\$(PLATFORM)

hoops_utils_static_md: Dev_Tools_release_static_md
hoops_utilsd_static_md: Dev_Tools_debug_static_md
hoops_utils_static_mt: Dev_Tools_release_static_mt
hoops_utilsd_static_mt: Dev_Tools_debug_static_mt

hoops_utils_static_mt_build:
	cd ..\\..\\..\\Dev_Tools\\utility\\source
	MSDEV hoops_utils_static.dsp /MAKE "hoops_utils_static - Win32 Release_mt"

hoops_utilsd_static_mt_build:
	cd ..\\..\\..\\Dev_Tools\\utility\\source
	MSDEV hoops_utils_static.dsp /MAKE "hoops_utils_static - Win32 Debug_mt"


hoops_utils_static_arm:
	$(MAKE) -f publish.make Dev_Tools_release_static_multi \
	"CONFIG = arm" \
	"PLATFORM = $(PLATFORM)"
hoops_utilsd_static_arm:
	$(MAKE) -f publish.make Dev_Tools_debug_static_multi \
	"CONFIG = arm" \
	"PLATFORM = $(PLATFORM)"
hoops_utils_static_arm_mt:
	$(MAKE) -f publish.make Dev_Tools_release_static_mt_multi \
	"CONFIG = arm" \
	"PLATFORM = $(PLATFORM)"
hoops_utilsd_static_arm_mt:
	$(MAKE) -f publish.make Dev_Tools_debug_static_mt_multi \
	"CONFIG = arm" \
	"PLATFORM = $(PLATFORM)"

hoops_utils_static_x86em:
	$(MAKE) -f publish.make Dev_Tools_release_static_multi \
	"CONFIG = x86em" \
	"PLATFORM = $(PLATFORM)"
hoops_utilsd_static_x86em:
	$(MAKE) -f publish.make Dev_Tools_debug_static_multi \
	"CONFIG = x86em" \
	"PLATFORM = $(PLATFORM)"
hoops_utils_static_x86em_mt:
	$(MAKE) -f publish.make Dev_Tools_release_static_mt_multi \
	"CONFIG = x86em" \
	"PLATFORM = $(PLATFORM)"
hoops_utilsd_static_x86em_mt:
	$(MAKE) -f publish.make Dev_Tools_debug_static_mt_multi \
	"CONFIG = x86em" \
	"PLATFORM = $(PLATFORM)"

#
# Dev_Tools general publishing
#
Dev_Tools_release_static_md:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Release_$(PLATFORM)_md\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include

Dev_Tools_debug_static_md:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Debug_$(PLATFORM)_md\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include

Dev_Tools_release_static_mt:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Release_$(PLATFORM)_mt\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include

Dev_Tools_debug_static_mt:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Debug_$(PLATFORM)_mt\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include


# This is currently for WinCE stuff
Dev_Tools_release_multi:
	-mkdir $(BIN_PATH)
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\bin\\$(PLATFORM)
	-mkdir ..\\include
	copy .\\Release_$(CONFIG)\\*.lib ..\\lib\\$(PLATFORM)
	copy .\\Release_$(CONFIG)\\*.dll ..\\bin\\$(PLATFORM)
	copy *.h ..\\include
	copy .\\Release_$(CONFIG)\\*.dll $(BIN_PATH)

Dev_Tools_debug_multi:
	-mkdir $(BIN_PATH)
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\bin\\$(PLATFORM)
	-mkdir ..\\include
	copy .\\Debug_$(CONFIG)\\*.lib ..\\lib\\$(PLATFORM)
	copy .\\Debug_$(CONFIG)\\*.dll ..\\bin\\$(PLATFORM)
	copy *.h ..\\include
	copy .\\Debug_$(CONFIG)\\*.dll $(BIN_PATH)
	copy .\\Debug_$(CONFIG)\\*.pdb $(BIN_PATH)

Dev_Tools_release_static_multi:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Release_md_$(CONFIG)\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include

Dev_Tools_debug_static_multi:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Debug_md_$(CONFIG)\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include
	
Dev_Tools_release_static_mt_multi:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Release_mt_$(CONFIG)\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include

Dev_Tools_debug_static_mt_multi:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Debug_mt_$(CONFIG)\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include







