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
 * \file write.c
 * \brief Source file to define file inputs and outputs.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2005-2015 Javier Burguete Tolosa. All rights reserved.
 */
#include "write.h"

/**
 * \fn int write_data(FILE *file, Variables *v, Parameters *p, \
 *   TransportParameters *tp, int n, int vsize, int nt)
 * \brief Function to write the solution in a binary file.
 * \param file
 * \brief binary file of the solution.
 * \param v
 * \brief arrays of variables.
 * \param p
 * \brief array of flow parameters.
 * \param tp
 * \brief array of transport parameters.
 * \param n
 * \brief number of mesh cells.
 * \param vsize
 * \brief size of the variables of a mesh cell in bytes.
 * \param nt
 * \brief number of transported solutes.
 * \return number of saved mesh cells on succes, 0 on error.
 */
#if !INLINE_WRITE_DATA
int write_data(FILE *file, Variables *v, Parameters *p, TransportParameters *tp,
	int n, int vsize, int nt)
{return _write_data(file, v, p, tp, n, vsize, nt);}
#endif

/**
 * \fn int read_data(FILE *file, Variables *v, int n, int vsize)
 * \brief Function to read the solution in a binary file.
 * \param file
 * \brief binary file of the solution.
 * \param v
 * \brief arrays of variables.
 * \param n
 * \brief number of mesh cells.
 * \param vsize
 * \brief size of the variables of a mesh cell in bytes.
 * \return number of read mesh cells on success, 0 on error.
 */
#if !INLINE_READ_DATA
int read_data(FILE *file, Variables *v, int n, int vsize)
{return _read_data(file, v, n, vsize);}
#endif

/**
 * \fn void write_variables(FILE *file, Variables *v, int i, int n, int nt)
 * \brief Function to write the variables of some mesh cells in an ASCII file.
 * \param file
 * \brief binary file of the solution.
 * \param v
 * \brief arrays of variables.
 * \param i
 * \brief number of the first mesh cell to write.
 * \param n
 * \brief number of mesh cells to write.
 * \param nt
 * \brief number of transported solutes.
 */
#if !INLINE_WRITE_VARIABLES
void write_variables(FILE *file, Variables *v, int i, int n, int nt)
{_write_variables(file, v, i, n, nt);}
#endif

/**
 * \fn void read_profile(FILE *file, Variables *v, int n, int profile, \
 *   int vsize)
 * \brief Function to read a longitudinal profile of a system of channels in an
 *   ASCII file.
 * \param file
 * \brief binary file of the solution.
 * \param v
 * \brief arrays of variables.
 * \param n
 * \brief number of mesh cells.
 * \param profile
 * \brief number of the saved profile.
 * \param vsize
 * \brief size of the variables of a mesh cell in bytes.
 */
#if !INLINE_READ_PROFILE
void read_profile(FILE *file, Variables *v, int n, int profile, int vsize)
{_read_profile(file, v, n, profile, vsize);}
#endif

/**
 * \fn void write_profile(FILE *file_read, FILE *file_write, int i, int i2, \
 *   int n, int profile, int *vsize, int nt)
 * \brief Function to write a longitudinal profile of a channel in an ASCII
 *   file.
 * \param file_read
 * \brief binary file to read the solution.
 * \param file_write
 * \brief ASCII file to write the longitudinal profile.
 * \param i
 * \brief number of the initial mesh cell of the channel.
 * \param i2
 * \brief number of the final mesh cell of the channel.
 * \param n
 * \brief number of mesh cells.
 * \param profile
 * \brief number of the saved profile.
 * \param vsize
 * \brief pointer to the size of the variables of a mesh cell in bytes.
 * \param nt
 * \brief number of transported solutes.
*/
#if !INLINE_WRITE_PROFILE
void write_profile(FILE *file_read, FILE *file_write, int i, int i2, int n,
	int profile, int *vsize, int nt)
{_write_profile(file_read, file_write, i, i2, n, profile, vsize, nt);}
#endif

