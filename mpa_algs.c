/*! \file mpa_algs.c
 *  \brief Implementation of basic MPA functions.
 *  \author Turan Kavak
 *  \date SS18
 *  \version 0.2a
 *
 *  Lecture: "Implementierung kryptographischer Verfahren", WS14/15.
 *  MPA-Project.
 *  Solution to implementation assignments.
 *
 *  THIS IS THE PART WHERE YOU HAVE TO PUT YOUR CODE!
 */

#include "mpa_algs.h"
#include "gmp_ext.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Function mpz_school_add.
 * Computes the sum c of two multi-precision
 * integers a and b.
 *
 * @param c the multi-precision sum a + b.
 * @param a the first multi-precision operand.
 * @param b the second multi-precision operand.
 */
void mpz_school_add(mpz_t c, mpz_t a, mpz_t b)
{
	// Anzahl Wörter die verarbeitet werden müssen
	mp_size_t woerter = fmax(mpz_size (a), mpz_size (b));

	// Variable um Ergebnis zu speichern
	mpz_t erg;
	mpz_init(erg);

	// Variablen für Schleife
	mp_limb_t carry1, carry2, carry, summe;
	carry = 0;
	mp_limb_t *pos = &summe;
	int i;

	// Schleife
	for(i = 0; i < woerter; i++)
	{
		// a[i] + b[i]
		carry1 = mp_add_limb(pos, mpz_getlimbn(a, i), mpz_getlimbn(b, i));
		// Addition von Carry
		if(carry != 0)
		{	
			carry2 = mp_add_limb(pos, summe, carry);
			mp_add_limb(&carry, carry1, carry2);
		}
		else
		{	
			carry = carry1;
		}
		// Speichern der Teilsumme
		mpz_setlimbn(erg, summe, i);
	}

	// Carry vom letzen Wort speichern
	mpz_setlimbn(erg, carry, woerter);
	// Ergebnisübergabe
	mpz_set(c, erg);
	// Speicherfreigabe
	mpz_clear(erg);
}

/**
 * Function mpz_mul_limb.
 * Computes the product c of a multi-precision integer a
 * and a single-precision integer b.
 *
 * @param c the multi-precision product a * b.
 * @param a the multi-precision operand.
 * @param b the single-precision operand.
 */
void mpz_mul_limb(mpz_t c, mpz_t a, mp_limb_t b)
{
	// Anzahl Wörter von a die verarbeitet werden müssen
	mp_size_t woerter = mpz_size(a);

	// Variable um Ergebnis zu speichern
	mpz_t erg;
	mpz_init(erg);

	// Variablen für Schleife
	mp_limb_t carry1, carry2, carry, produkt;
	carry = 0;
	mp_limb_t *pos = &produkt;
	int i;

	for(i = 0; i < woerter; i++)
	{
		// a[i] * b
		carry1 = mp_mul_limb(pos, mpz_getlimbn(a, i), b);
		// Addition des Carry
		if(carry != 0)
		{		
			carry2 = mp_add_limb(pos, produkt, carry);
			mp_add_limb(&carry, carry1, carry2);
		}
		else
		{
			carry = carry1;
		}
		// Speichern der Teilmultiplikation
		mpz_setlimbn(erg, produkt, i);
	}

	// Carry vom letzen Wort speichern
	mpz_setlimbn(erg, carry, woerter);
	// Ergebnisübergabe
	mpz_set(c, erg);
	// Speicherfreigabe
	mpz_clear(erg);
}

/**
 * Function mpz_mul_base.
 * Computes the product c of a multi-precision integer a
 * and the i-th power of the base B.
 *
 * @param c the multi-precision product a * B^i.
 * @param a the multi-precision operand.
 * @param i the base exponent.
 */
void mpz_mul_base(mpz_t c, mpz_t a, mp_size_t i)
{
	// Anzahl Wörter von a die verarbeitet werden müssen
	mp_size_t woerter = mpz_size(a);

	// Variable um Ergebnis zu speichern
	mpz_t erg;
	mpz_init(erg);
	
	// Variable für Schleifen
	int zaehler;

	// Schleife um die Blöcke von a um i-Position zu shiften
	for(zaehler = 0; zaehler < woerter; zaehler++)
	{
		mpz_setlimbn(erg, mpz_getlimbn(a, zaehler), zaehler+i);
	}
	
	// Ergebnisübergabe
	mpz_set(c, erg);
	// Speicherfreigabe
	mpz_clear(erg);
}

/**
 * Function mpz_school_mul.
 * Computes the product c of two multi-precision
 * integers a and b using the schoolbook method.
 *
 * @param c the multi-precision product a * b.
 * @param a the first multi-precision operand.
 * @param b the second multi-precision operand.
 */
void mpz_school_mul(mpz_t c, mpz_t a, mpz_t b)
{
	// Variablen
	mpz_t erg, teilErg;
	mpz_inits(erg, teilErg, NULL);
	int i;

	// Schleife
	for(i = 0; i < mpz_size(b); i++)
	{
		// a * b[i]
		mpz_mul_limb(teilErg, a, mpz_getlimbn(b, i));

		// Wörter des Teilergebnis um i-Wörter shiften
		mpz_mul_base(teilErg, teilErg, i);

		// Addieren des Teilergebnis auf Ergebnis
		mpz_school_add(erg, erg, teilErg);
	}

	// Ergebnisübergabe
	mpz_set(c, erg);
	// Speicherfreigabe
	mpz_clears(erg, teilErg, NULL);
}

/**
 * Function mpz_rec_mul.
 * Computes the product c of two multi-precision
 * integers a and b using a recursive multiplication method.
 *
 * @param c the multi-precision product a * b.
 * @param a the first multi-precision operand.
 * @param b the second multi-precision operand.
 */
void mpz_rec_mul(mpz_t c, mpz_t a, mpz_t b)
{
	// Variable um Ergebnis zu speichern
	mpz_t erg;
	mpz_init(erg);

	// Abfrage für Rekursiven Aufruf oder Berechnung der Multiplikation
	if(mpz_size(a) <= 1 && mpz_size(b) <= 1)
	{	
		// Variablen für Ergebnis der single-precision Multiplikation
		mp_limb_t produkt;
		mp_limb_t *pos = &produkt;
		// Carry
		mp_limb_t carry = 0;

		// Multiplikation
		carry = mp_mul_limb(pos, mpz_getlimbn(a, 0), mpz_getlimbn(b, 0));
		// Ergebnis speichern	
		mpz_setlimbn(erg, produkt, 0);	
		if(carry != 0)
		{ 
			mpz_setlimbn(erg, carry, 1);
		}
		// Ergebnisübergabe
		mpz_set(c, erg);
	}
	else
	{
		// Anzahl Blöcke
		mp_size_t woerter = fmax(mpz_size (a), mpz_size (b));
		// Bestimmen von m 
		int m = (int)ceil((double)woerter / 2);

		// Variaben um a und b aufzuteilen, sowie Teilergebnis zu speichern
		mpz_t a0, a1, b0, b1, teilerg;
		mpz_inits(a0, b0, a1, b1, teilerg, NULL);

		int i;
		// Bestimmen von a0/a1 und b0/b1
		for(i = 0; i < m; i++)
		{	
			// a0 und b0
			mpz_setlimbn(a0, mpz_getlimbn(a, i), i);
			mpz_setlimbn(b0, mpz_getlimbn(b, i), i);	
			// a1 und b1
			mpz_setlimbn(a1, mpz_getlimbn(a, m+i), i);
			mpz_setlimbn(b1, mpz_getlimbn(b, m+i), i);
		}

		// teilerg = (a(1) * b(1)) * B^2m
		mpz_rec_mul(teilerg, a1, b1);
		mpz_mul_base(teilerg, teilerg, 2*m);
		mpz_school_add(erg, erg, teilerg);

		// teilerg = (a(1) * b(0)) * B^m
		mpz_rec_mul(teilerg, a1, b0);
		mpz_mul_base(teilerg, teilerg, m);
		mpz_school_add(erg, erg, teilerg);

		// teilerg = (a(0) * b(1)) * B^m
		mpz_rec_mul(teilerg, a0, b1);
		mpz_mul_base(teilerg, teilerg, m);
		mpz_school_add(erg, erg, teilerg);

		// teilerg = (a(0) * b(0))
		mpz_rec_mul(teilerg, a0, b0);
		mpz_school_add(erg, erg, teilerg);

		// Ergebnisübergabe
		mpz_set(c, erg);
		// Speicherfreigabe
		mpz_clears(a0, b0, a1, b1, teilerg, NULL);
	}
	// Speicherfreigabe
	mpz_clear(erg);
}

/**
 * Function mpz_karatsuba_mul.
 * Computes the product c of two multi-precision
 * integers a and b using Karatsuba's multiplication method.
 *
 * @param c the multi-precision product a * b.
 * @param a the first multi-precision operand.
 * @param b the second multi-precision operand.
 */
void mpz_karatsuba_mul(mpz_t c, mpz_t a, mpz_t b)
{
	// Variable um Ergebnis zu speichern
	mpz_t erg;
	mpz_init(erg);

	// Abfrage für Rekursiven Aufruf oder Berechnung der Multiplikation
	if(mpz_size(a) <= 1 && mpz_size(b) <= 1)
	{	
		// Variablen für Ergebnis der single-precision Multiplikation
		mp_limb_t produkt;
		mp_limb_t *pos = &produkt;
		// Carry
		mp_limb_t carry = 0;

		// Multiplikation
		carry = mp_mul_limb(pos, mpz_getlimbn(a, 0), mpz_getlimbn(b, 0));
		// Ergebnis speichern	
		mpz_setlimbn(erg, produkt, 0);
		if(carry != 0)
		{ 
			mpz_setlimbn(erg, carry, 1);
		}
		// Ergebnisübergabe
		mpz_set(c, erg);
	}
	else
	{
		// Anzahl Wörter
		mp_size_t woerter = fmax(mpz_size (a), mpz_size (b));
		// Bestimmen von m 
		int m = (int)ceil((double)woerter / 2);

		// Variaben um a und b aufzuteilen, sowie Teilergebnisse zu speichern
		mpz_t a0, a1, b0, b1, teilerg1, teilerg2, teilerg3, teilerg4;
		mpz_inits(a0, b0, a1, b1, teilerg1, teilerg2, teilerg3, teilerg4, NULL);

		int i;
		// Bestimmen von a0/a1 und b0/b1
		for(i = 0; i < m; i++)
		{	
			// a0 und b0
			mpz_setlimbn(a0, mpz_getlimbn(a, i), i);
			mpz_setlimbn(b0, mpz_getlimbn(b, i), i);	
			// a1 und b1
			mpz_setlimbn(a1, mpz_getlimbn(a, m+i), i);
			mpz_setlimbn(b1, mpz_getlimbn(b, m+i), i);
		}

		// teilerg1 = a(1) * b(1)
		mpz_karatsuba_mul(teilerg1, a1, b1);
		// teilerg2 = a(1) + a(0)
		mpz_school_add(teilerg2, a1, a0);
		// teilerg3 = b(1) + b(0)
		mpz_school_add(teilerg3, b1, b0);
		// teilerg4 = a(0) * b(0)
		mpz_karatsuba_mul(teilerg4, a0, b0);
		
		// teilerg3 = (a(1) + a(0)) + (b(1) * b(0)) 
		mpz_karatsuba_mul(teilerg3, teilerg2, teilerg3);
		// teilerg3 = (a(1) + a(0)) + (b(1) * b(0)) - (a(1) * b(1))
		mpz_sub(teilerg3, teilerg3, teilerg4);
		// teilerg3 = (a(1) + a(0)) + (b(1) * b(0)) - (a(1) * b(1)) - (a(0) * b(0))
		mpz_sub(teilerg3, teilerg3, teilerg1);
		// teilerg3 = ((a(1) + a(0)) + (b(1) * b(0)) - (a(1) * b(1)) - (a(0) * b(0))) * B^m
		mpz_mul_base(teilerg3, teilerg3, m);
		
		// teilerg1 = (a(1) * b(1)) * B^2m
		mpz_mul_base(teilerg1, teilerg1, 2*m);

		// Zusammenfügen der Teilergebnisse durch Addition
		mpz_school_add(erg, teilerg1, teilerg4);
		mpz_school_add(erg, erg, teilerg3);

		// Ergebnisübergabe
		mpz_set(c, erg);
		// Speicherfreigabe
		mpz_clears(a0, b0, a1, b1,teilerg1, teilerg2, teilerg3, teilerg4, NULL);
	}
	// Speicherfreigabe
	mpz_clear(erg);
}
