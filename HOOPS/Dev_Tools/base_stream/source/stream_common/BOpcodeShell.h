// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef BBINFILETK_SHELL
#define BBINFILETK_SHELL

#include "BOpcodeHandler.h"
#include "BPolyhedron.h"



//! Handles the TKE_Shell opcode.
/*! 
    TK_Shell provides support for writing/reading the TKE_Shell opcode object to/from an HSF file. 

    The HOOPS/3dGS scene-graph can contain 'shell' primitives which consist of a points array and a connectivity list
        to define the facets of the shell.
*/
class BBINFILETK_API TK_Shell : public TK_Polyhedron {
    private:
        virtual TK_Status read_advanced (BStreamFileToolkit & tk);
        virtual TK_Status read_uncompressed_points (BStreamFileToolkit & tk);
        virtual TK_Status read_uncompressed_faces (BStreamFileToolkit & tk);
        virtual TK_Status read_bounding (BStreamFileToolkit & tk);




		virtual TK_Status read_advanced_ascii (BStreamFileToolkit & tk);
        virtual TK_Status read_uncompressed_points_ascii (BStreamFileToolkit & tk);
        virtual TK_Status read_uncompressed_faces_ascii (BStreamFileToolkit & tk);
        virtual TK_Status read_bounding_ascii (BStreamFileToolkit & tk);


        virtual TK_Status compute_advanced (BStreamFileToolkit & tk);
        virtual TK_Status write_advanced (BStreamFileToolkit & tk);
        virtual TK_Status write_uncompressed_points (BStreamFileToolkit & tk);
        virtual TK_Status write_uncompressed_faces (BStreamFileToolkit & tk);
        virtual TK_Status write_bounding (BStreamFileToolkit & tk);
        TK_Status write_null (BStreamFileToolkit & tk);





		virtual TK_Status write_advanced_ascii (BStreamFileToolkit & tk);
        virtual TK_Status write_uncompressed_points_ascii (BStreamFileToolkit & tk);
        virtual TK_Status write_uncompressed_faces_ascii (BStreamFileToolkit & tk);
        virtual TK_Status write_bounding_ascii (BStreamFileToolkit & tk);
        TK_Status write_null_ascii (BStreamFileToolkit & tk);
		TK_Status compute_advanced_ascii(BStreamFileToolkit & tk);


    protected:
        int             m_substage;     /*!< Tracks progress in subroutines */
        unsigned char   m_compression_scheme; /*!< The compression scheme currently in use */
        int             m_flistlen;      /*!< Length of the face list */
        int             *m_flist;        /*!< Facelist array; could be in tristrips format if (mp_subop & TKSH_TRISTRIPS)*/
        char            m_lodlevel;      /*!< The current lod level.  LOD level identifiers are numbers between 0 and 7, inclusive */

        /*!Allocate an array of the provided length for the face list, and set values if given.
            The face_list is an array of integers. The first integer is the number 
            of vertices that should be connected to form the first face. For example, 
            "3" for a triangle. The next three integers (in this example) are the 
            offsets into the points array at which the three x-y-z's can be found. 
            The first point in the points array is considered to be at offset zero, 
            so "0, 1, 2" in the face_list array would form the triangle from the 
            first three entries in points. 

            Continuing with the example, a second triangle might be specified as 
            "3, 0, 1, 3", meaning "form a face using three vertices. The vertices 
            are at offsets zero, one, and three in the points array". So this second 
            face happens to share an edge---the (0, 1) edge---with the first face. 
            A third face might be "4, 1, 2, 11, 12", forming a quadrilateral. Faces 
            continue being formed until an flist_length number of integers in face_list 
            have been processed. 

            One special case: if a vertex count in the list is negative, that means 
            "continue with the previous face and subtract a polygon formed from the 
            following vertices". This allows you to build faces with true holes in 
            them. Multiple holes get an "even-odd" rule applied. A region is part of 
            the face if there are an odd number of edges between it and infinity in 
            the plane of the face. Otherwise a region is considered exterior and is 
            not drawn. The edges of the hole receive the usual edge attributes. For 
            face-numbering purposes (for example, Edit_Shell_Faces) the hole is not 
            counted separately---it's a part of the face it's in. 

            If the TKSH_TRISTRIPS bit is set in suboptions (e.g. via the functoin 
            TK_Polyhedron::SetSubop), the shell is assumed to contain triangles only and 
            may not contain holes, and the "faces" are actually the way that 
            vertices are connected into triangle strips. Start with the length of 
            the strip, followed by the vertex indices to be connected. The first 
            3 vertices form a triangle, and every additional vertex is combined 
            with the two previous ones to define one additional triangle. Exactly as 
            with OpenGL's GL_TRIANGLE_STRIP primitive, the orientation of every even 
            triangle is reversed, beginning with the second. 
        */
        TK_Status SetFaces( int length, int const *face_list = 0 );
        //! Sets the lod level for this representation (as in HOOPS, '0' means original base resolution)
        TK_Status SetLodLevel( int level )       { m_lodlevel = (char)level; return TK_Normal; };
        /*! \return a pointer to the face list, an array of integers */
        int const *GetFaces() const { return m_flist; };
        /*! \return the length of the face list (as in HOOPS, '0' means original base resolution) */
        int GetFacesLength() const { return m_flistlen; };
        /*! \return the LOD level */
        int GetLodLevel() const { return m_lodlevel; };
        /*! overloads from TK_Polyhedron.  Creates the mp_edge_enumeration array that lists all of the 
            pairs of vertices that connect together as edges.  */
        TK_Status EnumerateEdges(); 

    public:
        TK_Shell ();
        ~TK_Shell ();

        //!obsolete.  Provided for compatibility
        void    set_flist (int count, int const * list = 0)       { SetFaces( count, list ); };

        TK_Status   Read (BStreamFileToolkit & tk);
        TK_Status   Write (BStreamFileToolkit & tk);


        TK_Status   ReadAscii (BStreamFileToolkit & tk); //!< Deprecated
        TK_Status   WriteAscii(BStreamFileToolkit & tk); //!< Deprecated

        void        Reset ();
}; //end declaration of class TK_Shell



/*
 * SOME UTILITY MACROS
 */
#define SEMI_PROTECT(x) do {x} while (0)

//warning: implicitly references the BStreamFileToolkit &, tk
#define ENSURE_ALLOCATED(var,type,count) SEMI_PROTECT( \
	if (count > 0) { \
		BSTREAM_ALLOC_ARRAY(var, count, type); \
		if( (var) == 0 ) \
			return tk.Error(); \
	} \
)

//warning: implicitly references the BStreamFileToolkit &, tk
#define ENSURE_ALLOCATED_CLEAR(var,type,count) SEMI_PROTECT( \
	BSTREAM_ALLOC_ARRAY(var, count, type); \
    if( (var) == 0 ) \
        return tk.Error(); \
    memset (var, 0, (count) * sizeof(type)); \
)

#endif
