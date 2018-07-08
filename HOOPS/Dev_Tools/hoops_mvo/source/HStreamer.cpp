// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "HStreamer.h"

#include "HOpcodeShell.h"
#include "HStream.h"

#include "HUtility.h"
#include "HTManager.h"
#include "HDB.h"

#include "vlist.h"
#include "vhash.h"

#include <stdlib.h>
#include <stdio.h>

#define BUFFER_CHUNK_SIZE 32*1024

class HStreamerBuffer
{
public:
	HStreamerBuffer( unsigned int id, unsigned int size, const char * data) {

		m_id = id;
		m_real_size = m_size = size;
		m_buffer_handle = 0;

		if(m_size) {

			if(!m_buffer_handle){
				m_real_data=m_data = new char[m_size];
				memcpy((char*)m_data, data, m_size);
			}else{
				m_real_data=m_data=data;
			}
		}
		else
			m_real_data=m_data=0;
	}

	~HStreamerBuffer() {
		if(m_real_data) 
			delete[] (char*)m_real_data;
	}

	void AdjustUnusedBuffer(unsigned int unused) {

		m_data += m_size-unused;
		m_size = unused;
	}
	
	unsigned int m_id;
	unsigned int m_size;
	const char * m_data;
	unsigned int m_real_size;
	const char * m_real_data;
	void * m_buffer_handle;
};


class HStreamerTK : public HStreamFileToolkit
{

public:

	enum {
		ReadPauseInfo,
		ReadDictionary,
		ReadNothing
	};
	
	HStreamerTK();
	virtual ~HStreamerTK();

    void Restart (void);

	// HNet related
	static void file_data_input_function(
			const char * message, unsigned int message_length, 
			unsigned int file_data_id, void * user_data);

	// HTKStreamFileToolkit overrides
	virtual TK_Status ReadBuffer (char * buffer, int size, int & data_read);
    virtual TK_Status PositionFile (int offset);
	virtual TK_Status OpenFile (char const * name, bool write);
    virtual TK_Status CloseFile (void);

    //we originally had these two as just one routine with a default argument, but the IRIX compiler doesn't 
    //allow a defaulted int routine to overload something with void args.
    virtual TK_Status LocateDictionary (int type);
    virtual TK_Status LocateDictionary () { return LocateDictionary(ReadDictionary); };

	HStreamerBuffer* GetFirstBuffer();
	HStreamerBuffer* PeekFirstBuffer();
	void AddBufferFirst(HStreamerBuffer* buffer);
	void AddBufferLast(HStreamerBuffer* buffer);

	int m_loc_dic_data_id;
	int m_dic_offset;
	int m_dic_type;

private:

	char m_tkFile[4096];
	vlist_s* m_pBufferQue;

	FILE * m_file;
	long m_next_data_id;

};

HStreamerTK::HStreamerTK()
{
	strcpy(m_tkFile,"");
	m_pBufferQue = new_vlist(malloc, free);
	m_loc_dic_data_id = 0;
	m_dic_offset = 0;
	m_dic_type = ReadDictionary;
	m_file = NULL;
	m_next_data_id=1;
}

HStreamerTK::~HStreamerTK()
{
	HStreamerBuffer* buffer;
	while((buffer = (HStreamerBuffer*) vlist_remove_first(m_pBufferQue))) delete buffer;
	delete_vlist(m_pBufferQue);

	if(m_file) fclose(m_file);
}

void HStreamerTK::Restart (void) 
{
	HStreamFileToolkit::Restart();

	HStreamerBuffer* buffer;
	while((buffer = (HStreamerBuffer*) vlist_remove_first(m_pBufferQue))) delete buffer;
	delete_vlist(m_pBufferQue);
	m_pBufferQue = new_vlist(malloc, free);

	PositionFile(0);

	m_loc_dic_data_id = 0;
	m_dic_offset = 0;
	m_dic_type = ReadDictionary;

	if(m_file) {
		fclose(m_file);
		m_file=NULL;
	}
}

HStreamerBuffer* HStreamerTK::GetFirstBuffer()
{
	HStreamerBuffer* buffer;
	buffer = (HStreamerBuffer*) vlist_remove_first(m_pBufferQue);
	return buffer;
}

HStreamerBuffer* HStreamerTK::PeekFirstBuffer()
{
	HStreamerBuffer* buffer;
	buffer = (HStreamerBuffer*) vlist_peek_first(m_pBufferQue);
	return buffer;
}


void HStreamerTK::AddBufferFirst(HStreamerBuffer* buffer)
{
	vlist_add_first( m_pBufferQue, (void*) buffer);
}

void HStreamerTK::AddBufferLast(HStreamerBuffer* buffer)
{
	vlist_add_last( m_pBufferQue, (void*) buffer);
}

void HStreamerTK::file_data_input_function(
		const char * message, unsigned int message_length, 
		unsigned int file_data_id, void * user_data)
{
	HStreamerTK* pHNetTk = (HStreamerTK*)user_data;
	pHNetTk->AddBufferLast(new HStreamerBuffer(file_data_id, message_length, message));
}

