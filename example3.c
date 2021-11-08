#include "multi_aw.h"
#include <sys/stat.h>
#include <errno.h>  
#include <png.h>

typedef struct image_data{
  char dir_name[64];      // directory name to output image
  int scale;              // number for enlarge the output image
  
  int m;                  // sampling number 
  double rang;            // range of sampling

  int ts;                 // time step per cycle
  
  double complex *p,*v;   // sound pressure and particles velocity 
  double mp,mv[3];        // maximum amplitude of each component
}IMD;

void make_directory(char *dir_name);
void pv_x(IMD *id,Maw *aw);
void pv_y(IMD *id,Maw *aw);
void pv_z(IMD *id,Maw *aw);
void output_pv(char *pl,IMD *id,Maw *aw);
void output_color_bar(IMD *id);

// color table
png_byte ct1[9][3]={{0x00,0x00,0x90},{0x00,0x0f,0xff},{0x00,0x90,0xff},{0x0f,0xff,0xee},
                    {0xff,0xff,0xff},{0xff,0xee,0x00},{0xff,0x70,0x00},{0xee,0x00,0x00},{0x7f,0x00,0x00}};
/*                    
png_byte ct1[9][3]={{0x00,0x00,0x90},{0x00,0x0f,0xff},{0x00,0x90,0xff},{0x0f,0xff,0xee},
                    {0x90,0xff,0x70},{0xff,0xee,0x00},{0xff,0x70,0x00},{0xee,0x00,0x00},{0x7f,0x00,0x00}};  
*/

int main()
{
  Maw aw;
  IMD id;
  
  init_maw(&aw);
  read_data_maw(&aw);
  print_data_maw(&aw); 
  setup_maw(&aw);   

  sprintf(id.dir_name,"images"); // directory name to output image
  id.scale=1;                    // number for enlarge the output image
  id.m=180;                      // sampling number 
  id.rang=4.0*aw.lambda0;        // range of sampling
  id.ts=40;                      // time step per cycle
  
  make_directory(id.dir_name);

  id.p=(double complex *)m_alloc2(id.m*id.m*1,sizeof(double complex),"example3.c, p");
  id.v=(double complex *)m_alloc2(id.m*id.m*3,sizeof(double complex),"example3.c, v");

  // x=0 plane
  pv_x(&id,&aw);
  output_pv("yz",&id,&aw);
  // y=0 plane
  pv_y(&id,&aw);
  output_pv("xz",&id,&aw);
  // z=0 plane
  pv_z(&id,&aw);
  output_pv("xy",&id,&aw);
  
  free(id.p);
  free(id.v);
  
  // output color bar image
  output_color_bar(&id);
  
  free_maw(&aw);
  return 0;
}

void make_directory(char *dir_name)
{
  int ret;
  
  ret=mkdir(dir_name,S_IRWXU|S_IRWXG);
  if(ret!=0 && errno!=EEXIST){
    printf("failed to make directory. Exit..");
    exit(1);
  }
}

void pv_x(IMD *id,Maw *aw)
{
  double complex p,v[3];
  double x[3],dr;
  int i,j,d;
  
  dr=id->rang*2.0/(double)(id->m-1);
  
  id->mp=0.0;
  for(i=0;i<3;i++) id->mv[i]=0.0;
  
  // x=0 plane  
  x[0]=0.0;
  #pragma omp parallel for schedule(dynamic) firstprivate(x) private(j,d,p,v) 
  for(i=0;i<id->m;i++){
    x[2]=id->rang-(double)i*dr;
    for(j=0;j<id->m;j++){
      x[1]=-id->rang+(double)j*dr;
      calc_maw_pv(&p,v,x,aw);
      
      #pragma omp critical
      if(cabs(p)>id->mp) id->mp=cabs(p);
      #pragma omp critical
      for(d=0;d<3;d++) if(cabs(v[d])>id->mv[d]) id->mv[d]=cabs(v[d]);
      
      id->p[i*id->m+j]=p;
      for(d=0;d<3;d++) id->v[i*id->m*3+j*3+d]=v[d];
    }
  }
}

void pv_y(IMD *id,Maw *aw)
{
  double complex p,v[3];
  double x[3],dr;
  int i,j,d;
  
  dr=id->rang*2.0/(double)(id->m-1);

  id->mp=0.0;
  for(i=0;i<3;i++) id->mv[i]=0.0;
  
  // y=0 plane  
  x[1]=0.0;
  #pragma omp parallel for schedule(dynamic) firstprivate(x) private(j,d,p,v) 
  for(i=0;i<id->m;i++){
    x[2]=id->rang-(double)i*dr;
    for(j=0;j<id->m;j++){
      x[0]=-id->rang+(double)j*dr;
      calc_maw_pv(&p,v,x,aw);
      
      #pragma omp critical
      if(cabs(p)>id->mp) id->mp=cabs(p);
      #pragma omp critical
      for(d=0;d<3;d++) if(cabs(v[d])>id->mv[d]) id->mv[d]=cabs(v[d]);
      
      id->p[i*id->m+j]=p;
      for(d=0;d<3;d++) id->v[i*id->m*3+j*3+d]=v[d];
    }
  }
}

