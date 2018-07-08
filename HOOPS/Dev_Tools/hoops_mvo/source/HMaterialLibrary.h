// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

/*! \file HMaterialLibrary.h
	Defines an interface to create sophisticated materials 
*/
#ifndef _HMATERIALLIBRARY_H
#define _HMATERIALLIBRARY_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "BStream.h"


class HTK_One_Material;

/*!
HMaterialLibrary is the main class for working with the material library.
*/	
class MVO_API HMaterialLibrary {

public:
	HMaterialLibrary();
	virtual ~HMaterialLibrary();

	/*
	 * basic usage
	 */

	/*! Scans the given directory path and records the names of materials it discovers.
		Also creates a style link from the materials segment to the segment that is currently open.
		This function should be called from somewhere above the model in the model's path,
		but materials_segment does not necessarily need to be in the models path.
		\param directory the location on disk of the materials
		\return true on success, false if no materials were found at the given location */
	bool RegisterMaterials (const char *directory);
	/*! Scans the given directory path and records the names of materials it discovers.
		\param directory the location on disk of the materials
		\return true on success, false if no materials were found at the given location */
	bool RegisterMaterials (const wchar_t *directory);
	/*! \return true if at least one material has been registered */
	bool IsValid () const { return m_Valid; }
	/*! After materials are registered, something in the model's ancestry needs to be styled by the materials
		segment, so that any and all name definitions are made available.  We suggest the scene key, but really we
		can use anything from which the model inherits attributes provided it is in the same subwindow as the model.
		This function applies the library styles to the current segment.
		\return true on success */
	bool ApplyStyles();
	/*! returns the name of the root segment under which resources are stored.
	    \returns name of the container segment */
	const char *GetContainerSegment () const;
	/*! \param buffer The buffer to write the results into.  If non-NULL, it will also be returned back to the user. 
		\param buffer_length The maximum number of characters to write into buffer.  If less than
		or equal to zero, the caller is assumed to have allocated sufficient space to hold all contents.
		\return a semicolon-separated list of legal values to be passed into SetEnvironment. 
	    To allow for wide character material names, the names are encoded as UTF8, which is mostly
		the same as ASCII with minor exceptions.  Convert to unicode before using these values to 
		populate a GUI for proper support of wide characters (e.g. Asian languages) being used in 
		material names. This function will return NULL until 
		after RegisterMaterials() is called.  If buffer is NULL (the default), the return value is
		a pointer to the internal array used to store the material list.  That array will be invalidated
		(and reallocated) upon the next call to CloneMaterial or RenameMaterial, so the caller should take care
		not to hang on to a stale pointer.  */ 
	const char *GetMaterialList(char *buffer = 0, int buffer_length = -1);
	/*! \param name the name of the material to search for.
		\return true if the given name is in the list. This function will always return false until 
		after RegisterMaterials() is called.  */ 
	bool IsValidMaterial (const char *name) const;
	/*! Applies all of the attributes of the given material to the currently open segment.
		Initializes the material if necessary by pulling the resources (DAT, shader and images) from disk.
		\param name UTF8-encoded name as passed back from GetMaterialList() 
		\return true on success */
	bool ApplyMaterial (const char *name);
	/*! Refresh the material from disk.  
		Note that any new materials that were cloned will not be affected.
	    \param name the material name
		\return true on success.  This function will return false if called on a material that is created via CloneMaterial, 
		since there is no corresponding data on disk.
	 */
	bool ReloadMaterial (const char *name);  

