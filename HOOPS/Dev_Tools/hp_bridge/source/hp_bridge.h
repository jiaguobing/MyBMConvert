/*! \file */ 

#ifndef __HP_BRIDGE_H_
#define __HP_BRIDGE_H_

#include "hc.h"
#include "parafrustdefs.h"

#define HP_ERROR_key_out_of_range					-1 /**< This errors is obsolete, as the HOOPS-Parasolid integration no longer renumbers HOOPS keys.  You should not see this error code. */
#define HP_ERROR_non_circle_key_out_of_range		-2 /**< This errors is obsolete, as the HOOPS-Parasolid integration no longer renumbers HOOPS keys.  You should not see this error code. */
#define HP_ERROR_key_does_not_exist					-3 /**< No HOOPS geometry exists for the given key. */
#define HP_ERROR_unable_to_determine_entity_class	-4 /**< The key provided does not map to an existing Parasolid entity.  You have probably performed a Parasolid operation that deleted this entity, without first calling HP_Delete_Entity_Geometry. */
#define HP_ERROR_non_mappable_entity_class			-5 /**< There is no clear mapping from the given HOOPS key to the requested Parasolid entity class. For example, if you pass the key to a HOOPS elliptical arc and ask for PK_FACE, you will get this error code returned. It makes more sense to ask for a PK_EDGE when passing a key to a HOOPS elliptical arc. */
#define HP_ERROR_unknown_entity_class				-6 /**< This error would likely indicate an internal error.  Contact technical support. */
#define HP_ERROR_not_implemented					-7 /**< This error would likely indicate an internal error, or possibly a case where there is no clear mapping from the given HOOPS key to a single entity of the requested class. Contact technical support. */
#define HP_ERROR_hoops_key_maps_to_invalid_entity_class -8 /**< This error would indicate an internal error.  Contact technical support. */
#define HP_ERROR_incorrect_face_tag_mapping			-9 /**< This error would indicate an internal error.  Contact technical support. */

#ifndef _MSC_VER
#define __wchar_t wchar_t
#endif

/*! 
\param pathToSchemaFiles Path to the directory where schema files are located.
\param useBulletinBoard Whether to enable automatic updates.
\param bUnicodeFilenames Enables or disables Unicode keys for frustum and filenames. The default value is true.

\brief Initializes the Parasolid kernel, registers the HOOPS specific implementation of the Parasolid GO_Routines, initializes HOOPS and its connection to the application's user interface.

\details
<p>This is a required call.  Call HP_Init prior to any other HP or PK function.  You must pass a valid string as the path to the schema files if you wish to read or write XMT files of any version other than the version of Parasolid to which your application is linked.</p>

<p>The useBulletinBoard argument will in general be PK_false.  However, if you wish to try out a new, undocumented feature of the integration, you may pass PK_true as the useBulletinBoard argument.  In this case, HP_Render_Entity and HP_Delete_Entity_Geometry should never be called;  instead, after each Parasolid operation, a single call to HP_Update(void) will populate (and de-populate) the HOOPS database with the appropriate geometry by analyzing the Parasolid bulletin board.  Contact support@techsoft3d.com if you require information or assistance with using this feature, which should be considered as being in a beta stage of development.</p>
*/
extern void HP_Init(const char* pathToSchemaFiles, PK_LOGICAL_t useBulletinBoard, bool bUnicodeFilenames=true);

extern void HP_Update(void);

/*!
\brief Shuts down the Parasolid kernel.
*/
extern void HP_Close(void);

extern PK_ERROR_code_t HP_Read_Xmt_File(const char* filename, PK_PARTITION_t file_partition, int* num_parts, PK_PART_t** parts, const PK_PART_receive_o_t* options, bool tesselate=true, const char * face_tessellation_level = nullptr);
extern PK_ERROR_code_t HP_Write_Xmt_File(const char* filename, int num_parts, const PK_PART_t* parts, const PK_PART_transmit_o_t* options);

