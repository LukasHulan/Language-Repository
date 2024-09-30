/*
* Number data type. Uses a union to handle both integers and doubles.
* 
* Examples
* myNumber1: type: INT, data: 0
* myNumber3: type: INT, data: 5
* myNumber4: type: INT, data: 53
* myNumber5: type: DOUBLE, data: 0.0
* myNumber5: type: DOUBLE, data: 2.5
* myNumber5: type: DOUBLE, data: 10.7
*/

typedef union numberUnion{
    int integer;
    double decimal;
} numberUnion;

typedef enum numberUnionFlag {
    INT,
    DOUBLE
} numberUnionFlag;

typedef struct numberData {
    numberUnion data;
    numberUnionFlag type;
} numberData;
