// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include "hc.h"
#include "HTools.h"
#include "HGlobals.h"
#include "BStream.h"

#include <stdint.h>
#include <functional>


//////////////////////////////////////////////////////////////////////////


#ifdef _WIN32
#	define OOC_API __declspec(dllexport)
#else
#	define OOC_API
#endif


//////////////////////////////////////////////////////////////////////////


class HInputHandlerOptions;
class PointCloudWorld;
class NodeData;


namespace ooc
{
	namespace delta
	{
		namespace on_disk
		{
			class NodeHandleSerializer;
			class NodeHandleDeserializer;
		}
	}
}


//////////////////////////////////////////////////////////////////////////


namespace ooc
{
	class ReifiedEnv;


	/*! The environment for a given point cloud. */
	class OOC_API Env {
		friend class ReifiedEnv;

	public:
		/*! Constructs an invalid point cloud environment. This invalid environment can be tested 
		    against with equality operators.  Aside from the equality operators and ooc::GetEnv, 
		    it is illegal to call any of the OOC API functions with an ooc::Env constructed from 
		    this constructor. It acts as a null pointer. */
		Env ();

		bool operator== (Env const & other) const;
		bool operator!= (Env const & other) const;

	private:
		void * opaque;
		uintptr_t key;
	};


	/*! A generic color class for storing RGB information. */
	class OOC_API RGBColor {
	public:
		/*! Constructs an invalid RGB color. */
		RGBColor()
			: r(-1.0f)
			, g(-1.0f)
			, b(-1.0f)
		{}

		/*! Constructs an RGB color. */
		RGBColor(float r, float g, float b)
			: r(r)
			, g(g)
			, b(b)
		{}

		/*! Returns whether or not the current color is valid. */
		bool IsValid () const {
			return r >= 0.0f && g >= 0.0f && b >= 0.0f;
		}

	public:
		float r;
		float g;
		float b;
	};


	/*! A generic point class for storing coordinate information. */
	class OOC_API Point {
	public:
		/*! Constructs a point at the coordinate system origin */
		Point ()
			: x(0.0f)
			, y(0.0f)
			, z(0.0f)
		{}

		/*! Contructs a point
		
			\param x x-coordinate of point
			\param y y-coordinate of point
			\param z z-coordinate of point
		*/
		Point (float x, float y, float z)
			: x(x)
			, y(y)
			, z(z)
		{}

		/*! Adds this point to parameter other
		
			\param other The point to add this point to
		*/
		Point operator+ (Point const & other) const
		{
			return Point(x + other.x, y + other.y, z + other.z);
		}

		/*! Subtracts parameter point from this point
		
			\param other The point to subtract from this point
		*/
		Point operator- (Point const & other) const
		{
			return Point(x - other.x, y - other.y, z - other.z);
		}

		/*! Multiplies scalar with this point
		
			\param scale The scalar to multiply with this point
		*/
		Point operator* (float scale) const
		{
			return Point(x * scale, y * scale, z * scale);
		}

		/*! Divides scalar with this point
		
			\param scale The scalar to divide with this point
		*/
		Point operator/ (float scale) const
		{
			return Point(x / scale, y / scale, z / scale);
		}

	public:
		float x; /*!< x component of point */
		float y; /*!< y component of point */
		float z; /*!< z component of point */
	};

	/*! A handle for a point cloud node. A NodeHandle is unique per point cloud tree. 
	    That is, if two handles belong to the same point cloud root, they will not compare equal. */
	class OOC_API NodeHandle {
		friend class delta::on_disk::NodeHandleSerializer;
		friend class delta::on_disk::NodeHandleDeserializer;

	private:
		NodeHandle (uint32_t node_file_id, uint32_t node_file_offset);

	public:
		/*! Constructs an invalid node handle of indeterminate state. */
		NodeHandle () {}
		
		/*! Constructs a node handle that is identical to node_data 
			\param node_data The node handle to copy
		*/
		NodeHandle (NodeData const & node_data);

