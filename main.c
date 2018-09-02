#include <stdio.h>
#include <stdlib.h>

#include "BigNum.h"

int main(int argc, char* argv[]) {
    // TESTING NEWTON'S BINOMIAL FORMULA
    int nb = 90;

    // POWER OF TWO
    BigNum* n = New_BigNum(nb);
    BigNum* two = New_BigNum(2);
    BigNum* ptwo = two->Power(two, n);

    printf("POWER OF TWO:\t\t");
    ptwo->View(ptwo);

    // SUM OF COMBINAISONS
    BigNum* save = NULL;
    BigNum* result = New_BigNum(0);
    BigNum* comb = NULL;
    BigNum* one = New_BigNum(1);
    BigNum* k = New_BigNum(0);

    while (k->Compare(k, n) <= 0) {
        comb = Combinaison(n, k);

        save = result;
        result = result->Add(result, comb);
        save->Free(save);

        comb->Free(comb);

        save = k;
        k = k->Add(k, one);
        save->Free(save);
    }

    printf("SUM OF COMBINAISONS:\t");
    result->View(result);

    n->Free(n);
    two->Free(two);
    ptwo->Free(ptwo);
    result->Free(result);
    one->Free(one);
    k->Free(k);

    return 0;
}
