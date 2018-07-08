
#ifndef __hardcopy_h
#define __hardcopy_h

#include <hc.h>
#include <vector>
#include <cstring>


/*! \mainpage
 * HardCopy is an extensible class for outputting hardcopy versions of your model.  It can be used as is or extended
 * by deriving from it and redefining the virtual methods.
 * 
 * The HardCopy class has one purpose, to reproduce your scene as exactly as possible on paper (or PDF, PostScript,
 * etc).  If you don't want to reproduce your scene exactly, you'll need to derive from HardCopy and override the
 * virtual methods.
 *
 * Hardcopy works by rendering the faces in your model to an image in one pass, then doing and hidden line rendering
 * over that image in the second pass.  Text, lines, edges, curves and other vector data are drawn in the second pass.
 * This results in output which can be efficiently handled by a printer.
 * 
 * The background image, generated in the first pass, can be extremely large.  For example, 8.5x11 at 600 dpi results
 * in a 128 MB image.  HOOPS can't render an image that large, so HardCopy does the difficult work of dividing that
 * image up into smaller parts which HOOPS can handle.  The smaller parts are compressed, written to temporary files,
 * then reassembled and cropped before being sent to the output file.  This enables HardCopy to make, given enough
 * time, arbitrarily large output files.
 *
 * Example Usage:
 *
 * Output an 8.5 by 11, 600 dpi pdf file
 * \code
 * HardCopy print_job(8.5, 11, 600);
 * print_job.Input(my_view_key);
 * print_job.Output("/drivers/pdf/output1.pdf");
 * \endcode
 *
 *
 * Output an 8.5 by 11, 1200 dpi pdf file and report the progress
 * \code
 * HardCopy print_job(8.5, 11, 1200);
 * print_job.Input(my_view_key);
 * while ( print_job.Spool() ) {
 *     ReportProgress( print_job.Progress() );
 * }
 * print_job.Output("/drivers/pdf/output1.pdf");
 * \endcode
 *
 * \note
 * You'll almost always want to derive from HardCopy and redefine DriverHint.  DriverHint suggests
 * a driver to use for generating the background image.  HardCopy::DriverHint returns "image" which
 * is reliable, always available, but slow.  In your version of DriverHint, you can return anything.
 * If what you return isn't valid, HardCopy will automatically try to pick from one of the available
 * drivers.
 */


namespace hardcopy {

class HardCopyTiledImageDriver;

enum STAGES
{
	ST_PREPASS = 0,
	ST_START_REAL_PASS,
	ST_REAL_PASS,
	ST_COMPLETE
};

struct subscreen
{
	subscreen();
	double x0;
	double x1;
	double y0;
	double y1;
};

class HardCopy
{
friend class HardCopyTiledImageDriver;

protected:
	HardCopyTiledImageDriver * m_image; ///< \private
	struct subscreen m_subscreen;		///< If you don't want to fit the whole page, modify this subscreen
	char const * m_hint;				///< Driver used during image pass, if available
	HC_KEY m_MyScene;					///< Segment containing the scene to print
	HC_KEY m_MySceneInclude;			///< Include of m_MyScene into the printer driver segment

	char m_SrcHsra[64];					///< Source driver HSRA
	double m_PixelRatio;				///< Source pixel ratio. This value is computed in HardCopy::Input
	double m_SrcWidth;					///< Input width (inches)
	double m_SrcHeight;					///< Input height (inches)
	double m_SrcDPI;					///< Input resolution (DPI)
	
	double m_DestWidth;					///< Output width (inches)
	double m_DestHeight;				///< Output height (inches)
	double m_dpi;						///< Output resolution (DPI)

	double m_work;						///< Percentage complete. Accessible through HardCopy::Progress
	int m_stage;						///< The stage HardCopy::Spool is in
	int m_updates;						///< Number of updates necessary to complete the image pass
	bool m_wysiwyg;						///< If false, use the whole printable area. If true, strictly print ONLY what is visible in the source driver.		

	virtual void Copy(HC_KEY src);
	virtual void BackgroundOptions();
	virtual char const * DriverHint() const;
	virtual void DriverSetup();
	virtual void PrePass();
	virtual void StartRealPass();
	virtual void RealPass();
	virtual void AdjustSubscreen();
	virtual void AdjustWindow(double x0, double x1, double y0, double y1) const;
	virtual void AdjustFonts();
	virtual void ResetFonts();


public:
	HardCopy(double width, double height, double dpi, bool wysiwyg, bool exporting_2d_dwf);
	virtual ~HardCopy();

	virtual void SetDriverHint(char const * hint);
	virtual bool Input(HC_KEY driver);
	virtual bool Output(HC_KEY driver);
	virtual bool OutputPage();
	virtual bool Spool(bool one_shot = false);
	virtual double Progress() const;

private:
	int m_maxTextureSize;
	bool m_exporting_2d_dwf;	

	struct FontInformation
	{
		FontInformation(HC_KEY segmentKey, float size, char * units);

		HC_KEY			m_SegmentKey;
		float			m_Size;
		char			m_Units[64];
	};

	std::vector<FontInformation> fontInfo;
};

};
#ifndef countof
# define countof(x) (sizeof(x)/sizeof(x[0]))
#endif

#endif