#ifdef _MSC_VER

/*! 
\param pathToSchemaFiles Path to the directory where schema files are located.
\param useBulletinBoard Whether to enable automatic updates.
\param bUnicodeFilenames Enables or disables Unicode keys for frustum and filenames. The default value is true.

\brief Initializes the Parasolid kernel, registers the HOOPS specific implementation of the Parasolid GO_Routines, initializes HOOPS and its connection to the application's user interface.

\details
<p>This is a required call.  Call HP_Init prior to any other HP or PK function.  You must pass a valid string as the path to the schema files if you wish to read or write XMT files of any version other than the version of Parasolid to which your application is linked.</p>

<p>The useBulletinBoard argument will in general be PK_false.  However, if you wish to try out a new, undocumented feature of the integration, you may pass PK_true as the useBulletinBoard argument.  In this case, HP_Render_Entity and HP_Delete_Entity_Geometry should never be called;  instead, after each Parasolid operation, a single call to HP_Update(void) will populate (and de-populate) the HOOPS database with the appropriate geometry by analyzing the Parasolid bulletin board.  Contact support@techsoft3d.com if you require information or assistance with using this feature, which should be considered as being in a beta stage of development.</p>
*/
extern void HP_Init(const __wchar_t* pathToSchemaFiles, PK_LOGICAL_t useBulletinBoard, bool bUnicodeFilenames=true);
extern PK_ERROR_code_t HP_Read_Xmt_File(const __wchar_t * filename, PK_PARTITION_t file_partition, int* num_parts, PK_PART_t** parts, const PK_PART_receive_o_t* options, bool tesselate=true, const char * face_tessellation_level = nullptr);
extern PK_ERROR_code_t HP_Write_Xmt_File(const __wchar_t * filename, int num_parts, const PK_PART_t* parts, const PK_PART_transmit_o_t* options);

#else

/*! 
\param pathToSchemaFiles Path to the directory where schema files are located.
\param useBulletinBoard Whether to enable automatic updates.
\param bUnicodeFilenames Enables or disables Unicode keys for frustum and filenames. The default value is true.

\brief Initializes the Parasolid kernel, registers the HOOPS specific implementation of the Parasolid GO_Routines, initializes HOOPS and its connection to the application's user interface.

\section Details
<p>This is a required call.  Call HP_Init prior to any other HP or PK function.  You must pass a valid string as the path to the schema files if you wish to read or write XMT files of any version other than the version of Parasolid to which your application is linked.</p>

<p>The useBulletinBoard argument will in general be PK_false.  However, if you wish to try out a new, undocumented feature of the integration, you may pass PK_true as the useBulletinBoard argument.  In this case, HP_Render_Entity and HP_Delete_Entity_Geometry should never be called;  instead, after each Parasolid operation, a single call to HP_Update(void) will populate (and de-populate) the HOOPS database with the appropriate geometry by analyzing the Parasolid bulletin board.  Contact support@techsoft3d.com if you require information or assistance with using this feature, which should be considered as being in a beta stage of development.</p>
*/
extern void HP_Init(const wchar_t* pathToSchemaFiles, PK_LOGICAL_t useBulletinBoard, bool bUnicodeFilenames=true);

