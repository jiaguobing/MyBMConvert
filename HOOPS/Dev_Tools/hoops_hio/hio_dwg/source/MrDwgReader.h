// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#pragma warning (disable: 4996) // standard string functions considered unsafe

#include "MrDwgWorldDraw.h"
#include "MrDwgViewportDraw.h"
#include "misc.h"

#include <hc.h>

#include <map>
#include <set>
#include <sstream>

#include <dbmain.h>
#include <dbsymtb.h>
#include <dbdim.h>
#include <dbproxy.h>

#define MTEXT_EXPLODE_FRAGMENTS 1

#ifdef MTEXT_EXPLODE_FRAGMENTS
struct AcDbMTextFragment;
#endif

class MrDwgReader
{
public:
	MrDwgReader(bool skip_unreferenced_blocks);
	~MrDwgReader();

	virtual void importDatabase(AcDbDatabase *pDatabase);

	/* Map used to convert non-latin font names */
	std::map<std::wstring, std::string> m_fontmap;
	/* Segment in which the current entity will be inserted (up to importEntity level) */
	HC_KEY entityInsertionSegment;
	/* Information about multi line text which needs to be restyled before being broken into fragments */
	mTextData m_multiline_text_info;
protected:
	/* m_blockCount is used to track progress */
	int m_blockCount;
	int m_3dSolids;

	/* m_xrefList includes the root model filename and is used
	 * to compute an overall hash for the DWG model.  The hash
	 * is used to load cached HSF files. */
	std::set<std::wstring> m_xrefList;

	/* These two maps are for resolving block references.  A block
	 * may be referenced before it's loaded.  While we handle block
	 * references, just save the key to the current segment and the
	 * ID to the referenced block.  After loading the whole block
	 * table, resolve these references then. */
	std::map<AcDbHandle, HC_KEY> m_recordKeyMap;
	std::multimap<HC_KEY, AcDbHandle> m_referenceKeyMap;

	/* The autocad model needs to me included under each of these segments. */
	std::vector<HC_KEY> m_clips;

	HC_KEY m_modelKey;
	HC_KEY m_modelRoot;

	/* This, m_layerKeyMap, is just simpler (for now).  It's associates the layerID's with
	 * the layer segment's key.  This is used for entities which are colored by layer. */
	std::map<AcDbHandle, HC_KEY> m_layerKeyMap;

	/* m_viewportMap is used to associate a layer ID with all of its viewports*/
	struct viewportData
	{
		HC_KEY viewportSegmentKey;		// Key to the segment where the viewport will be inserted
		bool isMatrixID;				// True if the viewport's modelling matrix is I
	};
	std::map<AcDbHandle, std::vector<viewportData> > m_viewportMap;

	std::stringstream m_main_color_map;	//main color map

	/* dimensionData and m_dimensions are used to regenerate missing dimensions at runtime */
	struct dimensionData
	{
		dimensionData(HC_KEY dKey, HC_KEY sKey, AcDbHandle h, AcDbBlockTableRecord * btr)
		{
			destinationKey = dKey;
			sourceKey = sKey;
			dimensionHandle = h;
			dimensionBTR = btr;
		}

		HC_KEY destinationKey;						// key to the segment where the dimension will be inserted
		HC_KEY sourceKey;							// key to the segment containing the recomputed dimension BTR
		AcDbHandle dimensionHandle;					// handle of the dimension object
		AcDbBlockTableRecord * dimensionBTR;		// recomputed dimension BTR
	};
	std::vector<dimensionData> m_dimensions;
	void importOneBlockTableRecord(dimensionData * dimensions);
	void recomputeAndImportDimensions(std::vector<dimensionData> * dimensionVector);
	void restyleAndImportMText(mTextData * mtext_info);
	void populate_fontmap();

