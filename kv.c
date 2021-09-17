// kv

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
//#include <unistd.h>

#define HTABLESIZE 1000

typedef struct{
  int key;
  char *value;
}ht;

ht* hash_table[HTABLESIZE];

unsigned int hash(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    //printf("%d: x before\n", x);
    x = x % HTABLESIZE;
    //printf("%d: x after\n", x);
    return x ;
}

int main(int argc, char *argv[]){
  
  FILE *fptr;
  fptr = fopen("database.txt","r");
  if (fptr) {
    char *line;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, fptr)) != -1) {
        printf("Retrieved line of length %zu:\n", read);
        printf("%s\n", line);
    }
    if(line)
      free(line);
    
  }
  fclose(fptr);
  int i;
  fptr = fopen("database.txt","w"); //do I need a backup?
  //read in input kv pairs
  for (i=1; i<argc; i++){
    printf("%s\n", argv[i]);

    //    char *k;
    //char *v;
    char *string  = argv[i];
    char *tempstr = strdup(string);
    char *k  = strsep(&tempstr, ",");
    char *v = strsep(&tempstr, ",");

    assert(tempstr == NULL);
    int x = atoi(k);
    printf("hash: %d\n ",hash(x));
    printf("%s   %s\n", k, v);
    fprintf(fptr,"%s,%s\n", k,v);
    free(tempstr);
    
  }
  fclose(fptr);
  return 0;
}
