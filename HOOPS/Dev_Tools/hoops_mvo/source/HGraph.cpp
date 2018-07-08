// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "HGraph.h"
#include "HDB.h"
#include "math.h"
#include <stdio.h>


//////////////////////    HBaseGraph    //////////////////////

HBaseGraph::HBaseGraph(HC_KEY plot_seg)
{
	m_plot_segment=plot_seg;

	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Set_Visibility("faces=on, markers=on, lines=on, edges=off");
		HC_Set_Marker_Symbol("@");
		HC_Set_Marker_Size(0.25);
		HC_Set_Text_Font("size=0.1 oru");
		HC_Set_Text_Alignment("**");
		HC_Open_Segment("Labels");
			HC_Open_Segment("Title");
				HC_Set_Text_Font("size=0.25 oru");
			HC_Close_Segment();
		HC_Close_Segment();
	HC_Close_Segment();

	m_show_frame=false;
	m_origin=HPoint(0,0,0);
	m_preserve_data=false;
	m_automatic_updates=true;
	m_force_update=false;

	m_labels.ReplaceAt(0, 0);
}

//destructor
HBaseGraph::~HBaseGraph()
{
	if(m_preserve_data)
	{
		size_t max=m_labels.Count();
		for(size_t i=0; i<max; ++i)
		{
			if(m_labels[i]!=0)
			{
				delete m_labels[i];
				m_labels[i]=0;
			}
		}

		if(m_legend.segment!=INVALID_KEY)
		{
			max=m_legend.entries.Count();
			for(size_t i=0; i<max; ++i)
			{
				if(m_legend.entries[i]!=0)
				{
					delete m_legend.entries[i];
					m_legend.entries[i]=0;
				}
			}
		}

	}
	else
	{
		size_t max=m_labels.Count();
		for (size_t i=0; i<max; ++i)
			RemoveLabel((int)i);

		RemoveLegend();

		HC_Delete_By_Key(m_plot_segment);
	}
}

void HBaseGraph::GetPlotTitle(char * title)
{
	HC_KEY text_key;
	char type[MVO_BUFFER_SIZE];
	HC_BOOLEAN found;
	float x,y,z;

	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Labels");
			HC_Open_Segment("Title");
				HC_Begin_Contents_Search(".", "text");
				found=HC_Find_Contents(type, &text_key);
				HC_End_Contents_Search();
				if(found) HC_Show_Text(text_key, &x, &y, &z, title);
				else title[0]=0;
			HC_Close_Segment();
		HC_Close_Segment();
	HC_Close_Segment();
}

//set the plot title color
void HBaseGraph::SetPlotTitleColor(const char * color)
{
	char str[MVO_BUFFER_SIZE];
	sprintf(str, "text=%s", color);

	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Labels");
			HC_Open_Segment("Title");
				HC_Set_Color(str);
			HC_Close_Segment();
		HC_Close_Segment();
	HC_Close_Segment();
}

//set the plot title color
void HBaseGraph::GetPlotTitleColor(char * color)
{
	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Labels");
			HC_Open_Segment("Title");
				HC_Show_One_Net_Color("text", color);
			HC_Close_Segment();
		HC_Close_Segment();
	HC_Close_Segment();
}

void HBaseGraph::GetPlotUnicodeTitle(unsigned short * title)
{
	HC_KEY text_key;
	char type[MVO_BUFFER_SIZE];
	HC_BOOLEAN found;
	float x,y,z;

	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Labels");
			HC_Open_Segment("Title");
				HC_Begin_Contents_Search(".", "text");
				found=HC_Find_Contents(type, &text_key);
				HC_End_Contents_Search();
				if(found) HC_Show_Unicode_Text(text_key, &x, &y, &z, title);
				else title[0]=0;
			HC_Close_Segment();
		HC_Close_Segment();
	HC_Close_Segment();

}

//get and set plot title font
void HBaseGraph::GetPlotTitleTextFont(char * font)
{
	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Labels");
			HC_Open_Segment("Title");
				HC_Show_Text_Font(font);
			HC_Close_Segment();
		HC_Close_Segment();
	HC_Close_Segment();
}
void HBaseGraph::SetPlotTitleTextFont(const char * font)
{
	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Labels");
			HC_Open_Segment("Title");
				HC_Set_Text_Font(font);
			HC_Close_Segment();
		HC_Close_Segment();
	HC_Close_Segment();
}

//get and set the plot title location
HPoint HBaseGraph::GetPlotTitleLocation()
{
	HC_KEY text_key;
	char str[MVO_BUFFER_SIZE];
	HC_BOOLEAN found;
	float x=0.0f,y=0.0f,z=0.0f;
	HPoint loc;

	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Labels");
			HC_Open_Segment("Title");
				HC_Begin_Contents_Search(".", "text");
				found=HC_Find_Contents(str, &text_key);
				HC_End_Contents_Search();
				if(found) HC_Show_Text(text_key, &x, &y, &z, str);
			HC_Close_Segment();
		HC_Close_Segment();
	HC_Close_Segment();

	loc.Set(x, y, z);
	return loc;
}
void HBaseGraph::SetPlotTitleLocation(HPoint loc)
{
	HC_KEY text_key;
	char str[MVO_BUFFER_SIZE];
	HC_BOOLEAN found;
	float x,y,z;

	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Labels");
			HC_Open_Segment("Title");
				HC_Begin_Contents_Search(".", "text");
				found=HC_Find_Contents(str, &text_key);
				HC_End_Contents_Search();
				if(found)
				{
					HC_Show_Text(text_key, &x, &y, &z, str);
					HC_Delete_By_Key(text_key);
					HC_Insert_Text(loc.x, loc.y, loc.z, str);
				}
			HC_Close_Segment();
		HC_Close_Segment();
	HC_Close_Segment();
}
//get and set the axis origin
HPoint HBaseGraph::GetPlotOrigin()
{
	return m_origin;
}

void HBaseGraph::SetPlotOrigin(HPoint origin)
{
	m_origin=origin;
}

//get and set plot frame visibility
bool HBaseGraph::GetFrameVisibility()
{
	return m_show_frame;
}

void HBaseGraph::SetFrameVisibility(bool visible)
{
	m_show_frame=visible;

	DrawFrame();
}

//get and set the frame pattern if it is visible
void HBaseGraph::GetFramePattern(char * pattern)
{
	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Frame");
			HC_Show_Net_Line_Pattern(pattern);
		HC_Close_Segment();
	HC_Close_Segment();			
}

void HBaseGraph::SetFramePattern(const char * pattern)
{
	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Frame");
			HC_Set_Line_Pattern(pattern);
			HC_Set_Edge_Pattern(pattern);
		HC_Close_Segment();
	HC_Close_Segment();		

	DrawFrame();
}

//get and set the frame color
void HBaseGraph::GetFrameColor(char * color)
{
	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Frame");
			HC_Show_One_Net_Color("lines", color);
		HC_Close_Segment();
	HC_Close_Segment();			
}

void HBaseGraph::SetFrameColor(const char * color)
{
	char str[MVO_BUFFER_SIZE];
	sprintf(str, "lines=%s, edges=%s", color, color);

	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Frame");
			HC_Set_Color(str);
		HC_Close_Segment();
	HC_Close_Segment();		

	DrawFrame();
}

//get and set the frame weight
float HBaseGraph::GetFrameWeight()
{
	float wt;

	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Frame");
			HC_Show_Net_Line_Weight(&wt);
		HC_Close_Segment();
	HC_Close_Segment();			

	return wt;
}

void HBaseGraph::SetFrameWeight(float weight)
{
	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Frame");
			HC_Set_Line_Weight(weight);
			HC_Set_Edge_Weight(weight);
		HC_Close_Segment();
	HC_Close_Segment();	

	DrawFrame();
}

//add a label at loc on the graph containing text str, returns the id
int HBaseGraph::AddLabel(const char * str, HPoint loc, HGraphPointFormat format, 
						 float xvector, float yvector, float zvector)
{
	char opt_label[MVO_BUFFER_SIZE];
	int n = (int)m_labels.Count();
	sprintf(opt_label, "H_label_id=%d", n);
	//add a new node
	m_labels.Append(new HGraphLabelNode());
	//get the Hoops segment where other data will be stored
	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Labels");
			m_labels[n]->segment=HC_KOpen_Segment("");
				if(format!=PointFormatCartesian) ConvertPoints(1, &loc, format, &loc, PointFormatCartesian);
				m_labels[n]->text_key=HC_KInsert_Text(loc.x, loc.y, loc.z, str);
				HC_Set_Text_Path(xvector, yvector, zvector);
				HC_Set_User_Options(opt_label);
			HC_Close_Segment();
		HC_Close_Segment();
	HC_Close_Segment();
	
	return n;
}

int HBaseGraph::AddUnicodeLabel(const unsigned short * str, HPoint loc, HGraphPointFormat format, 
					float xvector, float yvector, float zvector)
{
	char opt_label[MVO_BUFFER_SIZE];
	int n = (int)m_labels.Count();
	sprintf(opt_label, "H_label_id=%d", n);
	//add a new node
	m_labels.Append(new HGraphLabelNode());
	//get the Hoops segment where other data will be stored
	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Labels");
			m_labels[n]->segment=HC_KOpen_Segment("");
				if(format!=PointFormatCartesian) ConvertPoints(1, &loc, format, &loc, PointFormatCartesian);
				m_labels[n]->text_key=HC_KInsert_Unicode_Text(loc.x, loc.y, loc.z, str);
				HC_Set_Text_Path(xvector, yvector, zvector);
				HC_Set_User_Options(opt_label);
			HC_Close_Segment();
		HC_Close_Segment();
	HC_Close_Segment();

	return n;
}
//removes a previously added label
void HBaseGraph::RemoveLabel(int label_id)
{
	if(label_id >= (int)m_labels.Count() || m_labels[label_id]==0)
		return;

	HC_Delete_By_Key(m_labels[label_id]->segment);
	delete m_labels[label_id];
	m_labels[label_id]=0;
}

//show the contents of a label
void HBaseGraph::GetLabelContents(int label_id, char * str)
{
	float x,y,z;

	if(label_id >= (int)m_labels.Count() || m_labels[label_id]==0)
	{
		HDB::ReportError("Warning: Specified label does not exist!");
		return;
	}

	HC_Show_Text(m_labels[label_id]->text_key, &x, &y, &z, str);
}

void HBaseGraph::GetLabelUnicodeContents(int label_id, unsigned short * str)
{
	float x,y,z;

	if(label_id >= (int)m_labels.Count() || m_labels[label_id]==0)
	{
		HDB::ReportError("Warning: Specified label does not exist!");
		return;
	}

	HC_Show_Unicode_Text(m_labels[label_id]->text_key, &x, &y, &z, str);
}


void HBaseGraph::SetLabelContents(int label_id, const char * str)
{
	float x,y,z;
	HC_KEY label_key;
	char tmp[MVO_BUFFER_SIZE];

	if(label_id >= (int)m_labels.Count() || m_labels[label_id]==0)
	{
		HDB::ReportError("Warning: Specified label does not exist!");
		return;
	}

	//get the old label location and flush its contents
	label_key=m_labels[label_id]->text_key;
	HC_Show_Text(label_key, &x, &y, &z, tmp);
	HC_Delete_By_Key(label_key);

	//insert a label with the new string contents
	HC_Open_Segment_By_Key(m_labels[label_id]->segment);
		label_key=HC_KInsert_Text(x, y, z, str);
	HC_Close_Segment();

	m_labels[label_id]->text_key=label_key;
}

	
void HBaseGraph::SetLabelUnicodeContents(int label_id, const unsigned short * str)
{
	float x,y,z;
	HC_KEY label_key;
	char tmp[MVO_BUFFER_SIZE];

	if(label_id >= (int)m_labels.Count() || m_labels[label_id]==0)
	{
		HDB::ReportError("Warning: Specified label does not exist!");
		return;
	}

	//get the old label location and flush its contents
	label_key=m_labels[label_id]->text_key;
	HC_Show_Text(label_key, &x, &y, &z, tmp);
	HC_Delete_By_Key(label_key);

	//insert a label with the new string contents
	HC_Open_Segment_By_Key(m_labels[label_id]->segment);
		label_key=HC_KInsert_Unicode_Text(x, y, z, str);
	HC_Close_Segment();

	m_labels[label_id]->text_key=label_key;
}

//get and set the label font
void HBaseGraph::SetLabelTextFont(int label_id, const char * font)
{
	if(label_id >= (int)m_labels.Count() || m_labels[label_id]==0)
	{
		HDB::ReportError("Warning: Specified label does not exist!");
		return;
	}

	HC_Open_Segment_By_Key(m_labels[label_id]->segment);
		HC_Set_Text_Font(font);
	HC_Close_Segment();
}

