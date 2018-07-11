// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#ifndef OOC_LIB_BUILD
#	error "Do not include this file."
#endif

#include "PointCloudAPI.h"

#include "GetData_PutData.h"
#include "Misc.h"

#include "hc.h"

#include "HTools.h"
#include "HGlobals.h"

#include <stdint.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>


//////////////////////////////////////////////////////////////////////////


namespace ooc
{
	class NodeHandle;

	namespace delta
	{
		namespace in_memory
		{
			class AggrogateDelta;
		}

		namespace on_disk
		{
			class NodeHandleDeserializer;
			class VersionDeserializer;
		}
	}
}


//////////////////////////////////////////////////////////////////////////


namespace ooc
{
	namespace delta
	{
		enum CommittedToDisk {
			CommittedToDisk_No,
			CommittedToDisk_Yes,
		};


		namespace on_disk
		{
			enum DeltaKind : uint32_t {
				// Order is important because these are used for serialization purposes.
				// High bit is reserved for denoting compression.

				DeltaKind_Exclude_Node = 0,
				DeltaKind_Exclude_Points,
				DeltaKind_Intersect_Points,
			};
		}


		namespace in_memory
		{
			enum DeltaKind : uint32_t {
				DeltaKind_Aggrogate = 0,
				DeltaKind_File_Determined,
				DeltaKind_Exclude_Node,
				DeltaKind_Exclude_All_Points,
				DeltaKind_Exclude_Points,
				DeltaKind_Intersect_Points,
			};


			enum AcceptResult : uint32_t {
				AcceptResult_Accepted,
				AcceptResult_Rejected,
			};


			class NodeDelta {
				friend class AggrogateDelta;

			public:
				enum AcceptPointsResult {
					AcceptPointsResult_Success,
					AcceptPointsResult_Error_Unknown_Failure,
					AcceptPointsResult_Error_File_System_Failure,
				};

				enum ToAbsoluteIndicesResult {
					ToAbsoluteIndicesResult_Success,
					ToAbsoluteIndicesResult_Error_Unknown_Failure,
					ToAbsoluteIndicesResult_Error_Internal_Failure,
					ToAbsoluteIndicesResult_Error_File_System_Failure,
					ToAbsoluteIndicesResult_Error_Could_Not_Find_Node,
				};

				class ThreadData {
				public:
					ThreadData ();
					~ThreadData ();

				public:
					BStreamFileToolkit * accept_points_toolkit;
					BStreamFileToolkit * to_absolute_indices_toolkit;
				};

			protected:
				NodeDelta ();
				NodeDelta (CommittedToDisk ctd);
				virtual ~NodeDelta () {}

			public:
				virtual void Delete () = 0;
				virtual DeltaKind Kind () const = 0;
				virtual bool AcceptNode () = 0;

				// - This function is allowed to delete the calling object or any of its members.
				// - If this function returns either a pointer to the calling object or a replacement object.
				// - The calling object is deleted if and only if the returned pointer differs from that of the
				//   calling object.
				virtual NodeDelta * Optimize (OptimizeConfig const & config, MemoryUsage & mem_usage) = 0;

				virtual void SetCommittedToDisk ();
				CommittedToDisk GetCommittedToDisk () const;
				bool IsCommittedToDisk () const;
				bool IsOptimized () const;
				virtual bool UnmarkOptimized () = 0;

				AcceptPointsResult AcceptPoints (
					PointCloudWorld const & pcw,
					ThreadData & thread_data,
					AcceptResult out_results[], // `out_results[i]` corresponds to whether or not index `i` of was accepted according to this delta.
					size_t count);

				ToAbsoluteIndicesResult ToAbsoluteIndices (
					PointCloudWorld const & pcw,
					ThreadData & thread_data,
					std::vector<int32_t> & indices,
					int shell_original_size);

				virtual size_t ApproxMemoryUsage () const = 0;

			protected:
				static size_t ApproxMemoryUsageStatic ();

			private:
				virtual AcceptPointsResult AcceptPointsImpl (
					PointCloudWorld const & pcw,
					ThreadData & thread_data,
					AcceptResult out_results[],
					size_t count) = 0;