	/*
	 * for run-time tweaks
	 */
	/*! creates a copy of a material with a new name.  
	    \param old_material_name the material to be copied
		\param new_material_name the new material name.  It will be the old material name plus a 
				suffix guaranteed not to conflict with any materials yet defined, starting with "+1".
		\return true on success */
	bool CloneMaterial (const char *old_material_name, char *new_material_name);
	/*! creates a copy of a material with a new name.  
	    \param old_material_name the material to be copied
		\param new_material_name the new material name.
		\return true on success.  Fails if old_material_name is not a valid material or new_material_name is already in use. */
	bool RenameMaterial (const char *old_material_name, const char *new_material_name);
	/*! Allows the user to show the number of tweakables.  
		\param material_name the material name
		\param the number of materials.
		\return true on success (even if the count is 0) */
	bool GetTweakablesCount (const char *material_name, int *count);
	/*! Tweakables can be identified by a combination of material name and index.  
		\param material_name the material name
		\param index the tweakable index (0-based offset from the start of the comma-delimited tweakables string in the material's DAT file).
		\param tweakable_name the name of the tweakable, returned to user (e.g. for GUI labels)
		\param type the tweakable type.  Possible values are float, float2, float3 and float4
		\return true on success, false if material_name matches no material yet defined or if the index is out of range. */
	bool GetTweakableByIndex (const char *material_name, int index, char *tweakable_name, char *type);
	/*! Tweakables can be identified by a combination of material name and tweakable name.  
		\param material_name the material name
		\param tweakable_name the name of the tweakable
		\param index the tweakable index, returned to user.
		\param type the tweakable type.  Possible values are float, float2, float3 and float4
		\return true on success, false if material_name matches no material yet defined or if the index is out of range. */
	bool GetTweakableByName (const char *material_name, char const *tweakable_name, int *index, char *type);
	/*! A utility to determine the number of floating point values required to store the data for a given type.  
	    \param type must be one of: "float", "float2", "float3", "float4"
		\return count, or -1 if type is not one of the expected types */
	int CountFromType (const char *type);
	/*! pulls the initializer for a tweakable (specified by material_name and index) and returns the values to the user.
		\param material_name the material name
		\param the tweakable index (0-based offset from the start of the comma-delimited tweakables string in the material's DAT file).
		\param vals a pointer to up to 4 float values (depending on the type as returned by GetTweakable()) to be returned to the user currently specified in the hlsl code.
		\return true on success, false if material_name matches no material yet defined or if the index is out of range. */
	bool GetTweakableDefaults (const char *material_name, int index, float *vals);
	/*! pulls the initializer for a tweakable (specified by material_name and index) and replaces the values with values specified by the user.
		\param material_name the material name
		\param the tweakable index (0-based offset from the start of the comma-delimited tweakables string in the material's DAT file).
		\param vals a pointer to up to 4 float values (depending on the type as returned by GetTweakable()) to be injected into the hlsl code.
		\return true on success, false if material_name matches no material yet defined or if the index is out of range. */
	bool Tweak (const char *material_name, int index, const float *vals);
	/*! a convenience wrapper around Tweak (const char *, int, const float *)
		\param material_name the material name
		\param the tweakable index (0-based offset from the start of the comma-delimited tweakables string in the material's DAT file).
		\param val0 the first value injected into the hlsl code.
		\param val1 the second value injected into the hlsl code (if applicable, depending on the tweakable's type).
		\param val2 the third value injected into the hlsl code (if applicable, depending on the tweakable's type).
		\param val3 the fourth value injected into the hlsl code (if applicable, depending on the tweakable's type).
		\return true on success, false if material_name matches no material yet defined or if the index is out of range. */
	bool Tweak (const char *material_name, int index, float val0, float val1=0, float val2=0, float val3=0 );

	/*
	 * environment texture set up
	 */
	/*! \return a semicolon-separated list of legal values to be passed into SetEnvironment.  Returns NULL until after RegisterMaterials() is called */ 
	const char *GetEnvironmentList(char *buffer = 0, int buffer_length = -1);
	/*! \param name the name of the environment to search for.
		\return true if the given name is in the list. This function will always return false until 
		after RegisterMaterials() is called.  */ 
	bool IsValidEnvironment (const char *name) const;
	/*! An environment typically contains a cube map, and specifies the colors that are picked up 
		by reflective materials and optionally the color of the window background 
		\param name the environment to set
		\return true on success */
	bool SetEnvironment (const char *name);

	/*! Checks whether the material has already been initialized, and calls through 
		to Initialize if it has not.
		\param name the material name
		\return true on success */
	bool EnsureMaterialInitialized (const char *name);
	/*! Checks whether the environment has already been initialized, and calls through 
		to Initialize if it has not.
		\param name the material name
		\return true on success */
	bool EnsureEnvironmentInitialized (const char *name);

