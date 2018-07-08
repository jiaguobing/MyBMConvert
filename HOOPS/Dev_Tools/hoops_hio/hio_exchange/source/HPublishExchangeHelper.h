// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HPUBLISHEXCHANGEHELPER_H
#define _HPUBLISHEXCHANGEHELPER_H


#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#ifdef _MSC_VER
#include <tchar.h>
#endif
#include <wchar.h>
#include <map>

#include "hc.h"
#include "HTools.h"
#include "HUtilityPMI.h"
#include "vlist.h"
#include "varray.h"
#include "vhash.h"

#include <vector>
#include <unordered_set>
#include <algorithm>
#include <string>
#include <unordered_map>


#ifdef _DEBUG
#	ifdef _MSC_VER
#		define ASSERT(x) ((!(x))?__debugbreak():1)
#	else
#		define ASSERT(x) assert(x)
#	endif
#else
#	define ASSERT(x) do if (0 && (x)) {((void)(x));} while (0)
#endif

#define PI 3.1415926535897932384626433832795028841971693993751


class HInputHandlerOptions;
class HOutputHandlerOptions;
class HIOConnector3DX;
class HIOUtility3DX;
class BREP_Edge_Matcher;
class BREP_Topology;
class HIOConnector;
class HBaseView;
typedef int  BOOL;

#ifndef HIO_PUBLISH
class A3DPDFDocument;
class A3DPDF3DArtwork;
class A3DPDFView;
struct A3DPDFRgbColorData;
#endif


class BREP_Edge_Matcher
{
public:

	BREP_Edge_Matcher();
	~BREP_Edge_Matcher();
	struct vlist_s *		m_edgelist;
	struct vlist_s *		m_edgeidlist;
	struct vhash_s *		m_edgehash;
	HC_KEY * m_edgekeys;

	void AddModellerEdge(void *edgeid);
	void Restart();
	void StartEdgeIteration();
	bool NextEdge(HC_KEY edgekey, HC_KEY &origkey, void **origid);
 	long m_edgenum;
 };





class HPublishExchangeView
{
public:
	HC_KEY	m_markup_layer_key;
	HCamera* m_pcamera;
	char* m_name;

public:
	HPublishExchangeView(HC_KEY	 markup_layer_key = INVALID_KEY):
	  m_markup_layer_key(markup_layer_key), m_pcamera(NULL), m_name(NULL){}
	~HPublishExchangeView()
	{
		if(m_pcamera)
			delete m_pcamera;
		if(m_name)
			delete m_name;
	}

public:
	HFileIOResult Populate();
	HFileIOResult ConvertToPDFView(A3DPDFDocument* pDoc, A3DPDF3DArtwork* p3DArtwork, A3DPDFRgbColorData* sBackgroundColor) const;
};




struct CaseInsensitiveStringHash
{
	size_t operator()(std::string const & key) const
	{
		std::string lowerCaseString;
		std::for_each(key.begin(), key.end(), [&] (char c) { lowerCaseString += static_cast<char>(tolower(c)); });
		return std::hash<std::string>()(lowerCaseString);
	}
};

struct CaseInsensitiveStringEqual
{
	bool operator()(std::string const & lhs, std::string const & rhs) const
	{
		return (lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin(), [] (char a, char b) { return tolower(a) == tolower(b); }));
	}
};

struct RGBColor
{
	float red;
	float green;
	float blue;

	RGBColor() : red(-1), green(-1), blue(-1) {}
	RGBColor(float r, float g, float b) : red(r), green(g), blue(b) {}

	inline RGBColor operator+(RGBColor const & that) const { return RGBColor(red + that.red, green + that.green, blue + that.blue); }
	inline RGBColor operator-(RGBColor const & that) const { return RGBColor(red - that.red, green - that.green, blue - that.blue); }
	inline RGBColor operator*(float t) const { return RGBColor(red * t, green * t, blue * t); }
};

struct RGBAColor
{
	float red;
	float green;
	float blue;
	float alpha;

