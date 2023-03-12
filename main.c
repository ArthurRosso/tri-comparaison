#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX_ELT 32768

void fusionner(int t[], int d, int m, int f) {
  int aux[f - d + 1]; // !! Allocation dynamique sur la pile (standard C99)
  int i, j, k;
  memcpy(aux, &t[d],
         (f - d + 1) * sizeof(int)); // Copie des données à fusionner
  i = 0;
  j = m - d + 1;
  k = 0;
  while (i <= m - d && j <= f - d) {
    if (aux[i] <= aux[j]) {
      t[d + k++] = aux[i++]; // aux[i] est plus petit : on le place dans t
    } else {
      t[d + k++] = aux[j++]; // aux[j] est plus petit : on le place dans t
    }
  }
  for (; i <= m - d; t[d + k++] = aux[i++]); // le reste du tableau gauche
  for (; j <= f - d; t[d + k++] = aux[j++]); // le reste du tableau droit
}

void tri_fusion(int t[], int debut, int fin) {
  int milieu;
  if (debut < fin) {
    milieu = (debut + fin) / 2;
    tri_fusion(t, debut, milieu);
    tri_fusion(t, milieu + 1, fin);
    fusionner(t, debut, milieu, fin);
  }
}

//Permutação dos elementos
void echanger(int t[], int i, int j) {
  int temp = t[i];
  t[i] = t[j];
  t[j] = temp;
}

int partitionner(int t[], int d, int f) {
  int i = d;
  int j = f - 1; // On utilise i et j comme « pointeurs » qui se déplacent
  int pivot = f; // On choisit le dernier élément comme pivot

  while (i < j) {
    // On déplace i et j jusqu’à trouver des valeurs incohérentes % pivot
    while ((i < j) && (t[i] <= t[f])) {
      i++;
    }
    while ((i < j) && (t[j] > t[f])) {
      j--;
    }

    if (i < j) {
      echanger(t, i, j);
      i++;
      j--;
    }
  }
  if (t[i] <= t[f])
    i++; // Cf. ci-contre
  echanger(t, i, f);
  return i;
}

void tri_rapide(int t[], int debut, int fin) {
  int iPivot;
  if (fin > debut) {
    iPivot = partitionner(t, debut, fin);
    tri_rapide(t, debut, iPivot - 1);
    tri_rapide(t, iPivot + 1, fin);
  }
}

int cmpfunc(const void *a, const void *b) { return (*(int *)a - *(int *)b); }

int exists(int *M, int k, int n) {
  if (k >= 0 && k < n) {
    return 1;
  }
  return 0;
}

int pere(int k) { return (int)floor((k - 1) / 2); }

void remonter(int *M, int k, int n) {
  while (exists(M, pere(k), n) && M[k] > M[pere(k)]) {
    echanger(M, k, pere(k));
    k = pere(k);
  }
}

void transformerEnMaximierV1(int *M, int n) {
  int k;
  for (k = 1; k < n; k++) {
    remonter(M, k, n);
  }
}

int estFeuille(int *M, int k, int n) {
  if (2 * k >= (n - 1)) {
    return 1;
  }
  return 0;
}

int FG(int k) { return (2 * k) + 1; }

int FD(int k) { return (2 * k) + 2; }

void descendre(int *M, int k, int n) {
  if (k >= n) {
    return;
  }

  int i;

  while (!estFeuille(M, k, n)) {
    i = FG(k);
    if (exists(M, FD(k), n) && M[FD(k)] > M[FG(k)]) {
      i = FD(k);
    }

    if (M[k] >= M[i]) {
      break;
    } else {
      echanger(M, k, i);
      k = i;
    }
  }
}

void tri_tas(int *M, int n) {
  transformerEnMaximierV1(M, n);
  while (n > 1) {
    echanger(M, 0, n - 1);
    n--;
    descendre(M, 0, n);
  }
}


int main() {
  int i, n;
  int *vet;
  clock_t start, end;
  double time_taken_fusion;
  double time_taken_rapide;
  double time_taken_qsort;
  double time_taken_tas;
  srand(time(NULL));

  printf("x tri_fusion tri_rapide qsort tri_tas\n");

  for (n = 0; n <= MAX_ELT; n+=512) {
    vet = (int *)malloc(n * sizeof(int));

    for (i = 0; i < n; i++) {
      // generating random values between zero and 100
      vet[i] = rand() % 100;
    }

    start = clock();
    tri_fusion(vet, 0, n);
    end = clock();
    //counts the time the function takes to run
    time_taken_fusion = ((double)(end - start)) / CLOCKS_PER_SEC;

    free(vet);
    vet = (int *)malloc(n * sizeof(int));
    for (i = 0; i < n; i++) {
      // generating random values between zero and 100
      vet[i] = rand() % 100;
    }

    start = clock();
    tri_rapide(vet, 0, n);
    end = clock();

    time_taken_rapide = ((double)(end - start)) / CLOCKS_PER_SEC;

    free(vet);
    vet = (int *)malloc(n * sizeof(int));
    for (i = 0; i < n; i++) {
      // generating random values between zero and 100
      vet[i] = rand() % 100;
    }

    start = clock();
    qsort(vet, n, sizeof(int), cmpfunc);
    end = clock();

    time_taken_qsort = ((double)(end - start)) / CLOCKS_PER_SEC;

    vet = (int *)malloc(n * sizeof(int));

    for (i = 0; i < n; i++) {
      // generating random values between zero and 100
      vet[i] = rand() % 100;
    }

    start = clock();
    tri_tas(vet, n);
    end = clock();
    //counts the time the function takes to run
    time_taken_tas = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("%d %f %f %f %f\n", n, time_taken_fusion, time_taken_rapide, time_taken_qsort, time_taken_tas);

    free(vet);
  }

  return 0;
}

//plot './plot3' u 1:2 w l lc 3 t 'tri tas','./plot3' u 1:3 w l lc 4 t 'tri fusion', './plot3' u 1:4 w l lc 1 t 'tri rapide', './plot3' u 1:5 w l lc 15 t 'qsort'