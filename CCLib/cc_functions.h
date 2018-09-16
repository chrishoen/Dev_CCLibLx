#pragma once

/*==============================================================================
My math functions
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Arithmentic

   // Min,Max
   double cc_fmin (double aA,double aB);
   double cc_fmax (double aA,double aB);

   int    cc_imin (int aA,int aB);
   int    cc_imax (int aA,int aB);

   // Close to
   bool cc_closeto (double aA,double aB,double aResolution);
   bool cc_closeto (double aA,double aB,int aResolution);

   // Round to nearest integer
   int    cc_round (double aA);

   // Seeds random with current time
   void   cc_srand();

   // Return random double 0.0 <= x < 1.0
   double cc_frand();

   // Return random double A <= x < B
   double cc_frand(double aA,double aB);

   // Return random int 0 <= x < A
   int    cc_irand(int aA);

   // Return random int A <= x <= B
   int    cc_irand(int aA,int aB);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // String

   // Trim CRLF from the end of a string
   void cc_trimCRLF(char* aString);

   // Return string "true" "false" for a bool
   char* cc_string_from_bool(bool aValue);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Time

   // Return seconds from sec,min or hour
   //    1.0 == cc_time_from_string("1.0")
   //    1.0 == cc_time_from_string("1.0s")
   //   60.0 == cc_time_from_string("1.0m")
   // 3600.0 == cc_time_from_string("1.0h")
   double cc_time_from_string(char* aString);

   // Return "01:02:03" from seconds
   char*  cc_string_from_time(double aTime, char* aBuffer);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Index arithmetic

   // Index arithmetic for queues, wraps around
   inline int cc_index_add(int aIndex, int aDelta, int aSize)
   {
      aIndex += aDelta;
      if (aIndex >= aSize) aIndex -= aSize;
      return aIndex;

   }
   inline int cc_index_sub(int aIndex, int aDelta, int aSize)
   {
      aIndex -= aDelta;
      if (aIndex < 0 ) aIndex += aSize;
      return aIndex;

   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Round up to 16 byte boundary

   inline int cc_round_upto16(int aValue)
   {
      return ((aValue & 0xF) != 0) ? ((aValue & ~0xF) + 0x10) : aValue;

   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // File functions

   void cc_strupr(char* aString);

   void cc_strncpy(char* aDestin,const char* aSource,unsigned int aSize);


   bool cc_copy_file(char* aSourcePath, char* aDestinPath);

   bool cc_compare_files (char* aFile1Path, char* aFile2Path, int aNumOfBytes);

//******************************************************************************
//******************************************************************************
//******************************************************************************

