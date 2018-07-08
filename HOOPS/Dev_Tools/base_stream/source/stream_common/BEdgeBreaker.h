// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

 /*
 *
 *
 * BEdgeBreaker.h
 *
 * This file describes the public interface to the edgebreaker module, plus some internal 
 * definitions that are shared between one or more edgebreaker source code files
 * 
 * Some notes about the interface:
 * in_out
 *   Parameters with names ending in "in_out" are used for both input and output.
 *   On the way in they say how much was allocated.  On the way out, they say
 *   how much was used.
 * faces
 *   The following is a description of the HOOPS face list (taken from the
 *   standard HOOPS reference manual)...
 *   "The face_list is an array of integers. The first integer is the
 *    number of vertices that should be connected to form the first face. For 
 *    example, "3" for a triangle. The next three integers (in this example) 
 *    are the offsets into the points array at which the three x-y-z's can 
 *    be found. The first point in the points array is considered to be at 
 *    offset zero, so "0, 1, 2" in the face_list array would form the triangle 
 *    from the first three entries in points."
 *   a face list by tristrips is (again from the HOOPS 3dgs reference manual)
 *   "an array of integers that describes the triangle strips. It is a list of an 
 *    unlimited number of independent triangle strips placed contiguously in memory, 
 *    one after another. Each triangle strip begins with the number of vertices 
 *    referenced, and then the list of vertices. Vertices 0-2 form one triangle. 
 *    For every n such that 2 < n <= number of vertices, a new triangle is introduced. 
 *    This new triangle will be [n-2, n-1, n] for even n, and [n-1, n-2, n] for odd n 
 *    (to maintain consistent handedness of the triangles). Degenerate triangles 
 *    (triangles which reference the same vertex more than once) are allowed. The 
 *    length of this array should be given as a number of integers, the number of 
 *    triangles plus 3 per independent triangle strip. "
 * pointmap
 *   Vertex id's, sorted by order of appearance in the compressed stream.  This
 *   array is needed for transmitting vertex attributes (e.g. normals or texture
 *   coordinates) in parallel with their locations.  For example:
 *   for( i = 0 ; i < pointmap_len ; i++ ) {
 *      *out++ = texcoords[ pointmap[i] ].u;
 *	*out++ = texcoords[ pointmap[i] ].v;
 *   }
 * pcount
 *   the **number** of xyz triplets in the array of vertex locations.
 * flen
 *   the number of integers in the face list
 */
#ifndef __BEDGEBREAKER_H__
#define __BEDGEBREAKER_H__

#ifndef BSTREAM_DISABLE_EDGEBREAKER


#define EB_DEFAULT (-1)

typedef struct ET_Bounding_TAG {
    float x1, y1, z1;
    float x2, y2, z2;
} ET_Bounding;

#define HINT_ONE_PIECE		    0x0001
#define HINT_WATERTIGHT		    0x0002
#define HINT_MANIFOLD		    0x0004
#define HINT_NO_HANDLES		    0x0008
#define HINT_NO_BACKWARDS_OR_HOLES  0x0010
#define HINT_INPUT_BY_TRISTRIPS	    0x0020

#define STATUS_ERROR		0
#define STATUS_NORMAL		1
#define STATUS_COMPLETE		2
#define STATUS_WATERTIGHT	3
#define STATUS_TRY_AGAIN	4


/* Configurations for the compression function. */
typedef struct eb_compress_configs_TAG {
    /* number of bits of precision in compressed vertex locations.  11 by default. */
    int x_quantization, y_quantization, z_quantization;
    /* number of bits of precision in compressed vertex normals.  11 by default. */
    int x_quantization_normals, y_quantization_normals, z_quantization_normals;
    /* factor by which we allow the point count to grow to absorb any unclean data
     * (open edges, backwards faces, non-manifold surfaces, etc.).  Default value
     * is DEFAULT_POINT_FACTOR (see end of this file) -- a value of at least
     * 1.5 is recommended.  Compress function will return falure (0) if the 
     * specified value is insufficient. */
    float point_factor;		
    /* axis-aligned bounding volume of the surfaces' vertex locations.  We will calculate 
     * and transmit our own bounding volume if this pointer is set to NULL.  Because of 
     * speed and compression ratio considerations, the bounding box should be set if
     * (but only if) it is conveniently available from elsewhere, and also available 
     * on the decompression side. */
    ET_Bounding *bounding;
    /* As we make changes to the format of the edgebreaker stream, this variable will
     * allow us to write streams that are backward compatible with older readers.
     * Synchronized with version of BStream (the results of tk.GetTargetVersion()) */
    int target_version;
    int hints;
} eb_compress_configs;

