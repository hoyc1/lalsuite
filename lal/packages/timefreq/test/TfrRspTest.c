/*-----------------------------------------------------------------------
 * 
 * File Name: TfrRspTest.c
 * 
 * Author: Chassande-Mottin, E.
 * 
 * Revision: $Id: 
 * 
 *----------------------------------------------------------------------- 
 * 
 * NAME 
 *   main()
 *
 * SYNOPSIS 
 * 
 * DESCRIPTION 
 *   Compute the reassigned spectrogram of a test signal
 *   Test of TfrRsp.c
 * 
 * DIAGNOSTICS
 * 
 * CALLS
 * 
 * NOTES
 * 
 *----------------------------------------------------------------------- */


#include "TimeFreq.h"

int debuglevel=2;

int main(void)
{
  const INT4 Nsignal=16;
  const INT4 Nwindow=5;
  const INT4 Nfft=8;

  static Status status;

  REAL4Vector  *signal = NULL;
  CreateTimeFreqIn tfrIn;
  TimeFreqRep  *tfr = NULL; 
  TimeFreqParam *param = NULL;

  INT4 column;
  INT4 row;

  /*--------------------------------------------------------------------*/

  SCreateVector(&status, &signal, Nsignal);

  /*   signal->data[0]=1.0; */
  for (column = 0; column < signal->length; column++)
    signal->data[column]=(rand() % 10) / 2.0;
  
  /*     signal->data[column] = 1.0 - signal->data[column-1]; */
  /*     signal->data[column] = 1.0; */
  /*     signal->data[column]=(rand() % 10) / 2.0; */


  /*--------------------------------------------------------------------*/

  tfrIn.type=RSpectrogram;
  tfrIn.fRow=Nfft;              
  tfrIn.tCol=Nsignal; 
  tfrIn.wlengthT=Nwindow;
  tfrIn.wlengthF=0;

  /*--------------------------------------------------------------------*/

  CreateTimeFreqRep(&status, &tfr, &tfrIn);

  for (column = 0; column < tfr->tCol; column++)
    tfr->timeInstant[column]=column;    

  CreateTimeFreqParam(&status, &param, &tfrIn);

  for (column = 0; column < param->windowT->length; column++)
    param->windowT->data[column]=1.0;    

  /*   for (column = 0; column < param->windowF->length; column++) */
  /*     param->windowF->data[column]=1.0;     */

  /*--------------------------------------------------------------------*/

  TfrRsp(&status,signal,tfr,param);
  REPORTSTATUS(&status);

  /*--------------------------------------------------------------------*/

  printf("Signal:\n");
  for (column= 0; column < signal->length; column++)
    printf("%1.1f ",signal->data[column]);
  printf("\n\n");

  printf("TFR:\n");
  for (row= 0; row < (tfr->fRow/2+1); row++)
    {
    for (column= 0; column < tfr->tCol; column++)
      printf("%2.2f ",tfr->map[column][row]);
    printf("\n");
    }

  /*--------------------------------------------------------------------*/

  SDestroyVector(&status,&signal);
  DestroyTimeFreqRep(&status,&tfr);
  DestroyTimeFreqParam(&status,&param);

  LALCheckMemoryLeaks();

  return 0;
}

