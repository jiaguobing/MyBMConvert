
PLATFORM=
BIN_PATH = ..\\..\\..\\bin\\$(PLATFORM)

#
# base stream
#
base_stream: Dev_Tools_release
base_streamd: Dev_Tools_debug
base_stream_static: Dev_Tools_release_static
base_streamd_static: Dev_Tools_debug_static
base_stream_static_mt: Dev_Tools_release_static_mt
base_streamd_static_mt: Dev_Tools_debug_static_mt
base_stream_static_md: Dev_Tools_release_static_md
base_streamd_static_md: Dev_Tools_debug_static_md


#
# Dev_Tools general publishing
#

Dev_Tools_release:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\bin\\$(PLATFORM)
	-mkdir ..\\include
	copy .\\Release_$(PLATFORM)\\*.lib ..\\lib\\$(PLATFORM)
	copy .\\Release_$(PLATFORM)\\*.dll ..\\bin\\$(PLATFORM)
	copy *.h ..\\include
	copy stream_common\\*.h ..\\include
	-copy .\\Release_$(PLATFORM)\\*.dll $(BIN_PATH)

Dev_Tools_debug:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\bin\\$(PLATFORM)
	-mkdir ..\\include
	copy .\\Debug_$(PLATFORM)\\*.lib ..\\lib\\$(PLATFORM)
	copy .\\Debug_$(PLATFORM)\\*.dll ..\\bin\\$(PLATFORM)
	copy *.h ..\\include
	copy stream_common\\*.h ..\\include
	-copy .\\Debug_$(PLATFORM)\\*.dll $(BIN_PATH)
	-copy .\\Debug_$(PLATFORM)\\*.pdb $(BIN_PATH)

Dev_Tools_release_static:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Release_$(PLATFORM)_static\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include
	copy stream_common\\*.h ..\\include

Dev_Tools_debug_static:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Debug_$(PLATFORM)_static\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include
	copy stream_common\\*.h ..\\include

Dev_Tools_release_static_mt:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Release_$(PLATFORM)_mt_static\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include
	copy stream_common\\*.h ..\\include

Dev_Tools_debug_static_mt:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Debug_$(PLATFORM)_mt_static\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include
	copy stream_common\\*.h ..\\include

Dev_Tools_release_static_md:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Release_$(PLATFORM)_md_static\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include
	copy stream_common\\*.h ..\\include

Dev_Tools_debug_static_md:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Debug_$(PLATFORM)_md_static\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include
	copy stream_common\\*.h ..\\include

# This is currently for WinCE stuff
Dev_Tools_release_multi:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\bin\\$(PLATFORM)
	-mkdir ..\\include
	copy .\\Release_$(CONFIG)\\*.lib ..\\lib\\$(PLATFORM)
	copy .\\Release_$(CONFIG)\\*.dll ..\\bin\\$(PLATFORM)
	copy *.h ..\\include
	copy stream_common\\*.h ..\\include
	-copy .\\Release_$(CONFIG)\\*.dll $(BIN_PATH)

Dev_Tools_debug_multi:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\bin\\$(PLATFORM)
	-mkdir ..\\include
	copy .\\Debug_$(CONFIG)\\*.lib ..\\lib\\$(PLATFORM)
	copy .\\Debug_$(CONFIG)\\*.dll ..\\bin\\$(PLATFORM)
	copy *.h ..\\include
	copy stream_common\\*.h ..\\include
	-copy .\\Debug_$(CONFIG)\\*.dll $(BIN_PATH)
	-copy .\\Debug_$(CONFIG)\\*.pdb $(BIN_PATH)

Dev_Tools_release_static_multi:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Release_static_$(CONFIG)\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include
	copy stream_common\\*.h ..\\include

Dev_Tools_debug_static_multi:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Debug_static_$(CONFIG)\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include
	copy stream_common\\*.h ..\\include
	
Dev_Tools_release_static_mt_multi:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Release_static_$(CONFIG)_mt\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include
	copy stream_common\\*.h ..\\include

Dev_Tools_debug_static_mt_multi:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Debug_static_$(CONFIG)_mt\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include
	copy stream_common\\*.h ..\\include








