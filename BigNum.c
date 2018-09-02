#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "BigNum.h"

static void BigNum_Init(BigNum* this);
int* Simplify_Len(int* resultSequence, int* len);
int* To_Array(int number);

/******************************************************************************************/

BigNum* New_BigNum(int number) {
    BigNum* this = malloc(sizeof(BigNum));
    if (this == NULL) {
        printf("Not enough memory to create this bignum :(\n");
        return NULL;
    }
    BigNum_Init(this);
    this->SetSize(this, (number != 0) ? (int)log10(number) + 1 : 1);
    this->SetSequence(this, To_Array(number));

    return this;
}

/******************************************************************************************/

BigNum* New_BigNum_By_String(char* strSequence) {
    if (strSequence == NULL) return NULL;

    int len = strlen(strSequence);
    int* intSequence = malloc((len + 1) * sizeof(int));
    intSequence[len] = '\0';

    for (int i = 0; i < len; i++) {
        if (strSequence[len - 1 - i] < '0' || '9' < strSequence[len - 1 - i]) {
            printf("Wrong input: %c is not expected\n", strSequence[len - 1 - i]);
            return NULL;
        }
        intSequence[i] = strSequence[len - 1 - i] - '0';
    }
    BigNum* result = New_BigNum(0);
    result->SetSize(result, len);
    result->SetSequence(result, intSequence);

    return result;
}

/******************************************************************************************/

static void BigNum_Init(BigNum* this) {
    this->Add = BigNum_Add;
    this->Sub = BigNum_Sub;
    this->Mult = BigNum_Mult;
    this->Div = BigNum_Div;
    this->Power = BigNum_Power;
    this->Factorial = BigNum_Factorial;
    this->Compare = BigNum_Compare;
    this->SetSequence = BigNum_SetSequence;
    this->SetSize = BigNum_SetSize;
    this->GetSequence = BigNum_GetSequence;
    this->GetSize = BigNum_GetSize;
    this->Free = BigNum_Free;
    this->View = BigNum_View;

    this->size = 0;
    this->sequence = NULL;
}

/******************************************************************************************/

BigNum* BigNum_Add(BigNum* this, BigNum* other) {
    if (this == NULL || other == NULL)	return NULL;

    int len = ((this->Compare(this, other) > 0) ? this->GetSize(this) : other->GetSize(other));
    int carry = 0;
    int* resultSequence = malloc((len + 1) * sizeof(int)) ;

    resultSequence[len] = '\0';

    // Addition operation
    for (int i = 0; i < len; i++) {
        if (i < this->GetSize(this) && i < other->GetSize(other)) {
            resultSequence[i] = this->GetSequence(this)[i] + other->GetSequence(other)[i] + carry;
        } else if (i < this->GetSize(this) && i >= other->GetSize(other)) {
            resultSequence[i] = this->GetSequence(this)[i] + carry;             // Copying the remaining digit in this
        } else {
            resultSequence[i] = other->GetSequence(other)[i] + carry;           // Copying the remaining digit in other
        }
        // Update carry
        if (resultSequence[i] >= 10) {
            resultSequence[i] %= 10;
            carry = 1;
        } else {
            carry = 0;
        }
    }
    if (carry == 1) resultSequence[len] = 1;            // Include the final carry if it's not 0

    // Creating the result BigNum
    BigNum* result = New_BigNum(1);
    len++;
    resultSequence = Simplify_Len(resultSequence, &len);
    result->SetSize(result, len);
    result->SetSequence(result, resultSequence);

    return result;
}

/******************************************************************************************/

BigNum* BigNum_Sub(BigNum* this, BigNum* other){
    if (this == NULL || other == NULL)	return NULL;

    int len = (this->GetSize(this) > other->GetSize(other)) ? this->GetSize(this) : other->GetSize(other);
    int carry = 0;
    int* resultSequence = malloc(len * sizeof(int));

    // Substraction operation
    for (int i = 0; i < len ; i++) {
        if (i < this->GetSize(this) && i < other->GetSize(other)) {
            resultSequence[i] = this->GetSequence(this)[i] - other->GetSequence(other)[i] - carry;
            if (resultSequence[i] < 0) {        // When a carry is needed
                resultSequence[i] += 10;
                carry = 1;
            } else {
                carry = 0;
            }
        } else if (i < this->GetSize(this) && i >= other->GetSize(other)) {
            resultSequence[i] = this->GetSequence(this)[i] - carry;
            carry = 0;
        } else {
            printf("Soustraction negative!\n");
            return NULL;
        }
    }
    // Creating the result BigNum
    BigNum* result = New_BigNum(0);
    resultSequence = Simplify_Len(resultSequence, &len);
    result->SetSize(result, len);
    result->SetSequence(result, resultSequence);

    return result;
}

/******************************************************************************************/