				virtual ToAbsoluteIndicesResult ToAbsoluteIndicesSieve (
					PointCloudWorld const & pcw,
					ThreadData & thread_data,
					std::vector<int32_t> & all_absolute_indices,
					int32_t const removed_sentinel) = 0;

			protected:
				enum Flags : uint32_t {
					Flags_None				= 0,
					Flags_Committed_To_Disk	= 1 << 0,
					Flags_Optimized			= 1 << 1,
				};
				Flags flags;
			};


			class AggrogateDelta : public NodeDelta {
			public:
				typedef std::vector<NodeDelta *>::iterator iterator;

			private:
				AggrogateDelta () {}
				virtual ~AggrogateDelta ();

			public:
				static AggrogateDelta * New ();
				virtual void Delete () OVERRIDE;

				virtual DeltaKind Kind () const OVERRIDE
				{
					return DeltaKind_Aggrogate;
				}

				virtual bool AcceptNode () OVERRIDE;
				virtual bool UnmarkOptimized () OVERRIDE;
				virtual NodeDelta * Optimize (OptimizeConfig const & config, MemoryUsage & mem_usage) OVERRIDE;
				virtual void SetCommittedToDisk () OVERRIDE;
				void Add (NodeDelta * delta);

				virtual size_t ApproxMemoryUsage () const OVERRIDE;
				size_t ApproxMemoryUsageIncrease (NodeDelta * delta_to_add);

				iterator begin ()
				{
					return deltas.begin();
				}

				iterator end ()
				{
					return deltas.end();
				}

			private:
				virtual AcceptPointsResult AcceptPointsImpl (
					PointCloudWorld const & pcw,
					ThreadData & thread_data,
					AcceptResult out_results[],
					size_t count) OVERRIDE;

				virtual ToAbsoluteIndicesResult ToAbsoluteIndicesSieve (
					PointCloudWorld const & pcw,
					ThreadData & thread_data,
					std::vector<int32_t> & all_absolute_indices,
					int32_t const removed_sentinel) OVERRIDE;

				void Flatten (MemoryUsage & mem_usage);
				void MergeDeltas (MemoryUsage & mem_usage);
				NodeDelta * OptimizeHelper (OptimizeConfig const & config, MemoryUsage & mem_usage);

			private:
				std::vector<NodeDelta *> deltas;
			};


			class FileDeterminedDelta : public NodeDelta {
				enum ToolkitResult {
					ToolkitResult_Success,
					ToolkitResult_Error_File_System_Failure,
				};

			private:
				FileDeterminedDelta (on_disk::DeltaKind delta_file_kind, uint32_t delta_file_id, uint64_t delta_file_offset);
				virtual ~FileDeterminedDelta () {}

			public:
				static FileDeterminedDelta * New (
					on_disk::DeltaKind delta_file_kind,
					uint32_t delta_file_id,
					uint64_t delta_file_offset);

				virtual void Delete () OVERRIDE;

				virtual DeltaKind Kind () const OVERRIDE
				{
					return DeltaKind_File_Determined;
				}

				virtual bool AcceptNode () OVERRIDE;
				virtual bool UnmarkOptimized () OVERRIDE;
				virtual NodeDelta * Optimize (OptimizeConfig const & config, MemoryUsage & mem_usage) OVERRIDE;
				bool MergeIn (FileDeterminedDelta const * delta);

				virtual size_t ApproxMemoryUsage () const OVERRIDE;
				static size_t ApproxMemoryUsageStatic ();

			private:
				FILE * AcquireFile (PointCloudWorld const & pcw);
				bool ReleaseFile (FILE * file);

				template <typename Toolkit>
				struct ToolkitMeta {};

				template <typename Toolkit>
				ToolkitResult RunToolkit (
					PointCloudWorld const & pcw,
					ThreadData & thread_data,
					typename ToolkitMeta<Toolkit>::Env & env);

				virtual AcceptPointsResult AcceptPointsImpl (
					PointCloudWorld const & pcw,
					ThreadData & thread_data,
					AcceptResult out_results[],
					size_t count) OVERRIDE;

