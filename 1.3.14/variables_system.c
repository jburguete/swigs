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
 * \file variables_system.c
 * \brief Source file to define the necessary variables to define a system of
 *   channels.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2005-2015 Javier Burguete Tolosa. All rights reserved.
 */

// Global variables of swigs defined in variables_system.c

/**
 * \var message
 * \brief string to show messages.
 * \var mutex
 * \brief variable to make a mutex.
 * \var simulating
 * \brief boolean defining if the program is simulating.
 * \var spnew
 * \brief array with two typical points to init a new cross section.
 * \var tsnew
 * \brief array with two typical transient sections to init a new cross section.
 * \var csnew
 * \brief array of typical cross sections to define a new channel.
 * \var bfnew1t
 * \brief first typical times array to define the inner boundary condition of a
 *   new channel.
 * \var bfnew2t
 * \brief second typical times array to define the inlet boundary conditions of
 *   a new channel.
 * \var bfnew3t
 * \brief third typical times array to define the outlet boundary condition of a
 *   new channel.
 * \var bfnew1p1
 * \brief first typical parameters array to define the inner boundary condition
 *   of a new channel.
 * \var bfnew2p1
 * \brief second typical parameters array to define the inlet boundary condition
 *   of a new channel.
 * \var bfnew3p1
 * \brief third typical parameters array to define the outlet boundary condition
 *   of a new channel.
 * \var bfnew
 * \brief tyree typical boundary conditions of flow to define a new channel.
 * \var btnew
 * \brief a typical boundary condition of transport to define a new channel.
 * \var ctnew
 * \brief a typical struct defining the transport in a new channel.
 * \var cgnewx
 * \brief array of positions to define the channel geometry in a new channel.
 * \var chanew
 * \brief a typical channel struct to define a new system of channels.
 * \var sysnew
 * \brief a typical new system of channels.
 * \var sys
 * \brief the system of channels to simulate.
 */
char *message = NULL;
GMutex mutex[1];
int simulating = 0;
SectionPoint2 spnew[2] = { {0., 0., 0.03, 0}, {1., 1., 0., 0} };

TransientSection tsnew[2] = {
  {1, 0, 0., 0., 0., 1., 0.1, 0., spnew, spnew, "Transient1"},
  {1, 0, 0., 0., 0., 1., 0.1, 0., spnew, spnew, "Transient2"}
};

CrossSection csnew[NEW_GEOMETRY_SIZE] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0., 0., 90., 0., 0., 0., 0., 0., tsnew,
   "Section1", NULL, NULL},
  {0, 0, 0, 0, 0, 0, 0, 0, 1., 0., 90., 0., 0., 0., 0., 0., tsnew + 1,
   "Section2", NULL, NULL}
};
JBFLOAT bfnew1t[1] = { 0. };
JBFLOAT bfnew1p1[1] = { 0. };
JBFLOAT bfnew2t[1] = { 0. };
JBFLOAT bfnew2p1[1] = { 0. };
JBFLOAT bfnew3t[1] = { 0. };
JBFLOAT bfnew3p1[1] = { 0. };

BoundaryFlow bfnew[3] = {
  {0, 0, 0, 0, 0, 0, 0, 0., 0., bfnew1t, bfnew1p1, 0, "Inner1", 0},
  {0, 0, 0, 0, 0, 0, -1, 0., 0., bfnew2t, bfnew2p1, 0, "Inlet", 0},
  {0, 1, 1, 0, 0, 0, 1, 0., 0., bfnew3t, bfnew3p1, 0, "Outlet", 0}
};
BoundaryTransport btnew[1] =
  { {0, 1, 1, 0, 0, 0, 0., 0., 0., bfnew1t, bfnew1p1, "Boundary"} };
ChannelTransport ctnew[1] = { {-1, {{0, 0, 0}}, NULL} };
JBFLOAT cgnewx[NEW_GEOMETRY_SIZE] = { 0., 1. };
Channel chanew[1] = { {1, -1, 0, 1, 0, 1., {{1, cgnewx, csnew}},
                       {{0, 0, 0}}, bfnew + 1, NULL, NULL, NULL, NULL, "Channel"}
};

System sys[1], sysnew[1] = { {0, -1, 0., 0., 0., 0., 0.9, 0.5, 1e-12, 0.,
                              SECTION_WIDTH_MIN, FLOW_DEPTH_MIN,
                              GRANULOMETRIC_COEFFICIENT, NULL, NULL,
                              chanew, "New.xml", ".", "sol.tmp", NULL, NULL,
                              NULL, "./sol.tmp", NULL,
                              NULL, NULL}
};
