// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HSharedKey_H
#define _HSharedKey_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"

#include "vhash.h"
#include "HStream.h"
#include "HOpcodeShell.h"
#include "HPolyPoly.h"


#define OVERLOAD_INTERPRET_INT(base_class__name__abc)								\
	TK_Status   Interpret (BStreamFileToolkit & tk, HC_KEY key, int variant = 0)	\
	{																				\
		TK_Status status = base_class__name__abc::Interpret(tk,key,variant);		\
		m_key = key;																\
		return status;																\
	}
#define OVERLOAD_INTERPRET_CHAR(base_class__name__abc)								\
	TK_Status   Interpret (BStreamFileToolkit & tk, HC_KEY key, char const * name)	\
	{																				\
		TK_Status status = base_class__name__abc::Interpret(tk,key,name);			\
		m_key = key;																\
		return status;																\
	}

#define OVERLOAD_WRITE(base_class__name__abc)										\
	TK_Status   Write(BStreamFileToolkit & tk)      								\
	{																				\
		TK_Status status = base_class__name__abc::Write(tk);						\
		m_pPersIdent->AssociateKey(m_key);											\
		return status;																\
	}

#define OVERLOAD_EXECUTE(base_class___name__abc)								\
	TK_Status Execute (BStreamFileToolkit &tk)									\
	{																			\
		TK_Status status = base_class___name__abc::Execute(tk);					\
		m_pPersIdent->AssociateKey(last_key (tk));								\
		return status;															\
	}

#define OVERLOAD_WRITE_POLY(base_class__name__abc)									\
	TK_Status   Write(BStreamFileToolkit & tk)      								\
	{																				\
		TK_Status status = base_class__name__abc::Write(tk);						\
		for (int i = 0; i < m_primitive_count; i++)									\
			m_pPersIdent->AssociateKey(m_keys[i]);									\
		return status;																\
	}

#define OVERLOAD_EXECUTE_POLY(base_class___name__abc)							\
	TK_Status Execute (BStreamFileToolkit & tk)									\
	{																			\
		TK_Status status = base_class___name__abc::Execute(tk);					\
		for (int i = 0; i < m_primitive_count; i++)								\
			m_pPersIdent->AssociateKey(m_keys[i]);								\
		return status;															\
	}

/*! 
	SharedKey manages the mapping of HOOPS/3dGS objects to unique and persistent identifiers using a hash table. 
	It has essentially the same functionality as #HC_Renumber_Key but it does 
	this externally to HOOPS/3dGS.
*/
class MVO_API HSharedKey 
{
public:
	/*! Constructs an HSharedKeyObject.*/
	HSharedKey();
	~HSharedKey()
	{
		delete_vhash(m_pKeyIdentHashTable);
		delete_vhash(m_pIdentKeyHashTable);
	}
		
	/*!  This method assigns a given key a new persistent identifier.
		\param key The key of the entity to be given a persistent identifier.
		\return A the new persistent identifier or 0 if the key is already associated with an identifier.
	*/
	long AssociateKey(HC_KEY key);

	/*!  This method removes a key from the persistent identifier list.
		\param key The key of the entity to be removed.
		\return True if the key was successful removed from the list.
	*/
	bool DisAssociateKey(HC_KEY key);

	/*!  This method assigns a persistent identifier to the keys of all the entities in a given segment.
		\param seg Pass the name of the segment you want to begin the assignment with.  Pass 0 to associate the currently open key.  
	*/
	void AssociateKeysInSegment(const char *seg = 0);

	/*!  This metho removes the keys in a given segment from the persistent identifier list.
		\param seg Pass the name of the segment you want to begin the assignment with.  Pass 0 to associate the currently open key.  
	*/
	void DisAssociateKeysInSegment(const char *seg = 0);

	/*!  Given a persistent identifier, this method returns the HOOPS key associated with it.
	\param ident The persistent identifier to look up.
	\return The key associated with the given identifier. 
	*/
	HC_KEY GetKeyFromIdent(long ident);

	/*! Given a key, this method returns the persistent identifier associated with it.
	\param ident The key to look up.
	\return The indentifier associated with the given key.
	*/
	long GetIdentFromKey(HC_KEY ident);

	/*!  This method initializes custom stream toolkit opcode handlers to utilize shared keys.
	\param tk A pointer to the HStreamFileToolkit.
	*/
	void SetupStreamToolkit(HStreamFileToolkit *tk);
 