void pv_z(IMD *id,Maw *aw)
{
  double complex p,v[3];
  double x[3],dr;
  int i,j,d;
  
  dr=id->rang*2.0/(double)(id->m-1);

  id->mp=0.0;
  for(i=0;i<3;i++) id->mv[i]=0.0;
  
  // z=0 plane  
  x[2]=0.0;
  #pragma omp parallel for schedule(dynamic) firstprivate(x) private(j,d,p,v) 
  for(i=0;i<id->m;i++){
    x[1]=id->rang-(double)i*dr;
    for(j=0;j<id->m;j++){
      x[0]=-id->rang+(double)j*dr;
      calc_maw_pv(&p,v,x,aw);
      
      #pragma omp critical
      if(cabs(p)>id->mp) id->mp=cabs(p);
      #pragma omp critical
      for(d=0;d<3;d++) if(cabs(v[d])>id->mv[d]) id->mv[d]=cabs(v[d]);
      
      id->p[i*id->m+j]=p;
      for(d=0;d<3;d++) id->v[i*id->m*3+j*3+d]=v[d];
    }
  }
}

void output_pv(char *pl,IMD *id,Maw *aw)
{
  void output_png(int nt,double complex cet,char *pl,IMD *id);
  void output_color_bar(IMD *id);
  
  FILE *fp;
  char fn[128];
  double dt;
  int n;
  
  dt=(1.0/aw->f)/(double)id->ts;
  
  #pragma omp parallel for schedule(dynamic) 
  for(n=0;n<id->ts;n++){
    output_png(n,cexp(-I*2.0*M_PI*aw->f*dt*(double)n),pl,id);
  }

  // print info
  sprintf(fn,"%s/%s_info.txt",id->dir_name,pl);
  fp=fopen(fn,"wt");
  if(fp==NULL){
    printf("Failed to open the %s file. Exit...\n",fn);
    exit(1);
  }
  fprintf(fp,"the range of color bar\n");
  fprintf(fp,"p  is %8e to %8e\n",-id->mp,id->mp);
  fprintf(fp,"vx is %8e to %8e\n",-id->mv[0],id->mv[0]);
  fprintf(fp,"vy is %8e to %8e\n",-id->mv[1],id->mv[1]);
  fprintf(fp,"vz is %8e to %8e\n",-id->mv[2],id->mv[2]);
  fclose(fp);
}

void output_png(int nt,double complex cet,char *pl,IMD *id)
{
  int color_rgb(double x,png_byte *r,png_byte *g,png_byte *b); // -1 <= x <= 1

  FILE *fp,*fv[3];
  char fname[256],sf[3]={'x','y','z'};
  int j,i,sj,si,d,m,scale;
  png_uint_32 width,height;
  png_structp png_p,png_v[3];
  png_infop info_p,info_v[3];
  png_bytepp pd_p,pd_v[3];
  png_byte r,g,b;

  m=id->m;
  scale=id->scale;

  width =m*(scale+1);
  height=m*(scale+1);
  
  // p
  png_p =png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  info_p=png_create_info_struct(png_p);
  sprintf(fname,"%s/%s_p_%03d.png",id->dir_name,pl,nt);
  fp=fopen(fname,"wb");
  if(fp==NULL){
    printf("Failed to open the %s file. Exit...\n",fname);
    exit(1);
  }
  png_init_io(png_p,fp);
  png_set_IHDR(png_p,info_p,width,height,8,PNG_COLOR_TYPE_RGB,PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_DEFAULT,PNG_FILTER_TYPE_DEFAULT);
  pd_p=(png_bytepp)png_malloc(png_p,sizeof(png_bytep)*height);
  png_set_rows(png_p,info_p,pd_p);
  for(j=0;j<height;j++) pd_p[j]=(png_bytep)png_malloc(png_p,sizeof(png_byte)*width*3);
  // v
  for(d=0;d<3;d++){
    png_v[d] =png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    info_v[d]=png_create_info_struct(png_v[d]);
    sprintf(fname,"%s/%s_v%c_%03d.png",id->dir_name,pl,sf[d],nt);
    fv[d]=fopen(fname,"wb");
    if(fv[d]==NULL){
      printf("Failed to open the %s file. Exit...\n",fname);
      exit(1);
    }
    png_init_io(png_v[d],fv[d]);
    png_set_IHDR(png_v[d],info_v[d],width,height,8,PNG_COLOR_TYPE_RGB,PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,PNG_FILTER_TYPE_DEFAULT);
    pd_v[d]=(png_bytepp)png_malloc(png_v[d],sizeof(png_bytep)*height);
    png_set_rows(png_v[d],info_v[d],pd_v[d]);
    for(j=0;j<height;j++) pd_v[d][j]=(png_bytep)png_malloc(png_v[d],sizeof(png_byte)*width*3);
  }

  for(i=0;i<m;i++){
    for(j=0;j<m;j++){
      // p
      color_rgb(creal(cet*id->p[i*m+j])/id->mp,&r,&g,&b);
      for(si=0;si<=scale;si++){
        for(sj=0;sj<=scale;sj++){
          pd_p[i*(scale+1)+si][(j*(scale+1)+sj)*3+0]=r;
          pd_p[i*(scale+1)+si][(j*(scale+1)+sj)*3+1]=g;
          pd_p[i*(scale+1)+si][(j*(scale+1)+sj)*3+2]=b;
        }
      }      
      // v
      for(d=0;d<3;d++){
        color_rgb(creal(cet*id->v[i*m*3+j*3+d])/id->mv[d],&r,&g,&b);
        for(si=0;si<=scale;si++){
          for(sj=0;sj<=scale;sj++){
            pd_v[d][i*(scale+1)+si][(j*(scale+1)+sj)*3+0]=r;
            pd_v[d][i*(scale+1)+si][(j*(scale+1)+sj)*3+1]=g;
            pd_v[d][i*(scale+1)+si][(j*(scale+1)+sj)*3+2]=b;
          }
        }      
      }
    }
  }
  // p
  png_write_png(png_p,info_p,PNG_TRANSFORM_IDENTITY,NULL);
  for(j=0;j<height;j++) png_free(png_p,pd_p[j]);
  png_free(png_p,pd_p);
  fclose(fp);
  // v
  for(d=0;d<3;d++){
    png_write_png(png_v[d],info_v[d],PNG_TRANSFORM_IDENTITY,NULL);
    for(j=0;j<height;j++) png_free(png_v[d],pd_v[d][j]);
    png_free(png_v[d],pd_v[d]);
    fclose(fv[d]);
  }
}