		/*! Returns a node handle that represents an invalid node handle. The node 
		    handle returned by this function can be used as a representative invalid 
		    value. This value can be compared against other handles via the equality 
		    operators.
		*/
		static NodeHandle Invalid ();

		/* Returns a hash code for a given node handle. Provides a means to hash the 
		   value of an ooc::NodeHandle.
		
           A node handle will always return the same hash value in a given run of a 
           program. That being said, this function may return different values between 
           different runs of the program. In other words, the generated hash code is 
           not persistent between different program executions.
		*/
		size_t Hash () const;

		/*! Tests whether or not two node handles are equal.
		  
		    \param other The node handle to compare against.
		    \return true if other equals *this, false otherwise.
		*/
		bool operator== (NodeHandle const & other) const
		{
			return node_file_id == other.node_file_id && node_file_offset == other.node_file_offset;
		}

		/*! Tests whether or not two node handles are unequal.
		  
		    \param other The node handle to compare against.
		    \return true if other is unequal to *this, false otherwise.
		*/
		bool operator!= (NodeHandle const & other) const
		{
			return !(*this == other);
		}

		/*! Puts a strict ordering on node handles.
		
		    \param other The node handle to compare against.
		    \return true if other compares less than *this. Returns false otherwise.
		*/
		bool operator< (NodeHandle const & other) const
		{
			if (node_file_id < other.node_file_id) {
				return true;
			}
			if (node_file_id > other.node_file_id) {
				return false;
			}
			return node_file_offset < other.node_file_offset;
		}

	private:
		uint32_t node_file_id;
		uint32_t node_file_offset;
	};


	/*! Reports whether or not the key ooc_root is the root of a point cloud. 
	
		\param ooc_root The key to test
		\return true if ooc_root is the root of a point cloud and false otherwise. 
	*/
	OOC_API bool IsRoot (HC_KEY ooc_root);
	
	/*! This function is the means to obtain a point cloud's ooc::Env. The out_env 
	    is modified if and only if this function returns true. 
	
		\param ooc_root The root key of a point cloud.
		\param out_env Out parameter for returning the ooc::Env of a point cloud.
		\return true if ooc_root corresponds to a point cloud. Returns false otherwise. 
	*/
	OOC_API bool GetEnv (HC_KEY ooc_root, Env & out_env);
	
	/*! Returns the root key of the point cloud referred to by env. 
	
		\param env The environment of the point cloud to get the root key from.
		\return the root key of the point cloud referred to by env.
	*/
	OOC_API HC_KEY GetRoot (Env env);

	/*!  Destroys a point cloud instance. This both frees memory used by the point 
	     cloud as well as deletes the HOOPS segment tree that belonged to the point 
	     cloud. Passing env to other API functions after this function is called is 
	     illegal. 
	     
		\param env The environment of the point cloud to destroy.
		
		\sa ooc::Release
		\sa ooc::FileInputByKey
	*/
	OOC_API void Destroy (Env env);
	
	/*!  Releases a point cloud instance. This both frees memory used by the point 
	     cloud as well as removes the node populators from the HOOPS segment tree 
	     that belonged to the point cloud. Unlike ooc::Destroy, this function leaves 
	     intact the HOOPS segment structure of the point cloud and any node geometry 
	     that was already loaded into the segments. Passing env to other API functions 
	     after this function is called is illegal.

		\param env The environment of the point cloud to destroy.
		
		\sa ooc::Release
		\sa ooc::FileInputByKey
	*/
	OOC_API void Release (Env env);

	/*! Retrieves the node handle that corresponds to a segment key through the out 
	    parameter out_node_handle. 
	    
	    
		\param env The corresponding point cloud environment.
		\param node_segment_key The segment key to obtain a node handle from.
		\param out_node_handle Out parameter for the returned node handle.
		\return true if successful and false otherwise.
	*/
	OOC_API bool GetNodeHandle (Env env, HC_KEY node_segment_key, NodeHandle & out_node_handle);
	