void HBaseGraph::GetLabelTextFont(int label_id, char * font)
{
	if(label_id >= (int)m_labels.Count() || m_labels[label_id]==0)
	{
		HDB::ReportError("Warning: Specified label does not exist!");
		return;
	}

	HC_Open_Segment_By_Key(m_labels[label_id]->segment);
		HC_Show_Text_Font(font);
	HC_Close_Segment();
}

//get and set label text color
void HBaseGraph::SetLabelTextColor(int label_id, const char * color)
{
	char str[MVO_BUFFER_SIZE];

	if(label_id >= (int)m_labels.Count() || m_labels[label_id]==0)
	{
		HDB::ReportError("Warning: Specified label does not exist!");
		return;
	}
	sprintf(str, "text=%s", color);

	HC_Open_Segment_By_Key(m_labels[label_id]->segment);
		HC_Set_Color(str);
	HC_Close_Segment();
}

void HBaseGraph::GetLabelTextColor(int label_id, char * color)
{
	if(label_id >= (int)m_labels.Count() || m_labels[label_id]==0)
	{
		HDB::ReportError("Warning: Specified label does not exist!");
		return;
	}

	HC_Open_Segment_By_Key(m_labels[label_id]->segment);
		HC_Show_One_Net_Color("text", color);
	HC_Close_Segment();
}

//get and set label text alignment	
void HBaseGraph::SetLabelTextAlignment(int label_id, const char * alignment)
{
	if(label_id >= (int)m_labels.Count() || m_labels[label_id]==0)
	{
		HDB::ReportError("Warning: Specified label does not exist!");
		return;
	}

	HC_Open_Segment_By_Key(m_labels[label_id]->segment);
		HC_Set_Text_Alignment(alignment);
	HC_Close_Segment();
}

void HBaseGraph::GetLabelTextAlignment(int label_id, char * alignment)
{
	if(label_id >= (int)m_labels.Count() || m_labels[label_id]==0)
	{
		HDB::ReportError("Warning: Specified label does not exist!");
		return;
	}

	HC_Open_Segment_By_Key(m_labels[label_id]->segment);
		HC_Show_Net_Text_Alignment(alignment);
	HC_Close_Segment();
}

//get and set the location of a label on the graph
void HBaseGraph::SetLabelLocation(int label_id, HPoint loc, HGraphPointFormat format)
{
	if(label_id >= (int)m_labels.Count() || m_labels[label_id]==0)
	{
		HDB::ReportError("Warning: Specified label does not exist!");
		return;
	}

	if(format!=PointFormatCartesian) ConvertPoints(1, &loc, format, &loc, PointFormatCartesian);
	HC_Move_Text(m_labels[label_id]->text_key, loc.x, loc.y, loc.z);	
}

HPoint HBaseGraph::GetLabelLocation(int label_id)
{
	char str[MVO_BUFFER_SIZE];
	HPoint loc;

	if(label_id >= (int)m_labels.Count() || m_labels[label_id]==0)
	{
		HDB::ReportError("Warning: Specified label does not exist!");
		return HPoint(0.,0.,0.);
	}

	HC_Show_Text(m_labels[label_id]->text_key, &loc.x, &loc.y, &loc.z, str);

	return loc;
}

//get and set a label's text path
void HBaseGraph::SetLabelTextPath(int label_id, float xvector, float yvector, float zvector)
{

	if(label_id >= (int)m_labels.Count() || m_labels[label_id]==0)
	{
		HDB::ReportError("Warning: Specified label does not exist!");
		return;
	}

	HC_Open_Segment_By_Key(m_labels[label_id]->segment);
		HC_Set_Text_Path(xvector, yvector, zvector);
	HC_Close_Segment();
}

void HBaseGraph::GetLabelTextPath(int label_id, float *xvector, float *yvector, float *zvector)
{
	
	if(label_id >= (int)m_labels.Count() || m_labels[label_id]==0)
	{
		HDB::ReportError("Warning: Specified label does not exist!");
		return;
	}

	HC_Open_Segment_By_Key(m_labels[label_id]->segment);
		HC_Show_Text_Path(xvector, yvector, zvector);
	HC_Close_Segment();
}

//add a legend at loc
void HBaseGraph::AddLegend(HPoint loc, HGraphPointFormat format)
{
	if(m_legend.segment!=INVALID_KEY)
	{
		HDB::ReportError("Warning: Legend already initialized!");
		return;
	}
	m_legend.title=INVALID_KEY;

	HC_Open_Segment_By_Key(m_plot_segment);
		m_legend.segment=HC_KOpen_Segment("Legend");
			if(format!=PointFormatCartesian) ConvertPoints(1, &loc, format, &loc, PointFormatCartesian);
			HC_Translate_Object(loc.x, loc.y, loc.z);
			HC_Set_Text_Font("size=0.125 oru");
			HC_Set_Visibility("text, lines, faces, no markers");
			HC_Open_Segment("Title");
				HC_Set_Text_Font("size=0.2 oru");
				HC_Set_Text_Alignment("<*");
			HC_Close_Segment();
		HC_Close_Segment();
	HC_Close_Segment();

	DrawLegend();
}
//removes a previously added legend
void HBaseGraph::RemoveLegend()
{
	if(m_legend.segment==INVALID_KEY)
		return;

	HC_Delete_By_Key(m_legend.segment);
	m_legend.segment=INVALID_KEY;
	m_legend.title=INVALID_KEY;

	size_t max=m_legend.entries.Count();
	for(size_t i=0; i<max; ++i)
	{
		if(m_legend.entries[i]!=0)
		{
			delete m_legend.entries[i];
			m_legend.entries[i]=0;
		}
	}
	m_legend.entries.SetCount(0);
}

//get and set the legend title
void HBaseGraph::SetLegendTitle(const char * title)
{
	if(m_legend.segment==INVALID_KEY) return;

	if(m_legend.title!=INVALID_KEY)
		HC_Delete_By_Key(m_legend.title);
	if(title[0]==0)
		m_legend.title=INVALID_KEY;
	else
	{
		HC_Open_Segment_By_Key(m_legend.segment);
			HC_Open_Segment("Title");
				m_legend.title=HC_KInsert_Text(0.,0.,0.,title);
			HC_Close_Segment();
		HC_Close_Segment();
	}

	DrawLegend();
}
void HBaseGraph::GetLegendTitle(char * title)
{
	float x,y,z;

	if(m_legend.title!=INVALID_KEY)
		HC_Show_Text(m_legend.title, &x, &y, &z, title);
}

void HBaseGraph::RemoveLegendTitle()
{
	if(m_legend.title!=INVALID_KEY)
		HC_Delete_By_Key(m_legend.title);
	
	m_legend.title=INVALID_KEY;

	DrawLegend();
}

//get and set the legend title in unicode
void HBaseGraph::SetLegendUnicodeTitle(const unsigned short * title)
{
	if(m_legend.segment==INVALID_KEY) return;

	if(m_legend.title!=INVALID_KEY)
		HC_Delete_By_Key(m_legend.title);
	
	if(title[0]==0)
		m_legend.title=INVALID_KEY;
	else
	{
		HC_Open_Segment_By_Key(m_legend.segment);
			HC_Open_Segment("Title");
				m_legend.title=HC_KInsert_Unicode_Text(0.,0.,0.,title);
			HC_Close_Segment();
		HC_Close_Segment();
	}

	DrawLegend();
}
void HBaseGraph::GetLegendUnicodeTitle(unsigned short * title)
{
	float x,y,z;

	if(m_legend.title!=INVALID_KEY)
		HC_Show_Unicode_Text(m_legend.title, &x, &y, &z, title);
}

//get and set the legend title font
void HBaseGraph::SetLegendTitleTextFont(const char * font)
{
	if(m_legend.segment==INVALID_KEY) return;

	HC_Open_Segment_By_Key(m_legend.segment);
	HC_Open_Segment("Title");
		HC_Set_Text_Font(font);
	HC_Close_Segment();
	HC_Close_Segment();

	DrawLegend();
}
void HBaseGraph::GetLegendTitleTextFont(char * font)
{
	if(m_legend.segment==INVALID_KEY) return;

	HC_Open_Segment_By_Key(m_legend.segment);
	HC_Open_Segment("Title");
		HC_Show_Text_Font(font);
	HC_Close_Segment();
	HC_Close_Segment();
}

//get and set the legend entry font
void HBaseGraph::SetLegendTextFont(const char * font)
{
	if(m_legend.segment==INVALID_KEY) return;

	HC_Open_Segment_By_Key(m_legend.segment);
		HC_Set_Text_Font(font);
	HC_Close_Segment();
	
	DrawLegend();
}
void HBaseGraph::GetLegendTextFont(char * font)
{
	if(m_legend.segment==INVALID_KEY) return;

	HC_Open_Segment_By_Key(m_legend.segment);
		HC_Show_Text_Font(font);
	HC_Close_Segment();
}
//add an entry to the legend
int HBaseGraph::AddLegendEntry(const char * str, const char * color, HGraphLegendEntryType ltype)
{
	int n = (int)m_legend.entries.Count();
	char color_def[MVO_BUFFER_SIZE], text_size[MVO_BUFFER_SIZE];
	float fsize=0.;
	char opt_label[MVO_BUFFER_SIZE];
	sprintf(opt_label, "H_legend_entry_id=%d", n);
	
	sprintf(color_def, "faces=%s, lines=%s", color, color);

	if(m_legend.segment==INVALID_KEY) return -1;
	
	//add new entry
	m_legend.entries.Append(new HGraphLabelNode());

	HC_Open_Segment_By_Key(m_legend.segment);
		m_legend.entries[n]->segment=HC_KOpen_Segment("");
			HC_Show_One_Net_Text_Font("size", text_size);
			HCLOCALE(sscanf(text_size, "%f", &fsize));
			m_legend.entries[n]->text_key=HC_KInsert_Text(2*fsize,0.0f,0.0f, str);
			HC_Set_Text_Alignment("<*");
			HC_Set_Color(color_def);
			if(ltype==LegendEntryTypeLine)
				HC_Insert_Line(0.0f, 0.0f, 0.0f, fsize, 0.0f, 0.0f);
			else
			{
				HPoint mesh_points[4];
				mesh_points[0].Set(0.0f, -fsize/2.0f, 0.0f);
				mesh_points[1].Set(fsize, -fsize/2.0f, 0.0f);
				mesh_points[2].Set(0.0f, fsize/2.0f, 0.0f);
				mesh_points[3].Set(fsize, fsize/2.0f, 0.0f);

				HC_Insert_Mesh(2, 2, mesh_points);
			}
			HC_Set_User_Options(opt_label);
		HC_Close_Segment();
	HC_Close_Segment();
	
	DrawLegend();

	return n;
}

int HBaseGraph::AddLegendUnicodeEntry(const unsigned short * str, const char * color, HGraphLegendEntryType ltype)
{
	int n= (int)m_legend.entries.Count();
	char color_def[MVO_BUFFER_SIZE], text_size[MVO_BUFFER_SIZE];
	float fsize=0.;
	char opt_label[MVO_BUFFER_SIZE];
	sprintf(opt_label, "H_legend_entry_id=%d", n);
	
	sprintf(color_def, "faces=%s, lines=%s", color, color);
	
	if(m_legend.segment==INVALID_KEY) return -1;

	//add new entry
	m_legend.entries.Append(new HGraphLabelNode());

	HC_Open_Segment_By_Key(m_legend.segment);
		m_legend.entries[n]->segment=HC_KOpen_Segment("");
			HC_Show_One_Net_Text_Font("size", text_size);
			HCLOCALE(sscanf(text_size, "%f", &fsize));
			m_legend.entries[n]->text_key=HC_KInsert_Unicode_Text(2*fsize,0.0f,0.0f, str);
			HC_Set_Text_Alignment("<*");
			HC_Set_Color(color_def);
			if(ltype==LegendEntryTypeLine)
				HC_Insert_Line(0.0f, 0.0f, 0.0f, fsize, 0.0f, 0.0f);
			else
			{
				HPoint mesh_points[4];
				mesh_points[0].Set(0.0f, -fsize/2.0f, 0.0f);
				mesh_points[1].Set(fsize, -fsize/2.0f, 0.0f);
				mesh_points[2].Set(0.0f, fsize/2.0f, 0.0f);
				mesh_points[3].Set(fsize, fsize/2.0f, 0.0f);

				HC_Insert_Mesh(2, 2, mesh_points);
			}
			HC_Set_User_Options(opt_label);
		HC_Close_Segment();
	HC_Close_Segment();
	
	DrawLegend();

	return n;
}
void HBaseGraph::RemoveLegendEntry(int entry_id)
{
	if(m_legend.segment==INVALID_KEY || entry_id >= (int)m_legend.entries.Count() || m_legend.entries[entry_id]==0)
		return;

	HC_Delete_By_Key(m_legend.entries[entry_id]->segment);
	delete m_legend.entries[entry_id];
	m_legend.entries[entry_id]=0;

	DrawLegend();
}