/*!
\param filename Name of the Parasolid File to be parsed.
\param file_partition Partition to load parts into.
\param num_parts Pointer to number of parts loaded.
\param parts Pointer to array of parts loaded.
\param options Pointer to receive options structure.
\param tesselate Defaults to true

\brief Parses the specified Parasolid file and populates the modeling kernel with its contents, then maps the model to a corresponding HOOPS segment tree hierarchy including geometry and attributes. 

\section Details
<p>Loads parts from given filename into given partition.  If given partition is a null partition, then a new partition is created.  Arguments "parts" and "num_parts," if non NULL, are mutated to provide an array of parts loaded to the caller.  If parts is non NULL, then an array will be allocated within the HP_Read_Xmt_File function.  If parts is non NULL, then num_parts must also be non NULL.</p>

<p><b>When the caller is finished with this array, the caller should use PK_MEMORY_free(*parts) to free the allocated memory.</b> 
Num_parts indicates the number of parts in the array.</p>

<p>If the file contains assemblies, then these assemblies, and any instances within them, are expanded into PK_BODY entities.  Instanced entities are copied to become full-fledged PK_BODY entities.  The returned array of PK_PART entities will not contain any assemblies.</p>

<p>If NULL is passed as the value for options, then default values will be used for transmitting the file, except that the default format will be the text format.</p>

<p>See the Parasolid documentation of the PK_PART_receive function for further details;  HP_Read_Xmt_File is function is similar to this Parasolid call.</p>

<p>For C# Developers, this function is not available. Instead, we recommend that you use the native C# methods provided by Parasolid.</p>

\return Parasolid error code
*/
extern PK_ERROR_code_t HP_Read_Xmt_File(const wchar_t * filename, PK_PARTITION_t file_partition, int* num_parts, PK_PART_t** parts, const PK_PART_receive_o_t* options, bool tesselate=true);

/*!
\brief Write a collection of Parasolid entities to a file.

\param Name of file to save to.
\param Array of parts to save.
\param Number of parts to save.
\param Pointer to transmit options structure.

\section Details
<p>Writes parts from a partition into a given filename.</p>

<p>If NULL is passed as the value for options , then default values will be used for transmitting the file, except that the default format will be text format. See the Parasolid documentation of the PK_PART_transmit function for further details; this function is more or less identical to the Parasolid call. It is provided for convenience only - at this point equivalent functionality can be achieved by using the PK_PART_transmit function.</p>

\return Parasolid error code.
*/
extern PK_ERROR_code_t HP_Write_Xmt_File(const wchar_t * filename, int num_parts, const PK_PART_t* parts, const PK_PART_transmit_o_t* options);

#endif

/*!
\brief This routine will delete the geometric primitives associated with the given ACIS entities from the HOOPS database and update the mapping between the ACIS pointers and the HOOPS keys.

\param count Number of Parasolid entities in the entities array.
\param entities Array of Parasolid entity tagIDs.
\param PreserveSegments true means delete geometry only, preserve segment structure

\section Details
<p>This function must be called prior to making any Parasolid calls which could modify the topology of a given entity, if it has been previously rendered. Once the Parasolid calls that modify (or delete) the given entity have been completed, the user may choose to call HP_Render_Entities to display the result. This function need not be called if automatic updates are enabled (see HP_Init).</p>


<p>For C# Developers, this function is not available. Instead, we recommend that you use the native C# methods provided by Parasolid.</p>
*/
extern void HP_Delete_Entity_Geometry(unsigned long count, PK_ENTITY_t* entities, bool PreserveSegments=false);

//GH99
/*!
\brief This call will generate the tessellation of a set of Parasolid entities and store the primitives in the HOOPS database using the HOOPS/Parasolid Bridge.

\param count Number of Parasolid entities in the entities array
\param entities Array of Parasolid entities
\param transf Transformation matrix ID for view dependent tesselation (default is 0)
\param face_tessellation_level Facet tessellation level (default is custom). Allowed values are lowest, low, medium, high, highest, custom.

\section Details
This routine encapsulates and replaces the Parasolid PK_TOPOL_render_xxx routines for users of the HOOPS/Parasolid Bridge.  Call this function to populate the HOOPS graphical database. This function need not be called if automatic updates are enabled (see HP_Init).
*/
extern void HP_Render_Entities(unsigned long count, PK_ENTITY_t* entities, PK_TRANSF_t view_transf=NULTAG, PK_TRANSF_t* topol_transf=0, const char * face_tessellation_level = nullptr);

