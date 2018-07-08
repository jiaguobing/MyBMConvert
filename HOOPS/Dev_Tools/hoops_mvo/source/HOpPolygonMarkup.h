
#ifndef _MyPolygonMarkupOp_H
#define _MyPolygonMarkupOp_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"


class MVO_API MyPolygonMarkupOp : public HBaseOperator
{
public:
  	MyPolygonMarkupOp(HBaseView* view, int DoRepeat=0, int DoCapture=1);

	static void CreateMarkupPolygon(HBaseView *pView, const char *segment, unsigned int PolyLineLength, HPoint *pPolyline, bool emit_message);

	HBaseOperator * Clone();
	virtual const char * GetName();  
	
protected:

	virtual int OnLButtonDownAndMove(HEventInfo &hevent);
	virtual int OnLButtonDown(HEventInfo &hevent);
	virtual int OnLButtonUp(HEventInfo &hevent);

	virtual void MarkupSelection(HEventInfo &hevent);
	virtual void CompleteLastMarkup();
	virtual void BeginNewMarkup();
	
	HC_KEY m_LastKey;
	char markup_seg[4096];
	static long markup_incarnation;
};



#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



