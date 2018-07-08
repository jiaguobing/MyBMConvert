
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HTools.h"
#include "hc.h"
#include "hic.h"
#include "HImManager.h"
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "MyPolygonMarkupOp.h"
#include "HMarkupManager.h"

#include <stdlib.h>

MyPolygonMarkupOp::MyPolygonMarkupOp(HBaseView* view, int DoRepeat, int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture)
{
	m_bOpStarted = false;
	m_LastKey = -1;
	markup_seg[0] = '\0';
}


HBaseOperator * MyPolygonMarkupOp::Clone()
{
	return (HBaseOperator *)new MyPolygonMarkupOp(m_pView);
}


const char * MyPolygonMarkupOp::GetName() { return "MyPolygonMarkupOp"; }

long MyPolygonMarkupOp::markup_incarnation = 0;

void MyPolygonMarkupOp::CompleteLastMarkup()
{
	char type[128] = {""};
	int screen_points_count, polygon_points_count;
	HC_KEY key;	
	HPoint * screen_points = 0;
	HPoint * polygon_points = 0;
	
	HC_Open_Segment(markup_seg);{
		HC_Begin_Contents_Search(".","inked line");{
			if(HC_Find_Contents(type, &key)){
				HC_Show_Polyline_Count(key, &polygon_points_count);
				polygon_points = new HPoint[polygon_points_count];
				HC_Show_Polyline(key, &polygon_points_count, polygon_points);
				printf("Got the points for the polygon.\n");
			}
		}HC_End_Contents_Search();
		HC_Open_Segment("screen space coords");{
			HC_Begin_Contents_Search(".","inked line");{
				if(HC_Find_Contents(type, &key)){
					HC_Show_Polyline_Count(key, &screen_points_count);
					screen_points = new HPoint[screen_points_count];
					HC_Show_Polyline(key, &screen_points_count, screen_points);
					printf("Got the screen points for the polygon.\n");
				}
			}HC_End_Contents_Search();
		}HC_Close_Segment();

		HC_Flush_Contents(".","everything");
		
		if(screen_points && polygon_points){
			/* bounding box */
			HPoint min_point, max_point;
			int grid_rows, grid_cols;
			bool *grid;
			
			min_point = max_point = screen_points[0];
			for(int i=1; i<screen_points_count; ++i){
				if( min_point.x > screen_points[i].x ) min_point.x = screen_points[i].x;
				if( min_point.y > screen_points[i].y ) min_point.y = screen_points[i].y;
				if( max_point.x < screen_points[i].x ) max_point.x = screen_points[i].x;
				if( max_point.y < screen_points[i].y ) max_point.y = screen_points[i].y;
			}
			grid_rows = (int)((max_point.y - min_point.y)*100)+1;
			grid_cols = (int)((max_point.x - min_point.x)*100)+1;
			grid = new bool[(grid_rows+2)*(grid_cols+2)];
			
			if(grid){
				for(int row=0; row<grid_rows; ++row){
					for(int col=0; col<grid_cols; ++col){
						grid[row*grid_cols + col] = false;
					}
				}
				for(int i=0; i<screen_points_count; ++i){
					int row1, col1;
					int row2, col2;
					int len;
					
					col1 = (int)((screen_points[i].x-min_point.x)*100);
					row1 = (int)((screen_points[i].y-min_point.y)*100);
					if(i+1 < screen_points_count){
						col2 = (int)((screen_points[i+1].x-min_point.x)*100);
						row2 = (int)((screen_points[i+1].y-min_point.y)*100);
					} else {
						col2 = (int)((screen_points[0].x-min_point.x)*100);
						row2 = (int)((screen_points[0].y-min_point.y)*100);
					}
					grid[row1*grid_cols + col1] = true;
					grid[row2*grid_cols + col2] = true;
					printf("%d %d\n", row1, col1);
					len = (int)sqrt((col2-col1)*(col2-col1)+(row2-row1)*(row2-row1)) +1;
					printf("%d\n", len);
					if(len > 1){
						for(int d=0; d<len; ++d){
							int row3, col3;

							row3 = (int)((row2-row1)*(d/(float)(len-1))+row1);
							col3 = (int)((col2-col1)*(d/(float)(len-1))+col1);
							printf("%d %d %d\n", d, row3, col3);
							grid[row3*grid_cols + col3] = true;
						}
					}
					printf("%d %d\n", row2, col2);
				}
				printf("-\n");
				for(int row=0; row<grid_rows; ++row){
					printf("|");
					for(int col=0; col<grid_cols; ++col){
						if(grid[row*grid_cols + col])
							printf("*");
						else printf(" ");
					}
					printf("|\n");
				}
				printf("-\n");
				for(int row=0; row<grid_rows; ++row){
					for(int col=0; col<grid_rows; ++col){
						bool in = false;
						if(grid[row*grid_cols + col]){
						} else {
						}
					}
				}
				delete [] grid;
			}
			delete [] screen_points;
			delete [] polygon_points;
		}
	}HC_Close_Segment();
}

