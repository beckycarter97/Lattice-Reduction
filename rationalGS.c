// Attempt to implement basic vector operations for rational vectors
//Does not work
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define n 3

typedef struct rationals{
  int num; // numerator
  int denom; // denominator
} RATIONAL;

typedef RATIONAL VECTOR[n]; // rational vector is array of n rationals
typedef VECTOR BASIS[n]; // basis is an array of n vectors
typedef struct gs_bas{ // a Gram Schmidt basis consists of the norms and the coefficients
  RATIONAL norms[n];
  RATIONAL mu[n][n];
} GS_BASIS;

// Basic operations for rational numbers
RATIONAL rat_add(RATIONAL x, RATIONAL y){
  RATIONAL z;
  z.num = (x.num * y.denom) + (y.num * x.denom);
  z.denom = x.denom * y.denom;

}

RATIONAL rat_sub(RATIONAL x, RATIONAL y){
  RATIONAL z;
  z.num = (x.num * y.denom) - (y.num * x.denom);
  z.denom = x.denom * y.denom;
}

RATIONAL rat_mult(RATIONAL x, RATIONAL y){
  RATIONAL z;
  z.num = x.num * y.num;
  z.denom = x.denom * y.denom;

  return z;
}

RATIONAL rat_div(RATIONAL x, RATIONAL y){
  RATIONAL z;

  z.num = x.num / y.num;
  z.denom = x.denom / y.denom;

  return z;
}

void print_vector(VECTOR v){
  printf("[");
  for (size_t i = 0; i < n; i++) {
    printf("%i/%i,",v[i].num, v[i].denom);
  }
  printf("]\n");
}

void set_equal(VECTOR *u, VECTOR v){
  for (size_t i = 0; i < n; i++) {
    (*u)[i] = v[i];
  }
}

RATIONAL vec_prod(VECTOR v, VECTOR w){
  RATIONAL vec_product = {0,0};

  for (size_t i = 0; i < n; i++) {
    RATIONAL temp = rat_mult(v[i],w[i]);
    vec_product = rat_add(vec_product, temp);
  }
  return vec_product;
}

void gram_schmidt(BASIS *b, GS_BASIS *b_star){
  int i,j,k;
  BASIS gs_basis;

  for(i = 0; i < n; i ++){
    printf("i = %i\n", i);
    set_equal(&gs_basis[i], (*b)[i]);

    for(j = 0; j < i; j++){
      printf("\t j = %i\n", j);
      RATIONAL temp1 = vec_prod((*b)[i], gs_basis[j]);
      printf("%i/%i\n", temp1.num, temp1.denom);
      RATIONAL temp2 = rat_div(temp1,(b_star->norms[j]));
      printf("mu%i%i = %i/%i\n", i, j, temp2.num, temp2.denom);
      b_star->mu[i][j] = temp2;

      for(k = 0; k < n; k ++){
        gs_basis[i][k] = rat_sub(gs_basis[i][k],           rat_mult((b_star->mu[i][j]),gs_basis[j][k])) ;
      }
    }
    printf("[");
    for(k = 0; k < n; k++){
      printf("%i/%i,", gs_basis[i][k].num, gs_basis[i][k].denom);
    }
    printf("]\n");
    b_star->norms[i] = vec_prod(gs_basis[i],gs_basis[i]);
    printf("Norm %i = %i/%i\n", i, b_star->norms[i].num, b_star->norms[i].denom);
  }
}

void main(){
   BASIS basis;
   BASIS *b;
   GS_BASIS orth_basis;
   GS_BASIS *b_star;

   VECTOR v1 = {{1,1},{1,1},{1,1}};
   VECTOR v2 = {{-1,1},{0,1},{2,1}};
   VECTOR v3 = {{3,1},{5,1},{6,1}};
   set_equal(&basis[0], v1);
   set_equal(&basis[1], v2);
   set_equal(&basis[2], v3);

   b = &basis;
   b_star = &orth_basis;

   gram_schmidt(b, b_star);

}
