// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef HBINFILETK_POLYPOLY
#define HBINFILETK_POLYPOLY

#include "HOpcodeHandler.h"
#include "BPolyPoly.h"


//! Provides HOOPS/3dGS-specific handling of the TKE_Polyline and TKE_Line opcodes.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  

    The Interpret phase of this opcode is a little unusual.  Since we are dealing with multiple primitives, 
    instead of just one, Interpret operates on all of the geometry in the currently open segment, rather than
    what is specified as part of its "key" parameter.  The "key" that is currently passed into interpret refers
    to the first line or polyline that was encountered by the segment tree traversal, but we need to
    get all of the lines and polylines in the segment.

    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_PolyPolypoint : public TK_PolyPolypoint {
    protected:
		//! list of entities keys
		HC_KEY * m_keys;
    public:
		/*! constructor */
        HTK_PolyPolypoint(unsigned char op) : TK_PolyPolypoint(op) {
            m_keys = 0;
        };
        ~HTK_PolyPolypoint() {
            Reset();
        };

        TK_Status   Execute (BStreamFileToolkit & tk);
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=-1);
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special)
                                { return TK_PolyPolypoint::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
        void        Reset (void) {
	    delete[] m_keys;
	    m_keys = 0;
	    TK_PolyPolypoint::Reset();
	}
};



#endif
