#ifndef HBINFILETK_TOOLKIT
#define HBINFILETK_TOOLKIT


#ifndef HSTREAM_READONLY

#ifndef DOXYGEN_SHOULD_SKIP_THIS

class Internal_Key_Array {
    friend class HStreamFileToolkit;
    friend class Internal_Pending_Tag;
    friend class Internal_Segment_Processor;
    private:
        Internal_Key_Array() { m_data = 0; }
        ~Internal_Key_Array() { delete [] m_data; }

        int m_count;
        HC_KEY *m_data;
};

class Internal_Pending_Tag {
    friend class HStreamFileToolkit;
    friend class Internal_Segment_Processor;
    friend class HTK_Referenced_Segment;
    private:
        Internal_Pending_Tag() { m_next = 0; m_array = 0; }
        ~Internal_Pending_Tag() { delete m_array; }

        Internal_Pending_Tag *  m_next;
        HC_KEY                  m_key;    //m_key set to tk.m_last_keys[0], just for the purposes of matching tag requests
        int                     m_variant;
        Internal_Key_Array *    m_array;  //set to null unless we are associating multiple keys with a single tag call.
};


class Internal_Texture_List {
    friend class HStreamFileToolkit;
    private:
        Internal_Texture_List *     m_next;
        char *                      m_name;
        char *                      m_image;
        HC_KEY                      m_image_key;
        bool                        m_do_write_image;

    public:
        Internal_Texture_List (char const * name, char const * image, HC_KEY key = -1);
        ~Internal_Texture_List();
};


////////////////////////////////////////////////////////////////////////////////
class HBINFILETK_API HStreamFileToolkit; //forward declaration

// special output-only pseudo-object
class Internal_Segment_Processor : public BBaseOpcodeHandler {
    protected:
        char *                  m_name;             /*!< name, if supplied, for current segment */
        HC_KEY                  m_key;              /*!< */
        HC_KEY                  m_root;             /*!< key of start of current subtree */
        BBaseOpcodeHandler *    m_current_object;   /*!< object handler associated with current Hoops item */
        BBaseOpcodeHandler *    m_must_delete;      /*!< set to object handler if it is a clone to be deleted after use */
        HC_KEY                  m_item_key;         /*!< [original] Hoops key, if any, of current item */
        int                     m_saved_count;      /*!< count of multiple keys */
        HC_KEY *                m_saved_keys;       /*!< multiple key storage */
        HC_KEY                  m_renumbered;       /*!< user renumbered key calue, if any, of current item */
        unsigned char           m_renumbered_scope; /*!< scope of renumbered key */
        int                     m_start_offset;     /*!< file offset of [revisited] segment */

        bool                    m_use_polypolylines;        /*!< A flag to indicate that polylines in the current segment should be handled with HTK_PolyPolypoint, not the usual HTK_Polypoint */
        bool                    m_polylines_already_handled;  /*!< A flag to indicate that polylines in the current segment have already been handled by polypolypoint (thus any additional "polyline" geometry should be skipped). */

        void    save_keys (HStreamFileToolkit & tk) alter;
        void    restore_keys (HStreamFileToolkit & tk) alter;

    public:
        Internal_Segment_Processor (ID_Key key = -1, char const * name = 0);
        ~Internal_Segment_Processor ();

        TK_Status   Read (BStreamFileToolkit & tk) alter;
        TK_Status   Write (BStreamFileToolkit & tk) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant = 0) alter;
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter;
        void        Reset (void) alter;
        TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **) const;

        void        SetRoot (HC_KEY root) alter     { m_root = root;    }
        HC_KEY      GetRoot (void) const            { return m_root;    }
};

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#endif //HSTREAM_READONLY





//! The HStreamFileToolkit class provides support for exporting/importing HOOPS Stream File information from/to a HOOPS/3dGS scene-graph.
/*! 
    The HSF file reading process involves reading the opcode at the start of each piece of binary information,
    and calling the Read method of the associated opcode handler. After the opcode handler reports 
    that reading is complete, the Execute method should be called. 
    The ParseBuffer method of the BStreamFileToolkit object encapsulates this process.

    The HSF file writing process involves determining the proper opcode for each piece of binary 
    information, and calling the Interpret method of the associated opcode handler.  After interpretation is complete,  
    the Write method of the opcode handler should be called until writing is complete.  
    The GenerateBuffer method of the HStreamFileToolkit encapsualetes this process. 

    This class provides a HOOPS/3dGS-specific implementation of the BStreamFileToolkit object.  It has HOOPS/3dGS-specific
    opcode handlers registered with it (of the naming convention HTK_XXX).  These opcode handlers overload the following
    two methods of the base opcode handler:

     - Interpret :  this method will query the HOOPS/3dGS scene-graph (meaning, extract the graphics data); the data will then be written out to an HSF via the Write method
     - Execute :    this method will map the HSF data to custom data structures after it has been read in from the HSF file via the Read method
*/
class HBINFILETK_API2 HStreamFileToolkit : public BStreamFileToolkit {
    friend class Internal_Segment_Processor;
    friend class HTK_Open_Segment;
    friend class HTK_Referenced_Segment;
    private:
        int                             m_hoops_version;        /* Version of Hoops we are using */
        Internal_Segment_List *         m_unnamed_segments;     /* Segments which need to be renamed to unnamed when able */

#ifndef HSTREAM_READONLY
        Internal_Segment_Processor *    m_segment_processor;    /* Handler for top segment & revisited segments */
        Internal_Pending_Tag *          m_pending_tags;         /* List of items to have tags attatched. Tags should come absolutely last (even after user data), so tagging postponed to the end.  It is a list to allow for recursion if we ever need it */
        Internal_Texture_List *         m_texture_list;         /* Textures being processed */
        bool                            m_non_segment_revisits; /* Does the revisit list contain something other than segments? */
        int                             m_revisit_offset;       /* File offset of start of current revisit object (including compression if appropriate) */
        bool issorted_revisit(Internal_Revisit_Item **array, int count);
        void qsort_revisit(Internal_Revisit_Item ** first, Internal_Revisit_Item ** last) { qsort_revisit (first, last, 0); }
        void qsort_revisit(Internal_Revisit_Item **, Internal_Revisit_Item **, int);
        TK_Status sort_revisit();