TK_Status HStreamerTK::OpenFile (char const * name, bool write) 
{
	strcpy(m_tkFile, name);
	PositionFile(0);

	if(m_file) fclose(m_file);

	m_file = fopen(m_tkFile, "rb");

	if(m_file == NULL) 
		return Error("Unable to access file");

    return TK_Normal;
}

TK_Status HStreamerTK::CloseFile (void)
{
	strcpy(m_tkFile,"");

	if(m_file) {
	   	fclose(m_file);
		m_file = NULL;
	}

    return TK_Normal;
}

TK_Status HStreamerTK::ReadBuffer (char * buffer, int s_size, int & data_id)
{
	unsigned int size = s_size;
	
	// if the offset we have is something real large number like ~0, 
	// then we know that we have read past the end of file. 
	//if( GetFileOffset() < 0 )
 //       return Error ("read past end of file");

	int error;

	if(position() == 0){
		error = fseek(m_file, GetFileOffset(), SEEK_SET);
	}else{
		error = fseek(m_file, -(long)GetFileOffset(), SEEK_END);
	}

	if(error){
		;
	}

	data_id = m_next_data_id++;

READ_MORE_STUFF:
	if(size){

		size_t size_read;
		char buffer[BUFFER_CHUNK_SIZE];

		if(size>BUFFER_CHUNK_SIZE)
			size_read = fread((void*) buffer, 1, BUFFER_CHUNK_SIZE, m_file);
		else
			size_read = fread((void*) buffer, 1, size, m_file);

		if(size_read == 0) {
			size=0;
			goto READ_MORE_STUFF;
		}

		size-=(unsigned int)size_read;

		file_data_input_function((const char *)buffer, (unsigned int)size_read, data_id, (void *)this);

		goto READ_MORE_STUFF;

	}else{

		file_data_input_function((const char *)0, 0, data_id, (void *)this);
	}

	SetFileOffset(GetFileOffset()+size);
	return TK_Normal;
}

/*!
    Instructs the BStreamFileToolkit object to sets its internal file pointer to the 
	location of the file dictionary.  The dictionary contains entries for the file 
	locations of various representations of entities.

    Objects which are derived from BStreamFileToolkit may reimplement to support locating 
	a dictionary for a file that resides on a remote location such as a web server.  This 
	might involve passing a message that instructs the server process to locate the dictionary.
    \return The result of function call -- either TK_Normal or TK_Error.
*/
TK_Status HStreamerTK::LocateDictionary (int dic_type) 
{
    TK_Status status = TK_Normal;
	m_dic_type = dic_type;
	int unused;

    if (!(GetWriteFlags() & TK_Generate_Dictionary))
        return Error ("file not generated with dictionary");

	if(m_dic_offset){

		if(m_dic_type == HStreamerTK::ReadPauseInfo){
			// we just want to read the pauses and not the complete dictionary
			PositionFile(m_dic_offset);
			ReadBuffer(0, 36, unused);
		}else if(m_dic_type == HStreamerTK::ReadDictionary){
			// fire the request to read the complete dictionary
			PositionFile(m_dic_offset);
			ReadBuffer(0, ~0U, unused);
		}

	}else{

		if(!m_loc_dic_data_id){
			PositionFile(-5);
			ReadBuffer (0, 5, m_loc_dic_data_id);
		}
	}

	return status;
}

/*!
    Utility function which repositions the current file pointer.  This acts a wrapper for fseek, and is used by 
    Locate Dictionary and LocateEntity, but might be reimplemented by classes derived from BStreamFileToolkit
    to perform custom file repositioning.

    \param offset If positive, repositions the file pointer to offset bytes from the beginning of the file, otherwise
    repositions the file pointer to offset bytes from the end of the file. 
    \return The result of function call -- either TK_Normal or TK_Error.
*/
TK_Status HStreamerTK::PositionFile (int offset) 
{

    if (offset >= 0) {
		set_position(0);
		SetFileOffset(offset);
	}else{
		set_position(1);
		SetFileOffset(abs(offset));
	}

    return TK_Normal;
}

struct Key_Lod_Pair 
{
	HC_KEY key;
	int lod;

	Key_Lod_Pair (HC_KEY k, int l) : key (k), lod (l) {}
};

///////////////////////////////////////////////
// Class Pause_Info
class Pause_Info
{
public:
	// offset value of each pause
	int dictionary_format;
	vlist_s* pause_offsets;

	Pause_Info(){ 
	pause_offsets = new_vlist(malloc, free);
	dictionary_format = 0;
	};

	~Pause_Info(){
	delete_vlist(pause_offsets);
	};

	void Clear(){
	dictionary_format = 0;
	vlist_flush(pause_offsets);
	};
};


///////////////////////////////////////////////
// Class HTK_HStreamer_Shell
class HTK_HStreamer_Shell : public HTK_Shell
{
public:
	HTK_HStreamer_Shell(HStreamer * streamer) : HTK_Shell()
	{
		m_pStreamer = streamer;
	}