void MyPolygonMarkupOp::BeginNewMarkup()
{
	char owner[4096] = {""};
	++markup_incarnation;
	HC_Show_Owner_By_Key(m_LastKey, owner);
	snprintf(markup_seg, sizeof(markup_seg), "%s/Polygon Markup %ld", owner, markup_incarnation);
	HC_Open_Segment(markup_seg);{
		HC_Set_Visibility("off, lines=on");
		HC_Set_Selectability("off");
		HC_Set_Color("yellow");
		HC_Open_Segment("screen space coords");{
			HC_Set_Visibility("off");
		}HC_Close_Segment();
	}HC_Close_Segment();
}

void MyPolygonMarkupOp::MarkupSelection(HEventInfo &event)
{
	int offset1, offset2, offset3, count;
	HPoint window, world;
	HC_KEY key;

	m_ptNew = event.GetMouseWindowPos();

	HC_Open_Segment_By_Key(m_pView->GetViewKey());{
		count = HC_Compute_Selection(".", ".", "v", m_ptNew.x, m_ptNew.y);
		if(count > 0){
			/* Got something!  Either start and inscription or continue the last one. */
			HC_Show_Selection_Element(&key, &offset1, &offset2, &offset3);
			if(key != m_LastKey){
				if(m_LastKey != -1)
					CompleteLastMarkup();
				m_LastKey = key;
				BeginNewMarkup();
			} else {
			}
			HC_Show_Selection_Position(&window.x, &window.y, &window.z, &world.x, &world.y, &world.z);
			HC_Open_Segment(markup_seg);{
				HC_Insert_Ink(world.x, world.y, world.z);
				HC_Open_Segment("screen space coords");{
					HC_Insert_Ink(window.x, window.y, window.z);
				}HC_Close_Segment();
			}HC_Close_Segment();
		} else {
			/* Nothing selected. Either do nothing or end the last inscription */
			if(m_LastKey != -1)
				CompleteLastMarkup();
			m_LastKey = -1;
		}
	}HC_Close_Segment();
}

int MyPolygonMarkupOp::OnLButtonDown(HEventInfo &event)
{
	m_bOpStarted = true;
	MarkupSelection(event);
	return (HOP_OK);
}



int MyPolygonMarkupOp::OnLButtonDownAndMove(HEventInfo &event)
{
	if (!m_bOpStarted) 
		return HBaseOperator::OnNoButtonDownAndMove(event);

	MarkupSelection(event);
    m_pView->Update();    
	return (HOP_OK);
}



int MyPolygonMarkupOp::OnLButtonUp(HEventInfo &event)
{
	CompleteLastMarkup();
	m_LastKey = -1;
	m_bOpStarted = false;
    m_pView->Update();    
	return(HOP_READY);
}


void MyPolygonMarkupOp::CreateMarkupPolygon(HBaseView *pView, const char *segment, unsigned int PolyLineLength, HPoint *pPolyline, bool emit_message)
{
#if 0
	HMarkupManager *markupManager;
	HC_KEY freehand=0;
	if ((markupManager = pView->GetMarkupManager()))
	{
		HC_KEY activelayerkey;
		if ((activelayerkey = markupManager->GetCurrentLayerKey()) == -1)
			activelayerkey = markupManager->OpenLayer("");
		
		HC_Open_Segment_By_Key(activelayerkey);
 			freehand = HC_KOpen_Segment (segment);
				// this is now hoops default behaviour but might be useful for "no absolute line weight"
				//HImSetCallback("draw dc polyline","HImUtility_device_independent_polyline_weights");
				HC_Insert_Polyline(PolyLineLength,pPolyline);
				// let's set a round end cap so that we 
				// don't get spikes in the polyline
				HC_Set_Line_Pattern("(----)");
 			HC_Close_Segment();
		HC_Close_Segment();	
	}

	if (freehand && emit_message)
		markupManager->EmitMarkupSegment(freehand);
#endif
}
