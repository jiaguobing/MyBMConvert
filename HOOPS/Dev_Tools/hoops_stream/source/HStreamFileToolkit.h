// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

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
    friend class HTK_Reference;
    friend class HTK_Texture;
	friend class HTK_Named_Style_Def;
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
    friend class HTK_Image;
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
class HBINFILETK_API2 Internal_Segment_Processor : public BBaseOpcodeHandler {
    protected:
        char *                  m_name;             /*!< name, if supplied, for current segment */
        HC_KEY                  m_key;              /*!< [original] key of current segment */
		bool					m_top_level;	    /*!< flag to indicate top-level segment processor */
        HC_KEY                  m_root;             /*!< key of start of current subtree */
        BBaseOpcodeHandler *    m_current_object;   /*!< object handler associated with current Hoops item */
        BBaseOpcodeHandler *    m_must_delete;      /*!< set to object handler if it is a clone to be deleted after use */
        HC_KEY                  m_item_key;         /*!< [original] Hoops key, if any, of current item */
        int                     m_saved_count;      /*!< count of multiple keys */
        HC_KEY *                m_saved_keys;       /*!< multiple key storage */
        HC_KEY                  m_renumbered;       /*!< user renumbered key value, if any, of current item */
        unsigned char           m_renumbered_scope; /*!< scope of renumbered key */
		int						m_priority;			/*!< priority of a subsegment or geometry */
		bool					m_has_priority;		/*!< existence of an explicit priority of a subsegment or geometry */
        int                     m_start_offset;     /*!< file offset of [revisited] segment */
		bool					m_searching;	    /*!< flag to indicate if search in progress */
        unsigned char			m_use_polypolylines;        /*!< A bitmask to indicate that polylines (single/double precision) in the current segment should be handled with HTK_PolyPolypoint, not the usual HTK_Polypoint */
        unsigned char			m_polylines_already_handled;  /*!< A bitmask to indicate that polylines (single/double precision) in the current segment have already been handled by polypolypoint (thus any additional "polyline" geometry should be skipped). */
		bool					m_geometry_attributes;	/*!< a flag to indicate that the geometry is (or should be) open to process attributes */
		bool					m_geometry_open;	/*!< a flag to indicate there are attributes attached to a geometry */
        BBaseOpcodeHandler *    m_deferred;			/*!< set to object handler of the geometry if we are preparing to handle local attributes */
		int						m_search_pass;		/*!< keep track of search context */
		bool					m_look_for_bounds;	/*!< content searches don't search for bounds, so this is for manually looking for explicitly set boundings */

        void    save_keys (HStreamFileToolkit & tk);
        void    restore_keys (HStreamFileToolkit & tk);

    public:
		Internal_Segment_Processor (ID_Key key = -1, char const * name = 0);
        ~Internal_Segment_Processor ();

        TK_Status   Read (BStreamFileToolkit & tk);
        TK_Status   Write (BStreamFileToolkit & tk);
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant = 0);
        TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special);
        void        Reset ();
        TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **) const;

        void        SetRoot (HC_KEY root)           { m_root = root;    }
        HC_KEY      GetRoot () const				{ return m_root;    }
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
    friend class HTK_Reference;
    friend class HTK_Texture;
    friend class HTK_Image;
	friend class HTK_Named_Style_Def;
	friend class HTK_Delete_Object;
    private:
        int                             m_hoops_version;        /* Version of Hoops we are using */
        Internal_Segment_List *         m_unnamed_segments;     /* Segments which need to be renamed to unnamed when able */

		HC_KEY *						m_excluded_segments;	/* keys of segments (if any) to skip during write */
		int								m_excluded_count;		/* size of exclusion array */

#ifndef HSTREAM_READONLY
        Internal_Segment_Processor *    m_segment_processor;    /* Handler for top segment & revisited segments */
        Internal_Pending_Tag *          m_pending_tags;         /* List of items to have tags attached. Tags should come absolutely last (even after user data), so tagging postponed to the end.  It is a list to allow for recursion if we ever need it */
        Internal_Texture_List *         m_texture_list;         /* Textures being processed */
        bool                            m_non_segment_revisits; /* Does the revisit list contain something other than segments? */
        int                             m_revisit_offset;       /* File offset of start of current revisit object (including compression if appropriate) */
		BBaseOpcodeHandler *			m_thumbnail;			/* handler for a thumbnail (if any) to write */
		int								m_local_attributes;		/* flag presence of geometry-level attributes (used in GenerateOne) */
		BBaseOpcodeHandler *			m_must_delete;			/* pointer to any temporary handler to discard when done */
		char *							m_prepend_root;			/* a string to prepend on rooted segments */
		int								m_revisit_state;		/* state of progress in revisit loop */

		void *							m_deferred_styles_ptr;
		void cleanup_deferred_styles();
		void apply_deferred_styles();

        bool issorted_revisit(Internal_Revisit_Item *array[], int count);
        void qsort_revisit(Internal_Revisit_Item ** first, Internal_Revisit_Item ** last) { qsort_revisit (first, last, 0); }
        void qsort_revisit(Internal_Revisit_Item **, Internal_Revisit_Item **, int);
        TK_Status sort_revisit();

        TK_Status find_textures();

		
		enum Revisit {
            Nothing,
            Working,
			Force
        };
