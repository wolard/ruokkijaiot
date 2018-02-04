
#ifndef Shelf_h
#define Shelf_h

#include "Arduino.h"
#include <TimeLib.h>
struct Feedtime {
  public:
  int Hours;
  int Minutes;
};
class Shelf
{
 private:
   
 
    int _Hour;  
    int _Minute;  
    int _pin;  
    Feedtime _ftim;
   
public:  


Shelf( uint8_t ,Feedtime *ftime);

void InitShelf();



 
};

#endif
