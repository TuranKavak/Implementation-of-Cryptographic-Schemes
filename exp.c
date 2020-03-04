/* -*- Mode: C; c-file-style: "bsd" -*- */

/*
 * Name:        Turan Kavak
 * 
 * 
 * 
 * Implementierung kryptographischer Verfahren (SS 17)
 * Project I -- Exponentiation
 * 
 * C file for exercise 7 from project_exponentiation_SS17.pdf
 */

/*
  -- PLEASE INSERT RESULTS HERE --
  
user@VM-Linux:~/Desktop$ gcc exp.c -o exp -O3 -Wall -l gmp 
user@VM-Linux:~/Desktop$ ./exp
Testing algorithms:
Binary works properly
k-ary works properly
Improved k-ary works properly
Sliding window works properly
double & add works properly
NAF double & add works properly

Binary algorithm, 160 bits
Operations per exponentiation: 158.1 S, 79.6 M
Time per exponentiation: 399 microseconds

Binary algorithm, 1024 bits
Operations per exponentiation: 1021.8 S, 511.8 M
Time per exponentiation: 1254 microseconds

Double and Add algorithm, 160 bits
Operations per point multiplication: 157.6 D, 78.5 A
Time per point multiplication: 648 microseconds

NAF-Double and Add algorithm, 160 bits
Operations per point multiplication: 158.0 D, 53.1 A
Time per point multiplication: 583 microseconds

k-ary algorithm, 160 bits, k = 3
Operations per exponentiation: 158.4 S, 57.5 M
Time per exponentiation: 153 microseconds

k-ary algorithm, 160 bits, k = 4
Operations per exponentiation: 156.9 S, 52.0 M
Time per exponentiation: 151 microseconds

k-ary algorithm, 160 bits, k = 5
Operations per exponentiation: 155.9 S, 60.0 M
Time per exponentiation: 156 microseconds

k-ary algorithm, 160 bits, k = 6
Operations per exponentiation: 156.7 S, 87.0 M
Time per exponentiation: 182 microseconds

k-ary algorithm, 160 bits, k = 7
Operations per exponentiation: 154.9 S, 147.0 M
Time per exponentiation: 233 microseconds

k-ary algorithm, 1024 bits, k = 4
Operations per exponentiation: 1020.8 S, 268.0 M
Time per exponentiation: 900 microseconds

k-ary algorithm, 1024 bits, k = 5
Operations per exponentiation: 1020.8 S, 232.9 M
Time per exponentiation: 877 microseconds

k-ary algorithm, 1024 bits, k = 6
Operations per exponentiation: 1020.7 S, 230.9 M
Time per exponentiation: 875 microseconds

k-ary algorithm, 1024 bits, k = 7
Operations per exponentiation: 1020.7 S, 270.7 M
Time per exponentiation: 911 microseconds

k-ary algorithm, 1024 bits, k = 8
Operations per exponentiation: 1017.0 S, 380.0 M
Time per exponentiation: 999 microseconds

Improved k-ary algorithm, 160 bits, k = 3
Operations per exponentiation: 160.0 S, 56.5 M
Time per exponentiation: 152 microseconds

Improved k-ary algorithm, 160 bits, k = 4
Operations per exponentiation: 160.2 S, 46.9 M
Time per exponentiation: 147 microseconds

Improved k-ary algorithm, 160 bits, k = 5
Operations per exponentiation: 159.8 S, 47.0 M
Time per exponentiation: 145 microseconds

Improved k-ary algorithm, 160 bits, k = 6
Operations per exponentiation: 159.9 S, 58.0 M
Time per exponentiation: 158 microseconds

Improved k-ary algorithm, 160 bits, k = 7
Operations per exponentiation: 160.1 S, 86.0 M
Time per exponentiation: 180 microseconds

Improved k-ary algorithm, 1024 bits, k = 4
Operations per exponentiation: 1024.2 S, 262.9 M
Time per exponentiation: 895 microseconds

Improved k-ary algorithm, 1024 bits, k = 5
Operations per exponentiation: 1023.9 S, 219.9 M
Time per exponentiation: 861 microseconds

Improved k-ary algorithm, 1024 bits, k = 6
Operations per exponentiation: 1024.0 S, 201.9 M
Time per exponentiation: 848 microseconds

Improved k-ary algorithm, 1024 bits, k = 7
Operations per exponentiation: 1024.0 S, 209.7 M
Time per exponentiation: 857 microseconds

Improved k-ary algorithm, 1024 bits, k = 8
Operations per exponentiation: 1024.1 S, 255.0 M
Time per exponentiation: 895 microseconds

Sliding window algorithm, 160 bits, k = 3
Operations per exponentiation: 160.1 S, 43.4 M
Time per exponentiation: 145 microseconds

Sliding window algorithm, 160 bits, k = 4
Operations per exponentiation: 159.9 S, 39.3 M
Time per exponentiation: 142 microseconds

Sliding window algorithm, 160 bits, k = 5
Operations per exponentiation: 159.9 S, 41.7 M
Time per exponentiation: 140 microseconds

Sliding window algorithm, 160 bits, k = 6
Operations per exponentiation: 160.0 S, 54.2 M
Time per exponentiation: 153 microseconds

Sliding window algorithm, 160 bits, k = 7
Operations per exponentiation: 160.1 S, 83.2 M
Time per exponentiation: 176 microseconds

Sliding window algorithm, 1024 bits, k = 4
Operations per exponentiation: 1023.9 S, 212.5 M
Time per exponentiation: 861 microseconds

Sliding window algorithm, 1024 bits, k = 5
Operations per exponentiation: 1024.0 S, 185.8 M
Time per exponentiation: 837 microseconds

Sliding window algorithm, 1024 bits, k = 6
Operations per exponentiation: 1024.0 S, 177.5 M
Time per exponentiation: 828 microseconds

Sliding window algorithm, 1024 bits, k = 7
Operations per exponentiation: 1024.0 S, 191.3 M
Time per exponentiation: 847 microseconds

Sliding window algorithm, 1024 bits, k = 8
Operations per exponentiation: 1024.0 S, 241.1 M
Time per exponentiation: 894 microseconds

 */

