/*----------------------------------------------------------------------- 
 * 
 * File Name: CVectorAbs.c
 * 
 * Author: Creighton, T. D.
 * 
 * Revision: $Id$
 * 
 *----------------------------------------------------------------------- 
 * 
 * NAME 
 * CVectorAbs
 * 
 * SYNOPSIS 
 *
 *
 * DESCRIPTION 
 * Computes the magnitudes of a complex vector.
 * 
 * DIAGNOSTICS 
 * Null pointer, invalid input size, size mismatch.
 *
 * CALLS
 * 
 * NOTES
 * 
 *----------------------------------------------------------------------- */

#include <math.h>
#include "LALStdlib.h"
#include "VectorOps.h"

NRCSID(CVECTORABSC,"$Id$");

void
CVectorAbs (
    Status               *status,
    REAL4Vector          *out,
    const COMPLEX8Vector *in
    )
{
  COMPLEX8 *a;
  REAL4    *b;
  INT4      n;

  INITSTATUS (status, "CVectorAbs", CVECTORABSC);

  ASSERT (out, status, VECTOROPS_ENULL, VECTOROPS_MSGENULL);
  ASSERT (in, status, VECTOROPS_ENULL, VECTOROPS_MSGENULL);

  ASSERT (out->data, status, VECTOROPS_ENULL, VECTOROPS_MSGENULL);
  ASSERT (in->data, status, VECTOROPS_ENULL, VECTOROPS_MSGENULL);

  ASSERT (out->length > 0, status, VECTOROPS_ESIZE, VECTOROPS_MSGESIZE);
  ASSERT (in->length == out->length, status,
          VECTOROPS_ESZMM, VECTOROPS_MSGESZMM);

  a = in->data;
  b = out->data;
  n = out->length;

  while (n-- > 0)
  {
    REAL4 ar = a->re;
    REAL4 ai = a->im;

    if (fabs(ar) > fabs(ai))
    {
      REAL4 ratio = ai/ar;
      *b=fabs(ar)*sqrt(1.0 + ratio*ratio);
    }
    else
    {
      REAL4 ratio = ar/ai;
      *b=fabs(ai)*sqrt(1.0 + ratio*ratio);
    }

    ++a;
    ++b;
  }

  RETURN (status);
}

