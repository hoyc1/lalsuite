/*
 *  Copyright (C) 2007 Jolien Creighton, B.S. Sathyaprakash, Thomas Cokelaer
 *  Copyright (C) 2012 Leo Singer
 *     Andy Lundgren, Richard O'Shaughnessy
 *  Assembled from code found in:
 *    - LALInspiralStationaryPhaseApproximation2.c
 *    - LALInspiralChooseModel.c
 *    - LALInspiralSetup.c
 *    - LALSimInspiralTaylorF2ReducedSpin.c
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

#include <stdlib.h>
#include <math.h>
#include <lal/Date.h>
#include <lal/FrequencySeries.h>
#include <lal/LALConstants.h>
#include <lal/LALDatatypes.h>
#include <lal/LALSimInspiral.h>
#include <lal/Units.h>
#include <lal/XLALError.h>
#include "LALSimInspiralPNCoefficients.c"
#include <stdio.h>


typedef struct tagLALSimInspiralSF2Coeffs
{
    REAL8 m1, m2, mtot, eta;
    REAL8 kappa, gamma0;
    REAL8 prec_fac;
    REAL8 pn_beta, pn_sigma, pn_gamma;
    REAL8 dtdv2, dtdv3, dtdv4, dtdv5;
} LALSimInspiralSF2Coeffs;

// Prototypes 

void XLALSimInspiralSF2CalculateCoeffs(
    LALSimInspiralSF2Coeffs *coeffs,
    REAL8 m1, REAL8 m2, REAL8 chi, REAL8 kappa);

REAL8 XLALSimInspiralSF2Alpha(REAL8 v, LALSimInspiralSF2Coeffs coeffs);
REAL8 XLALSimInspiralSF2Beta(REAL8 v, LALSimInspiralSF2Coeffs coeffs);
REAL8 XLALSimInspiralSF2Zeta(REAL8 v, LALSimInspiralSF2Coeffs coeffs);

COMPLEX16 XLALSimInspiralSF2Polarization(
    REAL8 thetaJ, REAL8 psiJ, int mm);

COMPLEX16 XLALSimInspiralSF2Emission(
    REAL8 beta, int mm);


void XLALSimInspiralSF2CalculateOrientation(
    LALSimInspiralSF2Orientation *orientation,
    REAL8 m1, REAL8 m2, REAL8 v_ref,
    REAL8 lnhatx, REAL8 lnhaty, REAL8 lnhatz,
    REAL8 s1x, REAL8 s1y, REAL8 s1z )
{
    orientation->v_ref = v_ref;
    const REAL8 chi1 = sqrt(s1x*s1x+s1y*s1y+s1z*s1z);
    orientation->chi = chi1;
    orientation->kappa = (chi1 > 0.) ? (lnhatx*s1x+lnhaty*s1y+lnhatz*s1z)/chi1 : 1.;
    const REAL8 Jx0 = m1*m2*lnhatx/v_ref + m1*m1*s1x;
    const REAL8 Jy0 = m1*m2*lnhaty/v_ref + m1*m1*s1y;
    const REAL8 Jz0 = m1*m2*lnhatz/v_ref + m1*m1*s1z;
    const REAL8 thetaJ = acos(Jz0 / sqrt(Jx0*Jx0+Jy0*Jy0+Jz0*Jz0));
    orientation->thetaJ = thetaJ;
    const REAL8 psiJ = atan2(Jy0, -Jx0); /* FIXME: check that Jy0 and Jx0 are not both 0 */
    orientation->psiJ = psiJ;

    /* Rotate Lnhat back to frame where J is along z, to figure out initial alpha */
    const REAL8 rotLx = lnhatx*cos(thetaJ)*cos(psiJ) - lnhaty*cos(thetaJ)*sin(psiJ) + lnhatz*sin(thetaJ);
    const REAL8 rotLy = lnhatx*sin(psiJ) + lnhaty*cos(psiJ);
    orientation->alpha0 = atan2(rotLy, rotLx); /* FIXME: check that rotLy and rotLx are not both 0 */
}

void XLALSimInspiralSF2CalculateCoeffs(
    LALSimInspiralSF2Coeffs *coeffs,
    REAL8 m1, REAL8 m2, REAL8 chi, REAL8 kappa )
{
    const REAL8 quadparam = 1.;
    coeffs->m1 = m1;
    coeffs->m2 = m2;
    const REAL8 mtot = m1+m2;
    coeffs->mtot = mtot;
    const REAL8 eta = m1*m2/(mtot*mtot);
    coeffs->eta = eta;
    coeffs->kappa = kappa;
    coeffs->gamma0 = m1*chi/m2;

    coeffs->pn_beta = (113.*m1/(12.*mtot) - 19.*eta/6.)*chi*kappa;
    coeffs->pn_sigma = (  (5.*quadparam*(3.*kappa*kappa-1.)/2.)
                           + (7. - kappa*kappa)/96.  )
                       * (m1*m1*chi*chi/mtot/mtot);
    coeffs->pn_gamma = (5.*(146597. + 7056.*eta)*m1/(2268.*mtot)
                        - 10.*eta*(1276. + 153.*eta)/81.)*chi*kappa;

    coeffs->prec_fac = 5.*(4. + 3.*m2/m1)/64.;
    coeffs->dtdv2 = 743./336. + 11.*eta/4.;
    coeffs->dtdv3 = -4.*LAL_PI + coeffs->pn_beta;
    coeffs->dtdv4 = 3058673./1016064. + 5429.*eta/1008. + 617.*eta*eta/144. - coeffs->pn_sigma;
    coeffs->dtdv5 = (-7729./672.+13.*eta/8.)*LAL_PI + 9.*coeffs->pn_gamma/40.;
}

