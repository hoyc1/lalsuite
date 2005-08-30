#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "global.h"
#include "grid.h"

extern FILE *LOG;

extern double average_det_velocity[3];
extern double band_axis_norm;
extern double band_axis[3];

SKY_GRID_TYPE spherical_distance(SKY_GRID_TYPE ra0, SKY_GRID_TYPE dec0,
			  SKY_GRID_TYPE ra1, SKY_GRID_TYPE dec1)
{
SKY_GRID_TYPE ds;
ds=acos(sin(dec0)*sin(dec1)+cos(dec0)*cos(dec1)*cos(ra0-ra1));
return ds;
}

SKY_GRID_TYPE fast_spherical_distance(SKY_GRID_TYPE ra0, SKY_GRID_TYPE dec0,
			  SKY_GRID_TYPE ra1, SKY_GRID_TYPE dec1)
{
SKY_GRID_TYPE ds;
ds=1.0-(sin(dec0)*sin(dec1)+cos(dec0)*cos(dec1)*cos(ra0-ra1));
return ds;
}

/* Precompute values that are used later */
void precompute_values(SKY_GRID *grid)
{
long k;
SKY_GRID_TYPE e2,e3,e4,e5;
for(k=0;k<grid->npoints;k++){
		e2=cos(M_PI_2-grid->latitude[k]);
		e3=sin(M_PI_2-grid->latitude[k]);
		e4=cos(grid->longitude[k]);
		e5=sin(grid->longitude[k]);
		/* unit vector */
		grid->e[0][k]=e3*e4;
		grid->e[1][k]=e3*e5;
		grid->e[2][k]=e2;
		/* other useful values */
		grid->e[3][k]=e3;
		grid->e[4][k]=e4;
		grid->e[5][k]=e5;
		/* these values are needed for regression of plus and cross */
		grid->e[6][k]=e4*e5;
		grid->e[7][k]=e3*e4;
		grid->e[8][k]=e3*e5;
		
		grid->e[9][k]=e2*e2*e4;
		grid->e[10][k]=e2*e2*e5;
		grid->e[11][k]=e3*e3*e4;
		grid->e[12][k]=e3*e3*e5;
		grid->e[13][k]=e2*e3*e4;
		grid->e[14][k]=e2*e3*e5;

		grid->e[15][k]=e2*e4*e4;
		grid->e[16][k]=e2*e5*e5;
		grid->e[17][k]=e3*e4*e4;
		grid->e[18][k]=e3*e5*e5;
		grid->e[19][k]=e2*e4*e5;
		
		grid->e[20][k]=e2*e2*e4*e4;
		grid->e[21][k]=e2*e2*e5*e5;
		grid->e[22][k]=e3*e3*e4*e4;
		grid->e[23][k]=e3*e3*e5*e5;

		grid->e[24][k]=e2*e2*e4*e5;
		grid->e[25][k]=e3*e3*e4*e5;
	}

}

SKY_GRID *make_arcsin_grid(long num_ra, long num_dec)
{
SKY_GRID *grid;
RECT_SKY_GRID_PRIV *priv;
long i,j,k;
SKY_GRID_TYPE a,b;

/* set all up */
grid=do_alloc(1,sizeof(*grid));
grid->npoints=num_ra*num_dec;
grid->max_n_dec=num_dec;
grid->max_n_ra=num_ra;
grid->name="arcsin rectangular";
grid->latitude=do_alloc(grid->npoints, sizeof(SKY_GRID_TYPE));
grid->longitude=do_alloc(grid->npoints, sizeof(SKY_GRID_TYPE));
grid->band=do_alloc(grid->npoints, sizeof(*grid->band));
grid->band_f=do_alloc(grid->npoints, sizeof(*grid->band_f));
for(i=0;i<GRID_E_COUNT;i++)
	grid->e[i]=do_alloc(grid->npoints, sizeof(SKY_GRID_TYPE));
priv=do_alloc(1, sizeof(*priv));
priv->num_ra=num_ra;
priv->num_dec=num_dec;
grid->grid_priv=priv;

/* fill in the coordinates */
for(i=0;i<num_ra;i++){
	a=M_PI*(1.0+2.0*i)/num_ra;
	for(j=0;j<num_dec;j++){
		k=i*num_dec+j;
		b=asin(-1.0+(1.0+2.0*j)/num_dec);
		grid->latitude[k]=b;
		grid->longitude[k]=a;
		}
	}
precompute_values(grid);
return grid;
}