	/*! Retrieves the segment key that corresponds to a node handle. 
	
		\param env The point cloud environment for node_handle.
    	\param node_handle The node handle of the segment key to be returned.
    	\return the segment key that corresponds to the input node handle.
	*/
	OOC_API HC_KEY GetSegmentKey (Env env, NodeHandle const & node_handle);
	
	/*! Retrieves the shell key that corresponds to a node handle. Returns 
		HC_ERROR_KEY if the node currently has no shell data loaded in memory. 
		Note that the shell keys returned by this function are transient and may 
		be unloaded from memory by the point cloud system. 
	
		\param env The point cloud environment for node_handle.
		\param node_handle The node handle of the shell key to be returned.
		\return the shell key that corresponds to a node handle if successful. Returns HC_ERROR_KEY otherwise. 
	*/
	OOC_API HC_KEY GetShellKey (Env env, NodeHandle const & node_handle);

	/*! This function is a polling mechanism to provide users a means to determine 
	    if updates are needed for a point cloud. 
	
		
		\param env The point cloud environment for node_handle.
		\return true if data is currently being loaded into point cloud nodes. Returns 
		true if data has been loaded into point cloud nodes since the last time this 
		function was called. Returns false otherwise.
		\sa ooc::FileInputByKey
	*/
	OOC_API bool PollNodesLoadingOrHaveBeenLoaded (Env env);

	/*! Internal use only 
	    
	    \param in
	    \param out
	    \return int
	*/
	OOC_API int Configure (char const * in, char * out);


	namespace delta
	{
		class OOC_API SyncToken;

		/*! Designates the outcome of a synchronize operation. */
		enum SyncResult {
			SyncResult_Success,
		};

		/*! Allows users to call functions that require synchronization. This is done 
		    through the func callback passed into this function. func is called with an 
		    ooc::delta::SyncToken reference that corresponds to the environment env 
		    passed into this function. Thus the OOC API functions that accept an 
		    ooc::delta::SyncToken do not accept an ooc::Env because it is implied by 
		    the synchronization token. 
		    
		    \param env The point cloud environment to synchronize with.
    		\param func The callback to call when the point cloud is in a synchronized state
    		\return an ooc::delta::SyncResult return code.
    		\sa ooc::delta::DeleteNode
    		\sa ooc::delta::DeleteSpecificPoints
    		\sa ooc::delta::OptimizeDeltas
		*/
		OOC_API SyncResult SynchronizeWith (Env, std::function<void(SyncToken const &)>);

		class OOC_API InternalSynchronizer {
			friend SyncResult SynchronizeWith (Env, std::function<void(SyncToken const &)>);

		private:
			InternalSynchronizer (Env env);
			~InternalSynchronizer ();

			PointCloudWorld & GetWorld () const;

		private:
			void operator= (InternalSynchronizer const &);

		private:
			PointCloudWorld & pcw;
		};

		enum ModifyResult {
			ModifyResult_Success,
			ModifyResult_Error_Unknown_Failure,
			ModifyResult_Error_Internal_Failure,
			ModifyResult_Error_Could_Not_Find_Node,
			ModifyResult_Error_Input_Indices_Are_Not_Strictly_Increasing,
			ModifyResult_Error_Illegal_Input_Indices,
			ModifyResult_Error_Corrupt_Node,
			ModifyResult_Error_File_System_Failure,
		};

		/*! Deletes the given node. Subsequent uses of node_handle are illegal. 
		
			\param sync_token A token passed in to guarantee that synchronization 
			is taking place for a given point cloud.
    		\param node_handle The handle of the node to be deleted.
    		\return an ooc::delta::ModifyResult return code.
    		\sa ooc::delta::SynchronizeWith
		*/
		OOC_API ModifyResult DeleteNode (SyncToken const & sync_token, NodeHandle const & node_handle);
		