REAL8 XLALSimInspiralSF2Alpha(
    REAL8 v, LALSimInspiralSF2Coeffs coeffs)
{
    const REAL8 gamma0 = coeffs.gamma0;
    const REAL8 gam = gamma0*v;
    const REAL8 kappa = coeffs.kappa;

    const REAL8 sqrtfac = sqrt(1. + 2.*kappa*gam + gam*gam);
    const REAL8 logv = log(v);
    const REAL8 logfac1 = log(1. + kappa*gam + sqrtfac);
    const REAL8 logfac2 = log(kappa + gam + sqrtfac);

    const REAL8 prec_fac = coeffs.prec_fac;
    const REAL8 dtdv2 = coeffs.dtdv2;
    const REAL8 dtdv3 = coeffs.dtdv3;
    const REAL8 dtdv4 = coeffs.dtdv4;
    const REAL8 dtdv5 = coeffs.dtdv5;

    return prec_fac*(logfac2*(dtdv2*gamma0*pow(1,2) + dtdv3*kappa*pow(1,3) - (dtdv5*kappa*pow(1,5)*pow(gamma0,-2))/2. + (dtdv4*pow(1,4)*pow(gamma0,-1))/2. - (dtdv4*pow(1,4)*pow(gamma0,-1)*pow(kappa,2))/2. + (dtdv5*pow(1,5)*pow(gamma0,-2)*pow(kappa,3))/2.) + logfac1*(-(dtdv2*gamma0*kappa*pow(1,2)) - dtdv3*pow(1,3) + (kappa*pow(gamma0,3))/2. - (pow(gamma0,3)*pow(kappa,3))/2.) + logv*(dtdv2*gamma0*kappa*pow(1,2) + dtdv3*pow(1,3) - (kappa*pow(gamma0,3))/2. + (pow(gamma0,3)*pow(kappa,3))/2.) + sqrtfac*(dtdv3*pow(1,3) + (dtdv4*v*pow(1,4))/2. + (dtdv5*pow(1,5)*pow(gamma0,-2))/3. + (dtdv4*kappa*pow(1,4)*pow(gamma0,-1))/2. + (dtdv5*kappa*v*pow(1,5)*pow(gamma0,-1))/6. - (dtdv5*pow(1,5)*pow(gamma0,-2)*pow(kappa,2))/2. - pow(v,-3)/3. - (gamma0*kappa*pow(v,-2))/6. - dtdv2*pow(1,2)*pow(v,-1) - (pow(gamma0,2)*pow(v,-1))/3. + (pow(gamma0,2)*pow(kappa,2)*pow(v,-1))/2. + (dtdv5*pow(1,5)*pow(v,2))/3.));
}

REAL8 XLALSimInspiralSF2Zeta(
    REAL8 v, LALSimInspiralSF2Coeffs coeffs)
{
    const REAL8 gamma0 = coeffs.gamma0;
    const REAL8 gam = gamma0*v;
    const REAL8 kappa = coeffs.kappa;

    const REAL8 sqrtfac = sqrt(1. + 2.*kappa*gam + gam*gam);
    const REAL8 logv = log(v);
    const REAL8 logfac1 = log(1. + kappa*gam + sqrtfac);
    const REAL8 logfac2 = log(kappa + gam + sqrtfac);

    const REAL8 prec_fac = coeffs.prec_fac;
    const REAL8 dtdv2 = coeffs.dtdv2;
    const REAL8 dtdv3 = coeffs.dtdv3;
    const REAL8 dtdv4 = coeffs.dtdv4;
    const REAL8 dtdv5 = coeffs.dtdv5;

    return prec_fac*(dtdv3*gamma0*kappa*v*pow(1,3) + dtdv4*v*pow(1,4) + logfac2*(-(dtdv2*gamma0*pow(1,2)) - dtdv3*kappa*pow(1,3) + (dtdv5*kappa*pow(1,5)*pow(gamma0,-2))/2. - (dtdv4*pow(1,4)*pow(gamma0,-1))/2. + (dtdv4*pow(1,4)*pow(gamma0,-1)*pow(kappa,2))/2. - (dtdv5*pow(1,5)*pow(gamma0,-2)*pow(kappa,3))/2.) + logv*((kappa*pow(gamma0,3))/2. - (pow(gamma0,3)*pow(kappa,3))/2.) + logfac1*(dtdv2*gamma0*kappa*pow(1,2) + dtdv3*pow(1,3) - (kappa*pow(gamma0,3))/2. + (pow(gamma0,3)*pow(kappa,3))/2.) - pow(v,-3)/3. - (gamma0*kappa*pow(v,-2))/2. - dtdv2*pow(1,2)*pow(v,-1) + (dtdv4*gamma0*kappa*pow(1,4)*pow(v,2))/2. + (dtdv5*pow(1,5)*pow(v,2))/2. + sqrtfac*(-(dtdv3*pow(1,3)) - (dtdv4*v*pow(1,4))/2. - (dtdv5*pow(1,5)*pow(gamma0,-2))/3. - (dtdv4*kappa*pow(1,4)*pow(gamma0,-1))/2. - (dtdv5*kappa*v*pow(1,5)*pow(gamma0,-1))/6. + (dtdv5*pow(1,5)*pow(gamma0,-2)*pow(kappa,2))/2. + pow(v,-3)/3. + (gamma0*kappa*pow(v,-2))/6. + dtdv2*pow(1,2)*pow(v,-1) + (pow(gamma0,2)*pow(v,-1))/3. - (pow(gamma0,2)*pow(kappa,2)*pow(v,-1))/2. - (dtdv5*pow(1,5)*pow(v,2))/3.) + (dtdv5*gamma0*kappa*pow(1,5)*pow(v,3))/3.);
}

REAL8 XLALSimInspiralSF2Beta(
    REAL8 v, LALSimInspiralSF2Coeffs coeffs)
{
    const REAL8 kappa = coeffs.kappa;
    const REAL8 gamma0 = coeffs.gamma0;

    return acos((1. + kappa*gamma0*v)/sqrt(1. + 2.*kappa*gamma0*v + gamma0*gamma0*v*v));
}

COMPLEX16 XLALSimInspiralSF2Polarization(
    REAL8 thetaJ, REAL8 psiJ, int mm)
{
    COMPLEX16 plus_fac, cross_fac;
    switch (mm)
    {
        case 2:
            plus_fac = (1.+cos(thetaJ)*cos(thetaJ))/2.;
            cross_fac = -1.j*cos(thetaJ);
            break;
        case 1:
            plus_fac = sin(2.*thetaJ);
            cross_fac = -2.j*sin(thetaJ);
            break;
        case 0:
            plus_fac = 3.*sin(thetaJ)*sin(thetaJ);
            cross_fac = 0.;
            break;
        case -1:
            plus_fac = -sin(2.*thetaJ);
            cross_fac = -2.j*sin(thetaJ);
            break;
        case -2:
            plus_fac = (1.+cos(thetaJ)*cos(thetaJ))/2.;
            cross_fac = 1.j*cos(thetaJ);
            break;
        default:
            plus_fac = 0.;
            cross_fac = 0.;
    }
    
    return plus_fac*cos(2.*psiJ) + cross_fac*sin(2.*psiJ);
}