	TK_Status Execute (BStreamFileToolkit & tk)
	{
		TK_Status status = HTK_Shell::Execute(tk);

		if( !(mp_subop & TKSH_BOUNDING_ONLY) && (GetPointCount() != 0) )
		{
			if(m_pStreamer->m_pAlreadySweetenedList)
				vhash_replace_item(m_pStreamer->m_pAlreadySweetenedList, (void*) GetKey(), (void*) GetLodLevel(), 0);
		}

		return status;
	}
	TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **newhandler) const
	{
		*newhandler = new HTK_HStreamer_Shell(m_pStreamer);
		if (*newhandler != 0)
			return TK_Normal;
		else
			return tk.Error( "memory allocation in HTK__Net_Shell::clone failed" );
	}

protected:
	HStreamer * m_pStreamer;
};


///////////////////////////////////////////////
// Class HStreamer

// stream buffer poller timer callback 
bool read_stream_buffer_timer_function(float request_time, float actual_time, void * user_data)
{
	HStreamer * streamer = (HStreamer*) user_data;
	H_ASSERT(streamer);

	return streamer->ReadStreamBufferQue();
}


HStreamer::HStreamer()
{
	m_ptk					= 0;
	m_pBufferPollTimer		= 0;

	m_nPauseJump			= 0;
	m_pDiscardedBlockIds	= 0;
	m_pSweetenList			= 0;
	m_pPendingSweetenList	= 0;
	m_pAlreadySweetenedList = 0;
	m_pUnknownSizeList		= 0;
	m_ParseStage			= parse_Header;
	m_PauseInfo				= new Pause_Info();
	m_model_key				= INVALID_KEY;

	m_condition_notice_user_data		= 0;
	m_condition_notice_helper_function	= 0;
}

HStreamer::~HStreamer()
{
	// get rid of the timer
	if( m_pBufferPollTimer ) {
		HDB::GetHTManager()->UnRegisterClient( m_pBufferPollTimer );
		delete m_pBufferPollTimer;
		m_pBufferPollTimer = 0;
	}

	if( m_PauseInfo ) {
		delete m_PauseInfo;
		m_PauseInfo = 0;
	}

	if( m_ptk ) {
		delete m_ptk;
		m_ptk = 0;
	}

	if(m_pUnknownSizeList) 
		FreeKeyLodPairList(m_pUnknownSizeList);

	if(m_pSweetenList) 
		FreeKeyLodPairList(m_pSweetenList);

	if(m_pPendingSweetenList) 
		FreeKeyLodPairList(m_pPendingSweetenList);

	if( m_pAlreadySweetenedList) {
		delete_vhash(m_pAlreadySweetenedList);
		m_pAlreadySweetenedList = 0;
	}

	if( m_pDiscardedBlockIds ) {
		delete_vlist(m_pDiscardedBlockIds);
		m_pDiscardedBlockIds = 0;
	}

}
	
void HStreamer::Setup( HC_KEY model_key, int n_pause )
{
	H_ASSERT( (n_pause >= 0) && (n_pause <= 2) );

	m_sweeten_end_id = 0;
	m_model_key		= model_key;
	m_nPauseJump	= n_pause;
	m_ptk			= new HStreamerTK();
}

void HStreamer::SetConditionNoticeFunction( HStreamerConditionNoticeFunction condition_notice_helper_function, void * user_data)
{
	m_condition_notice_helper_function	= condition_notice_helper_function;
	m_condition_notice_user_data		= user_data;
}

void HStreamer::FreeKeyLodPairList(vlist_s*& plist)
{
	if( plist )
	{
		Key_Lod_Pair * kl_pair = 0;
		while((kl_pair = (Key_Lod_Pair *) vlist_remove_first(plist)))
			delete kl_pair;

		delete_vlist(plist);
		plist = 0;
	}
}

void HStreamer::LoadFile( const char * filename ) 
{
	int unused;

	m_ptk->SetOpcodeHandler (TKE_Shell, new HTK_HStreamer_Shell(this));

	if( m_pDiscardedBlockIds )
		delete_vlist(m_pDiscardedBlockIds);
	m_pDiscardedBlockIds = new_vlist(malloc, free);

	if(m_pUnknownSizeList)
		FreeKeyLodPairList(m_pUnknownSizeList);

	if(m_pSweetenList)
		FreeKeyLodPairList(m_pSweetenList);

	if(m_pPendingSweetenList)
		FreeKeyLodPairList(m_pPendingSweetenList);

	if(m_pAlreadySweetenedList)
		delete_vhash(m_pAlreadySweetenedList);
	m_pAlreadySweetenedList = new_vhash(1024, malloc, free);


	//StartPollingBufferQue create and register the timer
	if(!m_pBufferPollTimer)
		m_pBufferPollTimer = new HTClient( 0.01f, HTCS_PeriodicSkip, read_stream_buffer_timer_function, this );

	HDB::GetHTManager()->RegisterClient( m_pBufferPollTimer );

	// understand the file extension - if unknown better return
	char file_ext[MVO_BUFFER_SIZE]; 
	HUtility::FindFileNameExtension(filename, file_ext);
	if(strlen(file_ext) == 0 || !streq(file_ext,"hsf")) {
		return;
	}

	HStreamFileToolkit* tk = m_ptk;

	// setup the stream toolkit nicely
	tk->Restart();

	// ask the binary file toolkit to open the file
	tk->OpenFile(filename);

	tk->PositionFile(0);

	if( m_nPauseJump <= 0 ) {

		// if the user doesn't have any specific pause to 
		// jump to then read the complete file in one shot
		m_ParseStage = parse_Normal;
		m_ptk->ReadBuffer(0, ~0, unused);

	}else{

		m_ParseStage = parse_Header;
		m_ptk->ReadBuffer(0, 128, unused);
		m_ptk->LocateDictionary(HStreamerTK::ReadNothing);
	}

}


