// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

/* Note: always add exactly 0.01 to the version number for a normal
 *		 version-number 'bump'. We *were* adding 0.1 in the normal
 *		 case, but that was going to cause the numbers to run out too
 *		 soon.
 */

#ifndef METAFILE_H_DEFINED
#define METAFILE_H_DEFINED

Begin_HOOPS_Namespace

#define METAFILE_VERSION  1.25f  /* (the code assumes 2 fractional digits) */

#define DEFAULT_POINT_ARRAY_SIZE		100
#define DEFAULT_INT_ARRAY_SIZE			100
#define DEFAULT_BYTE_ARRAY_SIZE			1000
#define DEFAULT_FLOAT_ARRAY_SIZE		100

struct Read_In_Progress {
	Read_In_Progress *	next;

#	define		RIP_OUTERMOST	0
#	define		RIP_SEGMENT		1
#	define		RIP_FRONT		2
#	define		RIP_RENUMBER	3
#	define		RIP_GEOMETRY	4
#	define		RIP_FACE		5
#	define		RIP_EDGE		6
#	define		RIP_VERTEX		7
#	define		RIP_LOD			8
#	define		RIP_TRIM		9
#	define		RIP_PRIORITY	10
	int			type;
};

#define SMALL_SLEN		1024		// initial siz
#define BIG_SLEN		32768		// limit to doubling and amount added after that

struct Read_Metafile_Data {
	Thread_Data *	thread_data;
	Read_In_Progress *		read_in_progress;
	Read_In_Progress		outermost_read_in_progress;

	int					version;

	int					restore_state;

	Key					new_key;
	Key					last_key;

	Name				open_segment;
	Name				metafile_root;

	File_Data *			file_data;

	Point				default_point_array0[DEFAULT_POINT_ARRAY_SIZE];
	int					point_array_allocated0;
	int					point_array_count0;
	Point *				point_array0;

	DPoint				default_dpoint_array0[DEFAULT_POINT_ARRAY_SIZE];
	int					dpoint_array_allocated0;
	int					dpoint_array_count0;
	DPoint *			dpoint_array0;

	int					default_int_array0[DEFAULT_INT_ARRAY_SIZE];
	int					int_array_allocated0;
	int					int_array_count0;
	int *				int_array0;

	int					default_int_array1[DEFAULT_INT_ARRAY_SIZE];
	int					int_array_allocated1;
	int					int_array_count1;
	int *				int_array1;

	char				default_byte_array0[DEFAULT_BYTE_ARRAY_SIZE];
	int					byte_array_allocated0;
	int					byte_array_count0;
	char *				byte_array0;

	float				default_float_array0[DEFAULT_FLOAT_ARRAY_SIZE];
	int					float_array_allocated0;
	int					float_array_count0;
	float *				float_array0;

	double				default_double_array0[DEFAULT_FLOAT_ARRAY_SIZE];
	int					double_array_allocated0;
	int					double_array_count0;
	double *			double_array0;

	Plane				default_plane_array0[DEFAULT_POINT_ARRAY_SIZE];
	int					plane_array_allocated0;
	int					plane_array_count0;
	Plane *				plane_array0;

	DPlane				default_dplane_array0[DEFAULT_POINT_ARRAY_SIZE];
	int					dplane_array_allocated0;
	int					dplane_array_count0;
	DPlane *			dplane_array0;

	// a mesh uses a point-array slot; array_count == rows * columns
	int					mesh_rows0;
	int					mesh_columns0;

	// put these in here so they're on the heap
	char *				small_s0;
	char *				small_s1;
	char *				small_s2;
	char *				big_s0;
	char *				big_s1; 

	int					ss0_len; 
	int					ss1_len; 
	int					ss2_len;					
	int					bs0_len; 
	int					bs1_len;

	// and put these in here to forestall compiler complaints about unused variables. Usage is purely local to Parse_Command.
	double				da0[4][4];
	double				d0, d1, d2, d3, d4, d5;
	float				a0[4][4];
	Point *				p0;
	Point *				p1;
	Point *				p2;
	DPoint *			dp0;
	DPoint *			dp1;
	DPoint *			dp2;
	int					i0, i1, i2, i3;
	long				l0, l1;
	float				f0, f1, f2, f3, f4, f5;
	Point				c0, c1, c2;
	DPoint				dc0, dc1, dc2;
	void *				image_rows;
	int					width, height;
};

End_HOOPS_Namespace

#endif