void output_color_bar(IMD *id)
{
  int color_rgb(double x,png_byte *r,png_byte *g,png_byte *b); // -1 <= x <= 1
  
  FILE *fp;
  char fname[128];
  int j,i;
  
  png_uint_32 width,height;
  png_structp png;
  png_infop info;
  png_bytepp pdata;
  png_byte r,g,b;

  sprintf(fname,"%s/color_bar.png",id->dir_name);

  height=id->m*(id->scale+1);
  width=height/16;
  
  png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  info= png_create_info_struct(png);
  
  fp=fopen(fname,"wb");
  if(fp==NULL){
    printf("Failed to open the %s file. Exit...\n",fname);
    exit(1);
  }
  
  png_init_io(png, fp);
  png_set_IHDR(png,info,width,height,8,PNG_COLOR_TYPE_RGB,PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_DEFAULT,PNG_FILTER_TYPE_DEFAULT);
  pdata=(png_bytepp)png_malloc(png, sizeof(png_bytep)*height);
  png_set_rows(png,info,pdata);

  for(j=0;j<height;j++){
    pdata[j]=(png_bytep)png_malloc(png,sizeof(png_byte)*width*3);
  }
  
  for(i=0;i<height;i++){
    color_rgb(1.0-(2.0/(double)height)*(double)i,&r,&g,&b);
    for(j=0;j<width;j++){
      pdata[i][j*3+0]=r;
      pdata[i][j*3+1]=g;
      pdata[i][j*3+2]=b;
    }
  }
  
  png_write_png(png, info, PNG_TRANSFORM_IDENTITY, NULL);
  
  for(j=0;j<height;j++){
    png_free(png,pdata[j]);
  }
  png_free(png,pdata);
  fclose(fp);
}

int color_rgb(double x,png_byte *r,png_byte *g,png_byte *b) // -1 <= x <= 1
{
  double i_nc,dr,dg,db;
  unsigned int i,n,nc,nd;

  if(x<-1.0 || x>1.0){
    *r=0x00;    *g=0x00;    *b=0x00;
    return -1;
  }
  
  n=(unsigned int)floor(pow(2,23)*(x+1.0));
  nc=(unsigned int)pow(2,21);
  i_nc=1.0/(double)nc;
  
  if(n<nc*1)      i=1;
  else if(n<nc*2) i=2;
  else if(n<nc*3) i=3;
  else if(n<nc*4) i=4;
  else if(n<nc*5) i=5;
  else if(n<nc*6) i=6;
  else if(n<nc*7) i=7;
  else if(n<nc*8) i=8;
  else {
    *r=ct1[8][0];    *g=ct1[8][1];    *b=ct1[8][2];
    return 0;
  }
    
  nd=n-nc*(i-1);
  dr=(double)(ct1[i][0]-ct1[i-1][0])*i_nc;
  dg=(double)(ct1[i][1]-ct1[i-1][1])*i_nc;
  db=(double)(ct1[i][2]-ct1[i-1][2])*i_nc;
  *r=(png_byte)floor((double)ct1[i-1][0]+dr*(double)nd);
  *g=(png_byte)floor((double)ct1[i-1][1]+dg*(double)nd);
  *b=(png_byte)floor((double)ct1[i-1][2]+db*(double)nd);
  
  return 0;  
}

