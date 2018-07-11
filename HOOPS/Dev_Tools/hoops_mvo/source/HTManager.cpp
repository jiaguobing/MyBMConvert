// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <stdio.h>
#include "HTManager.h"
#include "HDB.h"
#include "vlist.h"
#include "vhash.h"

 
HTManager::HTManager ( int output_hz )
{
    m_output_hz = output_hz;
    m_interval = 1.0f / output_hz;
    m_buckets = 0;
    m_spillover = 0;
    m_active_clients = 0;
    m_current_bucket = 0;
    m_request_time = m_actual_time = 0;
    m_recently_deleted_clients = 0;
    m_recently_deleted_expirations = 0;
}


HTManager::~HTManager ()
{
    int i;

    if( m_active_clients != 0 ) {
        delete_vhash( m_active_clients );
    }
    if( m_recently_deleted_clients != 0 ) {
        delete_vlist( m_recently_deleted_clients );
    }
    if( m_recently_deleted_expirations != 0 ) {
        delete_vlist( m_recently_deleted_expirations );
    }
    if( m_buckets != 0 ) {
        for( i = 0 ; i < m_output_hz ; i++ ) {
            delete_vlist( m_buckets[i] );
        }
        delete [] m_buckets;
        m_buckets = 0;
    }
    if( m_spillover != 0 )
        delete_vlist( m_spillover );
}


void HTManager::Init( float start_time )
{
    int i;

    if( m_buckets == 0 ) {
        m_buckets = new struct vlist_s *[m_output_hz];
        for( i = 0 ; i < m_output_hz ; i++ ) {
            m_buckets[i] = new_vlist( malloc, free );
        }
    }
    if( m_spillover == 0 )
        m_spillover = new_vlist( malloc, free );
	if( m_active_clients == 0 )
        m_active_clients = new_vhash( 100, malloc, free );
	if( m_active_clients == 0 )
        m_active_clients = new_vhash( 100, malloc, free );
    if( m_recently_deleted_clients == 0 )
        m_recently_deleted_clients = new_vlist( malloc, free );
    if( m_recently_deleted_expirations == 0 )
        m_recently_deleted_expirations = new_vlist( malloc, free );

    m_request_time = m_actual_time = start_time;
}


void HTManager::Tick( float time )
{
    HTClient *client;
    float t, dt;
    bool keep_going = true;
	int expiration;

    if( m_buckets == 0 )
        Init( time );
    m_actual_time = time;
    while( m_request_time + m_interval < m_actual_time ) {
        while( (client = (HTClient *)vlist_remove_first( m_buckets[m_current_bucket] )) != 0 ) {
			if( vhash_lookup_item( m_active_clients, client, 0 ) ) {
				dt = client->GetInterval();
				switch( client->GetStyle() ) {
					case HTCS_Periodic: {
						//synthesize ticks to make up for the ones we missed
						for( t = client->GetNextRequest(); t < m_actual_time && keep_going ; t += dt ) {
							if( !vhash_lookup_item( m_active_clients, client, 0 ) )
								break;
							keep_going = client->Tick( t, (float)m_actual_time );
						}
						if( keep_going )
							ScheduleNextTick( client, t );
                        
					} break;
					case HTCS_PeriodicSkip: {
						//schedule next event, but no make up calls
						keep_going = client->Tick( (float)m_request_time, (float)m_actual_time );
						if( keep_going )
							ScheduleNextTick( client, (float)m_actual_time + dt );
					} break;
					case HTCS_Once: {
						//do nothing except call the one tick.
						client->Tick( (float)m_request_time, (float)m_actual_time );
					} break;
					default:
						//if you get here, the most likely causes are either that m_style was never set
						//(defaults to HTCS_Invalid at construction), or the client was deleted without
						//first unregistering itself.
						assert( 0 );
				}
			}
        }
        m_current_bucket++;
        m_request_time += m_interval;
        if( m_current_bucket == m_output_hz ) {
            //process spillover
            while((client = (HTClient *)vlist_peek_first( m_spillover )) != 0 ) {
                t = client->GetNextRequest();
                if( t - m_request_time >= 1 )
                    break;
                vlist_remove_first( m_spillover );
                ScheduleNextTick( client, t );
            } 
            m_current_bucket = 0;
            //take stuff off the front of the recent deletions list
            for(;;) {
				expiration = static_cast<int>(MVO_V2I(vlist_peek_first( m_recently_deleted_expirations )));
				if( expiration != 0 &&
					expiration < (int)m_actual_time ) {
					vlist_remove_first( m_recently_deleted_clients );
					vlist_remove_first( m_recently_deleted_expirations );
				}
				else
					break;
			}
        }
    }
} // end function HTManager::Tick


