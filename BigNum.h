#ifndef BIGNUM
#define BIGNUM

// Structure of a representation of a number
typedef struct BigNum {
    // Pointer on functions :
    /*  Add returns the addition between two BigNum.
        Return NULL if at least one of the parameters is NULL.
        BigNum* Add(BigNum*, BigNum*)                            */
    struct BigNum* (*Add)(struct BigNum*, struct BigNum*);

    /*  Sub returns the substraction between two BigNum,
        the first minus the second in parameters.
        Return NULL if at least one of the parameters is NULL or the result is negative
        BigNum* SUb(BigNum*, BigNum*)                            */
    struct BigNum* (*Sub)(struct BigNum*, struct BigNum*);

    /*  Mult returns the multiplition between two BigNum
        Return NULL if at least one of the parameters is NULL
        BigNum* Mult(BigNum*, BigNum*)                            */
    struct BigNum* (*Mult)(struct BigNum*, struct BigNum*);

    /*  Add returns the divition between two BigNum
        Return NULL if at least one of the parameters is NULL or in case of a division by zero
        BigNum* Div(BigNum*, BigNum*)                            */
    struct BigNum* (*Div)(struct BigNum*, struct BigNum*);

    /*  Power returns the result of the first BigNum raises to the second BigNum power
        Return NULL if one of the BigNum is NULL
        BigNum* Power(BigNum*, BigNum*)                         */
    struct BigNum* (*Power)(struct BigNum*, struct BigNum*);

    /*  Factorial returns factorial of a BigNum
        Return NULL if the BigNUm is NULL
        BigNum* Factorial(BigNum*, BigNum*)                     */
    struct BigNum* (*Factorial)(struct BigNum*);

    /*  Compare two BigNum
        Return a positive int if the first BigNum is bigger than the second
        a negative int if not and 0 if equals
        int Compare(BigNum*, BigNum*)                         */
    int (*Compare)(struct BigNum*, struct BigNum*);

    /*  Free tha allocated memory for a BigNum                   */
    void (*Free)(struct BigNum*);

    /*  Display the BigNum                                       */
    void (*View)(struct BigNum*);

    /*  Setter for the sequence of digits
        It also disallocate the former intSequence              */
    void (*SetSequence)(struct BigNum*, int*);

    /* Setter for the size/length of the BigNum                 */
    void (*SetSize)(struct BigNum*, int);

    /*  Getter for the sequence of digits                       */
    int* (*GetSequence)(struct BigNum*);

    /*  Getter for the size/length of the BigNum                */
    int (*GetSize)(struct BigNum*);

    // Data members
    int size;
    int* sequence;
} BigNum;

BigNum* New_BigNum(int number);
BigNum* New_BigNum_By_String(char* strSequence);
BigNum* BigNum_Add(BigNum* this, BigNum* other);
BigNum* BigNum_Sub(BigNum* this, BigNum* other);
BigNum* BigNum_Mult(BigNum* this, BigNum* other);
BigNum* BigNum_Div(BigNum* this, BigNum* other);
BigNum* BigNum_Power(BigNum* this, BigNum* index);
BigNum* BigNum_Factorial(BigNum* this);
int BigNum_Compare(BigNum* this, BigNum* other);
void BigNum_SetSequence(BigNum* this, int* other);
void BigNum_SetSize(BigNum* this, int size);
int* BigNum_GetSequence(BigNum* this);
int BigNum_GetSize(BigNum* this);
void BigNum_Free(BigNum* this);
void BigNum_View(BigNum* this);
BigNum* Combinaison(BigNum*, BigNum*);

#endif
