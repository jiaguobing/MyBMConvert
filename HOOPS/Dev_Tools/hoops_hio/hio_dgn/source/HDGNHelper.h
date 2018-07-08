// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HDGNHelper_H__02052008_1505__
#define _HDGNHelper_H__02052008_1505__

#include "HTools.h"
#include "HIOManager.h"
#include "vhash.h"
#include "vlist.h"

// DGNdirect includes
#include "OdaCommon.h"
#include "DgDatabase.h"
#include "Int32Array.h"
#include "IntArray.h"
#include "Gi/GiWorldGeometry.h"
#include "Gi/GiPolyline.h"
#include "Gi/GiGeometry.h"
#include "Gi/GiCommonDraw.h"
#include "Gi/GiWorldDraw.h"
#include "Ge/geelliparc2d.h"
#include "Ge/geelliparc3d.h"
#include "DgModelerGeometry.h"
#include "Br/BrBrep.h"
#include "Gs/Gs.h"
#include "ModelerDefs.h"
#include "DbObjectId.h"
#include "dbfiler.h"
#include "DgLevelMask.h"
#include "dgmodel.h"
#include "DgModelerGeometry/DgModelerModule.h"
#include "Wr/wrTriangulationParams.h"

#include "StaticRxObject.h"
#include "DynamicLinker.h"
#include "RxDynamicModule.h"
#include "DgModelerGeometryCreator.h"
#include "ExDgnServices.h"
#include "ExDgnHostAppServices.h"

#include "Gi/GiDrawable.h"
#include "DgLine.h"
#include "DgLineString.h"
#include "DgText.h"
#include "DgTextNode.h"
#include "DgShape.h"
#include "DgCurve.h"
#include "DgEllipse.h"
#include "DgArc.h"
#include "DgCone.h"
#include "DgComplexString.h"
#include "DgComplexShape.h"
#include "DgPointString.h"
#include "DgDimension.h"
#include "DgMultiline.h"
#include "DgBSplineCurve.h"
#include "Dg3DObject.h"
#include "DgRasterAttach.h"
#include "DgRaster.h"
#include "DgTagElement.h"
#include "DgCellHeader.h"
#include "DgBSplineSurface.h"
#include "DgLevelTableRecord.h"
#include "DgFontTableRecord.h"
#include "DgTextStyleTableRecord.h"
#include "DgDimStyleTableRecord.h"
#include "DgMultilineStyleTableRecord.h"
#include "DgLineStyleTableRecord.h"
#include "DgRegAppTableRecord.h"
#include "DgView.h"
#include "DgSharedCellDefinition.h"
#include "DgSharedCellReference.h"
#include "DgTagSetDefinition.h"
#include "DgReferenceAttach.h"
#include "DgMesh.h"
#include "RxObjectImpl.h"
#include "resbuf.h"

#include <set>
#include <string>
using namespace std;

class XRef;

class TreeNode
{

private:
	
	char*			 m_pchName;
	OdUInt16		 m_nNestedDepth;

public:
	//write set function for this
	VHash<char*,int> *m_vhashOfImmediateChildXref ;
	//VList<char*>	 m_vlistOfImmediateChildXref;
	VList<TreeNode*> m_ChildNodes;
	TreeNode*		 m_pParentNode;

	TreeNode(char *pChNode)
	{
		m_pchName = new char[strlen(pChNode) + 1];
		strcpy(m_pchName,pChNode);
		m_ChildNodes.Flush();

		m_pParentNode = NULL;
		m_nNestedDepth = 0;

		m_vhashOfImmediateChildXref = new VHash<char*,int>;
	}

	~TreeNode()
	{
		m_ChildNodes.Flush();
		 
		m_vhashOfImmediateChildXref->Flush();

		H_SAFE_DELETE_ARRAY(m_pchName);
	}

	void SetNestedDepth(OdUInt16 nNestedDepth){ m_nNestedDepth =  nNestedDepth;}
	
	OdUInt16 GetNestedDepth(){ return m_nNestedDepth;}

	char *GetName(){ return m_pchName;}
	
};

// This class is the base class for importers of all objects.           
// Every object class has this extension.  
class hRxObject : public OdRxObjectImpl< OdRxObject >
{
	
public:

	hRxObject();
	~hRxObject(){};

