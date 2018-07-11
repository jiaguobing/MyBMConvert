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
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Polyhedron::Interpret(tk, key, special); }


    public:
        //! custom Execute
        TK_Status   HExecute (BStreamFileToolkit & tk, int mesh_columns = 0) alter;
        //! custom Interpret
        TK_Status   HInterpret (BStreamFileToolkit & tk, HC_KEY key, int lod=0, int mesh_columns=0) alter;

        /*! internal use */
        bool        match_points (Recorded_Instance const * instance, float const * instance_points);
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
        /*! constructor */
        HTK_Mesh() : TK_Mesh () {}
        ~HTK_Mesh();

        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Mesh::Interpret(tk, key, special); }
        bool        Match_Instance (BStreamFileToolkit const & tk, Recorded_Instance alter * instance) alter;
}; 

#endif


