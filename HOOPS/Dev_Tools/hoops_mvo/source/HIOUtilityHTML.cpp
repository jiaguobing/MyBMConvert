// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <stdio.h>

#include "HIOUtilityHTML.h"
#include <wchar.h>


HFileOutputResult HIOUtilityHTML::FileOutputByKey(const wchar_t * FileName, HC_KEY key, HOutputHandlerOptions * options) 
{
	UNREFERENCED(key);

	if(!FileName) return OutputBadFileName;

	SetStartingOutput();

	FILE *page = wfopen(FileName, L"w");

	if(!page)
		return OutputFail;

	if(page == NULL)
		return OutputBadFileName;

	int width=0;
	int height=0;

	if(options) {
		width = (int)options->m_Window_Width;
		height = (int)options->m_Window_Height;
	}

	//set defaults
	if(width == 0) 
		width=50;
	if(height == 0) 
		height=50;

	//limits
	if(width>100) width=100;
	if(width<1) width=1;
	if(height>100) height=100;
	if(height<1) height=1;


	wchar_t bash_filename[4000] = {L""};
	wcscpy(bash_filename, FileName);
	wchar_t * base_name = wcsrchr(bash_filename, L'/');
	if(!base_name)
		base_name = wcsrchr(bash_filename, L'\\');
	if(!base_name)
		base_name = bash_filename;

	if(*base_name == L'/' || *base_name == L'\\')
		++base_name;


	wchar_t * dot = wcsrchr(base_name, L'.');
	if(dot)
		*dot = L'\0';
	H_UTF8 utf8_base_name(base_name);

	fputs(H_FORMAT_TEXT(
		"<html xmlns=\"http://www.w3.org/1999/xhtml\">\n"
		"<head>\n"
		"<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\n"
		"</head>\n\n"
		"<script type=\"text/javascript\">\n"
			"var ua = navigator.userAgent.toLowerCase();\n"
			"var platform = navigator.platform.toLowerCase();\n"
			"function browserCheck() {\n"
			"	if (ua.indexOf(\"msie\") != -1 || ua.indexOf(\"trident\") != -1)  { //browser is IE\n"
			"		if (platform == 'win32') { //browser is IE 32-bit\n"
			"			document.write('<center><font face=\"Arial, Helvetica, sans-serif\" color=\"#336699\"><h1>""%s""</h1><p><br><p>');\n"
			"			document.write('<object classid =\"clsid:AD5F3C4B-BD73-11D5-838B-0050042DF1E4\"""codebase=\"http://downloads.techsoft3d.com/HOOPSATL/hoopsatlcontrol.cab#version=-1,-1,-1,-1\" ""width=\"""%d""%%\" height=\"""%d""%%\">');\n"
			"			document.write('<param name=\"Filename\" value=\"""%s"".hsf\">');\n"
			"			document.write('<param name=\"AllowMenu\" value=\"1\">');\n"
			"			document.write('<param name=\"AxisMode\" value=\"0\">');\n"
			"			document.write('<param name=\"HardwareAntialiasing\" value=\"1\">');\n"
			"			document.write('<param name=\"AntialiasingLevel\" value=\"4\">');\n"
			"			document.write('<param name=\"DriverType\" value=\"opengl\">');\n"
			"			document.write('<param name=\"FramerateType\" value=\"FramerateTarget\">');\n"
			"			document.write('<param name=\"FrameRate\" value=\"0.05\">');\n"
			"			document.write('<param name=\"CullingThreshold\" value=\"150\">');\n"
			"			document.write('</object><br/>');\n"
			"			document.write('Download <a href=\"""%s"".hsf\">""%s"".hsf</a></font></center>');\n"
			"		} else if (platform == 'win64') { //browser is IE 64-bit\n"
			"			document.write('<h3>HOOPS ActiveX Control requires 32-bit Internet Explorer.</h3>');\n"
			"		} else { //IE undetermined\n"
			"			document.write('<h3>HOOPS ActiveX Control requires 32-bit Internet Explorer.</h3>');\n"
			"		}\n"
			"	} else { //browser is NOT IE\n"
			"		document.write('<h3>HOOPS ActiveX Control requires 32-bit Internet Explorer.</h3>');\n"
			"	}\n"
			"}\n"
		"</script>\n"
		"<body bgcolor=\"#ffffff\" onload=\"browserCheck()\"/>\n"
		"</html>\n"	,
		utf8_base_name.encodedText(),
		width, height,
		utf8_base_name.encodedText(),
		utf8_base_name.encodedText(),
		utf8_base_name.encodedText()
		), page);

	fclose(page);

	SetFinishedOutput();

	return OutputOK;
}