	virtual void addRef();
	virtual void release();
	virtual long numRefs() const;
	virtual void import( OdRxObjectPtr object ) = 0; //each object class need to describe itself via this method

private:
	long        m_references;     //used by each object separately
	ODRX_DECLARE_MEMBERS( hRxObject );
	
};

#define MAX_LEVEL_NAME_LEN 256

class HDGNImporter : public hRxObject
{
public:

	HDGNImporter(){;};
	~HDGNImporter(){;};

	void setDatabase(OdDgDatabasePtr database){ pDb = database ;};

	inline void OdGePoint2d_2_HPoint(const OdGePoint2d* odg_point, HPoint& h_point)
	{
		h_point.x = odg_point->x;
		h_point.y = odg_point->y;
		h_point.z = 0.0f;	
	}

	inline void OdGePoint3d_2_HPoint(const OdGePoint3d* odg_point, HPoint& h_point)
	{
		h_point.x = odg_point->x;
		h_point.y = odg_point->y;
		h_point.z = odg_point->z;
	}

	void setInputHandler(HInputHandler* hand){ m_inputHandler= hand;};

	HFileInputResult FileInputByKey(const char * FileName, HC_KEY model_key, HInputHandlerOptions * options);
	bool importDGNFile( const char * FileName, bool isXrefPresent);
	OdDgModelPtr getActiveModel(OdDgDatabasePtr pDatabase);

	virtual void import( OdRxObjectPtr object ){;}; 
	//In  iterateElm "int CHeaderid=0" is loaded by default as a"0" bby assuming CellHeader id will not be 0. 
	void iterateElm( OdDgElementIteratorPtr iterator, bool visibilityProcessing, bool checkProgress,int CHeaderid=0);

	HC_KEY getLevelKey( OdDgGraphicsElementPtr grElm );

	void setLineStyleAndWt(OdDgGraphicsElementPtr grElm);

	void importXData (OdResBufPtr pRb);

	int calculateTotalEntities( OdRxObjectPtr object );

	void importCellheaderWithHoleAttr(); 	
	bool AddGeometryInList(bool);

	bool isAttachedXREF(const char* FileName);
	
	//Vinit changes
	bool createTreeOfXrefs( const char * FileName,TreeNode *ParentNode =0);
	void fillTheCurrentXrefNodeValues(const char* FileName,TreeNode *ParentNode = 0);
	bool chkOnlySecondLevelXref(const char* FileName);
	bool chkXrefFileLevel(const char* FileName);


protected:
	
	//if m_bDGNImportAllModels = true importing all the models available in DGN file.
	//default value is m_bDGNImportAllModels = false  - Vinit M

	static bool m_bDGNImportAllModels;
	static bool m_bisFromEntity ;
	static bool m_bIgnoreOffEntities ;
	static char m_cCurrDir[1024] ;
	static bool m_bFromCellHeader ;
	static double m_coneFactor ;
	static bool m_bDGNRebaselineModel ;
	static bool m_bNoOffset ;
	static OdDgDatabasePtr pDb;
	static bool m_bFromSmartSolid ;
	static char m_cCellHeaderName[1024];
	static int m_totalEntities;
	static HInputHandler* m_inputHandler;
	static VHash<HC_KEY,int> *m_hashGeomCellID ;
	static double m_dMinX, m_dMinY, m_dMinZ;
	static OdSmartPtr< XRef > m_obXRef;
	static VHash<int,HC_KEY> m_hashLevelIDKey;

	static int m_iCellHeaderId;
	static bool m_bCellHeaderHoleFlag ;
	static bool m_bWorldDrawEntityFlag ;
	static HC_KEY m_hkShellKey;
	static HC_KEY m_hkSolidSegKey;
	static HC_KEY m_hkHoleSegKey;
	static HC_KEY m_hkSegTobeUsed;
	
	enum {SINGLE_ENTITY_CELLHEADER, BREAK_CELLHEADER, OPT_CELLHEADER};
	static int m_iCellHeaderOption ;

private:

	VStringHash<int> imported_xrefs;
	void resetStaticVars();

	
	//declare  OdGiGeometry_ as friend of this class
	template<class> friend class OdGiGeometry_; 
	friend class OdGiGeometry_ <class T>;


};

class XRef : public HDGNImporter
{

public:
	
	XRef()
	{
	};

	~XRef()
	{
		m_listOfXRefs.Flush();
		m_listOfXRefsOfXRefs.Flush();
	};

