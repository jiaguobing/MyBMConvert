#ifndef ACCESS_H
#define ACCESS_H

#include "dwf/XAML/OpcResourceMaterializer.h"
#include "dwf/XAML/OpcResourceSerializer.h"
#include "dwf/package/Resource.h"
#include "dwf/package/utility/ResourceContainer.h"

#ifndef DWFIO_SUPPORT_ONLY_LEGACY_VERSIONS
class Access : public WT_OpcResourceMaterializer, public WT_OpcResourceSerializer
{
public:
	Access(DWFToolkit::DWFResource & rRes, DWFToolkit::DWFResourceContainer & rCont)
		: _rResources(rCont), _rFixedPage(rRes) {}
	virtual ~Access() {}

	// WT_OpcResourceMaterializer
	WT_Result getPartInputStream(const DWFCore::DWFString & rzPath, DWFCore::DWFInputStream ** ppStream);

	// WT_OpcResourceSerializer
	WT_Result getPartOutputStream(const DWFCore::DWFString & rzMimeType, DWFCore::DWFString & rzPath, DWFCore::DWFOutputStream ** ppStream);

private:
	// The default initial buffer size for buffer output streams wrapping file output streams.
	static const size_t					_DEF_BUFFER_SIZE;

	DWFToolkit::DWFResourceContainer &	_rResources;
	DWFToolkit::DWFResource &           _rFixedPage;

	Access & operator =(Access&) {};
};
#endif

#endif //ACCESS_H