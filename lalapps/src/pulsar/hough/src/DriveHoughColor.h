/*
 *  Copyright (C) 2005 Badri Krishnan, Alicia Sintes  
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with with program; see the file COPYING. If not, write to the 
 *  Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
 *  MA  02111-1307  USA
 */

/**
 * \file DriveHoughColor.h
 * \author Alicia Sintes, Badri Krishnan
   \brief Header file for non-demodulated Hough search

 * File Name: DRIVEHOUGHCOLOR.h
 *
 * Authors: Sintes, A.M., Krishnan, B.
 *
 * Revision: $Id$
 *
 * History:   Created by Sintes June 16, 2003
 *    to test part of the Hough-Driver code.
 *    
 *-----------------------------------------------------------------------
 */
 
/*
 *   Protection against double inclusion (include-loop protection)
 *     Note the naming convention!
 */

#ifndef _DRIVEHOUGHCOLOR_H
#define _DRIVEHOUGHCOLOR_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glob.h>
#include <time.h>
#include <errno.h> 

#include <lal/Date.h>
#include <lal/DetectorSite.h>
#include <lal/LALDatatypes.h>
#include <lal/LALHough.h>
#include <lal/RngMedBias.h>
#include <lal/LALRunningMedian.h>
#include <lal/Velocity.h>
#include <lal/Statistics.h>
#include <lal/UserInput.h>
#include <lal/SFTfileIO.h>
#include <lal/NormalizeSFTRngMed.h>
#include <lal/LALInitBarycenter.h>
#include <lalapps.h>
#include <gsl/gsl_cdf.h>

#include "./PeakSelect.h"

/******************************************************
 *   Protection against C++ name mangling
 */

#ifdef  __cplusplus
extern "C" {
#endif


/******************************************************
 *  Assignment of Id string using NRCSID()
 */

NRCSID (DRIVEHOUGHCOLORH, "$Id$");

/******************************************************
 *  Error codes and messages.
 */
 
#define DRIVEHOUGHCOLOR_ENORM 0
#define DRIVEHOUGHCOLOR_ESUB  1
#define DRIVEHOUGHCOLOR_EARG  2
#define DRIVEHOUGHCOLOR_EBAD  3
#define DRIVEHOUGHCOLOR_EFILE 4
#define DRIVEHOUGHCOLOR_ENULL 5

#define DRIVEHOUGHCOLOR_MSGENORM "Normal exit"
#define DRIVEHOUGHCOLOR_MSGESUB  "Subroutine failed"
#define DRIVEHOUGHCOLOR_MSGEARG  "Error parsing arguments"
#define DRIVEHOUGHCOLOR_MSGEBAD  "Bad argument values"
#define DRIVEHOUGHCOLOR_MSGEFILE "Could not create output file"
#define DRIVEHOUGHCOLOR_MSGENULL "Null pointer"

/*********************************************************************/
/* Macros for printing errors & testing subroutines (from Creighton) */
/*********************************************************************/

#define ERROR( code, msg, statement )                                \
do {                                                                 \
  if ( lalDebugLevel & LALERROR )                                    \
    LALPrintError( "Error[0] %d: program %s, file %s, line %d, %s\n" \
                   "        %s %s\n", (code), *argv, __FILE__,       \
              __LINE__, DRIVEHOUGHCOLORH, statement ? statement :  \
                   "", (msg) );                                      \
} while (0)

#define INFO( statement )                                            \
do {                                                                 \
  if ( lalDebugLevel & LALINFO )                                     \
    LALPrintError( "Info[0]: program %s, file %s, line %d, %s\n"     \
                   "        %s\n", *argv, __FILE__, __LINE__,        \
              DRIVEHOUGHCOLORH, (statement) );                     \
} while (0)

#define SUB( func, statusptr )                                       \
do {                                                                 \
  if ( (func), (statusptr)->statusCode ) {                           \
    ERROR( DRIVEHOUGHCOLOR_ESUB, DRIVEHOUGHCOLOR_MSGESUB,      \
           "Function call \"" #func "\" failed:" );                  \
    return DRIVEHOUGHCOLOR_ESUB;                                  \
  }                                                                  \
} while (0)
/******************************************************************/

/* A global pointer for debugging. */
#ifndef NDEBUG
char *lalWatch;
#endif


/* ******************************************************************
 *  Structure, enum, union, etc., typdefs.
 */

typedef struct tagREAL8Cart3CoorVector{
  UINT4   	  length; /* number of elements */
  REAL8Cart3Coor  *data; /* x.y.z */
} REAL8Cart3CoorVector;

/*
 *  Functions Declarations (i.e., prototypes).
 */
int PrintHistogram(UINT4Vector *, CHAR *);
int PrintHmap2file(HOUGHMapTotal *, CHAR *, INT4 );
int PrintHmap2m_file(HOUGHMapTotal *, CHAR *, INT4 );


	     
void PrintHoughEvents (LALStatus       *status,
        	      FILE            *fpEvents,
	  	      INT4            houghThreshold,
		      HOUGHMapTotal   *ht,
	    	      HOUGHPatchGrid  *patch,
	 	      HOUGHDemodPar   *parDem);	     

void PrintHoughTemplates (LALStatus       *status,
        	      FILE            *fpEvents,
		      HOUGHMapTotal   *ht,
	    	      HOUGHPatchGrid  *patch,
	 	      HOUGHDemodPar   *parDem);	     

void Periodo2PSDrng (LALStatus  *status,
           REAL8Periodogram1    *psd,
           REAL8Periodogram1    *peri,
	   UINT2                *blocksRNG);

void PrintLogFile (LALStatus       *status,
		   CHAR            *dir,
		   CHAR            *basename,
		   CHAR            *skyfile,
		   CHAR            *executable);


/* ****************************************************** */

#ifdef  __cplusplus
}                /* Close C++ protection */
#endif


#endif     /* Close double-include protection _DRIVEHOUGHCOLOR_H */