		/*! Deletes specific points of the given node. point_indices must satisfy the following:

    		<ul>
    			<li>contain no duplicate values</li>
    			<li>contain only values in the inclusive range [0, node_point_count - 1] (see ooc::query::PointCount)</li>
   				<li>be sorted in increasing order</li>
   			</ul>
	
			If any of the above rules for point_indices are violated, an appropriate error code will be 
			returned through the ooc::delta::ModifyResult return value.
			 
			\param sync_token A token passed in to guarantee that synchronization is taking place for a given point cloud.
			\param node_handle The node handle of the points to be deleted.
			\param point_indices The list of point indices to be deleted from the node given by node_handle.
			\param count The number of indices in point_indices
			\return an ooc::delta::ModifyResult return code.
			\sa ooc::delta::SynchronizeWith
		*/
		OOC_API ModifyResult DeleteSpecificPoints (SyncToken const & sync_token, NodeHandle const & node_handle, int32_t const point_indices[], size_t count);

		/*! Designates the status of an optimize operation.  */
		enum OptimizeResult {
			OptimizeResult_Success,
			OptimizeResult_Error_Unknown_Failure,
		};

		/*!  Configuration control for ooc::delta::OptimizeDeltas. Currently there are no configuration options for this class. */
		class OOC_API OptimizeConfig {
		public:
			// Currently there are no config options.

		public:
			bool operator== (OptimizeConfig const & other) const;
			bool operator!= (OptimizeConfig const & other) const;
		};

		OOC_API OptimizeResult OptimizeDeltas (SyncToken const & sync_token, OptimizeConfig const & config);

		/*! A token that is passed into functions that can only be called during a 
		    synchronization process. This token helps ensure that such functions 
		    cannot be used outside of an OOC synchronization block. 
		    
		    \sa ooc::delta::SynchronizeWith
		*/
		class OOC_API SyncToken {
			friend SyncResult SynchronizeWith (Env, std::function<void(SyncToken const &)>);
			friend ModifyResult DeleteNode (SyncToken const &, NodeHandle const &);
			friend ModifyResult DeleteSpecificPoints (SyncToken const &, NodeHandle const &, int32_t const[], size_t);
			
			/*! Optimizes the internal representation of the changes made to the point cloud. 
				This function is automatically performed when loading in an OOCD file from disk. 

    			\param sync_token A token passed in to guarantee that synchronization is taking place for a given point cloud.
    			\param config Configuration options for optimization.
				\return an ooc::delta::OptimizeResult return code

				\sa ooc::delta::DeleteNode
    			\sa ooc::delta::DeleteSpecificPoints
    			\sa ooc::delta::SynchronizeWith
			*/
			friend OptimizeResult OptimizeDeltas (SyncToken const & sync_token, OptimizeConfig const & config);

		public:
			Env GetEnv () const;

		private:
			SyncToken (Env env, PointCloudWorld & pcw);
			PointCloudWorld & GetWorld () const;

		private:
			SyncToken (SyncToken const &); // disable
			SyncToken (SyncToken &&); // disable
			void operator= (SyncToken const &); // disable
			void operator= (SyncToken &&); // disable
			void operator& () const; // disable

		private:
			Env env;
			PointCloudWorld & pcw;
		};


		// NOTE: Because std::function is in the API, this function must be defined in the header to avoid ABI problems.
		inline OOC_API SyncResult SynchronizeWith (Env env, std::function<void(SyncToken const &)> func)
		{
			InternalSynchronizer synchronizer(env);
			PointCloudWorld & pcw = synchronizer.GetWorld();
			SyncToken const sync_token(env, pcw);
			func(sync_token);
			return SyncResult_Success;
		}
	}


	namespace io
	{
		/*! Designates the status of an IO operation. */
		enum IOResult {
			IOResult_Success,
			IOResult_Error_Unknown_Failure,
			IOResult_Error_No_HBaseView,
			IOResult_Error_Cyclic_File_Dependencies,
			IOResult_Error_Serialization_Failure,
			IOResult_Error_Deserialization_Failure,
			IOResult_Error_Illegal_File_Name,
			IOResult_Error_File_System_Failure,
			IOResult_Error_OOC_Root_Already_Exists,
			IOResult_Error_Invalid_Buffer,
		};