COMPLEX16 XLALSimInspiralSF2Emission(
    REAL8 beta, int mm)
{
    return pow(cos(beta/2.), 2+mm) * pow(sin(beta/2.), 2-mm);
}


/**
 * Find the least nonnegative integer power of 2 that is
 * greater than or equal to n.  Inspired by similar routine
 * in gstlal.
 */
static size_t CeilPow2(double n) {
    double signif;
    int exponent;
    signif = frexp(n, &exponent);
    if (signif < 0)
        return 1;
    if (signif == 0.5)
        exponent -= 1;
    return ((size_t) 1) << exponent;
}

/* Calculate the spin corrections for TaylorF2 
        reference -> <http://arxiv.org/pdf/0810.5336v3.pdf>
*/

typedef struct {
    REAL8 beta;
    REAL8 sigma;
    REAL8 gamma;
}sf2_spin_corr;

sf2_spin_corr sf2_spin_corrections(
        const REAL8 m1,               /**< mass of companion 1 (solar masses) */
        const REAL8 m2,               /**< mass of companion 2 (solar masses) */
		const REAL8 S1z,               /**< z component of the spin of companion 1 */
		const REAL8 S2z ) ;            /**< z component of the spin of companion 2  */


sf2_spin_corr sf2_spin_corrections(
        const REAL8 m1,               /**< mass of companion 1 (solar masses) */
        const REAL8 m2,               /**< mass of companion 2 (solar masses) */
		const REAL8 S1z,               /**< z component of the spin of companion 1 */
		const REAL8 S2z )             /**< z component of the spin of companion 2  */
{
    sf2_spin_corr spin_corrections;

    REAL8 M = m1 + m2;
    REAL8 v = m1 * m2 / (M * M);
    REAL8 d = (m1 - m2) / (m1 + m2);
    REAL8 xs = .5 * (S1z + S2z);
    REAL8 xa = .5 * (S1z - S2z);

    REAL8 sf2_beta = (113.L/12.L- 19.L/3.L * v) * xs + 113.L/12.L * d * xa;
    
    REAL8 sf2_sigma = v * (721.L/48.L * (xs*xs - xa*xa)-247.L/48.L*(xs*xs - xa*xa));
    sf2_sigma += (1-2*v)* (719/96.0 * (xs*xs + xa*xa) - 233.L/96.L * (xs*xs + xa*xa));
    sf2_sigma += d * (719/48.0 * xs*xa - 233.L/48.L * xs * xa);
    
    REAL8 sf2_gamma = (732985.L/2268.L - 24260.L/81.L * v - 340.L/9.L * v * v ) * xs;
    sf2_gamma += (732985.L/2268.L +140.L/9.0L * v) * xa * d;

    spin_corrections.beta = sf2_beta;
    spin_corrections.sigma = sf2_sigma;
    spin_corrections.gamma = sf2_gamma;
    return spin_corrections;
}

/**
 * Computes a single harmonic of the SpinTaylorF2 approximation.
 */