//show the contents of a legend entry
void HBaseGraph::GetLegendEntry(int entry_id, char * str, char * color, HGraphLegendEntryType * entry_type)
{
	float x, y, z;
	int line_count;

	if(entry_id >= (int)m_legend.entries.Count() || m_legend.entries[entry_id]==0)
	{
		HDB::ReportError("Warning: Specified legend entry does not exist!");
		return;
	}

	HC_Show_Text(m_legend.entries[entry_id]->text_key, &x, &y, &z, str);
	HC_Open_Segment_By_Key(m_legend.entries[entry_id]->segment);
		HC_Show_One_Net_Color("face", color);
		HC_Begin_Contents_Search(".", "lines");
		HC_Show_Contents_Count(&line_count);
		HC_End_Contents_Search();
	HC_Close_Segment();

	if(entry_type!=0)
	{
		if(line_count>0) *entry_type=LegendEntryTypeLine;
		else *entry_type=LegendEntryTypeBox;
	}
}
void HBaseGraph::GetLegendUnicodeEntry(int entry_id, unsigned short * str, char * color, HGraphLegendEntryType * entry_type)
{
	float x, y, z;
	int line_count;

	if(entry_id >= (int)m_legend.entries.Count() || m_legend.entries[entry_id]==0)
	{
		HDB::ReportError("Warning: Specified legend entry does not exist!");
		return;
	}

	HC_Show_Unicode_Text(m_legend.entries[entry_id]->text_key, &x, &y, &z, str);
	HC_Open_Segment_By_Key(m_legend.entries[entry_id]->segment);
		HC_Show_One_Net_Color("face", color);
		HC_Begin_Contents_Search(".", "lines");
		HC_Show_Contents_Count(&line_count);
		HC_End_Contents_Search();
	HC_Close_Segment();	

	if(entry_type!=0)
	{
		if(line_count>0) *entry_type=LegendEntryTypeLine;
		else *entry_type=LegendEntryTypeBox;
	}
}
//get and set the location of the legend on the graph
void HBaseGraph::SetLegendLocation(HPoint loc, HGraphPointFormat format)
{
	if(m_legend.segment==INVALID_KEY) return;

	HC_Open_Segment_By_Key(m_legend.segment);
		HC_Flush_Contents(".","modelling matrix");
		if(format!=PointFormatCartesian) ConvertPoints(1, &loc, format, &loc, PointFormatCartesian);
		HC_Translate_Object(loc.x, loc.y, loc.z);
	HC_Close_Segment();
}
HPoint HBaseGraph::GetLegendLocation()
{
	float mat[16]={0.};
	HPoint loc;

	if(m_legend.segment==INVALID_KEY) return HPoint(0.,0.,0.);

	HC_Open_Segment_By_Key(m_legend.segment);
		HC_Show_Modelling_Matrix(mat);
		loc.x=mat[12];
		loc.y=mat[13];
		loc.z=mat[14];		
	HC_Close_Segment();

	return loc;
}

//converts in_points from one system to another, returns out_points.
//out_points should already be allocated and the same size as in_points
void HBaseGraph::ConvertPoints(unsigned int in_count, const HPoint *in_points, HGraphPointFormat in_system, 
						   HPoint *out_points, HGraphPointFormat out_system)
{
	unsigned int i;
	HPoint tmp;

	if(in_system==PointFormatCartesian)
	{
		if(out_system==PointFormatPolarRadians)
		{
			for(i=0; i<in_count; ++i)
			{
				tmp.x=pow(in_points[i].x, 2)+pow(in_points[i].y, 2);
				tmp.y=atan2(in_points[i].y, in_points[i].x);
				tmp.z=in_points[i].z;

				out_points[i]=tmp;
			}
		}
		else if(out_system==PointFormatPolarDegrees)
		{
			for(i=0; i<in_count; ++i)
			{
				tmp.x=pow(in_points[i].x, 2)+pow(in_points[i].y, 2);
				tmp.y=static_cast<float>(180*atan2(in_points[i].y, in_points[i].x)/_PI);
				tmp.z=in_points[i].z;

				out_points[i]=tmp;
			}
		}
		else
		{
			for(i=0; i<in_count; ++i)
				out_points[i]=in_points[i];
		}
	}
	else if(in_system==PointFormatPolarRadians)
	{
		if(out_system==PointFormatCartesian)
		{
			for(i=0; i<in_count; ++i)
			{
				tmp.x=in_points[i].x*cos(in_points[i].y);
				tmp.y=in_points[i].x*sin(in_points[i].y);
				tmp.z=in_points[i].z;

				out_points[i]=tmp;
			}
		}
		else if(out_system==PointFormatPolarDegrees)
		{
			for(i=0; i<in_count; ++i)
			{
				tmp.x=in_points[i].x;
				tmp.y=static_cast<float>(180*in_points[i].y/_PI);
				tmp.z=in_points[i].z;

				out_points[i]=tmp;
			}
		}
		else
		{
			for(i=0; i<in_count; ++i)
				out_points[i]=in_points[i];
		}
	}
	else if(in_system==PointFormatPolarDegrees)
	{
		if(out_system==PointFormatCartesian)
		{
			for(i=0; i<in_count; ++i)
			{
				tmp.x=static_cast<float>(in_points[i].x*cos(180*in_points[i].y/_PI));
				tmp.y=static_cast<float>(in_points[i].x*sin(180*in_points[i].y/_PI));
				tmp.z=in_points[i].z;

				out_points[i]=tmp;
			}
		}
		else if(out_system==PointFormatPolarRadians)
		{
			for(i=0; i<in_count; ++i)
			{
				tmp.x=in_points[i].x;
				tmp.y=static_cast<float>(_PI*in_points[i].y/180);
				tmp.z=in_points[i].z;

				out_points[i]=tmp;
			}
		}
		else
		{
			for(i=0; i<in_count; ++i)
				out_points[i]=in_points[i];
		}
	}
	else
	{
		for(i=0; i<in_count; ++i)
			out_points[i]=in_points[i];
	}
}

void HBaseGraph::PreserveData(bool preserve)
{
	m_preserve_data=preserve;
}

void HBaseGraph::SetAutomaticUpdates(bool automatic)
{
	m_automatic_updates=automatic;
}

bool HBaseGraph::GetAutomaticUpdates()
{
	return m_automatic_updates;
}

void HBaseGraph::Update()
{
	m_force_update=true;
	DrawLegend();
	m_force_update=false;
}


void HBaseGraph::DrawLegend()
{
	char ctext_size[MVO_BUFFER_SIZE], ctitle_size[MVO_BUFFER_SIZE], units[MVO_BUFFER_SIZE];
	float ftitle, ftext, d;
	
	if(m_legend.segment==INVALID_KEY || 
		(!m_automatic_updates && !m_force_update)) 
		return;

	HC_Open_Segment_By_Key(m_legend.segment);
		HC_Show_One_Net_Text_Font("size", ctext_size);
		HC_Open_Segment("Title");
			HC_Show_One_Net_Text_Font("size", ctitle_size);
		HC_Close_Segment();
	HC_Close_Segment();

	HCLOCALE(sscanf(ctitle_size, "%f %s", &ftitle, units));
	if(strcmp(units, "oru")!=0)
		HDB::ReportError("Warning: Legend font should be specified in oru text size.");
	HCLOCALE(sscanf(ctext_size, "%f %s", &ftext, units));
	if(strcmp(units, "oru")!=0)
		HDB::ReportError("Warning: Legend font should be specified in oru text size.");

	d=(m_legend.title!=INVALID_KEY ? ftitle*1.5f : 0.0f);
	size_t max=m_legend.entries.Count();
	for(size_t i=0; i<max; ++i)
	{
		if(m_legend.entries[i]==0) continue;
		
		HC_Open_Segment_By_Key(m_legend.entries[i]->segment);
			HC_Flush_Contents(".", "modelling matrix");
			HC_Translate_Object(0., -d, 0.);
		HC_Close_Segment();

		d+=ftext*1.5f;
	}

}
//////////////////////////////////////////////////////////////


///////////////////////    HPlot2D    ////////////////////////
//constructor
HPlot2D::HPlot2D(HC_KEY plot_seg, HGraphPlotType plot_type) : HBaseGraph(plot_seg)
{
	m_plot_type=plot_type;
	m_aspect_ratio=0;

	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Grid");
			HC_Set_Color("lines=gray, edges=gray");
			HC_Set_Line_Pattern("...");
			HC_Set_Edge_Pattern("...");
		HC_Close_Segment();
	HC_Close_Segment();

	m_grid_type=GridTypeNone;

	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Axes");
			m_x_axis.segment=HC_KOpen_Segment("X-Axis");
			HC_Close_Segment();
		HC_Close_Segment();
	HC_Close_Segment();

	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Axes");
			m_y_axis.segment=HC_KOpen_Segment("Y-Axis");
			HC_Close_Segment();
		HC_Close_Segment();
	HC_Close_Segment();

	DrawAxes();
	
	m_points.ReplaceAt(0,0);
}

//destructor
HPlot2D::~HPlot2D()
{
	if(m_preserve_data)
	{
		size_t max=m_points.Count();
		for (size_t i=0; i<max; i++)
		{
			if(m_points[i]!=0)
			{
				delete m_points[i];
				m_points[i]=0;
			}
		}
	}
	else
	{
		size_t max=m_points.Count();
		for (size_t i=0; i<max; i++)
			RemoveDataSet((int)i);
	}
}

//get and set the plot title
void HPlot2D::SetPlotTitle(const char * title)
{
	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Labels");
			HC_Open_Segment("Title");
				HC_Set_Text_Alignment("*v");
				HC_Flush_Contents(".", "text");
				HC_Insert_Text((m_x_axis.max+m_x_axis.min)/2, m_y_axis.max+0.5, m_origin.z, title);
			HC_Close_Segment();
		HC_Close_Segment();
	HC_Close_Segment();
}

void HPlot2D::SetPlotUnicodeTitle(unsigned short * title)
{
	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Labels");
			HC_Open_Segment("Title");
				HC_Flush_Contents(".", "text");
				HC_Insert_Unicode_Text(m_origin.x, m_y_axis.max+0.5, m_origin.z, title);
			HC_Close_Segment();
		HC_Close_Segment();
	HC_Close_Segment();
}

HGraphPlotType HPlot2D::GetPlotType()
{
	return m_plot_type;
}

void HPlot2D::SetPlotOrigin(HPoint origin)
{
	HBaseGraph::SetPlotOrigin(origin);
	
	DrawAxes();
}
//get and set the grid type for this graph
HGraphGridType HPlot2D::GetGridType()
{
	return m_grid_type;
}

void HPlot2D::SetGridType(HGraphGridType gtype)
{
	//if the grid style isn't changing, then don't worry about it.
	if(m_grid_type==gtype) return;

	if(gtype==GridTypePolar)
		m_y_axis.grid_frequency=_PI/8;
	else if(gtype==GridTypeRectangular && m_grid_type==GridTypePolar)
		m_y_axis.grid_frequency=1.0;

	m_grid_type=gtype;
	
	DrawGrid();
}

//get and set the grid visibility
bool HPlot2D::GetGridVisibility()
{
	int vis=0;
	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Grid");
			vis = HC_Show_Existence("visibility");
		HC_Close_Segment();
	HC_Close_Segment();
	return vis > 0;
}

void HPlot2D::SetGridVisibility(bool value)
{
	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Grid");
			if (value)
				HC_UnSet_Visibility();
			else
				HC_Set_Visibility("lines=off");
		HC_Close_Segment();
	HC_Close_Segment();
}

//get and set the grid line pattern
void HPlot2D::GetGridPattern(char * pattern)
{
	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Grid");
			HC_Show_Net_Line_Pattern(pattern);
		HC_Close_Segment();
	HC_Close_Segment();
}

void HPlot2D::SetGridPattern(const char * pattern)
{
	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Grid");
			HC_Set_Line_Pattern(pattern);
		HC_Close_Segment();
	HC_Close_Segment();

	DrawGrid();
}

//get and set grid color
void HPlot2D::GetGridColor(char * color)
{
	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Grid");
			HC_Show_One_Net_Color("lines", color);
		HC_Close_Segment();
	HC_Close_Segment();

}
void HPlot2D::SetGridColor(const char * color)
{
	char str[MVO_BUFFER_SIZE];
	sprintf(str, "lines=%s, edges=%s", color, color);

	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Grid");
			HC_Set_Color(str);
		HC_Close_Segment();
	HC_Close_Segment();

	DrawGrid();
}

