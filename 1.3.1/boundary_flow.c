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
 * \file boundary_flow.c
 * \brief Source file to define flow boundary conditions structures and
 *   functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2014 Javier Burguete Tolosa.
 */
#include "boundary_flow.h"

/**
 * \var boundary_flow_extern_type
 * \brief Array of extern boundary flow types.
 * \var boundary_flow_inner_type
 * \brief Array of inner boundary flow types.
 */
int boundary_flow_extern_type[N_BOUNDARY_FLOW_EXTERN_TYPES] =
{
	BOUNDARY_FLOW_TYPE_Q,
	BOUNDARY_FLOW_TYPE_H,
	BOUNDARY_FLOW_TYPE_Z,
	BOUNDARY_FLOW_TYPE_Q_H,
	BOUNDARY_FLOW_TYPE_Q_Z,
	BOUNDARY_FLOW_TYPE_QT,
	BOUNDARY_FLOW_TYPE_HT,
	BOUNDARY_FLOW_TYPE_ZT,
	BOUNDARY_FLOW_TYPE_QT_HT,
	BOUNDARY_FLOW_TYPE_QT_ZT,
	BOUNDARY_FLOW_TYPE_QH,
	BOUNDARY_FLOW_TYPE_QZ,
	BOUNDARY_FLOW_TYPE_SUPERCRITICAL,
	BOUNDARY_FLOW_TYPE_GATE,
	BOUNDARY_FLOW_TYPE_JUNCTION
};
int boundary_flow_inner_type[N_BOUNDARY_FLOW_INNER_TYPES] =
{
	BOUNDARY_FLOW_TYPE_Q,
	BOUNDARY_FLOW_TYPE_QT,
	BOUNDARY_FLOW_TYPE_QH,
	BOUNDARY_FLOW_TYPE_QZ,
	BOUNDARY_FLOW_TYPE_DAM,
	BOUNDARY_FLOW_TYPE_PIPE,
	BOUNDARY_FLOW_TYPE_GATE,
	BOUNDARY_FLOW_TYPE_JUNCTION
};

/**
 * \fn void boundary_flow_print(BoundaryFlow *bf, FILE *file)
 * \brief Function to print a BoundaryFlow structure on a file.
 * \param bf
 * \brief BoundaryFlow structure.
 * \param file
 * \brief file.
 */
#if !INLINE_BOUNDARY_FLOW_PRINT
void boundary_flow_print(BoundaryFlow *bf, FILE *file)
{_boundary_flow_print(bf, file);}
#endif

/**
 * \fn void boundary_flow_error(BoundaryFlow *bf, char *m)
 * \brief Function to write an error message opening a BoundaryFlow structure.
 * \param bf
 * \brief BoundaryFlow structure.
 * \param m
 * \brief error message.
 */
#if !INLINE_BOUNDARY_FLOW_ERROR
void boundary_flow_error(BoundaryFlow *bf, char *m)
{_boundary_flow_error(bf, m);}
#endif

/**
 * \fn void boundary_flow_delete(BoundaryFlow *bf)
 * \brief Function to delete a BoundaryFlow structure.
 * \param bf
 * \brief BoundaryFlow structure.
 */
#if !INLINE_BOUNDARY_FLOW_DELETE
void boundary_flow_delete(BoundaryFlow *bf) {_boundary_flow_delete(bf);}
#endif

/**
 * \fn void boundary_flow_init_empty(BoundaryFlow *bf)
 * \brief Function to init an empty BoundaryFlow structure.
 * \param bf
 * \brief BoundaryFlow structure.
 */
#if !INLINE_BOUNDARY_FLOW_INIT_EMPTY
void boundary_flow_init_empty(BoundaryFlow *bf) {_boundary_flow_init_empty(bf);}
#endif

/**
 * \fn int boundary_flow_insert_junction(BoundaryFlow *bf, int position)
 * \brief Function to insert a JunctionData in a BoundaryFlow structure.
 * \param bf
 * \brief BoundaryFlow structure.
 * \param position
 * \brief insertion position.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_BOUNDARY_FLOW_INSERT_JUNCTION
int boundary_flow_insert_junction(BoundaryFlow *bf, int position)
{return _boundary_flow_insert_junction(bf, position);}
#endif

/**
 * \fn int boundary_flow_remove_junction(BoundaryFlow *bf, int position)
 * \brief Function to remove a JunctionData in a BoundaryFlow structure.
 * \param bf
 * \brief BoundaryFlow structure.
 * \param position
 * \brief deletion position.
 */