SKY_GRID *make_rect_grid(long num_ra, long num_dec)
{
SKY_GRID *grid;
RECT_SKY_GRID_PRIV *priv;
long i,j,k;
SKY_GRID_TYPE a,b;

/* set all up */
grid=do_alloc(1,sizeof(*grid));
grid->npoints=num_ra*num_dec;
grid->max_n_dec=num_dec;
grid->max_n_ra=num_ra;
grid->name="plain rectangular";
grid->latitude=do_alloc(grid->npoints, sizeof(SKY_GRID_TYPE));
grid->longitude=do_alloc(grid->npoints, sizeof(SKY_GRID_TYPE));
grid->band=do_alloc(grid->npoints, sizeof(*grid->band));
grid->band_f=do_alloc(grid->npoints, sizeof(*grid->band_f));
for(i=0;i<GRID_E_COUNT;i++)
	grid->e[i]=do_alloc(grid->npoints, sizeof(SKY_GRID_TYPE));
priv=do_alloc(1, sizeof(*priv));
priv->num_ra=num_ra;
priv->num_dec=num_dec;
grid->grid_priv=priv;

/* fill in the coordinates */
for(i=0;i<num_ra;i++){
	a=M_PI*(1.0+2.0*i)/num_ra;
	for(j=0;j<num_dec;j++){
		k=i*num_dec+j;
		b=M_PI_2*(-1.0+(1.0+2.0*j)/num_dec);
		grid->latitude[k]=b;
		grid->longitude[k]=a;
		}
	}
precompute_values(grid);
return grid;
}

SKY_GRID *make_sin_theta_grid(SKY_GRID_TYPE resolution)
{
SKY_GRID *grid;
SIN_THETA_SKY_GRID_PRIV *priv;
long i,j,k;
SKY_GRID_TYPE a,b;

/* set all up */
grid=do_alloc(1,sizeof(*grid));
grid->name="sin theta";
priv=do_alloc(1, sizeof(*priv));
priv->num_dec=ceil(M_PI/resolution);
priv->resolution=resolution;
priv->num_ra=do_alloc(priv->num_dec,sizeof(*priv->num_ra));
/* the total number of points is variable... */
grid->max_n_dec=priv->num_dec;
grid->max_n_ra=0;
grid->npoints=0;
for(i=0;i<priv->num_dec;i++){
	a=M_PI_2*(-1.0+(1.0+2.0*i)/priv->num_dec);
	priv->num_ra[i]=ceil(2.0*M_PI*cos(a)/resolution);
	/* make it always contain odd number of points.. makes plotting easier */
	priv->num_ra[i]|=1;
	if(priv->num_ra[i]>grid->max_n_ra)grid->max_n_ra=priv->num_ra[i];
	grid->npoints+=priv->num_ra[i];
	}

grid->latitude=do_alloc(grid->npoints, sizeof(SKY_GRID_TYPE));
grid->longitude=do_alloc(grid->npoints, sizeof(SKY_GRID_TYPE));
grid->band=do_alloc(grid->npoints, sizeof(*grid->band));
grid->band_f=do_alloc(grid->npoints, sizeof(*grid->band_f));
for(i=0;i<GRID_E_COUNT;i++)
	grid->e[i]=do_alloc(grid->npoints, sizeof(SKY_GRID_TYPE));
grid->grid_priv=priv;

/* fill in the coordinates */
k=0;
for(i=0;i<priv->num_dec;i++){
	a=M_PI_2*(-1.0+(1.0+2.0*i)/priv->num_dec);
	for(j=0;j<priv->num_ra[i];j++){
		b=M_PI*(1.0+2.0*j)/priv->num_ra[i];
		grid->latitude[k]=a;
		grid->longitude[k]=b;
		k++;
		}
	}
precompute_values(grid);
return grid;
}