int XLALSimInspiralSpinTaylorF2Harmonic(
	COMPLEX16FrequencySeries **hplus_out, /**< frequency-domain waveform */
	COMPLEX16FrequencySeries **hcross_out, /**< frequency-domain waveform */
	REAL8 phic,                     /**< orbital coalescence phase (rad) */
	REAL8 deltaF,                   /**< sampling frequency (Hz) */
	REAL8 m1_SI,                    /**< mass of companion 1 (kg) */
	REAL8 m2_SI,                    /**< mass of companion 2 (kg) */
	REAL8 fStart,                   /**< start GW frequency (Hz) */
	REAL8 r,                        /**< distance of source (m) */
        LALSimInspiralSF2Orientation orientation,
        int mm                          /**< harmonic number [-2..2] */
       )
{
    /* external: SI; internal: solar masses */
    const REAL8 m1 = m1_SI / LAL_MSUN_SI;
    const REAL8 m2 = m2_SI / LAL_MSUN_SI;
    const REAL8 m = m1 + m2;
    const REAL8 m_sec = m * LAL_MTSUN_SI;  /* total mass in seconds */
    const REAL8 eta = m1 * m2 / (m * m);
    const REAL8 piM = LAL_PI * m_sec;
    const REAL8 vISCO = 1. / sqrt(6.);
    const REAL8 fISCO = vISCO * vISCO * vISCO / piM;
    const REAL8 v0 = cbrt(piM * fStart);
    REAL8 shft, amp0, f_max;
    size_t i, n, iStart, iISCO;
    COMPLEX16 *data_p = NULL;
    COMPLEX16 *data_x = NULL;
    LIGOTimeGPS tC = {0, 0};

    REAL8 alpha, alpha_ref, zeta, zeta_ref, beta;
    COMPLEX16 prec_fac_p, prec_fac_x;

    /* orientation.psiJ = orientation.psiJ + psi;
     Add psi to get a different polarization if needed */

    LALSimInspiralSF2Coeffs coeffs;
    XLALSimInspiralSF2CalculateCoeffs(&coeffs, m1, m2, orientation.chi, orientation.kappa);

    alpha_ref = XLALSimInspiralSF2Alpha(v0, coeffs) - orientation.alpha0;
    zeta_ref = XLALSimInspiralSF2Zeta(v0, coeffs);
 
    COMPLEX16 SBfac_p, SBfac_x; /* complex sideband factors, both polarizations, mm=2 is first entry */
    SBfac_p = XLALSimInspiralSF2Polarization(orientation.thetaJ, orientation.psiJ, mm);
    SBfac_x = XLALSimInspiralSF2Polarization(orientation.thetaJ, orientation.psiJ + LAL_PI/4., mm);

    const REAL8 pn_beta = coeffs.pn_beta;
    const REAL8 pn_sigma = coeffs.pn_sigma;
    const REAL8 pn_gamma = coeffs.pn_gamma;

    /* phasing coefficients */
    const REAL8 pfaN = 3.L/(128.L * eta);
    const REAL8 pfa2 = 5.L*(743.L/84.L + 11.L * eta)/9.L;
    const REAL8 pfa3 = -16.L*LAL_PI + 4.L*pn_beta;
    const REAL8 pfa4 = 5.L*(3058.673L/7.056L + 5429.L/7.L * eta
                     + 617.L * eta*eta)/72.L - 10.L*pn_sigma;
    const REAL8 pfa5 = 5.L/9.L * (7729.L/84.L - 13.L * eta) * LAL_PI - pn_gamma;
    const REAL8 pfl5 = 5.L/3.L * (7729.L/84.L - 13.L * eta) * LAL_PI - pn_gamma *3.0;
    const REAL8 pfa6 = (11583.231236531L/4.694215680L - 640.L/3.L * LAL_PI * LAL_PI - 6848.L/21.L*LAL_GAMMA)
                     + eta * (-15737765635.L/3048192.L + 2255./12. * LAL_PI * LAL_PI)
                     + eta*eta * 76055.L/1728.L
                     - eta*eta*eta*  127825.L/1296.L ;
    const REAL8 pfl6 = -6848.L/21.L;
    const REAL8 pfa7 = LAL_PI * 5.L/756.L * ( 15419335.L/336.L + 75703.L/2.L * eta - 14809.L * eta*eta);

    /* flux coefficients */
    const REAL8 FTaN = XLALSimInspiralPNFlux_0PNCoeff(eta);
    const REAL8 FTa2 = XLALSimInspiralPNFlux_2PNCoeff(eta);
    const REAL8 FTa3 = XLALSimInspiralPNFlux_3PNCoeff(eta);
    const REAL8 FTa4 = XLALSimInspiralPNFlux_4PNCoeff(eta);
    const REAL8 FTa5 = XLALSimInspiralPNFlux_5PNCoeff(eta);
    const REAL8 FTl6 = XLALSimInspiralPNFlux_6PNLogCoeff(eta);
    const REAL8 FTa6 = XLALSimInspiralPNFlux_6PNCoeff(eta);
    const REAL8 FTa7 = XLALSimInspiralPNFlux_7PNCoeff(eta);

    /* energy coefficients */
    const REAL8 dETaN = 2. * XLALSimInspiralPNEnergy_0PNCoeff(eta);
    const REAL8 dETa1 = 2. * XLALSimInspiralPNEnergy_2PNCoeff(eta);
    const REAL8 dETa2 = 3. * XLALSimInspiralPNEnergy_4PNCoeff(eta);
    const REAL8 dETa3 = 4. * XLALSimInspiralPNEnergy_6PNCoeff(eta);

    COMPLEX16FrequencySeries *htilde_p;
    COMPLEX16FrequencySeries *htilde_x;

    /* Perform some initial checks */
    if (!hplus_out) XLAL_ERROR(XLAL_EFAULT);
    if (*hplus_out) XLAL_ERROR(XLAL_EFAULT);
    if (!hcross_out) XLAL_ERROR(XLAL_EFAULT);
    if (*hcross_out) XLAL_ERROR(XLAL_EFAULT);
    if (m1_SI <= 0) XLAL_ERROR(XLAL_EDOM);
    if (m2_SI <= 0) XLAL_ERROR(XLAL_EDOM);
    if (fStart <= 0) XLAL_ERROR(XLAL_EDOM);
    if (r <= 0) XLAL_ERROR(XLAL_EDOM);

    /* allocate htilde */
    f_max = CeilPow2(fISCO);
    n = f_max / deltaF + 1;
    XLALGPSAdd(&tC, -1 / deltaF);  /* coalesce at t=0 */
    htilde_p = XLALCreateCOMPLEX16FrequencySeries("htilde plus: FD waveform", &tC, 0.0, deltaF, &lalStrainUnit, n);
    if (!htilde_p) XLAL_ERROR(XLAL_EFUNC);
    htilde_x = XLALCreateCOMPLEX16FrequencySeries("htilde cross: FD waveform", &tC, 0.0, deltaF, &lalStrainUnit, n);
    if (!htilde_x) XLAL_ERROR(XLAL_EFUNC);
    memset(htilde_p->data->data, 0, n * sizeof(COMPLEX16));
    XLALUnitDivide(&htilde_p->sampleUnits, &htilde_p->sampleUnits, &lalSecondUnit);
    memset(htilde_x->data->data, 0, n * sizeof(COMPLEX16));
    XLALUnitDivide(&htilde_x->sampleUnits, &htilde_x->sampleUnits, &lalSecondUnit);

    /* extrinsic parameters */
    amp0 = -4. * m1 * m2 / r * LAL_MRSUN_SI * LAL_MTSUN_SI * sqrt(LAL_PI/12.L);
    shft = -LAL_TWOPI * (tC.gpsSeconds + 1e-9 * tC.gpsNanoSeconds);

    iStart = (size_t) ceil(fStart / deltaF);
    iISCO = (size_t) (fISCO / deltaF);
    iISCO = (iISCO < n) ? iISCO : n;  /* overflow protection; should we warn? */
    data_p = htilde_p->data->data;
    data_x = htilde_x->data->data;
    for (i = iStart; i < iISCO; i++) {
        const REAL8 f = i * deltaF;
        const REAL8 v = cbrt(piM*f);
        const REAL8 v2 = v * v;
        const REAL8 v3 = v * v2;
        const REAL8 v4 = v * v3;
        const REAL8 v5 = v * v4;
        const REAL8 v6 = v * v5;
        const REAL8 v7 = v * v6;
        const REAL8 v8 = v * v7;
        const REAL8 v9 = v * v8;
        const REAL8 v10 = v * v9;
        REAL8 phasing = 0.;
        REAL8 dEnergy = 0.;
        REAL8 flux = 0.;
        REAL8 amp;

        /* Let's not bother with phasings other than best-known */
        phasing += pfa7 * v7;
        phasing += (pfa6 + pfl6 * log(4.*v) ) * v6;
        phasing += (pfa5 + pfl5 * log(v/v0)) * v5;
        phasing += pfa4 * v4;
        phasing += pfa3 * v3;
        phasing += pfa2 * v2;
        phasing += 1.;

        switch (0) /* Also turn off amplitude corrections */
        {
            case -1:
            case 7:
                flux += FTa7 * v7;
            case 6:
                flux += (FTa6 + FTl6*log(16.*v2)) * v6;
                dEnergy += dETa3 * v6;
            case 5:
                flux += FTa5 * v5;
            case 4:
                flux += FTa4 * v4;
                dEnergy += dETa2 * v4;
            case 3:
                flux += FTa3 * v3;
            case 2:
                flux += FTa2 * v2;
                dEnergy += dETa1 * v2;
            case 0:
                flux += 1.;
                dEnergy += 1.;
                break;
            default:
                XLALDestroyCOMPLEX16FrequencySeries(htilde_p);
                XLALDestroyCOMPLEX16FrequencySeries(htilde_x);
                XLAL_ERROR(XLAL_ETYPE);
        }
        phasing *= pfaN / v5;
        flux *= FTaN * v10;
        dEnergy *= dETaN * v;

        alpha = XLALSimInspiralSF2Alpha(v, coeffs) - alpha_ref;
        beta = XLALSimInspiralSF2Beta(v, coeffs);
        zeta = XLALSimInspiralSF2Zeta(v, coeffs) - zeta_ref;

        prec_fac_p =
                /* XLALSimInspiralSF2Emission(beta, mm) * Remove this factor */
                SBfac_p
                * ( cos( (mm-2.) * alpha) + sin( (mm-2.) * alpha)*1.0j );
        prec_fac_x =
                /* XLALSimInspiralSF2Emission(beta, mm) * Remove this factor */
                SBfac_x
                * ( cos( (mm-2.) * alpha) + sin( (mm-2.) * alpha)*1.0j );

        // Note the factor of 2 b/c phic is orbital phase
        phasing += shft * f - 2.*phic - LAL_PI_4;
        phasing += 2.*zeta;
        amp = amp0 * sqrt(-dEnergy/flux) * v;
        data_p[i] = ((COMPLEX16)amp)*prec_fac_p*(cos(phasing) - sin(phasing) * 1.0j);
        data_x[i] = ((COMPLEX16)amp)*prec_fac_x*(cos(phasing) - sin(phasing) * 1.0j);
    }

    *hplus_out = htilde_p;
    *hcross_out = htilde_x;
    return XLAL_SUCCESS;
}



