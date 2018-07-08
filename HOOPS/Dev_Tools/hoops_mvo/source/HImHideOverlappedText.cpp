// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

#include "HImManager.h"
#include "HImHideOverlappedText.h"
#include "HUtilityAnnotation.h"
#include "HUtility.h"
#include "HUtilityQuadTree.h"
#include "vlist.h"

#include "hc.h" 
#include "hic.h"


#define DEBUG_HImHideOverlappedText 0

bool HImHideOverlappedText::m_bFilterOn = false;

void HImHideOverlappedText::RegisterCallbacks()
{
	HImRegisterCallback("HImHideOverlappedText_defer_text", defer_text);
	HImRegisterCallback("HIMHideOverlappedText_defer_annotation", defer_annotation);
	HImRegisterCallback("HImHideOverlappedText_update", update);
}

void HImHideOverlappedText::FilterOn()
{
	m_bFilterOn = true;
	HImSetCallback("draw text","HImHideOverlappedText_defer_text");
	HImSetCallback("draw segment","HIMHideOverlappedText_defer_annotation");
	HImSetCallback("draw segment tree","HImHideOverlappedText_update");
}

bool HImHideOverlappedText::IsFilterOn()
{
	return m_bFilterOn;
}

void HImHideOverlappedText::FilterOff()
{
	m_bFilterOn = false;
	HImUnSetOneCallback("draw text");
	HImUnSetOneCallback("draw segment");
	HImUnSetOneCallback("draw segment tree");
}


struct vlist_s * HImHideOverlappedText::text_list = 0;
struct vlist_s * HImHideOverlappedText::annotation_list = 0;

struct text_node_s {
	HIC_Text_Info const *text_info;
	HIC_Karacter *karacter;
	HIC_Rendition const *nr;
};

struct segment_node_s {
	HIC_Segment_Info const *segment_info;
	HIC_Rendition const *nr;
};

/* Three quick macros to help with sorting */
//#define ZVAL(x) ((*(x))->text_info->position.z)
#define ZVAL(x) (HIC_Show_Text_Position(((x))->text_info))->z

#if 0
#define MEDIANOF3INDEX(a,b,c) (((a)>(b))?(((b)>(c))?(1):(((a)>(c))?(2):(0))):(((a)>(c))?(0):((b)>(c))?(2):(1)))
#define SWAP_TEXT(a, b) {register struct text_node_s *tmp = (a); (a) = (b); (b) = tmp;}

void HImHideOverlappedText::zsort (
	struct text_node_s **ileft,
	struct text_node_s **iright ) 
{

	if( iright - ileft == 1 )
	{
		if( ZVAL(ileft) > ZVAL(iright) )
		   SWAP_TEXT( *ileft, *iright );
	}
	else if( ileft < iright) {
		struct text_node_s **left, **right;
		struct text_node_s **candidates[3];
		float	pivot;
		int 	m3i;

		candidates[0] = left = ileft;						/* left */
		candidates[1] = ileft + ( (iright - ileft) / 2 );	/* middle */
		candidates[2] = right = iright; 					/* right */

		/* find the best pivot and move it to the far right */
		m3i = MEDIANOF3INDEX(
			ZVAL(candidates[0]), 
			ZVAL(candidates[1]),
			ZVAL(candidates[2]) 
		);
		pivot = ZVAL(candidates[m3i]);
		SWAP_TEXT( *candidates[m3i], *right );

		/* should be one higher and one lower */

		for (;;) {
			left--;
			while( ZVAL(++left) <= pivot ) {
				if( left >= right ) 
				   goto partition_done;
			}
			SWAP_TEXT( *left, *right );

			right++;
			while( ZVAL(--right) >= pivot) {
				if( left >= right ) 
				   goto partition_done;
			}
			SWAP_TEXT( *left, *right );
		}
		partition_done:

		zsort(ileft, left - 1 );
		zsort(left + 1,  iright );
	}

} 
#endif

static bool sort_pred(text_node_s const * a, text_node_s const * b) {
	return ZVAL(a) < ZVAL(b);
}