	void populateXREFs();
	VList<char*> m_listOfXRefsOfXRefs;

private:
	VList<char*> m_listOfXRefs;
	char* m_cXRefSegName;
	void import( OdRxObjectPtr object );
	void fillVlistOfXref(OdDgReferenceAttachmentHeaderPtr element);
};

class Database : public HDGNImporter
{
	void import( OdRxObjectPtr object );
};


class Model : public HDGNImporter
{
public:
	void import( OdRxObjectPtr object );
};


class Line2d : public HDGNImporter
{
	void import( OdRxObjectPtr object );
};


class Line3d : public HDGNImporter
{
	void import( OdRxObjectPtr object );
};


class LineString2d : public HDGNImporter
{
	void import( OdRxObjectPtr object );
};


class LineString3d : public HDGNImporter
{
	void import( OdRxObjectPtr object );
};


class Text2d : public HDGNImporter
{
	void import( OdRxObjectPtr object );
};


class Text3d : public HDGNImporter
{
	void import( OdRxObjectPtr object );

};


class TextNode2d : public HDGNImporter
{
	void import( OdRxObjectPtr object );
};

class TextNode3d : public HDGNImporter
{
	void import( OdRxObjectPtr object );
};


class Shape2d : public HDGNImporter
{
	void import( OdRxObjectPtr object );
};


class Shape3d : public HDGNImporter
{
	void import( OdRxObjectPtr object );
};


class Curve2d : public HDGNImporter
{
	void import( OdRxObjectPtr object ){};
};


class Curve3d : public HDGNImporter
{
	void import( OdRxObjectPtr object ){};
};


class Arc2d : public HDGNImporter
{
private:	
	void import( OdRxObjectPtr object );

public:
	void importAsEllipticalArc(OdGeEllipArc2d ellipArc, double rot_angle, bool isClosed);

};


class Arc3d : public HDGNImporter
{
private:
	void import( OdRxObjectPtr object );
public:
	void importAsEllipticalArc(OdGeEllipArc3d ellipArc, bool isClosed);

	//declare  OdGiGeometry_ as friend of this class
	template<class> friend class OdGiGeometry_; 
	friend class OdGiGeometry_ <class T>;

};


//Case of multilevel inheritence : Mustafa B
class Ellipse2d :public Arc2d
{
	void import( OdRxObjectPtr object );
};


class Ellipse3d : public Arc3d
{
	void import( OdRxObjectPtr object );
};

class Cone : public HDGNImporter
{

	static double m_dDeviationForCone;
	void import( OdRxObjectPtr object );

	//declare  OdGiCommonDraw_ as friend of this class
	template<class> friend class OdGiCommonDraw_; 
	friend class OdGiCommonDraw_ <class T>;
	
};


class ComplexString : public HDGNImporter
{
	void import( OdRxObjectPtr object ){};
};


class ComplexShape : public HDGNImporter
{

	void import( OdRxObjectPtr object );

};


class PointString2d : public HDGNImporter
{
	void import( OdRxObjectPtr object ){};
};


class PointString3d : public HDGNImporter
{
	void import( OdRxObjectPtr object ){};

};


class Dimension : public HDGNImporter
{
	void import( OdRxObjectPtr object );

};


class Multiline : public HDGNImporter
{
	void import( OdRxObjectPtr object ){};

};


class BSplinePoles2d : public HDGNImporter
{
	void import( OdRxObjectPtr object ){};

};


class BSplinePoles3d : public HDGNImporter
{
	void import( OdRxObjectPtr object ){};
};


class BSplineKnots : public HDGNImporter
{
	void import( OdRxObjectPtr object ){};

};

class BSplineWeights : public HDGNImporter
{
	void import( OdRxObjectPtr object ){};

};


class BSpline : public HDGNImporter
{
public:
	void getUniformKnots(bool isClosed, int totalKnots, int order, float*& hKnots);
};

class BSplineCurve2d : public BSpline
{
	void import( OdRxObjectPtr object );
};

class BSplineCurve3d : public BSpline
{
	void import( OdRxObjectPtr object );
};


class BSplineSurface : public BSpline
{

	void import( OdRxObjectPtr object );
};


class Surface : public HDGNImporter
{

	void import( OdRxObjectPtr object );

};


class Solid : public HDGNImporter
{