				virtual ToAbsoluteIndicesResult ToAbsoluteIndicesSieve (
					PointCloudWorld const & pcw,
					ThreadData & thread_data,
					std::vector<int32_t> & all_absolute_indices,
					int32_t const removed_sentinel) OVERRIDE;

			private:
				uint32_t delta_file_id;
				uint64_t delta_file_offset; // Absolute offset from the beginning of the file to the on_disk::DeltaKind.
				// TODO: Should this class know the byte length of the delta on disk?
			};


			class ExcludeAllPointsDelta : public NodeDelta {
			private:
				static ExcludeAllPointsDelta canonical;

			protected:
				ExcludeAllPointsDelta () {}
				ExcludeAllPointsDelta (CommittedToDisk ctd);
				virtual ~ExcludeAllPointsDelta () {}

			public:
				static ExcludeAllPointsDelta * New (CommittedToDisk ctd = CommittedToDisk_No);
				virtual void Delete () OVERRIDE;

				virtual DeltaKind Kind () const OVERRIDE
				{
					return DeltaKind_Exclude_All_Points;
				}

				virtual bool AcceptNode () OVERRIDE;
				virtual bool UnmarkOptimized () OVERRIDE;
				virtual NodeDelta * Optimize (OptimizeConfig const & config, MemoryUsage & mem_usage) OVERRIDE;

				virtual size_t ApproxMemoryUsage () const OVERRIDE;
				static size_t ApproxMemoryUsageStatic ();

			protected:
				virtual AcceptPointsResult AcceptPointsImpl (
					PointCloudWorld const & pcw,
					ThreadData & thread_data,
					AcceptResult out_results[],
					size_t count) OVERRIDE;

				virtual ToAbsoluteIndicesResult ToAbsoluteIndicesSieve (
					PointCloudWorld const & pcw,
					ThreadData & thread_data,
					std::vector<int32_t> & all_absolute_indices,
					int32_t const removed_sentinel) OVERRIDE;
			};


			class ExcludeNodeDelta : public ExcludeAllPointsDelta {
			private:
				static ExcludeNodeDelta canonical;

				ExcludeNodeDelta () {}
				ExcludeNodeDelta (CommittedToDisk ctd);
				virtual ~ExcludeNodeDelta () {}

			public:
				static ExcludeNodeDelta * New (CommittedToDisk ctd = CommittedToDisk_No);
				virtual void Delete () OVERRIDE;

				virtual DeltaKind Kind () const OVERRIDE
				{
					return DeltaKind_Exclude_Node;
				}

				virtual bool AcceptNode () OVERRIDE;
				virtual NodeDelta * Optimize (OptimizeConfig const & config, MemoryUsage & mem_usage) OVERRIDE;

				virtual size_t ApproxMemoryUsage () const OVERRIDE;
				static size_t ApproxMemoryUsageStatic ();

			private:
				virtual AcceptPointsResult AcceptPointsImpl (
					PointCloudWorld const & pcw,
					ThreadData & thread_data,
					AcceptResult out_results[],
					size_t count) OVERRIDE;
			};


			class SpecificPointsDelta : public NodeDelta {
			public:
				typedef std::unordered_set<int32_t>::const_iterator const_iterator;

			protected:
				SpecificPointsDelta (int32_t const point_indices[], size_t count);
				SpecificPointsDelta (std::unordered_set<int32_t> && point_indices);
				~SpecificPointsDelta () {}

			public:
				virtual bool AcceptNode () OVERRIDE;
				bool IsEmpty () const;

				const_iterator begin () const
				{
					return point_indices.begin();
				}

				const_iterator end () const
				{
					return point_indices.end();
				}

			protected:
				bool Contains (int32_t point_index) const;
				static size_t ApproxMemoryUsageStatic (size_t point_count);

			protected:
				std::unordered_set<int32_t> point_indices;
			};


			class ExcludePointsDelta : public SpecificPointsDelta {
			private:
				ExcludePointsDelta (int32_t const point_indices[], size_t count);
				ExcludePointsDelta (std::unordered_set<int32_t> && point_indices);
				~ExcludePointsDelta () {}