	RGBAColor() : red(-1), green(-1), blue(-1), alpha(-1) {}
	RGBAColor(float r, float g, float b) : red(r), green(g), blue(b), alpha(1) {}
	RGBAColor(float r, float g, float b, float a) : red(r), green(g), blue(b), alpha(a) {}

	inline bool operator==(RGBAColor const & c) const { return red == c.red && blue == c.blue && green == c.green && alpha == c.alpha; }
	inline bool operator!=(RGBAColor const & c) const { return !(*this == c); }
};

struct Vector
{
	float x;
	float y;
	float z;

	Vector() {}
	Vector(float x, float y, float z) : x(x), y(y), z(z) {}
	Vector(Vector const & that) : x(that.x), y(that.y), z(that.z) {}

	Vector const operator+(Vector const & v) const { return Vector(x + v.x, y + v.y, z + v.z); }
	Vector const operator-(Vector const & v) const { return Vector(x - v.x, y - v.y, z - v.z); }
	Vector & operator+=(Vector const & v) { x += v.x; y += v.y; z += v.z; return *this; }
	Vector & operator-=(Vector const & v) { x -= v.x; y -= v.y; z -= v.z; return *this; }

	Vector const operator*(float s) const { return Vector(s * x, s * y, s * z); }
	Vector const operator/(float s) const { return operator*(1.0f / s); }
	Vector & operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
	Vector & operator/=(float s) { return operator*=(1.0f / s); }

	inline float Dot(Vector const & v) const { return x * v.x + y * v.y + z * v.z; }
	inline float Length() const { return sqrt(Dot(*this)); }
	inline Vector & Normalize()
	{
		float length = Length();
		if (length > 1e-30f)
			operator/=(length);
		else
			*this = Zero();
		return *this;
	}

	static inline Vector Zero() { return Vector(0, 0, 0); }
};

inline Vector operator*(float s, Vector const & v) { return v * s; }





typedef std::unordered_map<std::string, std::string, CaseInsensitiveStringHash, CaseInsensitiveStringEqual> StringStringHash;
typedef VArray<HPublishExchangeView *>		HPEViewsArray;
typedef std::vector<A3DDouble>				A3DDoubleArray;
typedef std::vector<A3DUns32>				A3DUns32Array;
typedef std::vector<PMI::Polyline>			PMIPolylineArray;
typedef std::vector<PMI::Polygon>			PMIPolygonArray;
typedef std::vector<PMI::Point>				PMIPointArray;
typedef std::vector<PMI::String>			PMIStringArray;
typedef std::vector<PMI::TextAttributes>	PMITextAttributesArray;
typedef std::vector<PMI::Entity>			PMIEntityArray;
typedef std::vector<A3DUTF8Char *>			A3DUTF8StringArray;
typedef std::vector<std::string>			StdStringArray;
typedef std::vector<HPoint>					HPointArray;
typedef std::vector<int>					IntArray;
typedef std::vector<float>					FloatArray;
typedef std::vector<RGBColor>				RGBColorArray;
typedef std::vector<RGBAColor>				RGBAColorArray;
typedef std::vector<char>					CharArray;
typedef std::vector<A3DUns8>				A3DUns8Array;
typedef std::vector<HC_KEY>					KeyArray;
typedef std::vector<Vector>					VectorArray;
typedef std::unordered_map<A3DUns32, IntArray> IndexHash;
typedef std::unordered_map<A3DEntity *, A3DUns32> EntityCountHash;
typedef std::unordered_map<HC_KEY, int> KeyCountHash;

enum HashPair
{
	Key = 0,
	Value
};

typedef std::unordered_map<HC_KEY, A3DUns32> KeyIndexHash;
typedef std::pair<std::string, HC_KEY> TextureSourcePair;

enum TextureSourcePairComponent
{
	Definition = 0,
	Source
};

struct TextureSourcePairHash
{
	size_t operator()(TextureSourcePair const & pair) const
	{
		return (std::hash<std::string>()(std::get<Definition>(pair)) ^ std::hash<HC_KEY>()(std::get<Source>(pair)));
	}
};

typedef std::unordered_map<TextureSourcePair, A3DUns32, TextureSourcePairHash> TextureSourceHash;