extern int show_edgebreaker_compress_size( 
			int pcount, 
			int flen, 
			int const *face_data, 
			int *stream_len_out,
			int *pointmap_count_out,
			//Configurations. Pass NULL for all default values 
			eb_compress_configs const *configs
			);
extern int edgebreaker_compress( 
			int pcount, 
			float const *points, 
			float const *normals, 
			int flen, 
			//Standard HOOPS shell face list.  See comment at top. 
			int const *fdata_in,		
			int *stream_len_in_out,
			//The compressed data  
			void *stream_out,		
			int *pointmap_len_in_out,
			//See comment at top of file. Can be NULL if not interested 
			int *pointmap_out,		
			//Configurations. Pass NULL for all default values 
			eb_compress_configs const *configs	
			);

typedef struct eb_decompress_configs_TAG {
    /* Should match what was passed into compress. */			
    ET_Bounding const *bounding;
} eb_decompress_configs;
extern int show_edgebreaker_decompress_size( 
			int stream_len, 
			void const *stream, 
			int *pcount_out, 
			int *normal_count_out, 
			int *flen_out 
			);
extern int edgebreaker_decompress( 
			int stream_len, 
			//The compressed data 
			void const *stream,		
			int *pcount_in_out, 
			float *points_out, 
			float *normals_out, 
                        //specifies how to interpret the following output, "faces_out" 
                        bool *by_tristrips_out,
                        //the length of the faces_out array
			int *flen_in_out,
			//Standard HOOPS shell face list, or face list by tristrips.  See comment at top. 	
			int *faces_out,
			//Should match what was passed into compress. 			
			eb_decompress_configs const *configs		
			);


/* 
 *   END PUBLIC INTERFACE
 */


/* 
 *        BEGIN PRIVATE DECLARATIONS -- INTERNAL USE ONLY
 */

    /* 
     * type definitions 
     */

/* edgebreaker header as of header scheme 0 */
typedef struct {
    char scheme;    
    char mtable_scheme;
    char points_scheme;
    char normals_scheme; 
    int opslen;	    
    int mtablelen;  
    int pointslen;  
    int pcount;	    
} edgebreaker_header0;

/* edgebreaker header as of header scheme 1 */
typedef struct {
    edgebreaker_header0 v0;
    int normalslen;
} edgebreaker_header1;

/* edgebreaker_header
 * Describes the encoding schemes and lengths for the connectivity and point location data.
 * The intent is for this entire module to convert a surface into "blind data", meaning
 * that the calling function knows nothing of its internals -- not even how to split connectivity
 * from point locations.
 * This stuff actually goes out to the file, so anything added has to go at the end!!!! */
typedef struct EDGEBREAKER_HEADER_TAG {
    char scheme;    /* identify the coding scheme so that we may introduce variants later (read: upgradable) */
    char mtable_scheme;
    char points_scheme;
    char normals_scheme; 
    int opslen;	    /* length of opcodes segment, in bytes */
    int mtablelen;  /* M operations need an auxiliary int to say how many vertices in the interior loop */
    int pointslen;  /* length of the points section of the stream */
    int pcount;	    /* number of points that will need to be allocated on the decompression side */
    int normalslen;
} edgebreaker_header;


#define MTABLE_HAS_LENGTHS	            0x1
#define MTABLE_HAS_M2STACKOFFSETS           0x2
#define MTABLE_HAS_M2GATEOFFSETS            0x4
#define MTABLE_HAS_DUMMIES	            0x8
#define MTABLE_HAS_PATCHES	            0x10
#define MTABLE_HAS_BOUNDING	            0x20
#define MTABLE_HAS_QUANTIZATION	            0x40
#define MTABLE_HAS_QUANTIZATION_NORMALS     0x80
/* The M table is used to handle interior loops in a surface.  Interior loops
 * are classified as one of two cases:  1) loops that were pre-existing in the surface
 * (possibly the result of a backwards face that was treated as an independent component)
 * and 2) loops that were created during a CASE S operation that split a bounding loop into 2.
 * The first case is handled by a CASE M operation, and only the mlength is needed.
 * The second case is handled by CASE M2, and requires all 3 pieces of info.
 * CASE S operations segment a surface unless, topologically, the model contains some 
 * kind of handles (e.g. a donut). because of the M table, CASE M and CASE M2 are expensive 
 * to encode, but we assume that they will be few in number relative to the overall model. */