long find_sin_theta_closest(SKY_GRID *grid, float RA, float DEC)
{
int i,j_start,j_stop,k, k_start, k_stop, best_i;
SIN_THETA_SKY_GRID_PRIV *priv=grid->grid_priv;
SKY_GRID_TYPE ds, best_ds=10.0;

k=floor((priv->num_dec*(DEC+M_PI_2))/M_PI+0.5);
k_start=k-1;
k_stop=k+1;
if(k_start<0)k_start=0;
if(k_stop>=priv->num_dec)k_stop=priv->num_dec-1;
if(k_stop<k_start)k_start=k_stop;

/* Find 3 lines worth of points to search .. Brute force, but reliable */
for(i=0,j_start=0;i<k_start;i++)j_start+=priv->num_ra[i];
for(j_stop=j_start;i<=k_stop;i++)j_stop+=priv->num_ra[i];

/* Search them for closest point */
best_i=-1;
for(i=j_start;i<j_stop;i++){
	ds=spherical_distance(RA, DEC, grid->longitude[i], grid->latitude[i]);
	if((best_i<0) || (ds<best_ds)){
		best_i=i;
		best_ds=ds;
		}
	}
return best_i;
}

void free_grid(SKY_GRID *grid)
{
long i;
/* free private fields */
if(!strcmp(grid->name,"sin theta")){
	SIN_THETA_SKY_GRID_PRIV *priv;
	priv=grid->grid_priv;
	free(priv->num_ra);
	free(priv);	
	} else
if(!strcmp(grid->name,"plain rectangular")){
	free(grid->grid_priv);
	} else
if(!strcmp(grid->name,"arcsin")){
	free(grid->grid_priv);
	} else {
	if(grid->grid_priv!=NULL){
		fprintf(stderr,"** Unknown grid type \"%s\", possible memory leak when freeing private structure\n", grid->name);
		fprintf(LOG,"** Unknown grid type \"%s\", possible memory leak when freeing private structure\n", grid->name);
		free(grid->grid_priv);
		}
	}
free(grid->latitude);
free(grid->longitude);
free(grid->band);
free(grid->band_f);
for(i=0;i<GRID_E_COUNT;i++)free(grid->e[i]);
free(grid);
}

void compute_list_map(SKY_SUPERGRID *sg)
{
long i,j,k,n;
sg->max_npatch=1;
for(i=0;i<sg->super_grid->npoints;i++){
	k=sg->reverse_map[i];
	j=sg->first_map[k];
	if(j==i)continue; /* do not mark very first item - we already know what it is */
	for(n=2;sg->list_map[j]>=0;j=sg->list_map[j],n++);
	sg->list_map[j]=i;
	if(n>sg->max_npatch)sg->max_npatch=n;
	}
}

void angle_assign_bands(SKY_GRID *grid, int n_bands)
{
int i,k;
SKY_GRID_TYPE angle, proj, x,y,z;
for(i=0;i<grid->npoints;i++){
	/* convert into 3d */
	x=cos(grid->longitude[i])*cos(grid->latitude[i]);
	y=sin(grid->longitude[i])*cos(grid->latitude[i]);
	z=sin(grid->latitude[i]);

	proj=x*band_axis[0]+y*band_axis[1]+z*band_axis[2];
	if(proj<-1.0)proj=-1.0;
	if(proj>1.0)proj=1.0;
	
	angle=acosf(proj);
	
	k=floor((angle/M_PI)*n_bands);
	if(k<0)k=0;
	if(k>=n_bands)k=n_bands-1;
	grid->band[i]=k;
	grid->band_f[i]=k;
	}
}

void S_assign_bands(SKY_GRID *grid, int n_bands, double s_f)
{
int i,k;
double S, large_S=0.7;
SKY_GRID_TYPE angle, proj, x,y,z;

for(i=0;i<grid->npoints;i++){
	/* convert into 3d */
	x=cos(grid->longitude[i])*cos(grid->latitude[i]);
	y=sin(grid->longitude[i])*cos(grid->latitude[i]);
	z=sin(grid->latitude[i]);

 	S=s_f+
		x*(average_det_velocity[0]*s_f+band_axis_norm*band_axis[0])+
		y*(average_det_velocity[1]*s_f+band_axis_norm*band_axis[1])+
		z*(average_det_velocity[2]*s_f+band_axis_norm*band_axis[2]);

	S=fabs(S)/band_axis_norm;
	
	if(S>=large_S){
		grid->band[i]=0;
		grid->band_f[i]=0;
		continue;
		}
	k=n_bands-floor(S*(n_bands-1)/large_S)-1;
	if(k>=n_bands)k=n_bands-1;
	if(k<1)k=1;

	grid->band[i]=k;
	grid->band_f[i]=k;
	}
}