	void import( OdRxObjectPtr object );

};


class RasterAttachmentComponentBinary : public HDGNImporter
{

	void import( OdRxObjectPtr object ){};

};


class RasterAttachmentComponentContinuous : public HDGNImporter
{
	void import( OdRxObjectPtr object ){};
};




class RasterAttachmentComponentRGB : public HDGNImporter
{

	void import( OdRxObjectPtr object ){};

};


class RasterAttachmentComponentClip : public HDGNImporter
{

	void import( OdRxObjectPtr object ){};

};


class RasterAttachmentComponentColorTable : public HDGNImporter
{
	void import( OdRxObjectPtr object ){};
};


class RasterAttachmentComponentGeo : public HDGNImporter
{

	void import( OdRxObjectPtr object ){};

};


class RasterAttachmentComponentRender : public HDGNImporter
{

	void import( OdRxObjectPtr object ){};

};


class RasterAttachmentComponentTransparency : public HDGNImporter
{

	void import( OdRxObjectPtr object ){};

};


class RasterAttachmentComponentRGBTransparency : public HDGNImporter
{

	void import( OdRxObjectPtr object ){};


};


class RasterAttachmentComponentMask : public HDGNImporter
{

	void import( OdRxObjectPtr object ){};

};


class RasterAttachmentHeader : public HDGNImporter
{

	void import( OdRxObjectPtr object ){};

};


class RasterHeader2d : public HDGNImporter
{

	void import( OdRxObjectPtr object ){};

};


class RasterHeader3d : public HDGNImporter
{
	void import( OdRxObjectPtr object ){};

};


class RasterComponent : public HDGNImporter
{

	void import( OdRxObjectPtr object ){};

};


class TagElement : public HDGNImporter
{

	void import( OdRxObjectPtr object ){};


};


class CellHeader2d : public HDGNImporter
{

	void import( OdRxObjectPtr object );
};


class CellHeader3d : public HDGNImporter
{
	void import( OdRxObjectPtr object );
};

class LevelTable : public HDGNImporter
{

	void import( OdRxObjectPtr object ){};

};


class LevelTableRecord : public HDGNImporter
{

	void import( OdRxObjectPtr object ){};

};


class FontTable : public HDGNImporter
{

	void import( OdRxObjectPtr object ){};

};


class FontTableRecord : public HDGNImporter
{
	void  import( OdRxObjectPtr object ){};

};


class TextStyleTable : public HDGNImporter
{

	void import( OdRxObjectPtr object ){};

};


class TextStyleTableRecord : public HDGNImporter
{
	void import( OdRxObjectPtr object ){};

};


class DimStyleTable : public HDGNImporter
{

	void import( OdRxObjectPtr object ){};


};


class DimStyleTableRecord : public HDGNImporter
{
	void import( OdRxObjectPtr object ){};

};


class MultilineStyleTable : public HDGNImporter
{

	void import( OdRxObjectPtr object ){};

};


class MultilineStyleTableRecord : public HDGNImporter
{

	void import( OdRxObjectPtr object ){};


};


class LineStyleTable : public HDGNImporter
{

	void import( OdRxObjectPtr object ){};


};


class LineStyleTableRecord : public HDGNImporter
{

	void import( OdRxObjectPtr object ){};


};


class RegAppTable : public HDGNImporter
{

	void import( OdRxObjectPtr object ){};

};


class RegAppTableRecord : public HDGNImporter
{

	void import( OdRxObjectPtr object ){};

};


class ViewTable : public HDGNImporter
{

	void import( OdRxObjectPtr object ){};

};


class View : public HDGNImporter
{
	void import( OdRxObjectPtr object );

};

class ViewGroupTable : public HDGNImporter
{
	void import( OdRxObjectPtr object );
};

class ViewGroup : public HDGNImporter
{

	void import( OdRxObjectPtr object );
};

class NamedViewTable : public HDGNImporter
{
	void import( OdRxObjectPtr object ){};

};


class SharedCellDefinitionTable : public HDGNImporter
{

	void import( OdRxObjectPtr object );

};


class SharedCellDefinition : public HDGNImporter
{

	void import( OdRxObjectPtr object );


};

class SharedCellReference : public HDGNImporter
{
public:
	SharedCellReference()
	{
		m_iSharedCellRefCounter = 0;
		m_bSharedCellDefTable = false;
	};

private:
	int m_iSharedCellRefCounter;
	bool m_bSharedCellDefTable;
	void import( OdRxObjectPtr object );
};


