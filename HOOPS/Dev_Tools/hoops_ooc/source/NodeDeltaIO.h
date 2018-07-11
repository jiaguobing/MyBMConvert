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
#include "NodeDelta.h"
#include "Misc.h"

#include <stdint.h>


//////////////////////////////////////////////////////////////////////////


#ifdef major
#	undef major
#endif

#ifdef minor
#	undef minor
#endif


//////////////////////////////////////////////////////////////////////////


namespace ooc
{
	namespace delta
	{
		namespace on_disk
		{
			// For primitive "integer" types.
			template <typename T>
			class SimpleVectorSerializer {
			public:
				SimpleVectorSerializer (std::vector<T> & vec)
					: vec(&vec)
					, curr_pos(0)
				{}

				TK_Status Serialize (CompressingToolkit & tk)
				{
					while (curr_pos < vec->size()) {
						T const & item = (*vec)[curr_pos];
						TK_Status status = PutData(tk, item);
						if (status != TK_Normal) {
							return status;
						}
						++curr_pos;
					}
					return TK_Normal;
				}

				void ResetSerialization ()
				{
					curr_pos = 0;
				}

			private:
				std::vector<T> * vec;
				size_t curr_pos;
			};


			// For objects that support Serialize and ResetSerialization.
			template <typename T>
			class ObjectVectorSerializer {
			public:
				ObjectVectorSerializer (std::vector<T> & vec)
					: vec(&vec)
					, curr_pos(0)
				{}

				TK_Status Serialize (CompressingToolkit & tk)
				{
					while (curr_pos < vec->size()) {
						T & item = (*vec)[curr_pos];
						TK_Status status = item.Serialize(tk);
						if (status != TK_Normal) {
							return status;
						}
						++curr_pos;
					}
					return TK_Normal;
				}

				void ResetSerialization ()
				{
					for (T & obj : *vec) {
						obj.ResetSerialization();
					}
					curr_pos = 0;
				}

			private:
				std::vector<T> * vec;
				size_t curr_pos;
			};


			//////////////////////////////////////////////////////////////////////////


			void SetFileDeterminedThreshold (float megabytes);


			inline DeltaKind FlagCompression (DeltaKind kind)
			{
				return static_cast<DeltaKind>(static_cast<uint32_t>(kind) | (1u << 31u));
			}

			inline DeltaKind MaskCompression (DeltaKind kind)
			{
				return static_cast<DeltaKind>(static_cast<uint32_t>(kind) & ~(1u << 31u));
			}

			inline bool DenotesCompression (DeltaKind kind)
			{
				return (static_cast<uint32_t>(kind) & (1u << 31u)) != 0u;
			}


			class NodeHandleSerializer {
			public:
				NodeHandleSerializer ()
					: serialization_stage(0)
				{}

				TK_Status Serialize (BStreamFileToolkit & tk, NodeHandle const & node_handle);
				void ResetSerialization ();

				static size_t ByteLength (NodeHandle const & node_handle);

			private:
				int serialization_stage;
			};


			class NodeDelta {
			protected:
				virtual ~NodeDelta () {}

			public:
				virtual void Delete () = 0;
				virtual DeltaKind Kind () const = 0;
				virtual TK_Status Serialize (CompressingToolkit & tk) = 0;
				virtual void ResetSerialization () = 0;
			};


			class ExcludeNodeDelta : public NodeDelta {
			private:
				ExcludeNodeDelta () {}
				virtual ~ExcludeNodeDelta () {}

			public:
				static ExcludeNodeDelta * New ();
				virtual void Delete ()  OVERRIDE;

				virtual DeltaKind Kind () const  OVERRIDE
				{
					return DeltaKind_Exclude_Node;
				}

				virtual TK_Status Serialize (CompressingToolkit & tk)  OVERRIDE;
				virtual void ResetSerialization ()  OVERRIDE;
			};


			class SpecificPointsDelta : public NodeDelta {
			protected:
				SpecificPointsDelta ();
				virtual ~SpecificPointsDelta () {}

			public:
				virtual DeltaKind Kind () const = 0;
				virtual TK_Status Serialize (CompressingToolkit & tk)  OVERRIDE;
				virtual void ResetSerialization ()  OVERRIDE;
				void Add (int32_t point_index);
				void Add (int32_t point_indices[], size_t count);

