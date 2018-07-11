// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HNetMessageRegistrar_H
#define _HNetMessageRegistrar_H

#ifndef _HNET_MESSAGE_REGISTRAR
#define _HNET_MESSAGE_REGISTRAR

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HBaseView.h"
#include "HOpObjectAnnotate.h"
#include "HOpCreateNurbs3D.h"
#include "HMarkupManager.h"
#include "vhash.h"
#include <stdio.h>


/*!
	MVOMessageHandler is a subclassed HNetMessageHandler that adds an HBaseView pointer.
*/
class MVOMessageHandler: public HNetMessageHandler {

public:
	/*! This constructor accepts a pointer to an HBaseView object. 
	\param hbaseview A pointer to the HBaseView object associated with this message handler.
	*/
	MVOMessageHandler(HBaseView * hbaseview){
		m_pHBaseView = hbaseview;
	};
	/*! This method passes the message data to the appropriate function in your application. 
		\param message_type  A character pointer denoting the type of message.
		\param data  A character pointer denoting the message data.
		\param data_length  An unsigned int indicating the length of the data in bytes.
	*/
	void ProcessMessage(
		const char * message_type,
		const char * data,
		unsigned int data_length) {;};

protected:
	//! A pointer to an HBaseView object.
	HBaseView * m_pHBaseView;
};

/*!
	HSetCameraMessageHandler is subclassed from MVOMessageHandler.  It handles the message "H_SET_CAMERA". 
*/
class HSetCameraMessageHandler: public MVOMessageHandler {
public:
	/*! Constructs an HSetCameraMessageHandler object.
	\param hbaseview A pointer to the HBaseView object associated with this message handler.
	*/
	HSetCameraMessageHandler(HBaseView * hbaseview):MVOMessageHandler(hbaseview){;};
	/*! This method receives the "H_SET_CAMERA" message and calls HBaseView::SetCameraFromMessage.
		\param message_type  A character pointer denoting the type of message.
		\param data  A character pointer denoting the message data.
		\param data_length  An unsigned int indicating the length of the data in bytes.
	*/
	void ProcessMessage(
		const char * message_type,
		const char * data,
		unsigned int data_length) {
		m_pHBaseView->SetCameraFromMessage(data, data_length);
	};
};

/*!
	HSetRenderModeMessageHandler is subclassed from MVOMessageHandler.  It handles the message "H_SET_RENDERMODE".
*/
class HSetRenderModeMessageHandler: public MVOMessageHandler {
public:
	/*! Constructs an HSetRenderModeMessageHandler object.
	\param hbaseview A pointer to the HBaseView object associated with this message handler.
	*/
	HSetRenderModeMessageHandler(HBaseView * hbaseview):MVOMessageHandler(hbaseview){;};
	/*! This method receives the "H_SET_RENDERMODE" message and calls HBaseView::SetRenderModeFromMessage.
		\param message_type  A character pointer denoting the type of message.
		\param data  A character pointer denoting the message data.
		\param data_length  An unsigned int indicating the length of the data in bytes.
	*/
	void ProcessMessage(
		const char * message_type,
		const char * data,
		unsigned int data_length) {
		m_pHBaseView->SetRenderModeFromMessage(data, data_length);
	};
};


/*!
	HInsertHSFDataMessageHandler is subclassed from MVOMessageHandler. It handles the message "H_INSERT_HSF_DATA". 
*/
class HInsertHSFDataMessageHandler: public MVOMessageHandler {
public:
	/*! Constructs an HInsertHSFDataMessageHandler object.
	\param hbaseview A pointer to the HBaseView object associated with this message handler.
	*/
	HInsertHSFDataMessageHandler(HBaseView * hbaseview):MVOMessageHandler(hbaseview){;};
	/*! This method receives the "H_INSERT_HSF_DATA" message and calls HBaseView::InsertHSFDataFromMessage.
		\param message_type  A character pointer denoting the type of message.
		\param data  A character pointer denoting the message data.
		\param data_length  An unsigned int indicating the length of the data in bytes.
	*/
	void ProcessMessage(
		const char * message_type,
		const char * data,
		unsigned int data_length) {

		m_pHBaseView->InsertHSFDataFromMessage(data, data_length);
	};
};

