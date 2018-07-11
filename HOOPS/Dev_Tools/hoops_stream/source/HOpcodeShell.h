// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

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
	virtual TK_Status Usefulness(BStreamFileToolkit & tk, HC_KEY key, int lod, float &score) const;
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
        /*! There are some edges that will appear on the read
            side (due to triangulation) that don't actually exist on the write side.
            We need to enumerate the edges again **not** by tristrips so that we can discover and flag
            the appropriate fake edges, and they can then be skipped by HPolyhedron::HInterpret */
        TK_Status LabelFakeEdges (); 
   
    public:
        TK_Status   Execute (BStreamFileToolkit & tk);
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=-1);
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special)
                                { return TK_Shell::Interpret(tk, key, special); }
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
        bool        Match_Instance (BStreamFileToolkit const & tk, Recorded_Instance * instance);

}; //end declaration of class TK_Shell


#endif