/*!
\brief This call will generate the tessellation of a Parasolid entity and store it in the HOOPS database using the HOOPS/Parasolid Bridge.

\param entity Parasolid entity
\param transf Transformation matrix ID for view dependent tesselation (default is 0)

\section Details 
This routine encapsulates and replaces the Parasolid PK_TOPOL_render_xxx routines for users of the HOOPS/Parasolid Bridge.  Call this function to populate the HOOPS graphical database.  This function need not be called if automatic updates are enabled (see HP_Init).
*/
extern void HP_Render_Entity(PK_ENTITY_t entity, PK_TRANSF_t trans=NULTAG, PK_TRANSF_t topol_transf=NULTAG, const char * face_tessellation_level = nullptr);

/*!
  Updates the given entity by re-tessellating it. The HOOPS segment hierarchy will be retained.
  This API function requires following conditions to succeed
  1. entity to be updated should be of PK_BODY_t class
  2. entity should have already been rendered first by calling HP_Render_Entity API
  3. 'create body segments' rendering option should be on
  
  \param entity The entity to be updated (should be of PK_BODY_t type)
  \param transView transformation (default is NULTAG)
  \param topol_transf Entity transformation (default is NULTAG)
  \return bool true if succeeds
*/
extern bool HP_Update_Entity(PK_ENTITY_t entity, PK_TRANSF_t view_trans=NULTAG, PK_TRANSF_t topol_transf=NULTAG);

/*!
\brief Shows the attribute values previously stored with HP_Set_Tessellation_Options. 

\param line_options Parasolid options structure. Returned to user. Passed by reference.
\param go_options Parasolid options structure. Returned to user. Passed by reference.
\param generation_settings Parasolid options structure. Returned to user. Passed by reference.

\section Details
Caller passes pointers to PK_TOPOL_xxx structures. These must be pointers to valid structures, as this function will not allocate memory.
*/
extern void HP_Show_Tessellation_Options(PK_TOPOL_render_line_o_t* line_Options, PK_TOPOL_render_facet_go_o_t* go_options, PK_TOPOL_facet_mesh_o_t* generation_settings);

/*!
\brief Provides control over how the Parasolid PK_Render routine maps geometry to the HOOPS database via the HOOPS/Parasolid GO_Routines.

\param line_options Parasolid options structure.
\param go_options Parasolid options structure.
\param generation_settings Parasolid options structure.

\section Details
The line_options, go_options, and generation_settings parameters are Parasolid option structures and are passed directly to the appropriate PK_Render_XXX routine. For details on what the Parasolid option structures are, please refer to the Parasolid Reference manual. These options are used during all HP_Render_Entity, HP_Read_Xmt_file, and HP_Update operations.
*/
extern void HP_Set_Tessellation_Options(PK_TOPOL_render_line_o_t* line_Options, PK_TOPOL_render_facet_go_o_t* go_options, PK_TOPOL_facet_mesh_o_t* generation_settings);

/*!
\brief Given a Parasolid tag ID, this routine will return all the HOOPS keys for the tessellated HOOPS geometric primitives associated with the Parasolid entity. 

\param tagID The Tag ID for the Parasolid entity provided for conversion.
\param max_count Maximum number of keys that should be returned.
\param list A quoted string or string variable specifying the particular types of Parasolid geometry that should be converted.
\param keys Array of associated HOOPS keys. Returned to user.
\param count Actual number of keys returned to user in keys array. Always less than or equal to max_count. Returned to user.

\section Details
<p>Given a Parasolid tag ID for any entity, this routine will fill the given array with HOOPS keys for the geometry in the HOOPS database representing that entity.  The value returned will be the total number of keys put into the keys array.  This number will never be greater than the max_count parameter passed in, which specifies the maximum number of keys desired.  The returned value may be less than max_count if the number of HOOPS keys corresponding to the given tagID is less than max_count.  The call HP_Compute_Geometry_Key_Count can be used to determine the size of the keys array that will be returned. </p>

<p>Parasolid body entities have, among others, both face and edge entities. The list specifies which of these entities, or both, should be considered when converting the Parasolid tagID. The list is of the form "specification, specification,...", where each specification can be either "type=value" or "no type".</p>

<p>The following choices are recognized for type:</p>
<table class="table_style1" style="margin: 20px;">
	<tr>
		<td>vertices</td><td>Calculate the keys of the HOOPS geometric primitives representing the PK_Vertex entities in the given Parasolid entity.</td>
	</tr>
	<tr>
		<td>edges</td><td>Calculate the keys of the HOOPS geometric primitives representing the PK_Edge entities in the given Parasolid entity.</td>
	</tr>
	<tr>
		<td>faces</td><td>Calculate the keys of the HOOPS geometric primitives representing the PK_Face entities in the given Parasolid entity.</td>
	</tr>
	<tr>
		<td>bodies</td><td>Calculate the keys of the HOOPS segments representing the PK_Body entities in the given Parasolid entity.</td>
	</tr>
</table>

<p>A negative return value for count indicates an error.  For an explanation of the possible error tokens, please see HOOPS/Parasolid Error Tokens.</p>
*/
extern long HP_Compute_Geometry_Keys(PK_ENTITY_t tagID, long max_count, HC_KEY* keys, const char* list);