/*!
	HFlushBaseViewMessageHandler is subclassed from MVOMessageHandler.  It handles the message "H_FLUSH_BASE_VIEW". 
*/
class HFlushBaseViewMessageHandler: public MVOMessageHandler {
public:
	/*! Constructs an HFlushBaseViewMessageHandler object.
	\param hbaseview A pointer to the HBaseView object associated with this message handler.
	*/
	HFlushBaseViewMessageHandler(HBaseView * hbaseview):MVOMessageHandler(hbaseview){;};

	/*! This method receives the "H_FLUSH_BASE_VIEW" and calls HBaseView::FlushFromMessage. 
		\param message_type  A character pointer denoting the type of message.
		\param data  A character pointer denoting the message data.
		\param data_length  An unsigned int indicating the length of the data in bytes.
	*/
	void ProcessMessage(
		const char * message_type,
		const char * data,
		unsigned int data_length) {

		m_pHBaseView->FlushFromMessage(data, data_length);
	};
};


/*!
	HDevNullMessageHandler is a subclassed HNetMessageHandler for message "H_DEV_NULL_DATA" 
*/
class HDevNullMessageHandler: public HNetMessageHandler {
public:
	/*! This method handles the "H_DEV_NULL_DATA" message .
		\param message_type  A character pointer denoting the type of message.
		\param data  A character pointer denoting the message data.
		\param data_length  An unsigned int indicating the length of the data in bytes.
	*/
	void ProcessMessage(
		const char * message_type,
		const char * data,
		unsigned int data_length) {

	};
};

/*!
	HSelectMessageHandler is subclassed from MVOMessageHandler.  It handles the messages 
	"H_SELECT_ENTITY", "H_DESELECT_ALL" and "H_DELETE_SELECTION_LIST".
*/
class HSelectMessageHandler: public MVOMessageHandler {
public:
	/*! Constructs an HSelectMessageHandler object.
	\param hbaseview A pointer to the HBaseView object associated with this message handler.
	*/
	HSelectMessageHandler(HBaseView * hbaseview):MVOMessageHandler(hbaseview){;};
	/*! This method handles the "H_SELECT_ENTITY", "H_DESELECT_ALL" and "H_DELETE_SELECTION_LIST"
	messages and calls HBaseView::DeSelectAllFromMessage, HBaseView::DeleteSelectionList and
	HBaseView::SelectFromMessage respectively.

		\param message_type  A character pointer denoting the type of message.
		\param data  A character pointer denoting the message data.
		\param data_length  An unsigned int indicating the length of the data in bytes.
	*/
	void ProcessMessage(
		const char * message_type,
		const char * data,
		unsigned int data_length) {

		if (strcmp(message_type,"H_DESELECT_ALL") == 0)
			m_pHBaseView->DeSelectAllFromMessage();
		else if (strcmp(message_type,"H_DELETE_SELECTION_LIST") == 0)
			m_pHBaseView->DeleteSelectionList();
		else
			m_pHBaseView->SelectFromMessage(data, data_length);
	};
};


/*!
	HDeleteByKeyMessageHandler is subclassed from MVOMessageHandler.  It handles the message "H_DELETE_BY_KEY". 
*/
class HDeleteByKeyMessageHandler: public MVOMessageHandler {
public:
	/*! Constructs an HDeleteByKeyMessageHandler object. 
	\param hbaseview A pointer to the HBaseView object associated with this message handler.
	*/
	HDeleteByKeyMessageHandler(HBaseView * hbaseview):MVOMessageHandler(hbaseview){;};