        TK_Status find_textures();
#endif

    public:
        /*! 
           Default constructor for an HStreamFileToolkit object 
        */
        HStreamFileToolkit (void);

        ~HStreamFileToolkit ();
        // clean out current state

        /*! 
        Initializes the file toolkit.  This is useful if a single toolkit object is to be used for dealing with different files, 
        rather than deleting the current toolkit object and creating a new instance.  Call this function after processing of the 
        current file is complete.
        */
        void        Restart (void) alter;

        // normal processing functions

        /*!
            Traverses the HOOPS database beginning at the currently open segment and calls the Interpret method of the opcode handler 
            associated with each object encountered.  The Interpret methods of the default object handlers query the HOOPS database to 
            obtain segment, geometry and attribute information.  After interpretation is complete, GenerateBuffer continually calls the 
            Write method of the opcode handler until writing is complete.  The Write methods of the default handlers convert the HOOPS 
            database information into a specific binary format.

            A HOOPS segment must currently be open before calling GenerateBuffer, or a segment name must be given.

            If the buffer being generated ends in the middle of an object, it will be saved along with any HOOPS database state information 
            and the next buffer will be appended to it.

            \param b A character pointer.  Returned to user. 
            \param s An integer denoting the length of the buffer in bytes.
            \param f An integer denoting the length of the buffer that was written back, in bytes.
            \param start_segment Explicit name of a hoops segment to process (currently open segment if null)
            \return The result of the function call.
        */
        TK_Status   GenerateBuffer (char * b, int s, int alter & f, char const * start_segment = 0) alter;


        /*!
            This method is similar to GenerateBuffer.  However, instead of traversing the HOOPS database beginning at the currently 
            open segment, only the item specified will be processed.  

            If the buffer being generated ends in the middle of an object, it will be saved along with any HOOPS database state information 
            and the next buffer will be appended to it.

            \param b A character pointer.  Returned to user. 
            \param s An integer denoting the length of the buffer in bytes.
            \param f An integer denoting the length of the buffer that was written back, in bytes.
            \param item Key of the single segment or piece of geometry to process.
            \return The result of the function call.
        */
        TK_Status   GenerateOneItemBuffer (char * b, int s, int alter & f, HC_KEY item) alter;


        /*!
            Traverses the HOOPS database beginning at the currently open segment and estimates the number of objects that
            will be written by a matching call (or series of calls) to GenerateBuffer()

            A HOOPS/3dGS segment must currently be open before calling CountObjects, or a segment must be specified.

            \param start_segment Explicit name of a Hoops segment to process (current segment if null).
            \return The result of the function call.
        */
        unsigned long   CountObjects (char const * start_segment = 0) alter;


        void    ActivateContext (ID_Key key);
        void    DeactivateContext (ID_Key key);
        void    NewFileContext (ID_Key key);


        /*!
            Determines the version of HOOPS/3dGS the toolkit is using.  Some features may not be present on older versions
            or may be handled differently or can be emulated; querying the version allows opcode handlers to adjust
            their behaviour accordingly.
            \return The version times 100, for instance, version 7.15 would be returned as 715.
        */
        int         GetHoopsVersion()       { if (m_hoops_version == 0) validate_hoops_version();   return m_hoops_version; }

#ifndef HSTREAM_READONLY
        /*! Generates clones of OpcodeHandlers to interpret geometry lists, such as those found in LODs
            intended primarily (but not exclusively) for internal use.
            \param type the string returned from HOOPS' Find_Contents.
            \param handler the opcode handler (returned to user)
            \return TK_Error on failure. 
        */
        TK_Status StringToHandler (char const *type, BBaseOpcodeHandler **handler);

        /*! maps HOOPS strings to the opcodes that handle them.
            intended primarily (but not exclusively) for internal use.
            \param type the string returned from HOOPS' Find_Contents.
            \param opcode_out the opcode (returned to user), assigned to one of the TKE_Object_Types as defined in BOpcodeHandler.h
            \return TK_Error on failure. 
        */
        TK_Status StringToOpcode (char const *type, unsigned char *opcode_out);
#endif

    protected:
        //! internal use
        void            validate_hoops_version();
        //! internal use
        void            add_unnamed_segment (HC_KEY key) alter;
        //! internal use
        void            read_completed () alter;
        //! internal use
        void            empty_lists (void) alter;
        //! internal use
        TK_Status       tag (int variant) alter;
        //! internal use
        unsigned long   count_segment_objects (HC_KEY key, char const * start_segment = 0) alter;
};


#endif //HBINFILETK_TOOLKIT