class KeyPath
{
public:
	KeyPath()
		: _count(0), _rootKeyDepth(0), _keys(256), _cacheValid(false) {}

	void AddKey(HC_KEY key)
	{
		if (_count == _keys.size())
		{
			KeyArray newKeys(static_cast<size_t>(_keys.size() * 1.333));
			memcpy(&newKeys[newKeys.size() - _count], &_keys[0], _count * sizeof(HC_KEY));
			_keys = newKeys;
		}
		_keys[_keys.size() - ++_count] = key;
		_cacheValid = false;
	}

	void RemoveKey()
	{
		if (_count > 0)
		{
			--_count;
			_cacheValid = false;
		}

		if (_count == 0)
			_rootKeyDepth = 0;
	}

	int GetKeyCount() const { return static_cast<int>(_count); }		// PShow_Net_* functions want an int for a count
	HC_KEY const * GetKeys() const { return &_keys[_keys.size() - _count]; }

	void SetRootKeyDepth() { _rootKeyDepth = _count + 1; }
	bool LeafIsRoot() const { return _rootKeyDepth == _count; }

	RGBColorArray const & GetEffectiveColorMap()
	{
		if (_count == 0)
			_colorMap.clear();
		else if (_cacheValid == false)
		{
			int colorMapCount;
			HC_PShow_Net_Color_Map_Count(GetKeyCount(), GetKeys(), &colorMapCount);
			_colorMap.resize(colorMapCount);
			char colorSpace[8] = "";
			HC_PShow_Net_Color_Map_By_Value(GetKeyCount(), GetKeys(), colorSpace, &colorMapCount, _colorMap.data());
			_cacheValid = true;
		}

		return _colorMap;
	}

private:
	size_t _count;
	size_t _rootKeyDepth;
	KeyArray _keys;
	bool _cacheValid;
	RGBColorArray _colorMap;
};

class HInputHandler;
class HOutputHandler;

class ProgressCallbacks
{
public:
	static void Init(HInputHandler * handler);
	static void Init(HOutputHandler * handler);
	static void Reset();

	static A3DVoid Start(A3DInt32 phase);
	static A3DVoid Size(A3DInt32 size);
	static A3DVoid Increment(A3DInt32 increment);
	static A3DVoid End();
	static A3DVoid Title(A3DUTF8Char * message);

	static A3DInt32 breakValue;

	static void ReportProgress(std::string const & message);
	static void ReportProgress(float percent);

private:
	static HInputHandler * inputHandler;
	static HOutputHandler * outputHandler;
	static A3DInt32 importPhase;
	static A3DInt32 phaseSize;
};


void quote_encoded_text(char const * encoded_text, char * out_quoted_encoded_text);
void quote_encoded_text(utf8_char const * encoded_text, char * out_quoted_encoded_text);


class HPublishExchangeExporter
{
public:
	HPublishExchangeExporter(HOutputHandler * handler);
	~HPublishExchangeExporter();

	void ReportBuildStarted();
	void ReportBuildProgress();
	void ReportBuildCompleted();


	HFileOutputResult Export(wchar_t const * file_name, HC_KEY model_key, HOutputHandlerOptions * options);
	A3DAsmModelFile * BuildPRCModel(HC_KEY model_key, HOutputHandlerOptions *options);

	A3DAsmModelFile * createModelFile();
	A3DAsmProductOccurrence * createOccurrence();
	A3DAsmPartDefinition* createPart(HC_KEY segmentkey, bool hasColor, bool lineVisibility, bool faceVisibility);
	A3DRiRepresentationItem * createRI(HC_KEY key, bool hasColor, bool lineVisibility, bool faceVisibility, bool usePointsAsTextureParameters, float const textureMatrix[16]);
	A3DRiRepresentationItem * createRiPolyWire(HC_KEY key);
	A3DRiRepresentationItem * createRiPolyWireMulti(VArray<HC_KEY> keys);
	A3DRiRepresentationItem * createRiMarkers(HC_KEY key, bool hasColor);
	A3DRiRepresentationItem * createRiPolyBrep(HC_KEY key, bool usePointsAsTextureParameters, float const textureMatrix[16]);
	A3DRiRepresentationItem * createRiBrepModel(HC_KEY key);
	A3DTess3D * createShellTesselation(HC_KEY key, bool usePointsAsTextureParameters, float const textureMatrix[16]);
	A3DTess3D * createLineTesselation(HC_KEY polylinekey);
	A3DTess3D * createLineTesselationMulti(VArray<HC_KEY> keys);
	A3DAsmProductOccurrence * createInclude(HC_KEY includeKey, bool hasColor);
	A3DAsmProductOccurrence * createOccurrenceRecursive(HC_KEY segmentkey, bool hasColor, bool isInclude);