	/*! This method receives the message H_DELETE_BY_KEY and calls #HBaseView::DeleteByKeyFromMessage.
		\param message_type  A character pointer denoting the type of message.
		\param data  A character pointer denoting the message data.
		\param data_length  An unsigned int indicating the length of the data in bytes.
	*/
	void ProcessMessage(
		const char * message_type,
		const char * data,
		unsigned int data_length) {

 		m_pHBaseView->DeleteByKeyFromMessage(data, data_length);
	};
};


/*!
	HSetWindowColorMessageHandler is subclassed from MVOMessageHandler.  It handles the message "H_SET_WINDOW_COLOR" 
*/
class HSetWindowColorMessageHandler: public MVOMessageHandler {
public:
	/*! Constructs an HSetWindowColorMessageHandler object.
	\param hbaseview A pointer to the HBaseView object associated with this message handler.
	*/
	HSetWindowColorMessageHandler(HBaseView * hbaseview):MVOMessageHandler(hbaseview){;};
	/*! This method receives the message H_SET_WINDOW_COLOR and calls #HBaseView::SetWindowColorFromMessage.
		\param message_type  A character pointer denoting the type of message.
		\param data  A character pointer denoting the message data.
		\param data_length  An unsigned int indicating the length of the data in bytes.
	*/
	void ProcessMessage(
		const char * message_type,
		const char * data,
		unsigned int data_length) {

 		m_pHBaseView->SetWindowColorFromMessage(data, data_length);
	};
};

/*!
	HSetModellingMatrixMessageHandler is subclassed from MVOMessageHandler.  It handles the message "H_SET_MODELLING_MATRIX". 
*/
class HSetModellingMatrixMessageHandler: public MVOMessageHandler {
public:
	/*! Constructs an HSetModellingMatrixMessageHandler object.
	\param hbaseview A pointer to the HBaseView object associated with this message handler.
	*/
	HSetModellingMatrixMessageHandler(HBaseView * hbaseview):MVOMessageHandler(hbaseview){;};
	/*! This method receives the message "H_SET_MODELLING_MATRIX" and calls #HBaseView::SetModellingMatrixFromMessage.
		\param message_type  A character pointer denoting the type of message.
		\param data  A character pointer denoting the message data.
		\param data_length  An unsigned int indicating the length of the data in bytes.
	*/
	void ProcessMessage(
		const char * message_type,
		const char * data,
		unsigned int data_length) {

 		m_pHBaseView->SetModellingMatrixFromMessage(data, data_length);
	};
};

/*!
	HMarkupMessageHandler is subclassed from MVOMessageHandler.  It handles the messages 
		"H_INSERT_MARKUP_LAYER_HSF_DATA", "H_SET_MARKUP_LAYER" and "H_DISABLE_CURRENT_MARKUP_LAYER"
*/
class HMarkupMessageHandler: public MVOMessageHandler {
public:
	/*! Constructs an HMarkupMessageHandler object.
	\param hbaseview A pointer to the HBaseView object associated with this message handler.
	*/
	HMarkupMessageHandler(HBaseView * hbaseview):MVOMessageHandler(hbaseview){;};
	/*! This method receives the messages "H_INSERT_MARKUP_LAYER_HSF_DATA", "H_SET_MARKUP_LAYER" 
	and "H_DISABLE_CURRENT_MARKUP_LAYER" and calls #HMarkupManager::InsertMarkupSegmentFromMessage, 
	#HMarkupManager::SetMarkupLayerFromMessage and #HMarkupManager::DisableCurrentMarkupLayerFromMessage 
	respectively.
		\param message_type  A character pointer denoting the type of message.
		\param data  A character pointer denoting the message data.
		\param data_length  An unsigned int indicating the length of the data in bytes.
	*/
	void ProcessMessage(
		const char * message_type,
		const char * data,
		unsigned int data_length) {

		if (strcmp(message_type,"H_INSERT_MARKUP_LAYER_HSF_DATA") == 0)
			m_pHBaseView->GetMarkupManager()->InsertMarkupSegmentFromMessage(data, data_length);
		else if (strcmp(message_type,"H_SET_MARKUP_LAYER") == 0)
			m_pHBaseView->GetMarkupManager()->SetMarkupLayerFromMessage(data, data_length);
		else if (strcmp(message_type,"H_DISABLE_CURRENT_MARKUP_LAYER") == 0)
			m_pHBaseView->GetMarkupManager()->DisableCurrentMarkupLayerFromMessage(data, data_length);
	};
};

