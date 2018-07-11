// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _H_STREAMER_H_
#define _H_STREAMER_H_

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif


#include "HTools.h"
#include "HStream.h"


class HStreamerTK;
class HStreamFileToolkit;
class HStreamerBuffer;
class HTClient;
class Pause_Info;
struct vlist_s;
struct vhash_s;

/*! 
    \var typedef void (*HStreamerConditionNoticeFunction)(unsigned int condition, void * user_data)
    \brief Type definition for the function to which notification about errors and other conditions are sent
*/
typedef void (*HStreamerConditionNoticeFunction)(unsigned int condition, void * user_data);

/*! \def HSTREAMER_CONDITION_COMPLETE_READ_PAUSES
    \brief The streamer has read all the pauses from the hsf file
*/
#define HSTREAMER_CONDITION_COMPLETE_READ_PAUSES	0

/*! \def HSTREAMER_CONDITION_COMPLETE_SWEETENING
    \brief The streamer has completed the previous request for sweetening
*/
#define HSTREAMER_CONDITION_COMPLETE_SWEETENING		2

/*! \def HSTREAMER_CONDITION_VIEW_UPDATE
    \brief The application should update the view
*/
#define HSTREAMER_CONDITION_VIEW_UPDATE				3

/*! \def HSTREAMER_CONDITION_NO_DICTIONARY
    \brief No dictionary was found when expected will continue reading normally
*/
#define HSTREAMER_CONDITION_NO_DICTIONARY			4

/*! \def HSTREAMER_CONDITION_VERSION_MISMATCH
    \brief File hsf version is newer than readers version will try to read anyway
*/
#define HSTREAMER_CONDITION_VERSION_MISMATCH		5

/*! \def HSTREAMER_CONDITION_COMPLETE_READ_DICTIONARY
    \brief The streamer has read the dictionary from the hsf file
*/
#define HSTREAMER_CONDITION_COMPLETE_READ_DICTIONARY	6



//! The HStreamer class provides functionality for on-demand and view-dependent streaming of HSF files
/*!
 HStreamer is a high-level class which encapsulates the details of dealing with the HOOPS/Stream and HOOPS/Net
 modules to achieve on-demand or view-dependent streaming.
 
 Usage: 
 Create an instance to this class. Call function Setup() to initialize it. Use the function LoadFile to load the file 
 for streaming. Now you can call any of it's high-level functions to sweeten (streaming more) the entities.
*/ 
class HStreamer
{
	friend class HTK_HStreamer_Shell;

public:

    /*!
	Constructs the HStreamer object
	*/
	HStreamer();

	/*!
	  Destructor. Deletes and frees the memory being used.
	*/
	~HStreamer();

	/*!
	Initializes the HStreamer object. This function has to be called before you start streaming using 
	the other interfaces of this object.

	\param model_key A key to HOOPS segment where the streamed data is to be inserted

	\param net_client A pointer to the HNetClient object which is associated with the directory on the
	server. This directory has the file which is requested for streaming.  If not provided streamer is
	for local files only.

	\param n_pause number of pauses (TK_Pause marker) in the HSF file to read to when the file is first loaded.
	Setting to 0 will read the complete file. 1 reads the segment structure and 2 reads the simplified geometry.
	*/
	void Setup( HC_KEY model_key, int n_pause=0 );

	/*!
	  Sets up a callback function to provide condition and error notices.
	  \param condition_notice_helper_function A pointer to function which will receive the notices and error
	  \param user_data	A void pointer to the pass through data you might be interested in receiving with the notices
	*/
	void SetConditionNoticeFunction( HStreamerConditionNoticeFunction condition_notice_helper_function,
										void * user_data);

	/*!
	  Begins streaming of the file
	  \param filename name of the file to stream in.
	*/
	void LoadFile( const char * filename );

	/*!
	Streams the geometry for given HOOPS Segment key.
	\param key HOOPS segment key
	*/
	void Sweeten(HC_KEY key);

	/*!
	  Streams the geometry which falls within the view of given camera position
	  \param camera_position A HPoint object representing camera position
	  \param camera_target A HPoint object representing camera target
	  \param camera_up A HPoint object representing view up vector
	  \param field Two floats representing camera field
	  \param projection camera projection
	*/
	void Sweeten( const HPoint& camera_position, 
				  const HPoint& camera_target, 
				  const HPoint& camera_up, 
				  const float field[2], 
				  const char * projection );



	/*! Returns a pointer to the HStreamToolkit object this streamer is associated with */
	HStreamFileToolkit * GetStreamFileToolkit() { return (HStreamFileToolkit*) m_ptk; };

	/*!
	Called by the MVO timer to poll the buffer que for reading
	\return false if the timer is required to be stopped
	*/
	bool ReadStreamBufferQue();

private:

	// cleans up the lists which contain new'ed objects
	void FreeKeyLodPairList(vlist_s*& plist);	

	// sweeten the next batch of entities on pending list
	void SweetenBatch();							
	// sweeten entities on the unknown-size list
	void SweetenNext();								
	
	// walks the HOOPS database begining at the start key to add each shell 
	// and mesh to the sweeten list for sweetening
	TK_Status TreeWalker (HC_KEY start_key, BStreamFileToolkit const * tk , int lod_level = -1);	

	// walks the HOOPS database begining at the start key and adds the geometry 
	// to the sweeten list for sweetening if it falls in the view of the camera supplied
	TK_Status TreeWalker(	HC_KEY start_key, 
							BStreamFileToolkit const * tk , 
							const HPoint& position, 
							const HPoint& target, 
							const HPoint& up,
							const float field[2], 
							const char * projection,
							int lod_level = -1);

	// recursive utility function to walk HOOPS database
	TK_Status Walk (BStreamFileToolkit const * tk, float const * in_matrix, int test_case, int lod_level);
	
	// parses the buffer in to the model key
	TK_Status ParseBuffer(HStreamerTK * tk, HStreamerBuffer * block);	
	// parses the pause information 
	TK_Status ParsePauseInfo(HStreamerBuffer * block);								

	// notifies the condition/error notice to the registered handler
	void NotifyCondition(unsigned int condition);									

	enum parse_stage {
		parse_Header,
		parse_FileInfo,
		parse_PauseInfo,
		parse_AllPauses,
		parse_Dictionary,
		parse_Single,
		parse_Normal,
		parse_Error
	};

	// the streamtoolkit to use for streaming
	HStreamerTK * m_ptk;				
	// MVO timer client registered with HTManager to recieve timer events
	HTClient * m_pBufferPollTimer;	

	int			m_nPauseJump;			// initial read to this pause in the streamfile
	vlist_s *	m_pDiscardedBlockIds;	// stream buffers requested but not of any significance any more
	vlist_s *	m_pSweetenList;			// list of shells for sweetening
	vlist_s *	m_pPendingSweetenList;	// list of shells which are requested for sweetening and are pending
	vhash_s *	m_pAlreadySweetenedList;// shells already sweetened	
	vlist_s *	m_pUnknownSizeList;		// shells of unknown size
	parse_stage m_ParseStage;			// current parsing state
	Pause_Info*	m_PauseInfo;			// information about pause in the current hsf file
	HC_KEY		m_model_key;			// the segment key where the file has to be read

	// callback function for condition and error notices
	HStreamerConditionNoticeFunction	m_condition_notice_helper_function;	
	// user data to be passed to the condition notice function
	void *								m_condition_notice_user_data;	

	// the id of the last of a batch of requests
	int m_sweeten_end_id;

};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



