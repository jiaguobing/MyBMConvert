
PLATFORM=
BIN_PATH = ..\\..\\..\\bin\\$(PLATFORM)


#
# hoops mvo
#
hoops_mvo: Dev_Tools_release
hoops_mvod: Dev_Tools_debug
hoops_mvo_static: Dev_Tools_release_static
hoops_mvod_static: Dev_Tools_debug_static
hoops_mvo_static_mt: Dev_Tools_release_static_mt
hoops_mvod_static_mt: Dev_Tools_debug_static_mt
hoops_mvo_static_md: Dev_Tools_release_static_md
hoops_mvod_static_md: Dev_Tools_debug_static_md

hoops_mvo_arm:
	$(MAKE) -f publish.make Dev_Tools_release_multi \
	"CONFIG = arm" \
	"PLATFORM = $(PLATFORM)"
hoops_mvod_arm:
	$(MAKE) -f publish.make Dev_Tools_debug_multi \
	"CONFIG = arm" \
	"PLATFORM = $(PLATFORM)"
hoops_mvo_x86em:
	$(MAKE) -f publish.make Dev_Tools_release_multi \
	"CONFIG = x86em" \
	"PLATFORM = $(PLATFORM)"
hoops_mvod_x86em:
	$(MAKE) -f publish.make Dev_Tools_debug_multi \
	"CONFIG = x86em" \
	"PLATFORM = $(PLATFORM)"

#
# Dev_Tools general publishing
#

Dev_Tools_release:
	-mkdir $(BIN_PATH)
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\bin\\$(PLATFORM)
	-mkdir ..\\include
	copy .\\Release_$(PLATFORM)\\*.lib ..\\lib\\$(PLATFORM)
	copy .\\Release_$(PLATFORM)\\*.dll ..\\bin\\$(PLATFORM)
	copy *.h ..\\include
	copy .\\Release_$(PLATFORM)\\*.dll $(BIN_PATH)

Dev_Tools_debug:
	-mkdir $(BIN_PATH)
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\bin\\$(PLATFORM)
	-mkdir ..\\include
	copy .\\Debug_$(PLATFORM)\\*.lib ..\\lib\\$(PLATFORM)
	copy .\\Debug_$(PLATFORM)\\*.dll ..\\bin\\$(PLATFORM)
	copy *.h ..\\include
	copy .\\Debug_$(PLATFORM)\\*.dll $(BIN_PATH)
	copy .\\Debug_$(PLATFORM)\\*.pdb $(BIN_PATH)

Dev_Tools_sdi_release:
	-mkdir $(BIN_PATH)
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\bin\\$(PLATFORM)
	-mkdir ..\\include
	copy .\\Release_$(PLATFORM)_SDI\\*.lib ..\\lib\\$(PLATFORM)
	copy .\\Release_$(PLATFORM)_SDI\\*.dll ..\\bin\\$(PLATFORM)
	copy *.h ..\\include
	copy .\\Release_$(PLATFORM)_SDI\\*.dll $(BIN_PATH)

Dev_Tools_sdi_debug:
	-mkdir $(BIN_PATH)
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\bin\\$(PLATFORM)
	-mkdir ..\\include
	copy .\\Debug_$(PLATFORM)_SDI\\*.lib ..\\lib\\$(PLATFORM)
	copy .\\Debug_$(PLATFORM)_SDI\\*.dll ..\\bin\\$(PLATFORM)
	copy *.h ..\\include
	copy .\\Debug_$(PLATFORM)_SDI\\*.dll $(BIN_PATH)
	
Dev_Tools_release_static:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Release_$(PLATFORM)_static\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include

Dev_Tools_debug_static:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Debug_$(PLATFORM)_static\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include

Dev_Tools_release_static_mt:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Release_$(PLATFORM)_mt_static\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include

Dev_Tools_debug_static_mt:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Debug_$(PLATFORM)_mt_static\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include

Dev_Tools_release_static_md:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Release_$(PLATFORM)_md_static\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include

Dev_Tools_debug_static_md:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Debug_$(PLATFORM)_md_static\\*.lib ..\\lib\\$(PLATFORM)
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
	copy Release_static_$(CONFIG)\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include

Dev_Tools_debug_static_multi:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Debug_static_$(CONFIG)\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include
	
Dev_Tools_release_static_mt_multi:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Release_static_$(CONFIG)_mt\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include

Dev_Tools_debug_static_mt_multi:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Debug_static_$(CONFIG)_mt\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include