			public:
				static ExcludePointsDelta * New (int32_t const point_indices[], size_t count, CommittedToDisk ctd = CommittedToDisk_No);
				static ExcludePointsDelta * New (std::unordered_set<int32_t> && point_indices, CommittedToDisk ctd = CommittedToDisk_No);
				virtual void Delete () OVERRIDE;

				virtual DeltaKind Kind () const OVERRIDE
				{
					return DeltaKind_Exclude_Points;
				}

				virtual bool UnmarkOptimized () OVERRIDE;
				virtual NodeDelta * Optimize (OptimizeConfig const & config, MemoryUsage & mem_usage) OVERRIDE;
				void MergeIn (ExcludePointsDelta const * delta, MemoryUsage & mem_usage);

				virtual size_t ApproxMemoryUsage () const OVERRIDE;
				static size_t ApproxMemoryUsageStatic (size_t point_count);

			private:
				virtual AcceptPointsResult AcceptPointsImpl (
					PointCloudWorld const & pcw,
					ThreadData & thread_data,
					AcceptResult out_results[],
					size_t count) OVERRIDE;

				virtual ToAbsoluteIndicesResult ToAbsoluteIndicesSieve (
					PointCloudWorld const & pcw,
					ThreadData & thread_data,
					std::vector<int32_t> & all_absolute_indices,
					int32_t const removed_sentinel) OVERRIDE;
			};


			class IntersectPointsDelta : public SpecificPointsDelta {
			private:
				IntersectPointsDelta (int32_t const point_indices[], size_t count);
				IntersectPointsDelta (std::unordered_set<int32_t> && point_indices);
				~IntersectPointsDelta () {}

			public:
				static IntersectPointsDelta * New (int32_t const point_indices[], size_t count, CommittedToDisk ctd = CommittedToDisk_No);
				static IntersectPointsDelta * New (std::unordered_set<int32_t> && point_indices, CommittedToDisk ctd = CommittedToDisk_No);
				virtual void Delete () OVERRIDE;

				virtual DeltaKind Kind () const OVERRIDE
				{
					return DeltaKind_Intersect_Points;
				}

				virtual bool UnmarkOptimized () OVERRIDE;
				virtual NodeDelta * Optimize (OptimizeConfig const & config, MemoryUsage & mem_usage) OVERRIDE;
				void MergeIn (IntersectPointsDelta const * delta, MemoryUsage & mem_usage);

				virtual size_t ApproxMemoryUsage () const OVERRIDE;
				static size_t ApproxMemoryUsageStatic (size_t point_count);

			private:
				virtual AcceptPointsResult AcceptPointsImpl (
					PointCloudWorld const & pcw,
					ThreadData & thread_data,
					AcceptResult out_results[],
					size_t count) OVERRIDE;

				virtual ToAbsoluteIndicesResult ToAbsoluteIndicesSieve (
					PointCloudWorld const & pcw,
					ThreadData & thread_data,
					std::vector<int32_t> & all_absolute_indices,
					int32_t const removed_sentinel) OVERRIDE;
			};


			class NodeDeltas {
			public:
				typedef std::unordered_map<NodeHandle, NodeDelta *, HashKeyAdapter<NodeHandle>> DeltaHash;
				typedef DeltaHash::const_iterator const_iterator;

			public:
				NodeDeltas ();
				~NodeDeltas ();
				void Add (NodeHandle const & node_handle, NodeDelta * delta);
				bool IsCommittedToDisk () const;
				OptimizeResult Optimize (OptimizeConfig const & config);

				MemoryUsage const & ApproxMemoryUsed () const
				{
					return approx_memory_used;
				}

				const_iterator begin () const
				{
					return hash.begin();
				}

				const_iterator end () const
				{
					return hash.end();
				}

				const_iterator find (NodeHandle const & node_handle) const
				{
					return hash.find(node_handle);
				}

			private:
				DeltaHash hash;
				bool optimized;
				MemoryUsage approx_memory_used;
				OptimizeConfig prev_optimize_config;
			};
		}
	}
}