			private:
				std::vector<int32_t> point_indices;

			private:
				int serialization_stage;
				SimpleVectorSerializer<int32_t> point_indices_serializer;
			};


			class ExcludePointsDelta : public SpecificPointsDelta {
			private:
				ExcludePointsDelta () {}
				virtual ~ExcludePointsDelta () {}

			public:
				static ExcludePointsDelta * New ();
				virtual void Delete ()  OVERRIDE;

				virtual DeltaKind Kind () const  OVERRIDE
				{
					return DeltaKind_Exclude_Points;
				}
			};


			class IntersectPointsDelta : public SpecificPointsDelta {
			private:
				IntersectPointsDelta () {}
				virtual ~IntersectPointsDelta () {}

			public:
				static IntersectPointsDelta * New ();
				virtual void Delete ()  OVERRIDE;

				virtual DeltaKind Kind () const  OVERRIDE
				{
					return DeltaKind_Intersect_Points;
				}
			};


			class TableOfContentsEntry {
			public:
				TableOfContentsEntry () {}
				TableOfContentsEntry (NodeHandle node_handle, uint64_t file_offset = 0);
				TK_Status Serialize (CompressingToolkit & tk);
				void ResetSerialization ();
				size_t ByteLength () const;
				NodeHandle const & GetNodeHandle () const;
				uint64_t GetFileOffset () const;
				void FixFileOffset (uint64_t file_offset);

			private:
				NodeHandle node_handle;
				uint64_t file_offset; // On disk this is offset from the end of the parent file delimiter. In memory, it is from the beginning of file.

			private:
				int serialization_stage;
				NodeHandleSerializer node_handle_serializer;
			};


			class TableOfContents {
			public:
				TableOfContents ();
				TK_Status Serialize (CompressingToolkit & tk);
				void ResetSerialization ();
				size_t ByteLength () const;
				void Add (TableOfContentsEntry const & entry);
				size_t Count () const;
				void Reserve (size_t capacity);
				TableOfContentsEntry & operator[] (size_t idx);
				TableOfContentsEntry const & operator[] (size_t idx) const;

			private:
				std::vector<TableOfContentsEntry> entries;

			private:
				int serialization_stage;
				ObjectVectorSerializer<TableOfContentsEntry> entries_serializer;
			};


			class DeltasSerializer {
			public:
				DeltasSerializer (std::vector<NodeDelta *> & deltas);
				TK_Status Serialize (CompressingToolkit & tk, TableOfContents & toc, uint64_t offset_from_parent_file_delim);
				void ResetSerialization ();

			private:
				DeltasSerializer (DeltasSerializer const &); // disable
				DeltasSerializer & operator= (DeltasSerializer const &); // disable;

			private:
				std::vector<NodeDelta *> & deltas;
				size_t curr_pos;
			
			private:
				int serialization_stage;
			};


			class Version {
				friend class VersionDeserializer;

			public:
				enum Kind : unsigned int {
					Kind_Invalid = 0,
					Kind_Delta_File,
					Kind_3DF,
					Kind_HPS,
				};

				struct BinaryEncoding {
				public:
					enum Reserved	{ ReservedBits	= 10 };
					enum Kind		{ KindBits		= 6 };
					enum Major		{ MajorBits		= 8 };
					enum Minor		{ MinorBits		= 8 };

				public:
					uint32_t reserved	: ReservedBits;
					uint32_t kind		: KindBits;
					uint32_t major		: MajorBits;
					uint32_t minor		: MinorBits;
				};

			public:
				static Version const & Invalid ();
				static Version const & Current (Kind kind);

				Version (Kind kind, unsigned int major, unsigned int minor);

				bool IsKnown (Kind kind) const;
				Kind GetKind () const { return kind; }

				TK_Status SerializeUTF8 (CompressingToolkit & tk);
				TK_Status SerializeBinary (CompressingToolkit & tk);
				void ResetSerialization ();

			public:
				bool operator== (Version const & other) const;
				bool operator!= (Version const & other) const;

			private:
				bool operator< (Version const & other) const;
				bool operator<= (Version const & other) const;

			private:
				Kind kind;
				unsigned int major;
				unsigned int minor;
			};


