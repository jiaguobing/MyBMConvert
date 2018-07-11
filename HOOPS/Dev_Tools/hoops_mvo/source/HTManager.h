// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

/*! \file HTManager.h
	Defines datatypes for the HTManager class
*/

#ifndef _H_HT_MANAGER_H
#define _H_HT_MANAGER_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif


#include "HTools.h"

class HTClient;
class HTDriver;

/*! 
    HTManager provides a centralized timing service for MVO.  It is intended to allow animation 
    or other ongoing actions to proceed without completely usurping control of the application.

    The timer service is completely platform and GUI independent and therefore requires timer ticks 
	to be delivered from whatever GUI or system calls are appropriate from the environment.  It is recommended 
    (though not required) that such ticks be delivered at the highest available frequency from the 
    system.  See CTDriver.cpp in hoops_mfc for an example.

    Once such ticks are set up, clients can register themselves to request notification at any 
    frequency.  Such clients must derive from HTClient.  A reference implementation is provided 
    by HTCObjectRotate.[cpp,h]

    The HTManager's sense of time is achieved by repeated calls to HOOPS/3dGS' HC_Show_Time.  
    Refer to the 3dGS reference manual.

    This platform-independent class keeps an array of linked lists to store all of the events 
    for one second, then has a spillover list to handle anything that needs to be scheduled further 
    out.  Its main job is to dispatch timer messages from the OS to whatever clients would like 
    to request them.  With this service in place, clients can safely release control during an 
    animation, knowing that control will be returned when it is needed.
*/
class MVO_API HTManager
{
public:
	/*! Constructs an HTManager object. 
	\param output_hz The number of buckets into which the timer manager splits a single second. 
	*/
    HTManager( int output_hz = 100 );
    virtual ~HTManager();

    /*! The mechanism for receiving timer events from the OS is platform dependent, so here we only provide 
		a function call for the external "timer driver" to deliver timer events.  It is recommended that 
		such events be delivered at the highest available frequency. */
    virtual void Tick( float time );
    /*! This method informs the timer manager of a client that would like to start receiving timer events.
	\param c A pointer to the HTClient that would like to receiver timer events.
	*/
    void RegisterClient( HTClient *c );
    /*!This method stops the timer service for the given client.
	\param c A pointer the HTClient that no longer wants to receive timer events.
	*/
    void UnRegisterClient( HTClient *c );
    /*! \return A pointer to the timer manager currently associated with hoops_mvo's central HDB object. */
    static HTManager *GetCurrentHTManager();

protected:
    double m_interval;               //!< 1.0 / m_output_hz
    double m_request_time;           //!< The time that the current bucket's events had requested.
    double m_actual_time;            //!< The actual time which may be different from m_request_time if control is lost for a while.
    int m_output_hz;                //!< The granularity of the timer, i.e. the number of buckets into which the timer manager splits a single second.  Timer events scheduled in the same bucket may be delivered sorted only by priority.
    struct vlist_s **m_buckets;     //!< The array of buckets of size m_output_hz. This array holds precisely 1 sec of events.
    int m_current_bucket;           //!< The bucket of events currently being dispatched.
    struct vlist_s *m_spillover;    //!< An array of things that need to be scheduled for more than 1s away.
    struct vhash_s *m_active_clients; //!< A hash table of all the currently registered clients.  Only currently registered clients can receive ticks, or even accessed in any way to make sure we don't access any memory after free. */
    struct vlist_s *m_recently_deleted_expirations; //!< A vlist parallel to m_recently_deleted_clients to indicate how long things need to stay on the list.
    struct vlist_s *m_recently_deleted_clients; //!< A list of recently deleted clients.  Only if an item is unregistered and then re-registered very quickly. 

    /*! This method must be called before any dispatching can be done.  Note it is automatically performed on the first call to 
		RegisterClient.  It allocates all required memory for member data. 
		\param start_time The time, as reported by #HC_Show_Time, at which this class was initialized. 
		*/
    void Init( float start_time );
    /*! This method finds the appropriate list to which the given client should be added so that an event is scheduled at 
	the right time. 
	\param c The client that is requesting a timer event.  
	\param time Pass the ideal time at which this timer event should be delivered.
	*/
    void ScheduleNextTick( HTClient *c, float time );
};

