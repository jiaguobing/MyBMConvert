#ifndef __PARAFILE_H
#define __PARAFILE_H

#include "parafrustdefs.h" // for def of bool

class CPartition;

#ifndef _MSC_VER
#define __wchar_t wchar_t
#endif

class Parafile
{
public:
	Parafile();
	Parafile(const char * in_face_tessellation);
	~Parafile();
	int OpenFile(char *pathName, PK_PARTITION_t file_partition, int* returned_num_parts, PK_PART_t** returned_parts, const PK_PART_receive_o_t *options,bool tesselate=true);
	int OpenFile(__wchar_t *pathName, PK_PARTITION_t file_partition, int* returned_num_parts, PK_PART_t** returned_parts, const PK_PART_receive_o_t *options,bool tesselate=true);
	int OpenFileFlat(char *pathName, PK_PARTITION_t file_partition, int* returned_num_parts, PK_PART_t** returned_parts, const PK_PART_receive_o_t *options,bool tesselate=true);
	int OpenFileFlat(__wchar_t *pathName, PK_PARTITION_t file_partition, int* returned_num_parts, PK_PART_t** returned_parts, const PK_PART_receive_o_t *options,bool tesselate=true);
//	char* GetSchemaPath( void ) { return m_schemaPath; }
	static bool FindSchemaFile( char* m_schemaDir, char* filename );
	static bool FindSchemaFile( __wchar_t* m_schemaDir, __wchar_t* filename );
	HC_KEY OpenAssembly(PK_ASSEMBLY_t assem,bool tesselate, int level);

private:
	int m_nParts;
	PK_PART_t* m_Parts;
	PK_PARTITION_t m_Partition;
	const char * m_face_tessellation;
};

#endif