	/*!  This method reinitializes the HSharedKey object by resetting the counter and deleting all the elements in both
	hash tables.*/
	void Reset();
		
private:
	unsigned int m_PersCounter;
	vhash_t *m_pKeyIdentHashTable;
	vhash_t *m_pIdentKeyHashTable;
};


/*! A custom HTK_Shell class which includes the HOOPS/3dgs key and pointer to an HSharedKey persistent indentifier object. */
class TK_PIShell : public HTK_Shell {
public:
	/*! Constructs a TK_PIShell object.
	\param persIdent A pointer to the shared key that you want to associate with this object.
	*/
	TK_PIShell(HSharedKey *persIdent) : HTK_Shell() 
	{
		m_pPersIdent = persIdent;
	}
	OVERLOAD_INTERPRET_INT(HTK_Shell)
	OVERLOAD_WRITE(HTK_Shell)
	OVERLOAD_EXECUTE(HTK_Shell)

	/*!  This method makes a copy of itself and return a pointer to the newly created object.
	\param tk A reference to the BStreamFileToolkit object. 
	\param handler The method will return a pointer to a copy of itself.
	\return A #TK_Status indicating whether the operation was successful. 
	*/
	virtual TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

private:
	HSharedKey *m_pPersIdent; 
	HC_KEY m_key;
};

/*! A custom HTK_Open_Segment class which includes the HOOPS/3dgs key and pointer to an HSharedKey persistent indentifier object */
class TK_PIOpen_Segment : public HTK_Open_Segment {
public:
	/*! Constructs a TK_PIOpen_Segment object.
	\param persIdent A pointer to the shared key that you want to associate with this object.
	*/
	TK_PIOpen_Segment(HSharedKey *persIdent) : HTK_Open_Segment() 
	{
		m_pPersIdent = persIdent;
	}
	
	OVERLOAD_INTERPRET_CHAR(HTK_Open_Segment)
	OVERLOAD_WRITE(HTK_Open_Segment)
	OVERLOAD_EXECUTE(HTK_Open_Segment)

	/*!  This method makes a copy of itself and return a pointer to the newly created object.
	\param tk A reference to the BStreamFileToolkit object. 
	\param handler The method will return a pointer to a copy of itself.
	\return A #TK_Status indicating whether the operation was successful. 
	*/
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

private:
	HSharedKey *m_pPersIdent; 
	HC_KEY m_key;
		
};

/*! A custom HTK_Circle class which includes the HOOPS/3dgs key and pointer to an HSharedKey persistent indentifier object */
class TK_PICircle : public HTK_Circle {
public:
	/*! Constructs a TK_PICircle object.
	\param persIdent A pointer to the shared key that you want to associate with this object.
	*/
	TK_PICircle(unsigned char opcode, HSharedKey *persIdent) : HTK_Circle(opcode) 
	{
		m_pPersIdent = persIdent;
	}
	OVERLOAD_INTERPRET_INT(HTK_Circle)
	OVERLOAD_WRITE(HTK_Circle)
	OVERLOAD_EXECUTE(HTK_Circle)

	/*!  This method makes a copy of itself and return a pointer to the newly created object.
	\param tk A reference to the BStreamFileToolkit object. 
	\param handler The method will return a pointer to a copy of itself.
	\return A #TK_Status indicating whether the operation was successful. 
	*/
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

private:
	HSharedKey *m_pPersIdent; 
	HC_KEY m_key;
	
};

/*! A custom HTK_Ellipse class which includes the HOOPS/3dgs key and pointer to an HSharedKey persistent indentifier object */
class TK_PIEllipse : public HTK_Ellipse {
public:
	/*! Constructs a HTK_Ellipse object.
	\param persIdent A pointer to the shared key that you want to associate with this object.
	*/
	TK_PIEllipse(unsigned char opcode, HSharedKey *persIdent) : HTK_Ellipse(opcode) 
	{
		m_pPersIdent = persIdent;
	}
	OVERLOAD_INTERPRET_INT(HTK_Ellipse)
	OVERLOAD_WRITE(HTK_Ellipse)
	OVERLOAD_EXECUTE(HTK_Ellipse)

	/*!  This method makes a copy of itself and return a pointer to the newly created object.
	\param tk A reference to the BStreamFileToolkit object. 
	\param handler The method will return a pointer to a copy of itself.
	\return A #TK_Status indicating whether the operation was successful. 
	*/
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

private:

	HSharedKey *m_pPersIdent; 
	HC_KEY m_key;

	
};