#include <stdio.h>
#include <time.h>
#include <assert.h>

#include <stdlib.h>	/* Zusätzlich benötigte Bibliothek */

#include <gmp.h> /* GNU Multi Precision library */

int ecc_op_double(mpz_t resultX, mpz_t resultY, mpz_t a, mpz_t b, mpz_t p, mpz_t inputX, mpz_t inputY);
int ecc_op_add(mpz_t resultX, mpz_t resultY, mpz_t a, mpz_t b, mpz_t p, mpz_t operand1_X, mpz_t operand1_Y, mpz_t operand2_X, mpz_t operand2_Y);

/* Prototypes for functions that you need to complete at the end of this file */
int get_random_exponent(mpz_t e, int bits);
int exponentiate_binary(mpz_t result, mpz_t g, mpz_t e, mpz_t modulus, long *count_S, long *count_M);
int exponentiate_k_ary(mpz_t result, mpz_t g, mpz_t e, mpz_t modulus, int k, long *count_S, long *count_M);
int exponentiate_improved_k_ary(mpz_t result, mpz_t g, mpz_t e, mpz_t modulus, int k, long *count_S, long *count_M);
int exponentiate_sliding_window(mpz_t result, mpz_t g, mpz_t e, mpz_t modulus, int k, long *count_S, long *count_M);

int ecc_double_add(mpz_t resultX, mpz_t resultY, mpz_t a, mpz_t b, mpz_t p, mpz_t q, mpz_t inputX, mpz_t inputY, mpz_t factor, long *count_D, long *count_A);
int ecc_naf_double_add(mpz_t resultX, mpz_t resultY, mpz_t a, mpz_t b, mpz_t p, mpz_t q, mpz_t inputX, mpz_t inputY, mpz_t factor, long *count_D, long *count_A);

int bintodec(char *bitstring, int algo); /* Zusätzlich benötigte Methode */

/* define a global random state here */
gmp_randstate_t state;

int main(int argc, char *argv[])
{
	enum {binary, ecc_binary, ecc_naf_binary, k_ary, improved_k_ary, sliding_window} algorithm;
	struct {int bits; int min_k; int max_k;} test_cases[] = {{160, 3, 7}, {1024, 4, 8}};
	int number_of_operations = 100; /* change this if necessary to adjust the number of exponentiations*/
	int r;

	mpz_t modulus, g, e, result;

	mpz_init(modulus);
	mpz_init(g);
	mpz_init(e);
	mpz_init(result);
	/* you can use this fixed modulus, so you do not need to choose a 1024 bit prime number */
	r = mpz_set_str(modulus, "FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD1"
			"29024E088A67CC74020BBEA63B139B22514A08798E3404DD"
			"EF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245"
			"E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7ED"
			"EE386BFB5A899FA5AE9F24117C4B1FE649286651ECE65381"
			"FFFFFFFFFFFFFFFF", 16);
	assert (r == 0);
	/* 	for simplicity we use a fixed base g but this does not mean, that you can 
		assume to always work with this single fixed base like in assignment 6 */
	r = mpz_set_str(g, "FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210"
			"FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210"
			"FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210"
			"FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210"
			"FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210"
			"FEDCBA9876543210", 16);
	assert (r == 0);


	/* TO BE IMPLEMENTED: initialize and seed the GMP random number generator here*/
	gmp_randinit_default(state);	 // INIT von state mit Algo

	unsigned long seed;
	FILE *f;
	f = fopen("/dev/urandom", "r");		
	if(f != NULL)
	{
		// Lese Seed aus /dev/urandom
		if (fread(&seed, sizeof(seed), 1, f) != 1)		
		{
		printf("Fehler beim ausführen von fread\n");
		exit(0);
		}
	}
	else
	{
		printf("Fehler beim ausführen von fopen\n");
		exit(0);
	}
	fclose(f);

	gmp_randseed_ui(state, seed);	// Setze Seed 


	//---------------------------------------------------
	// Testing algorithms
	//---------------------------------------------------
	mpz_t test_e, test_res;
	int test_k = 3;
	long cs, cm;

	mpz_init(test_e);
	mpz_init(test_res);

	// initialize test exponent
	r = mpz_set_str(test_e, "FFFFFC90FDAA22168C234C4C6628B80DC1CD1"
					"29024E088A67CC74020BBEA63B139B22514A08798E3404DD"
					"EF9519B3CD3A431A6DF25F14374FE1356D6D51C245"
					"E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7ED"
					"EE386BFB5A899FA5AE9F649286651ECE65381"
					"FFF125", 16);
	
	// ecc inits
	mpz_t a, b, p, q; // brainpoolP160r1
	long cA, cD;
	mpz_t basePoint_X, basePoint_Y;
	mpz_t testFactor;
	mpz_t test_resultPoint_X, test_resultPoint_Y;
	
	mpz_t resultPoint_X, resultPoint_Y;
	// init and set of curve parameters and point coordinates
	mpz_inits(a, b, p, q, basePoint_X, basePoint_Y, testFactor, test_resultPoint_X, test_resultPoint_Y, resultPoint_X, resultPoint_Y, NULL);
	
	mpz_set_str(p, "E95E4A5F737059DC60DFC7AD95B3D8139515620F", 16);
	mpz_set_str(a, "340E7BE2A280EB74E2BE61BADA745D97E8F7C300", 16);
	mpz_set_str(b, "1E589A8595423412134FAA2DBDEC95C8D8675E58", 16);
	
	mpz_set_str(q, "E95E4A5F737059DC60DF5991D45029409E60FC09", 16);
	
	mpz_set_str(basePoint_X, "BED5AF16EA3F6A4F62938C4631EB5AF7BDBCDBC3", 16);
	mpz_set_str(basePoint_Y, "1667CB477A1A8EC338F94741669C976316DA6321", 16);
	
	mpz_set_str(testFactor, "e95e4a5f737059dc4e7cb133c1d94f942460fc09", 16);
	
	mpz_set_str(test_resultPoint_X, "30b15f81552a92105f9da2527749edaf21f5571f", 16);
	mpz_set_str(test_resultPoint_Y, "d7d9d0ab3d671851282809a491e912d8851e379f", 16);
	
	assert (r == 0);

	// calculate g^e for comparison
	mpz_powm(test_res,g,test_e, modulus);

	printf("Testing algorithms:\n");
	
	// compare results of binary and mpz_pow
	r = exponentiate_binary(result, g, test_e, modulus,
			&cs, &cm);
	if (!mpz_cmp(result, test_res)){
		printf("Binary works properly\n");
	} else {
		printf("Binary failed\n");
	}
	mpz_set_ui(result,0);

	// compare results of k-ary and mpz_pow
	r = exponentiate_k_ary(result, g, test_e, modulus, test_k,
			&cs, &cm);
	if (!mpz_cmp(result, test_res)){
		printf("k-ary works properly\n");
	} else {
		printf("k-ary failed\n");
	}
	mpz_set_ui(result,0);

	// compare results of improved k-ary and mpz_pow
	r = exponentiate_improved_k_ary(result, g, test_e, modulus,
			test_k, &cs, &cm);
	if (!mpz_cmp(result, test_res)){
		printf("Improved k-ary works properly\n");
	} else {
		printf("Improved k-ary failed\n");
	}
	mpz_set_ui(result,0);

	// compare results of sliding window and mpz_pow
	r = exponentiate_sliding_window(result, g, test_e, modulus,
			test_k, &cs, &cm);
	if (!mpz_cmp(result, test_res)){
		printf("Sliding window works properly\n");
	} else {
		printf("Sliding window failed\n");
	}
	mpz_set_ui(result,0);

	// free memory
	mpz_clear(test_e);
	mpz_clear(test_res);
	
	// ecc testing
	
	r = ecc_double_add(resultPoint_X, resultPoint_Y, a, b, p, q, basePoint_X, basePoint_Y, testFactor, &cD, &cA);
	if (mpz_cmp(test_resultPoint_X, resultPoint_X) == 0 && mpz_cmp(test_resultPoint_Y, resultPoint_Y) == 0){
		printf("double & add works properly\n");
	} else {
		printf("double & add failed\n");
	}
	
	mpz_set_ui(resultPoint_X, 0);
	mpz_set_ui(resultPoint_Y, 0);
	
	r = ecc_naf_double_add(resultPoint_X, resultPoint_Y, a, b, p, q, basePoint_X, basePoint_Y, testFactor, &cD, &cA);
	if (mpz_cmp(test_resultPoint_X, resultPoint_X) == 0 && mpz_cmp(test_resultPoint_Y, resultPoint_Y) == 0){
		printf("NAF double & add works properly\n\n");
	} else {
		printf("NAF double & add failed\n\n");
	}
	
		
	//---------------------------------------------------
	// Testing Performance
	//---------------------------------------------------
	
	for (algorithm = binary; algorithm <= sliding_window; algorithm++) {
		int i;
		
		for (i = 0; i < sizeof test_cases/sizeof test_cases[0]; i++) {
			int bits = test_cases[i].bits;
			int min_k = test_cases[i].min_k;
			int max_k = test_cases[i].max_k;
			int k;
			
			if (algorithm == binary || algorithm == ecc_binary || algorithm == ecc_naf_binary) {
				min_k = 1;
				max_k = 1;
				
				if((algorithm == ecc_binary || algorithm == ecc_naf_binary) && bits > 160){
					continue;
				}
			}
			
			for (k = min_k; k <= max_k; k++) {
				long count_S;
				long count_M;
				clock_t start_time, end_time;
				int j;
				
				switch (algorithm) {
				case binary:
					printf("Binary algorithm, %i bits\n", bits);
					break;
				case ecc_binary:
					printf("Double and Add algorithm, %i bits\n", bits);
					break;
				case ecc_naf_binary:
					printf("NAF-Double and Add algorithm, %i bits\n", bits);
					break;
				case k_ary:
					printf("k-ary algorithm, %i bits, k = %i\n", bits, k);
					break;

				case improved_k_ary:
					printf("Improved k-ary algorithm, %i bits, k = %i\n", bits, k);
					break;

				case sliding_window:
					printf("Sliding window algorithm, %i bits, k = %i\n", bits, k);
					break;
				}

				count_S = 0;
				count_M = 0;
				start_time = clock();
				
				for (j = 0; j < number_of_operations; j++) {
					r = get_random_exponent(e, bits);
					assert(r == 0);

					switch (algorithm) {
					case binary:
						r = exponentiate_binary(result, g, e, modulus, &count_S, &count_M);
						break;

					case k_ary:
						r = exponentiate_k_ary(result, g, e, modulus, k, &count_S, &count_M);
						break;

					case improved_k_ary:
						r = exponentiate_improved_k_ary(result, g, e, modulus, k, &count_S, &count_M);
						break;
								    
					case sliding_window:
						r = exponentiate_sliding_window(result, g, e, modulus, k, &count_S, &count_M);
						break;
						
					case ecc_binary:
						r = ecc_double_add(resultPoint_X, resultPoint_Y, a, b, p, q, basePoint_X, basePoint_Y, e, &count_S, &count_M);
						break;
						
					case ecc_naf_binary:
						r = ecc_naf_double_add(resultPoint_X, resultPoint_Y, a, b, p, q, basePoint_X, basePoint_Y, e, &count_S, &count_M);
						break;
					}
					

					if (r == 1) {
						printf("TO BE IMPLEMENTED\n\n");
						break;
					}

					assert(r == 0);
					
					/* We ignore the exponentiation result in "result" */
				}
				
				end_time = clock();

				if (r == 0) {
					if(algorithm == ecc_binary || algorithm == ecc_naf_binary){
						printf("Operations per point multiplication: %.1f D, %.1f A\n",
					       ((double) count_S)/number_of_operations,
					       ((double) count_M)/number_of_operations);
					printf("Time per point multiplication: %.0f microseconds\n\n", ((double) (end_time - start_time))/CLOCKS_PER_SEC*1000000/number_of_operations);
					}else{
						printf("Operations per exponentiation: %.1f S, %.1f M\n",
					       ((double) count_S)/number_of_operations,
					       ((double) count_M)/number_of_operations);
					printf("Time per exponentiation: %.0f microseconds\n\n", ((double) (end_time - start_time))/CLOCKS_PER_SEC*1000000/number_of_operations);
					}
				}
			}
		}
	}

	mpz_clear(modulus);
	mpz_clear(g);
	mpz_clear(e);
	mpz_clear(result);
	mpz_clears(a, b, p, q, basePoint_X, basePoint_Y, testFactor, test_resultPoint_X, test_resultPoint_Y, resultPoint_X, resultPoint_Y, NULL);
	return 0;
}

/* Performs an ecc point doubling on the elliptic curve defined by a, b, p. The input point is doubled and written into the result point.

	--> DO NOT TOUCH THIS METHOD! <--

 */
int ecc_op_double(mpz_t resultX, mpz_t resultY, mpz_t a, mpz_t b, mpz_t p, mpz_t inputX, mpz_t inputY)
{
	// init
	mpz_t s, t1, t2;
	mpz_inits(s, t1, t2, NULL);
	// calculate s by calculating nominator and denominator separately
	mpz_mul(t1, inputX, inputX);
	mpz_mul_ui(t1, t1, 3);
	mpz_add(t1, t1, a);
	
	mpz_mul_ui(t2, inputY, 2);
	
	mpz_invert(t2, t2, p);
	
	mpz_mul(s, t1, t2);
	mpz_mod(s, s, p);
	// calculate result point
	mpz_t x3, y3;
	mpz_inits(x3, y3, NULL);
	
	mpz_mul(x3, s, s);
	mpz_sub(x3, x3, inputX);
	mpz_sub(x3, x3, inputX);
	mpz_mod(x3, x3, p);
	
	mpz_sub(y3, inputX, x3);
	mpz_mul(y3, y3, s);
	mpz_sub(y3, y3, inputY);
	mpz_mod(y3, y3, p);
	// set the result
	mpz_set(resultX, x3);
	mpz_set(resultY, y3);
	// free used variables
	mpz_clears(s, t1, t2, NULL);
	mpz_clears(x3, y3, NULL);
	return 1;
}

/* Performs an ecc point addition on the elliptic curve defined by a, b, p. The two operands are added and the result is written into the result point.

	--> DO NOT TOUCH THIS METHOD! <--

 */
int ecc_op_add(mpz_t resultX, mpz_t resultY, mpz_t a, mpz_t b, mpz_t p, mpz_t operand1_X, mpz_t operand1_Y, mpz_t operand2_X, mpz_t operand2_Y)
{
	if (mpz_cmp(operand1_X, operand2_X) == 0 && mpz_cmp(operand1_Y, operand2_Y) == 0){
		printf("The two passed operands are identical. Please use the function ecc_op_double to perform the point doubling.\n");
		return 1;
	}
	//init
	mpz_t s, t1, t2;
	mpz_inits(s, t1, t2, NULL);
	// calculate s by calculating nominator and denominator separately
	mpz_sub(t1, operand2_Y, operand1_Y);
	mpz_sub(t2, operand2_X, operand1_X);
	
	mpz_invert(t2, t2, p);
	
	mpz_mul(s, t1, t2);
	mpz_mod(s, s, p);
	// calculate result point
	mpz_t x3, y3;
	mpz_inits(x3, y3, NULL);
	
	mpz_mul(x3, s, s);
	mpz_sub(x3, x3, operand1_X);
	mpz_sub(x3, x3, operand2_X);
	mpz_mod(x3, x3, p);
	
	mpz_sub(y3, operand1_X, x3);
	mpz_mul(y3, y3, s);
	mpz_sub(y3, y3, operand1_Y);
	mpz_mod(y3, y3, p);
	// set the result
	mpz_set(resultX, x3);
	mpz_set(resultY, y3);
	// free used variables
	mpz_clears(s, t1, t2, NULL);
	mpz_clears(x3, y3, NULL);
	return 1;
}

/* Generate a random positive integer up to e bits */
int get_random_exponent(mpz_t e, int bits) 
{
	/*
	 * TO BE IMPLEMENTED!
	 * 
	 * (You will have to edit main() above to initialize the GMP random number generator.)
	 */
	
	// Lasse eine bits-Große Zahl ausgebeben
	mpz_urandomb(e, state, bits);

	return 0;
}


/*
 * Perform the modular exponentiation
 * 
 *    "result := g^e mod modulus",
 * 
 * using the binary method; operation tally in *count_s and *count_M
 */
int exponentiate_binary(mpz_t result, mpz_t g, mpz_t e, mpz_t modulus, long *count_S, long *count_M)
{
	/* TO BE IMPLEMENTED! */

	// Rechne den Exponenten in binär um
	char *ebinary = mpz_get_str(NULL,2,e); 

	//Init
	mpz_set(result, g);

	//Schleife
	int i;
    for(i = 1; i < mpz_sizeinbase(e,2); i++)
    {	
		// Quadrierung
		mpz_powm_ui(result, result, 2, modulus);
		(*count_S)++;
		// Abfragen ob Stelle = 1 ist
		if(ebinary[i]-'0' == 1)
		{
			// Multiplikation und Modulo
			mpz_mul(result, result, g);
			mpz_mod(result, result, modulus);
			(*count_M)++;
		}
    }

	// Speicher freigeben
	free(ebinary);	

	/* Whenever performing a modular squaring or multiplication, count the operations like this: */
	//(*count_S)++;
	//(*count_M)++;

	return 0; /* replace by "return 0" once you have an implementation */
}


/*
 * Perform the modular exponentiation
 * 
 *    "result := g^e mod modulus",
 * 
 * using the k-ary method parameterized by k;
 * operation tally in *count_s and *count_M
 */
int exponentiate_k_ary(mpz_t result, mpz_t g, mpz_t e, mpz_t modulus, int k, long *count_S, long *count_M)
{
	/* TO BE IMPLEMENTED! */

	// Array erzeugen für Tabellenberechnung
	mpz_t *arr;
	mpz_t tablesize;
	mpz_init(tablesize);
	mpz_ui_pow_ui(tablesize, 2, k);
	arr = malloc(mpz_get_ui(tablesize) * sizeof(mpz_t));	// Speicherplatz reservieren
	
	// g0, g1 und g2 selber setzen
	mpz_init(arr[0]);	
	mpz_set_str(arr[0],"1",10);

	mpz_init(arr[1]);	
	mpz_set(arr[1], g);

	mpz_init(arr[2]);	
	mpz_powm_ui(arr[2], g, 2, modulus);
	(*count_S)++;
	
	// gi's berechnen 
	int i;	
	for(i = 3; i < mpz_get_ui(tablesize); i++)
	{	
		mpz_init(arr[i]);					// 1. Initsialisieren
		mpz_mul(arr[i], arr[i-1], g);		// 2. g(i) = g(i-1) * g
		mpz_mod(arr[i], arr[i], modulus);	// 3. g(i) modulo rechnen
		(*count_M)++;
	}
	
	// Exponenten in Binär umwandeln
	char *ebinary = mpz_get_str(NULL,2,e);	// Exponenten in Binär wandeln	
	int laenge = mpz_sizeinbase(e,2);		// Anzahl Bits
	int et = laenge % k;					// Bestimmen aus wie vielen Bits Et (vorderster Bitblock) besteht

	// Hilsvariablen
	int pos;		// zum speichern der aktuellen Position im Bitstring/Array
	int bloecke;	// zum speichern der anzahl Blöcke des Binärstrings
	char *bits = malloc((k+1) * sizeof(char)); // Array für k-Bits
	
	// INIT, g = Et setzen
	if(et == 0)
	{
		int j;
		// Da et = 0, besteht der erste Block (Et) aus k-Bits
		for (j = 0; j < k; j++)
		{
			bits[j] = ebinary[j];			
		}
		bits[k] = '\0';

		mpz_set(result,arr[bintodec(bits, 0)]);	// g = Et
		pos = k;								// als nächstes zu bearbeitende Blockposition
		bloecke = (laenge / k) - 1;				// Bestimme Anzahl Blöcke die abgearbeitet werden müssen
	}
	else
	{	
		int j;
		// Da et ungleich 0, besteht der erste Block (Et) aus et-Bits
		for (j = 0; j < et; j++)
		{
			bits[j] = ebinary[j];
		}
		bits[et] = '\0';

		mpz_set(result,arr[bintodec(bits, 0)]);	// g = Et
		pos = et;								// als nächstes zu bearbeitende Blockposition
		bloecke = (laenge-et) / k;				// Bestimme Anzahl Blöcke die abgearbeitet werden müssen
	}

	// ALGO
	for (i = 0; i < bloecke; i++)
	{
		// Block bzw k-Bits einlesen
		int zaehler;
		for (zaehler = 0; zaehler < k; zaehler++)
		{
			bits[zaehler] = ebinary[pos];
			pos++;

			// Gleichzeitig schon quadrieren, damit keine extre Schleife nochmal benötigt wird
			// Insgesamt k-Mal durch Schleife
			mpz_powm_ui(result, result, 2, modulus);		
			(*count_S)++;
		}
		bits[k] = '\0';
		
		// mit gi aus Tabelle multiplizieren
		mpz_mul(result, result, arr[bintodec(bits, 0)]);
		(*count_M)++;
		// Modulo rechnen
		mpz_mod(result, result, modulus);
	}

	// Speicher freigeben
	free(bits);	
	free(ebinary);	
	for (i = 0; i < mpz_get_ui(tablesize); i++) 
	{
    	mpz_clear(arr[i]);
	}
	mpz_clear(tablesize);
	
	/* Whenever performing a modular squaring or multiplication, count the operations like this: */
	//(*count_S)++;
	//(*count_M)++;

	return 0; /* replace by "return 0" once you have an implementation */
}


/*
 * Perform the modular exponentiation
 * 
 *    "result := g^e mod modulus",
 * 
 * using the improved k-ary method (k-ary w/ reduced precomputation) parameterized by k;
 * operation tally in *count_s and *count_M
 */
int exponentiate_improved_k_ary(mpz_t result, mpz_t g, mpz_t e, mpz_t modulus, int k, long *count_S, long *count_M)
{
	/* TO BE IMPLEMENTED! */

	// Array erzeugen bzw Speicher reservieren -> Tabelle besteht aus ((2^k)/2)+2 Einträgen
	mpz_t *arr;
	mpz_t tablesize;
	mpz_init(tablesize);
	mpz_ui_pow_ui(tablesize, 2, k);				
	mpz_divexact_ui(tablesize, tablesize, 2); 
	mpz_add_ui(tablesize, tablesize, 2);		
	arr = malloc(mpz_get_ui(tablesize) * sizeof(mpz_t));
	
	// g0, g1, g2, g3 selber setzen
	mpz_init(arr[0]);	
	mpz_set_str(arr[0],"1",10);

	mpz_init(arr[1]);	
	mpz_set(arr[1], g);

	mpz_init(arr[2]);	
	mpz_powm_ui(arr[2], g, 2, modulus);
	(*count_S)++;

	mpz_init(arr[3]);	
	mpz_mul(arr[3], arr[1], arr[2]);
	mpz_mod(arr[3], arr[3], modulus);
	(*count_M)++;

	// Alle ungeraden gi's berechnen 
	int i;	
	for(i = 4; i < mpz_get_ui(tablesize); i++)
	{	
		mpz_init(arr[i]);					// INIT
		mpz_mul(arr[i], arr[i-1], arr[2]);	// MUL mit g^2
		mpz_mod(arr[i], arr[i], modulus);	// Modulo	
		(*count_M)++;						
	}
	
	// INIT ALGO
	mpz_set_ui(result, 1);
	
	// Hilfsvariablen
	char *ebinary = mpz_get_str(NULL,2,e);		// Exponenten in Binär wandeln
	int laenge = mpz_sizeinbase(e, 2);			// Länge des Exponenten
	int et = laenge % k;						// Bestimme größe/Anzahl Bits des ersten Blocks (Et)
	char *bits = malloc((k+1) * sizeof(char));  // Array für k-Bits
	int pos = 0;			// Aktuelle Position im Binärstring
	int bloecke;			// Anzahl k-Bit Blöcke die verarbeitet werden müssen			
	int posEnde = 0;		// Markiert Endposition des gefundenen Bitblocks	

	// Prüfe erstern Block (Et) auf die Anzahl Bits
	if(et == 0)
	{
		// Da Et genau k-Bits groß ist, kann dieser Block unten in der Schleife verarbeitet werden
		bloecke = (laenge / k);
	}
	// Block (Et) besteht weniger aus k-Bits und wird sofort abgearbeitet
	else
	{
		// Iteriere im Block (Et) durch die Bits von rechts um größte ungerade Zahl zu finden
		int i;
		for(i = et-1; i >= 0; i--)
		{
			// Abfrage, ob betrachtetes Bit = 1, wenn ja größte ungerade Zahl gefunden
			if(ebinary[pos+i]-'0' == 1 )
			{	
				// Markiere Position bis zu der der Block verarbeitet werden kann
				posEnde = pos + i;
				// Suche kann abgebrochen werden, da größte ungerade Zahl gefunden
				break;	
			}	
		}
		
		// Anzahl Bits die verarbeitet werden von Block Et (ohne vordere Nullen aufgefüllt)
		int ui = posEnde-pos+1;
	
		// Gibt Anzahl Nullen die da wären, um auf k-Bits zu kommen	
		int anzahlNullen = k-et;

		// Zunächst Array mit fehlenden Nullen belegen
		for (i = 0; i < anzahlNullen; i++)
		{
			bits[i] = '0';
			// Führe gleichzeitig die ersten Quadrierungen von ui durch
			mpz_powm_ui(result, result, 2, modulus);
			//(*count_S)++;			
		}
		
		// In das Array Bits kopieren um Index für Tabelle (gi) zu bestimmen
		for (i = 0; i < ui; i++)
		{
			bits[anzahlNullen] = ebinary[i];
			anzahlNullen++;
			// Führe gleichzeitig die letzten Quadrierungen von ui durch
			mpz_powm_ui(result, result, 2, modulus);
			(*count_S)++;		
		}
		bits[anzahlNullen+ui] = '\0';

		// Multiplikation mit gi
		mpz_mul(result, result, arr[bintodec(bits, 1)]);
		mpz_mod(result, result, modulus);
		(*count_M)++;
		
		// Führe fehlende et-ui = hi Quadrierungen durch
		for (i = 0; i < et-ui; i++)
		{	
			mpz_powm_ui(result, result, 2, modulus);
			(*count_S)++;			
		}
	
		// Speichere Anzahl noch zu bearbeitender Blöcke sowie die Position des nächsten Bits
		pos = et;
		bloecke = (laenge-et) / k;
	}
	// ALGO
	int j;
	for (j = 0; j < bloecke; j++)
	{
			// Iteriere im aktuellen Block durch die Bits von rechts um größte ungerade Zahl zu finden
			int i;
			for(i = k-1; i >= 0; i--)
			{
				// Abfrage, ob betrachtetes Bit = 1, wenn ja größte ungerade Zahl gefunden
				if(ebinary[pos+i]-'0' == 1 )
				{	
					// Markiere Position bis zu der der Block verarbeitet werden kann
					posEnde = pos + i;
					// Suche kann abgebrochen werden, da größte ungerade Zahl gefunden
					break;	
				}		
			}

			// Prüfe ob der aktuelle Block kein Null-Block ist -> Besteht nicht aus k-Mal Bit 0
			if(ebinary[pos+i]-'0' == 1)
			{
				// Hilfvariable, die Anzahl Bits die im Block verarbeitet werden angibt
				int hi = posEnde-pos+1; 

				// Array leer setzen
				int zaehler;
				for (zaehler = 0; zaehler <= k; zaehler++)
				{
					bits[zaehler] = '\0';
						
				}

				// Array belegen bzw Bits kopieren um Index für Tabelle (gi) zu bestimmen
				for (zaehler = 0; zaehler < hi; zaehler++)
				{
					bits[zaehler] = ebinary[pos+zaehler];

					// Führe gleichzeitig die ersten hi Quadrierungen durch
					mpz_powm_ui(result, result, 2, modulus);
					(*count_S)++;							
				}
				
				// Multiplikation mit gi
				mpz_mul(result, result, arr[bintodec(bits, 1)]);
				mpz_mod(result, result, modulus);
				(*count_M)++;	

				// Führe fehlende k-hi Quadrierungen durch
				for (i = 0; i < k-hi; i++)
				{
					mpz_powm_ui(result, result, 2, modulus);
					(*count_S)++;			
				}
			}
			// Aktueller Block ist ein Null-Block -> Besteht aus k-Mal Bit 0
			else
			{
				// k-Mal Quadrieren
				for (i = 0; i < k; i++)
				{
					mpz_powm_ui(result, result, 2, modulus);
					(*count_S)++;			
				}
				// Multiplikation mit g0
				mpz_mul(result, result, arr[0]);
				mpz_mod(result, result, modulus);
				(*count_M)++;	
			}
			pos += k;
	}

	// Freigabe von Speicher
	free(ebinary);
	free(bits);
	mpz_clear(tablesize);
	
	/* Whenever performing a modular squaring or multiplication, count the operations like this: */
	//(*count_S)++;
	//(*count_M)++;


	return 0; /* replace by "return 0" once you have an implementation */
}


/*
 * Perform the modular exponentiation
 * 
 *    "result := g^e mod modulus",
 * 
 * using the sliding window method parameterized by k;
 * operation tally in *count_s and *count_M
 */
int exponentiate_sliding_window(mpz_t result, mpz_t g, mpz_t e, mpz_t modulus, int k, long *count_S, long *count_M)
{
	/* TO BE IMPLEMENTED! */

	// Array erzeugen bzw Speicher reservieren -> Tabelle besteht aus ((2^k)/2)+2 Einträgen
	mpz_t *arr;
	mpz_t tablesize;
	mpz_init(tablesize);
	mpz_ui_pow_ui(tablesize, 2, k);				// 2^k Elemente
	mpz_divexact_ui(tablesize, tablesize, 2);	// Teile durch 2, um Plätze der geraden Zahlen zu entfernen
	mpz_add_ui(tablesize, tablesize, 2);		// Addition mit 2, um zuviel abgezogene Plätze wieder zu nehmen
	arr = malloc(mpz_get_ui(tablesize) * sizeof(mpz_t));
	
	// g0, g1, g2, g3 selber setzen
	mpz_init(arr[0]);	
	mpz_set_str(arr[0],"1",10);

	mpz_init(arr[1]);	
	mpz_set(arr[1], g);

	mpz_init(arr[2]);	
	mpz_powm_ui(arr[2], g, 2, modulus);
	(*count_S)++;

	mpz_init(arr[3]);	
	mpz_mul(arr[3], arr[2], g);
	mpz_mod(arr[3], arr[3], modulus);
	(*count_M)++;

	// Alle ungeraden gi's berechnen 
	int i;	
	for(i = 4; i < mpz_get_ui(tablesize); i++)
	{	
		mpz_init(arr[i]);					// INIT
		mpz_mul(arr[i], arr[i-1], arr[2]);	// MUL mit g^2
		mpz_mod(arr[i], arr[i], modulus);	// Modulo
		(*count_M)++;						
	}

	// INIT ALGO 
	mpz_set_ui(result, 1);

	// Hilfsvariablen für ALGO
	char *ebinary = mpz_get_str(NULL,2,e);		// Exponenten in Binär wandeln
	int laenge = mpz_sizeinbase(e, 2);		// Anzahl Bits des Exponent
	char *bits = malloc((k+1) * sizeof(char));  // Array für k-Bits
	int posAnfang = 0;	// Markiert Startposition des gefundenen Bitblocks				
	int posEnde = 0;	// Markiert Endposition des gefundenen Bitblocks	
	
	// ALGO
	int pos;					// Aktuelle Position im Array 
	for(pos = 0; pos < laenge;)	// Iteriere durch die Bits 
	{
		// Erkennen ob Bitbklock gefunden wurde
		if(ebinary[pos]-'0' == 1)
		{
			// Markiere gefundene Startposition des Bitblocks
			posAnfang = pos;			
			
			// Bestimme wie groß gefunder Bitblock ist -> Maximal k-Bits
			int i;
			for(i = k-1; i >= 0; i--)
			{
				// Erste Abfrage dafür, ob man Ende des Exponenten erreicht hat, dann Abbruch
				// Zweite Abfrage, ob betrachtetes Bit = 1, wenn ja Blockende gefunden
				if(pos+i < laenge && ebinary[pos+i]-'0' == 1)
				{	
					posEnde = pos + i;	// Markiere Endposition des Bitblocks
					pos += (i + 1);		// Erhöhe Position auf Bit, welches nach dem Bitblock betrachtet werden soll
					break;				// Bricht FOR-Schleife ab, da Blockende mit maximaler Größe gefunden
				}
				
			}
			
			// Hilfvariable, die Anzahl Bits im gefundenen Block angibt
			int blockgroesse = posEnde-posAnfang+1; 

			// Führe Quadrierungen durch
			for (i = 0; i < blockgroesse; i++)
			{	
				mpz_powm_ui(result, result, 2, modulus);
				(*count_S)++;			
			}

			// Array in den die Bits des Blocks gespeichert werden leer setzen
			int zaehler;
			for (zaehler = 0; zaehler <= k; zaehler++)
			{
				bits[zaehler] = '\0';				
			}
			
			// Array belegen bzw Bits kopieren
			for (zaehler = 0; zaehler < blockgroesse; zaehler++)
			{
				bits[zaehler] = ebinary[posAnfang+zaehler];
			}
				
			// MUL mit gi und anschließender Modulo
			mpz_mul(result, result, arr[bintodec(bits, 1)]);
			mpz_mod(result, result, modulus);
			(*count_M)++;	
		}
		// Kein Bitblock gefunden, das heißt betrachtetes Bit = 0
		else
		{
			// QUADRIERE, da Bit = 0
			mpz_powm_ui(result, result, 2, modulus);
			pos++;
			(*count_S)++;
		}
	}

	// Freigabe von Speicher
	free(ebinary);
	free(bits);
	mpz_clear(tablesize);
	
	/* Whenever performing a modular squaring or multiplication, count the operations like this: */
	//(*count_S)++;
	//(*count_M)++;

	return 0; /* replace by "return 0" once you have an implementation */
}

/*
 * Perform the elliptic curve multiplication 
 * 
 *    "result := factor * input mod p",
 * 
 * using the double and add algorithm.
 * operation tally in *count_D and *count_A
 *
 * Please reduce the factor with the prime group order q of the elliptic curve to avoid the point of infinity!
 */
int ecc_double_add(mpz_t resultX, mpz_t resultY, mpz_t a, mpz_t b, mpz_t p, mpz_t q, mpz_t inputX, mpz_t inputY, mpz_t factor, long *count_D, long *count_A)
{	
	/* TO BE IMPLEMENTED! */

	// Reduziere factor mit dem Gruppenordnung q
	mpz_mod(factor, factor, q);

	// Rechne Faktor in binär um
	char *ebinary = mpz_get_str(NULL,2,factor); 

	// Definiere Hilfsvariablen um Zwischenergebnisse (Punkt) zu speichern
	mpz_t tmpX;
	mpz_t tmpY;
	mpz_init(tmpX);
	mpz_init(tmpY);

	//Init
	mpz_set(resultX, inputX);
	mpz_set(resultY, inputY);

	//Schleife
	int i;
    for(i = 1; i < mpz_sizeinbase(factor,2); i++)
    {	
		// Double
		ecc_op_double(tmpX, tmpY, a, b, p, resultX, resultY);
		mpz_set(resultX, tmpX);
		mpz_set(resultY, tmpY);
		(*count_D)++;
		
		// Abfragen ob Stelle = 1 ist
		if(ebinary[i]-'0' == 1)
		{
			// Add
			ecc_op_add(tmpX, tmpY, a, b, p, resultX, resultY, inputX, inputY);
			mpz_set(resultX, tmpX);
			mpz_set(resultY, tmpY);
			(*count_A)++;
		}
    }

	// Speicher freigeben
	free(ebinary);
	mpz_clear(tmpX);
	mpz_clear(tmpY);

	/* Whenever performing a double or add, count the operations like this: */
	//(*count_D)++;
	//(*count_A)++;

	return 0; /* replace by "return 0" once you have an implementation */
}

/*
 * Perform the elliptic curve multiplication 
 * 
 *    "result := factor * input mod p",
 * 
 * using the NAF double and add algorithm.
 * operation tally in *count_D and *count_A
 *
 * Please reduce the factor with the prime group order q of the elliptic curve to avoid the point of infinity!
 */
int ecc_naf_double_add(mpz_t resultX, mpz_t resultY, mpz_t a, mpz_t b, mpz_t p, mpz_t q, mpz_t inputX, mpz_t inputY, mpz_t factor, long *count_D, long *count_A)
{
	/* TO BE IMPLEMENTED! */

	// Reduziere factor mit dem Gruppenordnung q
	mpz_mod(factor, factor, q);

	// Berechne Y-Kordinate von -P
	mpz_t inputYinverse;
	mpz_init(inputYinverse);
	mpz_neg(inputYinverse, inputY);
	mpz_mod(inputYinverse, inputYinverse, p);	

	// Hilfsvariablen für NAF Umrechnung
	mpz_t tmp;		
	mpz_init(tmp);
	int binaryLength =  mpz_sizeinbase(factor,2); // Binärlänge von factor

 	// int-Array für NAF-Bits (mit setzen von Null)
	int *factorNAF;
	factorNAF = calloc((binaryLength+1) , sizeof(int)); 

	// Belege NAF-Array von hinten da zunächst e0, e1, ... berechnet werden
	int zaehler = binaryLength; 

	// Rechne Faktor in die NAF Darstellung um
	// Prüfe ob factor größer gleich 1 ist
	while(mpz_cmp_ui(factor, 1) >= 0)
	{
		// Prüfe ob factor ungerade
		if(mpz_odd_p(factor)) 
		{
			// Berechne factor mod 4 und speichere in tmp
			mpz_mod_ui(tmp, factor, 4);					
			// setze ei = 2 - (factor mod 4)
			factorNAF[zaehler] = 2 - (mpz_get_ui(tmp));	
			
			// Berechen factor-ei
			if(factorNAF[zaehler] == -1)
			{
				// ei = -1, dann Additon
				mpz_add_ui(factor, factor, 1);	
			}
			else
			{	
				// ei = 1, dann Subtraktion
				mpz_sub_ui(factor, factor, 1);	
			}
		}
		// factor ist gerade, daher setze ei = 0
		else
		{
			factorNAF[zaehler] = 0; 
		}

		mpz_divexact_ui(factor, factor, 2);			// Setze factor = factor/2
		zaehler--;
	}

	// Init NAF-Double-and-Add
	mpz_set(resultX, inputX);
	mpz_set(resultY, inputY);

	/*  
	 * Da Array von hinten belegt wird kann die Stelle [0] mit einer Null belegt sein,
	 * da die NAF-Representation nicht immer ein Bit länger ist als die "normale" Binärdarstellung. 
     * -> Dementsprechend wird hier das zu erste betrachtende Bit festgelegt!
	 */  
	
	int start;
	if(factorNAF[0] == 1)
	{
		start = 1;
	}
	else
	{
		start = 2;
	}

	// Definiere Hilfsvariablen um Zwischenergebnisse (Punkt) zu speichern
	mpz_t tmpX;
	mpz_t tmpY;
	mpz_init(tmpX);
	mpz_init(tmpY);

	// Schleife NAF-Double-and-Add
	int i;
    for(i =  start; i < binaryLength+1; i++)
    {	
		// Double
		ecc_op_double(tmpX, tmpY, a, b, p, resultX, resultY);
		mpz_set(resultX, tmpX);
		mpz_set(resultY, tmpY);
		(*count_D)++;
		
		// Abfragen ob Bit = 1 ist
		if(factorNAF[i] == 1)
		{
			// Add
			ecc_op_add(tmpX, tmpY, a, b, p, resultX, resultY, inputX, inputY);
			mpz_set(resultX, tmpX);
			mpz_set(resultY, tmpY);
			(*count_A)++;
		}
		// Abfragen ob Bit = -1 ist
		if(factorNAF[i] == -1)
		{
			// Add
			ecc_op_add(tmpX, tmpY, a, b, p, resultX, resultY, inputX, inputYinverse);
			mpz_set(resultX, tmpX);
			mpz_set(resultY, tmpY);
			(*count_A)++;
    	}	
	}

	// Speicher freigeben
	free(factorNAF);
	mpz_clear(inputYinverse);
	mpz_clear(tmp);
	mpz_clear(tmpX);
	mpz_clear(tmpY);

	/* Whenever performing a double or add, count the operations like this: */
	//(*count_D)++;
	//(*count_A)++;

	return 0; /* replace by "return 0" once you have an implementation */
}

/*
 * Wandelt Bitstring in die Dezimaldarstellung um
 *
 * Rückgabe in int
 *
 * -> algo = 0 für k-ary
 * -> algo = 1 für improved-k-ary und sliding-window
 */
int bintodec(char *bitstring, int algo)
{
	// Hilfsvariable erzeugen
	mpz_t gi;							
	mpz_init(gi);

	// char to mpz_t 
	mpz_set_str(gi, bitstring,2);
	// mpz_t to int
	int index = mpz_get_ui(gi);	

	// Bei improved k-ary und sliding-window Array "nicht nach Reihenfolge", daher anpassen des Index
	if(algo == 1)
	{
		if(index > 3)			
		{
			index = ((index+1)/2)+1;
    	}
    }

	return index;
}
