// This is an example of calculation of sound pressure and particle velocity. 
#include "multi_aw.h"

// verification function of directional derivative using central difference.
void veri_dderv(Maw *aw,double *r,double *n);

int main()
{
  Maw aw;
  double complex p,dpdn,v[3],cet;
  double r[3],t,n[3],theta,phi;
  
  init_maw(&aw);           // initialize
  read_data_maw(&aw);      // search and read beam datafile
  print_data_maw(&aw);     // print beam data
  setup_maw(&aw);          // memory allocation and calculation of coefficients
  
  r[0]= 0.5e-3; // set x-coordinate [m]
  r[1]=-3.0e-3; // set y-coordinate [m]
  r[2]= 1.0e-3; // set z-coordinate [m]
  
  calc_maw_pv(&p,v,r,&aw); 
  printf("sound pressure and particle velocity at r=(% g,% g,% g)\n",r[0],r[1],r[2]);
  printf("p  =% 15.14e %+15.14e I\n",creal(p),cimag(p));
  printf("v_x=% 15.14e %+15.14e I\n",creal(v[0]),cimag(v[0]));
  printf("v_y=% 15.14e %+15.14e I\n",creal(v[1]),cimag(v[1]));
  printf("v_z=% 15.14e %+15.14e I\n",creal(v[2]),cimag(v[2]));
  
  t=0.01; // set time [sec]
  cet=cexp(-I*2.0*M_PI*aw.f*t);
  printf("Real value at r=(% g,% g,% g), t=%g \n",r[0],r[1],r[2],t);
  printf("p  =% 15.14e\n",creal(p*cet));
  printf("v_x=% 15.14e\n",creal(v[0]*cet));
  printf("v_y=% 15.14e\n",creal(v[1]*cet));
  printf("v_z=% 15.14e\n",creal(v[2]*cet));
  printf("\n");

  theta=0.3;
  phi=1.2;  // theta,phi are parameters for unit vector n 
  n[0]=sin(theta)*cos(phi);
  n[1]=sin(theta)*sin(phi);
  n[2]=cos(theta); // n is the unit vector defining the direction of directional derivative
  calc_maw_dpdn(&p,&dpdn,r,n,&aw);
  printf("Directional derivative of sound pressure at r=(% g,% g,% g)\n",r[0],r[1],r[2]);
  printf("n=(% g,% g,% g),|n|=1\n",n[0],n[1],n[2]);
  printf("dpdn=% 15.14e %+15.14e I\n",creal(dpdn),cimag(dpdn));
  veri_dderv(&aw,r,n); // verification function 
  
  free_maw(&aw); // free allocated memory
  return 0;
}

void veri_dderv(Maw *aw,double *r,double *n)
{
  double complex p0,p1,tv[3],dpdn;
  double h,tr[3];  
 
  h=1.0e-7*aw->lambda0;

  tr[0]=r[0]-h*n[0];
  tr[1]=r[1]-h*n[1];
  tr[2]=r[2]-h*n[2];
  calc_maw_pv(&p0,tv,tr,aw);
  
  tr[0]=r[0]+h*n[0];
  tr[1]=r[1]+h*n[1];
  tr[2]=r[2]+h*n[2];
  calc_maw_pv(&p1,tv,tr,aw);
  
  printf("Directional derivative using central difference. h=%g\n",h);
  dpdn=(p1-p0)/(2.0*h);
  printf("dpdn=% 15.14e %+15.14e I\n",creal(dpdn),cimag(dpdn));
}