//get and set the axis scale
HGraphAxisScale HPlot2D::GetAxisScale(HGraphAxisSelection axis, double * factor)
{
	if(axis==X_Axis)
	{
		if(factor!=0) *factor=m_x_axis.axis_scale_factor;
		return m_x_axis.axis_scale;
	}
	else if(axis==Y_Axis)
	{
		if(factor!=0) *factor=m_y_axis.axis_scale_factor;
		return m_y_axis.axis_scale;
	}
	else return AxisScaleLinear;
}

void HPlot2D::SetAxisScale(HGraphAxisSelection axis, HGraphAxisScale scale, double factor)
{
	if(axis==X_Axis)
	{
		if(m_x_axis.axis_scale!=scale || m_x_axis.axis_scale_factor!=factor)
		{
			m_x_axis.axis_scale=scale;
			if(m_x_axis.axis_scale==AxisScaleLinear && factor<=0.0) m_x_axis.axis_scale_factor=1.0;
			else if (m_x_axis.axis_scale==AxisScaleLogarithmic && factor<=0.0)m_x_axis.axis_scale_factor=10.0;
			else m_x_axis.axis_scale_factor=factor;

		}
	}		
	else if(axis==Y_Axis) 
	{
		if(m_y_axis.axis_scale!=scale)
		{
			m_y_axis.axis_scale=scale;
			if(m_y_axis.axis_scale==AxisScaleLinear && factor==0.0) m_y_axis.axis_scale_factor=1.0;
			else if (m_y_axis.axis_scale==AxisScaleLogarithmic && factor==0.0)m_y_axis.axis_scale_factor=10.0;
			else m_y_axis.axis_scale_factor=factor;
		}
	}
	else; //do nothing

	DrawAxes();
}

//get and set axis scale
void HPlot2D::GetAxisRange(HGraphAxisSelection axis, double* min, double* max)
{
	if(axis==X_Axis)
	{
		*min=m_x_axis.min;
		*max=m_x_axis.max;
	}
	else if(axis==Y_Axis)
	{
		*min=m_y_axis.min;
		*max=m_y_axis.max;
	}
}

void HPlot2D::SetAxisRange(HGraphAxisSelection axis, double min, double max)
{
	if(axis==X_Axis)
	{
		m_x_axis.min=min;
		m_x_axis.max=max;
	}
	else if(axis==Y_Axis)
	{
		m_y_axis.min=min;
		m_y_axis.max=max;
	}
	else;

	DrawAxes();
	DrawGrid();
	DrawFrame();
	SetPlotTitleLocation(HPoint((float)(m_x_axis.max+m_x_axis.min)/2.0f, (float)m_y_axis.max+0.5f, (float)m_origin.z));
	size_t points_count=m_points.Count();
	for(size_t i=0; i<points_count; ++i)
		if(m_points[i]!=0)
			DrawData((int)i);
}

//get and set axis visibility
bool HPlot2D::GetAxisVisibility(HGraphAxisSelection axis)
{
	if(axis==X_Axis)
		return m_x_axis.visible;
	else if(axis==Y_Axis)
		return m_y_axis.visible;
	else return false;
}
void HPlot2D::SetAxisVisibility(HGraphAxisSelection axis, bool vis)
{
	if(axis==X_Axis)
		m_x_axis.visible=vis;
	else if(axis==Y_Axis)
		m_y_axis.visible=vis;
	else;

	DrawAxes();
}

//get and set the axis color
void HPlot2D::GetAxisColor(HGraphAxisSelection axis, char * color)
{
	if(axis==X_Axis)
	{
		HC_Open_Segment_By_Key(m_x_axis.segment);
			HC_Show_One_Net_Color("lines", color);
		HC_Close_Segment();
	}
	else if(axis==Y_Axis)
	{
		HC_Open_Segment_By_Key(m_y_axis.segment);
			HC_Show_One_Net_Color("lines", color);
		HC_Close_Segment();
	}
	else;
}

void HPlot2D::SetAxisColor(HGraphAxisSelection axis, const char * color)
{
	char str[MVO_BUFFER_SIZE];
	sprintf(str, "lines=%s", color);

	if(axis==X_Axis)
	{
		HC_Open_Segment_By_Key(m_x_axis.segment);
			HC_Set_Color(str);
		HC_Close_Segment();
	}
	else if(axis==Y_Axis)
	{
		HC_Open_Segment_By_Key(m_y_axis.segment);
			HC_Set_Color(str);
		HC_Close_Segment();
	}
	else;

	DrawAxes();
}

//get and set the axis weight
float HPlot2D::GetAxisWeight(HGraphAxisSelection axis)
{
	float wt=-1.0;

	if(axis==X_Axis)
	{
		HC_Open_Segment_By_Key(m_x_axis.segment);
			HC_Show_Net_Line_Weight(&wt);
		HC_Close_Segment();
	}
	else if(axis==Y_Axis)
	{
		HC_Open_Segment_By_Key(m_y_axis.segment);
			HC_Show_Net_Line_Weight(&wt);
		HC_Close_Segment();
	}
	else;

	return wt;
}

void HPlot2D::SetAxisWeight(HGraphAxisSelection axis, float weight)
{
	if(axis==X_Axis)
	{
		HC_Open_Segment_By_Key(m_x_axis.segment);
			HC_Set_Line_Weight(weight);
		HC_Close_Segment();
	}
	else if(axis==Y_Axis)
	{
		HC_Open_Segment_By_Key(m_y_axis.segment);
			HC_Set_Line_Weight(weight);
		HC_Close_Segment();
	}
	else;

	DrawAxes();
}

//get and set the frequency of ticks along an axis
double HPlot2D::GetAxisTickFrequency(HGraphAxisSelection axis)
{
	if(axis==X_Axis)
		return m_x_axis.tick_frequency;
	else if(axis==Y_Axis)
		return m_y_axis.tick_frequency;
	else return -1.0;
}

void HPlot2D::SetAxisTickFrequency(HGraphAxisSelection axis, double freq)
{
	if(axis==X_Axis)
		m_x_axis.tick_frequency=freq;
	else if(axis==Y_Axis)
		m_y_axis.tick_frequency=freq;
	else;

	DrawAxes();
}

//get and set axis precision
int HPlot2D::GetAxisPrecision(HGraphAxisSelection axis)
{
	if(axis==X_Axis)
		return m_x_axis.precision;
	else if(axis==Y_Axis)
		return m_y_axis.precision;
	else return -1;
}	

void HPlot2D::SetAxisPrecision(HGraphAxisSelection axis, int precision)
{
	if(axis==X_Axis)
		m_x_axis.precision=precision;
	else if(axis==Y_Axis)
		m_y_axis.precision=precision;
	else;

	DrawAxes();
}

//get and set the size of ticks along an axis
double HPlot2D::GetAxisTickSize(HGraphAxisSelection axis)
{
	if(axis==X_Axis)
		return m_x_axis.tick_size;
	else if(axis==Y_Axis)
		return m_y_axis.tick_size;
	else return -1.0;
}

void HPlot2D::SetAxisTickSize(HGraphAxisSelection axis, double size)
{
	if(axis==X_Axis)
		m_x_axis.tick_size=size;
	else if(axis==Y_Axis)
		m_y_axis.tick_size=size;
	else;

	DrawAxes();
}

//get and set the grid mesh size on an axis
double HPlot2D::GetAxisGridFrequency(HGraphAxisSelection axis)
{
	if(axis==X_Axis)
		return m_x_axis.grid_frequency;
	else if(axis==Y_Axis)
		return m_y_axis.grid_frequency;
	else return -1.0;
}
void HPlot2D::SetAxisGridFrequency(HGraphAxisSelection axis, double freq)
{
	if(axis==X_Axis)
		m_x_axis.grid_frequency=freq;
	else if(axis==Y_Axis)
		m_y_axis.grid_frequency=freq;

	DrawGrid();
}

//get and set the grid range on an axis
void HPlot2D::GetAxisGridRange(HGraphAxisSelection axis, double* min, double* max)
{
	if(axis==X_Axis)
	{
		*min=m_x_axis.grid_min;
		*max=m_x_axis.grid_max;
	}
	else if(axis==Y_Axis)
	{
		*min=m_y_axis.grid_min;
		*max=m_y_axis.grid_max;
	}
}

void HPlot2D::SetAxisGridRange(HGraphAxisSelection axis, double min, double max)
{
	if(axis==X_Axis)
	{
		m_x_axis.grid_min=min;
		m_x_axis.grid_max=max;
	}
	else if(axis==Y_Axis)
	{
		m_y_axis.grid_min=min;
		m_y_axis.grid_max=max;
	}
	else;

	DrawGrid();
	DrawFrame();
}

//get and set axis label visibility
bool HPlot2D::GetAxisLabelVisibility(HGraphAxisSelection axis)
{
	if(axis==X_Axis)
		return m_x_axis.show_labels;
	else if(axis==Y_Axis)
		return m_y_axis.show_labels;
	else return false;
}
void HPlot2D::SetAxisLabelVisibility(HGraphAxisSelection axis, bool vis)
{
	if(axis==X_Axis)
		m_x_axis.show_labels=vis;
	else if(axis==Y_Axis)
		m_y_axis.show_labels=vis;
	else return;
}
//get and set the axis label font
void HPlot2D::GetAxisLabelTextFont(HGraphAxisSelection axis, char * font)
{
	HC_KEY seg;

	if(axis==X_Axis)
	{
		seg=m_x_axis.segment;
		HC_Open_Segment_By_Key(seg);
			HC_Show_Net_Text_Font(font);
		HC_Close_Segment();	
	}
	else if(axis==Y_Axis)
	{
		seg=m_y_axis.segment;
		HC_Open_Segment_By_Key(seg);
			HC_Show_Net_Text_Font(font);
		HC_Close_Segment();	
	}	
}

void HPlot2D::SetAxisLabelTextFont(HGraphAxisSelection axis, const char * font)
{
	HC_KEY seg;

	if(axis==X_Axis)
	{
		seg=m_x_axis.segment;
		HC_Open_Segment_By_Key(seg);
			HC_Set_Text_Font(font);
		HC_Close_Segment();	
	}
	else if(axis==Y_Axis)
	{
		seg=m_y_axis.segment;
		HC_Open_Segment_By_Key(seg);
			HC_Set_Text_Font(font);
		HC_Close_Segment();	
	}
}

//get and set the axis label text color
void HPlot2D::GetAxisLabelTextColor(HGraphAxisSelection axis, char * color)
{
	HC_KEY seg;

	if(axis==X_Axis)
	{
		seg=m_x_axis.segment;
		HC_Open_Segment_By_Key(seg);
			HC_Show_One_Net_Color("text", color);
		HC_Close_Segment();	
	}
	else if(axis==Y_Axis)
	{
		seg=m_y_axis.segment;
		HC_Open_Segment_By_Key(seg);
			HC_Show_One_Net_Color("text", color);
		HC_Close_Segment();	
	}
}

void HPlot2D::SetAxisLabelTextColor(HGraphAxisSelection axis, const char * color)
{
	HC_KEY seg;
	char str[MVO_BUFFER_SIZE];
	sprintf(str, "text=%s", color);

	if(axis==X_Axis)
	{
		seg=m_x_axis.segment;
		HC_Open_Segment_By_Key(seg);
			HC_Set_Color(str);
		HC_Close_Segment();	
	}
	else if(axis==Y_Axis)
	{
		seg=m_y_axis.segment;
		HC_Open_Segment_By_Key(seg);
			HC_Set_Color(str);
		HC_Close_Segment();	
	}
}

void HPlot2D::SetAxisLabelTextPath(HGraphAxisSelection axis, float xvector,	
		float yvector, float zvector)
{
	if(axis==X_Axis)
	{
		HC_Open_Segment_By_Key(m_x_axis.segment);
			HC_Set_Text_Path(xvector, yvector, zvector);
		HC_Close_Segment();
	}
	else if(axis==Y_Axis)
	{
		HC_Open_Segment_By_Key(m_y_axis.segment);
			HC_Set_Text_Path(xvector, yvector, zvector);
		HC_Close_Segment();
	}
}

void HPlot2D::GetAxisLabelTextPath(HGraphAxisSelection axis, float *xvector,	
		float *yvector, float *zvector)
{
	if(axis==X_Axis)
	{
		HC_Open_Segment_By_Key(m_x_axis.segment);
			HC_Show_Text_Path(xvector, yvector, zvector);
		HC_Close_Segment();
	}
	else if(axis==Y_Axis)
	{
		HC_Open_Segment_By_Key(m_y_axis.segment);
			HC_Show_Text_Path(xvector, yvector, zvector);
		HC_Close_Segment();
	}
}