/*!
\brief Given a Parasolid tag ID for any entity, this routine will return the number of HOOPS geometric primitives in the HOOPS database that graphically represent the Parasolid entity.  This allows you to create structures of the appropriate size for your subsequent HP_Compute_Geometry_Keys call. 

\param tagID The Tag ID for the Parasolid entity provided for computation.
\param list A quoted string or string variable specifying the particular types of Parasolid geometry that should be converted.
\param count Number of HOOPS keys associated with the given Parasolid entity. Returned to user.

\section Details
<p>Given a Parasolid tag ID for any entity, this routine will compute the number of HOOPS keys that exist for the geometry in the HOOPS database that represents this Parasolid entity.</p>

<p>Parasolid body entities have, among others, both face and edge entities. The list specifies which of these entities, or both, should be considered when converting the Parasolid TagID. The list is of the form "specification, specification,...", where each specification can be either "type=value" or "no type".</p>

<p>The following choices are recognized for type:</p>
<table class="table_style1" style="margin: 20px;">
	<tr>
		<td>vertices</td><td>Calculate the keys of the HOOPS geometric primitives representing the PK_Vertex entities in the given Parasolid entity.</td>
	</tr>
	<tr>
		<td>edges</td><td>Calculate the keys of the HOOPS geometric primitives representing the PK_Edge entities in the given Parasolid entity.</td>
	</tr>
	<tr>
		<td>faces</td><td>Calculate the keys of the HOOPS geometric primitives representing the PK_Face entities in the given Parasolid entity.</td>
	</tr>
	<tr>
		<td>bodies</td><td>Calculate the keys of the HOOPS geometric primitives representing the PK_Body entities in the given Parasolid entity.</td>
	</tr>
</table>

*/
extern long HP_Compute_Geometry_Key_Count(PK_ENTITY_t tagID, char* list);

/*!
\brief Given a HOOPS Key this routine will return the Parasolid Tag ID for the Parasolid entity that contains the specified HOOPS geometric entity. 

\param key HOOPS key provided for computing associated Parasolid entity TagID.
\param paraClass Parasolid Class for desired entity associated with the input HOOPS key.
\param TagID The requested Parasolid tagID. Returned to user. 

\section Details
<p>There is always one and only one Parasolid entity returned by this function.  The returned TagID will be of the type chosen by the caller via the "paraClass" parameter.  For instance, given a HOOPS key for an edge or face in a HOOPS shell, you may request the tagID of the corresponding PK_Vertex, PK_Edge, PK_Face, or PK_Body.</p>

<p>If no tagID can be found which corresponds to the given HOOPS key, a negative error token is returned.  For an explanation of the possible error tokens, please see HOOPS/Parasolid Error Tokens.</p>
*/
extern PK_ENTITY_t HP_Compute_TagID(HC_KEY key, PK_CLASS_t paraClass);

