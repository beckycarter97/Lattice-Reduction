#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Global Variables */
int n=3;
// double basis[n][n]; // Current basis vectors
// double coeffs[n][n]; //GS coefficients
// double orth_norms[n]; //Norms of GS orthogonalized vectors


// basis = {{1,1,1},{-1,0,2},{3,5,6}};

double vec_prod(double v[n], double w[n]){
  /* Calculates the l2 vector product of two vectors v and w of length n*/
  double accum = 0;
  double vec_product;
  int i;

  for (i=0; i < n; i++){
    accum += v[i]*w[i];
  }

  vec_product = accum/n;

  return vec_product;
}

double norm(double v[n])
{
  /*Calculates l2 norm of vector v*/
  double norm;

  norm = vec_prod(v,v);

  return norm;
}

void GramSchmidt(double basis[n][n])
{
  int i,j,k;
  double val1, val2, val;
  double orth_basis[n][n]; //Current GS orthogonalized vectors
  double coeffs[n][n]; //GS coefficients
  double orth_norms[n]; //GS norms

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
    orth_norms[i] = norm(orth_basis[i]);
  }

  for (i = 0; i < n; i++){
    printf("b_i*: [");
    for (j = 0; j < n; j++){
        printf("%lf,",orth_basis[i][j]);
    }
    printf("]\n");
  }

  for (i=0; i < n; i++){
    for (j = 0; j < i; j++){
      printf("mu_%i_%i = %lf\n", i,j,coeffs[i][j]);
    }
  }
}


void main()
{
  double basis[3][3] = {{1,1,1},{-1,0,2},{3,5,6}};
  GramSchmidt(basis);

}