//adds a data set, returns the id of the data set
int HPlot2D::AddDataSet(int points_count, const HPoint *points, HGraphPointFormat format, const HPoint *colors)
{
	HC_KEY key;
	int n = (int)m_points.Count();
	char opt_label[MVO_BUFFER_SIZE];
	sprintf(opt_label, "H_data_set_id=%d", n);
	//add a new node
	m_points.Append(new HGraphDataSetNode());
	//defaults to connected only in line graphs
	m_points[n]->line_vis=(m_plot_type==PlotTypeLine); 
	//draw bars?
	m_points[n]->bar_vis=(m_plot_type==PlotTypeBar);
	//half on either side of the data point
	m_points[n]->bar_width=1.0;
	//copy the point format
	m_points[n]->format=format;
	//save the number of points for easy reference
	m_points[n]->data_size=points_count;
	//polygon drawing defaults to off
	m_points[n]->polygon_vis=false;
	//get the Hoops segment where other data will be stored
	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Points");
			m_points[n]->segment=HC_KOpen_Segment("");
			if(format!=PointFormatCartesian)
			{
				HPoint *new_points=new HPoint[points_count];
				ConvertPoints(points_count, points, format, new_points, PointFormatCartesian);
				key=HC_KInsert_Shell(points_count, new_points, 0, 0);
				delete [] new_points;
			}
			else
			{
				key=HC_KInsert_Shell(points_count, points, 0, 0);
			}

			if(colors)
			{
				int i;
				HC_Open_Geometry(key);
				for(i=0; i<points_count; ++i)
				{				
					HC_Open_Vertex(i);
					HC_Set_Color_By_Value("geometry", "RGB", colors[i].x, colors[i].y, colors[i].z);
					HC_Close_Vertex();
				}
				HC_Close_Geometry();
			}
			HC_Set_Visibility((m_plot_type==PlotTypeScatter?"markers=on":"markers=off"));			
			HC_Set_User_Options(opt_label);
			HC_Close_Segment();
		HC_Close_Segment();
	HC_Close_Segment();

	DrawData(n);

	return n;
}

//removes a previously added data set
void HPlot2D::RemoveDataSet(int data_set)
{
	if(data_set >= (int)m_points.Count() || m_points[data_set]==0) 
		return;

	HC_Delete_By_Key(m_points[data_set]->segment);

	delete m_points[data_set];
	m_points[data_set]=0;
}

//get the size of a data set
int HPlot2D::GetDataSetSize(int data_set)
{
	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return -1;
	}

	return m_points[data_set]->data_size;	
}

//get the point format of a data set
HGraphPointFormat HPlot2D::GetDataSetFormat(int data_set)
{
	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return PointFormatCartesian;
	}

	return m_points[data_set]->format;
}

//get a point list, points should already be allocated
void HPlot2D::GetDataSet(int data_set, HPoint * points)
{
	int count, i, f_len, f_list;
	HC_KEY key;
	char type[MVO_BUFFER_SIZE];
	float x, y, z;

	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return;
	}

	HC_Open_Segment_By_Key(m_points[data_set]->segment);
		HC_Begin_Contents_Search(".", "markers");
		HC_Show_Contents_Count(&count);
		if(!count)
		{
			HC_End_Contents_Search();
			HC_Begin_Contents_Search(".", "shells");
			HC_Show_Contents_Count(&count);
			if(count)
			{
				HC_Find_Contents(type, &key);
				HC_Show_Shell(key, &count, points, &f_len, &f_list);
			}
			HC_End_Contents_Search();
		}
		else
		{

			if(count!=m_points[data_set]->data_size)
			{
				HDB::ReportError("Warning: Data set size mismatch.  Data set may have improperly modified.");
			}

			for(i=count-1; i>=0; --i)
			{
				HC_Find_Contents(type, &key);
				HC_Show_Marker(key, &x, &y, &z);
				points[i].x=x;
				points[i].y=y;
				points[i].z=z;
			}

			if(m_points[data_set]->format!=PointFormatCartesian)
				ConvertPoints(count, points, PointFormatCartesian, points, m_points[data_set]->format);

			HC_End_Contents_Search();
		}
	HC_Close_Segment();
}

//replace an existing data set
void HPlot2D::ReplaceDataSet(int data_set, int points_count, const HPoint *points, 
							 HGraphPointFormat format, const HPoint *colors)
{
	HC_KEY key;

	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return;
	}

	HC_Open_Segment_By_Key(m_points[data_set]->segment);
		HC_Flush_Contents(".", "geometry");

		if(format!=PointFormatCartesian)
		{
			HPoint *new_points=new HPoint[points_count];
			ConvertPoints(points_count, points, format, new_points, PointFormatCartesian);
			key=HC_KInsert_Shell(points_count, new_points, 0, 0);
			delete [] new_points;
		}
		else
		{
			key=HC_KInsert_Shell(points_count, points, 0, 0);
		}
		
		if(colors)
		{
			int i;
			HC_Open_Geometry(key);
			for(i=0; i<points_count; ++i)
			{				
				HC_Open_Vertex(i);
				HC_Set_Color_By_Value("geometry", "RGB", colors[i].x, colors[i].y, colors[i].z);
				HC_Close_Vertex();
			}
			HC_Close_Geometry();
		}

	HC_Close_Segment();
	m_points[data_set]->data_size=points_count;

	DrawData(data_set);
}

//get and set the data color
void HPlot2D::SetPointColor(int data_set, const char * color)
{
	char str[MVO_BUFFER_SIZE];
	sprintf(str, "markers=%s", color);

	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return;
	}

	HC_Open_Segment_By_Key(m_points[data_set]->segment);
		HC_Set_Color(str);
	HC_Close_Segment();
}
void HPlot2D::GetPointColor(int data_set, char * color)
{
	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return;
	}

	HC_Open_Segment_By_Key(m_points[data_set]->segment);
		HC_Show_One_Net_Color("markers", color);
	HC_Close_Segment();
}

//get and set the marker symbol for a data set
void HPlot2D::SetPointSymbol(int data_set, const char * symbol)
{
	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return;
	}

	HC_Open_Segment_By_Key(m_points[data_set]->segment);
		HC_Set_Marker_Symbol(symbol);
	HC_Close_Segment();
}

void HPlot2D::GetPointSymbol(int data_set, char * symbol)
{
	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return;
	}

	HC_Open_Segment_By_Key(m_points[data_set]->segment);
		HC_Show_Net_Marker_Symbol(symbol);
	HC_Close_Segment();
}

//get and set the marker size for a data set
void HPlot2D::SetPointSize(int data_set, double size)
{
	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return;
	}

	HC_Open_Segment_By_Key(m_points[data_set]->segment);
		HC_Set_Marker_Size(size);
	HC_Close_Segment();
}

float HPlot2D::GetPointSize(int data_set)
{
	float size;

	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return -1;
	}

	HC_Open_Segment_By_Key(m_points[data_set]->segment);
		HC_Show_Net_Marker_Size(&size);
	HC_Close_Segment();

	return size;
}

//get and set point visibility
bool HPlot2D::GetPointVisibility(int data_set)
{
	char vis[MVO_BUFFER_SIZE];

	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return false;
	}

	HC_Open_Segment_By_Key(m_points[data_set]->segment);
		HC_Show_One_Net_Visibility("markers", vis);
	HC_Close_Segment();

	return vis[1]=='n';
}
void HPlot2D::SetPointVisibility(int data_set, bool vis)
{
	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return;
	}

	HC_Open_Segment_By_Key(m_points[data_set]->segment);
		HC_Set_Visibility((vis?"markers=on":"markers=off"));
	HC_Close_Segment();
}
//get and set whether the points are connected by a polyline
bool HPlot2D::GetLineVisibility(int data_set)
{
	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return false;
	}

	return m_points[data_set]->line_vis;
}

void HPlot2D::SetLineVisibility(int data_set, bool vis)
{
	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return;
	}

	m_points[data_set]->line_vis=vis;

	DrawData(data_set);
}
//get and set the line pattern, if the points are connected
void HPlot2D::SetLinePattern(int data_set, const char * pattern)
{
	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return;
	}

	HC_Open_Segment_By_Key(m_points[data_set]->segment);
		HC_Set_Line_Pattern(pattern);
	HC_Close_Segment();
}

void HPlot2D::GetLinePattern(int data_set, char * pattern)
{
	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return;
	}

	HC_Open_Segment_By_Key(m_points[data_set]->segment);
		HC_Show_Net_Line_Pattern(pattern);
	HC_Close_Segment();
}

//get and set the line color, if the points are connected
void HPlot2D::SetLineColor(int data_set, const char * color)
{
	char str[MVO_BUFFER_SIZE];
	sprintf(str, "lines=%s", color);

	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return;
	}

	HC_Open_Segment_By_Key(m_points[data_set]->segment);
		HC_Set_Color(str);
	HC_Close_Segment();
}

void HPlot2D::GetLineColor(int data_set, char * color)
{
	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return;
	}

	HC_Open_Segment_By_Key(m_points[data_set]->segment);
		HC_Show_One_Net_Color("lines", color);
	HC_Close_Segment();
}

//get and set data line weight
float HPlot2D::GetLineWeight(int data_set)
{
	float weight;

	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return -1.0;
	}

	HC_Open_Segment_By_Key(m_points[data_set]->segment);
		HC_Show_Net_Line_Weight(&weight);
	HC_Close_Segment();

	return weight;
}
	
void HPlot2D::SetLineWeight(int data_set, float weight)
{
	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return;
	}

	HC_Open_Segment_By_Key(m_points[data_set]->segment);
		HC_Set_Line_Weight(weight);
	HC_Close_Segment();
}

//get and set bar visibility
bool HPlot2D::GetBarVisibility(int data_set)
{
	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return false;
	}

	return m_points[data_set]->bar_vis;
}
void HPlot2D::SetBarVisibility(int data_set, bool vis)
{
	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return;
	}

	m_points[data_set]->bar_vis=vis;

	DrawData(data_set);
}
//get and set a color map for bar charts
void HPlot2D::SetBarColorMap(int data_set, const char * map)
{
	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return;
	}

	HC_Open_Segment_By_Key(m_points[data_set]->segment);
		HC_Set_Color_Map(map);
	HC_Close_Segment();

	DrawData(data_set);
}
void HPlot2D::GetBarColorMap(int data_set, char * map)
{
	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return;
	}

	HC_Open_Segment_By_Key(m_points[data_set]->segment);
		HC_Show_Net_Color_Map(map);
	HC_Close_Segment();
}
//get and set color map by value (see HC_Set_Color_Map)
void HPlot2D::SetBarColorMapByValue(int data_set, int count, const HPoint * values, const char * color_space)
{
	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return;
	}

	HC_Open_Segment_By_Key(m_points[data_set]->segment);
	HC_Set_Color_Map_By_Value((color_space==0?"RGB":color_space), count, values);
	HC_Close_Segment();

	DrawData(data_set);
}
void HPlot2D::GetBarColorMapByValue(int data_set, int * count, HPoint * values,  char * color_space)
{
	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return;
	}

	HC_Open_Segment_By_Key(m_points[data_set]->segment);
	HC_Show_Net_Color_Map_By_Value(color_space, count, values);
	HC_Close_Segment();
}
//get and set the bar width
double HPlot2D::GetBarWidth(int data_set)
{
	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return -1;
	}

	return m_points[data_set]->bar_width;
}
void HPlot2D::SetBarWidth(int data_set, double width)
{
	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return;
	}

	m_points[data_set]->bar_width=width;

	DrawData(data_set);
}

//get and set bar edge visibility
bool HPlot2D::GetBarEdgeVisibility(int data_set)
{
	char value[MVO_BUFFER_SIZE];
	
	if (data_set >= (int)m_points.Count() || m_points[data_set] == 0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return false;
	}

	HC_Open_Segment_By_Key(m_points[data_set]->segment);
		HC_Show_One_Net_Visibility("edges", value);
	HC_Close_Segment();
	
	return value[1] == 'n'; //n for on, f for off
}
void HPlot2D::SetBarEdgeVisibility(int data_set, bool vis)
{
	char str[MVO_BUFFER_SIZE];

	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return;
	}

	sprintf(str, "edges=%s", (vis?"on":"off"));

	HC_Open_Segment_By_Key(m_points[data_set]->segment);
		HC_Set_Visibility(str);
	HC_Close_Segment();
}

bool HPlot2D::GetPolygonVisibility(int data_set)
{
	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return false;
	}
	
	return m_points[data_set]->polygon_vis;
}

void HPlot2D::SetPolygonVisibility(int data_set, bool vis)
{
	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return;
	}
	
	m_points[data_set]->polygon_vis=vis;
	DrawData(data_set);
}

void HPlot2D::GetPolygonColor(int data_set, char * color)
{
	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return;
	}
	
	HC_Open_Segment_By_Key(m_points[data_set]->segment);
		HC_Show_One_Net_Color("faces", color);
	HC_Close_Segment();
}

void HPlot2D::SetPolygonColor(int data_set, const char * color)
{
	char str[MVO_BUFFER_SIZE];
	sprintf(str, "faces=%s", color);

	if(data_set >= (int)m_points.Count() || m_points[data_set]==0)
	{
		HDB::ReportError("Warning: Specified data set does not exist!");
		return;
	}
	
	HC_Open_Segment_By_Key(m_points[data_set]->segment);
		HC_Set_Color(str);
	HC_Close_Segment();
}