/*! \enum HTCStyle
	Indicates how timer events will be delivered
*/
enum HTCStyle {
    HTCS_Invalid,       //!< An invalid default state for the client style.  The client style must be set before anything useful will happen.
    HTCS_Once,          //!< A single event will be delivered at the designated time, after which the timer client will automatically be unregistered.
    HTCS_Periodic,      //!< Events will be delivered at the interval specified with the timer client's "mt_interval" member variable.  If for any reason the timer manager falls behind, a series of "make-up" events will be delivered, with identical values for "actual_time", but different "request_time"'s
    HTCS_PeriodicSkip   //!< Similar to HTCS_Periodic but this option has no make-up events.
};

/*! var typedef bool(*HTClientTickFunction)(float request_time, float actual_time, void * user_data) */
typedef bool(*HTClientTickFunction)(float request_time, float actual_time, void * user_data);

/*!
    HTClient is a class from which any object that wishes to begin receiving timer events should derive.  
    It can work in one of two ways.  Clients can either register a C-style callback with the "tick_function" 
    argument of the constructor, or they can override the Tick() function to receive the calls directly.

    Most of the configuration work for an HTClient is handled by manipulating its member variables.  Some of
    these variables are initialized to usable values, whereas others are not.  For an HTClient derivative
    to do anything useful, three operations are required.  First, mt_style must be set to an #HTCStyle other than the
    default value of HTCS_Invalid.  Second, there must be some interface provided to receive timer events, either
    a callback or a function override.  Finally, a pointer to the client must be passed to HTManager::RegisterClient()

    If the callback mechanism is used to deliver timer events, there is a void pointer attached to the events
    that come back to the caller.  This interface should be used for situations where multiple timers are to be 
    associated with a single object.  A good example of such usage is in hoops_mvo's HOpCameraWalk operator, where
    an HTClient is instantiated directly and assigned to a member variable:
	<pre><code> m_pWalkTimer = new HTClient( 0.01f, HTCS_PeriodicSkip, WalkTimerClbk, this );</code></pre>
    WalkTimerClbk is a static function that simply uses the user_data pointer at the end to dispatch to one
    of the member functions of the HOpCameraWalk.

    Priority is used for sorting the list of timer events that happen within the span of one of the HTManager's
    bucket.  One example of where this might be appropriate is a timer client that polls a flag to call 
    #HC_Update_Display at the appropriate times.  Such a client should call SetPriorityLowest() on itself to 
    request that it be processed last.

    Troubleshooting:<br> 
    If no timer events are received, check the following:<br>
	<ol>
		<li type=A>
			There is a timer driver passing OS timer events into the timer manager correctly.
 		</li>
		<li type=A>
			Any overrides of Tick() exactly match this base class.  Double check capitalization and the argument list.
   		</li>
		<li type=A>
			The memeber #mt_style is set to something other than the default. 
  		</li>
		<li type=A>
			The client was registered via HTManager::RegisterClient().
  		</li>
		<li type=A>
		 The timer events can be delivered to a callback or a Tick() override, but not both.  If there is an attempt to 
            use both, the override wins.
		</li>
	</ol>
*/
class MVO_API HTClient
{
public:
	/*! Constructs an HTClient object.
		\param interval Pass the requested time between timer events.
		\param style This parameter defaults to HTCS_Invalid but before the client can be registered, it must be set
		to one of the #HTCStyle enumerations.  
		\param tick_function The callback function pointer which is generally a static function associated
				with the class.
		\param user_data A pointer passed back to the tick_function callback.  Generally set to "this"
				so that the tick callback can find its way to the class. */
    HTClient(   float interval = 0.1f, 
				HTCStyle style = HTCS_Invalid, 
				HTClientTickFunction tick_function = 0, 
				void* user_data = 0 ) {
        mt_interval = interval;
        mt_style = style;
        mt_next_request = 0;
        mt_priority = 0;
        mt_tick_function = tick_function;
        mt_user_data = user_data;
    }
    virtual ~HTClient();