	bool FindReferencedMaterials (HC_KEY segment_tree, char *list, int list_size);
	bool InitWrite (const char *list, int *size);
	bool InitRead (int version);
	TK_Status ParseBuffer (char const *buffer, int size);
	const char *GetCurrentEnvironment(char *buffer = 0, int buffer_length = -1);

	/*!	\param name the material name to be compiled.  
		\return true on success.  Outputs an hsf to the directory given to RegisterMaterials, which will fail if that 
		directory is either not writeable or invalid.  The output file will be of the same name as the material, except 
		with '/' converted to '.' and a ".hsf" appended to the end.  */
	bool Compile (const char *name);

	/*! \enum Read_Flags 
		options for file import */
	enum Read_Flags {
		Read_Flags_Default			= 0x0000, //!< use textures in the HMaterialLibrary in preference to any that are embedded in the data
		Prefer_Embedded				= 0x0001, //!< keep the embedded material if there is a name conflict between existing and embedded one
		Prefer_Highest_Resolution	= 0x0002, //!< keep the one with the highest resolution if there is a name conflict between existing and embedded one
	};
	void SetReadFlags (Read_Flags flags) { m_ReadFlags = flags; }; //!<see documentation on HMaterialLibrary::Read_Flags
	Read_Flags GetReadFlags () { return m_ReadFlags; }; //!<see documentation on HMaterialLibrary::Read_Flags

	/*! \enum Read_Flags 
	    options for file export */
	enum Write_Flags {
		Write_Flags_Default			= 0x0000, //!< write with no textures at all to keep file sizes compact
		Embedded					= 0x0001, //!< embed hsf versions of the materials into the data so that the receiving side 
		//Baked						= 0x0002, //!< write plain attributes instead of materials so that the receiving side can view without any special handling (not yet implemented)
		//Downsample				= 0x0004, //!< embed material data, but choose a variant with low resolution (not yet implemented)
	};
	void SetWriteFlags (Write_Flags flags) { m_WriteFlags = flags; }; //!<see documentation on HMaterialLibrary::Write_Flags
	Write_Flags GetWriteFlags () { return m_WriteFlags; }; //!<see documentation on HMaterialLibrary::Write_Flags

protected:
	const wchar_t *m_MaterialDirectory;		//!<the path to the materials and environments on disk
	struct vhash_s *m_MaterialHash;			//!<maps material names to the compiled material hsf file from which it came
	struct vhash_s *m_EnvironmentHash;		//!<maps environment names to the compiled material hsf file from which it came
	const char *m_MaterialList;				//!<the material list
	const char *m_EnvironmentList;			//!<the environment list
	const char *m_CurrentEnvironment;		//!<the name of the environment most recently set with SetEnvironment
	HC_KEY m_MaterialSegmentKey;			//!<the segment containing the materials and environments
	HC_KEY m_CurrentEnvironmentKey;			//!<the currently active environment
	Read_Flags m_ReadFlags;					//!<see documentation on HMaterialLibrary::Read_Flags
	Write_Flags m_WriteFlags;				//!<see documentation on HMaterialLibrary::Write_Flags
	bool m_Valid;							//!<self-explanatory

	/*! Loads resources from disk to populate material attributes 
		\param name the material name
		\param environment indicates whether the data will be used as an 
				environment (which should contain texture definitions but 
				no color settings).
		\return true on success */
	bool Initialize (const char *name, bool environment = false);

	/*! Reports an error.
		\param message the string to be shown to the user
		\param severity warning = 1, error = 2, fatal error = 3
	*/
	virtual void Error (const char *message, int severity = 1);
	/*! Pulls the comma-separated list of tweakables for a given material out of
		the material segment's user data.
		\param material_name the material name
		\param string the list of tweakables, returned to the caller
	*/
	bool GetTweakablesString (const char *material_name, char *string);
	/*! Pulls the shader source for a given material out of the material segment, 
		using Show_Shader()
		\param material_name the material name
		\param shader_source hlsl/glsl shader source code.  returned to user.
		\param options defaults to NULL, but if specified, a buffer in which to put the 
			options associated with a material's shader definition.
	*/
	bool GetShaderSource (const char *material_name, char *shader_source, char *options = 0);

