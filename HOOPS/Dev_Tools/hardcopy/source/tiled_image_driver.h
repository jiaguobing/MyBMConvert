
#ifndef __tiled_image_driver_h
#define __tiled_image_driver_h

#include <stdio.h> 
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "hc.h" 

namespace hardcopy {
struct HardCopyTile;
class HardCopy;

/*! \private */
class HardCopyTiledImageDriver
{
friend class HardCopy;

private:
	unsigned char * m_image;
	mutable unsigned char * m_band;
	HC_KEY m_hiddenSegKey;

	int m_bandfd;
	mutable unsigned char * m_line;
	int y_bands, x_bands;
	int one_y_step;
	int one_x_step;
	int m_blockCount;
	int m_width, m_height;

	int BPP;
	HC_KEY m_driverKey;
	int m_stage_block;
	int * m_blockFiles;
	int m_fd;
	char * m_imageName;
	char * m_textureName;
	struct HardCopyTile * m_tiles;
	bool m_error;

	int * m_maxTextureSize;
	bool m_dwf_2d_export;
	
	bool MergeBlockRows(int * blockFiles);
	HardCopyTiledImageDriver const & operator = (HardCopyTiledImageDriver const &);
	void SetWindowColor(hardcopy::HardCopy const * hc, int x, int y);
	void DefineTexture();
	void UnDefineTexture();
	int UpdateTiledDisplay(hardcopy::HardCopy const * hc, bool one_shot = false);
	void const * TiledImageRow(int row) const;

public:
	HardCopyTiledImageDriver(double width, double height, double dpi, HC_KEY model_seg, char const * hsra, int * maxTextureSize, bool dwf_2d_export, char const * driver = 0, int fd = -1);
	~HardCopyTiledImageDriver();
	void const * ImageRow(int row) const;
	int Height() const;
	int Width() const;
	int BytesPerPixel() const;
	int UpdateDisplay(hardcopy::HardCopy const * hc, bool one_shot = false);
	HC_KEY GetDriverKey();
	char const * GetImageName() const;
	bool error() const;
};
};
#endif

