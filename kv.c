// kv

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
//#include <unistd.h>

#define HTABLESIZE 10

typedef struct hash_node{
  int key;
  char *value;
  struct hash_node *next;
}hash_node;

hash_node *ht[HTABLESIZE];

void init_ht(){
  int i;
  for (i=0; i<HTABLESIZE; i++){
    ht[i] = NULL;
  }
}

void bad_command(){
  printf("bad command\n");
  return;
}

unsigned int hash(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    //printf("%d: x before\n", x);
    x = x % HTABLESIZE;
    //printf("%d: x after\n", x);
    return x ;
}

hash_node *create_hash_node(int key, char *value){
  //printf("----trying to insert %d %s------\n", key, value);
  hash_node *new_hn = (hash_node *) malloc(sizeof(hash_node));
  new_hn->key = key;
  value[strcspn(value, "\n")] = 0; //manage \n to avoid weird errors
  new_hn->value = (char *)malloc(sizeof(value));
  strcpy(new_hn->value, value);
  new_hn->next = NULL; //set this later if needed
  return new_hn;
}

void get_or_del(char *string, int b){
  //b = 1 for delete
  char *tempstr = strdup(string);
  char *k  = strsep(&tempstr, ",");
  if(tempstr != NULL){
    bad_command();
    return;
  }
  free(tempstr);
  char *ptr;
  long x;
  x = strtol(k, &ptr, 10);
  
  if (ptr!=NULL){
    bad_command();
    return;
  }
  int hash_key = hash(x);

  hash_node *temp_hn = ht[hash_key];

  while (temp_hn!=NULL){
    if (x == temp_hn->key){
      if(b){
        free(temp_hn);
        temp_hn = NULL;
      }else{
        printf("%d,%s\n", temp_hn->key, temp_hn->value);
      }
        return;
    }
    temp_hn = temp_hn->next;
  }
  printf("%ld not found\n", x);
  return;
}

void put(char *string){
  char *tempstr = strdup(string);
  char *k  = strsep(&tempstr, ",");
  char *v = strsep(&tempstr, ",");
  if(tempstr != NULL){
    bad_command();
    return;
  }
  free(tempstr);
  char *ptr;
  long x;
  x = strtol(k, &ptr, 10);
  
  if (ptr!=NULL || v==NULL){
    bad_command();
    return;
  }

  int hash_key = hash(x);
  
  hash_node *h = ht[hash_key];
  //printf("hash_key = %d\n", hash_key);
  //if no entry, store and return
  if ( h == NULL){
    ht[hash_key] = create_hash_node(x,v);
    //printf("created new key: %d, %s\n", x, v);
    return;
  } 
  
  //check if key exists already
  hash_node *temp_hn = ht[hash_key];
  hash_node *prev = NULL;
  while (temp_hn!=NULL){
    if (x == temp_hn->key){
        free(temp_hn->value);
        temp_hn->value = (char *)malloc(sizeof(v));
        strcpy(temp_hn->value,v);
        //printf("replaced key: %d, %s\n", x, v);
        return;
    }
    prev = temp_hn;
    temp_hn = temp_hn->next;
  }
  //key does not exist, append to end of list (temp_hn is NULL, use prev)
  prev->next = create_hash_node(x,v);
  //printf("created new key: %d, %s\n", x, v);
  
  return;
}

void read_from_database(){
  FILE *fptr;
  if(fptr = fopen("database.txt","r")){
    char *line;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, fptr)) != -1) {
        put(line);
    }
    if(line)
      free(line);
    fclose(fptr);
  }
  return;
}

void print_table(){
  int i;

  for (i=0; i<HTABLESIZE; i++){
    if(ht[i] != NULL){
      hash_node *temp_hn = ht[i];
      while (temp_hn!=NULL){
        printf("%d,%s\n", temp_hn->key, temp_hn->value);
        temp_hn = temp_hn->next;
      }
    }
  }
  return;
}

void dump(){
  FILE *fptr; 
  fptr = fopen("database.txt","w"); //do I need a backup?
  int i;

  for (i=0; i<HTABLESIZE; i++){
    if(ht[i] != NULL){
      hash_node *temp_hn = ht[i];
      while (temp_hn!=NULL){
        fprintf(fptr,"%d,%s\n", temp_hn->key, temp_hn->value);
        temp_hn = temp_hn->next;
      }
    }
  }
  fclose(fptr);
  return;
}

int main(int argc, char *argv[]){
  //initialize the hash table
  init_ht();

  //read stored k,v
  read_from_database();

  int i;

  //read in input kv pairs
  for (i=1; i<argc; i++){
    
    char *string = strdup(argv[i]);
    char *command  = strsep(&string, ",");
    
    switch(command[0]){
      case 'p':
        put(string);
        break;  
      case 'g':
        get_or_del(string, 0);
        break;
      case 'd':
        get_or_del(string, 1);
        break;
      case 'a':
        print_table();
        break;
      case 'c':
        init_ht();
        dump();
        break;
      default:
        bad_command();
        break;
    }
  }
  //dump into file
  dump();
  return 0;
}