void HTManager::ScheduleNextTick( HTClient *c, float time )
{
	HDB::Lock();
	HTClient *temp;
    int i, offset;

    if( time < m_request_time ) {
        time = (float)(m_request_time + m_interval);
    }
    c->SetNextRequest( time );
    if( time - m_request_time >= 1 ) {
        temp = (HTClient *) vlist_peek_first( m_spillover );
        if( ( temp == 0 ) || 
            ( time < temp->GetNextRequest())) {
            vlist_add_first( m_spillover, c );
        }
        else {
            /* insert into m_spillover, but maintain m_spillover's sorting (effectively
             * a bubble sort, but O(N^2) should still be relatively small in this case)*/
            vlist_reset_cursor( m_spillover );
            for(;;) {
                temp = (HTClient *) vlist_peek_cursor_next( m_spillover );
                if( ( temp == 0 ) || 
                    ( time < temp->GetNextRequest())) {
                    vlist_add_after_cursor( m_spillover, c );
                    break;
                }
                vlist_advance_cursor( m_spillover );
            }
        }
 		HDB::UnLock();
        return;
    }
    offset = (int)((time - m_request_time) * m_output_hz);
    if( offset <= 0 )
        offset = 1;
    i = (m_current_bucket + offset) % m_output_hz;
    /* if the client prefers to be the last item within a tick 
     * (e.g. drivers checking their dirty flags), add to the end */
    if( c->GetPriority() == 0)
        vlist_add_first( m_buckets[i], c );
    else
        vlist_add_last( m_buckets[i], c );
	HDB::UnLock();
 }



void HTManager::RegisterClient( HTClient *c )
{
	HDB::Lock();
 	int i;

    if( m_buckets == 0 )
        Init( c->GetNextRequest() );
	//ensure uniqueness of the active clients hash
	if( vhash_lookup_item( m_active_clients, c, 0 ) == VHASH_STATUS_SUCCESS ) {
		HDB::UnLock();
		return;
	}
 	if( vlist_item_exists( m_recently_deleted_clients, c )) {
		//remove any references to the new guy in the schedule
		for( i = 0 ; i < m_output_hz ; i++ ) {
			vlist_remove( m_buckets[i], c );
		}
		vlist_remove( m_spillover, c );
	}
	vhash_insert_item( m_active_clients, c, 0 );
    ScheduleNextTick( c, c->GetNextRequest() );
	HDB::UnLock();
 }


void HTManager::UnRegisterClient( HTClient *c )
{
	HDB::Lock();
 	int expiration;

	if( !m_active_clients ) {
		HDB::UnLock();
 		return;
	}
	vhash_remove_item( m_active_clients, c, 0 );
	//remove any references to the new guy in the schedule
	vlist_remove( m_spillover, c );
	expiration = (int)m_actual_time + 2;
	vlist_add_last( m_recently_deleted_clients, c );
	vlist_add_last( m_recently_deleted_expirations, MVO_I2V(expiration) );
	HDB::UnLock();
 }


HTManager *HTManager::GetCurrentHTManager()
{
    return HDB::GetHTManager();
}



HTClient::~HTClient() 
{
	HDB::GetHTManager()->UnRegisterClient( this );
}

