// To compile in command line use
// gcc GS.c -o GS -lm
// -lm needed to properly link to the math.h functions

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

#define n 3
#define delta 3/4 // 1/4 < delta < 1 used for Lovasz condition

/* Global Variables */
double basis[n][n] = {{1,1,1},{-1,0,2},{3,5,6}};
double orth_basis[n][n]; //Current GS orthogonalized vectors
double coeffs[n][n]; //GS coefficients
double orth_norms[n]; //Norms of GS orthogonalized vectors


/* Initialises orth_basis, coeffs and orth_norms to have all values 0 */
void init(){
  int i,j;

  for (i=0; i<n; i++){
    orth_norms[i] = 0;
    for (j=0; j<n; j++){
      orth_basis[i][j]=0;
      coeffs[i][j]=0;
    }
  }
}

double vec_prod(double v[n], double w[n]){
  /* Calculates the l2 vector product of two vectors v and w of length n*/
  double vec_product = 0;
  int i;

  for (i=0; i < n; i++){
    vec_product += v[i]*w[i];
  }

  return vec_product;
}

double norm_sq(double v[n]){
  /*Calculates l2 norm squared of vector v*/
  double norm;

  norm = vec_prod(v,v);

  return norm;
}

void print_current_basis(){
  for (size_t i = 0; i < n; i++) {
    printf("b_%li: [",i);
    for (size_t j = 0; j < n; j++) {
      printf("%lf,",basis[i][j]);
    }
    printf("]\n");
  }
}

void print_current_GS(){
  int i, j;

  for (i = 0; i < n; i++){
    printf("B_%i = %lf\n", i, orth_norms[i]);
  }

  for (i=0; i < n; i++){
    for (j = 0; j < i; j++){
      printf("mu_%i_%i = %lf\n", i,j,coeffs[i][j]);
    }
  }
}

void gram_schmidt(){
  int i,j,k;

  for (i = 0; i<n; i+=1){
    for(k=0; k<n; k+=1){
      orth_basis[i][k] = basis[i][k];
    }
    for (j = 0; j < i; j++) {
      coeffs[i][j] = vec_prod(basis[i], orth_basis[j])/orth_norms[j];
      for (k = 0; k < n; k++){
        orth_basis[i][k]-= coeffs[i][j]*orth_basis[j][k];
      }
    }
    orth_norms[i] = norm_sq(orth_basis[i]);
  }

  // print_current_GS();

}

void size_reduce(){
  for (size_t j = 1; j < n; j++) {
    for (size_t i = j-1; i > 0; i--) {
      int mu_i_j;
      mu_i_j = round(coeffs[j][i]);
      for (size_t k = 0; k < n; k++) {
        basis[j][k] -= mu_i_j*basis[i][k];
      }
    }
  }

}

void basic_lll(){
  // Calculate the Gram Schmidt orthogonalized vectors of the basis
  printf("Entering basic_lll\n");

  int k = 1;

  while (k < n-1) {
    print_current_basis();
    gram_schmidt();
    printf("k = %i\n",k);
    size_reduce();
    printf("\tsize reduced\n");

    int switched = FALSE;

    if (delta*orth_norms[k-1]
      <= (coeffs[k][k-1]*orth_norms[k-1] + orth_norms[k])) {
        // If Lovasz condition violated, switch b_k and b_k-1
        printf("\tSwitching\n");

        for (size_t j = 0; j < n; j++) {
          double temp = basis[k-1][j];
          basis[k-1][j] = basis[k][j];
          basis[k][j] = temp;
        }
        switched = TRUE;
        if(k > 1){
          k--;
        }
      }

      if (!switched){
        k++;
      }
  }

}

void main()
{
  init();
  basic_lll();

  printf("\n");
  print_current_basis();

}
