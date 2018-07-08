// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "HStream.h"
#include "HOpcodeShell.h"
#include "HSharedKey.h"

 

#define STARTING_TABLE_SIZE 10000


TK_Status TK_PIOpen_Segment::Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **newhandler) const 
{
    *newhandler = new TK_PIOpen_Segment(m_pPersIdent);
    if (*newhandler != 0)
		return TK_Normal;
    else
		return tk.Error();
}



TK_Status TK_PIShell::Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **newhandler) const 
{
    *newhandler = new TK_PIShell(m_pPersIdent);
    if (*newhandler != 0)
		return TK_Normal;
    else
		return tk.Error();
}

 
TK_Status TK_PIReferenced_Segment::Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **newhandler) const 
{
    *newhandler = new TK_PIReferenced_Segment(Opcode(), m_pPersIdent);
    if (*newhandler != 0)
		return TK_Normal;
    else
		return tk.Error();
}


TK_Status TK_PICircle::Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **newhandler) const 
{
    *newhandler = new TK_PICircle(Opcode(), m_pPersIdent);
    if (*newhandler != 0)
		return TK_Normal;
    else
		return tk.Error();
}


TK_Status TK_PIEllipse::Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **newhandler) const 
{
    *newhandler = new TK_PIEllipse(Opcode(), m_pPersIdent);
    if (*newhandler != 0)
		return TK_Normal;
    else
		return tk.Error();
}

 
TK_Status TK_PIGrid::Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **newhandler) const 
{
    *newhandler = new TK_PIGrid(m_pPersIdent);
    if (*newhandler != 0)
		return TK_Normal;
    else
		return tk.Error();
}


TK_Status TK_PIImage::Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **newhandler) const 
{
    *newhandler = new TK_PIImage(m_pPersIdent);
    if (*newhandler != 0)
		return TK_Normal;
    else
		return tk.Error();
}

 
TK_Status TK_PILine::Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **newhandler) const 
{
    *newhandler = new TK_PILine(m_pPersIdent);
    if (*newhandler != 0)
		return TK_Normal;
    else
		return tk.Error();
}

 
TK_Status TK_PIPolypoint::Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **newhandler) const 
{
    *newhandler = new TK_PIPolypoint(Opcode(), m_pPersIdent);
    if (*newhandler != 0)
		return TK_Normal;
    else
		return tk.Error();
}

TK_Status TK_PIPolyPolypoint::Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **newhandler) const 
{
    *newhandler = new TK_PIPolyPolypoint(Opcode(), m_pPersIdent);
    if (*newhandler != 0)
		return TK_Normal;
    else
		return tk.Error();
}



TK_Status TK_PIPoint::Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **newhandler) const 
{
    *newhandler = new TK_PIPoint(Opcode(), m_pPersIdent);
    if (*newhandler != 0)
		return TK_Normal;
    else
		return tk.Error();
}


TK_Status TK_PISpot_Light::Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **newhandler) const 
{
    *newhandler = new TK_PISpot_Light(m_pPersIdent);
    if (*newhandler != 0)
		return TK_Normal;
    else
		return tk.Error();
}

 
TK_Status TK_PIArea_Light::Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **newhandler) const 
{
    *newhandler = new TK_PIArea_Light(m_pPersIdent);
    if (*newhandler != 0)
		return TK_Normal;
    else
		return tk.Error();
}

 
TK_Status TK_PIMesh::Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **newhandler) const 
{
    *newhandler = new TK_PIMesh(m_pPersIdent);
    if (*newhandler != 0)
		return TK_Normal;
    else
		return tk.Error();
}

 
TK_Status TK_PINURBS_Curve::Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **newhandler) const 
{
    *newhandler = new TK_PINURBS_Curve(m_pPersIdent);
    if (*newhandler != 0)
		return TK_Normal;
    else
		return tk.Error();
}


 
TK_Status TK_PICutting_Plane::Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **newhandler) const 
{
    *newhandler = new TK_PICutting_Plane(m_pPersIdent);
    if (*newhandler != 0)
		return TK_Normal;
    else
		return tk.Error();
}

TK_Status TK_PIText::Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **newhandler) const 
{
    *newhandler = new TK_PIText(Opcode(), m_pPersIdent);
    if (*newhandler != 0)
		return TK_Normal;
    else
		return tk.Error();
}



HSharedKey::HSharedKey()
{
 	m_pKeyIdentHashTable = 0;
	m_pIdentKeyHashTable = 0;
}

void HSharedKey::Reset()
{
	m_PersCounter = 1;
	
	if (m_pKeyIdentHashTable)
		delete_vhash(m_pKeyIdentHashTable);
	if (m_pIdentKeyHashTable)
		delete_vhash(m_pIdentKeyHashTable);
	m_pKeyIdentHashTable = new_vhash(STARTING_TABLE_SIZE, malloc, free);
	m_pIdentKeyHashTable = new_vhash(STARTING_TABLE_SIZE, malloc, free);
}

long HSharedKey::AssociateKey(HC_KEY key)
{
	if (!vhash_lookup(m_pKeyIdentHashTable, (void *) key))
	{
		vhash_insert(m_pKeyIdentHashTable, (void *) key, (void *)(MVO_POINTER_SIZED_INT)m_PersCounter);
		vhash_insert(m_pIdentKeyHashTable, (void *)(MVO_POINTER_SIZED_INT) m_PersCounter, (void *)key);
		m_PersCounter++;
		return m_PersCounter - 1;
	}
	else
		return 0;
}

