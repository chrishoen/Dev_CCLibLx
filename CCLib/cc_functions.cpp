/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <time.h>

#include "cc_functions.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Min,Max

double cc_fmin (double aA,double aB) {return aA > aB ? aB : aA;}
double cc_fmax (double aA,double aB) {return aA > aB ? aA : aB;}

int    cc_imin (int aA,int aB) {return aA > aB ? aB : aA;}
int    cc_imax (int aA,int aB) {return aA > aB ? aA : aB;}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Close to

bool cc_closeto (double aA,double aB,double aResolution)
{
   return (fabs(aA - aB) <= aResolution);
}

bool cc_closeto (double aA,double aB,int aResolution)
{
   switch (aResolution)
   {
      case  6  : return (fabs(aA - aB) <= 1000000.0);
      case  5  : return (fabs(aA - aB) <= 100000.0);
      case  4  : return (fabs(aA - aB) <= 10000.0);
      case  3  : return (fabs(aA - aB) <= 1000.0);
      case  2  : return (fabs(aA - aB) <= 100.0);
      case  1  : return (fabs(aA - aB) <= 10.0);
      case  0  : return (fabs(aA - aB) <= 1.0);
      case -1  : return (fabs(aA - aB) <= 0.1);
      case -2  : return (fabs(aA - aB) <= 0.01);
      case -3  : return (fabs(aA - aB) <= 0.001);
      case -4  : return (fabs(aA - aB) <= 0.0001);
      case -5  : return (fabs(aA - aB) <= 0.00001);
      case -6  : return (fabs(aA - aB) <= 0.000001);
      case -7  : return (fabs(aA - aB) <= 0.0000001);
      case -8  : return (fabs(aA - aB) <= 0.00000001);
      default  : return (fabs(aA - aB) <= 0.01);
   }
   return false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Round to nearest integer

int cc_round (double aA)
{
   if   (aA >= 0.0)  return int(aA + 0.5);
   else              return int(aA - 0.5);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Random numbers

// Seeds random with current time
void cc_srand()
{
   unsigned int tTime = ((unsigned int)time(NULL))&0x7fff;
   srand(tTime*117);
}

// Return random double 0.0 <= x < 1.0
double cc_frand()
{
   return double(rand())/RAND_MAX;
}

// Return random double A <= x < B
double cc_frand(double aA,double aB)
{
   double tR=cc_frand();
   return aA + (aB-aA)*tR; 
}

// Return random int 0 <= x < A
int cc_irand(int aA)
{
   return rand() % aA;
}

// Return random int A <= x <= B
int cc_irand(int aA,int aB)
{
   return aA + rand() % (aB - aA + 1);
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
// Strings

// Trim CRLF from the end of a string
void cc_trimCRLF(char* aString)
{
   // Remove cr/lf at end of line
   unsigned tStringLen = (unsigned)strlen(aString);
   if (aString[tStringLen-1]==0xa)
   {
      aString[tStringLen-1]=0;

      if (aString[tStringLen-2]==0xd)
      {
         aString[tStringLen-2]=0;
      }
   }
}

// Return string "true" "false" for a bool
char* cc_string_from_bool(bool aValue)
{
   return aValue ? (char*)"true" : (char*)"false";
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Time conversions

double cc_time_from_string(char* aString)
{
   int tLength = (int)strlen(aString);
   char tString[40];
   double tTime=0.0;

   switch (aString[tLength-1])
   {
   case 's' :
   case 'S' :
      strncpy(tString,aString,tLength-1);
      tTime = atof(tString);
      break;
   case 'm' :
   case 'M' :
      strncpy(tString,aString,tLength-1);
      tTime = atof(tString);
      tTime *= 60.0;
      break;
   case 'h' :
   case 'H' :
      strncpy(tString,aString,tLength-1);
      tTime = atof(tString);
      tTime *= 3600.0;
      break;
   default:
      tTime = atof(aString);
      break;
   }
   return tTime;
}

char*  cc_string_from_time(double aTime, char* aBuffer)
{
   int     tTimeSeconds  = (int)aTime;
// double  tTimeSecondsF = aTime;

   int  tSecInOneMin   = 60;
   int  tSecInOneHour  = 60*60;
   int  tSecInOneDay   = 60*60*24;

   int  tSecInDay  = tTimeSeconds  % tSecInOneDay;
   int  tSecInHour = tTimeSeconds  % tSecInOneHour;
   int  tSecInMin  = tTimeSeconds  % tSecInOneMin;

   int  tHourInDay = tSecInDay  / tSecInOneHour;
   int  tMinInHour = tSecInHour / tSecInOneMin;

   sprintf(aBuffer,"%02d:%02d:%02d",tHourInDay,tMinInHour,tSecInMin);
   return aBuffer;
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
// Convert string to upper case

void cc_strupr(char* aString)
{
   unsigned char* tPtr = (unsigned char*)aString;
   int tIndex=0;
   while (tPtr[tIndex]!=0)
   {
      int tValue = (int)tPtr[tIndex];
      if ((tValue >= 97)&&(tValue <= 122))
      {
         tValue -= 32;
         tPtr[tIndex] = (unsigned char)tValue;
      }
      tIndex++;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy string

void cc_strncpy(char* aDestin,const char* aSource,unsigned int aSize)
{
   strncpy(aDestin,aSource,(size_t)aSize);
   aDestin[aSize-1]=0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// File operations

bool cc_copy_file(char* aSourcePath, char* aDestinPath)
{

   //--------------------------------------------------------------
   // Open source file

   FILE* tSourceFile = fopen(aSourcePath,"rb");
   if (tSourceFile == 0) return false;

   // Get source file size
   fseek(tSourceFile,0, SEEK_END);
   int tSourceSize = ftell(tSourceFile);
   rewind(tSourceFile);

   //--------------------------------------------------------------
   // Open destination file

   FILE* tDestinFile = fopen(aDestinPath,"wb");
   if (tDestinFile == 0) return false;

   //--------------------------------------------------------------
   // Copy source file to destination file

   char* tBuffer = (char*)malloc(1024);
   int  tReadResult  = 0;
   int  tWriteResult = 0;
   int  tReadCount   = 0;
   int  tWriteCount  = 0;

   // Loop 
   bool tGoing=true;
   while (tGoing)
   {
      // Read into buffer from source file
      tReadResult  = (int)fread(tBuffer,1,1024,tSourceFile);
      tReadCount += tReadResult;

      // Guard
      if (ferror(tSourceFile)) return false;

      // Write buffer to destination file
      tWriteResult = (int)fwrite(tBuffer,1,tReadResult,tDestinFile);
      tWriteCount += tWriteResult;

      // Guard
      if (ferror(tDestinFile)) return false;

      // Done 
      if (tWriteCount >= tSourceSize) tGoing=false;
   }


   //--------------------------------------------------------------
   // Done

   // Close files
   fclose(tSourceFile);
   fclose(tDestinFile);
   // Free
   free((void*)tBuffer);

   return true;
}

bool cc_compare_files (char* aFile1Path, char* aFile2Path, int aNumOfBytes)
{
   bool tSameFlag = true;
   bool tGoing    = true;

   //--------------------------------------------------------------
   // Open files

   FILE* tFile1File = fopen(aFile1Path,"rb");
   if (tFile1File == 0) return false;

   FILE* tFile2File = fopen(aFile2Path,"rb");
   if (tFile2File == 0) return false;

   //--------------------------------------------------------------
   // Allocate memory

   char* tBuffer1 = (char*)malloc(aNumOfBytes);
   char* tBuffer2 = (char*)malloc(aNumOfBytes);

   //--------------------------------------------------------------
   // Read from files

   int tReadResult1  = (int)fread(tBuffer1,1,aNumOfBytes,tFile1File);
   int tReadResult2  = (int)fread(tBuffer2,1,aNumOfBytes,tFile2File);

   //--------------------------------------------------------------
   // Compare read sizes

   if (tReadResult1 != tReadResult2)
   {
      tSameFlag = false;
      tGoing = false;
   }

   //--------------------------------------------------------------
   // Compare file content

   if (tGoing)
   {
      for (int i = 0; i < tReadResult1; i++)
      {
         if (tBuffer1[i] != tBuffer2[i]) 
         {
            tSameFlag = false;
            tGoing = false;
            break;
         }
      }
   }

   //--------------------------------------------------------------
   // Done

   // Close files
   fclose(tFile1File);
   fclose(tFile2File);

   // Free
   free((void*)tBuffer1);
   free((void*)tBuffer2);

   return tSameFlag;
}