/*!
	HAnnotateObjectMessageHandler is subclassed from MVOMessageHandler.  It handles the message "H_ANNOTATE_OBJECT".
*/
class HAnnotateObjectMessageHandler: public MVOMessageHandler {
public:
	/*! Constructs an HAnnotateObjectMessageHandler object.  
	\param hbaseview A pointer to the HBaseView object that is associated with this object.
	*/
	HAnnotateObjectMessageHandler(HBaseView * hbaseview):MVOMessageHandler(hbaseview){;};
	
	/*! This method receives the "H_ANNOTATE_OBJECT" message and calls #HOpObjectAnnotate::AnnotateObjectFromMessage.
		\param message_type  A character pointer denoting the type of message.
		\param data  A character pointer denoting the message data.
		\param data_length  An unsigned int indicating the length of the data in bytes.
	*/
	void ProcessMessage(
		const char * message_type,
		const char * data,
		unsigned int data_length) {

		HOpObjectAnnotate::AnnotateObjectFromMessage(m_pHBaseView, data, data_length);
	};
};

/*!
	HNurbsMessageHandler is subclassed from MVOMessageHandler.  It handles messages from 
	"H_INSERT_NURBS", "H_FINISH_NURBS_EDITING" and "H_MODIFY_CONTROL_POINT".
*/
class HNurbsMessageHandler: public MVOMessageHandler {
public:
	/*! Constructs an HNurbsMessageHandler object.
	\param hbaseview A pointer to the HBaseView object associated with this message handler.
	*/
	HNurbsMessageHandler(HBaseView * hbaseview):MVOMessageHandler(hbaseview){;};
	/*! This method receives the messages "H_INSERT_NURBS", "H_FINISH_NURBS_EDITING" 
	and "H_MODIFY_CONTROL_POINT" and calls #HOpCreateNurbs3D::InsertNurbsFromMessage, 
	#HOpCreateNurbs3D::FinishNurbsEditingFromMessage and #HOpCreateNurbs3D::ModifyControlPointFromMessage 
	respectively.
		\param message_type  A character pointer denoting the type of message.
		\param data  A character pointer denoting the message data.
		\param data_length  An unsigned int indicating the length of the data in bytes.
	*/
	void ProcessMessage(
		const char * message_type,
		const char * data,
		unsigned int data_length) {

		if (strcmp(message_type,"H_INSERT_NURBS") == 0)
			HOpCreateNurbs3D::InsertNurbsFromMessage(data, data_length, m_pHBaseView);
		else if(strcmp(message_type,"H_FINISH_NURBS_EDITING") == 0)
			HOpCreateNurbs3D::FinishNurbsEditingFromMessage(data, data_length,m_pHBaseView);
		else if(strcmp(message_type,"H_MODIFY_CONTROL_POINT") == 0)
			HOpCreateNurbs3D::ModifyControlPointFromMessage(data, data_length, m_pHBaseView);
 	};
};

