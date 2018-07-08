
#ifndef _MyFreehandMarkupOp_H
#define _MyFreehandMarkupOp_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"


class MVO_API MyFreehandMarkupOp : public HBaseOperator
{
public:
  	MyFreehandMarkupOp(HBaseView* view, int DoRepeat=0, int DoCapture=1);

	static void CreateMarkupFreehand(HBaseView *pView, const char *segment, unsigned int PolyLineLength, HPoint *pPolyline, bool emit_message);

	HBaseOperator * Clone();
	virtual const char * GetName();  
	
protected:

	virtual int OnLButtonDownAndMove(HEventInfo &hevent);
	virtual int OnLButtonDown(HEventInfo &hevent);
	virtual int OnLButtonUp(HEventInfo &hevent);

	virtual void MarkupSelection(HEventInfo &hevent);

	HC_KEY m_LastKey;
	static long markup_incarnation;
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



