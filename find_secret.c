#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<stdint.h>
#include <sys/ptrace.h>

int main(int argc, char *argv[]){
  if(argc != 2){
    printf("Pass the pid as argument!\n");
    return -1;
  }

  uint8_t* secret_addr = NULL;


  // /proc/$pid/maps: address maps of a process
  char path[30];
  sprintf(path, "/proc/%s/maps", argv[1]);
  printf("open %s...\n",path);


  //read file and get heap address
  FILE* fp = fopen(path,"r");
  char* line = NULL;
  size_t len = 0;
  ssize_t read;
  int count=3;
  char* addr = malloc(8);

  if(!fp){
    printf("Maps file for this pid is not existed!");
    return -1;
  }
  while ((read = getline(&line, &len, fp)) != -1){
    printf("%s",line);
    //get heap address
    if(line[strlen(line)-2]==']') {
	memcpy(addr,line,8);
        secret_addr = (uint8_t*)strtol(addr, NULL, 16);
	secret_addr += 0x2E0;  		//offset of secret
        break;
    }
  }
  printf("\nSecret address:%8lx\n",(long)secret_addr);
  

  //read secret 
  long* data;
  long ret;
  int i, j;
  pid_t pid;
  sscanf(argv[1], "%u", &pid);	  //pid: char* -> pid_t

  ptrace(PTRACE_ATTACH, pid, NULL, NULL);
  perror("Attach");
  printf("Secret:\n");
  for(i=0;i<4;i++) {
    ret = ptrace(PTRACE_PEEKDATA, pid, (void*)secret_addr+8*i, NULL);
    //output in big endian 
    for(j=0;j<8;j++) {
      printf("%x ",(uint8_t)ret%256);
      ret >>= 8;
    }
  }
  printf("\n");
  perror("Peekdata");
  ptrace(PTRACE_DETACH, pid, NULL, NULL);
  perror("Detach");

  return 0;
}