    /*! Timer events are delivered here first.  If this function is not overridden, the events will be delivered
		to whatever was set with the #mt_tick_function argument of the constructor or later on with SetTickFunction().
		\param request_time The time for which the timer event was originally scheduled.
		\param actual_time The time at which the tick was actually delivered which is greater than or equal to <i>request_time</i>.
		\return True if no errors were encountered. */
	virtual bool Tick( float request_time, float actual_time ){ 
        if( mt_tick_function ) 
            return mt_tick_function(request_time, actual_time, mt_user_data);
        else
            return true;
    };

    /*! This method sets the interval in which the HTClient would like to receive timer events.
	\param interval Pass a float that defines that period of time in between receiving timer events.*/
    void SetInterval( float interval ) { mt_interval = interval; };
    
	/*! \return The timer interval in which the HTClient would like to receive timer events. */
    float GetInterval() const { return mt_interval; };
    
	/*!This method sets how the client would like to receive tick calls.
	\param style Pass one the defined styles: #HTCS_Invalid, #HTCS_Once, #HTCS_Periodic or #HTCS_PeriodicSkip. Note 
	that you must select a style other than #HTCS_Invalid to receive timer events.
	*/
    void SetStyle( HTCStyle style ) { mt_style = style; };
    
	/*! \return The timer style which describes how the client would like to receive timer events.*/
    HTCStyle GetStyle() const { return mt_style; };
    
	/*!  This method sets the next timer request.  Generally this is only called from the timer manager after a timer 
	event has been delivered to a Periodic-style client.
	\param nr The time at which we would like to receive the next timer event.  */
    void SetNextRequest( float nr ) { mt_next_request = nr; };
    
	/*! \return The time of the next scheduled event. */
    float GetNextRequest() const { return mt_next_request; };
    
	/*! \return The priority level for this client to receive timer event. Within a single time slice, the HTManager 
	delivers timer events according to priority values of the given HTClients.  Note that clients with higher priorities 
	will receive timer events before those with lower priority levels.*/
    int  GetPriority() const { return mt_priority; };
    
	/*! This method sets the priority to the lowest supported value.  This client will subsequently be placed at the list 
		tail of any bucket it is scheduled into, as opposed to the head, as usual.  It has the effect of postponing 
		timer events very slightly. */
    void SetPriorityLowest() { mt_priority = -1; };
    
	/*! Use this method to pass data via the timer callback.  When the timer manager calls the #Tick() function, 
	it will pass the user data.
	\param user_data User defined data that may be specifically used for this client's Tick function. */
    void SetUserData( void* user_data ) { mt_user_data = user_data; };
    
	/*! \return The user data that is specific to this client. */
    void* GetUserData() const { return mt_user_data; };
    
	/*! Use this method to set the callback function that will handle the timer events.
	\param tick_function A pointer to the function that will handle this client's timer events. 
	*/
    void SetTickFunction( HTClientTickFunction tick_function) { mt_tick_function = tick_function; };
    
	/*! \return A pointer to the Tick function callback. */
    HTClientTickFunction GetTickFunction() const { return mt_tick_function; };

protected:
	/*! The client style which describes how this object would like to receive timer events.*/
    HTCStyle mt_style;                      

	/*! The interval, in seconds, at which tick events occur.  The first event will occur mt_interval 
	seconds after the call to HTManager::RegisterClient().  For #HTCS_Once clients, mt_interval specifies 
	the delay until the one and only timer event is received.*/
    float mt_interval;        

	/*! The time at which the next event is scheduled.*/
    float mt_next_request;                  

	/*! The priority determines the order in which to sort timer events within a single HTManager bucket.  
	In other words, it is a tie breaker for two timer events that are supposed to occur at the same time.
	*/
    int mt_priority;                        

	/*! A pointer passed back to the Tick function callback, if one is set.*/
    void* mt_user_data;                     
    
	/*! A function to call whenever timer events are received.  
	This is called automatically if and only if Tick() is not overridden.*/
	HTClientTickFunction mt_tick_function;  
};



#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