/*! A custom HTK_Grid class which includes the HOOPS/3dgs key and pointer to an HSharedKey persistent indentifier object */
class TK_PIGrid : public HTK_Grid {
public:
	/*! Constructs a TK_PIGrid object.
	\param persIdent A pointer to the shared key that you want to associate with this object.
	*/
	TK_PIGrid(HSharedKey *persIdent) : HTK_Grid() 
	{
		m_pPersIdent = persIdent;
	}
	OVERLOAD_INTERPRET_INT(HTK_Grid)
	OVERLOAD_WRITE(HTK_Grid)
	OVERLOAD_EXECUTE(HTK_Grid)

	/*!  This method makes a copy of itself and return a pointer to the newly created object.
	\param tk A reference to the BStreamFileToolkit object. 
	\param handler The method will return a pointer to a copy of itself.
	\return A #TK_Status indicating whether the operation was successful. 
	*/
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

private:
	HSharedKey *m_pPersIdent; 
	HC_KEY m_key;
	
};

/*! A custom HTK_Image class which includes the HOOPS/3dgs key and pointer to an HSharedKey persistent indentifier object */
class TK_PIImage : public HTK_Image {
public:
	/*! Constructs a TK_PIImage object.
	\param persIdent A pointer to the shared key that you want to associate with this object.
	*/
	TK_PIImage(HSharedKey *persIdent) : HTK_Image() 
	{
		m_pPersIdent = persIdent;
	}
	OVERLOAD_INTERPRET_INT(HTK_Image)
	OVERLOAD_WRITE(HTK_Image)
	OVERLOAD_EXECUTE(HTK_Image)

	/*!  This method makes a copy of itself and return a pointer to the newly created object.
	\param tk A reference to the BStreamFileToolkit object. 
	\param handler The method will return a pointer to a copy of itself.
	\return A #TK_Status indicating whether the operation was successful. 
	*/
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **) const;

private:
		HSharedKey *m_pPersIdent; 
	HC_KEY m_key;

	
};

/*! A custom HTK_Line class which includes the HOOPS/3dgs key and pointer to an HSharedKey persistent indentifier object */
class TK_PILine : public HTK_Line {
public:
	/*! Constructs a TK_PILine object.
	\param persIdent A pointer to the shared key that you want to associate with this object.
	*/
	TK_PILine(HSharedKey *persIdent) : HTK_Line() 
	{
		m_pPersIdent = persIdent;
	}
	OVERLOAD_INTERPRET_INT(HTK_Line)
	OVERLOAD_WRITE(HTK_Line)
	OVERLOAD_EXECUTE(HTK_Line)

	/*!  This method makes a copy of itself and return a pointer to the newly created object.
	\param tk A reference to the BStreamFileToolkit object. 
	\param handler The method will return a pointer to a copy of itself.
	\return A #TK_Status indicating whether the operation was successful. 
	*/
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

private:
		HSharedKey *m_pPersIdent; 
	HC_KEY m_key;

	
};

/*! A custom HTK_Polypoint class which includes the HOOPS/3dgs key and pointer to an HSharedKey persistent indentifier object */
class TK_PIPolypoint : public HTK_Polypoint {
public:
	/*! Constructs a TK_PIPolypoint object.
	\param persIdent A pointer to the shared key that you want to associate with this object.
	*/
	TK_PIPolypoint(unsigned char opcode, HSharedKey *persIdent) : HTK_Polypoint(opcode) 
	{
		m_pPersIdent = persIdent;
	}
	OVERLOAD_INTERPRET_INT(HTK_Polypoint)
	OVERLOAD_WRITE(HTK_Polypoint)
	OVERLOAD_EXECUTE(HTK_Polypoint)

	/*!  This method makes a copy of itself and return a pointer to the newly created object.
	\param tk A reference to the BStreamFileToolkit object. 
	\param handler The method will return a pointer to a copy of itself.
	\return A #TK_Status indicating whether the operation was successful. 
	*/
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

private:
		HSharedKey *m_pPersIdent; 
	HC_KEY m_key;

	
};

/*! A custom HTK_PolyPolypoint class which includes the HOOPS/3dgs key and pointer to an HSharedKey persistent indentifier object */
class TK_PIPolyPolypoint : public HTK_PolyPolypoint {
public:
	/*! Constructs a TK_PIPolyPolypoint object.
	\param persIdent A pointer to the shared key that you want to associate with this object.
	*/
	TK_PIPolyPolypoint(unsigned char opcode, HSharedKey *persIdent) : HTK_PolyPolypoint(opcode) 
	{
		m_pPersIdent = persIdent;
	}
	OVERLOAD_WRITE_POLY(HTK_PolyPolypoint)
	OVERLOAD_EXECUTE_POLY(HTK_PolyPolypoint)