#if !INLINE_BOUNDARY_FLOW_REMOVE_JUNCTION
void boundary_flow_remove_junction(BoundaryFlow *bf, int position)
{_boundary_flow_remove_junction(bf, position);}
#endif

/**
 * \fn int boundary_flow_copy(BoundaryFlow *bf, BoundaryFlow *bf_copy)
 * \brief Function to copy two BoundaryFlow structures.
 * \param bf
 * \brief BoundaryFlow structure copy.
 * \param bf_copy
 * \brief BoundaryFlow structure copied.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_BOUNDARY_FLOW_COPY
int boundary_flow_copy(BoundaryFlow *bf, BoundaryFlow *bf_copy)
{return _boundary_flow_copy(bf, bf_copy);}
#endif

/**
 * \fn int boundary_flow_open_xml(BoundaryFlow *bf, xmlNode *node, int position)
 * \brief Function to open a BoundaryFlow structure on a XML node.
 * \param bf
 * \brief BoundaryFlow structure.
 * \param node
 * \brief XML node.
 * \param position
 * \brief -1 on inlet, 0 on inner and 1 on outlet flow boundary condition.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_BOUNDARY_FLOW_OPEN_XML
int boundary_flow_open_xml(BoundaryFlow *bf, xmlNode *node, int position)
{return _boundary_flow_open_xml(bf, node, position);}
#endif

/**
 * \fn void boundary_flow_save_xml(BoundaryFlow *bf, xmlNode *node)
 * \brief Function to save a BoundaryFlow structure on a XML node.
 * \param bf
 * \brief BoundaryFlow structure.
 * \param node
 * \brief XML node.
 */
#if !INLINE_BOUNDARY_FLOW_SAVE_XML
void boundary_flow_save_xml(BoundaryFlow *bf, xmlNode *node)
{return _boundary_flow_save_xml(bf, node);}
#endif

/**
 * \fn int boundary_flow_type(BoundaryFlow *bf, int type)
 * \brief Function to get the flow boundary condition type.
 * \param bf
 * \brief BoundaryFlow structure.
 * \param type
 * \brief type.
 * \return flow boundary condition type.
 */
#if !INLINE_BOUNDARY_FLOW_TYPE
int boundary_flow_type(BoundaryFlow *bf, int type)
{return _boundary_flow_type(bf, type);}
#endif

/**
 * \fn int boundary_flow_parameter(BoundaryFlow *bf, JBDOUBLE t)
 * \brief Function to get the first parameter of a time dependent flow boundary
 *   condition.
 * \param bf
 * \brief BoundaryFlow structure.
 * \param t
 * \brief time.
 * \return first parameter.
 */
#if !INLINE_BOUNDARY_FLOW_PARAMETER
JBDOUBLE boundary_flow_parameter(BoundaryFlow *bf, JBDOUBLE t)
{return _boundary_flow_parameter(bf, t);}
#endif

/**
 * \fn int boundary_flow_parameter2(BoundaryFlow *bf, JBDOUBLE t)
 * \brief Function to get the second parameter of a time dependent flow boundary
 *   condition.
 * \param bf
 * \brief BoundaryFlow structure.
 * \param t
 * \brief time.
 * \return second parameter.
 */
#if !INLINE_BOUNDARY_FLOW_PARAMETER2
JBDOUBLE boundary_flow_parameter2(BoundaryFlow *bf, JBDOUBLE t)
{return _boundary_flow_parameter2(bf, t);}
#endif

/**
 * \fn int boundary_flow_parameter3(BoundaryFlow *bf, JBDOUBLE p)
 * \brief Function to get the first parameter of a parameter dependent flow
 *   boundary condition.
 * \param bf
 * \brief BoundaryFlow structure.
 * \param p
 * \brief parameter.
 * \return first parameter.
 */
#if !INLINE_BOUNDARY_FLOW_PARAMETER3
JBDOUBLE boundary_flow_parameter3(BoundaryFlow *bf, JBDOUBLE p)
{return _boundary_flow_parameter3(bf, p);}
#endif

/**
 * \fn int boundary_flow_integral(BoundaryFlow *bf, JBDOUBLE t, JBDOUBLE tmax)
 * \brief Function to get the integral of the first parameter of a flow boundary
 *   condition.
 * \param bf
 * \brief BoundaryFlow structure.
 * \param t
 * \brief initial time.
 * \param tmax
 * \brief final time.
 * \return first parameter integral.
 */
#if !INLINE_BOUNDARY_FLOW_INTEGRAL
JBDOUBLE boundary_flow_integral(BoundaryFlow *bf, JBDOUBLE t, JBDOUBLE tmax)
{return _boundary_flow_integral(bf, t, tmax);}
#endif
