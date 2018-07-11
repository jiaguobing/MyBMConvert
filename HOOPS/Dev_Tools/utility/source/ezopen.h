

#ifndef __ezopen__h__
#define __ezopen__h__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#if defined(UNIX_SYSTEM) || defined(OSX_SYSTEM)
# include <unistd.h>
#endif
#ifdef _MSC_VER
# include <assert.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <windows.h>
# include <assert.h>
# include <io.h>
# define open _open
#endif

#ifndef _MAX_DIR
# define _MAX_DIR 512
#endif

#ifndef BUFSIZE
# define BUFSIZE 4096
#endif

#ifdef _MSC_VER
# define _CRT_SECURE_NO_WARNINGS
#  pragma warning (disable: 4996)
#endif

static
int save_backup_copy(char const * utf8, char const * mode)
{
	(void)mode;
	int retval = -1;
	FILE * in = 0, * out = 0;
	size_t len, c;
	char * bfn = 0;
	char buf[BUFSIZE];

	len = strlen(utf8);
	bfn = (char*)malloc(len + 2);
	if(!bfn)
		goto DONE;

	strcpy(bfn, utf8);
	strcat(bfn, "~");
	in = fopen(utf8, "rb");
	if(!in){
		retval = 0;
		goto DONE;
	}

	out = fopen(bfn, "wb");

	if(!out)
		goto DONE;

	c = fread(buf, 1, BUFSIZE, in);
	if(c < 0)
		goto DONE;

	while(c > 0){
		size_t rc = fwrite(buf, 1, BUFSIZE, out);
		if(rc != c)
			goto DONE;
		c = fread(buf, 1, BUFSIZE, in);
		if(c < 0)
			goto DONE;
	}

	retval = 0;
DONE:
	if(in)
		fclose(in);
	if(out)
		fclose(out);
	if(bfn)
		free(bfn);
	return retval;
}


#ifdef _MSC_VER
static
void GenerateTempFileName ( char * tempFileNameOut, char const * extension ) {
		char temp_dir[_MAX_DIR];
		long dir_len = 0;
		unsigned int res;

		dir_len = GetTempPathA(_MAX_DIR, temp_dir);
		assert(dir_len != 0);
		assert(dir_len <= _MAX_DIR);
		res = GetTempFileNameA(temp_dir, "HOOPS", 0, tempFileNameOut);
		assert(res != 0);
		// if extension specified, replace .tmp with user-provided value
		if (extension) {
				char *old_extension = strrchr(tempFileNameOut, '.');
				if (extension[0] == '.')
						old_extension[0] = 0;
				else
						old_extension[1] = 0;
				strcat(tempFileNameOut, extension);
		}
}

static 
void check_error()
{
#if _DEBUG
	/* Copy the error to the buffer so we can see it
	   in the debugger, otherwise do nothing with it. */
	char error_msg[100] = {""};
	strcpy(error_msg, strerror(errno));
	return;
#endif
}

#endif

static
int new_tmp_file(char const * mode)
{
	(void)mode;
	int fd = -1;

#ifdef _MSC_VER

# ifndef SUFFIX
#  define SUFFIX ".whatever"
# endif

	do {
		char fn[_MAX_DIR];
		GenerateTempFileName(fn, SUFFIX);
		_unlink(fn);
		fd = _open(fn, _O_BINARY | _O_CREAT | _O_TEMPORARY | _O_EXCL | _O_RDWR, _S_IREAD | _S_IWRITE);
	}while(fd < 0 && errno == EEXIST);

	if(fd < 0){
		check_error();
		return 0;
	}

#else

	char tpl[] = {"/tmp/.ezopenXXXXXX"};
	fd = mkstemp(tpl);
	if (fd != -1) {
		unlink(tpl);
	}
#endif

	return fd;
}


/* monolithic.sh copies this function into a cpp file which is why this follows... */
#ifdef __cplusplus
extern "C" {
#endif

/* sed tag static */
static
int ezopen(char const * utf8, char const * mode)
{
	int fd = -1;

	if(!mode)
		mode = "w";

	if(!utf8 || 0 == strcmp(utf8, "")){
		fd = new_tmp_file(mode);	
	}
	else {
		long flags = 0;

		if(strchr(mode, '~')){
			if(-1 == save_backup_copy(utf8, mode)){
				return -1;
			}
		}

		if(strchr(mode, 'w'))
			flags |= O_CREAT | O_TRUNC;

		if(strchr(mode, 'a'))
			flags |= O_CREAT | O_APPEND;

#ifdef _MSC_VER
		fd = _open(utf8, O_RDWR | _O_BINARY | flags, _S_IREAD | _S_IWRITE);
#else
		fd = open(utf8, O_RDWR | flags, 0740);
#endif
	}

	return fd;
}

#ifdef __cplusplus
}
#endif

#endif

