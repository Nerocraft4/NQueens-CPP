// WordDistance.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
using namespace std;


// =============================================================================
// ALUMNOS =====================================================================
// =============================================================================

const char* NombreAlumno = "Pau";
const char* ApellidosAlumno = "Blasco Roca";
const char* NIAAlumno = "1600959"; // NIA alumno

bool CheckNIU(const char* niu)
{
	const char* filename = "NIUSAlumnes.csv";
	ifstream nius(filename);

	if (!nius.good()) {
		cout << "Error opening file NIUSAlumnes.csv" << endl;
		filename = "..\\NIUSAlumnes.csv";
		nius.open(filename);
		if (!nius.good()) {
			cout << "Error opening file ..\\NIUSAlumnes.csv" << endl;
			filename = "..\\..\\NIUSAlumnes.csv";
			nius.open(filename);
			if (!nius.good()) {
				cout << "Error opening file ..\\..\\NIUSAlumnes.csv" << endl;
				filename = "..\\..\\..\\NIUSAlumnes.csv";
				nius.open(filename);
				if (!nius.good()) {
					cout << "Error opening file ..\\..\\..\\NIUSAlumnes.csv" << endl;
					return false;
				}
			}
		}
	}
	cout << filename << " readed" << endl;
	char buf[256];
	while (!nius.eof()) {
		nius.getline(buf, 256);
		if (buf[0] == '\0') break;
		if (strncmp(niu, buf, 7) == 0) return true;
	}
	return false;
}

// =============================================================================
// PROBLEMA ====================================================================
// =============================================================================

// Clock =======================================================================

double Clock()
{
	LARGE_INTEGER cnt;
	LARGE_INTEGER fre;

	if (QueryPerformanceFrequency(&fre) && QueryPerformanceCounter(&cnt)) {
		return (double)cnt.QuadPart / (double)fre.QuadPart;
	}
	else return (double)clock() / (double)CLOCKS_PER_SEC;
}


// DistanciaDeEdicio ===========================================================

/*
* Begin
   Take the strings as input and also find their length.
   For i = 0 to l1
      dist[0][i] = i
   For j = 0 to l2
      dist[j][0] = j
   For j=1 to l1
      For i=1 to l2
         if(s1[i-1] == s2[j-1])
            track= 0
         else
            track = 1
         done
         t = MIN((dist[i-1][j]+1),(dist[i][j-1]+1))
         dist[i][j] = MIN(t,(dist[i-1][j-1]+track))
      Done
   Done
   Print the Levinstein distance: dist[l2][l1]
End
*/

int DistanciaDeEdicio(const char* paraula1, const char* paraula2)
{
	//trobem llargada de les paraules
	int length1 = strlen(paraula1) +1, length2 = strlen(paraula2)+1, i, j, temp, track;
	//printf("Longituds: %d  %d\n", length1, length2);
	// 
	//declarem matriu de distancies
	int** dist = new int* [length2];
	for (int i = 0; i < length2; ++i)
		dist[i] = new int[length1];

	//inicialitzem els costats M[j][i]
	for (i = 0; i < length1; i++) dist[0][i] = i;
	for (j = 0; j < length2; j++) dist[j][0] = j;

	//preomplim
	for (int i = 1; i < length2; i++) {
		for (int j = 1; j < length1; j++) {
			if (paraula1[j-1] == paraula2[i-1]) {
				dist[i][j] = 0;
			}
			else {
				dist[i][j] = 1;
			}
		}
	}

	//recorrem les paraules
	for (i = 1; i < length2; i++) {
		for (j = 1; j < length1; j++) {
			temp = min((dist[i - 1][j] + 1), (dist[i][j - 1] + 1));
			dist[i][j] = min(temp, (dist[i - 1][j - 1] + dist[i][j]));
			//comprovació dels frares, lletres swappejades. Sembla que va bé!
			if (i > 1 && j > 1 && paraula1[i - 1] == paraula2[j] && paraula1[i] == paraula2[j - 1] && paraula1[i - 1] != paraula2[j - 1]) {
				dist[i][j] = min(dist[i][j],dist[i-2][j-2]+1);
			}
			
		}
	}
	
	/*
	printf("    ");
	for (j = 0; j < length1; j++) {
		printf("%c ", paraula1[j]);
	}
	printf("\n");
	for (i = 0; i <= length2; i++) {
		if (i != 0) { printf("%c ", paraula2[i - 1]); }
		else {
			printf("  ");
		}
		for (j = 0; j <= length1; j++) {
			printf("%d ", dist[i][j]);
		}
		printf("\n");
	}
	*/
	//retornem l'última casella
	return dist[length2-1][length1-1];
}

// main ========================================================================

int main(int argc, char *argv[])
{
	if (!CheckNIU(NIAAlumno)) {
		cout << "Indentificacio dels alumnes incorrecte" << endl;
		return 1;
	}
	printf("NIA: %s\n", NIAAlumno);
	printf("NOM: %s\n", NombreAlumno);
	printf("COGNOMS: %s\n", ApellidosAlumno);
	if (argc != 3) {
		cout << "Us: WordDistance paraula1 paraula2" << endl;
		cout << "Sortida: distancia d'edicio" << endl;
		return 1;
	}
	cout << "Paraula 1: " << argv[1] << endl;
	cout << "Paraula 2: " << argv[2] << endl;
	double t0 = Clock();
	int distancia = DistanciaDeEdicio(argv[1], argv[2]);
	double t1 = Clock();
	printf("La distancia de edicio es: %d\n", distancia);
	printf("TEMPS: %lf\n\n", t1 - t0);
	return 0;
}



