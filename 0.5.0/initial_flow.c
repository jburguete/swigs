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

#include "initial_flow.h"

/**
 * \fn void initial_flow_print(InitialFlow *fic, FILE *file)
 * \brief Method to print a InitialFlow structure on a file.
 * \param fic
 * \brief InitialFlow structure.
 * \param file
 * \brief file.
 */
#if !INLINE_INITIAL_FLOW_PRINT
void initial_flow_print(InitialFlow *fic, FILE *file)
{_initial_flow_print(fic, file);}
#endif

/**
 * \fn void initial_flow_error(char *m)
 * \brief Method to write an error message opening a InitialFlow structure.
 * \param m
 * \brief error message.
 */
#if !INLINE_INITIAL_FLOW_ERROR
void initial_flow_error(char *m) {_initial_flow_error(m);}
#endif

/**
 * \fn void initial_flow_delete(InitialFlow *fic)
 * \brief Method to delete a InitialFlow structure.
 * \param fic
 * \brief InitialFlow structure.
 */
#if !INLINE_INITIAL_FLOW_DELETE
void initial_flow_delete(InitialFlow *fic) {_initial_flow_delete(fic);}
#endif

/**
 * \fn int initial_flow_copy(InitialFlow *fic, InitialFlow *fic_copy)
 * \brief Method to copy two InitialFlow structures.
 * \param fic
 * \brief InitialFlow structure copy.
 * \param fic_copy
 * \brief InitialFlow structure copied.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_INITIAL_FLOW_COPY
int initial_flow_copy(InitialFlow *fic, InitialFlow *fic_copy)
{return _initial_flow_copy(fic, fic_copy);}
#endif

/**
 * \fn int initial_flow_order(InitialFlow *fic)
 * \brief Method to check if the points of the initial flow condition are in
 *   correct order.
 * \return 1 on succes, 0 on error. 
 */
#if !INLINE_INITIAL_FLOW_ORDER
int initial_flow_order(InitialFlow *fic) {return _initial_flow_order(fic);}
#endif

/**
 * \fn int initial_flow_open_xml(InitialFlow *fic, xmlNode *node)
 * \brief Method to open a InitialFlow structure on a XML node.
 * \param fic
 * \brief InitialFlow structure.
 * \param node
 * \brief XML node.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_INITIAL_FLOW_OPEN_XML
int initial_flow_open_xml(InitialFlow *fic, xmlNode *node)
{return _initial_flow_open_xml(fic, node);}
#endif

/**
 * \fn void initial_flow_save_xml(InitialFlow *fic, xmlNode *node)
 * \brief Method to save a InitialFlow structure on a XML node.
 * \param fic
 * \brief InitialFlow structure.
 * \param node
 * \brief XML node.
 */
#if !INLINE_INITIAL_FLOW_SAVE_XML
void initial_flow_save_xml(InitialFlow *fic, xmlNode *node)
{_initial_flow_save_xml(fic, node);}
#endif