BigNum* BigNum_Mult(BigNum* this, BigNum* other) {
    if (this == NULL || other == NULL)	return NULL;

    BigNum* save = NULL;                                // To save a lost BigNum for disallocating it
    BigNum* result = New_BigNum(0);
    BigNum* subResult = NULL;                           // Result after the multiplication by one digit
    int* subResultSequence = NULL;
    int subLen = 0;                                     // Length of subResult
    int carry = 0;

    // Multiplication operation
    for (int i = 0; i < other->GetSize(other); i++) {
        // Allocate memory
        subLen = this->GetSize(this) + 1 + i;
        subResultSequence = malloc(subLen * sizeof(int));
        for (int k = 0; k < subLen; k++) subResultSequence[k] = 0;          // Reinitialize the sequence

        // Multiply operation
        for (int j = 0; j < this->GetSize(this); j++) {
            subResultSequence[j + i] = this->GetSequence(this)[j] * other->GetSequence(other)[i] + carry;
            carry = subResultSequence[j + i] / 10;               // Can be 0 if the multiplication the last is less than 10
            subResultSequence[j + i] %= 10;
        }
        // Add the potential carry to the array
        subResultSequence[subLen - 1] = carry;
        carry = 0;                                      // Reset the carry

        // Built the sub-result BigNum
        subResult = New_BigNum(0);
        subResultSequence = Simplify_Len(subResultSequence, &subLen);
        subResult->SetSize(subResult, subLen);
        subResult->SetSequence(subResult, subResultSequence);

        // Add the sub-result BigNum with result
        save = result;
        result = result->Add(result, subResult);
        save->Free(save);

        //Free the unecessary
        free(subResultSequence);
        free(subResult);
    }
    return result;
}

/******************************************************************************************/

BigNum* BigNum_Div(BigNum* this, BigNum* other) {
    if (this == NULL || other == NULL)	return NULL;

    if (other->GetSize(other) == 1 && other->GetSequence(other)[0] == 0) {
        printf("ERROR: division by zero.\n");
        return NULL;
    } else if (other->Compare(other, this) > 0) {
        return New_BigNum(0);
    } else if (other->Compare(other, this) == 0) {
        return New_BigNum(1);
    } else {
        BigNum* save = NULL;                    // To save a lost BigNum for disallocating it
        BigNum* subDividend = New_BigNum(0);
        BigNum* ten = New_BigNum(10);
        BigNum* unit = New_BigNum(0);
        int* tmpQuotientSequence = malloc((this->GetSize(this) + 1) * sizeof(int));
        int posDiv = this->GetSize(this) - other->GetSize(other) - 1;
        int i = 0;

        // Copy first part of dividend in subDividend
        subDividend->SetSize(subDividend, other->GetSize(other));
        int* tmpDiv = malloc((this->GetSize(this) + 1) * sizeof(int));
        if (tmpDiv == NULL)	return NULL;
        tmpDiv[this->GetSize(this)] = 0;
        while (i < other->GetSize(other)) {
            tmpDiv[i] = this->GetSequence(this)[this->GetSize(this) - other->GetSize(other) + i];
            i++;
        }
        subDividend->SetSequence(subDividend, tmpDiv);

        // Operation of division
        i = 0;
        while (posDiv >= 0) {
            // Extract a part of the dividend: multiply by 10 and add the next unit digit.
            if (subDividend->Compare(subDividend, other) < 0) {       // Don't enter in "if" when subDividend is bigger than other the first time
                unit->GetSequence(unit)[0] = this->GetSequence(this)[posDiv--];   // Get the next unit digit

                // Multiply by 10
                save = subDividend;
                subDividend = subDividend->Mult(subDividend, ten);
                save->Free(save);		// tmpDiv has been got free here

                // Add the unit digit
                save = subDividend;
                subDividend = subDividend->Add(subDividend, unit);
                save->Free(save);

                //posDiv--;
            }
            // Successive substractions
            tmpQuotientSequence[i] = 0;
            while (subDividend->Compare(subDividend, other) >= 0) {
            	save = subDividend;
                subDividend = subDividend->Sub(subDividend, other);
                save->Free(save);
                tmpQuotientSequence[i]++;           // Add one each time subtracting was possible
            }
            i++;            // Synchronise the rank of the quotient
        }
        // Copy to quotient array
        int* quotientSequence = malloc((i + 1) * sizeof(int));
        if (quotientSequence == NULL)	return NULL;
        quotientSequence[i] = '\0';
        for (int j = 0; j < i; j++) {                               // Inverse order while copying
            quotientSequence[j] = tmpQuotientSequence[i - j - 1];
        }
        // Creating the result (quotient)
        BigNum* result = New_BigNum(0);
        result->SetSize(result, i);
        result->SetSequence(result, quotientSequence);

        free(tmpQuotientSequence);
        ten->Free(ten);
        unit->Free(unit);
        subDividend->Free(subDividend);

        return result;
    }
}