/**
 * \fn void write_evolution(FILE *file_read, FILE *file_write, int n, \
 *   int section, int *vsize, Channel *channel)
 * \brief Function to write the temporal evolution of a channel cross section in
 *   an ASCII file.
 * \param file_read
 * \brief binary file to read the solution.
 * \param file_write
 * \brief ASCII file to write the temporal evolution of the cross section.
 * \param n
 * \brief number of mesh cells.
 * \param section
 * \brief number of the channel cross section.
 * \param vsize
 * \brief pointer to the size of the variables of a mesh cell in bytes.
 * \param channel
 * \brief pointer to the channel.
*/
#if !INLINE_WRITE_EVOLUTION
void write_evolution(FILE *file_read, FILE *file_write, int n, int section,
	int *vsize, Channel *c)
{_write_evolution(file_read, file_write, n, section, vsize, c);}
#endif

/**
 * \fn void read_evolution(int channel, int section, int type1, int type2, \
 *   JBFLOAT **x, JBFLOAT **y1, JBFLOAT **z1, int *nt, int vsize)
 * \brief Function to read the temporal evolution of some variables of a channel
 *   cross section to make a graphical.
 * \param channel
 * \brief number of the channel.
 * \param section
 * \brief number of the channel cross section.
 * \param type1
 * \brief type of the first variable to read.
 * \param type2
 * \brief type of the second variable to read.
 * \param x
 * \brief pointer to the array of times.
 * \param y1
 * \brief pointer to the array of the first variable.
 * \param z1
 * \brief pointer to the array of the second variable.
 * \param nt
 * \brief pointer to the number of solutes.
 * \param vsize
 * \brief size of the variables of a mesh cell in bytes.
 */
#if !INLINE_READ_EVOLUTION
void read_evolution(int channel, int section, int type1, int type2, JBFLOAT **x,
	JBFLOAT **y1, JBFLOAT **z1, int *n, int vsize)
{_read_evolution(channel, section, type1, type2, x, y1, z1, n, vsize);}
#endif

/**
 * \fn void write_advances(FILE *file, Parameters *p, int n)
 * \brief Function to write the flow advances in an ASCII file.
 * \param file
 * \brief ASCII file to write the flow advances.
 * \param p
 * \brief array of flow parameters.
 * \param n
 * \brief number of mesh cells.
 */
#if !INLINE_WRITE_ADVANCES
void write_advances(FILE *file, Parameters *p, int n)
{_write_advances(file, p, n);}
#endif

/**
 * \fn void write_plumes_channel(FILE *file, Parameters *p, \
 *   TransportParameters *tp, Transport *transport, Channel *channel)
 * \brief Function to write the danger plumes position of a channel in an ASCII
 *   file.
 * \param file
 * \brief ASCII file to write the danger plumes position.
 * \param p
 * \brief array of flow parameters.
 * \param tp
 * \brief array of transport parameters.
 * \param channel
 * \brief pointer to the channel.
 */
#if !INLINE_WRITE_PLUMES_CHANNEL
void write_plumes_channel(FILE *file, Parameters *p, TransportParameters *tp,
	Transport *transport, Channel *channel)
{_write_plumes_channel(file, p, tp, transport, channel);}
#endif

/**
 * \fn void write_plumes(FILE *file, Parameters *p, TransportParameters *tp, \
 *   JBDOUBLE t, System *s)
 * \brief Function to write the danger plumes position of a system of channels
 *   in an ASCII file.
 * \param file
 * \brief ASCII file to write the danger plumes position.
 * \param p
 * \brief array of flow parameters.
 * \param tp
 * \brief array of transport parameters.
 * \param t
 * \brief actual time.
 * \param s
 * \brief pointer to the system of channels.
 */
#if !INLINE_WRITE_PLUMES
void write_plumes(FILE *file, Parameters *p, TransportParameters *tp,
	JBDOUBLE t, System *s)
{_write_plumes(file, p, tp, t, s);}
#endif

/**
 * \fn void write_contributions(FILE *file, JBDOUBLE t, System *s)
 * \brief Function to write the boundary discharge contributions in an ASCII
 *   file.
 * \param file
 * \brief file to write the boundary discharges.
 * \param t
 * \brief actual time.
 * \param s
 * \brief system of channels.
 */
#if !INLINE_WRITE_CONTRIBUTIONS
void write_contributions(FILE *file, JBDOUBLE t, System *s)
{_write_contributions(file, t, s);}
#endif
