#include <iostream.h>
#include <stdio.h>
main()
{
  FILE *f;
  f=fopen("temperat.pal","a");
  int i;
  for (i=1;i<256;i=i+3)
  {
     fprintf(f,"%x %x %x %s",255,255-i,0,"\n");
     cout<<"x\n";
  }
  fclose(f);
  
}