void HImHideOverlappedText::draw_text( HIC_Rendition const *nr )
{
	unsigned long n_nodes = vlist_count(text_list);	

	if(n_nodes == 0)
		return;
	
	std::vector<text_node_s *> nodes(n_nodes);

	for(unsigned long i=0;i<n_nodes;i++) 
		nodes[i] = (struct text_node_s *)vlist_nth_item(text_list,i);

	std::sort(nodes.begin(), nodes.end(), sort_pred);

#if DEBUG_HImHideOverlappedText
	//To test zsort
	for(unsigned long i=0;i<n_nodes-1;i++)
		if(nodes[i]->text_info->position.z > nodes[i+1]->text_info->position.z) 
			{printf("HImHideOverlappedText::zsort is broken");}
#endif
			
	HIC_Int_Rectangle const * extent = HIC_Show_Window_Extent(nr);
	
	HQuadTree * quad_tree = new HQuadTree( 
		static_cast<float>(extent->left),
		static_cast<float>(extent->bottom),
		static_cast<float>(extent->right),
		static_cast<float>(extent->top),
		0, false, true);

	for(unsigned long i=0;i<n_nodes;i++) {

		//float w = nodes[i]->text_info->baseline.x;
		float w = (HIC_Show_Text_Baseline(nodes[i]->text_info))->x;

		//float h = nodes[i]->nr->text_rendition->font->Aheight;
		float h = HIC_Show_Text_Final_Font_Size(nodes[i]->nr);

		//float x = nodes[i]->text_info->position.x;
		float x = (HIC_Show_Text_Position(nodes[i]->text_info))->x;

		//float y = nodes[i]->text_info->position.y;
		float y = (HIC_Show_Text_Position(nodes[i]->text_info))->y;
		
		HQuadTreeItem item(x,y,x+w,y+h);

		if(quad_tree->Insert(&item) == HQuadtreeInsertStatusSuccess)
			HIC_Draw_Text(nodes[i]->nr, nodes[i]->text_info);

#if DEBUG_HImHideOverlappedText
		float z = nodes[i]->text_info->position.z;
		HT_DC_Point p[5];
		p[0].x = x; p[0].y = y;
		p[1].x = x + w; p[1].y = y;
		p[2].x = x + w; p[2].y = y + h;
		p[3].x = x; p[3].y = y + h;
		p[4].x = x; p[4].y = y;
		p[0].z = p[1].z = p[2].z = p[3].z = p[4].z = z;
		HIC_Draw_DC_Polyline(nr, 5, p);
#endif
	}	

	delete quad_tree;
} 


void HImHideOverlappedText::update( HIC_Rendition const *nr, HIC_Segment_Info const *seg )
{
	if(text_list || annotation_list) {
		//Reentry problem
		HIC_Draw_Segment( nr, seg );
		return;
	}

	text_list = new_vlist(malloc,free);
	annotation_list = new_vlist(malloc,free);

	HIC_Draw_Segment( nr, seg );

	draw_text(nr);

	struct text_node_s * node;
	while((node = (struct text_node_s *) vlist_remove_first(text_list)) != 0) {
		free(node->karacter);
		HIC_Free_Rendition(node->nr);
		HIC_Free_Text_Info(node->text_info);
		free(node);
	}
	delete_vlist(text_list);
	text_list = 0;

	/* Draw the deferred annotation segments */
	struct segment_node_s * annotation;
	while((annotation = (struct segment_node_s*) vlist_remove_first(annotation_list)) != 0) {
		HIC_Draw_Segment(annotation->nr, annotation->segment_info);
		HIC_Free_Rendition(annotation->nr);
		HIC_Free_Segment_Info(annotation->segment_info);
		free(annotation);
	}
	delete_vlist(annotation_list);
	annotation_list = 0;
} 


void HImHideOverlappedText::defer_text(HIC_Rendition const *nr, HIC_Text_Info const *text_info)
{
	/*	If there is no text list we're drawing the deferred annotation
		segments. */
	if(!text_list) {
		HIC_Draw_Text( nr, text_info);
		return;
	}

	HIC_Karacter const * string = HIC_Show_Text_String(text_info);
	int length = HIC_Show_Text_Length(text_info);

	if(!length) return;

	struct text_node_s * node = (struct text_node_s *) malloc(sizeof(struct text_node_s));

	// save the pointer and increase the reference count
	node->nr = HIC_New_Rendition(nr); 

	node->text_info = HIC_New_Text_Info(text_info);

	node->karacter = (HIC_Karacter*) malloc(length*sizeof(HIC_Karacter));
	memcpy(node->karacter, string, length*sizeof(HIC_Karacter));

	vlist_add_first(text_list, (void*) node);
} 

void HImHideOverlappedText::defer_annotation(HIC_Rendition const* nr, HIC_Segment_Info const* si)
{
	HC_KEY key = HIC_Show_Segment_Key(si);

	/*	If there is no text list then the overlapped text callback is just
		going to return the text.  If that is the case then we're in the
		phase where we're drawing the deferred annotation list. */
	HIC_Unsafe_Threading(nr, true);
	if(text_list && HUtilityAnnotation::IsAnnotation(key)){
		/*	If this IS an annotation key, do something interesting */
		struct segment_node_s * annotation = (struct segment_node_s *) malloc(sizeof(struct segment_node_s));
		annotation->nr = HIC_New_Rendition(nr);
		//annotation->key = key; 
		annotation->segment_info = HIC_Copy_Segment_Info(si);

		vlist_add_first(annotation_list, annotation);
	} else {
		/*	If this is not an annotation key, just draw it. */
		HIC_Draw_Segment(nr, si);
	}
}