void HPlot2D::Update()
{
	HBaseGraph::Update();

	m_force_update=true;
	DrawAxes();
	DrawGrid();
	DrawFrame();
	m_force_update=false;

	if (GetAspectRatio() > 0)
		SetAspectRatio(m_aspect_ratio);
}

void HPlot2D::DrawAxes()
{
	double i, d_num;
	char num[MVO_BUFFER_SIZE];

	if(!m_automatic_updates && !m_force_update)
		return;

	//x_axis
	HC_Open_Segment_By_Key(m_x_axis.segment);
	HC_Flush_Contents(".", "geometry");
	if(m_x_axis.visible)
	{
		HC_Set_Text_Alignment("^");
		HC_Insert_Line(m_x_axis.min, m_origin.y, m_origin.z, m_x_axis.max, m_origin.y, m_origin.z);
		for(i=m_x_axis.tick_frequency; m_origin.x+i<=m_x_axis.max || m_origin.x-i>=m_x_axis.min; i+=m_x_axis.tick_frequency)
		{
			if(m_origin.x+i>=m_x_axis.min && m_origin.x+i<=m_x_axis.max)
			{
				HC_Insert_Line(m_origin.x+i, m_origin.y-m_x_axis.tick_size/2, m_origin.z, m_origin.x+i, m_origin.y+m_x_axis.tick_size/2, m_origin.z);
				if(m_x_axis.show_labels)
				{
					d_num=(m_x_axis.axis_scale==AxisScaleLinear ? i*m_x_axis.axis_scale_factor : 
						pow(m_x_axis.axis_scale_factor, i));
					d_num+=m_origin.x;
					if(m_x_axis.precision<0)
						sprintf(num, (fmod(d_num>0?d_num:-d_num, 1.0)>0. ? "%.3f" : "%.0f"), d_num);
					else
						sprintf(num, "%.*f", m_x_axis.precision, d_num);
					HC_Insert_Text(m_origin.x+i, m_origin.y-m_x_axis.tick_size, m_origin.z, num);
				}

			}

			if(m_origin.x-i>=m_x_axis.min && m_origin.x-i<=m_x_axis.max)
			{
				HC_Insert_Line(m_origin.x-i, m_origin.y-m_x_axis.tick_size/2, m_origin.z, m_origin.x-i, m_origin.y+m_x_axis.tick_size/2, m_origin.z);
				if(m_x_axis.show_labels)
				{
					d_num=(m_x_axis.axis_scale==AxisScaleLinear ? i*m_x_axis.axis_scale_factor : 
						pow(m_x_axis.axis_scale_factor, i));
					d_num=m_origin.x-d_num;
					if(m_x_axis.precision<0)
						sprintf(num, (fmod(d_num>0?d_num:-d_num, 1.0)>0. ? "%.3f" : "%.0f"), d_num);
					else
						sprintf(num, "%.*f", m_x_axis.precision, d_num);
					HC_Insert_Text(m_origin.x-i, m_origin.y-m_x_axis.tick_size, m_origin.z, num);
				}
			}
		}
	}
	HC_Close_Segment();

	//y_axis
	HC_Open_Segment_By_Key(m_y_axis.segment);
	HC_Flush_Contents(".", "geometry");
	if(m_y_axis.visible)
	{
		HC_Set_Text_Alignment("<");
		HC_Insert_Line(m_origin.x, m_y_axis.min, m_origin.z, m_origin.x, m_y_axis.max, m_origin.z);
		for(i=m_y_axis.tick_frequency; m_origin.y+i<=m_y_axis.max || m_origin.y-i>=m_y_axis.min; i+=m_y_axis.tick_frequency)
		{
			if(m_origin.y+i>=m_y_axis.min && m_origin.y+i<=m_y_axis.max)
			{
				HC_Insert_Line(m_origin.x-m_y_axis.tick_size/2, m_origin.y+i, m_origin.z, m_origin.x+m_y_axis.tick_size/2, m_origin.y+i, m_origin.z);
				if(m_y_axis.show_labels)
				{
					d_num=(m_y_axis.axis_scale==AxisScaleLinear ? i*m_y_axis.axis_scale_factor : 
						pow(m_y_axis.axis_scale_factor, i));
					d_num+=m_origin.y;
					if(m_y_axis.precision<0)
						sprintf(num, (fmod(d_num>0?d_num:-d_num, 1.0)>0. ? "%.3f" : "%.0f"), d_num);
					else
						sprintf(num, "%.*f", m_y_axis.precision, d_num);
					HC_Insert_Text(m_origin.x+m_x_axis.tick_size, m_origin.y+i, m_origin.z, num);
				}
			}

			if(m_origin.y-i>=m_y_axis.min && m_origin.y-i<=m_y_axis.max)
			{
				HC_Insert_Line(m_origin.x-m_y_axis.tick_size/2, m_origin.y-i, m_origin.z, m_origin.x+m_y_axis.tick_size/2, m_origin.y-i, m_origin.z);
				if(m_y_axis.show_labels)
				{
					d_num=(m_y_axis.axis_scale==AxisScaleLinear ? i*m_y_axis.axis_scale_factor : 
						pow(m_y_axis.axis_scale_factor, i));
					d_num=m_origin.y-d_num;
					if(m_y_axis.precision<0)
						sprintf(num, (fmod(d_num>0?d_num:-d_num, 1.0)>0. ? "%.3f" : "%.0f"), d_num);
					else
						sprintf(num, "%.*f", m_y_axis.precision, d_num);
					HC_Insert_Text(m_origin.x+m_x_axis.tick_size, m_origin.y-i, m_origin.z, num);
				}
			}
		}
	}
	HC_Close_Segment();
}

