#ifndef HOPCODE_HANDLER
#define HOPCODE_HANDLER

#ifndef HBINFILETK_TOOLKIT
    #include "HStreamFileToolkit.h"
#endif


////////////////////////////////////////////////////////////////////////////////

//! Provides HOOPS/3dGS-specific handling of the TKE_Open_Segment opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Open_Segment : public TK_Open_Segment {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter
                                { return TK_Open_Segment::Interpret(tk, key, variant); }
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter;
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};


//! Provides HOOPS/3dGS-specific handling of the TKE_Close_Segment opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Close_Segment : public TK_Close_Segment {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Close_Segment::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

//! Provides HOOPS/3dGS-specific handling of the TKE_Reopen_Segment opcode.
class HBINFILETK_API HTK_Reopen_Segment : public TK_Reopen_Segment {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Reopen_Segment::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

//! Provides HOOPS/3dGS-specific handling of the TKE_Include_Segment and TKE_Style_Segment opcodes.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Referenced_Segment : public TK_Referenced_Segment {
    protected:
        int         mh_stage;   /*!< create a class-specific 'stage' var to avoid confusion with base class 'stage' variable */
        HC_KEY      m_root;     /*!< root of current tree, to see if referenced can be relative */

    public:
		/*! constructor */
        HTK_Referenced_Segment (char opcode) : TK_Referenced_Segment (opcode), mh_stage (0) {}

        TK_Status   Write (BStreamFileToolkit & tk) alter;  //
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Referenced_Segment::Interpret(tk, key, special); }
        void        Reset (void) alter;
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		/*! Records the top of a segment tree in order to determine whether referenced segment can be a relative reference
		    \param root Key of segment at top of tree */
		void        SetRoot (HC_KEY root) alter     { m_root = root;    }

		/*! \returns the key of the root segment  (see SetRoot) */
        HC_KEY      GetRoot (void) const            { return m_root;    }
};

//! Provides HOOPS/3dGS-specific handling of the TKE_Repeat_Object opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Instance : public TK_Instance {
    public:
		/*! constructor */
        HTK_Instance (int from_index=0, int from_variant=0, int to_index=0, int to_variant=0,
                      int options=0, float const * xform=0);

        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Instance::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};


////////////////////////////////////////
//! Provides HOOPS/3dGS-specific handling of the TKE_LOD opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.
*/
class HBINFILETK_API HTK_LOD : public TK_LOD {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_LOD::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};


////////////////////////////////////////
//! Provides HOOPS/3dGS-specific handling of the TKE_Renumber_Key_Local and TKE_Renumber_Key_Global opcodes.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Renumber : public TK_Renumber {
    public:
		/*! constructor */
        HTK_Renumber (unsigned char opcode, HC_KEY key = 0) : TK_Renumber (opcode, key) {}

        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Renumber::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};


////////////////////////////////////////////////////////////////////////////////

//! Provides HOOPS/3dGS-specific handling of the TKE_Color opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Color : public TK_Color {
    friend class HTK_Color_Composite;
    public:
		/*! constructor */
        HTK_Color (int mask=0, int channels=0)
            : TK_Color () { m_mask = mask; m_channels = (short)channels; }

        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Color::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

//! Provides HOOPS/3dGS-specific handling of the TKE_Color_RGB opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Color_RGB : public TK_Color_RGB {
    public:
		/*! constructor */
        HTK_Color_RGB (int mask=0, float const * rgb=0)
            : TK_Color_RGB () {
                m_mask = mask;
                if (rgb != 0) {
                    m_rgb[0] = rgb[0];  m_rgb[1] = rgb[1];  m_rgb[2] = rgb[2];
                }
            }

        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Color_RGB::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

//! Provides HOOPS/3dGS-specific handling of the TKE_Color_By_Value opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Color_By_Value : public TK_Color_By_Value {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Color_By_Value::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

