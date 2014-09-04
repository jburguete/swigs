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

/*
variables_mesh.c

Source file to define the necessary variables to define a mesh

Author: Javier Burguete Tolosa
Copyright (c) 2005-2013. All rights reserved
*/

/*
Global variables of swigs defined in variables_mesh.c

int nth: number of threads compatible with the mesh
int nthreads: number of threads
JBDOUBLE t: simulation time
JBDOUBLE dt: size of a time step
JBDOUBLE tmax: time of the next time step
int n: number of mesh cells
int njunctions: number of junctions
int vsize: size in bytes of the variables
int *cell_thread: array of index of mesh cells corresponding to every thread
Parameters *p: array of flow parameters
TransportParameters *tp: array of transport parameters
Variables v[1]: struct with the variables
Junction *junction: array of junctions
Mass *mass: array of masses
GSList **list_channels: array of channels lists
*/

int nth,nthreads;

JBDOUBLE t,dt,tmax;
int n,njunctions,vsize;

int *cell_thread=NULL;

Parameters *p=NULL;
TransportParameters *tp=NULL;
Variables v[1];
Junction *junction=NULL;
Mass *m=NULL;

GSList **list_channels=NULL;