#endif

    public:
        /*! 
           Default constructor for an HStreamFileToolkit object 
        */
        HStreamFileToolkit ();

        ~HStreamFileToolkit ();
        // clean out current state

        /*! 
        Initializes the file toolkit.  This is useful if a single toolkit object is to be used for dealing with different files, 
        rather than deleting the current toolkit object and creating a new instance.  Call this function after processing of the 
        current file is complete.
        */
        void        Restart ();

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
        TK_Status   GenerateBuffer (char * b, int s, int & f, char const * start_segment = 0);


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
        TK_Status   GenerateOneItemBuffer (char * b, int s, int & f, HC_KEY item);


        /*!
            Traverses the HOOPS database beginning at the currently open segment and estimates the number of objects that
            will be written by a matching call (or series of calls) to GenerateBuffer()

            A HOOPS/3dGS segment must currently be open before calling CountObjects, or a segment must be specified.

            \param start_segment Explicit name of a Hoops segment to process (current segment if null).
            \return The result of the function call.
        */
        unsigned POINTER_SIZED_INT   CountObjects (char const * start_segment = 0);


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


        /*!
            Registers an opcode handler which will be called by the toolkit after the header and before it gets into
	    other processing (and possibly compressing data).
            This allows the user to include a simple thumbnail image which an application might load as a preview for
	    things like file selection.
            \param handler A pointer to an BBaseOpcodeHandler object.
        */
        void        SetThumbnail (BBaseOpcodeHandler * handler);

        /*!
            Generates an opcode handler preset with the provided image data which will be called by the toolkit after the header and before it gets into
	    other processing (and possibly compressing data).
            This allows the user to include a simple thumbnail image which an application might load as a preview for
	    things like file selection.
            \param format format of the thumbnail. Formats are specified in #TKO_Thumbnail_Formats
			\param width width of the thumbnail image
			\param height height of the thumbnail image
			\param data array of thumbnail image data
        */
        void        SetThumbnail (int format, int width, int height, unsigned char const * data);

        /*!
            Writes the file associated with the toolkit (internal utility).
            \return The result of the function call.
        */
		TK_Status	Write_Stream_File ();

		/*!
			Adds to the list of keys indicating segments to skip during write
			\param count the number of keys in the array
			\param keys the array of keys of segments
		*/
		void		AddExcludedSegments (int count, HC_KEY const keys[]);
		/*!
			Adds one key to the list of segments to skip during write
			\param key the key of one segment
		*/
		void		AddExcludedSegment (HC_KEY key) { AddExcludedSegments (1, &key); }
		/*!
			Determines if a particular segment key is on the exclusion list
			\param key key of the segment to be checked
			\return true if the segment is to be excluded
		*/
		bool		SegmentIsExcluded (HC_KEY key) const;
		/*!
			Clears the list of excluded segments;
		*/
		void		ClearExcludedSegments ();
		/*!
			Gets the current number of keys of excluded segments
			\return number of keys
		*/
		int			GetExcludedSegmentCount () const	{ return m_excluded_count;			}
		/*!
			Gets the array of keys of excluded segments
			\return the array of keys of excluded segments
		*/
		HC_KEY const *	GetExcludedSegments () const	{ return m_excluded_segments;		}

		/*!
			Sets a segment to prepend to rooted segments well reading
			\param segment name of segment
		*/
		void SetPrependRootSegment(const char * segment);
		/*!
			Gets the segment to prepend to rooted segments well reading
			\param segment name of segment
		*/
		const char * GetPrependRootSegment() { return m_prepend_root; }


        /*! See if we can match a previous ExRef and just include the segment it generated
            \return true if the function matched and handled the reference, false if we need to try to read the reference
        */
		bool	MatchPreviousExRef () const;

		char	const						**m_search_strings;

    protected:
        //! internal use
        void            validate_hoops_version();
        //! internal use
        void            add_unnamed_segment (HC_KEY key);
        //! internal use
        void            read_completed ();
        //! internal use
        void            empty_lists ();
        //! internal use
        TK_Status       tag (int variant);
        //! internal use
        unsigned HLONG   count_segment_objects (HC_KEY key, char const * start_segment = 0);

		virtual void FileDone() {
			apply_deferred_styles();
		}

};


#endif //HBINFILETK_TOOLKIT
