// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef H_UTILITY_QUADTREE_H
#define H_UTILITY_QUADTREE_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"

enum HQuadtreeInsertStatus {
	HQuadtreeInsertStatusUnknown,	
    HQuadtreeInsertStatusError,    // internal error
    HQuadtreeInsertStatusSuccess,  // rect was successfully inserted into the node
    HQuadtreeInsertStatusOverlap,  // overlaps with something that is already there
    HQuadtreeInsertStatusBadFit    // rect goes beyond the boundaries of the tree
};


class MVO_API HRectangle {

public:
    //default constructor
    HRectangle( float Left=-1.0f, float Bottom=-1.0f, 
            float Right=1.0f, float Top=1.0f, float Z=0.0f) {
        SetCoordinates(Left, Bottom, Right, Top, Z);
    };
    //copy constructor
    HRectangle( HRectangle *rect ) {
        SetCoordinates( rect->m_Left, rect->m_Bottom, rect->m_Right, rect->m_Top, rect->m_Z);
    };
    virtual ~HRectangle() {};

    float GetLeft() { return m_Left; };
    float GetRight() { return m_Right; };
    float GetTop() { return m_Top; };
    float GetBottom() { return m_Bottom; };
    float GetZ() { return m_Z; };

    void SetCoordinates( float Left, float Bottom, float Right, float Top , float Z=0.0f){
        if( Left > Right ) { float tmp=Left; Left=Right; Right=tmp; }
        if( Bottom > Top ) { float tmp=Bottom; Bottom=Top; Top=tmp; }
        m_Left = Left;   m_Right = Right;   m_Top = Top;   m_Bottom = Bottom; m_Z = Z;
    };

	bool IsRectInside( HRectangle *rect, HRectangle *area ) {
		if( (rect->m_Left < area->m_Left) ||
			(rect->m_Bottom < area->m_Bottom) ||
			(rect->m_Right > area->m_Right) ||
			(rect->m_Top > area->m_Top)) return false; else return true;
	};

	bool IsRectAbove( HRectangle *rect, float y ) { 
		if(rect->m_Bottom>y) return true; else return false;
	};

	bool IsRectBelow( HRectangle *rect, float y) {
		if(rect->m_Top<y) return true; else return false;
	};

	bool IsRectRight( HRectangle *rect, float x) {
		if(rect->m_Left>x) return true; else return false;
	};

	bool IsRectLeft( HRectangle *rect, float x) {
		if(rect->m_Right<x) return true; else return false;
	};

	bool RectOverlap(HRectangle *rect1, HRectangle *rect2) {
		if( rect1->m_Left > rect2->m_Right || rect1->m_Right < rect2->m_Left ||
			rect1->m_Top < rect2->m_Bottom || rect1->m_Bottom > rect2->m_Top) 
			return false; else return true;
	};

protected:

	float m_Right;
    float m_Left;
    float m_Top;
    float m_Bottom;
    float m_Z;

};

class MVO_API HQuadTreeItem : public HRectangle {

public:

	HQuadTreeItem(HQuadTreeItem const *item):
		HRectangle(item->m_Left, item->m_Bottom, item->m_Right, item->m_Top){
		m_pData=item->m_pData;
	}
	
    HQuadTreeItem(
			float Left=-1.0f, float Bottom=-1.0f,
			float Right=1.0f, float Top=1.0f,
			void * data=0)
		: HRectangle(Left, Bottom, Right, Top)
	{
		UNREFERENCED(data);
        m_pData = 0;
    }

	virtual ~HQuadTreeItem() {;}

private:
	void * m_pData;
};



class MVO_API HQuadTreeNode : public HRectangle {

public:

    HQuadTreeNode( float Left, float Bottom, float Right, float Top, HQuadTreeNode * Parent):
            HRectangle(Left, Bottom, Right, Top){
        m_pParent=Parent;
        m_pNE=m_pNW=m_pSE=m_pSW=0;
        m_pContents=0;
		m_CopiedItems=false;
    };

    virtual ~HQuadTreeNode();

protected:

    HQuadtreeInsertStatus Insert(HQuadTreeItem *item, int MaxDepth, bool AllowOverlap, bool CopyItem);

	bool OverlapInChildren(HQuadTreeItem *item);

    HQuadTreeNode * m_pParent;
    HQuadTreeNode * m_pNE;
    HQuadTreeNode * m_pNW;
    HQuadTreeNode * m_pSE;
    HQuadTreeNode * m_pSW;

    struct vlist_s * m_pContents;	

	bool m_CopiedItems;
}; 


class MVO_API HQuadTree:public HQuadTreeNode {

public:

    HQuadTree(
			float Left=-1.0f, float Bottom=-1.0,
			float Right=1.0f, float Top=1.0f, int MaxDepth=0, 
			bool AllowOverlap=true , bool CopyItems = false)
		: HQuadTreeNode( Left, Bottom, Right, Top, 0)
	{ 
        m_MaxDepth = MaxDepth;
		m_AllowOverlap = AllowOverlap;
		m_CopyItems = CopyItems;
    }

    ~HQuadTree() { 
    }

    HQuadtreeInsertStatus Insert(HQuadTreeItem *item) {
        return HQuadTreeNode::Insert(item, m_MaxDepth, m_AllowOverlap, m_CopyItems);
    }

private:

    unsigned int m_MaxDepth;
	bool m_AllowOverlap;
	bool m_CopyItems;

}; 



#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