class TagSetDefinitionTable : public HDGNImporter
{
	void import( OdRxObjectPtr object ){};

};


class TagSetDefinition : public HDGNImporter
{
	void import( OdRxObjectPtr object ){};

};


class TagDefinition : public HDGNImporter
{
	void import( OdRxObjectPtr object ){};

};

class TagDefinitionSetTable : public HDGNImporter
{
	void import( OdRxObjectPtr object ){};

};

class TagDefinitionSet : public HDGNImporter
{
	void import( OdRxObjectPtr object ){};

};


class ColorTable : public HDGNImporter
{
	void import( OdRxObjectPtr object ){};

};

class Mesh : public HDGNImporter
{
	void import( OdRxObjectPtr object );

};

//worlddraw() callback classes

class HOdExDgnSystemServices :	public OdExDgnSystemServices, 
	public OdExDgnHostAppServices
{
	ODRX_USING_HEAP_OPERATORS(OdExDgnSystemServices);
};

extern OdStaticRxObject< HOdExDgnSystemServices > g_HIO_OdExDgnSystemServices ;

//worldDraw() class
class sampleTraits : public OdStaticRxObject<OdGiSubEntityTraits>
{
	//ODRX_DECLARE_MEMBERS(OdGiSubEntityTraits);

	virtual void setColor(OdUInt16 color) {}

	virtual void setTrueColor(const OdCmEntityColor& cmColor) {}

	virtual void setLayer(OdDbStub* layerId) {}

	virtual void setLineType(OdDbStub* linetypeId) {}

	virtual void setSelectionMarker(OdGsMarker markerId) {}

	virtual void setFillType(OdGiFillType fillType) {}

	virtual void setFillPlane(const OdGeVector3d* /*pNormal*/ = 0) {}

	virtual void setLineWeight(OdDb::LineWeight lw) {}

	virtual void setLineTypeScale(double dScale = 1.0) {}

	virtual void setLineTypeScaleToFit(bool /*doIt*/) /*= 0*/ {};

	virtual void setThickness(double dThickness) {}

	virtual void setPlotStyleName(OdDb::PlotStyleNameType, OdDbStub* = 0) {}

	virtual void setMaterial(OdDbStub* /*materialId*/) {}

	virtual void setMapper(const OdGiMapper * /*pMapper*/) {}

	virtual void setVisualStyle(const OdDbStub* /*visualStyleId*/) {}

	virtual OdUInt16 color() const { return 0; }

	virtual OdCmEntityColor trueColor() const { return OdCmEntityColor(); }

	virtual OdDbStub* layer() const { return 0; }

	virtual OdDbStub* lineType() const { return 0; }

	virtual OdGiFillType fillType() const { return kOdGiFillAlways; }

	virtual bool fillPlane(OdGeVector3d& /*normal*/) { return false; }

	virtual OdDb::LineWeight lineWeight() const { return OdDb::kLnWtByLwDefault; }

	virtual double lineTypeScale() const { return 0.; }

	virtual double thickness() const { return 0.; }

	virtual OdDb::PlotStyleNameType plotStyleNameType() const { return OdDb::kPlotStyleNameByLayer; }

	virtual OdDbStub* plotStyleNameId() const {  return 0; }

	virtual OdDbStub* material() const {  return 0; }

	virtual const OdGiMapper* mapper() const { return 0; }

	virtual OdDbStub* visualStyle() const { return 0; }

	virtual void addLight(const OdDbStub* lightId){}

};

//worldDraw() class
class shellStore
{
public:

	OdGePoint3dArray pnts;
	OdInt32Array     faces;

	void shell( OdInt32 numVertices, const OdGePoint3d* vertexList, OdInt32 faceListSize, const OdInt32* faceList, const OdGiEdgeData* pEdgeData = 0, const OdGiFaceData* pFaceData = 0, const OdGiVertexData* pVertexData = 0)
	{
		pnts.resize(numVertices);
		memcpy(pnts.asArrayPtr(), vertexList, sizeof(OdGePoint3d)*numVertices);
		faces.resize(faceListSize);
		memcpy(faces.asArrayPtr(), faceList, sizeof(OdInt32)*faceListSize);
	}
};