typedef struct {
    int flags;
    /* mlengths, used for both CASE_M and CASE_M2 operations, indicate the 
     * length of the interior loops that are encountered. */
    int *mlengths;
    int mlengths_used;
    int mlengths_allocated;
    /* m2stackoffsets indicate how many steps from the top of the stack we have
     * to go back to find the start of the loop we just encountered  */
    int *m2stackoffsets;
    int m2stackoffsets_used;
    int m2stackoffsets_allocated;
    /* m2gateoffsets indicate the number of links to travel along the 
     * s loop to find the contact vertex (cannot be deduced from other info) */
    int *m2gateoffsets;
    int m2gateoffsets_used;
    int m2gateoffsets_allocated;
    /* Fake vertices are used to plug holes and to make geometry simpler and more robust.
     * The dummies array keeps track of those vertices so that we can eliminate them (and
     * any triangles that reference them) later. */
    int *dummies;
    int dummies_used;
    int dummies_allocated;
    /* non-manifold vertices have to be split.  The patches array repairs the splits. */
    int *patches;
    int patches_used;
    int patches_allocated;
    /* xyz-aligned bounding info for the points */
    ET_Bounding *bounding;
    /* number of bits precision in each direction for vertex locations */
    int x_quantization;
    int y_quantization;
    int z_quantization;
    /* number of bits precision in each direction for vertex normals (target_version >= 651)*/
    int x_quantization_normals;
    int y_quantization_normals;
    int z_quantization_normals;
} mtable_info;


/* The half-edge data structure contains one directed edge.
 * There will usually be two half-edges that reference any 
 * given two connected vertices.  Half edges are a
 * fully-connected data structure in that they allow triangles
 * to know about their neighbors. Note that everything is an index
 * into so that arrays may be relocated for reallocation */
typedef struct HALF_EDGE_TAG {
    int start;
    int twin;
} half_edge;

/* The half-edge array contains our representation of the entire surface. */
typedef struct {
    half_edge *edges;
    int allocated;
    int used;
    int *visitations;  /* in case we need to keep track of the order of visitation */
    /* no visitations_allocated member -- we just allocate 1 int per edge for simplicity */
    int visitations_used;
} half_edge_array;

typedef struct {
    int *data;
    int allocated;
    int used;
} int_stack;

typedef struct {
    unsigned int *data; /* output payload */
    unsigned int *rdata;/* input read data */
    int allocated;	/* amount of data allocated */
    int used;		/* current int being written */
    int bit;		/* current bit getting written */
    int rused;		/* next int to be read */
    int rbit;		/* next bit to be read */
    int can_reallocate; /* if we don't own the pointer, we can't reallocate it */
    int status;		/* a flag indicating an error condition */
    /* precalculated values to efficiently mask reads */
    unsigned int mask[33]; 
    /* precalculated values maximum possible value given bit length  */
    unsigned int range[33]; 
} varstream;


typedef struct {
    int *loops;          /* for each vertex, the loop of edges that it is a part of. */
	int loops_allocated;
    int *loops_edges;    /* for each loop, except those created by s ops, the id of one of its. First entry left
                          * blank because loop id 0 is reserved to mean "no loop" */
    int loops_used;      /* the highest loop id used so far */
    int loops_edges_allocated; /* the allocated length of loops_edges */

    int np_allocated;
    int *P;              /* for each edge, the previous edge in the doubly-linked loop */
    int *N;              /* for each edge, the next edge in the doubly-linked loop */
} loop_table;



typedef short INT16;
typedef int INT32;

    /* 
     * Macros
     */
#define CASE_C 0
#define CASE_L 1
#define CASE_E 2
#define CASE_R 3
#define CASE_S 4
#define CASE_M 5
#define CASE_M2 6
/* the number of bytes to add to get to the next 4-byte alignment.  returns 0,1,2, or 3 */
#define TONEXT4(i) (3 - (((i)-1)%4))
/* the maximum value that can be stored in a given number of unsigned bits */
#define MAXVAL(bits) ((1<<(bits))-1)
/* factor by which we allow the points array to grow (to absorb nonmanifolds)
 * before giving up and falling back to our backup method */
#define BIG_FLOAT (1e20f)
#define DEFAULT_QUANTIZATION (11)
#define DEFAULT_POINT_FACTOR (1.5f)
#define DEFAULT_HINTS (0)
/* an upper bound on the size, in bytes of a point int the compressed stream --
 * used for allocating the compression stream */
#define POINTSIZE 12 
/* Reserved identifiers with special meaning */
#define GARBAGE_VERTEX	((int)0x80808080)
#define DUMMY_VERTEX    ((int)0x80000003)
#define GARBAGE ((int)0x80808080)           /* all 4 bytes must be same to remain compatible with memset */
#define VERTEX_SPECIAL(x) ((unsigned int)(x) & 0x80000000)
#define INVALIDATE_VERTEX(x) ((x) |= 0x80000000) 
#define EA_VERTEX_INDEX(x) ((x) & ~0x80000000)
#define PROXY_VERTEX_INDEX(proxy_hash,v) (VERTEX_SPECIAL(v)?lookup_vertex((proxy_hash),(v)):v)

