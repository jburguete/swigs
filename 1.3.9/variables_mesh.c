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

/**
 * \file variables_mesh.c
 * \brief Source file to define the necessary variables to define a mesh.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2005-2015 Javier Burguete Tolosa. All rights reserved.
 */

// Global variables of swigs defined in variables_mesh.c

/**
 * \var nth
 * \brief number of threads compatible with the mesh.
 * \var nthreads
 * \brief number of threads.
 * \var n
 * \brief number of mesh cells.
 * \var njunctions
 * \brief number of junctions.
 * \var vsize
 * \brief size in bytes of the variables.
 * \var t
 * \brief simulation time.
 * \var dt
 * \brief size of a time step.
 * \var tmax
 * \brief time of the next time step.
 * \var cell_thread
 * \brief array of index of mesh cells corresponding to every thread.
 * \var p
 * \brief array of flow parameters.
 * \var tp
 * \brief array of transport parameters.
 * \var v
 * \brief struct with the variables.
 * \var junction
 * \brief array of junctions.
 * \var m
 * \brief array of masses.
 * \var list_channels
 * \brief array of channels lists.
 */
int nth, nthreads, n, njunctions, vsize;
JBDOUBLE t, dt, tmax;
int *cell_thread = NULL;
Parameters *p = NULL;
TransportParameters *tp = NULL;
Variables v[1];
Junction *junction = NULL;
Mass *m = NULL;
GSList **list_channels = NULL;
