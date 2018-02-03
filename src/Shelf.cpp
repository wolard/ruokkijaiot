
#include "Shelf.h"



Shelf::Shelf(uint8_t pin, Feedtime *ftime)
{
 pinMode(pin, OUTPUT);
  _pin = pin;
  _ftim=*ftime;
  
}  
void Shelf::InitShelf()
{
Serial.println("tunnit");
Serial.println(_ftim.Hours);

}


