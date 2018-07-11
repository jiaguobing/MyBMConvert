
PLATFORM=
BIN_PATH = ..\\..\\..\\bin\\$(PLATFORM)

hp_bridge: Dev_Tools_release
hp_bridged: Dev_Tools_debug

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







