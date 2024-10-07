#include <stdio.h>

struct test1 {
    struct {
        int a;
        int b;
    };

    struct {
        int c;
        int d;
    };
};

struct test2 {
    struct {
        int a;
        int b;
    } inner1;

    struct {
        int a;
        int b;
    } inner2;
};

int main() {
    struct test1 myStruct1;
    myStruct1.a = 1;
    myStruct1.b = 2;
    myStruct1.c = 3;
    myStruct1.d = 4;

    struct test2 myStruct2;
    myStruct2.inner1.a = 1;
    myStruct2.inner1.b = 2;
    myStruct2.inner2.a = 3;
    myStruct2.inner2.b = 4;

    printf("Test1: %d, %d, %d, %d", 
           myStruct1.a, myStruct1.b, myStruct1.c, myStruct1.d);
    printf("\n");
    printf("Test2: %d, %d, %d, %d", 
           myStruct2.inner1.a, myStruct2.inner1.b, myStruct2.inner2.a, myStruct2.inner2.b);
    printf("\n");
}