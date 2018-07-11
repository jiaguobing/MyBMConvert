// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef HBINFILETK_POLYHEDRON
#define HBINFILETK_POLYHEDRON

#include "BPolyhedron.h"

/*!
    This is only an interface class...
    Note that the names are different --
    If we tried casting a shell/mesh down to TK_Polyhedron and up to HTK_Polyhedron, it
    would still find Execute/Interpret from the virtual table & call the one on the real
    class.  We therefore disallow calling through the virtual functions and require use of
    function with a similar signature.
*/
class HBINFILETK_API HTK_Polyhedron : public TK_Polyhedron {
    private:
        HTK_Polyhedron():TK_Polyhedron(0){};
        TK_Status   Execute (BStreamFileToolkit & tk);
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0);
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special)
                                { return TK_Polyhedron::Interpret(tk, key, special); }


    public:
        //! custom Execute
        TK_Status   HExecute (BStreamFileToolkit & tk, int mesh_columns = 0);
        //! custom Interpret
        TK_Status   HInterpret (BStreamFileToolkit & tk, HC_KEY key, int lod=0, int mesh_columns=0);

        /*! internal use */
        bool        match_points (Recorded_Instance const * instance, float const instance_points[]);
        /*! internal use */
        bool        match_attributes (Recorded_Instance const * instance);
};


//! Provides HOOPS/3dGS-specific handling of the TKE_Mesh opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Mesh : public TK_Mesh {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk);
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0);
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special)
                                { return TK_Mesh::Interpret(tk, key, special); }
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
        bool        Match_Instance (BStreamFileToolkit const & tk, Recorded_Instance * instance);
}; 

#endif


