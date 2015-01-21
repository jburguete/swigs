/*
SWIGS (Shallow Water in Irregular Geometries Simulator): a software to simulate
transient or steady flows with solute transport in channels, channel networks
and rivers.

Copyright 2005-2015 Javier Burguete Tolosa.

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

#include "mesh.h"

#if !INLINE_PARAMETERS_PRINT
void parameters_print(Parameters *p,FILE *file) {_parameters_print(p,file);}
#endif

#if !INLINE_MASS_START
void mass_start(Mass *m) {_mass_start(m);}
#endif

#if !INLINE_MASS_COPY
void mass_copy(Mass *m,Mass *mcopy) {_mass_copy(m,mcopy);}
#endif

#if !INLINE_FLOW_MASS
JBDOUBLE flow_mass(Parameters *p,int n) {return _flow_mass(p,n);}
#endif

#if !INLINE_FLOW_MASS_ERROR
JBDOUBLE flow_mass_error(Mass *m,Parameters *p,int n)
{return _flow_mass_error(m,p,n);}
#endif

#if !INLINE_TRANSPORT_MASS
JBDOUBLE transport_mass(TransportParameters *tp,int n)
{return _transport_mass(tp,n);}
#endif

#if !INLINE_TRANSPORT_MASS_ERROR
JBDOUBLE transport_mass_error(Mass *m,TransportParameters *tp,int n)
{return _transport_mass_error(m,tp,n);}
#endif

#if !INLINE_TRANSPORT_VARIABLES_INTERPOLATE
void transport_variables_interpolate
	(Parameters *p,TransportParameters *tp,int n,InitialTransport *it)
{_transport_variables_interpolate(p,tp,n,it);}
#endif

#if !INLINE_TRANSPORT_VARIABLES_OPEN
void transport_variables_open
	(Parameters *p,TransportParameters *tp,int n,InitialTransport *it)
{_transport_variables_open(p,tp,n,it);}
#endif

#if !INLINE_VARIABLES_CREATE
int variables_create(Variables *v,int n,int *vsize,int nt)
{return _variables_create(v,n,vsize,nt);}
#endif

#if !INLINE_MESH_HMIN
JBDOUBLE mesh_hmin(Parameters *p) {return _mesh_hmin(p);}
#endif

#if !INLINE_MESH_ACTUALIZE
void mesh_actualize(Parameters *p,int n) {_mesh_actualize(p,n);}
#endif

#if !INLINE_INITIAL_VARIABLES_OPEN
void initial_variables_open(Parameters *p,TransportParameters *tp,
	InitialFlow *ifc,ChannelTransport *ct,int nif,int *nit,int nt)
{_initial_variables_open(p,tp,ifc,ct,nif,nit,nt);}
#endif

#if !INLINE_PART_OPEN
int part_open(int thread) {return _part_open(thread);}
#endif

#if !INLINE_PART_OPEN_EXIT
void part_open_exit(int thread) {_part_open_exit(thread);}
#endif

#if !INLINE_SORT_INT
void sort_int(int *x,int *ni,int n) {_sort_int(x,ni,n);}
#endif

#if !INLINE_SORT_INVERSE_INT
void sort_inverse_int(int *x,int *ni,int n) {_sort_inverse_int(x,ni,n);}
#endif

#if !INLINE_SYSTEM_LIST
void system_list() {_system_list();}
#endif

#if !INLINE_JUNCTION_PRINT
void junction_print(Junction *junction,FILE *file)
{_junction_print(junction,file);}
#endif

#if !INLINE_JUNCTION_DELETE
void junction_delete(Junction *junction) {_junction_delete(junction);}
#endif

#if !INLINE_JUNCTION_CREATE
int junction_create(Junction *junction) {return _junction_create(junction);}
#endif

#if !INLINE_JUNCTION_ADD_PARAMETER
int junction_add_parameter(Junction *junction,Parameters *parameter,
	TransportParameters *transport,JBFLOAT angle)
{return _junction_add_parameter(junction,parameter,transport,angle);}
#endif

#if !INLINE_JUNCTION_ADD_INLET
int junction_add_inlet(Junction *junction,Channel *channel)
{return _junction_add_inlet(junction,channel);}
#endif

#if !INLINE_JUNCTION_ADD_OUTLET
int junction_add_outlet(Junction *junction,Channel *channel)
{return _junction_add_outlet(junction,channel);}
#endif

#if !INLINE_JUNCTION_ADD_TRIBUTARY
int junction_add_tributary(Junction *junction,Channel *channel,int pos,int pos2)
{return _junction_add_tributary(junction,channel,pos,pos2);}
#endif

#if !INLINE_JUNCTION_SET_VOLUMES
int junction_set_volumes(Junction *junction)
{return _junction_set_volumes(junction);}
#endif

#if !INLINE_JUNCTIONS_DELETE
void junctions_delete(Junction **junction,int *n)
{_junctions_delete(junction,n);}
#endif

#if !INLINE_JUNCTIONS_OPEN
int junctions_open(Junction **junction,int *n)
{return _junctions_open(junction,n);}
#endif

#if !INLINE_MESH_DELETE
void mesh_delete(System *system,Parameters **p,TransportParameters **tp,
	Variables *v,Junction **junction,Mass **m,int *n,int *njunctions)
{_mesh_delete(system,p,tp,v,junction,m,n,njunctions);}
#endif

#if !INLINE_MESH_OPEN
int mesh_open
(
	System *system,Parameters **p,TransportParameters **tp,Variables *v,
	Junction **junction,Mass **m,int *n,int *njunctions,int *vsize
)
{return _mesh_open(system,p,tp,v,junction,m,n,njunctions,vsize);}
#endif

#if MAKE_MESH==1
#define _GNU_SOURCE
#include <locale.h>
#include <unistd.h>
#include "variables_system.c"
#include "variables_mesh.c"
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
	xmlSetCompressMode(9);
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
	if (!system_open_xml(sys,argc[1],"./")) return 2;
	if (!mesh_open(sys,&p,&tp,v,&m,&n,&vsize)) return 3;
	return 0;
}
#endif
