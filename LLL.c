// To compile in command line use
// gcc GS.c -o GS -lm
// -lm needed to properly link to the math.h functions

#include "LLL.h"
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

/* Calculates the l2 vector product of two vectors v and w of length n*/
double vec_prod(double v[n], double w[n]){
  double vec_product = 0;
  int i;

  for (i=0; i < n; i++){
    vec_product += v[i]*w[i];
  }

  return vec_product;
}

/*Calculates l2 norm squared of vector v*/
double norm_sq(double v[n]){
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

/* Calculates Gram-Schmidt orthogonalization of current basis */
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

  print_current_GS();

}

/* Calculates size reduction of mu_k_l if necessary */
void size_reduce(int k,int l){
  printf("Size reduce k=%i, l = %i\n", k, l);

  int i, j, r;
  double val;
  val = coeffs[k][l];

  if (fabs(val) > 0.5){
    r = round(val); // should probably actually check that l < k
    printf("%lf \t %i\n", val, r);
    for (i = 0; i < n; i++){ //should probably make a function for array subtraction
      basis[k][i] -= r*basis[l][i];
    }
    for (j = 0; j < l-1; j++){
      coeffs[k][j] -= r*coeffs[l][j];
    }
    coeffs[k][l] -= r;
    printf("New mukl = %lf\n", coeffs[k][l]);
  }
}

void lll_swap(int k){
  printf("LLL_swap k =%i\n", k);

  int i,j;
  double mu;
  double B;

  mu = coeffs[k][k-1];
  B = orth_norms[k] - pow(mu,2)*orth_norms[k-1];

  printf("\t mu: %lf, B: %lf\n", mu, B);

  coeffs[k][k-1] = mu*orth_norms[k-1]/B;

  orth_norms[k] = orth_norms[k-1]*orth_norms[k]/B;
  orth_norms[k-1] = B;

  for (i=0; i<n; i++){
    int temp = basis[k-1][i];
    basis[k-1][i] = basis[k][i];
    basis[k][i] = temp;

  }

  for (j = 0; j < k-1; j++){
    double temp = coeffs[k-1][j];
    coeffs[k-1][j] = coeffs[k][j];
    coeffs[k][j] = temp;
  }

  for (j = k; j<n; j++){
    double temp = coeffs[i][k];
    coeffs[i][k] = coeffs[i][k-1] - mu*coeffs[i][k];
    coeffs[i][k-1] = coeffs[k][k-1]*coeffs[i][k-1] + (1-mu*coeffs[k][k-1])*temp;
  }
}

void lll(){
  int k, l;

  k = 1;
  while (k<n) {
    printf("\n1\tk = %i:\n",k);
    print_current_basis();
    print_current_GS();

    size_reduce(k, k-1);
    
    if (orth_norms[k] < ((0.75 - pow(coeffs[k][k-1],2))*orth_norms[k-1])) {
      printf("2\n");
      lll_swap(k);
      if (k > 1) {
        k--;
      }
      continue;
    }

    for (l = k-2; l>=0; l--){
      size_reduce(k,l);
    }
    k++;
  }
}


void main()
{
  init();
  gram_schmidt();
  lll();

  printf("\n");
  print_current_basis();

}
