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

/*
variables_system.c

Source file to define the necessary variables to define a system of channels

Author: Javier Burguete Tolosa
Copyright (c) 2005-2014. All rights reserved
*/

/*
Global variables of swigs defined in variables_system.c

char *message: string to show messages
GMutex *mutex: variable to make mutex
int simulating: boolean defining if the program is simulating
JBFLOAT friction_dz: accuracy of the height in friction calculation
SectionPoint2 spnew[2]: two typical points of a cross section
TransientSection tsnew[2]: two typical transient sections
JBFLOAT csnewt1[1], csnewt2[1]: two typical times to define cross sections
CrossSection csnew[NEW_GEOMETRY_SIZE]: array of typical cross sections
JBFLOAT bfnew1t[1], bfnew2t[1], bfnew3t[1]: three typical times to define
	boundary conditions
JBFLOAT bfnew1p[1], bfnew2p[1], bfnew3p[1]: three typical parameters to define
	boundary conditions
BoundaryFlow bfnew[3]: tyree typical boundary conditions of flow
BoundaryTransport btnew[1]: a typical boundary condition of transport
ChannelTransport ctnew[1]: a typical struct defining the transport in a channel
JBFLOAT cgnewx[NEW_GEOMETRY_SIZE]: array of positions to define a channel geometry
Channel chanew[1]: a typical channel
System sysnew[1]: a typical system of channels
System sys[1]: the system of channels to simulate
*/

char *message=NULL;
GMutex mutex[1];
int simulating=0;
JBFLOAT friction_dz=FRICTION_DZ;
SectionPoint2 spnew[2]={{0.,0.,0.03,0},{1.,1.,0.,0}};
TransientSection tsnew[2]={{1,0,0.,0.,0.,1.,0.1,0.,spnew,spnew,"Transient1"},
	{1,0,0.,0.,0.,1.,0.1,0.,spnew,spnew,"Transient2"}};
JBFLOAT csnewt1[1]={0.};
JBFLOAT csnewt2[1]={0.};
CrossSection csnew[NEW_GEOMETRY_SIZE]=
	{{0,0,0,0,0,0,0.,0.,90.,0.,0.,0.,0.,0.,csnewt1,tsnew,"Section1"},
	{0,0,0,0,0,0,1.,0.,90.,0.,0.,0.,0.,0.,csnewt2,tsnew+1,"Section2"}};
JBFLOAT bfnew1t[1]={0.};
JBFLOAT bfnew1p1[1]={0.};
JBFLOAT bfnew2t[1]={0.};
JBFLOAT bfnew2p1[1]={0.};
JBFLOAT bfnew3t[1]={0.};
JBFLOAT bfnew3p1[1]={0.};
BoundaryFlow bfnew[3]={{0,1,1,0,0,0,bfnew1t,bfnew1p1,0,"Inner1",0},
	{0,1,1,0,0,0,bfnew2t,bfnew2p1,0,"Inlet",0},
	{0,2,1,0,0,0,bfnew3t,bfnew3p1,0,"Outlet",0}};
BoundaryTransport btnew[1]={{0,1,1,0,0,0,0.,bfnew1t,bfnew1p1,"Boundary"}};
ChannelTransport ctnew[1]={{-1,{{0,0,0}},NULL}};
JBFLOAT cgnewx[NEW_GEOMETRY_SIZE]={0.,1.};
Channel chanew[1]={{1,-1,0,1,0,1.,{{1,cgnewx,csnew}},
	{{0,0,0}},bfnew+1,NULL,NULL,NULL,NULL,"Channel"}};
System sys[1],sysnew[1]={{0,-1,0.,0.,0.,0.,0.9,0.5,1e-12,0.,SECTION_WIDTH_MIN,
	FLOW_DEPTH_MIN,GRANULOMETRIC_COEFFICIENT,NULL,NULL,chanew,"New.xml",".",
	"sol.tmp","./sol.tmp"}};
