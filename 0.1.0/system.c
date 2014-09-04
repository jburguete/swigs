/*
SWIGS (Shallow Water in Irregular Geometries Simulator): a software to simulate
transient or steady flows with solute transport in channels, channel networks and rivers

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

#include "system.h"

#if !INLINE_SYSTEM_PRINT
void system_print(System *s,FILE *file) {_system_print(s,file);}
#endif

#if !INLINE_SYSTEM_ERROR
void system_error(System *s,char *m) {_system_error(s,m);}
#endif

#if !INLINE_SYSTEM_INIT
void system_init(System *s) {_system_init(s);}
#endif

#if !INLINE_SYSTEM_DELETE
void system_delete(System *s) {_system_delete(s);}
#endif

#if !INLINE_SYSTEM_COPY
int system_copy(System *s,System *s_copy) {return _system_copy(s,s_copy);}
#endif

#if !INLINE_SYSTEM_JUNCTIONS_TEST
int system_junctions_test(System *s) {return _system_junctions_test(s);}
#endif

#if !INLINE_SYSTEM_OPEN_START
int system_open_start(System *s,char *name,char *directory,char *buffer)
{return _system_open_start(s,name,directory,buffer);}
#endif

#if !INLINE_SYSTEM_OPEN_XML
int system_open_xml(System *s,char *name,char *directory)
{return _system_open_xml(s,name,directory);}
#endif

#if !INLINE_SYSTEM_SAVE_XML
void system_save_xml(System *s) {_system_save_xml(s);}
#endif

#if MAKE_SYSTEM==1
#define _GNU_SOURCE
#include <locale.h>
#include <unistd.h>
#include "variables_system.c"
int main(int argn,char **argc)
{
	#if JB_DEBUG_FILE
		stderr=g_fopen("debug.log","w");
	#endif
	#if DEBUG_MAIN
		fprintf(stderr,"Main\n");
	#endif
	g_thread_init(NULL);
	#ifdef G_OS_WIN32
		nthreads=4;
	#else
		nthreads=(int)sysconf(_SC_NPROCESSORS_CONF);
	#endif
	mutex=g_mutex_new();
	xmlKeepBlanksDefault(0);
	#if JB_INTERFACE!=JB_INTERFACE_DEMO
		xmlSetCompressMode(9);
	#endif
	setlocale(LC_ALL,"");
	setlocale(LC_NUMERIC,"C");
	bindtextdomain("swigs","po");
	bind_textdomain_codeset("swigs","UTF-8");
	textdomain("swigs");
	#if DEBUG_MAIN
		fprintf(stderr,"M copying channel\n");
	#endif
	if (!system_copy(sys,sysnew)) return 3;
	if (argn!=2)
	{
		jbw_show_error2(gettext("Usage is"), gettext("swigs simulation_file"));
		return 1;
	}
	system_open_xml(sys,argc[1],"./");
	return 0;
}
#endif