/*!
	HSetHandednessMessageHandler is subclassed from MVOMessageHandler.  It handles the message "H_SET_HANDEDNESS".
*/
class HSetHandednessMessageHandler: public MVOMessageHandler {
public:
	/*! Constructs an HSetHandednessMessageHandler object.
	\param hbaseview A pointer to the HBaseView object associated with this message handler.
	*/
	HSetHandednessMessageHandler(HBaseView * hbaseview):MVOMessageHandler(hbaseview){;};
	/*! This method receives the "H_SET_HANDEDNESS" message and calls #HBaseView::SetHandednessFromMessage.
		\param message_type  A character pointer denoting the type of message.
		\param data  A character pointer denoting the message data.
		\param data_length  An unsigned int indicating the length of the data in bytes.
	*/
	void ProcessMessage(
		const char * message_type,
		const char * data,
		unsigned int data_length) {
		m_pHBaseView->SetHandednessFromMessage(data, data_length);
	};
};

/*!
	HSetFontSizeMessageHandler is subclassed from MVOMessageHandler. It handles the message "H_SET_FONT_SIZE".
*/
class HSetFontSizeMessageHandler: public MVOMessageHandler {
public:
	/*! Constructs an HSetFontSizeMessageHandler object.
	\param hbaseview A pointer to the HBaseView object associated with this message handler.
	*/
	HSetFontSizeMessageHandler(HBaseView * hbaseview):MVOMessageHandler(hbaseview){;};
	/*! This method receives the "H_SET_FONT_SIZE" message and calls #HBaseView::SetFontSizeFromMessage.
		\param message_type  A character pointer denoting the type of message.
		\param data  A character pointer denoting the message data.
		\param data_length  An unsigned int indicating the length of the data in bytes.
	*/
	void ProcessMessage(
		const char * message_type,
		const char * data,
		unsigned int data_length) {
		m_pHBaseView->SetFontSizeFromMessage(data, data_length);
	};
};


//! The HNetMessageRegistrar class maps messages from HNetClients or .snap files to MVO calls
/*!  
	Subclassed HNetMessageHandler objects are registered for specific messages.  
*/

class HNetMessageRegistrar {

public:

	/*! 
	  Constructs an HNetMessageRegistrar object.	  
	  \param new_hbaseview This is a pointer to a HBaseView object this registrar instance will deliever messages to.
	  \param new_hnetclient Pass a pointer to an HNetClient will you want messages to come from that object or pass null for
	  messages to be generated from .snap files.
	*/
	HNetMessageRegistrar(
		HBaseView * new_hbaseview, 
		HNetClient * new_hnetclient=0) {
		
		hbaseview=new_hbaseview; 
		hnetclient=new_hnetclient;

		if(!hnetclient)
			message_handler_hash = new_vhash(1, malloc, free);
		else
			message_handler_hash = 0;

		default_message_handler=0;

	};

	/*! 
		The HNetMessageRegistrar destuctor cleans up all HMessageHandler class instance that
		have been added to it. 
	*/
	virtual ~HNetMessageRegistrar() {

		UnSetHandlers();

		if(message_handler_hash) {
			delete_vhash(message_handler_hash);
			message_handler_hash=0;
		}
	}