	/* importUnknownEntity is used to import entities that are not on the entity list,
	 * such as object enablers. In order for object enablers to be imported successfully
	 * all required DLLs and DBX files need to be in the bin directory */
	void importUnknownEntity(AcDbEntity * pEnt);

	virtual void progress(double percentComplete) const;
	virtual void importDwfInfo(AcDbDatabase *pDatabase);

	friend void importAcDb2LineAngularDimension(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDb2dVertex(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDb3PointAngularDimension(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDb2dPolyline(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDb3dPolyline(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDb3dPolylineVertex(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDb3dSolid(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbAlignedDimension(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbArc(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbAttribute(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbAttributeDefinition(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbBlockBegin(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbBlockEnd(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbBlockReference(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbBody(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbCircle(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbCurve(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbDiametricDimension(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbDimension(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbEllipse(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbFace(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbFaceRecord(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbFcf(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbFrame(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbHatch(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbLeader(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbLine(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbMInsertBlock(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbMText(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
#ifdef MTEXT_EXPLODE_FRAGMENTS
	friend static int callback_AcDbMTextFragments(AcDbMTextFragment *frag, void *param);
#endif
	friend void importAcDbMline(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbOle2Frame(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbOleFrame(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbOrdinateDimension(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbPoint(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbPolyFaceMesh(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbPolyFaceMeshVertex(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbPolygonMesh(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbPolygonMeshVertex(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbPolyline(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbProxyEntity(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbRadialDimension(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbRasterImage(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbRay(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbRegion(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbRotatedDimension(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbSequenceEnd(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbShape(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbSolid(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbSpline(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbTable(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbText(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbTrace(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbVertex(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbViewport(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbWipeout(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbXline(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);
	friend void importAcDbZombieEntity(MrDwgReader * mrDwgReader, AcDbEntity *pEnt);


	virtual void importAcDb2LineAngularDimension(AcDbEntity *pEnt);
	virtual void importAcDb2dVertex(AcDbEntity *pEnt);
	virtual void importAcDb3PointAngularDimension(AcDbEntity *pEnt);
	virtual void importAcDb2dPolyline(AcDbEntity *pEnt);
	virtual void importAcDb3dPolyline(AcDbEntity *pEnt);
	virtual void importAcDb3dPolylineVertex(AcDbEntity *pEnt);
	virtual void importAcDb3dSolid(AcDbEntity *pEnt);
	virtual void importAcDbAlignedDimension(AcDbEntity *pEnt);
	virtual void importAcDbArc(AcDbEntity *pEnt);
	virtual void importAcDbAttribute(AcDbEntity *pEnt);
	virtual void importAcDbAttributeDefinition(AcDbEntity *pEnt);
	virtual void importAcDbBlockBegin(AcDbEntity *pEnt);
	virtual void importAcDbBlockEnd(AcDbEntity *pEnt);
	virtual void importAcDbBlockReference(AcDbEntity *pEnt);
	virtual void importAcDbBody(AcDbEntity *pEnt);
	virtual void importAcDbCircle(AcDbEntity *pEnt);
	virtual void importAcDbCurve(AcDbEntity *pEnt);
	virtual void importAcDbDiametricDimension(AcDbEntity *pEnt);
	virtual void importAcDbDimension(AcDbEntity *pEnt);
	virtual void importAcDbEllipse(AcDbEntity *pEnt);
	virtual void importAcDbFace(AcDbEntity *pEnt);
	virtual void importAcDbFaceRecord(AcDbEntity *pEnt);
	virtual void importAcDbFcf(AcDbEntity *pEnt);
	virtual void importAcDbFrame(AcDbEntity *pEnt);
	virtual void importAcDbHatch(AcDbEntity *pEnt);
	virtual void importAcDbLeader(AcDbEntity *pEnt);
	virtual void importAcDbLine(AcDbEntity *pEnt);
	virtual void importAcDbMInsertBlock(AcDbEntity *pEnt);
	virtual void importAcDbMText(AcDbEntity *pEnt);
#ifdef MTEXT_EXPLODE_FRAGMENTS
	virtual void importAcDbMTextFragment(AcDbMTextFragment* pAcDbMTextFragment, AcDbEntity* pEnt);
#endif
	virtual void importAcDbMline(AcDbEntity *pEnt);
	virtual void importAcDbOle2Frame(AcDbEntity *pEnt);
	virtual void importAcDbOleFrame(AcDbEntity *pEnt);
	virtual void importAcDbOrdinateDimension(AcDbEntity *pEnt);
	virtual void importAcDbPoint(AcDbEntity *pEnt);
	virtual void importAcDbPolyFaceMesh(AcDbEntity *pEnt);
	virtual void importAcDbPolyFaceMeshVertex(AcDbEntity *pEnt);
	virtual void importAcDbPolygonMesh(AcDbEntity *pEnt);
	virtual void importAcDbPolygonMeshVertex(AcDbEntity *pEnt);
	virtual void importAcDbPolyline(AcDbEntity *pEnt);
	virtual void importAcDbProxyEntity(AcDbEntity *pEnt);
	virtual void importAcDbRadialDimension(AcDbEntity *pEnt);
	virtual void importAcDbRasterImage(AcDbEntity *pEnt);
	virtual void importAcDbRay(AcDbEntity *pEnt);
	virtual void importAcDbRegion(AcDbEntity *pEnt);
	virtual void importAcDbRotatedDimension(AcDbEntity *pEnt);
	virtual void importAcDbSequenceEnd(AcDbEntity *pEnt);
	virtual void importAcDbShape(AcDbEntity *pEnt);
	virtual void importAcDbSolid(AcDbEntity *pEnt);
	virtual void importAcDbSpline(AcDbEntity *pEnt);
	virtual void importAcDbTable(AcDbEntity *pEnt);
	virtual void importAcDbText(AcDbEntity *pEnt);
	virtual void importAcDbTrace(AcDbEntity *pEnt);
	virtual void importAcDbVertex(AcDbEntity *pEnt);
	virtual void importAcDbViewport(AcDbEntity *pEnt);
	virtual void importAcDbWipeout(AcDbEntity *pEnt);
	virtual void importAcDbXline(AcDbEntity *pEnt);
	virtual void importAcDbZombieEntity(AcDbEntity *pEnt);
	
	virtual void linkDimension(AcDbDimension * pDim);

	virtual void DrawEntity(AcDbEntity *pEnt, AcGiRegenType regenType);

	virtual HC_KEY GetBlockTableRecordSegKey(AcDbBlockTableRecord *pRecord);
	virtual HC_KEY GetAttrStateSegKey(AcDbEntity *pEnt, bool is_zombie_entity = false);
	virtual HC_KEY GetLayoutSegKey(ACHAR *layoutName);
	virtual HC_KEY GetLayerSegKey(ACHAR *layoutName);

	virtual void importXdata(AcDbEntity *pEnt);
	virtual bool importEntity(AcDbEntity *pEnt);
	void importEntityFromHandle(AcDbHandle handle);
	virtual void importBlockTable(AcDbBlockTable *pBlockTable);
	virtual void importViewportTable(AcDbViewportTable *pViewportTable);

	virtual void importLayerTable(AcDbLayerTable *pLayerTable);
	virtual bool applyEntityLinetype(AcDbEntity *pEnt);
	virtual bool importLinetypeAndApply(AcDbLinetypeTableRecord *pLinetypeRecord, double scale);

	struct mLineStyleDefComponent
	{
		double offset;
		AcCmColor color;
		AcDbObjectId linetypeId;
	};

	typedef std::vector<mLineStyleDefComponent> mLineStyleDef;
	std::map<AcDbHandle, mLineStyleDef> m_MlineStyles;

	virtual void importMlineStyleDictionary(AcDbDictionary *pDictionary);

	bool m_skip_unreferenced_blocks;
};