/******************************************************************************************/

BigNum* BigNum_Power(BigNum* this, BigNum* index) {
    if (this == NULL || index == NULL)	return NULL;

    BigNum* result = New_BigNum(1);
    BigNum* save = NULL;
    BigNum* i = New_BigNum(0);
    BigNum* one = New_BigNum(1);

    while (i->Compare(i, index) < 0) {
        save = result;
        result = result->Mult(result, this);
        save->Free(save);

        save = i;
        i = i->Add(i, one);
        save->Free(save);
    }
    // Free the unecessary
    i->Free(i);
    one->Free(one);

    return result;
}

/******************************************************************************************/

BigNum* BigNum_Factorial(BigNum* this) {
    if (this == NULL)	return NULL;

    BigNum* result = New_BigNum(1);
    BigNum* save = NULL;
    BigNum* i = New_BigNum(1);
    BigNum* one = New_BigNum(1);

    while (i->Compare(i, this) <= 0) {
        save = result;
        result = result->Mult(result, i);
        save->Free(save);

        save = i;
        i = i->Add(i, one);
        save->Free(save);

    }
    // Free the unecessary
    i->Free(i);
    one->Free(one);

    return result;
}

/******************************************************************************************/

int BigNum_Compare(BigNum* this, BigNum* other) {
    if (this == NULL || other == NULL) {
        printf("One of the BigNums is NULL.\n");                    // And will return 0
    } else if (this->GetSize(this) != other->GetSize(other)) {

        return this->GetSize(this) - other->GetSize(other);
    } else {
        for (int i = this->GetSize(this) - 1; i >= 0; i--) {        // Compare each digit from the most significant to the less significant
            if (this->GetSequence(this)[i] != other->GetSequence(other)[i]) {
                return this->GetSequence(this)[i] - other->GetSequence(other)[i];
            }
        }
    }
    return 0;           // Case of equality between the two BigNum
}

/******************************************************************************************/

void BigNum_SetSequence(BigNum* this, int* other) {
    if (this->sequence != NULL)	free(this->sequence);
    this->sequence = other;
}

/******************************************************************************************/

void BigNum_SetSize(BigNum* this, int size) {
    this->size = size;
}

/******************************************************************************************/

int* BigNum_GetSequence(BigNum* this) {
    return this->sequence;
}

/******************************************************************************************/

int BigNum_GetSize(BigNum* this) {
    return this->size;
}

/******************************************************************************************/

void BigNum_Free(BigNum* this) {
    free(this->GetSequence(this));
    free(this);
}

/******************************************************************************************/

void BigNum_View(BigNum* this) {
    if (this == NULL) {
        printf("This is NULL\n");
        return;
    }
    int len = this->GetSize(this);
    char* string = malloc((len + 1) * sizeof(char));
    string[len] = '\0';                 // Set the end of the array

    for (int i = 0; i < len; i++) {                             // Conversion for each digit
        string[i] = this->GetSequence(this)[len - 1 - i] + '0';	// Conversion int to char
    }
    printf("%s\n", string);
    free(string);
}

/******************************************************************************************/

int* To_Array(int number) {
    int len = (number != 0) ? (int)log10(number) + 1 : 1;                       // Find the sequence's size
    int* array = malloc((len + 1) * sizeof(int));
    array[len] = -1;                            // End of array is recognizable by -1
    int position = 0;                           // Position in the array
    while (number >= 10) {
        array[position++] = number % 10;        // % 10 to get the unit digit
        number = number / 10;                   // Divided by 10 to remove the units digits
    }
    array[len - 1] = number;                    // Include the last digit in the array

    return array;
}

/******************************************************************************************/

int* Simplify_Len(int* resultSequence, int* len) {
	// Find the correct lenght by ignoring the unsignificative zeros
    while (*len > 1 && resultSequence[(*len) - 1] == 0) (*len)--;

    // Copy of resultSequence in finalSequence
    int* finalSequence = malloc((*len + 1) * sizeof(int));
    finalSequence[*len] = '\0';
    for (int i = 0; i < *len; i++) {
        finalSequence[i] = resultSequence[i];
    }
    free(resultSequence);

    return finalSequence;
}

/******************************************************************************************/

BigNum* Combinaison(BigNum* upper, BigNum* lower) {
	if (upper == NULL || lower == NULL)	return NULL;

    BigNum* numerator = upper->Factorial(upper);
    BigNum* substraction = upper->Sub(upper, lower);
    BigNum* subFact = substraction->Factorial(substraction);
    BigNum* lowerFact = lower->Factorial(lower);
    BigNum* denominator = subFact->Mult(subFact, lowerFact);
    BigNum* result = numerator->Div(numerator, denominator);

    numerator->Free(numerator);
    substraction->Free(substraction);
    subFact->Free(subFact);
    lowerFact->Free(lowerFact);
    denominator->Free(denominator);

    return result;
}
