// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#if _MSC_VER
#include <windows.h>
#else
#include <dlfcn.h>
#include <sys/types.h>
#include <dirent.h>
#include <map>
#endif

#include "hversion.h"

#include "HIOManager.h"
#include "HDB.h"
#include "HUtility.h"

#include "vhash.h"
#include "vlist.h"

#include "HIOUtilityHOOPS.h"
#include "HIOUtilityPDF.h"
#include "HIOUtilityVRML.h"
#include "HIOUtilityObj.h"
#include "HIOUtilityPly.h"
#include "HIOUtilityStl.h"
#include "HIOUtilityHsf.h"
#include "HIOUtilityHTML.h"
#ifdef USE_IMAGE_MAGICK
#include "HIOUtilityMagick.h"
#endif
#include "HIOUtilityIV.h"
#include "HIOUtilityPointCloud.h"

#include "HGlobals.h"
#include "HUtilityLocaleString.h"

#include <stdio.h>
#include <stdlib.h>

#include "utf_utils.h"

//Associates a library pointer and a corresponding handler pointer
struct lib_handler_pair {
	void * left;
	void * right;
};


#ifdef WINDOWS_SYSTEM
//Recursive function that gets all subdirectories, returns total # of dirs (including start)
static int find_subdirectories(char *start, char **results = 0, int count = 0)
{
	HANDLE find_handle;
	char find_spec[4096];
	WIN32_FIND_DATA find_data;
	BOOL find_result;

	if (start[strlen(start)-1] == '\\')
		sprintf (find_spec, "%s*", start);
	else
		sprintf (find_spec, "%s\\*", start);
#ifndef _WIN32_WCE
	find_handle = FindFirstFile(find_spec, &find_data);
#else
	find_handle = FindFirstFile((LPCWSTR)find_spec, &find_data);
#endif
	if (find_handle != INVALID_HANDLE_VALUE)
	{
		//if the current directory is a valid directory, then add it
		if(results)
		{
			results[count] = new char[MVO_BUFFER_SIZE];
			strcpy(results[count], start);
		}
		count++;

		//scan subdirectories
		while ((find_result = FindNextFile(find_handle, &find_data)) != 0) {
#ifndef _WIN32_WCE
			if(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY &&
				!streq(find_data.cFileName,".."))
#else
			if(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY &&
				!streq((const char *)(find_data.cFileName),".."))
#endif
			{
				char temp[MVO_BUFFER_SIZE];
				if (start[strlen(start)-1] == '\\')
					sprintf(temp, "%s%s\\", start, find_data.cFileName);
				else
					sprintf(temp, "%s\\%s\\", start, find_data.cFileName);
				count = find_subdirectories(temp, results, count);
			}
		} 
	}
	FindClose(find_handle);

	return count;
}
#endif


HIOManager::HIOManager():
m_input_types(0),
m_input_types_allocated(0),
m_output_types(0),
m_output_types_allocated(0)
{
	input_handlers = new_vhash(1,malloc,free);
	output_handlers = new_vhash(1,malloc,free);
	owned_input_handlers = new_vlist(malloc,free);
	owned_output_handlers = new_vlist(malloc,free);
	loaded_input_handlers = new_vlist(malloc,free);
	loaded_output_handlers = new_vlist(malloc,free);
	
	connector_procs = new_vhash(1,malloc,free);
	created_connectors = new_vhash(1,malloc,free);
}

HIOManager::~HIOManager()
{
	vhash_s * libraries_to_free = new_vhash(20, malloc,free);
	vlist_s * libraries_to_free_list = new_vlist(malloc,free);
	vlist_s * created_connectors_list = new_vlist(malloc,free);
	vlist_s * connector_procs_list = new_vlist(malloc,free);

	delete_vhash(input_handlers);
	delete_vhash(output_handlers);

	if(owned_input_handlers) {
		HInputHandler * ihandler;
		while((ihandler = (HInputHandler *) vlist_remove_first(owned_input_handlers)) != 0)
			delete ihandler;
		delete_vlist(owned_input_handlers);
	}

	if(owned_output_handlers) {
		HOutputHandler * ohandler;
		while((ohandler = (HOutputHandler *) vlist_remove_first(owned_output_handlers)) != 0)
			delete ohandler;
		delete_vlist(owned_output_handlers);
	}

	// Clean up the connector stuff here
	if(connector_procs) {
		vhash_string_keys_to_vlist(connector_procs, connector_procs_list, malloc);
		vhash_string_key_pair_t * connector_pair;
		while((connector_pair = (vhash_string_key_pair_t *) vlist_remove_first(connector_procs_list)) != 0) 
		{
			lib_handler_pair * pair = (lib_handler_pair *)connector_pair->item;
			delete pair;
			free(connector_pair);
		}

		delete_vhash(connector_procs);
		delete_vlist(connector_procs_list);
	}

	if(created_connectors) {
		vhash_to_vlist(created_connectors, created_connectors_list, malloc);
		vhash_pair_t * connector_pair;
		while((connector_pair = (vhash_pair_t *) vlist_remove_first(created_connectors_list)) != 0) 
		{
			HIO_FREE_CONNECTOR_FUNCTION Free = (HIO_FREE_CONNECTOR_FUNCTION) connector_pair->item;
			Free(connector_pair->key);
			free(connector_pair);
		}
		delete_vhash(created_connectors);
		delete_vlist(created_connectors_list);
	}

	//as we free these handlers, we build up a list of unique library handles so we can unload them last
	if(loaded_input_handlers) {
		lib_handler_pair *pair;
		while((pair = (lib_handler_pair *) vlist_remove_first(loaded_input_handlers)) != 0) 
		{
#ifdef WINDOWS_SYSTEM
#	ifndef _WIN32_WCE
			HIO_CREATE_FREE_FUNCTION Free = (HIO_CREATE_FREE_FUNCTION) GetProcAddress ((HINSTANCE)pair->left, "Free");
#	else
			HIO_CREATE_FREE_FUNCTION Free = (HIO_CREATE_FREE_FUNCTION) GetProcAddress ((HINSTANCE)pair->left, (LPCWSTR)"Free");
#	endif
#else
			HIO_CREATE_FREE_FUNCTION Free = (HIO_CREATE_FREE_FUNCTION) dlsym (pair->left, "Free");
#endif		
			if (Free)
				Free(pair->right);
			vhash_insert_item(libraries_to_free, pair->left, pair->left);
			delete pair;
		}
		delete_vlist(loaded_input_handlers);
	}

	if(loaded_output_handlers) {
		lib_handler_pair *pair;
		while((pair = (lib_handler_pair *) vlist_remove_first(loaded_output_handlers)) != 0) 
		{
#ifdef WINDOWS_SYSTEM
#	ifndef _WIN32_WCE
			HIO_CREATE_FREE_FUNCTION Free = (HIO_CREATE_FREE_FUNCTION) GetProcAddress ((HINSTANCE)pair->left, "Free");
#	else
			HIO_CREATE_FREE_FUNCTION Free = (HIO_CREATE_FREE_FUNCTION) GetProcAddress ((HINSTANCE)pair->left, (LPCWSTR)"Free");
#	endif
#else
			HIO_CREATE_FREE_FUNCTION Free = (HIO_CREATE_FREE_FUNCTION) dlsym (pair->left, "Free");
#endif
			Free(pair->right);
			vhash_insert_item(libraries_to_free, pair->left, pair->left);
			delete pair;
		}
		delete_vlist(loaded_output_handlers);
	}

	//now, convert to vlist, then iterate and unload the HIO libraries
	vhash_to_vlist(libraries_to_free, libraries_to_free_list, malloc);
	vhash_pair_t * lib_handle;
	while((lib_handle = (vhash_pair_t *) vlist_remove_first(libraries_to_free_list)) != 0){
#ifdef WINDOWS_SYSTEM
		FreeLibrary((HMODULE)lib_handle->item);
#else
		dlclose(lib_handle->item);
#endif
		free(lib_handle);
	}
	delete_vhash(libraries_to_free);
	delete_vlist(libraries_to_free_list);
	delete [] m_input_types;
	delete [] m_output_types;
}

HInputHandler * HIOManager::RegisterInputHandler(const char * file_type, HInputHandler * handler)
{

	if(!file_type)
		return nullptr;

	char ext_out[MVO_BUFFER_SIZE];
	HUtility::FindFileNameExtension(file_type, ext_out);
	
	HInputHandler * old_handler = (HInputHandler *) vhash_remove_string_key(input_handlers, ext_out);

	vhash_insert_string_key(input_handlers, ext_out, handler);

	delete [] m_input_types;
	m_input_types = 0;

	return old_handler;
}

HOutputHandler * HIOManager::RegisterOutputHandler(const char * file_type, HOutputHandler * handler)
{

	if(!file_type) return 0;

	char ext_out[MVO_BUFFER_SIZE];
	HUtility::FindFileNameExtension(file_type, ext_out);

	HOutputHandler * old_handler = (HOutputHandler *) vhash_remove_string_key(output_handlers, ext_out);

	vhash_insert_string_key(output_handlers, ext_out, handler);

	delete [] m_output_types;
	m_output_types = 0;

	return old_handler;
}

HIOConnector * HIOManager::RegisterConnector(const char * file_type, HIO_CREATE_FREE_FUNCTION Create, HIO_FREE_CONNECTOR_FUNCTION Free)
{

	if(!file_type) return 0;

	char ext_out[MVO_BUFFER_SIZE];
	HUtility::FindFileNameExtension(file_type, ext_out);
	
	lib_handler_pair * pair = (lib_handler_pair *) vhash_remove_string_key(connector_procs, ext_out);
	delete pair;

	pair = new lib_handler_pair();
	pair->left = (void *)Create;
	pair->right = (void *)Free;

	vhash_insert_string_key(connector_procs, ext_out, pair);

	return reinterpret_cast<HIOConnector *>(pair);
}

HInputHandler * HIOManager::GetInputHandler(const char * file_type)
{
	if(!file_type) return 0;

	char ext_out[MVO_BUFFER_SIZE];
	HUtility::FindFileNameExtension(file_type, ext_out);

	return static_cast<HInputHandler *>(vhash_lookup_string_key(input_handlers, ext_out));
}

#ifdef _MSC_VER
HInputHandler * HIOManager::GetInputHandler(const unsigned short * file_type){
	return GetInputHandler((wchar_t const *) file_type);
}
#endif

HInputHandler * HIOManager::GetInputHandler(const wchar_t * file_type)
{
	if(!file_type) return 0;

	wchar_t ext_out[MVO_BUFFER_SIZE];
	HUtility::FindFileNameExtension(file_type, ext_out);

	return GetInputHandler((char *) H_UTF8(ext_out).encodedText());
}

HOutputHandler * HIOManager::GetOutputHandler(const char * file_type)
{
	if(!file_type) return 0;

	char ext_out[MVO_BUFFER_SIZE];
	HUtility::FindFileNameExtension(file_type, ext_out);

	return static_cast<HOutputHandler *>(vhash_lookup_string_key(output_handlers, ext_out));
}

HOutputHandler * HIOManager::GetOutputHandler(const wchar_t * file_type)
{
	if(!file_type) return 0;

	wchar_t ext_out[MVO_BUFFER_SIZE];
	HUtility::FindFileNameExtension(file_type, ext_out);

	return GetOutputHandler((char*)H_UTF8(ext_out).encodedText());
}

void HIOManager::RecordOutputHandler (HOutputHandler * handler)
{
	vlist_add_first(owned_output_handlers, handler);
	handler->RegisterOutputHandlerTypes();
}

void HIOManager::RecordInputHandler (HInputHandler * handler)
{
	vlist_add_first(owned_input_handlers, handler);
	handler->RegisterInputHandlerTypes();
}

void HIOManager::RegisterHandlers()
{
	RecordInputHandler(new HIOUtilityHMF());
	RecordInputHandler(new HIOUtilityObj());
	RecordInputHandler(new HIOUtilityStl());
	RecordInputHandler(new HIOUtilityHsf());
	RecordInputHandler(new HIOUtilityPly());
	RecordInputHandler(new HIOUtilityPointCloud());

#ifndef _WIN32_WCE
	RecordInputHandler(new HIOUtilityIV());
	RecordOutputHandler(new HIOUtilityPDF());
	RecordInputHandler(new HIOUtilityVRML());
#endif

	RecordOutputHandler(new HIOUtilityHMF());
	RecordOutputHandler(new HIOUtilityPostscript());
	RecordOutputHandler(new HIOUtilityHPGL2());
	RecordOutputHandler(new HIOUtilityCGM());
	RecordOutputHandler(new HIOUtilityHsf());
	RecordOutputHandler(new HIOUtilityHTML());
	RecordOutputHandler(new HIOUtilityPly());
	RecordOutputHandler(new HIOUtilityObj());
	RecordOutputHandler(new HIOUtilityStl());

#ifdef USE_IMAGE_MAGICK
	RecordInputHandler(new HIOUtilityMagick());
	RecordOutputHandler(new HIOUtilityMagick());
#endif

	//scan directory
#ifdef WINDOWS_SYSTEM
#ifndef _WIN32_WCE
	//search path based off of: http://msdn.microsoft.com/library/default.asp?url=/library/en-us/dllproc/base/dynamic-link_library_search_order.asp

	int i = 0, str_size = 0, env_count = 0, subdir_count = 0;
	char *app_path = new char[MVO_BUFFER_SIZE];
	char *sys_path = new char[MVO_BUFFER_SIZE];
	char *win_path = new char[MVO_BUFFER_SIZE];
	char *cur_path = new char[MVO_BUFFER_SIZE];
	char *env_path;

	//use this to figure out Windows version
	OSVERSIONINFO win_info;
	win_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&win_info);

	GetModuleFileName(0, app_path, MVO_BUFFER_SIZE);			//application's directory
	char *char_ptr = strrchr(app_path, '\\');
	*char_ptr = 0;

	//scan HIO_Plugins directory for all subdirectories
	char hio_path[MVO_BUFFER_SIZE];
	sprintf(hio_path, "%s\\hio_plugins", app_path);
	subdir_count = find_subdirectories(hio_path);

	GetSystemDirectory(sys_path, MVO_BUFFER_SIZE);				//system directory	
	GetWindowsDirectory(win_path, MVO_BUFFER_SIZE);				//windows directory
	strcpy(cur_path, ".\\");									//current directory
	
	str_size = GetEnvironmentVariable("PATH", 0, 0);			//PATH directories
	env_path = new char[str_size + 1];
	GetEnvironmentVariable("PATH", env_path, str_size);

	//now populate array of all paths. the order is reversed so higher priorities (app_path) can
	//override lower priorities (cur_path, win_path, etc.)
	for(i = 0; i < str_size; i++)
		if(env_path[i] == ';')
			env_count++;
	if(env_path[0] != '\0')
		env_count++;
	
	char **directory_names = new char*[4+env_count+subdir_count];
	directory_names[0] = strtok(env_path, ";");
	for(i = 1; i < env_count; i++)
		directory_names[i] = strtok(0, ";");

	directory_names[i] = cur_path;
	directory_names[i+1] = win_path;
	directory_names[i+2] = sys_path;
	directory_names[i+3] = app_path;
	find_subdirectories(hio_path, directory_names, 4+env_count);


	for(i = 0; i < 4+env_count+subdir_count; i++)
	{
		HANDLE find_handle;
		char find_spec[4096];
		WIN32_FIND_DATA find_data;
		BOOL find_result;

		char *directory_name = directory_names[i];
		if(directory_name)
		{
			size_t dir_len = strlen(directory_name);
			if (directory_name[dir_len-1] == '\\')
				sprintf (find_spec, "%s*.*", directory_name);
			else
				sprintf (find_spec, "%s\\*.*", directory_name);

			find_handle = FindFirstFile(find_spec, &find_data);

			if (find_handle != INVALID_HANDLE_VALUE)
			{
				char extension[64];
#ifdef WIN64
#	ifdef _DEBUG
					sprintf(extension,"%d_x64d.hio", VISUALIZE_3DF_VERSION);
#	else
					sprintf(extension,"%d_x64.hio", VISUALIZE_3DF_VERSION);
#	endif
#else
#	ifdef _DEBUG
					sprintf(extension,"%dd.hio", VISUALIZE_3DF_VERSION);
#	else
					sprintf(extension,"%d.hio", VISUALIZE_3DF_VERSION);
#	endif
#endif

				while ((find_result = FindNextFile(find_handle, &find_data)) != 0) {
					if (strstr(find_data.cFileName, extension)) {
						bool used = false;
						void * handler = 0;

#if (_MSC_VER > 1200)
						typedef bool (CALLBACK *SETDLLPROC)(LPCTSTR);
						HINSTANCE kernel32 = LoadLibrary("KERNEL32.dll");
						if(kernel32)
						{
							SETDLLPROC SetDllDir = (SETDLLPROC) GetProcAddress (kernel32, "SetDllDirectoryA");

							if(SetDllDir)
								SetDllDir(directory_name);
							else
							{
								// If SetDllDirectory function does not exist, then add directory to path
								char new_path[MVO_BUFFER_SIZE];
								char old_path[MVO_BUFFER_SIZE];
								str_size = GetEnvironmentVariable("PATH",0,0);
								GetEnvironmentVariable("PATH", old_path, str_size);
								sprintf(new_path, "%s;%s", old_path, directory_name);
								SetEnvironmentVariable("PATH", new_path);			//PATH directories
							}
						}
#endif

						char * full_file_path = new char[dir_len + strlen(find_data.cFileName) + 3];
						if(directory_name[dir_len-1] == '\\')
							sprintf(full_file_path, "%s%s", directory_name, find_data.cFileName);
						else
							sprintf(full_file_path, "%s\\%s", directory_name, find_data.cFileName);
						// Suppress "program can't start because X.dll is missing from your computer" dialog
						UINT oldErrorMode = GetErrorMode();
						SetErrorMode(oldErrorMode | SEM_FAILCRITICALERRORS);
						void * library = LoadLibrary(full_file_path);
						SetErrorMode(oldErrorMode);
#ifdef _DEBUG
						if(!library){
							DWORD err = GetLastError();
							LPVOID lpMsgBuf;
							LPVOID lpDisplayBuf;

							FormatMessage(
								FORMAT_MESSAGE_ALLOCATE_BUFFER |
								FORMAT_MESSAGE_FROM_SYSTEM |
								FORMAT_MESSAGE_IGNORE_INSERTS,
								0,
								err,
								MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
								(LPTSTR)&lpMsgBuf,
								0, 0);

							lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
								(lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)"LoadLibrary")+1024)*sizeof(TCHAR)); 

							wsprintf((LPTSTR)lpDisplayBuf, 
								TEXT("%s failed loading %s\nThe error was: (%u) %s"), 
								"LoadLibrary", full_file_path, err, lpMsgBuf); 

							MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

							LocalFree(lpMsgBuf);
							LocalFree(lpDisplayBuf);
						}
#endif
						HIO_CREATE_FREE_FUNCTION Create;
						if((Create = (HIO_CREATE_FREE_FUNCTION) GetProcAddress ((HINSTANCE)library, "CreateInput")) != 0) {
							handler = Create(this);
							if(handler)
							{
								((HInputHandler *)handler)->SetModuleDirectory(directory_name);
								lib_handler_pair *temp_pair = new lib_handler_pair();
								temp_pair->left = library;
								temp_pair->right = handler;
								vlist_add_first(loaded_input_handlers, temp_pair);
								used = true;
							}
						}

						if((Create = (HIO_CREATE_FREE_FUNCTION) GetProcAddress ((HINSTANCE)library, "CreateOutput")) != 0) {
							handler = Create(this);
							if(handler)
							{
								((HOutputHandler *)handler)->SetModuleDirectory(directory_name); 
								lib_handler_pair *temp_pair = new lib_handler_pair();
								temp_pair->left = library;
								temp_pair->right = handler;
								vlist_add_first(loaded_output_handlers, temp_pair);
								used = true;
							}
						}

						//if handler is null, then loaded DLL is invalid, so unload
						if(!used)
							FreeLibrary((HMODULE) library);
						delete [] full_file_path;
					}

				} 
			}
			FindClose(find_handle);
		}
	}

	for(i = env_count; i < 4+env_count+subdir_count; i++)
		delete [] directory_names[i];

	delete [] env_path;
	delete [] directory_names;
#endif
#else
	/* Unix HIO search */
	std::map<std::string,bool> loaded_plugins;
	char * directories[4096]; 
	int num_directories = 0;
	directories[num_directories++] = new char [2];
	strcpy(directories[0],".");

#ifdef __APPLE__
	const char * linker_path = getenv("DYLD_LIBRARY_PATH");
#else
	const char * linker_path = getenv("LD_LIBRARY_PATH");
#endif
	if (linker_path != NULL)
	{
		char * begin = const_cast<char*>(linker_path);
		char * end = begin;
		bool do_break = false;
	    	do
		{
			if (*end == '\0' || *end == ':')
			{
				int len = end-begin;
				char * dir = new char [len+1];
				memcpy(dir,begin, len * sizeof(char));
				dir[len] = '\0';
				directories[num_directories++] = dir;
				begin = end + 1;
				do_break = *end == '\0';
			}
			end++;
		}
		while(!do_break);
	}

	for (int i =0; i < num_directories; i++)
	{
		DIR	*dir = opendir (directories[i]);
		struct dirent	*entry;
	
		if (dir) 
		{
			char extension[64];

#ifdef _DEBUG
			sprintf(extension,"%dd.hio", VISUALIZE_3DF_VERSION);
#else
			sprintf(extension,"%d.hio", VISUALIZE_3DF_VERSION);
#endif

            size_t dir_len = strlen(directories[i]);
			while ((entry = readdir(dir))) {
				if (strstr(entry->d_name, extension) && loaded_plugins.find(entry->d_name) == loaded_plugins.end()) {
					char * tmp = (char*)malloc(dir_len + strlen(entry->d_name) + 3); 
					if (!tmp) {
						continue;
					}
                    if(directories[i][dir_len-1] == '/' || directories[i][dir_len-1] == '\\')
                        sprintf(tmp, "%s%s", directories[i], entry->d_name);
                    else
                        sprintf(tmp, "%s/%s", directories[i], entry->d_name);
	
					//std::cout << entry->d_name << st::endl;
					HIO_CREATE_FREE_FUNCTION Create;
					bool used = false;
					void * handler = 0;
					void * library = dlopen(tmp, RTLD_NOW);
					if (library == 0) {
						fprintf(stderr, "%s\n", dlerror());
						free(tmp);
						continue;
					}

					if(Create = (HIO_CREATE_FREE_FUNCTION) dlsym(library, "CreateInput"))
					{
						handler = Create(this);
						if(handler)
						{
							lib_handler_pair *temp_pair = new lib_handler_pair();
							temp_pair->left = library;
							temp_pair->right = handler;
							vlist_add_first(loaded_input_handlers, temp_pair);
							used = true;
						}
					}
					else
						printf("dlsym (CreateInput):\n%s\n\n", dlerror());

					if(Create = (HIO_CREATE_FREE_FUNCTION) dlsym(library, "CreateOutput"))
					{
						handler = Create(this);
						if(handler)
						{
							lib_handler_pair *temp_pair = new lib_handler_pair();
							temp_pair->left = library;
							temp_pair->right = handler;
							vlist_add_first(loaded_output_handlers, temp_pair);
							used = true;
						}
					}
					else
						printf("dlsym (CreateOutput):\n%s\n\n", dlerror());

					if(!used)
						dlclose(library);
					else
						loaded_plugins[entry->d_name] = true;

					free(tmp);
				}
			}
		}
		closedir(dir);
	}

	for (int i =0; i < num_directories; i++)
		delete [] directories[i];
#endif
}

HIOManager *HIOManager::GetCurrentHIOManager()
{
	return HDB::GetHIOManager();
}

const char * HIOManager::GetInputHandlerTypes()
{ 
	if(!m_input_types) {
		m_input_types_allocated = 100;
		m_input_types = new char[m_input_types_allocated];
		strcpy(m_input_types, ";");

		vlist_t * handlers = new_vlist(malloc, free);
		vhash_string_keys_to_vlist(input_handlers, handlers, malloc);
		
		START_LIST_ITERATION(vhash_string_key_pair_t, handlers);
			if(strlen(temp->key) +3 > (m_input_types_allocated - strlen(m_input_types) -1)) {
				m_input_types_allocated += (int)strlen(temp->key) + 100;
				char * tmp = new char[m_input_types_allocated];
				strcpy(tmp, m_input_types);
				delete [] m_input_types;
				m_input_types = tmp;
			}
			strcat(m_input_types, H_FORMAT_TEXT("%s;",temp->key));
			free(temp);
		END_LIST_ITERATION(handlers);

		delete_vlist(handlers);
	}

	return m_input_types; 
}

const char * HIOManager::GetOutputHandlerTypes()
{ 
	if(!m_output_types) {
		m_output_types_allocated = 100;
		m_output_types = new char[m_output_types_allocated];
		strcpy(m_output_types, ";");

		vlist_t * handlers = new_vlist(malloc, free);
		vhash_string_keys_to_vlist(output_handlers, handlers, malloc);

		START_LIST_ITERATION(vhash_string_key_pair_t, handlers);
			if(strlen(temp->key) +3 > (m_output_types_allocated - strlen(m_output_types) -1)) {
				m_output_types_allocated += (int)strlen(temp->key) + 100;
				char * tmp = new char[m_output_types_allocated];
				strcpy(tmp, m_output_types);
				delete [] m_output_types;
				m_output_types = tmp;
			}
			strcat(m_output_types, H_FORMAT_TEXT("%s;",temp->key));
			free(temp);
		END_LIST_ITERATION(handlers);

		delete_vlist(handlers);
	}

	return m_output_types; 
}


HIOConnector * HIOManager::CreateConnector(const char * file_type)
{
	if(!file_type) return 0;

	char ext_out[MVO_BUFFER_SIZE];
	HUtility::FindFileNameExtension(file_type, ext_out);

	//For a given extension, call the appropriate "CreateConnector", and associate the newly
	//created connector address with the appropriate "FreeConnector"

	lib_handler_pair *pair = (lib_handler_pair *) vhash_lookup_string_key(connector_procs, ext_out);

	if(pair)
	{
		HIO_CREATE_FREE_FUNCTION create = (HIO_CREATE_FREE_FUNCTION) pair->left;
		if(create)
		{
			HIOConnector *new_connector = (HIOConnector *) create(0);
			vhash_insert_item(created_connectors, new_connector, pair->right);
			return new_connector;
		}
		else
			return 0;
	}
	else
		return 0;
}


void HIOManager::FreeConnector(HIOConnector * old_connector)
{
	if(!old_connector) return;

	//Given the old HIOConnector, we scan the created_connectors vhash to find the corresponding 
	//"FreeConnector"
	HIO_FREE_CONNECTOR_FUNCTION Free = (HIO_FREE_CONNECTOR_FUNCTION) vhash_lookup(created_connectors, old_connector);

	if(Free)
	{
		Free(old_connector);
		vhash_remove(created_connectors, old_connector);
	}
}

void HIOManager::GetFileOutputResultString(HFileOutputResult result, char *result_str)
{
	switch(result){
		case OutputOK:
			sprintf(result_str, "%s", "OutputOK");
			break;
		case OutputVersionMismatch:
			sprintf(result_str, "%s", "OutputVersionMismatch");
			break;
		case OutputFail:
			sprintf(result_str, "%s", "OutputFail");
			break;
		case OutputNotHandled:
			sprintf(result_str, "%s", "OutputNotHandled");
			break;
		case OutputBadFileName:
			sprintf(result_str, "%s", "OutputBadFileName");
			break;
		case OutputBadOptions:
			sprintf(result_str, "%s", "OutputBadOptions");
			break;
		case OutputBadLicense:
			sprintf(result_str, "%s", "OutputBadLicense");
			break;
		default:
			sprintf(result_str, "%s", "Unknown error");
			break;
	}
}


HIOConnector::HIOConnector() 
{
	mhash_Hoops2Kernel = new_vhash( 4086, malloc, free );
	mhash_Kernel2Hoops = new_vhash( 4086, malloc, free );

}

HIOConnector::~HIOConnector()
{
	if (mhash_Hoops2Kernel)
		delete_vhash(mhash_Hoops2Kernel);
	if (mhash_Kernel2Hoops)
		delete_vhash(mhash_Kernel2Hoops);
}

HC_KEY HIOConnector::GetHoopsEntity(void * pKernelEntity)
{
	return GetHoopsEntity(pKernelEntity, 0);
}

HC_KEY HIOConnector::GetHoopsEntity(void * pKernelEntity, HC_KEY modelKey)
{
	UNREFERENCED(modelKey);

	void * item = 0;
	HC_KEY key = INVALID_KEY;
	vhash_lookup_item(mhash_Kernel2Hoops, pKernelEntity, &item);
	key = MVO_V2I(item);
	return key;
}

void * HIOConnector::GetKernelEntity(HC_KEY key)
{
	return GetKernelEntity(key, 0);
}

void * HIOConnector::GetKernelEntity(HC_KEY key, HC_KEY modelKey)
{
	UNREFERENCED(modelKey);

	void * pKernelEntity = 0;
	vhash_lookup_item(mhash_Hoops2Kernel, MVO_I2V(key), &pKernelEntity);
	return pKernelEntity;
}

bool HIOConnector::GetHoopsEntities(void * pKernelEntity, vlist_s *ret_HoopsKeysList)
{
	return GetHoopsEntities(pKernelEntity, ret_HoopsKeysList, 0);
}

bool HIOConnector::GetHoopsEntities(void * pKernelEntity, vlist_s *ret_HoopsKeysList, HC_KEY modelKey)
{
	UNREFERENCED(modelKey);

	if(!ret_HoopsKeysList)
		return false;

	void* key;
	int n = 0;
	while( VHASH_STATUS_SUCCESS == vhash_lookup_nth_item(mhash_Kernel2Hoops, pKernelEntity, n++, &key) )
		vlist_add_last(ret_HoopsKeysList, key);

	return true;
}

bool HIOConnector::GetKernelEntities(HC_KEY key, vlist_s *ret_KernelEntitiesList)
{
	return GetKernelEntities(key, ret_KernelEntitiesList, 0);
}

bool HIOConnector::GetKernelEntities(HC_KEY key, vlist_s *ret_KernelEntitiesList, HC_KEY modelKey)
{
	UNREFERENCED(modelKey);

	if(!ret_KernelEntitiesList)
		return false;

	void * pKernelEntity = 0;
	int n = 0;
	while( VHASH_STATUS_SUCCESS == vhash_lookup_nth_item(mhash_Hoops2Kernel, MVO_I2V(key), n++, &pKernelEntity) )
		vlist_add_last(ret_KernelEntitiesList, pKernelEntity);

	return true;
}

void HIOConnector::AddConnection(HC_KEY key, void* pKernelEntity)
{
	return AddConnection(key, pKernelEntity, 0);
}

void HIOConnector::AddConnection(HC_KEY key, void* pKernelEntity, HC_KEY modelKey)
{
	UNREFERENCED(modelKey);

	vhash_insert_item( mhash_Hoops2Kernel, MVO_I2V(key), pKernelEntity );
	vhash_insert_item( mhash_Kernel2Hoops, pKernelEntity, MVO_I2V(key) );
}

void HIOConnector::OptimizeTree(const char * segname, const char * option_string)
{
	HC_Optimize_Segment_Tree( segname, option_string); 

	HC_KEY  in_owner  = INVALID_KEY;
	int		in_region = 0;

	HC_KEY  out_owner  = INVALID_KEY;
	HC_KEY  out_key	 = INVALID_KEY;
	int		out_region = 0;

	struct vhash_s *updated_hash_Hoops2Kernel	= new_vhash( 4086, malloc, free );
	struct vhash_s *updated_hash_Kernel2Hoops	= new_vhash( 4086, malloc, free );

	// update the HOOPS to Kernel mapping
	vlist_s *list_Hoops2Kernel					= new_vlist(malloc,free);
	vhash_to_vlist(mhash_Hoops2Kernel, list_Hoops2Kernel, malloc);
	START_LIST_ITERATION(vhash_pair_t, list_Hoops2Kernel);

		HC_Show_Optimized_Mapping ( ".", "forward", in_owner, MVO_V2I(temp->key),  in_region,
												    &out_owner, &out_key, &out_region ); 
		vhash_insert_item( updated_hash_Hoops2Kernel, MVO_I2V(out_key), temp->item );

	END_LIST_ITERATION(list_Hoops2Kernel);

	while(vhash_pair_t * pair = (vhash_pair_t *) vlist_remove_first(list_Hoops2Kernel)) 
		delete pair;

	delete_vlist(list_Hoops2Kernel);
	delete_vhash(mhash_Hoops2Kernel);

	// update the Kernel to HOOPS mapping
	vlist_s *list_Kernel2Hoops					= new_vlist(malloc,free);
	vhash_to_vlist(mhash_Kernel2Hoops, list_Kernel2Hoops, malloc);
	START_LIST_ITERATION(vhash_pair_t, list_Kernel2Hoops);

		HC_Show_Optimized_Mapping ( ".", "forward", in_owner, MVO_V2I(temp->item),  in_region,
									&out_owner, &out_key, &out_region ); 
		vhash_insert_item( updated_hash_Kernel2Hoops, temp->key, MVO_I2V(out_key));

	END_LIST_ITERATION(list_Kernel2Hoops);

	while(vhash_pair_t * pair = (vhash_pair_t *) vlist_remove_first(list_Kernel2Hoops)) 
		delete pair;

	delete_vlist(list_Kernel2Hoops);
	delete_vhash(mhash_Kernel2Hoops);

	mhash_Hoops2Kernel = updated_hash_Hoops2Kernel;
	mhash_Kernel2Hoops = updated_hash_Kernel2Hoops;

}

HInputHandler::HInputHandler() {
	m_InputOps = HInputOpNone;
	m_progress = 0;
	m_percent_progress1 = 0.0;
	m_percent_progress2 = -1.0;
	m_information = 0;
	m_information_list = new_vlist(malloc,free);
	m_progress_callback = 0;
	m_percent_progress_callback = 0;
	m_information_callback = 0;
	m_module_directory = 0;
}

HInputHandler::~HInputHandler() {
	delete [] m_progress;
	delete [] m_information;
	if (m_information_list) {
		while ((m_information = (wchar_t*)vlist_remove_first(m_information_list)) != 0)
			delete [] m_information;
		delete_vlist(m_information_list);
	}
	delete [] m_module_directory;
	// m_progress_callback and m_information_callback and m_percent_progress_callback must be cleaned up by the app 
}

HFileInputResult HInputHandler::FileInputByKey(const char * filename, HC_KEY key, HInputHandlerOptions * options)
{
	return FileInputByKey((wchar_t const*)H_WCS(filename).encodedText(), key, options);
}

HFileInputResult HInputHandler::FileInputByKey(const wchar_t * filename, HC_KEY key, HInputHandlerOptions * options)
{
	UNREFERENCED(filename);
	UNREFERENCED(key);
	UNREFERENCED(options);

	return InputNotHandled;
}

HFileInputResult HInputHandler::FileInputToImageKey(const char * filename, HC_KEY  *image_key, HC_KEY segment_key, HInputHandlerOptions * options)
{
	return FileInputToImageKey((wchar_t const*)H_WCS(filename).encodedText(), image_key, segment_key, options);
}

HFileInputResult HInputHandler::FileInputToImageKey(const wchar_t * filename, HC_KEY  *image_key, HC_KEY segment_key, HInputHandlerOptions * options)
{
	UNREFERENCED(filename);
	UNREFERENCED(image_key);
	UNREFERENCED(segment_key);
	UNREFERENCED(options);

	return InputNotHandled;
}

void HInputHandler::SetModuleDirectory(const char *module_directory)
{
	SetModuleDirectory((wchar_t const*)H_WCS(module_directory).encodedText());
}

void HInputHandler::SetModuleDirectory(const wchar_t *module_directory)
{
	delete [] m_module_directory;
	m_module_directory = new wchar_t[wcslen(module_directory) + 1];
	wcscpy(m_module_directory, module_directory);
}

HFileInputResult HInputHandler::FileInputByKey(const unsigned short * filename, HC_KEY key, HInputHandlerOptions * options){
	H_UTF16 utf16;
	utf16.encodedText((utf16_char const*)filename);
	return FileInputByKey((wchar_t const*)H_WCS(utf16).encodedText(), key, options);;
}

HFileInputResult HInputHandler::FileInputToImageKey(const unsigned short * filename, HC_KEY  *image_key, HC_KEY segment_key, HInputHandlerOptions * options){
	H_UTF16 utf16;
	utf16.encodedText((utf16_char const*)filename);

	return FileInputToImageKey((wchar_t const*)H_WCS(utf16).encodedText(), image_key, segment_key, options);
}

const wchar_t * HInputHandler::GetInputProgress()
{
	return m_progress;
}

float HInputHandler::GetInputPercentProgress()
{
	return m_percent_progress1;
}

const wchar_t * HInputHandler::GetInputInformation()
{
	delete [] m_information;
	m_information = (wchar_t *)vlist_remove_first(m_information_list);
	return m_information;
}

HIONoticeCallback * HInputHandler::SetInputProgressNoticeCallback(HIONoticeCallback *notice_callback)
{
	HIONoticeCallback *old_notice_callback = m_progress_callback;
	m_progress_callback = notice_callback;
	return old_notice_callback;
}

HIOPercentNoticeCallback * HInputHandler::SetInputPercentProgressNoticeCallback(HIOPercentNoticeCallback *percent_notice_callback)
{
	HIOPercentNoticeCallback *old_notice_callback = m_percent_progress_callback;
	m_percent_progress_callback = percent_notice_callback;
	return old_notice_callback;
}

HIONoticeCallback * HInputHandler::SetInputInformationNoticeCallback(HIONoticeCallback *notice_callback)
{
	HIONoticeCallback *old_notice_callback = m_information_callback;
	m_information_callback = notice_callback;
	return old_notice_callback;
}

void HInputHandler::SetStartingInput(const char *filename)
{
	SetStartingInput((wchar_t const*)H_WCS(filename).encodedText());
}

void HInputHandler::SetStartingInput(const wchar_t *filename)
{
	if (filename)
		ReportInputProgress(filename);
	ReportInputProgress("starting");
	ReportInputPercentProgress(0.0f);
	delete[]m_information;	
	while ((m_information = (wchar_t *)vlist_remove_first(m_information_list)) != 0)
		delete[]m_information;
}

void HInputHandler::SetFinishedInput()
{
	ReportInputPercentProgress(1.0f);
	ReportInputProgress("finished");
}

void HInputHandler::ReportInputProgress(const char * progress)
{
	ReportInputProgress((wchar_t const*)H_WCS(progress).encodedText());
}

void HInputHandler::ReportInputProgress(const wchar_t * progress)
{
	delete [] m_progress;
	if (!progress) {
		m_progress = nullptr;
		return;
	}
	m_progress = new wchar_t[wcslen(progress) + 1];
	wcscpy(m_progress, progress);
	if (m_progress_callback)
		m_progress_callback->Notice(m_progress);
}

void HInputHandler::ReportInputPercentProgress(float progress)
{
	if (progress < 0.0f)
		progress = 0.0f;

	if (progress > 1.0f)
		progress = 1.0f;

	if (m_percent_progress1 > progress) {
		m_percent_progress2 = -1;
	}

	m_percent_progress1 = progress;

	if (m_percent_progress1 - m_percent_progress2 >= 0.01) {
		if (m_percent_progress_callback)
			m_percent_progress_callback->Notice(m_percent_progress1);
		m_percent_progress2 = m_percent_progress1;
	}
}

void HInputHandler::ReportInputInformation(const char * information)
{
	ReportInputInformation((wchar_t const*)H_WCS(information).encodedText());
}
void HInputHandler::ReportInputInformation(const wchar_t * information)
{
	if (!information)
		return;
	wchar_t * tmp_information = new wchar_t[wcslen(information)+1];
	wcscpy(tmp_information, information);
	vlist_add_last(m_information_list, tmp_information);
	if (m_information_callback)
		m_information_callback->Notice(tmp_information);
}



HOutputHandler::HOutputHandler() {
	m_OutputOps = HOutputOpNone;
	m_progress = nullptr;
	m_percent_progress1 = 0.0f;
	m_percent_progress2 = -1.0f;
	m_information = nullptr;
	m_information_list = new_vlist(malloc,free);
	m_progress_callback = nullptr;
	m_percent_progress_callback = nullptr;
	m_information_callback = nullptr;
	m_module_directory = nullptr;
}

HOutputHandler::~HOutputHandler() {
	delete [] m_progress;
	delete [] m_information;
	if (m_information_list) {
		while ((m_information = (wchar_t *)vlist_remove_first(m_information_list)) != 0)
			delete [] m_information;
		delete_vlist(m_information_list);
	}
	delete [] m_module_directory;

	// m_progress_callback and m_information_callback and m_percent_progress_callback must be cleaned up by the app 
}

HFileOutputResult HOutputHandler::FileOutputByKey(const char * filename, HC_KEY key, HOutputHandlerOptions * options)
{
	return FileOutputByKey((wchar_t const*)H_WCS(filename).encodedText(), key, options);
}

HFileOutputResult HOutputHandler::FileOutputByKey(const wchar_t * filename, HC_KEY key, HOutputHandlerOptions * options)
{
	UNREFERENCED(filename);
	UNREFERENCED(key);
	UNREFERENCED(options);

	return OutputNotHandled;
}

HFileOutputResult HOutputHandler::FileOutputByKey(const unsigned short * filename, HC_KEY key, HOutputHandlerOptions * options)
{
	H_UTF16 utf16;
	utf16.encodedText((utf16_char const*) filename);
	return FileOutputByKey((wchar_t const*)H_WCS(utf16).encodedText(), key, options);
}

HFileOutputResult HOutputHandler::FileOutputFromImageKey(const char * filename, HC_KEY image_key, HOutputHandlerOptions * options)
{
	return FileOutputFromImageKey((wchar_t const*)H_WCS(filename).encodedText(), image_key, options);
}

HFileOutputResult HOutputHandler::FileOutputFromImageKey(const wchar_t * filename, HC_KEY image_key, HOutputHandlerOptions * options)
{
	UNREFERENCED(filename);
	UNREFERENCED(image_key);
	UNREFERENCED(options);

	return OutputNotHandled;
}

const char * HOutputHandler::GetOutputDefaultHSRA() 
{
	return 0;
}


const char * HOutputHandler::GetOutputProgress()
{
	return m_progress;
}

float HOutputHandler::GetOutputPercentProgress()
{
	return m_percent_progress1;
}


void HOutputHandler::SetModuleDirectory(const char *module_directory)
{
	delete [] m_module_directory;
	m_module_directory = new char[strlen(module_directory) + 1];
	strcpy(m_module_directory, module_directory);
}

void HOutputHandler::SetModuleDirectory(const wchar_t *module_directory)
{
	SetModuleDirectory(H_ASCII_TEXT(module_directory));
}

const wchar_t * HOutputHandler::GetOutputInformation()
{
	delete [] m_information;
	m_information = (wchar_t *)vlist_remove_first(m_information_list);
	return m_information;
}

HIONoticeCallback * HOutputHandler::SetOutputProgressNoticeCallback(HIONoticeCallback *notice_callback)
{
	HIONoticeCallback *old_notice_callback = m_progress_callback;
	m_progress_callback = notice_callback;
	return old_notice_callback;
}
HIOPercentNoticeCallback * HOutputHandler::SetOutputPercentProgressNoticeCallback(HIOPercentNoticeCallback *notice_callback)
{
	HIOPercentNoticeCallback *old_notice_callback = m_percent_progress_callback;
	m_percent_progress_callback = notice_callback;
	return old_notice_callback;
}
HIONoticeCallback * HOutputHandler::SetOutputInformationNoticeCallback(HIONoticeCallback *notice_callback)
{
	HIONoticeCallback *old_notice_callback = m_information_callback;
	m_information_callback = notice_callback;
	return old_notice_callback;
}

void HOutputHandler::SetStartingOutput()
{
	ReportOutputProgress("starting");
	ReportOutputPercentProgress(0.0f);
	delete [] m_information;	
	while ((m_information = (wchar_t *)vlist_remove_first(m_information_list)) != 0) {
		continue;
	}
	delete [] m_information;
}

void HOutputHandler::SetFinishedOutput()
{
	ReportOutputProgress("finished");
	ReportOutputPercentProgress(1.0f);
}

void HOutputHandler::ReportOutputProgress(const char * progress)
{
	delete [] m_progress;
	if (!progress) {
		m_progress = nullptr;
		return;
	}
	m_progress = new char[strlen(progress) + 1];
	sprintf(m_progress, "%s", progress);
	if (m_progress_callback)
		m_progress_callback->Notice(m_progress);
}

void HOutputHandler::ReportOutputPercentProgress(float progress)
{
	if (progress < 0.0f)
		progress = 0.0f;

	if (progress > 1.0f)
		progress = 1.0f;

	if (m_percent_progress1 > progress) {
		m_percent_progress2 = -1;
	}

	m_percent_progress1 = progress;

	if (m_percent_progress1 - m_percent_progress2 >= 0.01) {
		if (m_percent_progress_callback)
			m_percent_progress_callback->Notice(m_percent_progress1);
		m_percent_progress2 = m_percent_progress1;
	}
}

void HOutputHandler::ReportOutputInformation(const char * information)
{
	if (!information)
		return;

	size_t len = strlen(information);

	wchar_t * tmp_information = new wchar_t[len + 1];
	for (size_t i = 0; i < len; ++i)
		tmp_information[i] = information[i];
	tmp_information[len] = L'\0';

	vlist_add_last(m_information_list, tmp_information);
	if (m_information_callback)
		m_information_callback->Notice(tmp_information);
}



HIOPercentNoticeCallback::~HIOPercentNoticeCallback() {}

HInputHandlerOptions::~HInputHandlerOptions() {}

HOutputHandlerOptions::HOutputHandlerOptions() {
	m_Key = INVALID_KEY;
	m_pHBaseView = 0;
	m_pHBaseModel = 0;
	m_pCamera = 0;
	m_pExtendedData = 0;
	m_pHSRAOptions = 0;
	m_pWindowColor = 0;
	m_Window_Width = 0;
	m_Window_Height = 0;
	m_Paper_Width = 8.5;
	m_Paper_Height = 11;
	m_Image_Dpi = 100;
	m_Paper_Dpi = 300;
	m_Version = 0;
	m_bUseWindowColor = false;
	m_ForceGrayscale = false;
	m_bFastPrint = false;
	m_bConserveMemory = false;
	m_bUseSubscreen = true;
	m_bWindowFrame = false;
	m_b3dOutput = false;
	m_bAscii = false;
	m_bStlNormals = false;
	m_bWriteAnimations = false;
	m_PageMargins[0] = m_PageMargins[1] = m_PageMargins[2] = m_PageMargins[3] = 0;
	m_pBehaviorManager = 0;
	m_pDWFModelName = 0;
	m_pPDFResourceDirectory = NULL;
	m_OutputType = H_OUTPUT_TYPE_DEFAULT;
	m_license = 0;
	m_pPRCAsmModelFile = 0;	

	m_antialiasLevel = 1;
	m_jpeg_quality = 0;
	m_imageOutputBuffer = NULL;
	m_imageOutputBufferSize = 0;

	m_bActivateOnPageOpen = false;
	m_fAnnotLeft = 0.5;
	m_fAnnotRight = 0.5;
	m_fAnnotBottom = 2;
	m_fAnnotTop = 0.5;
	m_ratio_cm_to_internal_units = 0.0;

	m_ePrcBrepCompression = PrcBrepCompressionNone;
	m_bPrcRemoveBRep = false;
	m_bPrcRemoveAttributes = false;
	m_bPrcCompressTessellation = false;
	m_bPrcUseNameUserData = false;
	m_iPrcNameUserDataIndex = 0;

	m_bAVIShowCompressionDialog = true;
	m_bAVICodec[0] = m_bAVICodec[1] = m_bAVICodec[2] = m_bAVICodec[3] = '\0';

	m_bExporting2DDwf = false;
	m_lineFuseThreshold = 100;
}

void HOutputHandlerOptions::Init(HOutputHandlerOptions const *o)
{	
	m_pHBaseView = o->m_pHBaseView;
	m_pHBaseModel = o->m_pHBaseModel;
	m_pCamera = o->m_pCamera;
	m_Key = INVALID_KEY;
	m_pHSRAOptions = o->m_pHSRAOptions;
	m_bUseWindowColor = o->m_bUseWindowColor;
	m_pWindowColor = o->m_pWindowColor;
	m_Window_Width = o->m_Window_Width;
	m_Window_Height = o->m_Window_Height;
	m_Paper_Width = o->m_Paper_Width;
	m_Paper_Height = o->m_Paper_Height;
	m_ForceGrayscale = o->m_ForceGrayscale;
	m_bFastPrint = o->m_bFastPrint;
	m_bConserveMemory = o->m_bConserveMemory;
	m_Image_Dpi = o->m_Image_Dpi;
	m_Paper_Dpi = o->m_Paper_Dpi;
	m_pExtendedData = o->m_pExtendedData;
	m_Version = o->m_Version;
	m_bUseSubscreen = o->m_bUseSubscreen;
	m_bWindowFrame = o->m_bWindowFrame;
	m_b3dOutput = o->m_b3dOutput;
	m_bAscii = o->m_bAscii;
	m_bStlNormals = o->m_bStlNormals;
	m_bWriteAnimations = o->m_bWriteAnimations;
	m_pBehaviorManager = o->m_pBehaviorManager;
	m_pDWFModelName = o->m_pDWFModelName;
	m_OutputType = o->m_OutputType;
	m_license = o->m_license;
	m_pPRCAsmModelFile = o->m_pPRCAsmModelFile;
	m_antialiasLevel = o->m_antialiasLevel;
	m_jpeg_quality = o->m_jpeg_quality;
	m_imageOutputBuffer = o->m_imageOutputBuffer;
	m_imageOutputBufferSize = o->m_imageOutputBufferSize;

	m_bActivateOnPageOpen = o->m_bActivateOnPageOpen;
	m_fAnnotLeft = o->m_fAnnotLeft;
	m_fAnnotRight = o->m_fAnnotRight;
	m_fAnnotBottom = o->m_fAnnotBottom;
	m_fAnnotTop = o->m_fAnnotTop;
	m_ratio_cm_to_internal_units = o->m_ratio_cm_to_internal_units;
	m_ePrcBrepCompression = o->m_ePrcBrepCompression;
	m_bPrcCompressTessellation = o->m_bPrcCompressTessellation;
	m_bPrcRemoveBRep = o->m_bPrcRemoveBRep;
	m_bPrcRemoveAttributes = o->m_bPrcRemoveAttributes;
	m_bPrcUseNameUserData = o->m_bPrcUseNameUserData;
	m_iPrcNameUserDataIndex = o->m_iPrcNameUserDataIndex;

	m_bAVIShowCompressionDialog = o->m_bAVIShowCompressionDialog;
	m_bExporting2DDwf = o->m_bExporting2DDwf;
	m_lineFuseThreshold = o->m_lineFuseThreshold;
}


HOutputHandlerOptions::~HOutputHandlerOptions() {}

bool HOutputHandlerOptions::PopulateOptions()
{
	return true;
}

char const * HOutputHandlerOptions::GetType() const
{
	return "HOutputHandlerOptions";
}

#include <map>

typedef std::map<HC_KEY, H_FORMAT_TEXT> KEY_NAME_MAP;

ExportNameMap::ExportNameMap()
	: _imp(new KEY_NAME_MAP())
{}

ExportNameMap::~ExportNameMap()
{
	KEY_NAME_MAP * knm = (KEY_NAME_MAP*)_imp;
	delete knm;
}

H_FORMAT_TEXT ExportNameMap::lookup(HC_KEY key) const
{
	KEY_NAME_MAP * knm = (KEY_NAME_MAP*)_imp;
	KEY_NAME_MAP::iterator iter = knm->find(key);
	if(iter != knm->end()){
		return iter->second;
	}
	return "not found";
}

void ExportNameMap::tag(HC_KEY key, H_FORMAT_TEXT const & name)
{
	KEY_NAME_MAP * knm = (KEY_NAME_MAP*)_imp;
	knm->insert(std::make_pair(key, name));
}