void HPlot2D::DrawGrid()
{
	double i;
	HPoint p1, p2, p3;
	int n;

	if(!m_automatic_updates && !m_force_update)
		return;

	HC_Open_Segment_By_Key(m_plot_segment);
	HC_Open_Segment("Grid");

	HC_Flush_Contents(".", "geometry");
	HC_Set_Rendering_Options("general displacement=4");

	if(m_grid_type==GridTypeRectangular) 
	{
		for(i=m_x_axis.grid_min; i<=m_x_axis.grid_max; i+=m_x_axis.grid_frequency)
			HC_Insert_Line(i, m_y_axis.grid_min, m_origin.z, i, m_y_axis.grid_max, m_origin.z);

		for(i=m_y_axis.grid_min; i<=m_y_axis.grid_max; i+=m_y_axis.grid_frequency)
			HC_Insert_Line(m_x_axis.grid_min, i, m_origin.z, m_x_axis.grid_max, i, m_origin.z);

	}

	else if(m_grid_type==GridTypePolar)
	{
		double x_m, x_M, y_m, y_M, max_dist, theta_1, theta_2, eps=0.0001;

		x_M=(m_origin.x-m_x_axis.grid_min > m_x_axis.grid_max-m_origin.x ? 
			m_origin.x-m_x_axis.grid_min : m_x_axis.grid_max-m_origin.x);
		y_M=(m_origin.y-m_y_axis.grid_min > m_y_axis.grid_max-m_origin.y ? 
			m_origin.y-m_y_axis.grid_min : m_y_axis.grid_max-m_origin.y);
		max_dist=sqrt(y_M*y_M+x_M*x_M);

		HC_Set_Visibility("face=off, edge=on");
		for(i=m_x_axis.grid_frequency; i <= max_dist; i+=m_x_axis.grid_frequency)
		{
			//whole circles (easy)
			if(m_origin.x+i<=m_x_axis.grid_max && m_origin.y+i<=m_y_axis.grid_max
			&& m_origin.x-i>=m_x_axis.grid_min && m_origin.y-i>=m_y_axis.grid_min)
			{
				p1=HPoint(static_cast<float>(m_origin.x+i), m_origin.y, m_origin.z);
				p2=HPoint(m_origin.x, static_cast<float>(m_origin.y+i), m_origin.z);
				p3=HPoint(static_cast<float>(m_origin.x-i), m_origin.y, m_origin.z);
				HC_Insert_Circle(&p1, &p2, &p3);
			}
			
			//clipped circles
			else
			{
				x_m=m_origin.x-i;
				x_M=m_origin.x+i;
				y_m=m_origin.y-i;
				y_M=m_origin.y+i;
				theta_1=theta_2=0.;
				//start at positive x-axis and proceed around counter-clockwise
				if(x_M > m_x_axis.grid_max) x_M=m_x_axis.grid_max;
				if(y_M > m_y_axis.grid_max) y_M=m_y_axis.grid_max;
				if(x_m < m_x_axis.grid_min) x_m=m_x_axis.grid_min;
				if(y_m < m_y_axis.grid_min) y_m=m_y_axis.grid_min;

				//clipped on the right (quadrant I)
				if(x_M==m_x_axis.grid_max && sqrt(pow(m_x_axis.grid_max-m_origin.x, 2)+pow(m_y_axis.grid_max-m_origin.y,2))>i)
				{
					//get the first endpoint
					p1.x=static_cast<float>(x_M);
					theta_1=H_ACOS((x_M-m_origin.x)/i);
					p1.y=static_cast<float>(m_origin.y+i*sin(theta_1));
					p1.z=0.;

					//find the next endpoint of the arc

					//clipped at the top
					if(y_M==m_y_axis.grid_max)
					{
						p3.y=static_cast<float>(y_M);
						theta_2=H_ASIN((y_M-m_origin.y)/i);
						p3.x=static_cast<float>(m_origin.x+i*cos(theta_2));
						p3.z=0.;
					}
					//clipped on the left
					else if(x_m==m_x_axis.grid_min)
					{
						p3.x=static_cast<float>(x_m);
						theta_2=H_ACOS((x_m-m_origin.x)/i);
						p3.y=static_cast<float>(m_origin.y+i*sin(theta_2));
						p3.z=0.;
					}
					//clipped on the bottom
					else if(y_m==m_y_axis.grid_min)
					{
						p3.y=static_cast<float>(y_m);
						theta_2=_PI-H_ASIN((y_m-m_origin.y)/i);
						p3.x=static_cast<float>(m_origin.x+i*cos(theta_2));
						p3.z=0.;
					}
					//clipped only on the right
					else
					{
						p3.x=static_cast<float>(x_M);
						theta_2=static_cast<float>(2*_PI-H_ACOS((x_M-m_origin.x)/i));
						p3.y=static_cast<float>(m_origin.y+i*sin(theta_2));
						p3.z=0.0f;
					}

					//compute midpoint of arc
					p2.x=static_cast<float>(m_origin.x+i*cos((theta_1+theta_2)/2));
					p2.y=static_cast<float>(m_origin.y+i*sin((theta_1+theta_2)/2));
					p2.z=0.0f;

					//draw it
					HC_Insert_Circular_Arc(&p1, &p2, &p3);
				}

				//clipped on the top (quadrant II)
				if(y_M==m_y_axis.grid_max && sqrt(pow(m_y_axis.grid_max-m_origin.y, 2)+pow(m_x_axis.grid_min-m_origin.x, 2))>i)
				{
					//get the first endpoint
					p1.y=static_cast<float>(y_M);
					theta_1=static_cast<float>(_PI-H_ASIN((y_M-m_origin.y)/i));
					p1.x=static_cast<float>(m_origin.x+i*cos(theta_1));
					p1.z=0.0f;

					//find the next endpoint of the arc

					//clipped on the left
					if(x_m==m_x_axis.grid_min)
					{
						p3.x=static_cast<float>(x_m);
						theta_2=static_cast<float>(H_ACOS((x_m-m_origin.x)/i));
						p3.y=static_cast<float>(m_origin.y+i*sin(theta_2));
						p3.z=0.0f;
					}
					//clipped on the bottom
					else if(y_m==m_y_axis.grid_min)
					{
						p3.y=static_cast<float>(y_m);
						theta_2=static_cast<float>(_PI-H_ASIN((y_m-m_origin.y)/i));
						p3.x=static_cast<float>(m_origin.x+i*cos(theta_2));
						p3.z=0.0f;
					}
					//clipped on the right
					else if(x_M==m_x_axis.grid_max)
					{
						p3.x=static_cast<float>(x_M);
						theta_2=static_cast<float>(2*_PI-H_ACOS((x_M-m_origin.x)/i));
						p3.y=static_cast<float>(m_origin.y+i*sin(theta_2));
						p3.z=0.0f;
					}
					//clipped only on the top
					else
					{
						p3.y=static_cast<float>(y_M);
						theta_2=static_cast<float>(2*_PI+H_ASIN((y_M-m_origin.y)/i));
						p3.x=static_cast<float>(m_origin.x+i*cos(theta_2));
						p3.z=0.0f;
					}

					//compute midpoint of arc
					p2.x=static_cast<float>(m_origin.x+i*cos((theta_1+theta_2)/2));
					p2.y=static_cast<float>(m_origin.y+i*sin((theta_1+theta_2)/2));
					p2.z=0.0f;

					//draw it
					HC_Insert_Circular_Arc(&p1, &p2, &p3);
				}

				//clipped on the left (quadrant III)
				if(x_m==m_x_axis.grid_min && sqrt(pow(m_x_axis.grid_min-m_origin.x, 2)+pow(m_y_axis.grid_min-m_origin.y, 2))>i)
				{
					//get the first endpoint
					p1.x=static_cast<float>(x_m);
					theta_1=static_cast<float>(2*_PI-H_ACOS((x_m-m_origin.x)/i));
					p1.y=static_cast<float>(m_origin.y+i*sin(theta_1));
					p1.z=0.0f;

					//find the next endpoint of the arc

					//clipped on the bottom
					if(y_m==m_y_axis.grid_min)
					{
						p3.y=static_cast<float>(y_m);
						theta_2=static_cast<float>(_PI-H_ASIN((y_m-m_origin.y)/i));
						p3.x=static_cast<float>(m_origin.x+i*cos(theta_2));
						p3.z=0.0f;
					}
					//clipped on the right
					else if(x_M==m_x_axis.grid_max)
					{
						p3.x=static_cast<float>(x_M);
						theta_2=static_cast<float>(2*_PI-H_ACOS((x_M-m_origin.x)/i));
						p3.y=static_cast<float>(m_origin.y+i*sin(theta_2));
						p3.z=0.0f;
					}
					//clipped at the top
					else if(y_M==m_y_axis.grid_max)
					{
						p3.y=static_cast<float>(y_M);
						theta_2=static_cast<float>(2*_PI+H_ASIN((y_M-m_origin.y)/i));
						p3.x=static_cast<float>(m_origin.x+i*cos(theta_2));
						p3.z=0.0f;
					}
					//clipped only on the left
					else
					{
						p3.x=static_cast<float>(x_m);
						theta_2=static_cast<float>(2*_PI+H_ACOS((x_m-m_origin.x)/i));
						p3.y=static_cast<float>(m_origin.y+i*sin(theta_2));
						p3.z=0.0f;
					}

					//compute midpoint of arc
					p2.x=static_cast<float>(m_origin.x+i*cos((theta_1+theta_2)/2));
					p2.y=static_cast<float>(m_origin.y+i*sin((theta_1+theta_2)/2));
					p2.z=0.0f;

					//draw it
					HC_Insert_Circular_Arc(&p1, &p2, &p3);
				}

				//clipped on the bottom (quadrant IV)
				if(y_m==m_y_axis.grid_min && sqrt(pow(m_y_axis.grid_min-m_origin.y, 2)+pow(m_x_axis.grid_max-m_origin.x, 2))>i)
				{
					//get the first endpoint
					p1.y=static_cast<float>(y_m);
					theta_1=static_cast<float>(H_ASIN((y_m-m_origin.y)/i));
					p1.x=static_cast<float>(m_origin.x+i*cos(theta_1));
					p1.z=0.0f;

					//find the next endpoint of the arc

					//clipped on the right
					if(x_M==m_x_axis.grid_max)
					{
						p3.x=static_cast<float>(x_M);
						theta_2=static_cast<float>(-H_ACOS((x_M-m_origin.x)/i));
						p3.y=static_cast<float>(m_origin.y+i*sin(theta_2));
						p3.z=0.0f;
					}
					//clipped on the top
					else if(y_M==m_y_axis.grid_max)
					{
						p3.y=static_cast<float>(y_M);
						theta_2=static_cast<float>(H_ASIN((y_M-m_origin.y)/i));
						p3.x=static_cast<float>(m_origin.x+i*cos(theta_2));
						p3.z=0.0f;
					}
					//clipped on the left
					else if(x_m==m_x_axis.grid_min)
					{
						p3.x=static_cast<float>(x_m);
						theta_2=static_cast<float>(H_ACOS((x_m-m_origin.x)/i));
						p3.y=static_cast<float>(m_origin.y+i*sin(theta_2));
						p3.z=0.0f;
					}
					//clipped only on the bottom
					else 
					{
						p3.y=static_cast<float>(y_m);
						theta_2=static_cast<float>(_PI-H_ASIN((y_m-m_origin.y)/i));
						p3.x=static_cast<float>(m_origin.x+i*cos(theta_2));
						p3.z=0.0f;
					}

					//compute midpoint of arc
					p2.x=static_cast<float>(m_origin.x+i*cos((theta_1+theta_2)/2));
					p2.y=static_cast<float>(m_origin.y+i*sin((theta_1+theta_2)/2));
					p2.z=0.0f;

					//draw it
					HC_Insert_Circular_Arc(&p1, &p2, &p3);
				}
			} //end clipped circle drawing
		} //end for

		//draw radial lines
		p1.x=m_origin.x;
		p1.y=m_origin.y;
		p1.z=m_origin.z;
		
		for(n=1; (theta_1=n*m_y_axis.grid_frequency)<2*_PI; ++n)
		{
			if(theta_1<_PI/2) //quadrant I
			{
				x_M=m_x_axis.grid_max;
				y_M=m_origin.y+((x_M-m_origin.x)/cos(theta_1))*sin(theta_1);

				if(y_M<=m_y_axis.grid_max)
				{
					p2.x=static_cast<float>(x_M);
					p2.y=static_cast<float>(y_M);
					p2.z=0.0f;
				}
				else
				{
					p2.y=static_cast<float>(m_y_axis.grid_max);
					p2.x=static_cast<float>(m_origin.x+((p2.y-m_origin.y)/sin(theta_1))*cos(theta_1));
					p2.z=0.0f;
				}
			}
			else if(fabs(theta_1-_PI/2)<eps)
			{
				p2.y=static_cast<float>(m_y_axis.grid_max);
				p2.x=static_cast<float>(m_origin.x+((p2.y-m_origin.y)/sin(theta_1))*cos(theta_1));
				p2.z=0.0f;
			}
			else if(theta_1<_PI) //quadrant II
			{
				x_M=static_cast<float>(m_x_axis.grid_min);
				y_M=static_cast<float>(m_origin.y+((x_M-m_origin.x)/cos(theta_1))*sin(theta_1));

				if(y_M<=m_y_axis.grid_max)
				{
					p2.x=static_cast<float>(x_M);
					p2.y=static_cast<float>(y_M);
					p2.z=0.0f;
				}
				else
				{
					p2.y=static_cast<float>(m_y_axis.grid_max);
					p2.x=static_cast<float>(m_origin.x+((p2.y-m_origin.y)/sin(theta_1))*cos(theta_1));
					p2.z=0.0f;
				}
			}
			else if(theta_1<3*_PI/2) //quadrant III
			{
				x_M=static_cast<float>(m_x_axis.grid_min);
				y_M=static_cast<float>(m_origin.y+((x_M-m_origin.x)/cos(theta_1))*sin(theta_1));

				if(y_M>=m_y_axis.grid_min)
				{
					p2.x=static_cast<float>(x_M);
					p2.y=static_cast<float>(y_M);
					p2.z=0.0f;
				}
				else
				{
					p2.y=static_cast<float>(m_y_axis.grid_min);
					p2.x=static_cast<float>(m_origin.x+((p2.y-m_origin.y)/sin(theta_1))*cos(theta_1));
					p2.z=0.0f;
				}
			}
			else if(fabs(theta_1-3*_PI/2)<eps)
			{
				p2.y=static_cast<float>(m_y_axis.grid_min);
				p2.x=static_cast<float>(m_origin.x+((p2.y-m_origin.y)/sin(theta_1))*cos(theta_1));
				p2.z=0.0f;
			}
			else //quadrant IV
			{
				x_M=static_cast<float>(m_x_axis.grid_max);
				y_M=static_cast<float>(m_origin.y+((x_M-m_origin.x)/cos(theta_1))*sin(theta_1));

				if(y_M>=m_y_axis.grid_min)
				{
					p2.x=static_cast<float>(x_M);
					p2.y=static_cast<float>(y_M);
					p2.z=0.0f;
				}
				else
				{
					p2.y=static_cast<float>(m_y_axis.grid_min);
					p2.x=static_cast<float>(m_origin.x+((p2.y-m_origin.y)/sin(theta_1))*cos(theta_1));
					p2.z=0.0f;
				}
			}

			HC_Insert_Line(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
		}

	}

	else; //None

	HC_Close_Segment();
	HC_Close_Segment();
}

double HPlot2D::GetAspectRatio()
{
	return m_aspect_ratio;
}

void HPlot2D::SetAspectRatio(double ratio)
{
	if (ratio > 0) {
		m_aspect_ratio = ratio;
		HC_Open_Segment_By_Key(m_plot_segment);
			if (HC_Show_Existence("modelling matrix"))
				HC_UnSet_Modelling_Matrix();

			double bottom,top, left,right, vert,horiz;
			top		= m_y_axis.max>m_y_axis.grid_max ? m_y_axis.max : m_y_axis.grid_max;
			bottom	= m_y_axis.min<m_y_axis.grid_min ? m_y_axis.min : m_y_axis.grid_min;
			right	= m_x_axis.max>m_x_axis.grid_max ? m_x_axis.max : m_x_axis.grid_max;
			left	= m_x_axis.min<m_x_axis.grid_min ? m_x_axis.min : m_x_axis.grid_min;

			vert  = top - bottom;
			horiz = right - left;

			if (horiz==0)
				HDB::ReportError("Warning: Cannot set aspect ratio: graph has width of zero");
			else if (vert == 0)
				HDB::ReportError("Warning: Cannot set aspect ratio: graph has height of zero");

			if (horiz >= vert) // shore up vertical
				if (ratio >= 1)	// extend horizontal
					HC_Scale_Object(ratio, horiz/vert, 1);
				else			// extend vertical
					HC_Scale_Object(1, horiz/(vert * ratio), 1);
			else // shore up horizontal
				if (ratio >= 1)	// extend horizontal
					HC_Scale_Object(ratio * vert/horiz, 1, 1);
				else			// extend vertical
					HC_Scale_Object(vert/horiz, 1/ratio, 1);
		HC_Close_Segment();
	}
}

void HPlot2D::UnSetAspectRatio()
{
	m_aspect_ratio=0;
	HC_Open_Segment_By_Key(m_plot_segment);
		if (HC_Show_Existence("modelling matrix"))
			HC_UnSet_Modelling_Matrix();
	HC_Close_Segment();
}

void HPlot2D::DrawFrame()
{
	double top, bottom, left, right;

	if(!m_automatic_updates && !m_force_update)
		return;

	top=m_y_axis.max>m_y_axis.grid_max ? m_y_axis.max : m_y_axis.grid_max;
	bottom=m_y_axis.min<m_y_axis.grid_min ? m_y_axis.min : m_y_axis.grid_min;
	right=m_x_axis.max>m_x_axis.grid_max ? m_x_axis.max : m_x_axis.grid_max;
	left=m_x_axis.min<m_x_axis.grid_min ? m_x_axis.min : m_x_axis.grid_min;
	
	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Frame");
			HC_Flush_Contents(".", "geometry");
			HC_Set_Visibility("lines=on");

		if(m_show_frame)
		{
			HC_Insert_Line(left, top, m_origin.z, right, top, m_origin.z);
			HC_Insert_Line(left, bottom, m_origin.z, left, top, m_origin.z);
			HC_Insert_Line(right, bottom, m_origin.z, right, top, m_origin.z);
			HC_Insert_Line(left, bottom, m_origin.z, right, bottom, m_origin.z);
		}

		HC_Close_Segment();
	HC_Close_Segment();
}

void HPlot2D::DrawData(int data_set)
{
	int count=m_points[data_set]->data_size, color_count;
	HPoint *data_points, bar[4];
	HC_KEY geo_key;

	data_points=new HPoint[count];

	GetDataSet(data_set, data_points);

	HC_Open_Segment_By_Key(m_points[data_set]->segment);

	HC_Begin_Contents_Search(".", "polylines");
	HC_Show_Contents_Count(&count);
	if(count>0)	HC_Flush_Contents(".", "polylines");
	HC_End_Contents_Search();

	HC_Begin_Contents_Search(".", "segment");
	HC_Show_Contents_Count(&count);
	if(count>0)	HC_Flush_Contents(".", "segment");
	HC_End_Contents_Search();

	HC_Show_Net_Color_Map_Count(&color_count);

	if(m_points[data_set]->line_vis)
		HC_Insert_Polyline(m_points[data_set]->data_size, data_points);

	if(m_points[data_set]->polygon_vis)
	{
		count=m_points[data_set]->data_size;
		int *f_list=new int[count+1];

		f_list[0]=count;
		for(int i=0; i<count; ++i)
			f_list[i+1]=i;
		
		HC_Open_Segment("");
		HC_Set_Visibility("faces");
		HC_Insert_Shell(count, data_points, count+1, f_list);
		HC_Close_Segment();

		delete [] f_list;
	}

	if(m_points[data_set]->bar_vis)
	{
		for(int i=0; i<m_points[data_set]->data_size; ++i)
		{

			bar[0].x=static_cast<float>(data_points[i].x-m_points[data_set]->bar_width/2);
			bar[0].y=m_origin.y;
			bar[0].z=m_origin.z;

			bar[1].x=static_cast<float>(data_points[i].x+m_points[data_set]->bar_width/2);
			bar[1].y=m_origin.y;
			bar[1].z=m_origin.z;

			bar[2].x=static_cast<float>(data_points[i].x-m_points[data_set]->bar_width/2);
			bar[2].y=data_points[i].y;
			bar[2].z=m_origin.z;

			bar[3].x=static_cast<float>(data_points[i].x+m_points[data_set]->bar_width/2);
			bar[3].y=data_points[i].y;
			bar[3].z=m_origin.z;

			HC_Open_Segment("");
			geo_key=HC_KInsert_Mesh(2, 2, bar);
			HC_Set_Visibility("no markers");
			HC_Set_Color_By_Index("faces", i%color_count);
			HC_Close_Segment();

			HC_Open_Geometry(geo_key);
			HC_Open_Edge(1,2);
			HC_Set_Visibility("edges=off");
			HC_Close_Edge();
			HC_Close_Geometry();
		}
	}

	HC_Close_Segment();
	delete [] data_points;
}

//////////////////////////////////////////////////////////////


//////////////////////    HPieChart    ///////////////////////

//constructor
HPieChart::HPieChart(HC_KEY plot_seg) : HBaseGraph(plot_seg)
{
	m_scale=1.0;
	m_pie.ReplaceAt(0,0);
	m_pie_total=0.0;
}

//destructor
HPieChart::~HPieChart()
{
	if(m_preserve_data)
	{
		size_t max=m_pie.Count();
		for (size_t i=0; i<max; i++)
		{
			if(m_pie[i]!=0)
			{
				delete m_pie[i];
				m_pie[i]=0;
			}
		}
	}
	else
	{
		size_t max=m_pie.Count();
		for (size_t i=0; i<max; i++)
			RemovePieSlice((int)i);
	}
}

//get and set the plot title
void HPieChart::SetPlotTitle(const char * title)
{
	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Labels");
			HC_Open_Segment("Title");
				HC_Set_Text_Alignment("*v");
				HC_Flush_Contents(".", "text");
				HC_Insert_Text(m_origin.x, m_origin.y+m_scale*1.35, m_origin.z, title);
			HC_Close_Segment();
		HC_Close_Segment();
	HC_Close_Segment();
}

void HPieChart::SetPlotUnicodeTitle(unsigned short * title)
{
	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Labels");
			HC_Open_Segment("Title");
				HC_Flush_Contents(".", "text");
				HC_Insert_Unicode_Text(m_origin.x, m_scale*1.35, m_origin.z, title);
			HC_Close_Segment();
		HC_Close_Segment();
	HC_Close_Segment();
}

//adds a slice to a pie chart, returns the id of the pie slice
int HPieChart::AddPieSlice(double size)
{
	int n = (int)m_pie.Count();
	char opt_label[MVO_BUFFER_SIZE];
	sprintf(opt_label, "H_pie_slice_id=%d", n);
	m_pie.Append(new HGraphPieSlice);
	m_pie[n]->amount=size;
	m_pie[n]->text_key=INVALID_KEY;
	m_pie_total+=size;

	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Pie");
			m_pie[n]->segment=HC_KOpen_Segment("");
			HC_Set_User_Options(opt_label);
			HC_Close_Segment();
		HC_Close_Segment();
	HC_Close_Segment();

	DrawPie();
	return n;
}

//removes (eats) a pie slice
void HPieChart::RemovePieSlice(int slice_id)
{
	if(slice_id >= (int)m_pie.Count() || m_pie[slice_id]==0) 
		return;

	m_pie_total-=m_pie[slice_id]->amount;
	HC_Delete_By_Key(m_pie[slice_id]->segment);
	delete m_pie[slice_id];
	m_pie[slice_id]=0;
}

//get and set the pie slice color map
void HPieChart::SetPieColorMap(const char * map)
{
	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Pie");
			HC_Set_Color_Map(map);
		HC_Close_Segment();
	HC_Close_Segment();

	DrawPie();
}

void HPieChart::GetPieColorMap(char * map)
{
	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Pie");
			HC_Show_Net_Color_Map(map);
		HC_Close_Segment();
	HC_Close_Segment();
}

//get and set color map by value (see HC_Set_Color_Map)
void HPieChart::SetPieColorMapByValue(int count, const HPoint * values, const char * color_space)
{
	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Pie");
			HC_Set_Color_Map_By_Value((color_space==0?"RGB":color_space), count, values);
		HC_Close_Segment();
	HC_Close_Segment();

	DrawPie();	
}
void HPieChart::GetPieColorMapByValue(int * count, HPoint * values,  char * color_space)
{
	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Pie");
			HC_Show_Net_Color_Map_By_Value(color_space, count, values);
		HC_Close_Segment();
	HC_Close_Segment();
}

//add or replace a label to a pie slice
void HPieChart::AddPieSliceLabel(int slice_id, const char * str)
{
	HC_Open_Segment_By_Key(m_pie[slice_id]->segment);
		m_pie[slice_id]->text_key=HC_KInsert_Text(0.,0.,0., str);
	HC_Close_Segment();

	DrawPie();
}

void HPieChart::AddPieSliceUnicodeLabel(int slice_id, const unsigned short * str)
{
	HC_Open_Segment_By_Key(m_pie[slice_id]->segment);
		m_pie[slice_id]->text_key=HC_KInsert_Unicode_Text(0.,0.,0., str);
	HC_Close_Segment();

	DrawPie();
}

//remove the label from a previously labeled pie slice
void HPieChart::RemovePieSliceLabel(int slice_id)
{
	if(slice_id >= (int)m_pie.Count() || m_pie[slice_id] == 0)
	{
		HDB::ReportError("Warning: Specified pie slice does not exist.");
		return;
	}

	HC_Delete_By_Key(m_pie[slice_id]->text_key);
	m_pie[slice_id]->text_key=INVALID_KEY;

	//no need to redraw the rest of the pie
}

//show the contents of a label
void HPieChart::GetPieSliceLabelContents(int slice_id, char * str)
{
	float x, y, z;

	if(slice_id >= (int)m_pie.Count() || m_pie[slice_id] == 0)
	{
		HDB::ReportError("Warning: Specified pie slice does not exist.");
		return;
	}

	HC_Show_Text(m_pie[slice_id]->text_key, &x, &y, &z, str);
}

void HPieChart::GetPieSliceUnicodeLabelContents(int slice_id, unsigned short * str)
{
	float x, y, z;

	if(slice_id >= (int)m_pie.Count() || m_pie[slice_id] == 0)
	{
		HDB::ReportError("Warning: Specified pie slice does not exist.");
		return;
	}

	HC_Show_Unicode_Text(m_pie[slice_id]->text_key, &x, &y, &z, str);
}

//get and set the slice label font
void HPieChart::SetPieSliceLabelTextFont(int slice_id, const char * font)
{
	if(slice_id >= (int)m_pie.Count() || m_pie[slice_id] == 0)
	{
		HDB::ReportError("Warning: Specified pie slice does not exist.");
		return;
	}

	HC_Open_Segment_By_Key(m_pie[slice_id]->segment);{
		HC_Set_Text_Font(font);
	}HC_Close_Segment();
}

void HPieChart::GetPieSliceLabelTextFont(int slice_id, char * font)
{
	if(slice_id >= (int)m_pie.Count() || m_pie[slice_id] == 0)
	{
		HDB::ReportError("Warning: Specified pie slice does not exist.");
		return;
	}

	HC_Open_Segment_By_Key(m_pie[slice_id]->segment);{
		HC_Show_Text_Font(font);
	}HC_Close_Segment();
}

bool HPieChart::GetPieEdgeVisibility()
{
	char value[MVO_BUFFER_SIZE];
	
	HC_Open_Segment_By_Key(m_plot_segment);{
		HC_PShow_One_Net_Visibility(0, 0, "edges", value);
	}HC_Close_Segment();
	
	return value[1] == 'n'; //n for on, f for off
}
void HPieChart::SetPieEdgeVisibility(bool vis)
{
	char str[MVO_BUFFER_SIZE];
	sprintf(str, "edges=%s", (vis?"on":"off"));

	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Set_Visibility(str);
	HC_Close_Segment();
}

//get and set pie slice edge visibility
void HPieChart::GetPieEdgeColor(char * color)
{
	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Show_One_Net_Color("edges", color);
	HC_Close_Segment();		
}

void HPieChart::SetPieEdgeColor(const char * color)
{
	char str[MVO_BUFFER_SIZE];
	sprintf(str, "edges=%s", color);

	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Set_Color(str);
	HC_Close_Segment();		
}

double HPieChart::GetPieRadius()
{
	return m_scale;
}

void HPieChart::SetPieRadius(double radius)
{
	m_scale=radius;

	DrawPie();
	DrawFrame();
	SetPlotTitleLocation(HPoint(m_origin.x, static_cast<float>(m_origin.y+m_scale*1.35), m_origin.z));
}

void HPieChart::SetPlotOrigin(HPoint origin)
{
	m_origin=origin;
	
	DrawPie();
	DrawFrame();
	SetPlotTitleLocation(HPoint(m_origin.x, static_cast<float>(m_origin.y+m_scale*1.35), m_origin.z));
}
void HPieChart::Update()
{
	HBaseGraph::Update();
	m_force_update=true;
	DrawFrame();
	DrawPie();
	m_force_update=true;
}

void HPieChart::DrawFrame()
{
	HPoint p1, p2, p3;
	HC_KEY circle_key;

	if(!m_automatic_updates && !m_force_update)
		return;
	
	HC_Open_Segment_By_Key(m_plot_segment);
		HC_Open_Segment("Frame");
			HC_Flush_Contents(".", "geometry");
			HC_Set_Visibility("faces=off, edges=on");

			if(m_show_frame)
			{
				p1=HPoint(static_cast<float>(m_origin.x+m_scale), m_origin.y, m_origin.z);
				p2=HPoint(m_origin.x, static_cast<float>(m_origin.y+m_scale), m_origin.z);
				p3=HPoint(static_cast<float>(m_origin.x-m_scale), m_origin.y, m_origin.z);
				circle_key=HC_KInsert_Circle(&p1, &p2, &p3);
				HC_Set_Circular_Center_By_Key(circle_key, m_origin.x, m_origin.y, m_origin.z);
			}

		HC_Close_Segment();
	HC_Close_Segment();
}

void HPieChart::DrawPie()
{
	double theta_1=0., theta_2=0.;
	int color_count;
	HC_KEY wedge_key;
	HPoint p1, p2, p3;

	if(!m_automatic_updates && !m_force_update)
		return;

	size_t	max = m_pie.Count();
	int		j=0;

	for(size_t i=0; i<max; ++i)
	{
		if(m_pie[i]==0)
			continue;

		theta_1=theta_2;
		theta_2=theta_1+2*_PI*m_pie[i]->amount/m_pie_total;

		p1.x=static_cast<float>(m_origin.x+m_scale*cos(theta_1));
		p1.y=static_cast<float>(m_origin.y+m_scale*sin(theta_1));
		p1.z=m_origin.z;
		p2.x=static_cast<float>(m_origin.x+m_scale*cos((theta_1+theta_2)/2));
		p2.y=static_cast<float>(m_origin.y+m_scale*sin((theta_1+theta_2)/2));
		p2.z=m_origin.z;
		p3.x=static_cast<float>(m_origin.x+m_scale*cos(theta_2));
		p3.y=static_cast<float>(m_origin.y+m_scale*sin(theta_2));
		p3.z=m_origin.z;

		HC_Open_Segment_By_Key(m_pie[i]->segment);
		HC_Flush_Contents(".", "geometry, colors, no text, no text font");

		if(m_pie[i]->text_key!=INVALID_KEY) 
			HC_Move_Text(m_pie[i]->text_key, 1.1*(p2.x-m_origin.x)+m_origin.x, 1.1*(p2.y-m_origin.y)+m_origin.y, p2.z);

		HC_Show_Net_Color_Map_Count(&color_count);
		wedge_key=HC_KInsert_Circular_Wedge(&p1, &p2, &p3);
		HC_Set_Circular_Center_By_Key(wedge_key, m_origin.x, m_origin.y, m_origin.z);
		HC_Set_Color_By_Index("faces", j++%color_count);

		HC_Close_Segment();
	}

}








