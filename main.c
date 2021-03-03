#include "autocomplete.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    struct term *terms;
    int nterms;
    int lowest, highest;
    read_in_terms(&terms, &nterms, "cities.txt");
    lowest = lowest_match(terms, nterms, "Tor");
    highest = highest_match(terms, nterms, "Tor");

    struct term *answer;
    int n_answer;
    autocomplete(&answer, &n_answer, terms, nterms, "Tor");

    /*
    for (int i = 0; i < nterms; i++){
        printf("Term = %s, Weight = %f\n", terms[i].term, terms[i].weight);
    }
    printf("Lowest Match = %d, Term = %s\n", lowest, terms[lowest].term);
    printf("Highest Match = %d, Term = %s\n", highest, terms[highest].term);
    */
    
    for (int j = 0; j < n_answer; j++){
        printf("Autocomplete Answer: %s, Weight = %f\n", answer[j].term, answer[j].weight);
    }
    //free allocated blocks here -- not required for the project, but good practice
    free(terms);
    return 0;
}
