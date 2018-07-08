#ifndef HBINFILETK_SHELL
#define HBINFILETK_SHELL

#include "HOpcodeHandler.h"
#include "BOpcodeShell.h"
#include "HPolyhedron.h"


//! Provides HOOPS/3dGS-specific handling of the TKE_Shell opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Shell : public TK_Shell {
    protected:
	//! Determines the order in which data appears with TK_Priority_Heuristic write flag (the default)
	virtual TK_Status Usefulness(BStreamFileToolkit & tk, HC_KEY key, int lod, float alter &score) const;
        TK_Status Revisit(BStreamFileToolkit & tk, float priority=0.0f, int lod=-1) const;
	//! variant that doesn't have a priority value (which is used for sorting order)
        TK_Status Revisit(BStreamFileToolkit & tk, int lod) const { return Revisit (tk, 0.0f, lod); }
	/*! figure out what revisits are required during this pass.
            \param tk the toolkit
            \param lod_in the lod level requested by the caller
            \param do_continue return value to indicate whether the interpret process is complete
            \return TK_Error on failure, TK_Revisit to completely suppress all writing, otherwise TK_Normal
        */
        TK_Status ProcessRevisits( BStreamFileToolkit & tk, int lod_in, bool *do_continue );
    

    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=-1) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Shell::Interpret(tk, key, special); }

        bool        Match_Instance (BStreamFileToolkit const & tk, Recorded_Instance alter * instance) alter;
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

}; //end declaration of class TK_Shell


#endif