/*!
\brief Provides control over the manner in which Parasolid models are mapped to the HOOPS Segment Tree by the HOOPS/Parasolid GO_Routines implementation.

\param list A quoted string or a string variable containing a list of the desired settings.

\section Details
<p>The following choices are recognized:</p>

<h4>[no] create body segments [= on, off]&nbsp;</h4>
    </b> 
    <blockquote> 
      <p>Controls how each Parasolid Body entity is mapped to the HOOPS segment 
        tree.&nbsp;</p>
      <p> 
      <p>When you specify "create body segments = on",&nbsp; a HOOPS segment is 
        created for each Parasolid body.&nbsp; These HOOPS segments are created 
        as sub segments of the currently open segment.&nbsp; This is useful when 
        you wish to apply unique HOOPS display/rendering attributes to a group 
        of geometric primitives representing a Parasolid Body, or expect to apply 
        additional transformations to individual Parasolid Bodies.&nbsp;</p>
      <p> 
      <p>When you specify "create body segments = off", no extra HOOPS segments 
        are created for individual bodies when they are rendered.&nbsp;</p>
      <p> 
      <p>The default is "create body segments".</p>
    </blockquote>
    <b> 
    <h4>[no] merge faces [= on, off]</h4>
    </b> 
    <blockquote> 
      <p>Controls how the Parasolid face entities are mapped to HOOPS shell primitives.&nbsp;</p>
      <p> 
      <p>When you specify "merge faces", when each subsequent PK_BODY is finished 
        being rendered, the HOOPS shells that have been generated (one for each 
        face) will be merged together into a single shell.&nbsp; In the case of 
        a multi-colored PK_BODY, the shells corresponding to PK_FACES that have 
        the same color attribute will be merged into one HOOPS shell, resulting 
        in as many HOOPS shells are there are distinct colors in the PK_BODY entity. 
        This mapping results in optimal rendering speed at the expense of ease 
        of manipulating individual PK_FACE entities. Applications that are concerned 
        only with viewing, or that do not need to select at the PK_FACE level 
        (to perform 'face level' Parasolid operations) may gain rendering speed 
        advantages by making use of this feature.&nbsp;</p>
      <p> 
      <p>When you specify "no merge faces", one HOOPS shell remains for each PK_FACE 
        rendered.&nbsp; This mode is required if your application must interact 
        with individual PK_FACEs.&nbsp;</p>
      <p> 
      <p>The default is "no merge faces".</p>
    </blockquote>
    <b> 
    <h4>[no] preserve color [= on, off]</h4>
    </b> 
    <blockquote> 
      <p>Controls whether the Parasolid rendered geometry will be partitioned 
        into separate HOOPS segments based on color attributes.&nbsp; This will 
        result in correct rendering of the geometry while maximizing performance 
        by grouping like-colored geometry into the same segment.&nbsp;</p>
      <br>
      <p>When you specify "preserve color = on" , then a HOOPS segment is created 
        for each unique color in the Parasolid geometry being rendered.&nbsp; 
        These HOOPS segments are created as sub segments of the currently open 
        segment.&nbsp; When specified in conjunction with "create body segments 
        = on", the segment that is currently open at the time the color segments 
        are created will be the segment that is unique to the PK_BODY being rendered.&nbsp; 
        Otherwise, the currently open segment will be whatever HOOPS segment the 
        user has opened at the time that the PK_TOPOL_render_facet or PK_TOPOL_render_line 
        is called.&nbsp; Within each created HOOPS segment, the color attributes 
        of the segment will be set (for lines and faces) to the appropriate values.&nbsp;</p>
    </blockquote>
    <blockquote> 
      <p>When you specify "preserve color = off", then color attributes in the 
        Parasolid geometry are ignored when HOOPS geometry is created.&nbsp;</p>
      <p> 
      <p>The default is "preserve color".&nbsp;</p>
      <p> 
      <p>In addition to segments created for each unique color attribute set in 
        the Parasolid geometry being rendered, one additional segment will be 
        created if there is any Parasolid geometry that does not have any color 
        attribute set on it.&nbsp; This segment, like the colored segments, will 
        be created as a child segment of the currently open segment and a sibling 
        segment of any colored segments if any exist.&nbsp; No color attributes 
        will be set in this segment.&nbsp;</p>
      <p> 
      <p>Parasolid PK_EDGEs in general do not have color attributes attached to 
        them.&nbsp; If an edge itself does not have a color attribute attached, 
        then these edges are treated as if they have the color attribute of their 
        owning face.&nbsp; However, an edge may have more than one owning face 
        (usually an edge will have two owning faces), and each of the owning faces 
        may have a different color attribute.&nbsp; In this unusual case, the 
        color of the PK_EDGE is undefined, and may be assigned a color of any 
        one of its owning faces.&nbsp;</p>
      <p> 
      <p>If you wish for PK_EDGEs to ignore color attributes, but you do want 
        faces to be colored by their color attributes, then you can set a global 
        color attribute for line color and specify an "attribute lock" on line 
        color, which will override the line color attributes set in the created 
        color segments (in the HOOPS world, Parasolid PK_EDGEs will correspond 
        to HOOPS line entities, <i>not</i> HOOPS edge entities).&nbsp; As an example 
        of how to do this, you could, in the same segment that is open when you 
        call PK_TOPOL_render_xxxx, call the following two functions:&nbsp;</p>
      <p> 
      <p>&nbsp;&nbsp;&nbsp; HC_Set_Color("lines = white");&nbsp; <br>
        &nbsp;&nbsp;&nbsp; HC_Set_Rendering_Options("attribute lock = (color = 
        (lines))");</p>
    </blockquote>

*/
extern void HP_Set_Rendering_Options(const char* list);