bool HSharedKey::DisAssociateKey(HC_KEY key)
{
	long ident;
	if ((ident = (long) (MVO_POINTER_SIZED_INT) vhash_lookup(m_pKeyIdentHashTable, (void *)key)) != 0)
	{
		vhash_remove(m_pKeyIdentHashTable,(void *) key);
		vhash_remove(m_pIdentKeyHashTable,(void *) (MVO_POINTER_SIZED_INT)ident);
		return true;
	}
	else
		return false;
}


void HSharedKey::AssociateKeysInSegment(const char *seg)
{
	char type[MVO_BUFFER_SIZE];
	HC_KEY key;

	if (!seg)
	{
		AssociateKey(HC_KOpen_Segment("."));
		HC_Close_Segment();
		HC_Begin_Contents_Search("./...","segment, include, geometry");
	}
	else
		HC_Begin_Contents_Search(seg,"segment, include, geometry");
	while (HC_Find_Contents(type,&key))
	{
		AssociateKey(key);
	}
	HC_End_Contents_Search();
}

void HSharedKey::DisAssociateKeysInSegment(const char *seg)
{
	char type[MVO_BUFFER_SIZE];
	HC_KEY key;
	if (!seg)
	{
		DisAssociateKey(HC_KOpen_Segment("."));
		HC_Close_Segment();
		HC_Begin_Contents_Search("./...","segment, include, geometry");
	}
	else
		HC_Begin_Contents_Search(seg,"segment, include, geometry");
	while (HC_Find_Contents(type,&key))
	{
		DisAssociateKey(key);
	}
	HC_End_Contents_Search();
}




HC_KEY HSharedKey::GetKeyFromIdent(long ident)
{
	return reinterpret_cast<HC_KEY>(
		vhash_lookup(
			m_pIdentKeyHashTable, 
			reinterpret_cast<void *>(static_cast<MVO_POINTER_SIZED_INT>(ident)))
	);
}

long HSharedKey::GetIdentFromKey(HC_KEY key)
{
	return static_cast<long>(reinterpret_cast<MVO_POINTER_SIZED_INT>(
		vhash_lookup(
			m_pKeyIdentHashTable,
			reinterpret_cast<void *>(key)
		)
	));
}

 
void HSharedKey::SetupStreamToolkit(HStreamFileToolkit *mytool)
{
    mytool->SetOpcodeHandler (TKE_Shell,                new  TK_PIShell(this));
    mytool->SetOpcodeHandler (TKE_Open_Segment,         new  TK_PIOpen_Segment(this));                               
    mytool->SetOpcodeHandler (TKE_Include_Segment,      new  TK_PIReferenced_Segment(TKE_Include_Segment,this));
    mytool->SetOpcodeHandler (TKE_Style_Segment,        new  TK_PIReferenced_Segment(TKE_Style_Segment,this));
    mytool->SetOpcodeHandler (TKE_Circle,               new  TK_PICircle(TKE_Circle,this));
    mytool->SetOpcodeHandler (TKE_Circular_Arc,         new  TK_PICircle(TKE_Circular_Arc,this));
    mytool->SetOpcodeHandler (TKE_Circular_Chord,       new  TK_PICircle(TKE_Circular_Chord,this));
    mytool->SetOpcodeHandler (TKE_Circular_Wedge,       new  TK_PICircle(TKE_Circular_Wedge,this));
    mytool->SetOpcodeHandler (TKE_Ellipse,              new  TK_PIEllipse(TKE_Ellipse,this));
    mytool->SetOpcodeHandler (TKE_Elliptical_Arc,       new  TK_PIEllipse(TKE_Elliptical_Arc,this));
    mytool->SetOpcodeHandler (TKE_Grid,                 new  TK_PIGrid(this));
    mytool->SetOpcodeHandler (TKE_Image,                new  TK_PIImage(this));
    mytool->SetOpcodeHandler (TKE_Line,                 new  TK_PILine(this));
    mytool->SetOpcodeHandler (TKE_Polyline,             new  TK_PIPolypoint(TKE_Polyline,this));
    mytool->SetOpcodeHandler (TKE_PolyPolyline,         new  TK_PIPolyPolypoint(TKE_PolyPolyline,this));
    mytool->SetOpcodeHandler (TKE_Polygon,              new  TK_PIPolypoint(TKE_Polygon,this));
    mytool->SetOpcodeHandler (TKE_Marker,               new  TK_PIPoint(TKE_Marker,this));
    mytool->SetOpcodeHandler (TKE_Distant_Light,        new  TK_PIPoint(TKE_Distant_Light,this));
    mytool->SetOpcodeHandler (TKE_Local_Light,          new  TK_PIPoint(TKE_Local_Light,this));
    mytool->SetOpcodeHandler (TKE_Spot_Light,           new  TK_PISpot_Light(this));
    mytool->SetOpcodeHandler (TKE_Area_Light,           new  TK_PIArea_Light(this));
    mytool->SetOpcodeHandler (TKE_Mesh,                 new  TK_PIMesh(this));
    mytool->SetOpcodeHandler (TKE_NURBS_Curve,          new  TK_PINURBS_Curve(this));
    mytool->SetOpcodeHandler (TKE_Cutting_Plane,        new  TK_PICutting_Plane(this));
    mytool->SetOpcodeHandler (TKE_Text,                 new  TK_PIText(TKE_Text,this));
    mytool->SetOpcodeHandler (TKE_Text_With_Encoding,   new  TK_PIText(TKE_Text_With_Encoding,this));
}



