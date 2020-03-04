/**
 * Differential Power Analysis of an AES sbox
 *  
 * Please supply the following information: 
 * \author Turan Kavak
 * 
 * Insert your code here and mail in this file ONLY.
 * 
 * Please have a look at DPA_attacks.h to get to know how the structs are definied
 *
 * Do not change the function prototypes
 *       
 */

#include "DPA_attacks.h"
#include <stdlib.h>
#include <string.h>

AESChallenge* scan_data(const char* file, const unsigned int max_measurements)
{
  // Initialisierung des Anfangs der verketteten Liste
  AESChallenge *anfang = NULL;
  
  // Hilfsvariablen
  AESChallenge *zeiger;
  double time, power;
  int i, sample, lesefehler;
  // Variablen für Dateiname und Datei
  int laenge = strlen(file);
  char dateipfad[laenge+5];
  FILE *datei;
  
  // Schleife zum Lesen aller Messungen
  for(i = 0; i < max_measurements; i++)
  {  	
	// Zu lesende Messung öffnen, zuvor Dateinamen zusammensetzen
	sprintf(dateipfad, "%s_%d",file, i);
	datei = fopen(dateipfad, "r");
	if(datei == NULL)
	{
      printf("FEHLER !\nDatei 'trace_%d' konnte nicht geöffnet werden zum Lesen.\n", i);
	  exit(0);
	}

	// Lesen der ersten Messung
    if(anfang == NULL)
	{
      // Reservieren Speicherplatz fürs erste Element
	  anfang = malloc(sizeof(AESChallenge));
	  if(anfang == NULL)
	  {
        // Speicher konnte nicht reserviert werden
	    return 0;
	  }
	  
	  // Füllen des Structs
	  anfang -> challenge = i;
	  for(sample = 0; sample < TRACELENGTH; sample++)
	  {
		// Lese Time und Power
      	lesefehler = fscanf(datei,"%lf %lf", &time, &power);
		if(lesefehler != 2)
        {
          printf("FEHLER !\nIn Datei 'trace_%d' Fehler beim Lesen.\n", i);
	      exit(0);
        }
		// Speichern der Zahl
		anfang -> dTime[sample] = time; 
  		anfang -> dPower[sample] = power;		
	  }
	  anfang -> next = NULL;
	}
	// Lesen aus folgenden Messungen
	else
	{
      // Zeiger auf das erste Element
	  zeiger = anfang;
	  // Iteriere bis zum letzen aktuellen Element
	  while(zeiger -> next != NULL)
      {
		zeiger = zeiger -> next;
	  }
	  // Reservieren Speicherplatz für hinzufügende Element
	  zeiger -> next = malloc(sizeof(AESChallenge));
      if(zeiger -> next == NULL)
	  {
	    // Speicher konnte nicht reserviert werden
	    return 0;
	  }
	  // zeiger auf neuen Speicherplatz
	  zeiger = zeiger -> next;
	  
	  // Füllen des Structs
	  zeiger -> challenge = i;
	  for(sample = 0; sample < TRACELENGTH; sample++)
	  {
		// Lese Time und Power
      	lesefehler = fscanf(datei,"%lf %lf",&time, &power);
		if(lesefehler != 2)
        {
          printf("FEHLER !\nIn Datei 'trace_%d' Fehler beim Lesen.\n", i);
	      exit(0);
        }
		// Speichern der Zahl
		zeiger -> dTime[sample] = time; 
        zeiger -> dPower[sample] = power; 
	  }
	  zeiger -> next = NULL;
	}
	
	// Gelesene datei schließen
	fclose(datei);
  }

  return anfang;
}

MeanAndVar* calculate_mean_var(const AESChallenge* challenge, const unsigned int max_measurements)
{
  // Hilfsvariablen
  int i, j;
  const AESChallenge *zeiger = challenge;;
  // Struct zum Speichern von Mean und Var
  MeanAndVar *result;
  result = malloc(sizeof(MeanAndVar));
  if(result == NULL)
  {
	// Speicher konnte nicht reserviert werden
	return 0; 
  }

  // Dateien zum Schreiben öffnen
  FILE *datei1, *datei2;
  datei1 = fopen("Mean.csv", "w");
  datei2 = fopen("Var.csv", "w");
  if(datei1 == NULL || datei2 == NULL)
  {
    printf("FEHLER !\nDatei konnte nicht geöffnet werden zum Schreiben.\n");
	exit(0);
  } 
  
  if(max_measurements != 0)
  {
    // Berechnen von Mean's (Spaltenweiße)
    for(i = 0; i < TRACELENGTH; i++)
    {
	  // Berechnen der Summe
	  for(j = 0; j < max_measurements; j++)
  	  {
	   result -> dMean[i] += zeiger -> dPower[i];
	   zeiger = zeiger -> next;
  	  }
	  // Fehlende Division
	  result -> dMean[i] /= max_measurements;
	  zeiger = challenge;
	  // Schreiben des Ergebnisses in Datei
      fprintf(datei1, "%f;%.32f\n", zeiger -> dTime[i], result -> dMean[i]);
    }

    // Berechnen von Var's (Spaltenweiße)
    for(i = 0; i < TRACELENGTH; i++)
    {
	  // Berechnen der Summe
	  for(j = 0; j < max_measurements; j++)
  	  {
		result -> dVar[i] += (zeiger -> dPower[i] - result -> dMean[i])*(zeiger -> dPower[i] - result -> dMean[i]);
		zeiger = zeiger -> next;
  	  }
	  // Fehlende Division
	  result -> dVar[i] /= max_measurements - 1;
	  zeiger = challenge;
	  // Schreiben des Ergebnisses in Datei
      fprintf(datei2, "%f;%.32f\n", zeiger -> dTime[i], result -> dVar[i]);
    }
  }
  else
  {
    printf("FEHLER !\nKeine Messungen vorhanden, wodurch Division durch Null bei Berechnungen.\n");
	exit(0);
  }

  // Dateien schließen
  fclose(datei2);
  fclose(datei1);

  return result;
}