/*!
\brief This routine allows developers to query what the current rendering option settings are as previously stored with HP_Set_Rendering_Options.

\param options A string which the function will write the current rendering options into
*/
extern void HP_Show_Rendering_Options(char* list);
extern void HP_Show_One_Rendering_Option(const char* type, char* value);
extern int HP_CheckConsistency(void);
extern const __wchar_t * HP_Get_Schema_Path(void);
extern void HP_Set_Schema_Path(const char* schema_path);
extern unsigned char HP_Get_Hash_Level(void);
extern void HP_Set_Hash_Level(unsigned char level);

/*!
\brief Allows developer to tell the bridge to honor the assembly definition that may be contained within a XT file.

\param flatten_assembly Indicates whether or not assemblies contained within an XT file are to be flattened

\section Details
If the parameter is true then the bridge flattens the assembly by turning all instances into bodies with an appropriately applied transform. If false then instances will be honored and created as included objects within the segment hierarchy. The default setting is false.
*/
extern void HP_Set_Instances_To_Bodies(bool flatten);

/*! Allows the developer to determine what the modeller will do when it encounters assemblies within a file.

\section Details
This routine allows developers to see what the current behaviour of the bridge is when it encounters an assembly. If true then the bridge flattens the assembly by turning all instances into bodies with an appropriately applied transform. If false then instances will be honored and created as included objects within the segment hierarchy.
*/
extern bool HP_Get_Instances_To_Bodies(void);
extern int HP_Check_Consistency(void);

/*!
\brief This routine will map the provided HOOPS Key to the Parasolid entity.

\param key HOOPS key that needs to be associated to the Parasolid entity.
\param TagID Parasolid entity that should be associated to the HOOPS key.
\param success Success of operation. Returned to user. 

\section Details
The Parasolid entity must exist and there must be no existing mapping to this key for this function to succeed. 
*/
extern bool HP_Associate_Key_To_Entity(PK_ENTITY_t entity, HC_KEY key);
extern void HP_Compute_Selected_Entity_List(PK_ENTITY_t** entities, int* numentities);

#endif