		/*! Loads the given OOC or OOCD file into the HOOPS segment segment_key. Accepts 
			both / and \ as file path seperators. options.m_pHBaseView must point to a valid 
			HBaseView object. If options.m_bOOCAutoUpdate is set, then the HBaseView the file 
			gets loaded with will automatically update the HOOPS scene when nodes get populated. 
			This option is set to true by default. The automatic update logic makes periodic 
			calls to ooc::PollNodesLoadingOrHaveBeenLoaded in order to determine if an update 
			is needed. If options.m_bOOCAutoCleanup is set, then ooc::Release is called when the 
			HBaseView the file gets loaded with gets destructed. This option is set to true by default. 
			
			\param file_name - The file_name of the point cloud file to load.
			\param segment_key - The segment key to load the point cloud file into.
			\param options - Configuration options for loading the file.
			\return an ooc::io::IOResult return code.
			\sa ooc::Release
		    \sa ooc::PollNodesLoadingOrHaveBeenLoaded
		*/
		OOC_API IOResult FileInputByKey (wchar_t const * file_name, HC_KEY segment_key, HInputHandlerOptions const & options);
		OOC_API IOResult FileInputByKey (char * buffer, int buffer_size, wchar_t const * file_name, HC_KEY segment_key, HInputHandlerOptions const & options);

		/*! Saves to disk any changes made to the point cloud. Beware that moving or renaming dependent 
		    files will impact the saved delta file (OOCD) because the OOCD file references them. The 
		    most likely result would be that the file cannot be loaded.
		    <br>
			A file is generated in the directory xxx/yyy_deltas/ as zzz.oocd where

			<ul>
				<li>xxx is the root directory for the OOC files</li>
				<li>yyy is given by xxx/yyy.ooc</li>
				<li>zzz is the generated file_name for the changes saved to disk.</li>
			</ul>
			
			Example: C:/some_dir/ooc_house_root/house.ooc C:/some_dir/ooc_house_root/house_deltas/00535fe253-0000251e.oocd
			Here

			<ul>
				<li>xxx is C:/some_dir/ooc_house_root</li>
				<li>yyy is house</li>
				<li>zzz is 00535fe253-0000251e </li>
			</ul>
		
			\param env The environment of the point cloud whose uncommitted changes will be committed to disk.
			\param out_generated_file_name If non-null, then the pointed to H_UTF8 is populated with the absolute file path of the generated file. / is used as the file path seperator in the generated file.
			\return an ooc::io::IOResult return code.
			\sa ooc::delta::DeleteNode
    		\sa ooc::delta::DeleteSpecificPoints
    		\sa ooc::delta::HasUncommittedDeltas
		*/
		OOC_API IOResult CommitDeltasToFile (Env env, H_UTF8 * out_generated_file_name = 0);
		OOC_API IOResult CommitDeltasToFile (char * buffer, int buffer_size, Env env, H_UTF8 * out_generated_file_name = 0);

		/*! Same as ooc::io::CommitDeltasToFile(ooc::Env env, H_UTF8 * out_generated_file_name), but instead of writing 
			the OOCD file to a generated file name, the file path is supplied by file_name. 
		*/
		OOC_API IOResult CommitDeltasToFile (Env env, H_UTF8 const & file_name);
		OOC_API IOResult CommitDeltasToFile (char * buffer, int buffer_size, Env env, H_UTF8 const & file_name);

		/*! Queries whether or not there are any uncommitted deltas in memory for a particular point cloud tree. 
		
			\param env The environment of the point cloud to query.
			\return true if there have been any changes made to the point cloud that have not been committed 
			to disk. Returns false otherwise.
			\sa ooc::io::CommitDeltasToFile
    		\sa ooc::io::CommitDeltasToFile

		*/
		OOC_API bool HasUncommittedDeltas (Env env);
	}


