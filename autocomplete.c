#include "autocomplete.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

int cmpterm(const void *a, const void *b){
    struct term *termA = (struct term *)a;
    struct term *termB = (struct term *)b;
    return strcmp(termA->term, termB->term);
}

int cmpweight(const void *a, const void *b){
    struct term *termA = (struct term *)a;
    struct term *termB = (struct term *)b;
    return (termB->weight - termA->weight);
}

void read_in_terms(struct term **terms, int *pnterms, char *filename) {
    char line[200];
    char *string;

    FILE *fp = fopen(filename, "r");
    fgets(line, sizeof(line), fp);
    *pnterms = atoi(line);
    *terms = (struct term *)malloc(*pnterms * sizeof(struct term));
    for(int i = 0; i < *pnterms; i++){
        fgets(line, sizeof(line), fp);
        (*terms)[i].weight = strtod(line, &string);
        strncpy((*terms)[i].term, string + 1, strlen(string) - 2);
    }
    qsort(*terms, *pnterms, sizeof(struct term), cmpterm);
    fclose(fp);
}

int lowest_match(struct term *terms, int nterms, char *substr){
    int l = 0;
    int r = nterms;
    while(l < r){
        int m = floor((l + r)/2);
        if (strncmp(terms[m].term, substr, strlen(substr)) < 0){
            l = m + 1;
        }
        else{
            r = m;
        }
    }
    return l;
}

int highest_match(struct term *terms, int nterms, char *substr){
    int l = 0;
    int r = nterms;
    while(l < r){
        int m = floor((l + r)/2);
        if (strncmp(terms[m].term, substr, strlen(substr)) > 0){
            r = m;
        }
        else{
            l = m + 1;
        }
    }
    return r - 1;
}

void autocomplete(struct term **answer, int *n_answer, struct term *terms, int nterms, char *substr){
    int lowest = lowest_match(terms, nterms, substr);
    int highest = highest_match(terms, nterms, substr);
    *n_answer = highest - lowest + 1;
    *answer = (struct term *)malloc(*n_answer * sizeof(struct term));
    for (int i = 0; i < *n_answer; i++){
        (*answer)[i] = terms[i+lowest];
    }
    qsort(*answer, *n_answer, sizeof(struct term), cmpweight);
}