	/*! \param name the name of the material to search for.
		\return if the given name is in the list, return a pointer to the location in the 
		m_MaterialList where the requested material name starts.  This function will always 
		return NULL until after RegisterMaterials() is called.  */ 
	const char *FindValidMaterial (const char *name) const;

	/*! \param name the name of the material be compiled.
		\param bufferlist a vlist of SerializationNode objects onto which the compile results should be appended
		\return true on success */ 
	bool Compile (const char *name, void *bufferlist);

private:
	struct vlist_s *m_SerializationBufferList;
	BStreamFileToolkit *m_tk;

	enum ErrorCode {
		HMatOK = 1,
		HMatParseError = -1,
		HMatFileError  = -2,
	};

	void Init();
	void RecordHSFMaterial (const wchar_t *path, const wchar_t *filename);
	void FindHSFMaterials (const wchar_t *path);
	bool HasIndexFile (const wchar_t *path, const wchar_t *subpath, const wchar_t *subdir, const wchar_t *index_file);
	void FindSubdirectories (const wchar_t *path, const wchar_t *subpath);
	bool GenerateSerializationBufferList (const char *materials);
	const char *HashToString (struct vhash_s *hash, char *buffer = 0, int buffer_size = -1) const;
	ErrorCode slurp_dat_file(				   
			const wchar_t *index_file, 
			const wchar_t *subdir, 
			const wchar_t *path, 
			void *datlist);
	friend class HTK_One_Material;
	friend class HTK_Material_Composite;
};




class MVO_API HTK_Material_Composite : public TK_Material
{
public:
	HTK_Material_Composite (HMaterialLibrary *hmat, HC_KEY key) : TK_Material() {
		H_ASSERT (hmat != NULL);
		H_ASSERT (key != -1);
		m_Root = key;
		m_HMat = hmat;
	}

	HTK_Material_Composite (HMaterialLibrary *hmat) : TK_Material() {
		H_ASSERT (hmat != NULL);
		m_Root = -1;
		m_HMat = hmat;
	}

	TK_Status Interpret(BStreamFileToolkit &tk, ID_Key key, int variant);
	TK_Status Execute(BStreamFileToolkit & tk);

protected:
	HC_KEY m_Root;
	HMaterialLibrary *m_HMat;
};


class MVO_API HTK_One_Material : public BBaseOpcodeHandler
{
public:
	HTK_One_Material ();
	HTK_One_Material (HMaterialLibrary *hmat);
	~HTK_One_Material ();

	TK_Status Interpret(BStreamFileToolkit &tk, HMaterialLibrary *hmat, const char *name);
	TK_Status Execute(BStreamFileToolkit & tk);
	TK_Status Read (BStreamFileToolkit & tk);
	TK_Status Write (BStreamFileToolkit & tk);
	void      Reset ();

	/*! \enum Subop_Flags
		indicates what data the currently encoded format contains */
	enum Subop_Flags {
		OMF_Environment = 0x0001,
		OMF_Instancee	= 0x0002,
		OMF_Tweaks		= 0x0004,
		OMF_Embedded	= 0x0008,
		OMF_Height_Field= 0x0010,
	};
	/*! \enum Tweak_Type
		tweaks only come in sets of 1 to 4 floating point values for now, but that may change in the future */
	enum Tweak_Type {
		OMT_Float1 = 0x1,
		OMT_Float2 = 0x2,
		OMT_Float3 = 0x3,
		OMT_Float4 = 0x4,
	};

protected:
	HMaterialLibrary *m_HMat;			//not locally owned
	void *m_MaterialDescriptor;			//not locally owned
	char *m_Name;						
	int m_NameLength;
	char *m_Instancee;
	int m_InstanceeLength;
	int m_TweakCount;
	int *m_TweakNameLengths;	
	char **m_TweakNames;	
	char *m_TweakTypes;
	float **m_Tweaks;
	float m_HeightFieldScale;
	unsigned int m_subop;
	int m_substage;
	int m_substage2;

	//the rest of the variables are all used for embedding hsf data
#define ONEMATBUFSIZE 512
	void *m_subtk;
	FILE *m_HSF;
	int m_EmbeddedLength;
	unsigned char m_EmbeddingBuffer[ONEMATBUFSIZE];
	int m_EmbeddingBufferUsed;

	void Init ();
};





#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif
