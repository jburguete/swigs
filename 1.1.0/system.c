/*
SWIGS (Shallow Water in Irregular Geometries Simulator): a software to simulate
transient or steady flows with solute transport in channels, channel networks
and rivers.

Copyright 2005-2014 Javier Burguete Tolosa.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice,
		this list of conditions and the following disclaimer.

	2. Redistributions in binary form must reproduce the above copyright notice,
		this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY Javier Burguete Tolosa ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL Javier Burguete Tolosa OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * \file system.c
 * \brief Source file to define channels system structures and functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2014 Javier Burguete Tolosa.
 */
#include "system.h"

/**
 * \fn void system_print(System *s, FILE *file)
 * \brief Function to print a System structure in a file.
 * \param s
 * \brief System structure.
 * \param file
 * \brief file.
 */
#if !INLINE_SYSTEM_PRINT
void system_print(System *s, FILE *file) {_system_print(s, file);}
#endif

/**
 * \fn void system_error(System *s, char *m)
 * \brief Function to write an error message opening a System structure.
 * \param s
 * \brief System structure.
 * \param m
 * \brief error message.
 */
#if !INLINE_SYSTEM_ERROR
void system_error(System *s, char *m) {_system_error(s, m);}
#endif

/**
 * \fn void system_delete(System *s)
 * \brief Function to delete a System structure.
 * \param s
 * \brief System structure.
 */
#if !INLINE_SYSTEM_DELETE
void system_delete(System *s) {_system_delete(s);}
#endif

/**
 * \fn void system_init_empty(System *s)
 * \brief Function to init an empty System structure.
 * \param s
 * \brief System structure.
 */
#if !INLINE_SYSTEM_INIT_EMPTY
void system_init_empty(System *s) {_system_init_empty(s);}
#endif

/**
 * \fn int system_insert_channel(System *s, int i)
 * \brief Function to insert a channel in a channels system.
 * \param s
 * \brief System structure.
 * \param i
 * \brief insertion position.
 * \return 1 on success, 0 on error.
 */
#if !INLINE_SYSTEM_INSERT_CHANNEL
int system_insert_channel(System *s, int i)
{return _system_insert_channel(c, i);}
#endif

/**
 * \fn void system_remove_channel(System *s, int i)
 * \brief Function to remove a channel in a channels system.
 * \param s
 * \brief System structure.
 * \param i
 * \brief deletion position.
 */
#if !INLINE_SYSTEM_REMOVE_CHANNEL
void system_remove_channel(System *s, int i) {_system_remove_channel(c, i);}
#endif

/**
 * \fn int system_insert_transport(System *s, int i)
 * \brief Function to insert a transported solute in a system.
 * \param s
 * \brief System structure.
 * \param i
 * \brief insertion position.
 */
#if !INLINE_SYSTEM_INSERT_TRANSPORT
int system_insert_transport(System *system, int i)
{return _system_insert_transport(system, i);}
#endif

/**
 * \fn void system_remove_transport(System *s, int i)
 * \brief Function to remove a transported solute in a system.
 * \param s
 * \brief System structure.
 * \param i
 * \brief deletion position.
 */
#if !INLINE_SYSTEM_REMOVE_TRANSPORT
void system_remove_transport(System *system, int i)
{_system_remove_transport(system, i);}
#endif

/**
 * \fn int system_create(System *s, char *name, char *directory, char *buffer)
 * \brief Function to create a System structure.
 * \param s
 * \brief System structure.
 * \param name
 * \brief name.
 * \param directory
 * \brief directory.
 * \param buffer
 * \brief file name with absolute path.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_SYSTEM_CREATE
int system_create(System *s, char *name, char *directory, char *buffer)
{return _system_create(s, name, directory, buffer);}
#endif

/**
 * \fn int system_copy(System *s, System *s_copy)
 * \brief Function to copy two System structures.
 * \param s
 * \brief System structure copy.
 * \param s_copy
 * \brief System structure copied.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_SYSTEM_COPY
int system_copy(System *s, System *s_copy) {return _system_copy(s, s_copy);}
#endif

/**
 * \fn int system_check(System *s)
 * \brief Function to check if the system data are correct.
 * \param s
 * \brief System structure.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_SYSTEM_CHECK
int system_check(System *s) {return _system_check(s);}
#endif

/**
 * \fn int system_open_xml(System *s, int nt, xmlNode *node)
 * \brief Function to open a System structure on a XML node.
 * \param s
 * \brief System structure.
 * \param nt
 * \brief number of transported solutes.
 * \param node
 * \brief XML node.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_SYSTEM_OPEN_XML
int system_open_xml(System *s, char *name, char *directory)
{return _system_open_xml(s, name, directory);}
#endif

/**
 * \fn void system_save_xml(System *s, xmlNode *node)
 * \brief Function to save a System structure on a XML node.
 * \param s
 * \brief System structure.
 * \param node
 * \brief XML node.
 */
#if !INLINE_SYSTEM_SAVE_XML
void system_save_xml(System *s) {_system_save_xml(s);}
#endif

#if MAKE_SYSTEM==1
#define _GNU_SOURCE
#include <locale.h>
#include <unistd.h>
#include "variables_system.c"
int main(int argn, char **argc)
{
	#if JB_DEBUG_FILE
		stderr=g_fopen("debug.log", "w");
	#endif
	#if DEBUG_MAIN
		fprintf(stderr, "Main\n");
	#endif
	g_thread_init(NULL);
	#ifdef G_OS_WIN32
		nthreads=4;
	#else
		nthreads=(int)sysconf(_SC_NPROCESSORS_CONF);
	#endif
	mutex=g_mutex_new();
	xmlKeepBlanksDefault(0);
	xmlSetCompressMode(9);
	setlocale(LC_ALL, "");
	setlocale(LC_NUMERIC, "C");
	bindtextdomain("swigs", "po");
	bind_textdomain_codeset("swigs", "UTF-8");
	textdomain("swigs");
	#if DEBUG_MAIN
		fprintf(stderr, "M copying channel\n");
	#endif
	if (!system_copy(sys, sysnew)) return 3;
	if (argn!=2)
	{
		jbw_show_error2(gettext("Usage is"), gettext("swigs simulation_file"));
		return 1;
	}
	system_open_xml(sys, argc[1], "./");
	return 0;
}
#endif