			enum DeltaFileOption : uint32_t {
				// There currently are no delta file options. This is just a placeholder for now.
				// 
				// Future option might be an option to denote that the deltas are ordered in the
				// file in a certain way, which could allow read optimizations, for example.
				//
				// If the high 22 bits are off, then the delta file option represents a single value.
				// Otherwise the delta file option represents a bitfield of values.
			};


			class DeltaFile {
			public:
				DeltaFile (Version const & delta_file_version, H_UTF8 const & parent_file);
				~DeltaFile ();
				TK_Status Serialize (CompressingToolkit & tk);
				void ResetSerialization ();
				void Add (NodeHandle const & node_handle, NodeDelta * delta);

			private:
				Version delta_file_version;
				Version hoops_3df_version;
				H_UTF8 parent_file;
				uint64_t offset_from_parent_file_delim;
				std::vector<DeltaFileOption> delta_file_options;
				std::vector<NodeDelta *> deltas;
				TableOfContents toc;

			private:
				int serialization_stage;
				DeltasSerializer deltas_serializer;
				uint32_t halves_buffer[2];
			};


			//////////////////////////////////////////////////////////////////////////
		

			class DeltaFileToolkit;


			class DeltaCallbackHandler {
			public:
				virtual void ReceiveDelta (NodeHandle const & node_handle, in_memory::NodeDelta * delta) = 0;
				virtual void ReceiveParentFile (H_UTF8 const & file) = 0;
			};


			class Chunk32Deserializer {
			public:
				Chunk32Deserializer ();
				TK_Status Deserialize (BStreamFileToolkit & tk, uint64_t & out);
				TK_Status Deserialize (BStreamFileToolkit & tk, uint32_t & out);
#ifdef OSX_SYSTEM
				TK_Status Deserialize (BStreamFileToolkit & tk, unsigned long & out);
#endif
				void ResetDeserialization ();

			private:
				uint32_t chunks[3];

			private:
				int deserialization_stage;
			};


			class NodeHandleDeserializer {
			public:
				NodeHandleDeserializer ();
				TK_Status Deserialize (DeltaFileToolkit & tk, NodeHandle & node_handle);
				void ResetDeserialization ();

			private:
				int deserialization_stage;
			};


			class TOCE_Deserializer {
			public:
				TOCE_Deserializer ();
				TK_Status Deserialize (DeltaFileToolkit & tk, TableOfContentsEntry & toc_entry);
				void ResetDeserialization ();

			private:
				NodeHandle node_handle;
				uint64_t file_offset;

			private:
				int deserialization_stage;
				NodeHandleDeserializer node_handle_deserializer;
				Chunk32Deserializer chunk32_deserializer;
			};


			class TOC_Deserializer {
			public:
				TOC_Deserializer ();
				TK_Status Deserialize (DeltaFileToolkit & tk, TableOfContents & toc);
				void ResetDeserialization ();

			private:
				size_t unproccessed_toc_entry_count;
				TableOfContentsEntry toc_entry;

			private:
				int deserialization_stage;
				Chunk32Deserializer chunk32_deserializer;
				TOCE_Deserializer toce_deserializer;
			};


			class ExcludeNodeDeltaDeserializer {
			public:
				ExcludeNodeDeltaDeserializer () {}
				TK_Status Deserialize (DeltaFileToolkit & tk, in_memory::NodeDelta *& out_delta);
				void ResetDeserialization () {}
			};


			template <typename D>
			class SpecificPointsDeltaDeserializer {
			public:
				SpecificPointsDeltaDeserializer (on_disk::DeltaKind const & kind);

				TK_Status Deserialize (
					DeltaFileToolkit & tk,
					in_memory::NodeDelta *& out_delta,
					TableOfContentsEntry const & toc_entry);

				void ResetDeserialization ();

			private:
				SpecificPointsDeltaDeserializer (SpecificPointsDeltaDeserializer const &); // disable
				void operator= (SpecificPointsDeltaDeserializer const &); // disable

			private:
				on_disk::DeltaKind const & kind;
				bool is_file_determined_delta;
				int32_t point_index;
				size_t total_point_count;
				size_t unprocessed_point_count;
				std::unordered_set<int32_t> point_indices;

			private:
				int deserialization_stage;
				Chunk32Deserializer chunk32_deserializer;
			};


			class DeltaDeserializer {
			public:
				DeltaDeserializer ();

