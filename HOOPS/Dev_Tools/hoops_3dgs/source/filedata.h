// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef FILEDATA_H_DEFINED
#define FILEDATA_H_DEFINED

Begin_HOOPS_Namespace

#define			FILE_BLOCK_SIZE			1024

#define			FILE_For_Reading		(int)0
#define			FILE_For_Writing		(int)1

#define			DIR_Select_Readable		(int)0x01
#define			DIR_Select_Writeable	(int)0x02
#define			DIR_Select_Executable	(int)0x04
#define			DIR_Select_File			(int)0x08
#define			DIR_Select_Directory	(int)0x10

#define			FILE_Warn_Once_FLOAT_ERROR 0x1

struct File_Data {
	void *			fhandle;
	int				current_buffer;
	char *			bp;
	char *			bpe;
	char *			back_bp;
	char *			back_bpe;
	char const *	message;
	char const *	message2;
	Name			filename;
	int				message_count;
	int				user_data;
	int				indent;
	int				line_size;
	int				line_position;
	int				line_number;
	int				warnings_given;
	float			version;
	bool			eof;
	bool			back_eof;
	bool			for_writing;
	bool			fatal_error;
	bool			recovering;
	bool			forced_comment;

	// Waste one byte so that &buffer[0][FILE_BLOCK_SIZE] is legal. One more to make even.
	char			buffer[2][FILE_BLOCK_SIZE + 2];

	char			sprintf_buffer[SPRINTF_BUFFER_SIZE];
};

#define END_OF_FILE(fdata) ((fdata)->bpe == (fdata)->bp && (fdata)->eof)

End_HOOPS_Namespace

#endif
