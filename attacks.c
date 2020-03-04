/**
 * Fault attack against RSA with CRT
 *  
 * Please supply the following information: 
 * \author Turan Kavak
 * 
 * Insert your code here and mail in this file ONLY.
 *
 * Do not change the function prototypes
 *
 * You can make use of all functions of the GMP library 
 */

#include "attacks.h"

unsigned int bellcore_attack(const mpz_t n, const mpz_t y, const mpz_t y_faulty,
  mpz_t p, mpz_t q)
{
  // Hilfvariable für y'-y
  mpz_t tmp;
  mpz_init(tmp);
  mpz_sub(tmp, y_faulty, y);
  // Berechnen von q und p
  mpz_gcd(q, tmp,n);
  mpz_cdiv_q(p, n, q);
  // Speicherfreigabe
  mpz_clear(tmp);  
  return 1;
}
  
 
unsigned int lenstra_attack(const mpz_t n, const mpz_t e, const mpz_t x, const mpz_t y_faulty,
  mpz_t p, mpz_t q)
{
  // Hilfvariable für y'^e - x
  mpz_t tmp;
  mpz_init(tmp);
  mpz_powm(tmp, y_faulty, e, n);  
  mpz_sub(tmp, tmp, x);
  // Berechnen von p und q
  mpz_gcd(q, tmp,n);
  mpz_cdiv_q(p, n, q);
  // Speicherfreigabe
  mpz_clear(tmp);  
  return 1;
}

/**
 * Bellcore (fault1.txt):
 *
 * p = f9555b790d60dcb3fdcdf464b88ab7bb629bfce037f4154927df19fcdb1b4c7327d41b17d848455cffbda7e8080c08600be3af126df6c481ab25da70bec471c0fb
 *
 * q = 9f44ddf28f05904455669a629df988adf203812f56aa8047c7db9bb7b4e61dd67b027e80d8700a77471943cc76370ced07056ef808a12b2a467c159e586c33
 *
 * Lenstra (fault2.txt):
 * 
 * p = d4693216ca3210f1491477d556e709141f6b5ea57e8b64a51011190d607b6b92a601857e4ad26e2b45123804ebdd08ccd15b0e50edcdc8754d5b2bb99dc8286087
 *
 * q = eacd987fce4c2815b8e1f6557a4120cd822763baa732e6fbd2d35d61b85f8278263ce068cddf6099ba885cda0b4ed1c2374de5d34b265fec3358611905ae81
 *
 */