bool HStreamer::ReadStreamBufferQue()
{
	static TK_Status status = TK_Normal;
	int unused;

	while(1) {

		HStreamerBuffer* block = m_ptk->GetFirstBuffer();

		if(!block) break;

		if((int)block->m_id == m_ptk->m_loc_dic_data_id) {

			if(block->m_size == 0) {
				m_ptk->m_loc_dic_data_id = 0;
			}
			else if(block->m_data[4] != (char) TKE_Termination){
				// minor sanity check -- last byte should be Termination
				m_ptk->Error ("file does not end correctly");

			}else{
				const unsigned char * cm = (const unsigned char *) block->m_data;

				m_ptk->m_dic_offset = (cm[3]<<24) | (cm[2]<<16) | (cm[1]<<8) | (cm[0]<<0);

				if(m_ptk->m_dic_type == HStreamerTK::ReadPauseInfo){
					// we just want to read the pauses and not the complete dictionary
					m_ptk->PositionFile(m_ptk->m_dic_offset);
					m_ptk->ReadBuffer(0, 36, unused);
				}else if(m_ptk->m_dic_type == HStreamerTK::ReadDictionary){
					// fire the request to read the complete dictionary
					m_ptk->PositionFile(m_ptk->m_dic_offset);
					m_ptk->ReadBuffer(0, ~0U, unused);
				}
			}

		}
		else if( block->m_size == 0) {
		
			if(status == TK_Pending && m_ParseStage != parse_Normal) {

				// make a read request for more only if we are not in parse_Normal mode 
				// in which case we have already made our read requests
				m_ptk->ReadBuffer(0, BUFFER_CHUNK_SIZE, unused);

			} 
			else if((int)block->m_id == m_sweeten_end_id ) {

				// if there is nothing in sweeten list, but we know we 
				// must sweeten more things from the scene
				if( !m_pSweetenList) {

					// nothing in sweeten list
					if( m_pPendingSweetenList ) {

						// we do have pending list - that's our new sweeten list
						m_pSweetenList = m_pPendingSweetenList;
						m_pPendingSweetenList = 0;
					}else
						NotifyCondition(HSTREAMER_CONDITION_COMPLETE_SWEETENING);
				}

				if(m_pSweetenList && vlist_count(m_pSweetenList)) 
					SweetenBatch();
			}

		}else{
			
			if(vlist_item_exists(m_pDiscardedBlockIds, (void*) block->m_id)){

				// we wont recieve any more blocks with this id
				// why not tidy up our discard blocks list
				if(!block->m_size)
					vlist_remove( m_pDiscardedBlockIds, (void*) block->m_id);
			}else{

				if(block->m_size)
					status = ParseBuffer(m_ptk, block);
			}
		}

		H_ASSERT( status != TK_Error );

		//block may be back on queue
		if(m_ptk->PeekFirstBuffer() != block)
			delete block;
	}

	if( status == TK_Complete) {
		NotifyCondition(HSTREAMER_CONDITION_VIEW_UPDATE);
		status = TK_Normal;
	}

	return true;
}


void HStreamer::Sweeten(HC_KEY key)
{
	if(m_pUnknownSizeList)
		FreeKeyLodPairList(m_pUnknownSizeList);

	if(m_pSweetenList)
		FreeKeyLodPairList(m_pSweetenList);

	if(m_pPendingSweetenList)
		FreeKeyLodPairList(m_pPendingSweetenList);

	m_pSweetenList = new_vlist(malloc, free);

	TreeWalker(key, GetStreamFileToolkit() );

	if(vlist_count(m_pSweetenList)) 
		SweetenBatch();
}


