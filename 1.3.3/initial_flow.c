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
 * \file initial_flow.c
 * \brief Source file to define initial flow conditions structures and
 *   functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2015 Javier Burguete Tolosa.
 */
#include "initial_flow.h"

/**
 * \fn void initial_flow_print(InitialFlow *ifc, FILE *file)
 * \brief Function to print an InitialFlow structure on a file.
 * \param ifc
 * \brief InitialFlow structure.
 * \param file
 * \brief file.
 */
#if !INLINE_INITIAL_FLOW_PRINT
void initial_flow_print(InitialFlow *ifc, FILE *file)
{_initial_flow_print(ifc, file);}
#endif

/**
 * \fn void initial_flow_error(char *m)
 * \brief Function to write an error message opening an InitialFlow structure.
 * \param m
 * \brief error message.
 */
#if !INLINE_INITIAL_FLOW_ERROR
void initial_flow_error(char *m) {_initial_flow_error(m);}
#endif

/**
 * \fn void initial_flow_delete(InitialFlow *ifc)
 * \brief Function to delete an InitialFlow structure.
 * \param ifc
 * \brief InitialFlow structure.
 */
#if !INLINE_INITIAL_FLOW_DELETE
void initial_flow_delete(InitialFlow *ifc) {_initial_flow_delete(ifc);}
#endif

/**
 * \fn void initial_flow_init_empty(InitialFlow *ifc)
 * \brief Function to init an empty InitialFlow structure.
 * \param ifc
 * \brief InitialFlow structure.
 */
#if !INLINE_INITIAL_FLOW_INIT_EMPTY
void initial_flow_init_empty(InitialFlow *ifc) {_initial_flow_init_empty(ifc);}
#endif

/**
 * \fn int initial_flow_copy(InitialFlow *ifc, InitialFlow *ifc_copy)
 * \brief Function to copy two InitialFlow structures.
 * \param ifc
 * \brief InitialFlow structure copy.
 * \param ifc_copy
 * \brief InitialFlow structure copied.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_INITIAL_FLOW_COPY
int initial_flow_copy(InitialFlow *ifc, InitialFlow *ifc_copy)
{return _initial_flow_copy(ifc, ifc_copy);}
#endif

/**
 * \fn int initial_flow_check(InitialFlow *ifc)
 * \brief Function to check if the points of the initial flow conditions are in
 *   correct order.
 * \param ifc
 * \brief InitialFlow structure.
 * \return 1 on succes, 0 on error. 
 */
#if !INLINE_INITIAL_FLOW_CHECK
int initial_flow_check(InitialFlow *ifc) {return _initial_flow_check(ifc);}
#endif

/**
 * \fn int initial_flow_open_xml(InitialFlow *ifc, xmlNode *node)
 * \brief Function to open an InitialFlow structure on a XML node.
 * \param ifc
 * \brief InitialFlow structure.
 * \param node
 * \brief XML node.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_INITIAL_FLOW_OPEN_XML
int initial_flow_open_xml(InitialFlow *ifc, xmlNode *node)
{return _initial_flow_open_xml(ifc, node);}
#endif

/**
 * \fn void initial_flow_save_xml(InitialFlow *ifc, xmlNode *node)
 * \brief Function to save an InitialFlow structure on a XML node.
 * \param ifc
 * \brief InitialFlow structure.
 * \param node
 * \brief XML node.
 */
#if !INLINE_INITIAL_FLOW_SAVE_XML
void initial_flow_save_xml(InitialFlow *ifc, xmlNode *node)
{_initial_flow_save_xml(ifc, node);}
#endif