	namespace query
	{
		class QueryIterator;

		/*! Designates whether or not data from memory or from disk.  */
		enum Storage {
			Storage_Memory,
			Storage_Disk,
		};

		/*! ooc::query::Filter is an abstract class used to indicate what point data should be retrieved from ooc::query::QueryPoints.  */
		class OOC_API Filter {
			friend class QueryIterator;

		public:
			Filter () {}
			virtual ~Filter () {}

			/*! Determines whether or not points in memory are rejected by the filter. 
			
				\return true if the filter rejects points that are loaded into memory. Returns false otherwise. 
			*/
			virtual bool RejectPointsInMemory () = 0;
			
			/*! Determines whether or not points *not* in memory are rejected by the filter. 
			
				\return true if the filter rejects points that are not loaded into memory. Returns false otherwise. 
			*/
			virtual bool RejectPointsOnDisk () = 0;
			
			/*! Determines whether or not a point cloud node is rejected by the filter. 
			
				\param node_handle The handle of the node to accept or reject.
    			\return true if the node is rejected. Returns false otherwise.
			*/
			virtual bool RejectNode (NodeHandle const & node_handle) = 0;
			
			/*! Determines whether or not a bounding box of points is rejected by the filter. 
				
    			\param min_bound The minimum corner of the bounding box to accept or reject in object space.
    			\param max_bound The maximum corner of the bounding box to accept or reject in object space.
    			\return true if the bounding is rejected. Returns false otherwise.	
			*/
			virtual bool RejectBounding (Point const & min_bound, Point const & max_bound) = 0;
			
			/*! Determines whether or not a single point is accepted by the filter. 
			
				\param point The coordinates of the point to accept or not in object space.
				\return true if the point is accepted. Returns false otherwise. 
			*/
			virtual bool AcceptPoint (Point const & point, size_t point_index) = 0;
		};

		/*! ooc::query::QueryResult is the current result of ooc::query::QueryIterator. It contains 
			information about the queried point data. */
		class OOC_API QueryResult {
			friend class QueryIterator;

		private:
			QueryResult ();

		public:
			/*! Yields the currently processed point's node handle. 
			
				\return the node handle of the current result
			*/
			NodeHandle const & GetNodeHandle () const;
			
			/*! Yields the currently processed point's coordinates in object space. 
			
				\return the object space coordinates for the point of the current result
			*/
			RGBColor const & GetNodePointColor () const;

			/*! Yields the currently processed point's RGB color. 
			
				\return the RGB color for the point of the current result
			*/
			Point const & GetNodePoint () const;
			
			/*! Yields the currently processed point's index. 
			
				\return the index for the point of the current result.
			*/
			size_t GetNodePointIndex () const;
			
			/*! Yields the currently processed point's storage. 
			
				\return the storage for the point of the current result.
			*/
			Storage GetStorage () const;

		private:
			NodeHandle node_handle;
			RGBColor node_point_color;
			Point node_point;
			size_t node_point_index;
			Storage storage;
		};

		/*! Allows users to query the points in a point cloud tree. Based on the input filter, the returned 
			points may include both points in memory and on disk. 
		
			\param env The environment of the point cloud to query points from.
			\param filter The filter to select points with.
			\return an iterator to the queried points.
		*/
		OOC_API QueryIterator QueryPoints (Env env, Filter & filter);
		
		/*! Determines the number of points in a given node.
			
    		\param env The point cloud environment for node_handle.
    		\param handle The handle of the node whose point count is to be returned.
    		\return the point count of the node given by node_handle if successful. Returns a negative value otherwise.
		*/
		OOC_API int PointCount (Env env, NodeHandle const & handle);

		/*! The iterator returned from a call to ooc::query::QueryPoints. */
		class OOC_API QueryIterator {
			friend QueryIterator QueryPoints (Env, Filter &);