void HStreamer::Sweeten(const HPoint& camera_position, 
						const HPoint& camera_target, 
						const HPoint& camera_up, 
						const float field[2], 
						const char * projection )
{
	if(m_pUnknownSizeList)
		FreeKeyLodPairList(m_pUnknownSizeList);

	if(m_pSweetenList)
		FreeKeyLodPairList(m_pSweetenList);

	if(m_pPendingSweetenList)
		FreeKeyLodPairList(m_pPendingSweetenList);

	m_pSweetenList = new_vlist(malloc, free);

	TreeWalker( m_model_key, m_ptk, camera_position, camera_target, camera_up, field, projection );

	if(vlist_count(m_pSweetenList)) 
		SweetenBatch();
}

void HStreamer::SweetenBatch()
{

	if(!m_pUnknownSizeList)
		m_pUnknownSizeList = new_vlist(malloc, free);

	Key_Lod_Pair * shell_to_sweeten = 0;
	unsigned long requested_bytes = 0;
	bool read_requested = false;

	HStreamFileToolkit * tk = GetStreamFileToolkit();
	H_ASSERT(tk);

	while((shell_to_sweeten = (Key_Lod_Pair *) vlist_remove_first(m_pSweetenList))) {

		TK_Status stat = TK_Normal;
		// locate the entity and kick off the read, rest will be taken care
		// by ReadBufferQue and ParseBuffer functions
		//HC_KEY shell_to_sweeten = (HC_KEY)vlist_remove_first(m_pSweetenList);
		stat = tk->LocateEntity( shell_to_sweeten->key, shell_to_sweeten->lod );
		H_ASSERT( stat == TK_Normal );

		// we have a smart dictionary in place now, lets try to get the estimate of
		// size of this entity and request the appropriate amount of data from the server
		int offset = 0;
		int ent_size = 0;

		stat = tk->GetOffset(shell_to_sweeten->key, shell_to_sweeten->lod, offset, ent_size);

		if(ent_size) {

			read_requested = true;
			tk->ReadBuffer(0, ent_size, m_sweeten_end_id);
			vhash_replace_item(m_pAlreadySweetenedList, (void*) shell_to_sweeten->key, (void*) shell_to_sweeten->lod, 0);

			if( shell_to_sweeten->lod != 0 ) {
				shell_to_sweeten->lod--;
				if( !m_pPendingSweetenList )
					m_pPendingSweetenList = new_vlist(malloc, free);
				vlist_add_last(m_pPendingSweetenList, shell_to_sweeten);
			}
			else
				delete shell_to_sweeten;

			requested_bytes+=ent_size;

			if(requested_bytes>65535) break;

		}else{

			vlist_add_last(m_pUnknownSizeList, (void*)shell_to_sweeten);
		}
	}

	if(!vlist_count(m_pSweetenList)) {

		FreeKeyLodPairList(m_pSweetenList);

		if(!vlist_count(m_pUnknownSizeList))
			FreeKeyLodPairList(m_pUnknownSizeList);
		else 
			if(!read_requested) SweetenNext();
	}
}

void HStreamer::SweetenNext()
{
	if( m_pSweetenList && vlist_count(m_pSweetenList))
		return;

	if( !m_pUnknownSizeList )
		return;

	if( vlist_count(m_pUnknownSizeList) <= 0 )
		return;


	HStreamFileToolkit * tk = GetStreamFileToolkit();
	H_ASSERT(tk);

	TK_Status stat = TK_Normal;
	int amt_read = 0;
	// locate the entity and kick off the read, rest will be taken care
	// by ReadBufferQue and ParseBuffer functions
	Key_Lod_Pair * shell_to_sweeten = (Key_Lod_Pair *)vlist_remove_first(m_pUnknownSizeList);
	stat = tk->LocateEntity( shell_to_sweeten->key, shell_to_sweeten->lod );
	H_ASSERT( stat == TK_Normal );

	// we have a smart dictionary in place now, lets try to get the estimate of
	// size of this entity and request the appropriate amount of data from the server
	int offset = 0;
	int ent_size = 0;
	stat = tk->GetOffset(shell_to_sweeten->key, shell_to_sweeten->lod, offset, ent_size);
	if( ent_size != 0 )
		tk->ReadBuffer(0, ent_size, amt_read);
	else
		tk->ReadBuffer(0, BUFFER_CHUNK_SIZE, amt_read);
	vhash_replace_item(m_pAlreadySweetenedList, (void*) shell_to_sweeten->key, (void*) shell_to_sweeten->lod, 0);
	delete shell_to_sweeten;

}


