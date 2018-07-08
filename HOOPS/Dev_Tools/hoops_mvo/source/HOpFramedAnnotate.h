

#ifndef _HOPGRAMEDANNOTATE_H
#define _HOPGRAMEDANNOTATE_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"
#include "HEventListener.h"


class MVO_API HOpFramedAnnotate : public HBaseOperator
{
public:

     HOpFramedAnnotate (HBaseView* view, bool bAddToLayer = false, int DoRepeat=0, int DoCapture=1);
    ~HOpFramedAnnotate();

	virtual const char * GetName();  
    int OnLButtonDown (HEventInfo &hevent);
	int OnLButtonDownAndMove(HEventInfo &hevent);
    int OnLButtonUp (HEventInfo &hevent);
	int OnKeyDown(HEventInfo &hevent);
	HBaseOperator * Clone();  /*! returns a pointer to a copy of the operator */

protected:
 	unsigned short	*m_pString;		/*!< pointer to annotation text  */

 	HC_KEY m_pNoteKey;		/*!< selected annotation key */

	void finalize_text(bool emit_message=false);   
	bool m_bAddToLayer;		/*!< allow annotation to be added to markup layers */

	void TransmitMarkupSegment();


};



#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