/**
 * Computes the stationary phase approximation to the Fourier transform of
 * a chirp waveform with phase given by \eqref{eq_InspiralFourierPhase_f2}
 * and amplitude given by expanding \f$1/\sqrt{\dot{F}}\f$. If the PN order is
 * set to -1, then the highest implemented order is used.
 * \author B.S. Sathyaprakash
 */
int XLALSimInspiralSpinTaylorF2(
	COMPLEX16FrequencySeries **htilde_out, /**< frequency-domain waveform */
	REAL8 psi,                      /**< desired polarization */
	REAL8 phic,                     /**< orbital coalescence phase (rad) */
	REAL8 deltaF,                   /**< sampling frequency (Hz) */
	REAL8 m1_SI,                    /**< mass of companion 1 (kg) */
	REAL8 m2_SI,                    /**< mass of companion 2 (kg) */
	REAL8 fStart,                   /**< start GW frequency (Hz) */
	REAL8 r,                        /**< distance of source (m) */
	REAL8 s1x,                      /**< initial value of S1x */
	REAL8 s1y,                      /**< initial value of S1y */
	REAL8 s1z,                      /**< initial value of S1z */
	REAL8 lnhatx,                   /**< initial value of LNhatx */
	REAL8 lnhaty,                   /**< initial value of LNhaty */
	REAL8 lnhatz,                   /**< initial value of LNhatz */
	int phaseO,                     /**< twice PN phase order */
	int amplitudeO                  /**< twice PN amplitude order */
       )
{
    const REAL8 lambda = -1987./3080.;
    const REAL8 theta = -11831./9240.;

    /* external: SI; internal: solar masses */
    const REAL8 m1 = m1_SI / LAL_MSUN_SI;
    const REAL8 m2 = m2_SI / LAL_MSUN_SI;
    const REAL8 m = m1 + m2;
    const REAL8 m_sec = m * LAL_MTSUN_SI;  /* total mass in seconds */
    const REAL8 eta = m1 * m2 / (m * m);
    const REAL8 piM = LAL_PI * m_sec;
    const REAL8 vISCO = 1. / sqrt(6.);
    const REAL8 fISCO = vISCO * vISCO * vISCO / piM;
    const REAL8 v0 = cbrt(piM * fStart);
    REAL8 shft, amp0, f_max;
    size_t i, n, iStart, iISCO;
    int mm;
    COMPLEX16 *data = NULL;
    LIGOTimeGPS tC = {0, 0};

    REAL8 alpha, alpha_ref, zeta, zeta_ref, beta;
    COMPLEX16 prec_fac;

    LALSimInspiralSF2Orientation orientation;
    XLALSimInspiralSF2CalculateOrientation(&orientation, m1, m2, v0, lnhatx, lnhaty, lnhatz, s1x, s1y, s1z);

    orientation.psiJ = orientation.psiJ + psi;

    /* fprintf(stdout, "thetaJ = %f\n", orientation.thetaJ * 180./LAL_PI);
    fprintf(stdout, "psiJ = %f\n", orientation.psiJ * 180./LAL_PI);
    fprintf(stdout, "alpha0 = %f\n", orientation.alpha0 * 180./LAL_PI);
    fprintf(stdout, "chi = %f\n", orientation.chi);
    fprintf(stdout, "kappa = %f\n", orientation.kappa);
    fprintf(stdout, "v0 = %f\n", v0); */


    LALSimInspiralSF2Coeffs coeffs;
    XLALSimInspiralSF2CalculateCoeffs(&coeffs, m1, m2, orientation.chi, orientation.kappa);

    alpha_ref = XLALSimInspiralSF2Alpha(v0, coeffs) - orientation.alpha0;
    zeta_ref = XLALSimInspiralSF2Zeta(v0, coeffs);
 
    COMPLEX16 SBfac[5]; /* complex sideband factors, mm=2 is first entry */
    for(mm = -2; mm <= 2; mm++)
    {
        SBfac[2-mm] = XLALSimInspiralSF2Polarization(orientation.thetaJ, orientation.psiJ, mm);
        // fprintf(stdout, "m = %i, %f, %f\n", mm, creal(SBfac[2-mm]), cimag(SBfac[2-mm]));
    }

    const REAL8 pn_beta = coeffs.pn_beta;
    const REAL8 pn_sigma = coeffs.pn_sigma;
    const REAL8 pn_gamma = coeffs.pn_gamma;

    /* phasing coefficients */
    const REAL8 pfaN = 3.L/(128.L * eta);
    const REAL8 pfa2 = 5.L*(743.L/84.L + 11.L * eta)/9.L;
    const REAL8 pfa3 = -16.L*LAL_PI + 4.L*pn_beta;
    const REAL8 pfa4 = 5.L*(3058.673L/7.056L + 5429.L/7.L * eta
                     + 617.L * eta*eta)/72.L - 10.L*pn_sigma;
    const REAL8 pfa5 = 5.L/9.L * (7729.L/84.L - 13.L * eta) * LAL_PI - pn_gamma;
    const REAL8 pfl5 = 5.L/3.L * (7729.L/84.L - 13.L * eta) * LAL_PI - pn_gamma *3.0;
    const REAL8 pfa6 = (11583.231236531L/4.694215680L - 640.L/3.L * LAL_PI * LAL_PI - 6848.L/21.L*LAL_GAMMA)
                     + eta * (-15335.597827L/3.048192L + 2255./12. * LAL_PI * LAL_PI - 1760./3.*theta +12320./9.*lambda)
                     + eta*eta * 76055.L/1728.L
                     - eta*eta*eta*  127825.L/1296.L ;
    const REAL8 pfl6 = -6848.L/21.L;
    const REAL8 pfa7 = LAL_PI * 5.L/756.L * ( 15419335.L/336.L + 75703.L/2.L * eta - 14809.L * eta*eta);

    /* flux coefficients */
    const REAL8 FTaN = XLALSimInspiralPNFlux_0PNCoeff(eta);
    const REAL8 FTa2 = XLALSimInspiralPNFlux_2PNCoeff(eta);
    const REAL8 FTa3 = XLALSimInspiralPNFlux_3PNCoeff(eta);
    const REAL8 FTa4 = XLALSimInspiralPNFlux_4PNCoeff(eta);
    const REAL8 FTa5 = XLALSimInspiralPNFlux_5PNCoeff(eta);
    const REAL8 FTl6 = XLALSimInspiralPNFlux_6PNLogCoeff(eta);
    const REAL8 FTa6 = XLALSimInspiralPNFlux_6PNCoeff(eta);
    const REAL8 FTa7 = XLALSimInspiralPNFlux_7PNCoeff(eta);

    /* energy coefficients */
    const REAL8 dETaN = 2. * XLALSimInspiralPNEnergy_0PNCoeff(eta);
    const REAL8 dETa1 = 2. * XLALSimInspiralPNEnergy_2PNCoeff(eta);
    const REAL8 dETa2 = 3. * XLALSimInspiralPNEnergy_4PNCoeff(eta);
    const REAL8 dETa3 = 4. * XLALSimInspiralPNEnergy_6PNCoeff(eta);

    COMPLEX16FrequencySeries *htilde;

    /* Perform some initial checks */
    if (!htilde_out) XLAL_ERROR(XLAL_EFAULT);
    if (*htilde_out) XLAL_ERROR(XLAL_EFAULT);
    if (m1_SI <= 0) XLAL_ERROR(XLAL_EDOM);
    if (m2_SI <= 0) XLAL_ERROR(XLAL_EDOM);
    if (fStart <= 0) XLAL_ERROR(XLAL_EDOM);
    if (r <= 0) XLAL_ERROR(XLAL_EDOM);

    /* allocate htilde */
    f_max = CeilPow2(fISCO);
    n = f_max / deltaF + 1;
    XLALGPSAdd(&tC, -1 / deltaF);  /* coalesce at t=0 */
    htilde = XLALCreateCOMPLEX16FrequencySeries("htilde: FD waveform", &tC, 0.0, deltaF, &lalStrainUnit, n);
    if (!htilde) XLAL_ERROR(XLAL_EFUNC);
    memset(htilde->data->data, 0, n * sizeof(COMPLEX16));
    XLALUnitDivide(&htilde->sampleUnits, &htilde->sampleUnits, &lalSecondUnit);

    /* extrinsic parameters */
    amp0 = -4. * m1 * m2 / r * LAL_MRSUN_SI * LAL_MTSUN_SI * sqrt(LAL_PI/12.L);
    shft = -LAL_TWOPI * (tC.gpsSeconds + 1e-9 * tC.gpsNanoSeconds);

    iStart = (size_t) ceil(fStart / deltaF);
    iISCO = (size_t) (fISCO / deltaF);
    iISCO = (iISCO < n) ? iISCO : n;  /* overflow protection; should we warn? */
    data = htilde->data->data;
    for (i = iStart; i < iISCO; i++) {
        const REAL8 f = i * deltaF;
        const REAL8 v = cbrt(piM*f);
        const REAL8 v2 = v * v;
        const REAL8 v3 = v * v2;
        const REAL8 v4 = v * v3;
        const REAL8 v5 = v * v4;
        const REAL8 v6 = v * v5;
        const REAL8 v7 = v * v6;
        const REAL8 v8 = v * v7;
        const REAL8 v9 = v * v8;
        const REAL8 v10 = v * v9;
        REAL8 phasing = 0.;
        REAL8 dEnergy = 0.;
        REAL8 flux = 0.;
        REAL8 amp;

        switch (phaseO)
        {
            case -1:
            case 7:
                phasing += pfa7 * v7;
            case 6:
                phasing += (pfa6 + pfl6 * log(4.*v) ) * v6;
            case 5:
                phasing += (pfa5 + pfl5 * log(v/v0)) * v5;
            case 4:
                phasing += pfa4 * v4;
            case 3:
                phasing += pfa3 * v3;
            case 2:
                phasing += pfa2 * v2;
            case 0:
                phasing += 1.;
                break;
            default:
                XLALDestroyCOMPLEX16FrequencySeries(htilde);
                XLAL_ERROR(XLAL_ETYPE);
        }
        switch (amplitudeO)
        {
            case -1:
            case 7:
                flux += FTa7 * v7;
            case 6:
                flux += (FTa6 + FTl6*log(16.*v2)) * v6;
                dEnergy += dETa3 * v6;
            case 5:
                flux += FTa5 * v5;
            case 4:
                flux += FTa4 * v4;
                dEnergy += dETa2 * v4;
            case 3:
                flux += FTa3 * v3;
            case 2:
                flux += FTa2 * v2;
                dEnergy += dETa1 * v2;
            case 0:
                flux += 1.;
                dEnergy += 1.;
                break;
            default:
                XLALDestroyCOMPLEX16FrequencySeries(htilde);
                XLAL_ERROR(XLAL_ETYPE);
        }
        phasing *= pfaN / v5;
        flux *= FTaN * v10;
        dEnergy *= dETaN * v;

        alpha = XLALSimInspiralSF2Alpha(v, coeffs) - alpha_ref;
        beta = XLALSimInspiralSF2Beta(v, coeffs);
        zeta = XLALSimInspiralSF2Zeta(v, coeffs) - zeta_ref;

        prec_fac = 0.j;
        for(mm = -2; mm <= 2; mm++)
        {
            prec_fac += 
                XLALSimInspiralSF2Emission(beta, mm)
                * SBfac[2-mm]
                * ( cos( (mm-2.) * alpha) + sin( (mm-2.) * alpha)*1.0j );
        }

        // Note the factor of 2 b/c phic is orbital phase
        phasing += shft * f - 2.*phic;
        phasing += 2.*zeta;
        amp = amp0 * sqrt(-dEnergy/flux) * v;
        data[i] = ((COMPLEX16)amp)*prec_fac*(cos(phasing - LAL_PI_4) - sin(phasing - LAL_PI_4) * 1.0j);
    }

    *htilde_out = htilde;
    return XLAL_SUCCESS;
}