	void SetName(A3DEntity * entity, HC_KEY key, std::string const & defaultName = "");

	bool IsEnvironmentTexture(char *texturename);
	bool FindTexture(char const * texturename, char * definition);
	A3DUns32 GetPictureIndex(HC_KEY imageKey);
	A3DUns32 GetTextureApplicationIndex(char const * textureDefinition, HC_KEY sourceImageKey);

	void SetColorAndVisibility(A3DRootBaseWithGraphics* pBase, HC_KEY segmentkey, bool hasColor,  bool isVisible, char const * colorType = "faces");

	void gatherPMI(HC_KEY segmentKey, vlist_t * pmiList);

	std::string GetCaseSensitiveFontFamilyName(char const * fontName);
	bool getTextMarkupCodes(A3DUns32Array & codes, A3DDoubleArray & coordinates, StdStringArray & utf8Strings, A3DUTF8StringArray & textStrings, HPoint * annotationPosition = nullptr);
	A3DMkpAnnotationItem * createAnnotation(HC_KEY annotKey);
	A3DTessMarkup * createAnnotationTessellation();
	A3DMkpAnnotationItem * createText(HC_KEY textKey);
	A3DTessMarkup * createTextTessellation();
	A3DMkpAnnotationItem * createPMI(HC_KEY segmentkey);
	A3DTessMarkup * createPMITessellation(PMI::Entity const * in_entity);
	A3DMkpView * createView(HC_KEY segmentkey);
	int BuildHoopsViewArray( HPEViewsArray& hoopsviews);
	int AddVisualizeCurrentView( HPEViewsArray& hoopsviews);
	A3DRiRepresentationItem* GatherLinesAndCreateRi();

	static void initPrcExportParams(A3DRWParamsExportPrcData & out_export_data, HOutputHandlerOptions * in_options = 0);

	static size_t m_lineFuseThreshold;
	HC_KEY	m_modelKey;
	HBaseView *m_pView;
	struct vlist_s *m_RI_list;
	struct vhash_s *m_texturehash;
	struct vhash_s *m_instancehash;

	KeyIndexHash m_ImageIndexHash;
	TextureSourceHash m_TextureIndexHash;

	VArray<HC_KEY> m_cadviews_key;
	struct vhash_s *m_pmihash;
	KeyPath m_keyPath;
	StringStringHash m_fontNameFamilyNameHash;

	HOutputHandler * m_OutputHandler;
	int m_TotalSegmentCount;
	int m_ProcessedSegmentCount;
	KeyCountHash m_KeySegmentCountHash;

	HOutputHandlerOptions * m_OutputOptions;
};

class HUtilityXMLTag;


typedef VHash<A3DAsmProductOccurrence *, HC_KEY> POccurrenceKeyHash;
typedef VArray<char const *> StringArray;

void parseGraphicsData( const A3DEntity* ph3dx_entity, 
					   A3DUns32* puiLayerIndex,
					   A3DUns32* puiStyleIndex,
					   A3DUns16* pusBehaviour);
class HPublishExchangeEntityReferenceManager;

class HPublishExchangeImporter
{
public:
	HPublishExchangeImporter(HInputHandler * handler);
	virtual ~HPublishExchangeImporter();
	virtual HFileInputResult Import(const wchar_t * FileName, HC_KEY modelKey, HInputHandlerOptions * pInputOptions = 0);

protected:
	bool WriteConfigFile ( wchar_t const * filename, int config_selection = 0 );

