// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

/*
 * implements a quad tree that can determine quickly if something 
 * about to be inserted overlaps with something that is already there.
*/

#include "HUtilityQuadTree.h"
#include "vlist.h"

HQuadTreeNode::~HQuadTreeNode() 
{
	if(m_pNE) delete m_pNE;
	if(m_pNW) delete m_pNW;
	if(m_pSE) delete m_pSE;
	if(m_pSW) delete m_pSW;

	if(m_pContents) {

		if(m_CopiedItems) {

			HQuadTreeItem * item;
			while((item = (HQuadTreeItem *) vlist_remove_first(m_pContents)) != 0)
				delete item;
		}

		delete_vlist(m_pContents);
	}
};


HQuadtreeInsertStatus HQuadTreeNode::Insert(HQuadTreeItem *item, int MaxDepth, bool AllowOverlap, bool CopyItem)
{

    if(!item) return HQuadtreeInsertStatusError;

	//IF NOT AT MAX DEPTH TRY TO DECEND TREE
    if( MaxDepth > 0 ){
        float tb_split = (m_Bottom+m_Top) / 2.0f;
        float rl_split = (m_Left+m_Right) / 2.0f;

        if(IsRectAbove(item,tb_split)) {
            if(IsRectRight(item, rl_split)) {
                if(!m_pNE) m_pNE = new HQuadTreeNode(rl_split, tb_split, m_Right, m_Top, this);
                return m_pNE->Insert(item, MaxDepth-1, AllowOverlap, CopyItem);
            }
            else if(IsRectLeft(item, rl_split)) {
                if(!m_pNW) m_pNW = new HQuadTreeNode(m_Left, tb_split, rl_split, m_Top, this);
                return m_pNW->Insert(item, MaxDepth-1, AllowOverlap, CopyItem);
            }
        }
        else if(IsRectBelow(item,tb_split)) {
            if(IsRectRight(item, rl_split)) {
                if(!m_pSE) m_pSE = new HQuadTreeNode(rl_split, m_Bottom, m_Right, tb_split, this);
                return m_pSE->Insert(item, MaxDepth-1, AllowOverlap, CopyItem);
            }
            else if(IsRectLeft(item, rl_split)) {
                if(!m_pSW) m_pSW = new HQuadTreeNode(m_Left, m_Bottom, rl_split, tb_split, this);
                return m_pSW->Insert(item, MaxDepth-1, AllowOverlap, CopyItem);
            }
        }
    }

	//DOES THIS RECT FIT HERE
	if(!IsRectInside(item,this)) return HQuadtreeInsertStatusBadFit;

	if(!AllowOverlap && m_pContents) {

		//WE MUST TEST AGAINST OVERLAP WITH ALL EXISTING CONTENTS
		HQuadTreeItem * test_item;

		vlist_reset_cursor(m_pContents);

		while((test_item = (HQuadTreeItem *) vlist_peek_cursor(m_pContents)) != 0) {

			if(RectOverlap(test_item, item)) return HQuadtreeInsertStatusOverlap;

			vlist_advance_cursor(m_pContents);
		}

		//NOW WE MUST CHECK THE CHILDREN FOR OVERLAPS
		if(OverlapInChildren(item)) return HQuadtreeInsertStatusOverlap;
	}

	if(!m_pContents) 
		m_pContents = new_vlist(malloc,free);
	
	if(CopyItem){
		m_CopiedItems=true;
		vlist_add_first(m_pContents, (void*) new HQuadTreeItem(item));
	}else
		vlist_add_first(m_pContents, (void*) item);

	return HQuadtreeInsertStatusSuccess;
}


bool HQuadTreeNode::OverlapInChildren(HQuadTreeItem *item)
{
	float tb_split = (m_Bottom+m_Top) / 2.0f;
	float rl_split = (m_Left+m_Right) / 2.0f;

	if((m_pSW || m_pSE) && !IsRectAbove(item, tb_split)) {

		if(m_pSW && !IsRectRight(item, rl_split))
			if(m_pSW->OverlapInChildren(item)) return true;

		if(m_pSE && !IsRectLeft(item, rl_split)) 
			if(m_pSE->OverlapInChildren(item)) return true;
	}

	if((m_pNW || m_pNE) && !IsRectBelow(item, tb_split)) {

		if(m_pNW && !IsRectRight(item, rl_split)) 
			if(m_pNW->OverlapInChildren(item)) return true;

		if(m_pNE && !IsRectLeft(item, rl_split)) 
			if(m_pNE->OverlapInChildren(item)) return true;
	}

	if(m_pContents) {

		//WE MUST TEST AGAINST OVERLAP WITH ALL EXISTING CONTENTS
		HQuadTreeItem * test_item;

		vlist_reset_cursor(m_pContents);

		while((test_item = (HQuadTreeItem *) vlist_peek_cursor(m_pContents)) != 0) {

			if(RectOverlap(test_item, item)) return true;

			vlist_advance_cursor(m_pContents);
		}
	}

	return false;
} 



