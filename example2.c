// This is an example of intensity distribution analysis.
#include "multi_aw.h"

int main()
{
  Maw aw;
  FILE *fp1,*fp2;
  double complex p,v[3];
  double x[3],rang,dr,*ip,*iv;
  int max,i,j;
  
  init_maw(&aw);
  read_data_maw(&aw);
  print_data_maw(&aw); 
  setup_maw(&aw);
  
  max=200;
  rang=4.0*aw.lambda0;
  dr=rang*2/(double)(max-1);
  
  ip=(double *)m_alloc2(max,sizeof(double),"exampl2.c,ip");
  iv=(double *)m_alloc2(max,sizeof(double),"exampl2.c,iv");
  
  // x=0 plane
  if((fp1=fopen("Ip_yz.txt","wt"))==NULL){    printf("Can not open the file.\n");    exit(1);  }
  fprintf(fp1,"%s\n","# y z sound_pressure_intensity");
  if((fp2=fopen("Iv_yz.txt","wt"))==NULL){    printf("Can not open the file.\n");    exit(1);  }
  fprintf(fp2,"%s\n","# y z particle_velocity_intensity");
  x[0]=0.0;
  for(i=0;i<max;i++){
    x[1]=-rang+(double)i*dr;
    #pragma omp parallel for schedule(dynamic) firstprivate(x) private(p,v) // omp parallel
    for(j=0;j<max;j++){
      x[2]=-rang+(double)j*dr;
      calc_maw_pv(&p,v,x,&aw);
      ip[j]=creal(p*conj(p));
      iv[j]=creal(v[0]*conj(v[0]))+creal(v[1]*conj(v[1]))+creal(v[2]*conj(v[2]));
    }
    for(j=0;j<max;j++){ 
      x[2]=-rang+(double)j*dr;
      fprintf(fp1,"%g %g %15.14e\n",x[1],x[2],ip[j]);
      fprintf(fp2,"%g %g %15.14e\n",x[1],x[2],iv[j]);
    }
    fprintf(fp1,"\n");
    fprintf(fp2,"\n");
  }
  fclose(fp1);
  fclose(fp2);
  
  // y=0 plane
  if((fp1=fopen("Ip_xz.txt","wt"))==NULL){    printf("Can not open the file.\n");    exit(1);  }
  fprintf(fp1,"%s\n","# x z sound_pressure_intensity");
  if((fp2=fopen("Iv_xz.txt","wt"))==NULL){    printf("Can not open the file.\n");    exit(1);  }
  fprintf(fp2,"%s\n","# x z particle_velocity_intensity");
  x[1]=0.0;
  for(i=0;i<max;i++){
    x[0]=-rang+(double)i*dr;
    #pragma omp parallel for schedule(dynamic) firstprivate(x) private(p,v) // omp parallel
    for(j=0;j<max;j++){
      x[2]=-rang+(double)j*dr;
      calc_maw_pv(&p,v,x,&aw);
      ip[j]=creal(p*conj(p));
      iv[j]=creal(v[0]*conj(v[0]))+creal(v[1]*conj(v[1]))+creal(v[2]*conj(v[2]));
    }
    for(j=0;j<max;j++){
      x[2]=-rang+(double)j*dr;
      fprintf(fp1,"%g %g %15.14e\n",x[0],x[2],ip[j]);
      fprintf(fp2,"%g %g %15.14e\n",x[0],x[2],iv[j]);
    }
    fprintf(fp1,"\n");
    fprintf(fp2,"\n");
  }
  fclose(fp1);
  fclose(fp2);

  // z=0 plane  
  if((fp1=fopen("Ip_xy.txt","wt"))==NULL){    printf("Can not open the file.\n");    exit(1);  }
  fprintf(fp1,"%s\n","# x y sound_pressure_intensity");
  if((fp2=fopen("Iv_xy.txt","wt"))==NULL){    printf("Can not open the file.\n");    exit(1);  }
  fprintf(fp2,"%s\n","# x y particle_velocity_intensity");
  x[2]=0.0;
  for(i=0;i<max;i++){
    x[0]=-rang+(double)i*dr;
    #pragma omp parallel for schedule(dynamic) firstprivate(x) private(p,v) // omp parallel
    for(j=0;j<max;j++){
      x[1]=-rang+(double)j*dr;
      calc_maw_pv(&p,v,x,&aw);
      ip[j]=creal(p*conj(p));
      iv[j]=creal(v[0]*conj(v[0]))+creal(v[1]*conj(v[1]))+creal(v[2]*conj(v[2]));
    }
    for(j=0;j<max;j++){
      x[1]=-rang+(double)j*dr;
      fprintf(fp1,"%g %g %15.14e\n",x[0],x[1],ip[j]);
      fprintf(fp2,"%g %g %15.14e\n",x[0],x[1],iv[j]);
    }
    fprintf(fp1,"\n");
    fprintf(fp2,"\n");
  }
  fclose(fp1);
  fclose(fp2);
  
  printf("intensity plot is finished\n");
  
  free_maw(&aw);
  return 0;
}