	HFileInputResult LoadFile(const wchar_t * FileName, HInputHandlerOptions * pInputOptions);

	void ReportParseStarted();
	void ReportParseProgress();
	void ReportParseCompleted();

	A3DInt32 CreateAndPushCascadedAttributesFace(const A3DRiRepresentationItem* pRepItem,
											const A3DTessBase* pTessBase,
											const A3DTessFaceData* psTessFaceData,
											A3DUns32 uiFaceIndex,
											const A3DMiscCascadedAttributes* pFatherAttr,
											A3DMiscCascadedAttributes** ppAttr,
											A3DMiscCascadedAttributesData* psAttrData);

	A3DInt32 CreateAndPushCascadedAttributes(const A3DRootBaseWithGraphics* pBase,
										const A3DMiscCascadedAttributes* pFatherAttr,
										A3DMiscCascadedAttributes** ppAttr,
										A3DMiscCascadedAttributesData* psAttrData);

	void parseRiSet(A3DRiSet* p, A3DMiscCascadedAttributes* pFatherAttr, double modelscale);

	void parseAttributes(const A3DEntity* pEntity);

 
	void BuildShell(A3DTess3D* p, A3DRiRepresentationItem* pRepItem, A3DMiscCascadedAttributes* pFatherAttr, A3DTessBaseData *tessbasedata, double modelscale);
	void BuildWires(A3DTess3D* p, A3DRiRepresentationItem* pRepItem, A3DMiscCascadedAttributes* pFatherAttr, A3DTessBaseData *tessbasedata);
	void BuildPolyWires(A3DTess3D* p, A3DRiRepresentationItem* pRepItem, A3DMiscCascadedAttributes* pFatherAttr, A3DTessBaseData *tessbasedata);
	void BuildMarkup(A3DTess3D* p, A3DTessBaseData *tessbasedata);

	void getMarkupTesselation(const A3DTessBaseData *psTessBaseData, const A3DTessMarkupData *psTessMarkupData,
			PMIPolylineArray & out_polylines, PMIPolygonArray & out_polygones, PMIStringArray & out_strings, PMITextAttributesArray & out_text_attributes, PMI::Options* out_pmi_options = NULL);


	void parseTess(A3DTessBase* p, A3DRiRepresentationItem* pRepItem, 
				  A3DMiscCascadedAttributes* pFatherAttr, double modelscale);

	void parseBrepData(A3DTopoBrepData* p);
	void parseConnex(A3DTopoConnex* p,A3DTopoBrepData* p2);
	void parseShell(A3DTopoShell* p, bool invert = false);
	void parseFace(A3DTopoFace* p);
	void parseLoop(A3DTopoLoop* p);
	void parseCoEdge(A3DTopoCoEdge* p);
	void parseEdge(A3DTopoEdge* p);
	void parseVertex(A3DTopoVertex* p);
	void parseMultipleVertex(A3DTopoMultipleVertex* p);
	void parseUniqueVertex(A3DTopoUniqueVertex* p);
	void parseBody(A3DTopoBody* p);
	void parseContext(A3DTopoContext* p);
	void SetMaterial(A3DMiscCascadedAttributesData *attr);
	H_UTF8 GetLinePattern(A3DUns32 index);
	void SetLinePattern(A3DGraphStyleData const * styleData);
	void SetGraphicsAttributes( A3DEntity *pEntity, HC_KEY key);
	void setName(const A3DRootBase* pRootBase, H_UTF8 utf8_defaultname = "node");

	void parseRiBrepModel(A3DRiBrepModel* p);
	void parseRiCSys(A3DRiCoordinateSystem* p);
	void parseRiCurve(A3DRiCurve* p);
	void parseRiPointSet(A3DRiPointSet* p);

	void parseRI(A3DRiRepresentationItem* p, A3DMiscCascadedAttributes* pFatherAttr, double modelscale);
	void parsePart(A3DAsmPartDefinition* p, A3DMiscCascadedAttributes* pFatherAttr, double modelscale);