		public:
			enum Status {
				Status_Alive,
				Status_Dead,
				Status_Error_Unknown_Failure,
				Status_Error_File_System_Failure,
				Status_Error_Deserialization_Failure,
				Status_Error_Could_Not_Find_Node,
				Status_Error_Corrupt_Node,
			};

		private:
			QueryIterator (Filter & filter, Status status);
			QueryIterator (Env env, Filter & filter, NodeHandle const & node_handle);

		public:
			/*! Constructs a dead query iterator. 
				\return The dead iterator.
			*/
			QueryIterator ();
			
			/*! Constructs a query iterator from another with the same state and progress of other. 
				\return The query iterator to construct from.
			*/
			QueryIterator (QueryIterator && other);
			~QueryIterator ();

			/*! This is only legal to call when this->GetStatus() returns ooc::query::QueryIterator::Status_Alive. 
			
				\return the current point result of the query.
			*/
			QueryResult const & operator* () const;
			
			/*! This is only legal to call when this->GetStatus() returns ooc::query::QueryIterator::Status_Alive. 
			
				\return the current point result of the query.
			*/
			QueryResult const * operator-> () const;

			/*! 
			
				\return the status of the iterator.
			*/
			Status GetStatus () const;
			
			/*!  Advances the iterator to the next point result. This is only legal to call when this->GetStatus() returns ooc::query::QueryIterator::Status_Alive.
			*/ 
			void Advance ();

		private:
			void InvalidateWith (Status status);

			enum HookStatus {
				HookStatus_Success_Hooked,
				HookStatus_Success_Skipped,
				HookStatus_Error_Unknown_Failure,
				HookStatus_Error_File_System_Failure,
				HookStatus_Error_Deserialization_Failure,
				HookStatus_Error_Corrupt_Node,
			};

			void Hook ();
			HookStatus HookNode (NodeHandle const & node_handle, HC_KEY const node_segment_key);
			HookStatus HookNodePoints (HC_KEY const node_segment_key);
			HookStatus HookNodePointsAgainstShell (HC_KEY const node_shell_key);
			HookStatus HookNodePointsAgainstDisk (HC_KEY const node_segment_key);

		private:
			QueryIterator (QueryIterator const &); // disable
			void operator= (QueryIterator const &); // disable
			void operator= (QueryIterator &&); // disable

		private:
			void * typeless_pending_node_keys;
			void * typeless_point_buffer;
			void * typeless_color_buffer;
			size_t point_buffer_idx;
			Filter & filter;
			Env const env;
			QueryResult query_result;
			Status status;
		};
	}


	namespace preprocess
	{
		enum PreprocessStatus {
			PreprocessStatus_Success,				/*!< Preprocess completed without problem. */
			PreprocessStatus_Unknown_Failure,		/*!< An unknown problem occurred. */
			PreprocessStatus_Setup_Failure,			/*!< A problem occurred during pre-process setup (directory creation, conflicting files, etc.). */
			PreprocessStatus_First_Pass_Failure,	/*!< A problem occurred during initial pass (parsing text file for bounding box, binary file generation). */
			PreprocessStatus_Second_Pass_Failure,	/*!< A problem occurred during second pass (spatially sorting all the points). */
			PreprocessStatus_Third_Pass_Failure,	/*!< A problem occurred during third pass (node generation and randomization, index generation). */
		};


		class OOC_API Preprocessor {
		public:
			Preprocessor (wchar_t const * output_file_name);
			~Preprocessor ();

			PreprocessStatus Run ();

			void SetLogFile (wchar_t const * log_file);
			void AddPointCloudFile (wchar_t const * point_cloud_file);
			void SetMaxShellSize (int shell_size);
			void SetMaxMemoryUsage (size_t max_memory_usage);
			void SetSubSamplePercentage (double percentage);
			void SetCullingBoundingBox (Point const & min, Point const & max);
			void OverwriteExistingFiles (bool overwrite);

		private:
			void * opaque_point_cloud;
		};
	}
}