	/*!
		This method calls SetMessageHandler() for all default MVO messages.
		HNetMessageRegistrar may be subclassed to set custom message handlers.
		Custom handlers may also be set by calling SetMessageHandler() for the
		custom message handlers directly.

		This function must be called by the user to set the default handlers.
		If it is not called, no default handlers will be set.
	*/
	virtual void SetHandlers(){

		SetMessageHandler( new HSetCameraMessageHandler(hbaseview), "H_SET_CAMERA");
		SetMessageHandler( new HSetRenderModeMessageHandler(hbaseview), "H_SET_RENDERMODE");
		SetMessageHandler( new HInsertHSFDataMessageHandler(hbaseview), "H_INSERT_HSF_DATA");
		SetMessageHandler( new HFlushBaseViewMessageHandler(hbaseview), "H_FLUSH_BASE_VIEW");
		SetMessageHandler( new HDevNullMessageHandler(), "H_DEV_NULL_DATA");
		SetMessageHandler( new HSelectMessageHandler(hbaseview), "H_SELECT_ENTITY");
		SetMessageHandler( new HSelectMessageHandler(hbaseview), "H_DESELECT_ALL");
		SetMessageHandler( new HDeleteByKeyMessageHandler(hbaseview), "H_DELETE_BY_KEY");
		SetMessageHandler( new HSelectMessageHandler(hbaseview), "H_DELETE_SELECTION_LIST");
		SetMessageHandler( new HSetModellingMatrixMessageHandler(hbaseview), "H_SET_MODELLING_MATRIX");
		SetMessageHandler( new HAnnotateObjectMessageHandler(hbaseview), "H_ANNOTATE_OBJECT");
		SetMessageHandler( new HNurbsMessageHandler(hbaseview), "H_INSERT_NURBS");
		SetMessageHandler( new HNurbsMessageHandler(hbaseview), "H_MODIFY_CONTROL_POINT");
		SetMessageHandler( new HNurbsMessageHandler(hbaseview), "H_FINISH_NURBS_EDITING");
		SetMessageHandler( new HSetWindowColorMessageHandler(hbaseview), "H_SET_WINDOW_COLOR");
		SetMessageHandler( new HMarkupMessageHandler(hbaseview), "H_DISABLE_MARKUP_LAYER");
		SetMessageHandler( new HMarkupMessageHandler(hbaseview), "H_SET_MARKUP_LAYER");
		SetMessageHandler( new HMarkupMessageHandler(hbaseview), "H_INSERT_MARKUP_LAYER_HSF_DATA");
		SetMessageHandler( new HSetHandednessMessageHandler(hbaseview), "H_SET_HANDEDNESS");
		SetMessageHandler( new HSetFontSizeMessageHandler(hbaseview), "H_SET_FONT_SIZE");

	};

	/*!
	  This method clears all message handlers that have been set.
	  It is called when the registrar is deleted but may be called
	  at any time to remove handlers.
	*/	  
	virtual void UnSetHandlers(){

		if(hnetclient){	

			hnetclient->EnumerateMessageHandlers(
				net_unset_message_handler_helper, hnetclient);

		}else
		if(message_handler_hash){
			vhash_string_key_map_function(
					message_handler_hash,
					non_net_unset_message_handler_helper,
					(void*)this);
		}

	};

	/*!
	  This method sets a handler for a specific message.
	  The old handler if one existed is returned to caller.

	  \param handler A pointer to a subclassed HNetMessageHandler for the specifed message.  A value of 0 will unset the handler.
	  \param message_type A character pointer denoting the type of message to be handled.
		If message_type is 0, then the object is set as the default message handler and any message 
		which doesn't have a specific handler is dispatched to the default handler.
	  \return A pointer to the handler that was previously registered to handle this message if one 
	        existed. If not, then the return value is 0.


		For example:

		<pre>\code SetMessageHandler(0,"H_INSERT_NURBS"); \endcode</pre>		
		This example removes the handler for message H_INSERT_NURBS.

		<pre>\code SetMessageHandler( new HMarkupMessageHandler(hbaseview), "H_INSERT_MARKUP_LAYER_HSF_DATA");\endcode</pre>
		This example sets HMarkupMessageHandler for message H_INSERT_MARKUP_LAYER_HSF_DATA.

		<pre>\code SetMessageHandler( new MyDefaultHandler(hbaseview), 0);\endcode</pre>
		This example sets MyDefaultHandler as the handler for all messages without other handlers. 

	*/	  
	virtual HNetMessageHandler * SetMessageHandler(
			HNetMessageHandler * handler,
			const char * message_type)
	{
		HNetMessageHandler * old_handler=0;
		if(hnetclient)
			old_handler = hnetclient->SetMessageHandler(handler,message_type);
		else
		if(message_type){
			old_handler = (HNetMessageHandler *)vhash_remove_string_key(message_handler_hash, message_type);
			if(handler)
				vhash_insert_string_key(message_handler_hash, message_type, (void*) handler);
		}else{
			if(default_message_handler) old_handler = default_message_handler;
			default_message_handler = handler;
		}
		return old_handler;
	}

