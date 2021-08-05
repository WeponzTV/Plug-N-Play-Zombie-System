// Disable fopen_s warnings
#pragma warning( disable : 4996 )

#include "Utils.h"

void EulerToQuaternion(float angle,float &w, float &x, float &y,float &z)
{
    float c1; float c2; float c3;
	float s1; float s2; float s3;
    c1 = 1.0;
    c2 = cos(angle / 2 / TORADIAN);
    c3 = 1.0;
    s1 = 0.0;
    s2 = sin(angle / 2 / TORADIAN);
    s3 = 0.0;
    w = (c1*c2*c3)-(s1*s2*s3);
    x = (s1*s2*c3)+(c1*c2*s3);
    y = (s1*c2*c3)+(c1*s2*s3);
    z = (c1*s2*c3)-(s1*c2*s3);
}

void EulerToQuaternion(float h, float a, float b, float &w, float &x, float &y, float &z) {
	float c1 = cos(h/2 / TORADIAN);
    float s1 = sin(h/2 / TORADIAN);
    float c2 = cos(a/2 / TORADIAN);
    float s2 = sin(a/2 / TORADIAN);
    float c3 = cos(b/2 / TORADIAN);
    float s3 = sin(b/2 / TORADIAN);
    w =c1*c2*c3 - s1*s2*s3;
  	x =c1*c2*s3 + s1*s2*c3;
	y =s1*c2*c3 + c1*s2*s3;
	z =c1*s2*c3 - s1*c2*s3;

}

bool fexists( const char* FileName )
{
    FILE* fp = fopen( FileName, "rb" );
    
    if( fp != NULL )
    {
        fclose( fp );
        return true;
    }

    return false;
}


void itoLittleEndian(int i, char* c, int off = 0) {
	c[0+off] = (i & 0x000000FF);
	c[1+off] = (i & 0x0000FF00) >> 8;
	c[2+off] = (i & 0x00FF0000) >> 16;
	c[3+off] = (i & 0xFF000000) >> 24;
}

void stoLittleEndian(short i, char* c, int off = 0) {
	c[0+off] = (i & 0x00FF);
	c[1+off] = (i & 0xFF00) >> 8;
}

void ftoLittleEndian(float i, char* c, int off=0) {
	char* c2 = (char*) &i;

	c[0+off] = c2[0];
	c[1+off] = c2[1];
	c[2+off] = c2[2];
	c[3+off] = c2[3];
}

int leToI(char* c, int off) {
	return  (c[0 + off] & 0xFF)
		| ((c[1 + off] & 0xFF) << 8)
		| ((c[2 + off] & 0xFF) << 16)
		| ((c[3 + off] & 0xFF) << 24);
}

short leToS(char* c, int off) {
	return (c[0 + off] & 0xFF)
		| ((c[1 + off] & 0xFF) << 8);
}

float leToF(char* c, int off) {
	float retVal;
	char *returnFloat = (char*)& retVal;

	// swap the bytes into a temporary buffer
	returnFloat[0] = c[3 + off] & 0xFF;
	returnFloat[1] = c[2 + off] & 0xFF;
	returnFloat[2] = c[1 + off] & 0xFF;
	returnFloat[3] = c[0 + off] & 0xFF;

	return retVal;
}