void mask_far_points(SKY_GRID *grid, SKY_GRID_TYPE ra, SKY_GRID_TYPE dec, SKY_GRID_TYPE radius)
{
int i;
SKY_GRID_TYPE ds;
for(i=0;i<grid->npoints;i++){
	ds=acos(sin(grid->latitude[i])*sin(dec)+
		cos(grid->latitude[i])*cos(dec)*
		cos(grid->longitude[i]-ra));
	if(ds>radius){
		grid->band[i]=-1;
		grid->band_f[i]=-1;
		}
	}
}

void mask_small_cos(SKY_GRID *grid, SKY_GRID_TYPE x, SKY_GRID_TYPE y, SKY_GRID_TYPE z, SKY_GRID_TYPE cos_level)
{
int i;
SKY_GRID_TYPE ds,x0,y0,z0;
ds=sqrt(x*x+y*y+z*z);
x0=x/ds;
y0=y/ds;
z0=z/ds;

for(i=0;i<grid->npoints;i++){
	ds=fabs(grid->e[0][i]*x0+
	   grid->e[1][i]*y0+
	   grid->e[2][i]*z0);
	if(ds<cos_level){
		grid->band[i]=-1;
		grid->band_f[i]=-1;
		}
	}
}

void propagate_far_points_to_super_grid(SKY_GRID *grid, SKY_SUPERGRID *super_grid)
{
long k, offset, pi;
for(pi=0;pi<grid->npoints;pi++){
	if(grid->band[pi]>=0)continue;
	for(k=0,offset=super_grid->first_map[pi];offset>=0;offset=super_grid->list_map[offset],k++){
		super_grid->super_grid->band[offset]=-1;
		super_grid->super_grid->band_f[offset]=-1;
		}
	}
}

void propagate_far_points_from_super_grid(SKY_GRID *grid, SKY_SUPERGRID *super_grid)
{
long k, offset, pi;
int nonzero;
for(pi=0;pi<grid->npoints;pi++){
	nonzero=0;
	for(k=0,offset=super_grid->first_map[pi];offset>=0;offset=super_grid->list_map[offset],k++){
		if(super_grid->super_grid->band[offset]>=0){
			nonzero=1;
			break;
			}
		}
	if(!nonzero){
		grid->band[pi]=-1;
		grid->band_f[pi]=-1;
		}
	}
}

SKY_SUPERGRID *make_rect_supergrid(SKY_GRID *grid, int ra_factor, int dec_factor)
{
SKY_SUPERGRID *sg;
RECT_SKY_GRID_PRIV *priv;
long i,j,k, ra_start, dec_start, di,dj;
if(strcmp(grid->name,"plain rectangular") && 
   strcmp(grid->name, "arcsin")){
   	fprintf(stderr,"** Internal error: cannot make rectangular supergrid from %s\n", grid->name);
	exit(-1);
   	}
priv=grid->grid_priv;

sg=do_alloc(1, sizeof(*sg));
if(!strcmp(grid->name,"arcsin")){
	sg->super_grid=make_arcsin_grid(priv->num_ra*ra_factor, priv->num_dec*dec_factor);
	} else {
	sg->super_grid=make_rect_grid(priv->num_ra*ra_factor, priv->num_dec*dec_factor);
	}

sg->first_map=do_alloc(grid->npoints, sizeof(*sg->first_map));
sg->reverse_map=do_alloc(sg->super_grid->npoints, sizeof(*sg->reverse_map));
sg->list_map=do_alloc(sg->super_grid->npoints, sizeof(*sg->list_map));
//sg->max_npatch=ra_factor*dec_factor;

/* clear the arrays */
for(i=0;i<grid->npoints;i++)sg->first_map[i]=-1;
for(i=0;i<sg->super_grid->npoints;i++){
	sg->reverse_map[i]=-1;
	sg->list_map[i]=-1;
	}

ra_start=(ra_factor+1)/2;
dec_start=(dec_factor+1)/2;

k=0;
for(i=ra_start;i<priv->num_ra*ra_factor;i+=ra_factor)
	for(j=dec_start;j<priv->num_dec*dec_factor;j+=dec_factor){
		sg->first_map[k]=i*priv->num_dec*dec_factor+j;
		for(di=0;di<ra_factor;di++)
			for(dj=0;dj<dec_factor;dj++){
				sg->reverse_map[(i+di-ra_start)*priv->num_dec*dec_factor+j+dj-dec_start]=k;
				}
		k++;
		}

compute_list_map(sg);
return sg;
}