//! Provides HOOPS/3dGS-specific handling of the TKE_Color_By_Index and TKE_Color_By_Index_16 opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Color_By_Index : public TK_Color_By_Index {
    public:
		/*! constructor */
        HTK_Color_By_Index (unsigned char opcode, int mask=0, int index=0)
            : TK_Color_By_Index (opcode) { m_mask = mask; m_index = index; }

        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Color_By_Index::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};


//! Provides HOOPS/3dGS-specific handling of the TKE_Color_By_FIndex opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Color_By_FIndex : public TK_Color_By_FIndex {
    public:
		/*! constructor */
        HTK_Color_By_FIndex (int mask=0, float index=0.0f)
            : TK_Color_By_FIndex () { m_mask = mask; m_index = index; }

        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Color_By_FIndex::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

/*! entire color setting will be interpreted into the composite pseudo-object,
    which will contain one or more simple objects which can actually be written.
    reading will just interpret the simple objects directly, as usual */
class HBINFILETK_API HTK_Color_Composite : public BBaseOpcodeHandler {
    protected:
        BBaseOpcodeHandler *     m_parts[16];   //!< Array of simple color handlers which together represent the full color specification

    public:
        HTK_Color_Composite ();
        ~HTK_Color_Composite ();

        TK_Status   Read (BStreamFileToolkit & tk) alter;
        TK_Status   Write (BStreamFileToolkit & tk) alter;
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return BBaseOpcodeHandler::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};


////////////////////
//! Provides HOOPS/3dGS-specific handling of the TKE_Color_Map opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Color_Map : public TK_Color_Map {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Color_Map::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

////////////////////
//! Provides HOOPS/3dGS-specific handling of the TKE_Callback opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Callback : public TK_Callback {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Callback::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

////////////////////

//! Provides HOOPS/3dGS-specific handling of the TKE_Rendering_Options opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Rendering_Options : public TK_Rendering_Options {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Rendering_Options::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

////////////////////
//! Provides HOOPS/3dGS-specific handling of the TKE_Heuristics opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Heuristics : public TK_Heuristics {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Heuristics::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

////////////////////
//! Provides HOOPS/3dGS-specific handling of the TKE_Visibility opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Visibility : public TK_Visibility {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Visibility::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

////////////////////
//! Provides HOOPS/3dGS-specific handling of the TKE_Selectability opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Selectability : public TK_Selectability {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Selectability::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

//! Provides HOOPS/3dGS-specific handling of the TKE_Matrix opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Matrix : public TK_Matrix {
    public:
		/*! constructor */
        HTK_Matrix (unsigned char opcode) : TK_Matrix (opcode) {}

        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Matrix::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