#define GARBAGE_EDGE ((int)0x80808080)
#define MULTIPLE_EDGE ((int)0x80000001)
#define EDGE_SPECIAL(x) ((unsigned int)(x) & 0x80000000)
#define EDGE_INVALID(x) ((unsigned int)(x) & 0x80000000)
#define INVALIDATE_EDGE(x) ((x) |= 0x80000000) 
#define REAL_EDGE_INDEX(x) ((x) & ~0x80000000)

//converting an int to a void * and back requires a multiple type cast
#define I2V(x) ((void *)(POINTER_SIZED_INT)(x))
#define V2I(x) ((int)(POINTER_SIZED_INT)(void *)(x))


/* leave h->start uninitialized, since it is always immediately filled in by caller */
//#define HALF_EDGE_INIT(h) ((h)->twin=GARBAGE_EDGE)
#define HALF_EDGE_INIT(h) ((h)->start=(h)->twin=GARBAGE_EDGE)

/* We are operating "implicit mode" only here, meaning that since we have only triangles
 * in our half edge structure, we can use simple arithmetic to go around a triangle.]
 * These macros are only valid applied to edge indices, not vertex indices. */
#define HNEXT(i) (3*((i)/3) + ((i)+1)%3)
#define HPREV(i) (3*((i)/3) + ((i)+2)%3)


    /* 
     * External function declarations (functions from BEdgeBreakerCommon.cpp)
     */
extern void mtable_info_init( mtable_info *m );
extern void mtable_info_free( mtable_info *m );
extern int lookup_vertex(const struct vhash_s *proxy_hash, int v);
extern void predict( 
    half_edge_array const *ea, 
    int ei,                     
    int third_vertex_unknown,   
    char const *touched,
    struct vhash_s const *proxy_hash,
    int const *quantized_points,
    int *prediction_out );
extern int old_predict( 
    int const *associations, 
    int const *points, 
    ET_Bounding const *bounding, 
    int x_quantization,
    int y_quantization,
    int z_quantization,
    int *out );
extern int old_pack_points( 
    mtable_info *mtable, int *associations, edgebreaker_header *hptr,
    int original_pointcount, int const *pointmap, float const *points, 
    int buffsize, void *buffer_out, 
    eb_compress_configs const *configs );
extern int old_unpack_points( 
    int const *associations, 
    edgebreaker_header const *hptr,
    void const *diffs_in, 
    float *points_out, 
    ET_Bounding const *bounding, 
    int x_quantization, int y_quantization, int z_quantization );
extern int old_unpack_normals( 
    int const *associations, 
    edgebreaker_header const *hptr,
    void const *diffs_in, 
    float *normals_out, 
    int x_quantization_normals, int y_quantization_normals, int z_quantization_normals );
extern int int_stack_init( int_stack *s );
extern int int_stack_expand( int_stack *s );
extern void int_stack_free( int_stack *s );
extern int int_stack_pop( int_stack *s );
extern int int_stack_pop_internal( int_stack *s, int offset, int *out );
extern void int_stack_push( int_stack *s, int n );
extern void int_stack_push_if_unique( int_stack *s, int n );
extern int validate_associations( 
	int const *associations, 
	int pointcount );
extern int half_edge_array_init( half_edge_array *ea, int initial_size );
extern void half_edge_array_free( half_edge_array *ea );
extern int half_edge_array_expand( half_edge_array *ea );
extern int half_edge_array_append( half_edge_array *ea, half_edge **out );
extern int validate_edge ( half_edge_array const *ea, half_edge const *a, loop_table const *loops );
extern void *default_malloc( size_t size, void *user_data );
extern void default_free( void *ptr, void *user_data );
extern void default_new_vertex( int a, int b, int c, void *user_data );
extern void vsinit_write( varstream *vs, int size, void *pointer );
/* vsinit_read variable, 'pointer' is non-const, since BIGENDIAN systems swap32 the incoming data */
extern void vsinit_read( varstream *vs, int size, void *pointer );
extern void vsfree( varstream *vs );
extern void vsswap( varstream *vs );
extern void vsput( varstream *vs, const int *numbits_array, int val );
extern int vsget( varstream *vs, const int *numbits_array );


#endif //BSTREAM_DISABLE_EDGEBREAKER
#endif //__BEDGEBREAKER_H__


