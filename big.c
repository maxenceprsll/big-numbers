#include "big.h"

#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void bign_create_empty(struct bign *self) {
  self->data=NULL;
  self->capacity=0;
  self->size=0;
}

void bign_enlarge(struct bign *self){
  uint32_t *data=calloc(self->capacity*2+1,sizeof(uint32_t));
  for(size_t i=0;i<self->capacity;i++){
    data[i]=self->data[i];
  }
  if(self->size>0){
    free(self->data);
  }
  self->data=data;
  self->capacity=self->capacity*2+1;
}

void bign_create_from_value(struct bign *self, uint32_t val) {
  bign_create_empty(self);
  bign_enlarge(self);
  self->size++;
  self->data[0]=val;
}

size_t str_length(const char *str){
  size_t n=0;
  while(str[n]!='\0'){
    n++;
  }
  return n;
}

uint32_t hex_to_int(const char cha){
  if(cha>='0'&&cha<='9'){
    return cha-'0';
  }else if(cha>='A'&&cha<='Z'){
    return cha-'A'+10;
  } return cha-'a'+10;
}

uint32_t str_to_int(const char *str,int end){
  uint32_t n=0;
  uint32_t pow=1;
  for(int i=end;i>=(end-8>=0?end-8:0);i--){
    n+=hex_to_int(str[i])*pow;
    pow*=16;
  }
  return n;
}

void bign_create_from_string(struct bign *self, const char *str) {
  bign_create_empty(self);
  size_t lStr=str_length(str);
  int end=(lStr%8==0)?7:lStr%8-1;
  int i=(lStr%8==0)?lStr/8-1:lStr/8;
  while((int)self->capacity<=i+1){
        bign_enlarge(self);
      }
  self->size=i+1;
  int first=0;
  while(i>=0){
    uint32_t n=str_to_int(str,end);
    if(n!=0||i==0||first){
      first=1;
      self->data[i]=n;
    }else{
      self->size--;
    }
    i--;
    end+=8;
  }
}

void bign_copy_from_other(struct bign *self, const struct bign *other) {
  if(self->size!=0){
    free(self->data);
  }
  bign_create_empty(self);  size_t i=0;
  while(self->size<other->size){
    if(self->size==self->capacity){
      bign_enlarge(self);
    }
    self->data[i]=other->data[i];
    self->size++;
    i++;
  }
}

void bign_move_from_other(struct bign *self, struct bign *other) {
  bign_destroy(self);
  self->data=other->data;
  self->size=other->size;
  self->capacity=other->capacity;
  bign_create_empty(other);
}

void bign_destroy(struct bign *self) {
  if(self->size>0){
    free(self->data);
    self->size=0;
  }
  self->data=NULL;
  self->capacity=0;
}

void bign_print(const struct bign *self) {
  printf("bign : ");
  for(size_t i=0;i<self->size;i++){
    printf("%u ",self->data[i]);
  }
}

int bign_cmp(const struct bign *lhs, const struct bign *rhs) {
  if(lhs->size<rhs->size){
    return -1;
  }else if(lhs->size>rhs->size){
    return 1;
  }
  for(int i=lhs->size-1;i>-1;i--){
    if(lhs->data[i]!=rhs->data[i]){
      return (lhs->data[i]>rhs->data[i])?1:-1;
    }
  }
  return 0;
}

int bign_cmp_zero(const struct bign *self) {
  if(self->size>0){
    return self->data[0];
  }
  return 0;
}

void bign_add(struct bign *self, const struct bign *lhs, const struct bign *rhs) {
  int64_t c=0;
  struct bign res;
  bign_create_empty(&res);
  const struct bign *min=(lhs->size<=rhs->size)?lhs:rhs;
  printf("min :");bign_print(min);printf("\n");
  const struct bign *max=(lhs->size>rhs->size)?lhs:rhs;
  printf("max :");bign_print(max);printf("\n");
  while(res.capacity<min->size+max->size+1){
    bign_enlarge(&res);
  }
  int64_t s;
  for(size_t i=0;i<max->size;i++){
    printf("c%zu: %llu\n",i,c);
    s=(int64_t)(min->data[i])+(int64_t)(((i<min->size)?max->data[i]:0))+c;
    printf("s%zu: %llu\n",i,s);
    res.data[i]=s%0x100000000; 
    if(i<max->size-1){
      c=s/0x100000000;
    }
    res.size++;
  }
  printf("c : %lld\n",c);
  if(c!=0){
    res.data[max->size]=c;
    res.size++;
  }
  bign_move_from_other(self,&res);
  printf("test : ");bign_print(self);printf("\n");
}


void bign_sub(struct bign *self, const struct bign *lhs, const struct bign *rhs) {
}

void bign_mul(struct bign *self, const struct bign *lhs, const struct bign *rhs) {
}


void bign_mul_karatsuba(struct bign *self, const struct bign *lhs, const struct bign *rhs) {
}

void bign_mul_short(struct bign *self, const struct bign *lhs, uint32_t rhs) {
}

// https://janmr.com/blog/2012/11/basic-multiple-precision-short-division/
void bign_div_short(struct bign *quo, uint32_t *rem, const struct bign *lhs, uint32_t rhs) {
}


// https://janmr.com/blog/2014/04/basic-multiple-precision-long-division/
void bign_div(struct bign *quo, struct bign *rem, const struct bign *lhs, const struct bign *rhs) {
}


void bign_exp(struct bign *self, const struct bign *lhs, const struct bign *rhs) {
}





// int main(int argc, char const *argv[]){
//   struct bign b;
//   struct bign b2;
//   bign_create_from_string(&b,"1");
//   bign_create_from_string(&b2,"2");
//   printf("cmp:%d\n",bign_cmp(&b,&b2));
//   bign_destroy(&b);
//   bign_destroy(&b2);
//   return 0;
// }








/*
 *
 * bigz
 *
 */

void bigz_create_empty(struct bigz *self) {
  self->n.data=NULL;
  self->n.capacity=0;
  self->n.size=0;
  self->positive=true;
}

void bigz_enlarge(struct bigz *self){
  uint32_t *data=calloc(self->n.capacity*2+1,sizeof(uint32_t));
  for(size_t i=0;i<self->n.capacity;i++){
    data[i]=self->n.data[i];
  }
  if(self->n.size>0){
    free(self->n.data);
  }
  self->n.data=data;
  self->n.capacity=self->n.capacity*2+1;
}

void bigz_create_from_value(struct bigz *self, int32_t val) {
  bigz_create_empty(self);
  bigz_enlarge(self);
  self->n.size++;
  if(val<0){
    val=(uint32_t)(val*-1);
    self->positive=false;
  }
  self->n.data[0]=val;
}

uint32_t cha_to_int(const char cha){
  if(cha>='0'&&cha<='9'){
    return cha-'0';
  }else if(cha>='A'&&cha<='Z'){
    return cha-'A'+10;
  } return cha-'a'+10;
}

uint32_t all_to_int(const char *str,int end,const unsigned base){
  uint32_t n=0;
  uint32_t pow=1;
  int c=0;
  if(str[end-8]=='-'){
    c++;
  }
  for(int i=end;i>=(end-8>=0+c?end-8+c:0);i--){
    n+=cha_to_int(str[i])*pow;
    pow*=base;
  }
  return n;
}

void bigz_create_from_string(struct bigz *self, const char *str, unsigned base) {
  bigz_create_empty(self);
  size_t lStr=str_length(str);
  int end=(lStr%8==0)?7:lStr%8-1;
  int i=(lStr%8==0)?lStr/8-1:lStr/8;
  while((int)self->n.capacity<=i+1){
        bigz_enlarge(self);
      }
  self->n.size=i+1;
  int first=0;
  while(i>=0){
    uint32_t n=all_to_int(str,end,base);
    if(n!=0||i==0||first){
      first=1;
      self->n.data[i]=n;
    }else{
      self->n.size--;
    }
    i--;
    end+=8;
  }
}

void bigz_copy_from_other(struct bigz *self, const struct bigz *other) {
}

void bigz_move_from_other(struct bigz *self, struct bigz *other) {
}

void bigz_destroy(struct bigz *self) {
}

void bigz_print(const struct bigz *self) {
}

int bigz_cmp(const struct bigz *lhs, const struct bigz *rhs) {
  return 0;
}

int bigz_cmp_zero(const struct bigz *self) {
  if(self->n.size==1&&self->n.data[0]==0){
    return 0;
  }
  return self->positive;
}

void bigz_add(struct bigz *self, const struct bigz *lhs, const struct bigz *rhs) {
}

void bigz_sub(struct bigz *self, const struct bigz *lhs, const struct bigz *rhs) {
}

void bigz_mul(struct bigz *self, const struct bigz *lhs, const struct bigz *rhs) {
}

void bigz_div(struct bigz *quo, struct bigz *rem, const struct bigz *lhs, const struct bigz *rhs) {
}

void bigz_gcd(struct bigz *self, const struct bigz *lhs, const struct bigz *rhs) {
}
