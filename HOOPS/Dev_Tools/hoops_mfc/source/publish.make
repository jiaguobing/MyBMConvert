
PLATFORM=
BIN_PATH = ..\\..\\..\\bin\\$(PLATFORM)


#
# hoops mfc
#
hoops_mfc: Dev_Tools_release
hoops_mfcd: Dev_Tools_debug
hoops_mfcu: Dev_Tools_release_unicode
hoops_mfcud: Dev_Tools_debug_unicode

hoops_mfc_sdi: Dev_Tools_sdi_release
hoops_mfc_sdid: Dev_Tools_sdi_debug
hoops_mfcu_sdi: Dev_Tools_sdi_release_unicode
hoops_mfcu_sdid: Dev_Tools_sdi_debug_unicode

hoops_mfc_static: Dev_Tools_release_static
hoops_mfcd_static: Dev_Tools_debug_static

hoops_mfc_static_mt: Dev_Tools_release_static_mt
hoops_mfcd_static_mt: Dev_Tools_debug_static_mt
hoops_mfcu_static_mt: Dev_Tools_release_static_mt_unicode
hoops_mfcud_static_mt: Dev_Tools_debug_static_mt_unicode

hoops_mfc_static_md: Dev_Tools_release_static_md
hoops_mfcd_static_md: Dev_Tools_debug_static_md
hoops_mfcu_static_md: Dev_Tools_release_static_md_unicode
hoops_mfcud_static_md: Dev_Tools_debug_static_md_unicode

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

Dev_Tools_release_unicode:
	-mkdir $(BIN_PATH)
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\bin\\$(PLATFORM)
	-mkdir ..\\include
	copy .\\Release_$(PLATFORM)_unicode\\*.lib ..\\lib\\$(PLATFORM)
	copy .\\Release_$(PLATFORM)_unicode\\*.dll ..\\bin\\$(PLATFORM)
	copy *.h ..\\include
	copy .\\Release_$(PLATFORM)_unicode\\*.dll $(BIN_PATH)

Dev_Tools_debug_unicode:
	-mkdir $(BIN_PATH)
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\bin\\$(PLATFORM)
	-mkdir ..\\include
	copy .\\Debug_$(PLATFORM)_unicode\\*.lib ..\\lib\\$(PLATFORM)
	copy .\\Debug_$(PLATFORM)_unicode\\*.dll ..\\bin\\$(PLATFORM)
	copy *.h ..\\include
	copy .\\Debug_$(PLATFORM)_unicode\\*.dll $(BIN_PATH)
	copy .\\Debug_$(PLATFORM)_unicode\\*.pdb $(BIN_PATH)

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
	
Dev_Tools_sdi_release_unicode:
	-mkdir $(BIN_PATH)
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\bin\\$(PLATFORM)
	-mkdir ..\\include
	copy .\\Release_$(PLATFORM)_unicode_SDI\\*.lib ..\\lib\\$(PLATFORM)
	copy .\\Release_$(PLATFORM)_unicode_SDI\\*.dll ..\\bin\\$(PLATFORM)
	copy *.h ..\\include
	copy .\\Release_$(PLATFORM)_unicode_SDI\\*.dll $(BIN_PATH)

Dev_Tools_sdi_debug_unicode:
	-mkdir $(BIN_PATH)
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\bin\\$(PLATFORM)
	-mkdir ..\\include
	copy .\\Debug_$(PLATFORM)_unicode_SDI\\*.lib ..\\lib\\$(PLATFORM)
	copy .\\Debug_$(PLATFORM)_unicode_SDI\\*.dll ..\\bin\\$(PLATFORM)
	copy *.h ..\\include
	copy .\\Debug_$(PLATFORM)_unicode_SDI\\*.dll $(BIN_PATH)
	
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

Dev_Tools_release_static_mt_unicode:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Release_$(PLATFORM)_mt_static_unicode\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include

Dev_Tools_debug_static_mt_unicode:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Debug_$(PLATFORM)_mt_static_unicode\\*.lib ..\\lib\\$(PLATFORM)
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

Dev_Tools_release_static_md_unicode:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Release_$(PLATFORM)_md_static_unicode\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include

Dev_Tools_debug_static_md_unicode:
	-mkdir ..\\lib
	-mkdir ..\\lib\\$(PLATFORM)
	-mkdir ..\\include
	copy Debug_$(PLATFORM)_md_static_unicode\\*.lib ..\\lib\\$(PLATFORM)
	copy *.h ..\\include