	/*!  This method makes a copy of itself and return a pointer to the newly created object.
	\param tk A reference to the BStreamFileToolkit object. 
	\param handler The method will return a pointer to a copy of itself.
	\return A #TK_Status indicating whether the operation was successful. 
	*/
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

private:
	HSharedKey *m_pPersIdent; 
	
};

/*! A custom HTK_Point class which includes the HOOPS/3dgs key and pointer to an HSharedKey persistent indentifier object */
class TK_PIPoint : public HTK_Point {
public:
	/*! Constructs a TK_PIPoint object.
	\param persIdent A pointer to the shared key that you want to associate with this object.
	*/
	TK_PIPoint(unsigned char opcode, HSharedKey *persIdent) : HTK_Point(opcode) 
	{
		m_pPersIdent = persIdent;
	}
	OVERLOAD_INTERPRET_INT(HTK_Point)
	OVERLOAD_WRITE(HTK_Point)
	OVERLOAD_EXECUTE(HTK_Point)

	/*!  This method makes a copy of itself and return a pointer to the newly created object.
	\param tk A reference to the BStreamFileToolkit object. 
	\param handler The method will return a pointer to a copy of itself.
	\return A #TK_Status indicating whether the operation was successful. 
	*/
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

private:
		HSharedKey *m_pPersIdent; 
	HC_KEY m_key;

	
};

/*! A custom HTK_Spot_Light class which includes the HOOPS/3dgs key and pointer to an HSharedKey persistent indentifier object */
class TK_PISpot_Light : public HTK_Spot_Light {
public:
	/*! Constructs a TK_PISpot_Light object.
	\param persIdent A pointer to the shared key that you want to associate with this object.
	*/
	TK_PISpot_Light(HSharedKey *persIdent) : HTK_Spot_Light() 
	{
		m_pPersIdent = persIdent;
	}
	OVERLOAD_INTERPRET_INT(HTK_Spot_Light)
	OVERLOAD_WRITE(HTK_Spot_Light)
	OVERLOAD_EXECUTE(HTK_Spot_Light)

	/*!  This method makes a copy of itself and return a pointer to the newly created object.
	\param tk A reference to the BStreamFileToolkit object. 
	\param handler The method will return a pointer to a copy of itself.
	\return A #TK_Status indicating whether the operation was successful. 
	*/
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

private:
		HSharedKey *m_pPersIdent; 
	HC_KEY m_key;

	
};

/*! A custom HTK_Area_Light class which includes the HOOPS/3dgs key and pointer to an HSharedKey persistent indentifier object */
class TK_PIArea_Light : public HTK_Area_Light {
public:
	/*! Constructs a TK_PIArea_Light object.
	\param persIdent A pointer to the shared key that you want to associate with this object.
	*/
	TK_PIArea_Light(HSharedKey *persIdent) : HTK_Area_Light() 
	{
		m_pPersIdent = persIdent;
	}
	OVERLOAD_INTERPRET_INT(HTK_Area_Light)
	OVERLOAD_WRITE(HTK_Area_Light)
	OVERLOAD_EXECUTE(HTK_Area_Light)

	/*!  This method makes a copy of itself and return a pointer to the newly created object.
	\param tk A reference to the BStreamFileToolkit object. 
	\param handler The method will return a pointer to a copy of itself.
	\return A #TK_Status indicating whether the operation was successful. 
	*/
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

private:
		HSharedKey *m_pPersIdent; 
	HC_KEY m_key;

	
};

/*! A custom HTK_Mesh class which includes the HOOPS/3dgs key and pointer to an HSharedKey persistent indentifier object */
class TK_PIMesh : public HTK_Mesh {
public:
	/*! Constructs a TK_PIMesh object.
	\param persIdent A pointer to the shared key that you want to associate with this object.
	*/
	TK_PIMesh(HSharedKey *persIdent) : HTK_Mesh() 
	{
		m_pPersIdent = persIdent;
	}
	OVERLOAD_INTERPRET_INT(HTK_Mesh)
	OVERLOAD_WRITE(HTK_Mesh)
	OVERLOAD_EXECUTE(HTK_Mesh)

	/*!  This method makes a copy of itself and return a pointer to the newly created object.
	\param tk A reference to the BStreamFileToolkit object. 
	\param handler The method will return a pointer to a copy of itself.
	\return A #TK_Status indicating whether the operation was successful. 
	*/
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

private:
		HSharedKey *m_pPersIdent; 
	HC_KEY m_key;

	
};

