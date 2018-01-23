#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int do_stuff(){
  return 0;
}

int main(){
  int f = fork();
  if (!f){
    do_stuff();
  }
  else{
    int status;
    int cpid = wait(&status);
  }
  return 0;
}