SKY_SUPERGRID *make_sin_theta_supergrid(SKY_GRID *grid, int factor)
{
SKY_SUPERGRID *sg;
SIN_THETA_SKY_GRID_PRIV *priv;
int i,j,k, pi, pk, ra_pk;
SKY_GRID_TYPE ds, best_ds;
if(strcmp(grid->name,"sin theta")){
   	fprintf(stderr,"** Internal error: cannot make sin theta supergrid from %s\n", grid->name);
	exit(-1);
   	}
priv=grid->grid_priv;

sg=do_alloc(1, sizeof(*sg));
sg->super_grid=make_sin_theta_grid(priv->resolution/factor);

sg->first_map=do_alloc(grid->npoints, sizeof(*sg->first_map));
sg->reverse_map=do_alloc(sg->super_grid->npoints, sizeof(*sg->reverse_map));
sg->list_map=do_alloc(sg->super_grid->npoints, sizeof(*sg->list_map));

fprintf(stderr,"npoints=%d super grid npoints=%d\n", grid->npoints, sg->super_grid->npoints);
/* clear the arrays */
for(i=0;i<grid->npoints;i++)sg->first_map[i]=-1;
for(i=0;i<sg->super_grid->npoints;i++){
	sg->reverse_map[i]=-1;
	sg->list_map[i]=-1;
	}

k=0;
pk=0;
ra_pk=0;
pi=0;
sg->first_map[0]=0;
sg->first_map[grid->npoints-1]=sg->super_grid->npoints-1;
sg->reverse_map[sg->super_grid->npoints-1]=grid->npoints-1;
sg->reverse_map[0]=0;
for(k=1;k<sg->super_grid->npoints-1;k++){
	#if 0
	fprintf(stderr, "patch=(%.2f,%.2f) super=(%.2f,%.2f)\n", 
		grid->longitude[pk],grid->latitude[pk],
		sg->super_grid->longitude[k], sg->super_grid->latitude[k]
		);
	#endif
	
	#if 0  /* older and really fast way.. is it right ?? */
	if((sg->super_grid->longitude[k]<sg->super_grid->longitude[k-1])){
			/* crossing RA=0 boundary */
	//		fprintf(stderr, "@");
			//pk=pk-priv->num_ra[pi]+1;
			pk=ra_pk;
			} else
	if((pk+1<grid->npoints)&& (grid->longitude[pk+1]>=grid->longitude[pk]) && 
		(sg->super_grid->longitude[k]-grid->longitude[pk]>=
			grid->longitude[pk+1]-sg->super_grid->longitude[k])){
	//		fprintf(stderr, "+");
			pk++;
			}
	if((pi+1<priv->num_dec) && (sg->super_grid->latitude[k]-grid->latitude[pk]>=
		grid->latitude[ra_pk+priv->num_ra[pi]]-sg->super_grid->latitude[k])){
	//		fprintf(stderr, "#");
			ra_pk+=priv->num_ra[pi];
			pk=ra_pk;
			pi++;
			}
	//fprintf(stderr, "k=%d pk=%d pi=%d\n", k, pk, pi);
	sg->reverse_map[k]=pk;	
	sg->first_map[pk]=k; /* not the most efficient, but it works */

        #else /* a good deal slower, but much surer */
	
	if((sg->super_grid->longitude[k]<sg->super_grid->longitude[k-1])){
			/* crossing RA=0 boundary */
	//		fprintf(stderr, "@");
			//pk=pk-priv->num_ra[pi]+1;
			pk=ra_pk;
			}
			else
	if((pk+1<grid->npoints)&& (grid->longitude[pk+1]>=grid->longitude[pk]) && 
		(sg->super_grid->longitude[k]-grid->longitude[pk]>=
			grid->longitude[pk+1]-sg->super_grid->longitude[k])){
	//		fprintf(stderr, "+");
			pk++;
			}
	if((pi+1<priv->num_dec) && (sg->super_grid->latitude[k]-grid->latitude[pk]>=
		grid->latitude[ra_pk+priv->num_ra[pi]]-sg->super_grid->latitude[k])){
	//		fprintf(stderr, "#");
			ra_pk+=priv->num_ra[pi];
			pk=ra_pk;
			pi++;
			}
	

	best_ds=10.0;
	j=pk;
	
	i=pk-2*grid->max_n_ra-1;
	if(i<0)i=0;
	for(;(i<(pk+2*grid->max_n_ra+1)) && (i<grid->npoints);i++){
		/* Try approximate comparison first */
		ds=fabs(grid->longitude[i]-sg->super_grid->longitude[k]);
		/* check that we are not far enough in RA */
		/* The (ds<1.0) is to check that we are not jumping 2*PI */
		if((cos(grid->latitude[i])*ds>best_ds)&&(ds<6.0))continue;
		ds=spherical_distance(grid->longitude[i], grid->latitude[i],
				sg->super_grid->longitude[k], sg->super_grid->latitude[k]);
		if(ds<best_ds){
			j=i;
			best_ds=ds;
			}
		}

	if(pk!=j)fprintf(stderr, "k=%d pk=%d pi=%d j=%d  %d\n", k, pk, pi, j, pk-j);
	sg->reverse_map[k]=j;	
	sg->first_map[j]=k; /* not the most efficient, but it works */

	#endif

	}
compute_list_map(sg);
return sg;
}