	/*!
	  A snap file created by a hnet_server may be loaded and "played back" using this method.
	  It can only be used if the HMessageRegistrar was created without a HNetClient.

	  \param file_name The name of the snap file to load.
	  \return This returns true if the file opened and was processes and false if loading 
		failed because the file counln't be read or the registrar had a HNetClient.
	*/

	bool ReadSnapFile(const char * file_name) {

		FILE * file = fopen(file_name,"rb");

		if(file==NULL) return false;
		if(hnetclient) return false;

		unsigned long line_number = 0;
		unsigned long line_buffer_size = 1024;
		char * line_buffer;
		unsigned long n;
		unsigned long expected_n;
		int a_char;
		int read_again = 1;

		line_buffer = (char*) malloc(line_buffer_size);
		fseek(file,0,SEEK_SET);

READ_A_LINE:
		n=0;
		expected_n=0;
		line_number++;

		while(1) {
			/*resize buffer as needed*/
			if(n == line_buffer_size) {
				char * new_line_buffer;
				new_line_buffer = (char*) malloc(line_buffer_size*2);
				memcpy(new_line_buffer, line_buffer, line_buffer_size);
				free(line_buffer);
				line_buffer = new_line_buffer;
				line_buffer_size = line_buffer_size*2;
			}
			a_char = fgetc(file);
			if(a_char == EOF) {
				read_again=0;
				break;
			}
			if(expected_n) {
				line_buffer[n] = (char) a_char;
				n++;
				if(n==expected_n) {
					/*have a whole line*/
					ProcessMessage(line_buffer, n);
					break;
				}
			}else{
				if(a_char == 0x0D || a_char == 0x0A) {
					if(n == 0) continue;
					/*this is a error in the file*/
					read_again=0;
					break;
				}
				if (a_char == ' ') {
					line_buffer[n] = '\0';
					sscanf(line_buffer, "%ld", &expected_n);
					n=0;
				}else{
					line_buffer[n] = (char) a_char;
					n++;
				}
			}
		}
		if(read_again)
			goto READ_A_LINE;
		free(line_buffer);
		return true;
	};


protected:

	/*!
	  This is used by the snap file loader to send a single message.

	  \param message A pointer to the message data.
	  \param message_length The lenght of the message data.
	*/

	virtual void ProcessMessage(
			const char * message,
			unsigned long message_length) {

			if(hnetclient) return;

			char * message_type = (char*) malloc(message_length);
			unsigned long n = 0;

			while(n < message_length && message[n] != ' ') {
				message_type[n] = message[n];
				n++;
			}
			message_type[n] = '\0';
			n++;
			HNetMessageHandler * message_handler =
				(HNetMessageHandler *) vhash_lookup_string_key(message_handler_hash, message_type);

			if(!message_handler)
				message_handler = default_message_handler;
			if(message_handler) 
				message_handler->ProcessMessage(message_type, &message[n], message_length-n);
			free(message_type);
	}

	//! This is the HBaseView for this registrar.
	HBaseView * hbaseview; 
	//! This is the HNetClient for this registrar.
	HNetClient * hnetclient;

private:
	struct vhash_s * message_handler_hash;
	HNetMessageHandler * default_message_handler;

	static void net_unset_message_handler_helper(
		void * handler,
		const char * message,
		void * user_data){

		((HNetClient*)user_data)->SetMessageHandler(0,message);
		delete (HNetMessageHandler*)handler;
	};

	static void non_net_unset_message_handler_helper(
			void * handler,
			const char * message,
			void * user_data){

		HNetMessageRegistrar * self = (HNetMessageRegistrar *) user_data;
		self->SetMessageHandler(0,message);
		delete (HNetMessageHandler*)handler;
	};

};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif
#endif