//worldDraw() class
template<class T> 
class OdGiGeometry_ : public T
{
	
public:
	OdArray<shellStore>     &m_WireCache;

	shellStore *appendWire()
	{
		shellStore *pWire = m_WireCache.append();
		return pWire;
	}

	OdGiGeometry_(OdArray<shellStore> &pWireCache) 
		: m_WireCache(pWireCache)
	{
	}

	virtual void pline(const OdGiPolyline& pline , OdUInt32 n1 , OdUInt32 n2 );
	

	virtual OdGeMatrix3d getModelToWorldTransform() const
	{
		return OdGeMatrix3d();
	};
	virtual OdGeMatrix3d getWorldToModelTransform() const
	{
		return OdGeMatrix3d();
	};

	virtual void pushModelTransform(const OdGeVector3d& vNormal){};

	virtual void pushModelTransform(const OdGeMatrix3d& xMat);
	
	virtual void popModelTransform(){};


	// For drawing various primitives.
	//
	virtual void circle(const OdGePoint3d& center, double radius, const OdGeVector3d& normal);

	virtual void circle(const OdGePoint3d& p1, const OdGePoint3d& p2, const OdGePoint3d& p3);
	
	virtual void circularArc(const OdGePoint3d& center,double radius,
		const OdGeVector3d& normal,const OdGeVector3d& startVector,
		double sweepAngle,OdGiArcType arcType = kOdGiArcSimple);
	

	virtual void circularArc(const OdGePoint3d& start,const OdGePoint3d& point,
		const OdGePoint3d& end,OdGiArcType arcType = kOdGiArcSimple);
	

	virtual void polyline(OdInt32 nbPoints,const OdGePoint3d* pVertexList,
		const OdGeVector3d* pNormal = NULL,OdGsMarker lBaseSubEntMarker = -1) ;
	

	virtual void polygon(OdInt32 nbPoints, const OdGePoint3d* pVertexList);
	

	virtual void mesh(OdInt32 rows,
		OdInt32 columns,
		const OdGePoint3d* pVertexList,
		const OdGiEdgeData* pEdgeData = NULL,
		const OdGiFaceData* pFaceData = NULL,
		const OdGiVertexData* pVertexData = NULL);
	
	virtual void shell(OdInt32 nbVertex,
		const OdGePoint3d* pVertexList,
		OdInt32 faceListSize,
		const OdInt32* pFaceList,
		const OdGiEdgeData* pEdgeData,
		const OdGiFaceData* pFaceData,
		const OdGiVertexData* pVertexData );
	
	virtual void text(const OdGePoint3d& position,
		const OdGeVector3d& normal, const OdGeVector3d& direction,
		double height, double width, double oblique, const OdString& msg);
	
	virtual void text(const OdGePoint3d& position,
		const OdGeVector3d& normal, const OdGeVector3d& direction,
		const OdChar* msg, OdInt32 length, bool raw, const OdGiTextStyle* pTextStyle);
	

	void ray(const OdGePoint3d& first, const OdGePoint3d& second);
	
	virtual void nurbs(const OdGeNurbCurve3d& nurbs){};

	virtual void xline(const OdGePoint3d& p1, const OdGePoint3d& p2);
	
	virtual void ellipArc(const OdGeEllipArc3d& arc,
		const OdGePoint3d* pEndPointsOverrides = 0,
		OdGiArcType arcType = kOdGiArcSimple);
	
	virtual void draw(const OdGiDrawable* pD)
	{
		// OdGiDrawable::DrawableType drawable = pD->drawableType();
		pD->drawableType();
	}

	// If you push a clip boundary onto the stack you must pop it;
	//
	virtual void pushClipBoundary(OdGiClipBoundary* pBoundary){};
	virtual void popClipBoundary(){};

	virtual void worldLine(const OdGePoint3d pnts[2]);
	
	virtual const OdGiPathNode* currentGiPath() const
	{
		return 0;
	}
	
private:
	OdGiGeometry_();
	OdSmartPtr< HDGNImporter > m_pHDGNImporter;
	Arc3d m_arc3d;
};


template<class T> 
class OdGiCommonDraw_ : public T
{
	OdGiContext* pCtx;
public:

	OdGiCommonDraw_() {}

	virtual double deviation(const OdGiDeviationType t, const OdGePoint3d& p) const ;