// BEWARE: These frequency series are centered with zero frequency at the CENTER
SphHarmFrequencySeries*  XLALSimInspiralSpinTaylorF2Modes(
	REAL8 phiRef,                   /**< reference orbital phase (rad) */
	REAL8 deltaF,                   /**< sampling interval (Hz) */
	REAL8 m1_SI,                       /**< mass of companion 1 (kg) */
	REAL8 m2_SI,                       /**< mass of companion 2 (kg) */
	REAL8 S1x,                      /**< x-component of the dimensionless spin of object 1*/
	REAL8 S1y,                      /**< y-component of the dimensionless spin of object 1 */
	REAL8 S1z,                      /**< z-component of the dimensio
	nless spin of object 1 */
	REAL8 lnhatx,                   /**< x-component of the initial angular momentum direction */
	REAL8 lnhaty,                   /**< y-component of the initial angular momentum direction */
	REAL8 lnhatz,                   /**< z-component of the initial angular momentum direction */
	REAL8 f_min,                    /**< starting GW frequency (Hz) */
	REAL8 f_ref,
	REAL8 f_max,                    /**< ending GW frequency (Hz) */
	REAL8 r,                        /**< distance of source (m) */
	int amplitudeO,                 /**< twice post-Newtonian amplitude order */
	int phaseO,                     /**< twice post-Newtonian order */
	int lmax                 /**< generate all modes w
ith l <= lmax */
	){

    /* Stupid constants, these should be built into the phasing */
    const REAL8 lambda = -1987./3080.;
    const REAL8 theta = -11831./9240.;

    SphHarmFrequencySeries* hlm;
    LALSimInspiralSF2Orientation * orientation;
    const REAL8 m1 = m1_SI / LAL_MSUN_SI;
    const REAL8 m2 = m2_SI / LAL_MSUN_SI;
    const REAL8 mt = m1 + m2;
    const REAL8 eta = m1*m2/mt/mt;
    const REAL8 m_sec = mt * LAL_MTSUN_SI;  /* total mass in seconds */
    const REAL8 piM = LAL_PI * m_sec;
    const REAL8 v0 = cbrt(piM * f_min);
    const REAL8 fISCO = 6.*sqrt(6.)/piM;
    REAL8 shft, amp0;
    size_t i, n, iStart, iISCO;
    LIGOTimeGPS tC = {0, 0};

    int m;

  // Convert orientation into our frame
  const REAL8 v_ref = cbrt(piM * f_ref); // Fixme: (pi M f)^(1/3)
  XLALSimInspiralSF2CalculateOrientation(orientation, m1, m2, v_ref, lnhatx, lnhaty, lnhatz, S1x,S1y,S1z);

  // Get coefficients of frequency expansion
    LALSimInspiralSF2Coeffs coeffs;
    XLALSimInspiralSF2CalculateCoeffs(&coeffs, m1, m2, orientation->chi, orientation->kappa);


    const REAL8 pn_beta = coeffs.pn_beta;
    const REAL8 pn_sigma = coeffs.pn_sigma;
    const REAL8 pn_gamma = coeffs.pn_gamma;

    /* phasing coefficients */
    const REAL8 pfaN = 3.L/(128.L * eta);
    const REAL8 pfa2 = 5.L*(743.L/84.L + 11.L * eta)/9.L;
    const REAL8 pfa3 = -16.L*LAL_PI + 4.L*pn_beta;
    const REAL8 pfa4 = 5.L*(3058.673L/7.056L + 5429.L/7.L * eta
                     + 617.L * eta*eta)/72.L - 10.L*pn_sigma;
    const REAL8 pfa5 = 5.L/9.L * (7729.L/84.L - 13.L * eta) * LAL_PI - pn_gamma;
    const REAL8 pfl5 = 5.L/3.L * (7729.L/84.L - 13.L * eta) * LAL_PI - pn_gamma *3.0;
    const REAL8 pfa6 = (11583.231236531L/4.694215680L - 640.L/3.L * LAL_PI * LAL_PI - 6848.L/21.L*LAL_GAMMA)
                     + eta * (-15335.597827L/3.048192L + 2255./12. * LAL_PI * LAL_PI - 1760./3.*theta +12320./9.*lambda)
                     + eta*eta * 76055.L/1728.L
                     - eta*eta*eta*  127825.L/1296.L ;
    const REAL8 pfl6 = -6848.L/21.L;
    const REAL8 pfa7 = LAL_PI * 5.L/756.L * ( 15419335.L/336.L + 75703.L/2.L * eta - 14809.L * eta*eta);

    /* flux coefficients */
    const REAL8 FTaN = XLALSimInspiralPNFlux_0PNCoeff(eta);
    const REAL8 FTa2 = XLALSimInspiralPNFlux_2PNCoeff(eta);
    const REAL8 FTa3 = XLALSimInspiralPNFlux_3PNCoeff(eta);
    const REAL8 FTa4 = XLALSimInspiralPNFlux_4PNCoeff(eta);
    const REAL8 FTa5 = XLALSimInspiralPNFlux_5PNCoeff(eta);
    const REAL8 FTl6 = XLALSimInspiralPNFlux_6PNLogCoeff(eta);
    const REAL8 FTa6 = XLALSimInspiralPNFlux_6PNCoeff(eta);
    const REAL8 FTa7 = XLALSimInspiralPNFlux_7PNCoeff(eta);

    /* energy coefficients */
    const REAL8 dETaN = 2. * XLALSimInspiralPNEnergy_0PNCoeff(eta);
    const REAL8 dETa1 = 2. * XLALSimInspiralPNEnergy_2PNCoeff(eta);
    const REAL8 dETa2 = 3. * XLALSimInspiralPNEnergy_4PNCoeff(eta);
    const REAL8 dETa3 = 4. * XLALSimInspiralPNEnergy_6PNCoeff(eta);

    /* extrinsic parameters */
    amp0 = -4. * m1 * m2 / r * LAL_MRSUN_SI * LAL_MTSUN_SI * sqrt(LAL_PI/12.L);
    shft = -LAL_TWOPI * (tC.gpsSeconds + 1e-9 * tC.gpsNanoSeconds); 

    if (f_max < f_min) { f_max = CeilPow2(fISCO); }
    n = 2*(f_max / deltaF + 1);
    XLALGPSAdd(&tC, -1 / deltaF);  /* coalesce at t=0 */

    // Create a (2l+1)=5-dimensional array of Complex16FrequencySeries to hold the modes in the COROTATING frame. (This will be converted in place to the FD by a rotation operator)
    COMPLEX16FrequencySeries * hTildeArray[5];
    REAL8FrequencySeries * betaSeries, *alphaSeries, *zetaSeries;
       for (m=-2; m<=2; m++) {
          COMPLEX16FrequencySeries *htilde;
          if (!htilde_out) XLAL_ERROR(XLAL_EFAULT);
          if (*htilde_out) XLAL_ERROR(XLAL_EFAULT);

          /* allocate htilde */
          htilde = XLALCreateCOMPLEX16FrequencySeries("htilde: FD waveform", &tC, 0.0, deltaF, &lalStrainUnit, n);
          if (!htilde) XLAL_ERROR(XLAL_EFUNC);
          memset(htilde->data->data, 0, n * sizeof(COMPLEX16));
          XLALUnitDivide(&htilde->sampleUnits, &htilde->sampleUnits, &lalSecondUnit);

          hTildeArray[m+2] = htilde;
    }
    alphaSeries = XLALCreateCOMPLEX16FrequencySeries("alpha(f)", &tC, 0.0, deltaF, &lalStrainUnit, n);
    betaSeries = XLALCreateCOMPLEX16FrequencySeries("beta(f)", &tC, 0.0, deltaF, &lalStrainUnit, n);
    zetaSeries = XLALCreateCOMPLEX16FrequencySeries("zeta(f)", &tC, 0.0, deltaF, &lalStrainUnit, n);
    memset(alphaSeries->data->data, 0, n * sizeof(COMPLEX16));
    memset(betaSeries->data->data, 0, n * sizeof(COMPLEX16));
    memset(zetaSeries->data->data, 0, n * sizeof(COMPLEX16));
    


    // Loop over the frequency bins
    iStart = (size_t) ceil(fStart / deltaF);
    iISCO = (size_t) (fISCO / deltaF);
    iISCO = (iISCO < n) ? iISCO : n;  /* overflow protection; should we warn? */
    data = htilde->data->data;
    int nMid = n/2;
    for (i = iStart; i < iISCO; i++) {
      const REAL8 f = i * deltaF;
      const REAL8 v = cbrt(piM*f);
      const REAL8 v2 = v * v;
      const REAL8 v3 = v * v2;
      const REAL8 v4 = v * v3;
      const REAL8 v5 = v * v4;
      const REAL8 v6 = v * v5;
      const REAL8 v7 = v * v6;
      const REAL8 v8 = v * v7;
      const REAL8 v9 = v * v8;
      const REAL8 v10 = v * v9;
      REAL8 phasing = 0.;
      REAL8 dEnergy = 0.;
      REAL8 flux = 0.;
      REAL8 amp;

      switch (phaseO)
        {
        case -1:
        case 7:
          phasing += pfa7 * v7;
        case 6:
          phasing += (pfa6 + pfl6 * log(4.*v) ) * v6;
        case 5:
          phasing += (pfa5 + pfl5 * log(v/v0)) * v5;
        case 4:
          phasing += pfa4 * v4;
        case 3:
          phasing += pfa3 * v3;
        case 2:
          phasing += pfa2 * v2;
        case 0:
          phasing += 1.;
          break;
        default:
          XLALDestroyCOMPLEX16FrequencySeries(htilde);
          XLAL_ERROR(XLAL_ETYPE);
        }
      switch (amplitudeO)
        {
        case -1:
        case 7:
          flux += FTa7 * v7;
        case 6:
          flux += (FTa6 + FTl6*log(16.*v2)) * v6;
          dEnergy += dETa3 * v6;
        case 5:
          flux += FTa5 * v5;
        case 4:
          flux += FTa4 * v4;
          dEnergy += dETa2 * v4;
        case 3:
          flux += FTa3 * v3;
        case 2:
          flux += FTa2 * v2;
          dEnergy += dETa1 * v2;
        case 0:
          flux += 1.;
          dEnergy += 1.;
          break;
        default:
          XLALDestroyCOMPLEX16FrequencySeries(htilde);
          XLAL_ERROR(XLAL_ETYPE);
        }
      phasing *= pfaN / v5;
      flux *= FTaN * v10;
      dEnergy *= dETaN * v;
      
      

      // Note the factor of 2 b/c phic is orbital phase
      phasing += shft * f - 2.*phic;
      phasing += 2.*zeta;
      amp = amp0 * sqrt(-dEnergy/flux) * v;

      // Be VERY VERY CAREFUL here, to make sure I am populating everything correctly
      hTildeArray[4].data.data[nMid+i-1] = ((COMPLEX16)amp)*(cos(phasing - LAL_PI_4) - sin(phasing - LAL_PI_4) * 1.0j);  //m =-2 in corotating frame
      hTildeArray[0].data.data[nMin-i+1] = conj(((COMPLEX16)amp)*(cos(phasing - LAL_PI_4) - sin(phasing - LAL_PI_4) * 1.0j); // m=2 in corotating frame

      alphaSeries.data.data[nMid+i-1] = XLALSimInspiralSF2Alpha(v, coeffs) - alpha_ref;
      alphaSeries.data.data[nMid-i+1] = XLALSimInspiralSF2Alpha(v, coeffs) - alpha_ref;
      betaSeries.data.data[nMid+i-1] = XLALSimInspiralSF2Beta(v, coeffs);
      betaSeries.data.data[nMid-i+1] = XLALSimInspiralSF2Beta(v, coeffs);
      zetaSeries.data.data[nMid+i-1] = XLALSimInspiralSF2Zeta(v, coeffs) - zeta_ref;
      zetaSeries.data.data[nMid-i+1] = XLALSimInspiralSF2Zeta(v, coeffs) - zeta_ref;

    }
        // add the waveforms to an hlm object, after the loop to populate the arrays is finished
       for (m=-2; m<=2; m++{
           hlm  = XLALSphHarmFrequencySeriesAddMode(hlm, hTildeArray[m+2], l, m);
         }
              // Perform the rotation operation. Using a black box
              XLALSimInspiralPrecessionRotateModesFD(hlm, alpha, beta, gam);
  return hlm;
}
