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
 * \file editor_transient_section.h
 * \brief Header file to define transient section editor structures and methods.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2014 Javier Burguete Tolosa.
 */
#ifndef EDITOR_TRANSIENT_SECTION__H
#define EDITOR_TRANSIENT_SECTION__H 1

#include "transient_section.h"

/**
 * \struct EditorTransientSection
 * \brief Structure to define an editor of transient sections.
 */
typedef struct
{
/**
 * \var array
 * \brief JBWArrayEditor structure to define the section points.
 * \var graphic
 * \brief JBWGraphic structure to show the transient section graphic.
 * \var entry_name
 * \brief GtkEntry structure to set the transient section name.
 * \var array_type
 * \brief array of GtkRadioButton structures to set the transient section type.
 * \var entry_t
 * \brief GtkSpinButton structure to set the transient section date.
 * \var entry_u
 * \brief GtkSpinButton structure to set the transient section velocity.
 * \var entry_contraction
 * \brief GtkSpinButton structure to set the transient section
 *   expansion/contraction coefficient.
 * \var entry_hmax
 * \brief GtkSpinButton structure to set the transient section maximum depth.
 * \var entry_dz
 * \brief GtkSpinButton structure to set the transient section vertical step
 *   size calculating the friction coefficients.
 * \var label_name
 * \brief GtkLabel structure to show  a label of the transient section name.
 * \var label_t
 * \brief GtkLabel structure to show  a label of the transient section date.
 * \var label_u
 * \brief GtkLabel structure to show  a label of the transient section velocity.
 * \var label_contraction
 * \brief GtkLabel structure to show  a label of the transient section
 *   expansion/contraction coefficient.
 * \var label_hmax
 * \brief GtkLabel structure to show  a label of the transient section maximum
 *   depth.
 * \var label_dz
 * \brief GtkLabel structure to show  a label of the transient section vertical
 *   step size calculating the friction coefficients.
 * \var grid
 * \brief GtkGrid structure to pack the widgets.
 */
	JBWArrayEditor *array;
	JBWGraphic *graphic;
	GtkEntry *entry_name;
	GtkButton *button_insert, *button_remove, *button_plot;
	GtkRadioButton *array_type[N_TRANSIENT_SECTION_TYPES];
	GtkSpinButton *entry_t, *entry_u, *entry_contraction, *entry_hmax,
		*entry_dz;
	GtkLabel *label_name, *label_t, *label_u, *label_contraction, *label_hmax,
		*label_dz;
	GtkFrame *frame_type;
	GtkGrid *grid;
} EditorTransientSection;

#endif
