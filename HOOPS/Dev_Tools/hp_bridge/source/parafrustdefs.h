#ifndef __PARAFRUSTDEFS_H_
#define __PARAFRUSTDEFS_H_

#ifdef NEED_BOOL_TYPE
#undef NEED_BOOL_TYPE
	typedef int bool;
	
	const bool true = !0;
	const bool false = 0;
#endif

#define HP_ERROR_unable_to_determine_entity_class		-4
#define HP_ERROR_non_mappable_entity_class				-5
#define HP_ERROR_unknown_entity_class					-6
#define HP_ERROR_not_implemented						-7
#define HP_ERROR_hoops_key_maps_to_invalid_entity_class -8
#define HP_ERROR_incorrect_face_tag_mapping				-9

#define MAX_SCHEMA_PATH_LENGTH		2048

#define HP_GEOMETRY_TYPE_vertices	1
#define HP_GEOMETRY_TYPE_edges		2 
#define HP_GEOMETRY_TYPE_faces		4
#define HP_GEOMETRY_TYPE_bodies		8
#define HP_GEOMETRY_TYPE_instances	16
#define HP_GEOMETRY_TYPE_assemblies 32
#endif