/*! A custom HTK_NURBS_Curve class which includes the HOOPS/3dgs key and pointer to an HSharedKey persistent indentifier object */
class TK_PINURBS_Curve : public HTK_NURBS_Curve {
public:
	/*! Constructs a TK_PINURBS_Curve object.
	\param persIdent A pointer to the shared key that you want to associate with this object.
	*/
	TK_PINURBS_Curve(HSharedKey *persIdent) : HTK_NURBS_Curve() 
	{
		m_pPersIdent = persIdent;
	}
	OVERLOAD_INTERPRET_INT(HTK_NURBS_Curve)
	OVERLOAD_WRITE(HTK_NURBS_Curve)
	OVERLOAD_EXECUTE(HTK_NURBS_Curve)

	/*!  This method makes a copy of itself and return a pointer to the newly created object.
	\param tk A reference to the BStreamFileToolkit object. 
	\param handler The method will return a pointer to a copy of itself.
	\return A #TK_Status indicating whether the operation was successful. 
	*/
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

private:
		HSharedKey *m_pPersIdent; 
	HC_KEY m_key;

	
};

/*! A custom HTK_Cutting_Plane class which includes the HOOPS/3dgs key and pointer to an HSharedKey persistent indentifier object */
class TK_PICutting_Plane : public HTK_Cutting_Plane {
public:
	/*! Constructs a TK_PICutting_Plane object.
	\param persIdent A pointer to the shared key that you want to associate with this object.
	*/
	TK_PICutting_Plane( HSharedKey *persIdent) : HTK_Cutting_Plane() 
	{
		m_pPersIdent = persIdent;
	}
	OVERLOAD_INTERPRET_INT(HTK_Cutting_Plane)
	OVERLOAD_WRITE(HTK_Cutting_Plane)
	OVERLOAD_EXECUTE(HTK_Cutting_Plane)

	/*!  This method makes a copy of itself and return a pointer to the newly created object.
	\param tk A reference to the BStreamFileToolkit object. 
	\param handler The method will return a pointer to a copy of itself.
	\return A #TK_Status indicating whether the operation was successful. 
	*/
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

private:
		HSharedKey *m_pPersIdent; 
	HC_KEY m_key;

	
};

/*! A custom HTK_Text class which includes the HOOPS/3dgs key and pointer to an HSharedKey persistent indentifier object */
class TK_PIText : public HTK_Text {
public:
	/*! Constructs a TK_PIText object.
	\param persIdent A pointer to the shared key that you want to associate with this object.
	*/
	TK_PIText(unsigned char opcode, HSharedKey *persIdent) : HTK_Text(opcode) 
	{
		m_pPersIdent = persIdent;
	}
	OVERLOAD_INTERPRET_INT(HTK_Text)
	OVERLOAD_WRITE(HTK_Text)
	OVERLOAD_EXECUTE(HTK_Text)

	/*!  This method makes a copy of itself and return a pointer to the newly created object.
	\param tk A reference to the BStreamFileToolkit object. 
	\param handler The method will return a pointer to a copy of itself.
	\return A #TK_Status indicating whether the operation was successful. 
	*/
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

private:
		HSharedKey *m_pPersIdent; 
	HC_KEY m_key;

	
};

/*! A custom HTK_Referenced_Segment class which includes the HOOPS/3dgs key and pointer to an HSharedKey persistent indentifier object */
class TK_PIReferenced_Segment : public HTK_Referenced_Segment {
public:
	/*! Constructs a TK_PIReferenced_Segment object.
	\param persIdent A pointer to the shared key that you want to associate with this object.
	*/
	TK_PIReferenced_Segment(unsigned char opcode, HSharedKey *persIdent) : HTK_Referenced_Segment(opcode) 
	{
		m_pPersIdent = persIdent;
	}
	OVERLOAD_INTERPRET_INT(HTK_Referenced_Segment)
	OVERLOAD_WRITE(HTK_Referenced_Segment)
	OVERLOAD_EXECUTE(HTK_Referenced_Segment)

	/*!  This method makes a copy of itself and return a pointer to the newly created object.
	\param tk A reference to the BStreamFileToolkit object. 
	\param handler The method will return a pointer to a copy of itself.
	\return A #TK_Status indicating whether the operation was successful. 
	*/
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

private:
	HSharedKey *m_pPersIdent; 
	HC_KEY m_key;

	
};



#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif





 