				TK_Status Deserialize (
					DeltaFileToolkit & tk,
					in_memory::NodeDelta *& out_delta,
					TableOfContentsEntry const & toc_entry);

				void ResetDeserialization ();

			private:
				on_disk::DeltaKind kind;

			private:
				int deserialization_stage;
				ExcludeNodeDeltaDeserializer exclude_all_deserializer;
				SpecificPointsDeltaDeserializer<in_memory::ExcludePointsDelta> exclusion_deserializer;
				SpecificPointsDeltaDeserializer<in_memory::IntersectPointsDelta> intersection_deserializer;
			};


			class DeltasDeserializer {
			public:
				DeltasDeserializer ();

				TK_Status Deserialize (
					DeltaFileToolkit & tk,
					std::vector<in_memory::NodeDelta *> & deltas,
					TableOfContents const & toc,
					DeltaCallbackHandler & handler);

				void ResetDeserialization ();

			private:
				size_t deserialization_stage;
				DeltaDeserializer delta_deserializer;
			};


			class VersionDeserializer {
			public:
				VersionDeserializer ();
				void ResetDeserialization ();

				TK_Status DeserializeUTF8 (DeltaFileToolkit & tk, Version & version, uint64_t & offset);
				TK_Status DeserializeBinary (DeltaFileToolkit & tk, Version & version);

			private:
				int deserialization_stage;
				uint32_t raw_encoding;
				char buff[64];
				char * pBuff;
			};


			class TailByteLenDeserializer {
			public:
				TailByteLenDeserializer ();
				void ResetDeserialization ();

				TK_Status Deserialize (
					DeltaFileToolkit & tk,
					bool allow_seek,
					uint64_t & tail_byte_len,
					uint64_t & actual_tail_byte_len_offset);

			private:
				uint64_t offset_skew;

			private:
				int deserialization_stage;
				uint32_t halves_buffer[2];
			};


			class DeltaFileDeserializer {
			public:
				DeltaFileDeserializer ();
				TK_Status Deserialize (DeltaFileToolkit & tk, DeltaCallbackHandler & handler);
				void ResetDeserialization ();

			public:
				static io::IOResult FileInputByKey (char * buffer, int buffer_size, wchar_t const * filename, HC_KEY segment_key, HInputHandlerOptions const & options);

			private:
				static bool IsDeltaFile (H_UTF8 const & file);
				static uint32_t NewDeltaFileID ();

			private:
				Version delta_file_utf8_version;
				H_UTF8 parent_file;
				uint64_t offset_from_parent_file_delim; // Offset is relative to beginning of file.
				std::vector<in_memory::NodeDelta *> deltas;
				Version delta_file_binary_version;
				Version hoops_binary_version;
				TableOfContents toc;
				uint64_t tail_byte_len;
				uint64_t actual_tail_byte_len_offset;

				bool peeked;
				unsigned char peek_byte;

			private:
				int deserialization_stage;
				VersionDeserializer version_deserializer;
				Chunk32Deserializer chunk32_deserializer;
				TOC_Deserializer toc_deserializer;
				DeltasDeserializer deltas_deserializer;
				TailByteLenDeserializer tail_byte_len_deserializer;
			};


			class DeltaFileToolkit : public HijackedToolkit {
			private:
				class Hijacker : public TK_Hijack {
				public:
					Hijacker (DeltaCallbackHandler & handler);
					virtual TK_Status Read (BStreamFileToolkit & tk)  OVERRIDE;
					virtual void Reset ()  OVERRIDE;

				private:
					DeltaFileDeserializer deserializer;
					DeltaCallbackHandler & handler;
				};

			public:
				DeltaFileToolkit (
					uint32_t delta_file_id,
					DeltaCallbackHandler & handler,
					MemoryUsage const & approx_memory_used);

				MemoryUsage const & ApproxMemoryUsed () const
				{
					return approx_memory_used;
				}

				void SuppressDiagnostics (bool state);
				uint32_t DeltaFileID () const;
				TK_Status PossiblyAtEOF (bool & result);

			private:
				Hijacker hijacker;
				MemoryUsage const & approx_memory_used;
				uint32_t delta_file_id;
				bool suppress_diagnostics;
				bool suppress_errors_save;
				bool logging_save;
			};
		}
	}
}