TK_Status HStreamer::ParseBuffer(HStreamerTK * tk, HStreamerBuffer * block)
{
	TK_Status status = TK_Normal;
	int unused = 0;

	HC_Open_Segment_By_Key(m_model_key);
	switch( m_ParseStage )
	{
		case parse_Header :	
		{
			bool version_mismatch = false;
			status = tk->ParseBuffer (block->m_data, block->m_size, TK_Single);
			if (status == TK_Version) 
			{
				version_mismatch = true;

				NotifyCondition(HSTREAMER_CONDITION_VERSION_MISMATCH);

				// try again, we might not trip over anything...
				tk->Restart();
				tk->SetReadFlags (tk->GetReadFlags() | TK_Ignore_Version);
				status = tk->ParseBuffer (block->m_data, block->m_size, TK_Pause);
			}
			if (status == TK_Error) 
			{
				m_ParseStage = parse_Error;  
				return status;
			}
			if( status == TK_Complete )
			{
				if(tk->Unused()){
					block->AdjustUnusedBuffer(tk->Unused());
					m_ptk->AddBufferFirst(block); 
				}

				m_ParseStage = parse_FileInfo; 
			}
			break;
		}
		case parse_FileInfo:		
		{
			status = tk->ParseBuffer (block->m_data, block->m_size, TK_Single);
			if( status == TK_Complete )
			{
				// check if there is a dictionary
				if( (tk->GetWriteFlags() & TK_Generate_Dictionary) == TK_Generate_Dictionary) {

					// we don't need the rest of the chunk so discard
					vlist_add_last(m_pDiscardedBlockIds, (void*) block->m_id);

					m_ParseStage = parse_PauseInfo;	
					status = tk->LocateDictionary(HStreamerTK::ReadPauseInfo);

				} else {

					NotifyCondition(HSTREAMER_CONDITION_NO_DICTIONARY);

					if(tk->Unused()){
						block->AdjustUnusedBuffer(tk->Unused());
						m_ptk->AddBufferFirst(block); 
					}

					// fire a request to read everything
					tk->ReadBuffer(0, ~0, unused);
					m_ParseStage = parse_Normal;	
				}
			}
			break;
		}
		case parse_PauseInfo :	
		{
			// read the dictionary here
			status = ParsePauseInfo(block);
			if( status == TK_Complete ) {

				if( m_PauseInfo->dictionary_format >= 2 ) {

					m_ParseStage = parse_AllPauses; 

					int offset_till_last_pause = (int) (MVO_POINTER_SIZED_INT) vlist_peek_last(m_PauseInfo->pause_offsets);
					tk->PositionFile(0);
					tk->ReadBuffer(0, offset_till_last_pause, unused);

				}else{

					m_ParseStage = parse_Dictionary; 

					if(tk->Unused()){
						block->AdjustUnusedBuffer(tk->Unused());
						m_ptk->AddBufferFirst(block); 
					}
				}
			}
			break;
		}
		case parse_AllPauses : 
		{
			// read till the pause
			status = tk->ParseBuffer(block->m_data, block->m_size, TK_Pause);
			if(status == TK_Complete) {

				if(vlist_count(m_PauseInfo->pause_offsets) > 0)
					vlist_remove_first(m_PauseInfo->pause_offsets);

				if(vlist_count(m_PauseInfo->pause_offsets) > 0) {

					m_ParseStage = parse_AllPauses;	

					if(tk->Unused()){
						block->AdjustUnusedBuffer(tk->Unused());
						m_ptk->AddBufferFirst(block); 
					}

				}else{

					// we don't need the rest of the chunk so discard
					vlist_add_last(m_pDiscardedBlockIds, (void*) block->m_id);

					m_ParseStage = parse_Dictionary; 
					NotifyCondition(HSTREAMER_CONDITION_COMPLETE_READ_PAUSES);
					status = tk->LocateDictionary(HStreamerTK::ReadDictionary);
				}
			}
			break;
		}
		case parse_Dictionary : 
		{
			status = tk->ParseBuffer(block->m_data, block->m_size, TK_Single);

			if(status == TK_Complete) {


				m_ParseStage = parse_Single;	
				NotifyCondition(HSTREAMER_CONDITION_COMPLETE_READ_DICTIONARY);

				// we don't need the rest of the chunk so discard
				vlist_add_last(m_pDiscardedBlockIds, (void*) block->m_id);

				SweetenNext();
			}

			break;
		}
		case parse_Single :	
		{
			// read the entities here
			status = tk->ParseBuffer(block->m_data, block->m_size, TK_Single);

			if(status == TK_Complete) {

				// we don't need the rest of the chunk so discard
				vlist_add_last(m_pDiscardedBlockIds, (void*) block->m_id);
				SweetenNext();
			}
			break;
		}
		case parse_Normal :	
		{
			status = tk->ParseBuffer(block->m_data, block->m_size, TK_Normal);
			break;
		}
		case parse_Error :
		{
			status = TK_Error;
			break;
		}

	}
	HC_Close_Segment();

	return status;
}