	A3DInt32 productOccurrenceGetExternalData(A3DAsmProductOccurrenceData const * psPOccData,
											 A3DAsmProductOccurrence *& pExternalData);
	A3DInt32 productOccurrenceGetLocation(A3DAsmProductOccurrenceData const * psPOccData,
										 A3DMiscCartesianTransformation *& pLocation);
	bool prototypeGetPartRecursive(A3DAsmProductOccurrence * prototype, 
								   A3DMiscCascadedAttributes * pAttr, 
								   double modelscale,
								   bool use_external_data);
	void productOccurrenceGetPart(A3DAsmProductOccurrenceData const * psPOccData, 
								  A3DMiscCascadedAttributes * pAttr, 
								  double modelscale);
	void productOccurrenceGetPMI( const A3DAsmProductOccurrenceData& sPOccData, HC_KEY pocc_seg_key, HPublishExchangeEntityReferenceManager* psEntityReferenceManager);
	bool prototypeGetOccurrencesRecursive(A3DAsmProductOccurrence * prototype,
										  A3DMiscCascadedAttributes * pAttr,
										  double modelscale);
	void productOccurrenceGetChildren(A3DAsmProductOccurrenceData const * psPOccData,
									  A3DMiscCascadedAttributes * pAttr,
									  double modelscale);

	void parsePOccurrence(A3DAsmProductOccurrence * p, 
						  A3DMiscCascadedAttributes * pFatherAttr, 
						  double modelscale);						

	void parseModelFile(A3DAsmModelFile * p);

	void traverseAnnotationReference(const A3DMkpAnnotationReference *pAnnot);
	void SetMatrix(A3DMiscTransformation *trans);
	void SetMatrix2(A3DMiscCartesianTransformationData trans);
	void traverseAnnotation(const A3DMkpAnnotationEntity *pAnnot);
	void traverseAnnotationItem(const A3DMkpAnnotationItem *pAnnot);
	void traverseAnnotationSet(const A3DMkpAnnotationSet *pAnnot);
	void getLeaderLinesAndSymbols(const A3DMkpLeader *pMarkup, PMIPolylineArray & out_leader_lines, PMIPolygonArray & out_leader_symbols);
	void traverseMarkup(const A3DMkpMarkup *pMarkup, A3DMiscCascadedAttributesData* psAttribData = NULL, HC_KEY* pmarkup_key = NULL);
	void PopulateTextures();

	int parseRootData( A3DEntity* ph3dx_entity, H_UTF8* pname, A3DUns32* puiPersistentId = NULL, A3DUns32* puiNonPersistentId = NULL);
	//void parseGraphicsData( A3DEntity* ph3dx_entity,  A3DUns32* puiLayerIndex = NULL, A3DUns32* puiStyleIndex = NULL, A3DUns16* pusBehaviour = NULL);
	void traverseView(const A3DMkpView *pView);

	void parseAnnotation( const A3DMkpAnnotationEntity* pAnnotation, A3DMiscCascadedAttributes* pAttr);
	void parseAnnotations( A3DMkpAnnotationEntity** ppAnnotations, A3DUns32 uiAnnotationsSize);

	/*Capture management*/
	void parseCaptures( HC_KEY pview_poowner_key, A3DMkpView** pph3dx_view, A3DUns32 count);
	void parseCapture( A3DMkpView* ph3dx_view, HC_KEY pview_owner_key);
	void add_default_view_condition_style( HC_KEY pview_poowner_key, A3DMkpView** pph3dx_view, A3DUns32 count);
	void add_default_view_condition_style_in_sub_levels( HC_KEY poo_owner_key);
	/*annotation items let you redefine visibility, position, graphics atttributes to any entity under product occurence view owner*/
	void traverseLinkedItems( const A3DMkpViewData& h3dx_viewdata, const H_UTF8& view_name);
	/*default view management*/
	void get_array_of_pmi_visible( const A3DMkpAnnotationEntity *pAnnot, A3DMiscCascadedAttributes * pFatherAttr, VArray<void*>& array_of_markups);
	int get_array_of_pmi_visible( A3DAsmProductOccurrence * p, A3DMiscCascadedAttributes * pFatherAttr, VArray<void*>& array_of_markups);
	int ComputeCADDefaultOrientation(HC_KEY poo_key, HC_KEY layer_key);
	int get_po_views( A3DAsmProductOccurrence * po, VArray<void*>& view_array);
	int build_default_view( A3DAsmProductOccurrence * p, HC_KEY pview_poowner_key);
	/*Treatment of camera, cutting plane*/
	void traverseSceneDisplayParameters( A3DMkpView* ph3dx_view, const A3DMkpViewData& h3dx_viewdata, HC_KEY pview_poowner_key, const H_UTF8& view_name);
	int parseCamera( A3DGraphCamera* m_pCamera, HC_KEY pview_poowner_key, HC_KEY layer_key);
	void computeCamera( HC_KEY pview_poowner_key, const A3DMkpViewData& h3dx_viewdata, const H_UTF8& view_name, HC_KEY layer_key);
	int parseClippingPlane( HC_KEY pocc_seg_key, A3DSurfPlane* pClippingPlane, const char *view_name);