unsigned char getSboxOut(unsigned char input, unsigned char key)
{
  unsigned char Sbox[256] =
  {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
    0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
    0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
    0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
    0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
    0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
    0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
    0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
    0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
    0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
    0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
    0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
    0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
    0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
    0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
    0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
    0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
  };
  
  return Sbox[input ^ key];
}


unsigned char getHW(unsigned char b)
{
  unsigned char zaehler = 0;
  
  while(b > 0)
  {
	// Prüfe ob letztes Bit eine 1
    zaehler += b & 0x01;
	// Shift nach Rechts, um restliche Bits zu betrachten
	b = b >> 1;
  }

  return zaehler;
}


unsigned char correlation_attack(const AESChallenge* challenge, const MeanAndVar * MeanVarTrace,  const unsigned int max_measurements)
{
  // Hilfsvariablen
  const AESChallenge *zeiger = challenge;
  int i, j, z;

  // Variablen für Matrix/Varianz/Mittelwert von HW
  unsigned char hw[max_measurements][256];
  double hwMean[256], hwVar[256] = {0};

  // Berechnen der HW-Matrix (Zeilenweiße)
  for(i = 0; i < max_measurements; i++)
  {
	// Durchlaufen aller Keys für ein Input
	for(j = 0; j <= 255; j++)
  	{
	  hw[i][j] = getHW(getSboxOut(zeiger -> challenge, j));
	}
	// Iterieren zum nächsten Input
    zeiger = zeiger  -> next;
  }

  // Berechne Mittelwert von HW Matrix (Spaltenweiße)
  for(i = 0; i <= 255; i++)
  {
	// Berechnen der Summe
	for(j = 0; j < max_measurements; j++)
  	{
		hwMean[i] += hw[j][i];
	}
	// Fehlendene Division
	hwMean[i] /= max_measurements;
  }

  // Berechne Varianz von HW Matrix (Spaltenweiße)
  for(i = 0; i <= 255; i++)
  {
	// Berechnen der Summe
	for(j = 0; j < max_measurements; j++)
  	{
		  hwVar[i] += (hw[j][i] - hwMean[i]) * (hw[j][i] - hwMean[i]);
	}
	// Fehlendene Division
	hwVar[i] /= max_measurements - 1;
  }
  
  // Variablen für Correlation
  double cor[256][TRACELENGTH] = {{0, 0}};
  double maxCor = 0;
  unsigned char key = 0;

  // Berechnen der Korrelation
  // Iterieren durch H
  for(i = 0; i <= 255; i++)
  {
	// Iterieren durch P
	for(j = 0; j < TRACELENGTH; j++) 
  	{
	  zeiger = challenge;
	  // Prüfen ob Varianz im Nenner gleich Null -> Corr = 0
	  if(MeanVarTrace -> dVar[j] != 0 && hwVar[i] != 0)
	  {
		// Berechnen der Covarianz
		for(z = 0; z < max_measurements; z++)
		{
		  cor[i][j] += (zeiger -> dPower[j] - MeanVarTrace -> dMean[j]) * (hw[z][i] - hwMean[i]);
		  zeiger = zeiger  -> next;
		}
		cor[i][j] /= max_measurements - 1;
	    // Fehledende Divisionen
		cor[i][j] /= sqrt(MeanVarTrace -> dVar[j]) * sqrt(hwVar[i]);
	  }
	  else
	  {	
        cor[i][j] = 0;
	  }
			
	  // Prüfen ob neues Maximum gefunden, wenn ja Key speichern 
	  if(fabs(cor[i][j]) > fabs(maxCor))
	  {
	    maxCor = cor[i][j];
        key = i;
      }
	}
  }

  // Datei zum Schreiben öffnen
  FILE *ziel;
  ziel = fopen("Correlation.csv", "w");
  if(ziel == NULL)
  {
    printf("FEHLER !\nDatei 'Correlation.csv' konnte nicht geöffnet werden zum Schreiben.\n");
	exit(0);
  }

  // In Datei schreiben
  // Korrelationsergebnisse pro Key Spaltenweiße
  zeiger = challenge;
  for(i = 0; i < TRACELENGTH; i++)
  {
	// Schreiben der Zeit
	fprintf(ziel, "%f", zeiger -> dTime[i]);
	zeiger = zeiger  -> next;	
	for(j = 0; j <= 255; j++) 
  	{
      // Schreiben des Ergebnisses
      fprintf(ziel, ";%.32f", cor[j][i]);	
    }
    // Nächste Zeile in Datei
    fprintf(ziel, "\n");
  }
  // Schließen der Datei
  fclose(ziel);

  return key;
}
/**
 * Schlüssel ist 238 in dezimal und 0xEE in hexadezimal
 */