TK_Status HStreamer::ParsePauseInfo(HStreamerBuffer * block)
{
	// ASSUMPTION: I AM ASSUMING THAT WE HAVE ATLEAST 12 BYTES IN THIS BLOCK TO
	// BE ABLE TO PARSE THE PAUSE INFORMATION. IF THIS IS NOT THE CASE, THEN WE NEED 
	// TO IMPLEMENT THE ACCUMULATION MECHANISM. - RAJESH B (4-SEPT-02)
	H_ASSERT( block->m_size >= 12 );
	if( block->m_size < 12 )
		return TK_Error;

	// we have enough bytes to try and parse
	if( block->m_data[0] != 'D')
		// this doesn't look like the start of a dictionary - something's wrong
		return TK_Error;

	m_PauseInfo->dictionary_format = block->m_data[1];
	if( m_PauseInfo->dictionary_format < 2 )
		// this dictionary format doesn't support pause information
		return TK_Complete;

	int n_pauses = block->m_data[2] | (block->m_data[3] << 8);
	if( n_pauses < m_nPauseJump )
	{
		// Non-fatal error - the file doesn't have as many pauses as 
		// we are expecting - format inconsistent???
		H_ASSERT(0);
		return TK_Complete;
	}

	unsigned char const * data_ptr = (unsigned char const *)&block->m_data[4];
	for( int i = 0; i < m_nPauseJump; i++ )
	{
		int offset = data_ptr[0] | (data_ptr[1] << 8) | (data_ptr[2] << 16) | (data_ptr[3] << 24);
		data_ptr += 4;
		vlist_add_last(m_PauseInfo->pause_offsets, (void*) offset);
	}


	return TK_Complete;
}


void HStreamer::NotifyCondition(unsigned int condition)
{
	if( m_condition_notice_helper_function )
		m_condition_notice_helper_function(condition, m_condition_notice_user_data);
}



enum Bounding_Test_Cases {
    Test_Outside,
    Test_Inside,
    Test_Intersects,
    Test_Unknown
};

enum Point_Test_Cases {
    Off_Left    = 0x01,
    Off_Right   = 0x02,
    Off_Bottom  = 0x04,
    Off_Top     = 0x08,
    Behind_Eye  = 0x10
};

static int transform_and_test_bounds (float const * bounds, float const * matrix) {
    float           test[3], x, y, w;
    int             i;
    int             common = ~0, any = 0, current;

    for (i=0; i<8; i++) {
        test[0] = bounds[(i & 1) ? 0 : 3];
        test[1] = bounds[(i & 2) ? 1 : 4];
        test[2] = bounds[(i & 4) ? 2 : 5];

        w = matrix[3] * test[0] + matrix[7] * test[1] + matrix[11] * test[2] + matrix[15];

        if (w < 0.01f)
            current = Behind_Eye;
        else {
            current = 0;
            w = 1.0f / w;

            x = (matrix[0] * test[0] + matrix[4] * test[1] + matrix[8] * test[2] + matrix[12]) * w;
            y = (matrix[1] * test[0] + matrix[5] * test[1] + matrix[9] * test[2] + matrix[13]) * w;

            if (x < -1.0f)
                current |= Off_Left;
            else if (x > 1.0f)
                current |= Off_Right;

            if (y < -1.0f)
                current |= Off_Bottom;
            else if (y > 1.0f)
                current |= Off_Top;
        }

        common &= current;
        any |= current;
    }

    if (common != 0)
        return Test_Outside;

    if (any != 0)
        return Test_Intersects;

    return Test_Inside;
}

TK_Status HStreamer::Walk (BStreamFileToolkit const * tk, float const * in_matrix, int test_case, int lod_level) 
{
    float           bounds[6];
    float           new_matrix[16];
    float const *   use_matrix = in_matrix;
    char            type[MVO_BUFFER_SIZE];
    HC_KEY          key = -1;
    TK_Status       status = TK_Normal;
    HC_KEY          current_segment = HC_KCreate_Segment (".");
    int             offset, length;

    // is this segment at least partially visible?
	// if inside, no need for bounding check here or below
    if (test_case != Test_Inside) {  

        if (tk->GetBounds (current_segment, bounds) == TK_Normal) {
            // transform and test the bounds
            if ((test_case = transform_and_test_bounds (bounds, use_matrix)) == Test_Outside)
                return TK_Normal;
        }
    }

	if (tk->GetOffset (current_segment, 0, offset, length) == TK_Normal && length > 0) {
	    // TODO: RECORD current_segment (if we haven't recorded or processed it already)
		// check if we already have sweetened these entities;
		if( !vhash_lookup_item(m_pAlreadySweetenedList, (void*) current_segment, 0 ))
			vlist_add_last(m_pSweetenList, (void*) new Key_Lod_Pair (current_segment, 0));
	}
	else {
		// mark this segment as "sweetened on the segment browser"
	}

    // now check subsegments
    if (test_case != Test_Inside && HC_Show_Existence ("modelling matrix")) {
        HC_Show_Modelling_Matrix (new_matrix);
        HC_Compute_Matrix_Product (new_matrix, in_matrix, new_matrix);
        use_matrix = new_matrix;
    }

    HC_Begin_Contents_Search (".", "subsegments, shells, meshes");
        while (HC_Find_Contents (type, &key)) {
            switch (type[0]) {
                case 's': 
					
					if (type[1] == 'e') {
						HC_Open_Segment_By_Key (key);
							status = Walk (tk, use_matrix, test_case, lod_level);
						HC_Close_Segment ();

						if (status != TK_Normal)
							return status;
						break;
					}

				// shells drop through to this case
				case 'm': {
					// transform and test the bounds if we can get the bounds
					if (test_case == Test_Inside || tk->GetBounds (key, bounds) != TK_Normal ||
						transform_and_test_bounds (bounds, use_matrix) != Test_Outside) {
						// record this shell/mesh for sweetening
						int		lod_to_try;

						if( lod_level != -1 )
							lod_to_try = lod_level;
						else if(vhash_lookup_item(m_pAlreadySweetenedList, (void*) key, (void**)&lod_to_try))
							--lod_to_try;
						else 
							lod_to_try = 7;

						while (lod_to_try >= 0) {
							if (tk->GetOffset (key, lod_to_try, offset, length) == TK_Normal && offset > 0) {
								vlist_add_last(m_pSweetenList, (void*)new Key_Lod_Pair (key, lod_to_try));
								break;
							}
							--lod_to_try;
						}

					}

				}	break;

                case 'i': {
                    char        included[MVO_BUFFER_SIZE];

                    // use full pathname instead of key to avoid problems with local renumbering
                    HC_Show_Include_Segment (key, included);

                    HC_Open_Segment (included);
                        status = Walk (tk, use_matrix, test_case, lod_level);
                    HC_Close_Segment ();

                    if (status != TK_Normal)
                        return status;
                }   break;

                default: {
                    status = tk->Error ("Unexpected search result");
                    goto end_search;
                }   //nobreak;
            }
        }
      end_search:
    HC_End_Contents_Search ();

    return status;
}