	/*traverse annotation, to take in account pmi associated to the view*/
	void traverseAnnotations( const A3DMkpViewData& h3dx_viewdata, const H_UTF8& view_name, const HC_KEY& pview_owner_key, A3DUns16 usBehaviour);
	void includeMarkupAnnotationItemInView(const A3DMkpAnnotationItem *pAnnot, const char * view_name, bool is_default_view);
	void includeMarkupAnnotationSetInView(const A3DMkpAnnotationSet *pAnnot, const char * view_name, bool is_default_view);
	void includeMarkupAnnotationInView(const A3DMkpAnnotationEntity *pAnnot, const char * view_name, bool is_default_view);
	void treatReference(A3DEntity* pEntity, const A3DMkpMarkup *pMarkup);
	void treatReferenceOnTopology(A3DEntity* pEntity,const A3DMkpMarkup *pMarkup);
	int GetDLLVersion() const ;

	HIOConnector *			m_pConnector;
	HIOConnector *			m_pPMIConnector;
	HIOConnector3DX	*		m_pConnectivityInfo;
	HC_KEY					m_modelKey;
	HBaseView*				m_pView;
	HC_KEY					m_markup_segment;
	HC_KEY					m_view_style_hide;
	HC_KEY					m_view_style_show;
	HC_KEY					m_current_pocc_seg_key;
	wchar_t *				m_fileName;
	POccurrenceKeyHash		m_part_key_hash;
	POccurrenceKeyHash		m_dead_part_hash;
	POccurrenceKeyHash		m_external_part_key_hash;
	POccurrenceKeyHash		m_dead_external_part_hash;
	POccurrenceKeyHash		m_poccurrence_key_hash;
	POccurrenceKeyHash		m_dead_poccurrence_hash;
	PMIEntityArray			m_pmi_entities;
	struct vhash_s *		m_annotationhash;
	struct vlist_s *		m_brepfacelist;
	BREP_Edge_Matcher *		m_edgematcher;
	BREP_Topology *			m_TopologyManager;
	bool					m_bMergeFaces;
	bool					m_bCalculatePhysicalProperties;
	int						m_AnnotationCounter;
	bool					m_bHasBrepStructure;
	A3DTopoFace **			m_PartFaces;
	vlist_t		 *			m_PartFaceEdges;
	int						m_PartFaceNum;
	double					m_body_scale;
 	StringArray				m_name_array;
	StringArray				m_type_array;
	int						m_default_entry_index;
	bool					m_store_user_attributes;
	double					m_cadmodel_unit;
	void**					m_PartTessFaces;
	bool					m_use_prc_active_view_as_defaultview;
	bool					m_bImportHiddenObjects;
	bool					m_bImportNonBrepFaces;
	A3DEModellerType		m_FileType;
	float					m_NormalAngleCosine;
	HInputHandler *			m_InputHandler;
	A3DUns32				m_TotalPoccCount;
	A3DUns32				m_ProcessedPoccCount;
	EntityCountHash			m_EntityPOccCountHash;
};


class HPublishExchangeCommon
{
public:
	static A3DInt32 SetLicense(const char *pCustomerKey);
	static bool RuntimeLinkExchangePublish();
};



#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif
