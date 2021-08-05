#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#define PI					(314159265/100000000)
#define TODEGREES			(3.14159265359f / 180.0f)
#define TORADIAN			(180.0f / 3.14159265359f)

void EulerToQuaternion(float angle,float &w, float &x, float &y,float &z);
void EulerToQuaternion(float h, float a, float b, float &w, float &x, float &y, float &z);
bool fexists( const char* FileName );

void itoLittleEndian(int i, char* c, int off);
void stoLittleEndian(short i, char* c, int off);
void ftoLittleEndian(float i, char* c, int off);

int leToI(char* c, int off);
short leToS(char* c, int off);
float leToF(char* c, int off);