TK_Status HStreamer::TreeWalker (HC_KEY start_key, BStreamFileToolkit const * tk , int lod_level)
{
    TK_Status       status;

    HC_Open_Segment_By_Key (start_key);
        status = Walk (tk, 0, Test_Inside, lod_level);
    HC_Close_Segment ();

    return status;
}


TK_Status HStreamer::TreeWalker(HC_KEY start_key, BStreamFileToolkit const * tk , 
								const HPoint& position, 
								const HPoint& target, 
								const HPoint& up,
								const float field[2], 
								const char * projection,
								int lod_level)
{
    float           pos[16], proj[16], complete[16];
    float           eye[3], horiz[3], eye_distance;
    TK_Status       status;

//	float field[2];
//	char projection[MVO_BUFFER_SIZE];
//	GetHoopsView()->GetCamera(&position,&target,&up,field[0],field[1],projection);

    // convert camera info into starting matrix
    eye[0] = target.x - position.x;
    eye[1] = target.y - position.y;
    eye[2] = target.z - position.z;

    eye_distance = (float)HC_Compute_Vector_Length (eye);
    HC_Compute_Normalized_Vector (eye, eye);
    HC_Compute_Cross_Product (&up.x, eye, horiz);

    pos[0]  = horiz[0];
    pos[1]  = up.x;
    pos[2]  = eye[0];
    pos[3]  = 0.0f;
    pos[4]  = horiz[1];
    pos[5]  = up.y;
    pos[6]  = eye[1];
    pos[7]  = 0.0f;
    pos[8]  = horiz[2];
    pos[9]  = up.z;
    pos[10] = eye[2]; 
    pos[11] = 0.0f; 
    pos[12] = -(float)HC_Compute_Dot_Product (&target.x, horiz);
    pos[13] = -(float)HC_Compute_Dot_Product (&target.x, &up.x);
    pos[14] = -(float)HC_Compute_Dot_Product (&target.x, eye);
    pos[15] = 1.0f;

#if 0
    if (oblique) {
        if (oblique[0] != 0.0f) {
            float       obtan = (float)tan (oblique[0]);

            pos[0]  += pos[2]  * obtan;
            pos[4]  += pos[6]  * obtan;
            pos[8]  += pos[10] * obtan;
            pos[12] += pos[14] * obtan;
        }
        if (oblique[1] != 0.0f) {
            float       obtan = (float)tan (oblique[1]);

            pos[1]  += pos[2]  * obtan;
            pos[5]  += pos[6]  * obtan;
            pos[9]  += pos[10] * obtan;
            pos[13] += pos[14] * obtan;
        }
    }
#endif

    proj[0]  = 2.0f / field[0];
    proj[1]  = 0.0f;
    proj[2]  = 0.0f;
    proj[3]  = 0.0f;
    proj[4]  = 0.0f;
    proj[5]  = 2.0f / field[1];
    proj[6]  = 0.0f;
    proj[7]  = 0.0f;
    proj[8]  = 0.0f;
    proj[9]  = 0.0f;
    proj[10] = 1.0f / eye_distance;
    proj[11] = (projection[0] == 'p') ? proj[10] : 0.0f;
    proj[12] = 0.0f;
    proj[13] = 0.0f;
    proj[14] = 0.0f;
    proj[15] = 1.0f;

    HC_Compute_Matrix_Product (pos, proj, complete);

    HC_Open_Segment_By_Key (start_key);
        status = Walk (tk, complete, Test_Unknown, lod_level);
    HC_Close_Segment ();

    return status;
}

