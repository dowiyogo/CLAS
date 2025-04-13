#include <cstdlib>
#include <time.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>

#include "../Adafruit_ADS1015.h"

static const int CHANNEL = 0;
#define BUTTON_PIN 7
#define ADC_WIRE 1
#define ADC_REOSTATE 0

Adafruit_ADS1115 ads;

volatile uint16_t val_wire;
volatile uint16_t val_reostate;

char DATAFILE[50] = "DATA/ADC_Values.txt";

int main(int argc, char *argv[])
{
  if (wiringPiSetup () < 0) {
    printf ("Unable to setup wiringPi\n");
    return 1;
  }
  time_t rawtime;
  struct tm * timeinfo;
  //std::ofstream ofile(DATAFILE,std::ofstream::out | std::ofstream::app);
  ads.setGain(GAIN_ONE);
  ads.begin();
  val_wire = ads.readADC_SingleEnded(ADC_WIRE);
  //usleep(50000);
  //val_reostate = ads.readADC_SingleEnded(ADC_REOSTATE);
  time (&rawtime);
  timeinfo = localtime (&rawtime);

  //std::cout<<val_wire<<"\t"<<val_reostate<<"\t"<<asctime(timeinfo);
  std::cout<<val_wire<<"\t"<<asctime(timeinfo);
  return 0;
}
 