void rotate_xz(SKY_GRID_TYPE RA_in, SKY_GRID_TYPE DEC_in, 
			SKY_GRID_TYPE * RA_out, SKY_GRID_TYPE * DEC_out, 
			SKY_GRID_TYPE angle)
{
SKY_GRID_TYPE x,y,z,x2,y2,z2;

/* convert into 3d */
x=cos(RA_in)*cos(DEC_in);
y=sin(RA_in)*cos(DEC_in);
z=sin(DEC_in);

x2=cos(angle)*x+sin(angle)*z;
y2=y;
z2=-sin(angle)*x+cos(angle)*z;

*DEC_out=atan2f(z2, sqrt(x2*x2+y2*y2));
*RA_out=atan2f(y2, x2);
if(*RA_out <0) *RA_out+=2*M_PI;
//fprintf(stderr,"%f %f --> %f %f\n", RA_in, DEC_in, *RA_out, *DEC_out);
}

void rotate_xy(SKY_GRID_TYPE RA_in, SKY_GRID_TYPE DEC_in, 
			SKY_GRID_TYPE * RA_out, SKY_GRID_TYPE * DEC_out, 
			SKY_GRID_TYPE angle)
{
SKY_GRID_TYPE x,y,z,x2,y2,z2;

/* convert into 3d */
x=cos(RA_in)*cos(DEC_in);
y=sin(RA_in)*cos(DEC_in);
z=sin(DEC_in);

x2=cos(angle)*x-sin(angle)*y;
y2=sin(angle)*x+cos(angle)*y;
z2=z;

*DEC_out=atan2f(z2, sqrt(x2*x2+y2*y2));
*RA_out=atan2f(y2, x2);
if(*RA_out <0) *RA_out+=2*M_PI;
//fprintf(stderr,"%f %f --> %f %f\n", RA_in, DEC_in, *RA_out, *DEC_out);
}

void rotate_grid_xz(SKY_GRID *grid, SKY_GRID_TYPE angle)
{
long i;
for(i=0;i<grid->npoints;i++){
	rotate_xz((grid->longitude[i]), (grid->latitude[i]),
		&(grid->longitude[i]), &(grid->latitude[i]),
		angle);
	}
precompute_values(grid);
}

void rotate_grid_xy(SKY_GRID *grid, SKY_GRID_TYPE angle)
{
long i;
for(i=0;i<grid->npoints;i++){
	rotate_xy((grid->longitude[i]), (grid->latitude[i]),
		&(grid->longitude[i]), &(grid->latitude[i]),
		angle);
	}
precompute_values(grid);
}