	virtual OdGiRegenType regenType() const              { return kOdGiRenderCommand; }
	virtual bool regenAbort() const                      { return false;              }
	virtual OdGiSubEntityTraits& subEntityTraits() const { return (OdGiSubEntityTraits&)m_traits; }
	//virtual OdGiSubEntityTraits& subEntityTraits() const { return (OdGiSubEntityTraits&)*this; }
	virtual bool isDragging() const                      { return false;              }
	virtual OdUInt32 numberOfIsolines() const            { return 4;				  }
	virtual OdGiContext* context() const                 { return pCtx;               }
	void	setContext(OdGiContext* pUserContext)			 { pCtx = pUserContext;		  }

	
private:
	Cone m_cone;
	sampleTraits m_traits;
};

//Setting the contex for worlddraw() function.
class OdGiContextDumper : public OdStaticRxObject<OdGiContext>
{
	OdDbDatabase* m_pDb;

public:
	OdGiContextDumper(
		OdDbDatabase* pDb) : m_pDb(pDb) 
	{}

	OdDbBaseDatabase* database() const 
	{ return (OdDbBaseDatabase*)m_pDb; }

	OdGiDrawablePtr openDrawable(OdDbStub*) { return (OdGiDrawable*)0; }
};

class OdWiresWorldGeometry : public OdGiGeometry_<OdStaticRxObject<OdGiWorldGeometry> >
	, public OdGiCommonDraw_<OdStaticRxObject<OdGiWorldDraw> >
{
	ODRX_USING_HEAP_OPERATORS(OdGiGeometry_<OdStaticRxObject<OdGiWorldGeometry> >);

public:
	OdWiresWorldGeometry(OdArray<shellStore> &pWireCache)
		: OdGiCommonDraw_<OdStaticRxObject<OdGiWorldDraw> >()
		, OdGiGeometry_<OdStaticRxObject<OdGiWorldGeometry> >(pWireCache)
	{
	}
	virtual void draw(const OdGiDrawable* pD) 
	{
		pD->worldDraw(this);
	}
	virtual OdGiGeometry& rawGeometry() const            
	{ 
		return (OdGiGeometry&)*this;      
	}
	virtual OdGiWorldGeometry& geometry() const          
	{ 
		return (OdGiWorldGeometry&)*this; 
	}

	virtual void setExtents(const OdGePoint3d *pNewExtents) {  }
};
//Callback code for worldDraw() function ends

class OdDgImporter
{
private:

	Database                                 m_databaseimporter;
	Model                                    m_modelimporter;
	CellHeader2d                             m_cellHeader2dimporter;
	CellHeader3d                             m_cellHeader3dimporter;
	Solid                                    m_solidimporter;
	Surface                                  m_surfaceimporter;
	Cone                                     m_coneimporter;
	Line2d                                   m_line2dimporter;
	Line3d                                   m_line3dimporter;
	LineString2d                             m_lineString2dimporter;
	LineString3d                             m_lineString3dimporter;
	Shape2d									 m_shape2dimporter;
	Shape3d									 m_shape3dimporter;
	Ellipse2d                                m_ellipse2dimporter;
	Ellipse3d                                m_ellipse3dimporter;
	Arc2d                                    m_arc2dimporter;
	Arc3d                                    m_arc3dimporter;
	ComplexShape                             m_complexShapeimporter;
	BSplineCurve2d							 m_bSplineCurve2dimporter;
	BSplineCurve3d                           m_bSplineCurve3dimporter;
	BSplineSurface							 m_bSplineSurfaceimporter;
	Text2d                                   m_bText2dimporter;
	Text3d                                   m_bText3dimporter;
	XRef									 m_bReferenceHeaderimporter;	
	SharedCellDefinition					 m_bSharedCellDefinitionimporter;
	SharedCellDefinitionTable				 m_bSharedCellDefinitionTableimporter;
	SharedCellReference						 m_bSharedCellReferenceimporter	;		 
	Mesh									 m_bMeshimporter;
	Dimension                                m_dimensionImporter;
	View								     m_viewImporter;
	ViewGroupTable							 m_viewGroupTableImporter;
	ViewGroup								 m_viewGroupImporter;
	TextNode2d                               m_textNode2dImporter;
	TextNode3d                               m_textNode3dImporter;


public:
	OdDgImporter();
	~OdDgImporter();
};

#endif //_HDGNHelper_H__02052008_1505__