//! Provides HOOPS/3dGS-specific handling of various opcodes.
/*!  
  This class provides provides support for writing/reading opcode objects which represent a simple choice from an
    enumerated set (TKO_Enumerations). This currently includes the opcodes:
        TKE_Face_Pattern, TKE_Window_Pattern, TKE_Marker_Symbol, TKE_Text_Alignment, TKE_Window_Frame, TKE_Handedness
*/
class HBINFILETK_API HTK_Enumerated : public TK_Enumerated {
    public:
		/*! constructor */
        HTK_Enumerated (unsigned char opcode) : TK_Enumerated (opcode) {}

        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Enumerated::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

//! Provides HOOPS/3dGS-specific handling of the TKE_Line_Weight, TKE_Edge_Weight, TKE_Marker_Size and TKE_Text_Spacing opcodes
class HBINFILETK_API HTK_Size : public TK_Size {
    public:
		/*! constructor */
        HTK_Size (unsigned char opcode) : TK_Size (opcode) {}

        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Size::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

//! Provides HOOPS/3dGS-specific handling of the TKE_Line_Pattern and TKE_Edge_Pattern opcodes.
/*!  
    TKE_Line_Pattern and TKE_Edge_Pattern opcode objects represent a composite of the dashing pattern, cap style, 
	and join style.
*/
class HBINFILETK_API HTK_Linear_Pattern : public TK_Linear_Pattern {
    public:
		/*! constructor */
        HTK_Linear_Pattern (unsigned char opcode) : TK_Linear_Pattern (opcode) {}

        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Linear_Pattern::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

//! Provides HOOPS/3dGS-specific handling of the TKE_Streaming_Mode opcode.
class HBINFILETK_API HTK_Streaming : public TK_Streaming {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Streaming::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

////////////////////////////////////////////////////////////////////////////////
//! Provides HOOPS/3dGS-specific handling of the TKE_User_Options opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_User_Options : public TK_User_Options {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_User_Options::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

////////////////////////////////////////////////////////////////////////////////
//! Provides HOOPS/3dGS-specific handling of the TKE_Unicode_Options opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Unicode_Options : public TK_Unicode_Options {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Unicode_Options::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

//! Provides HOOPS/3dGS-specific handling of the TKE_User_Index opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_User_Index : public TK_User_Index {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_User_Index::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};


//! Provides HOOPS/3dGS-specific handling of the TKE_User_Value opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_User_Value : public TK_User_Value {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_User_Value::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

//! Provides HOOPS/3dGS-specific handling of the TKE_Camera opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API2 HTK_Camera : public TK_Camera {
    public:
		/*! constructor */
        HTK_Camera (unsigned char opcode = TKE_Camera) : TK_Camera (opcode) {}

        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Camera::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};


//! Provides HOOPS/3dGS-specific handling of the TKE_Window opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Window : public TK_Window {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Window::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};


//! Provides HOOPS/3dGS-specific handling of the TKE_Clip_Region opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/

class HBINFILETK_API HTK_Clip_Region : public TK_Clip_Region {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Clip_Region::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};


//! Provides HOOPS/3dGS-specific handling of the TKE_Text_Font opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Text_Font : public TK_Text_Font {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Text_Font::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

////////////////////////////////////////////////////////////////////////////////

//! Provides HOOPS/3dGS-specific handling of the TKE_Bounding and TKE_Bounding_Info opcodes.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.
        
    Two different opcodes are used as follows:
        The TKE_Bounding opcode will exporta bounding volume that was explicitly set on a HOOPS segment (if any).
        The TKE_Bounding_Info will cause an explicit computation of the HOOPS/3dGS bounding information and export it. 
*/
class HBINFILETK_API2 HTK_Bounding : public TK_Bounding {
    public:
		/*! default constructor */
        HTK_Bounding (unsigned char opcode)
            : TK_Bounding (opcode) {}
		/*! constructor that accepts min and max values */
        HTK_Bounding (unsigned char opcode, float * min, float * max)
            : TK_Bounding (opcode, min, max) {}
		/*! constructor that accepts center and radius values */
        HTK_Bounding (unsigned char opcode, float * center, float radius)
            : TK_Bounding (opcode, center, radius) {}

        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Bounding::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

/////////////////////////////////////////////////////////////////////
//! Provides HOOPS/3dGS-specific handling of the TKE_Marker, TKE_Text_Path TKE_Distant_Light, and TKE_Local_Light opcodes.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Point : public TK_Point {
    public:
		/*! constructor */
        HTK_Point (unsigned char opcode) : TK_Point (opcode) {}

        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Point::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};


//! Provides HOOPS/3dGS-specific handling of the TKE_Line opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Line : public TK_Line {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Line::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};


//! Provides HOOPS/3dGS-specific handling of the TKE_Polyine and TKE_Polygon opcodes.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Polypoint : public TK_Polypoint {
    public:
		/*! constructor */
        HTK_Polypoint (unsigned char opcode) : TK_Polypoint (opcode) {}

        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Polypoint::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

//! Provides HOOPS/3dGS-specific handling of the TKE_NURBS_Curve opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_NURBS_Curve : public TK_NURBS_Curve {
    public:

        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_NURBS_Curve::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

//! Provides HOOPS/3dGS-specific handling of the TKE_NURBS_Surface opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_NURBS_Surface : public TK_NURBS_Surface {
    protected:

        /*! a helper function to extract from HOOPS the list of trims associated with a NURBS surface */
        TK_Status   interpret_trim (BStreamFileToolkit & tk, int) alter;
        /*! a helper function to insert into HOOPS the list of trims associated with a NURBS surface */
        TK_Status   execute_trim (BStreamFileToolkit & tk, HT_NURBS_Trim *) alter;

    public:

        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_NURBS_Surface::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

//! Provides HOOPS/3dGS-specific handling of the TKE_Area_Light opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Area_Light : public TK_Area_Light {
    public:

        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Area_Light::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

//! Provides HOOPS/3dGS-specific handling of the TKE_Spot_Light opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Spot_Light : public TK_Spot_Light {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Spot_Light::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

//! Provides HOOPS/3dGS-specific handling of the TKE_Cutting_Plane opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Cutting_Plane : public TK_Cutting_Plane {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Cutting_Plane::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

//! Provides HOOPS/3dGS-specific handling of the TKE_Circle, TKE_Circular_Arc, TKE_Circular_Chord and TKE_Circular_Wedge opcodes.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Circle : public TK_Circle {
    public:
		/*! constructor */
        HTK_Circle (unsigned char opcode) : TK_Circle (opcode) {}

        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Circle::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

//! Provides HOOPS/3dGS-specific handling of the TKE_Ellipse and TKE_Elliptical_Arc opcodes.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Ellipse : public TK_Ellipse {
    public:
		/*! constructor */
        HTK_Ellipse (unsigned char opcode) : TK_Ellipse (opcode) {}

        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Ellipse::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

//! Provides HOOPS/3dGS-specific handling of the TKE_Cylinder opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Cylinder : public TK_Cylinder {
    public:
		/*! constructor */
        HTK_Cylinder () : TK_Cylinder () {}

        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Cylinder::Interpret(tk, key, special); }
        TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

//! Provides HOOPS/3dGS-specific handling of the TKE_PolyCylinder opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_PolyCylinder : public TK_PolyCylinder {
    public:
		/*! constructor */
        HTK_PolyCylinder () : TK_PolyCylinder () {}

        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_PolyCylinder::Interpret(tk, key, special); }
        TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

//! Provides HOOPS/3dGS-specific handling of the TKE_Grid opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Grid : public TK_Grid {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Grid::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

////////////////////////////////////////////////////////////////////////////////
//! Provides HOOPS/3dGS-specific handling of the TKE_Text and TKE_Text_With_Encoding opcodes.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Text : public TK_Text {
    public:
		/*! constructor */
        HTK_Text (unsigned char opcode) : TK_Text (opcode) {}

        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Text::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};

////////////////////////////////////////////////////////////////////////////////
//! Provides HOOPS/3dGS-specific handling of the TKE_Font opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API HTK_Font : public TK_Font {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter
                                { return TK_Font::Interpret(tk, key, variant); }
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter;
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};


////////////////////////////////////////////////////////////////////////////////
//! Provides HOOPS/3dGS-specific handling of the TKE_Image opcode.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API2 HTK_Image : public TK_Image {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
                                { return TK_Image::Interpret(tk, key, special); }
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};


////////////////////////////////////////////////////////////////////////////////
//! Provides HOOPS/3dGS-specific handling of the TKE_Texture.
/*!  
    This class complements the read/write functionality implemented in the base class by 
    overloading the Interpret and Execute methods to provide HOOPS/3dGS-specific opcode handling.  
    
    Writing:  The HOOPS/3dGS database is queried in the Interpret method, and this data is then written to the HSF file by the Write method
    Reading:  The data is read from the HSF file by the Read method, and is mapped to the HOOPS/3dGS scene-graph 
    within the Execute method.

    Export/import of additional user-data (which is associated with this HSF object) would typically be done by overloading the Write and Read methods.      
*/
class HBINFILETK_API2 HTK_Texture : public TK_Texture {
    public:
        TK_Status   Execute (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter
                                { return TK_Texture::Interpret(tk, key, variant); }
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter;
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;
};


#endif //HOPCODE_HANDLER

