#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdlib.h>
#include <wiringPi.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

#define FWD 28
#define BWD 29


int fd=0, n;
char buf[100];
/*
 * 'open_port()' - Open serial port 1.
 *
 * Returns the file descriptor on success or -1 on error.
 */

int open_port(void)
{
  fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1)  {
   /*
    * Could not open the port.
    */
    perror("open_port: Unable to open /dev/ttyf1 - ");
  }
  else

    fcntl(fd, F_SETFL, 0);

  return (fd);
}

bool check_fwd(){
  return !digitalRead(FWD);
}

bool check_bwd(){
  return !digitalRead(BWD);
}

int main(int argc, char *argv[])
{
  if (argc!=2)
  {
    std::cout<<"you must insert step to move fordward (+x) or backward (-x)"
      "./move_piezomotor +x)"<<std::endl;
    exit (1);  
  }
  if (wiringPiSetup () < 0) {
    printf ("Unable to setup wiringPi\n");
    return 1;
  }
  pinMode(FWD,INPUT);
  pinMode(BWD,INPUT);
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);

  if (check_fwd() && check_bwd()){
    
    std::cout<<"ERROR: Possible  Raspberry melt down!!!!\t"<<asctime(timeinfo)<<std::endl;
    exit(1);
  }
  else if (check_fwd() && argv[1][0] == '+'){
    std::cout<<"ERROR: Forward limit reached!!!!\t"<<asctime(timeinfo)<<std::endl;
    exit(1);
  }
  else if (check_bwd() && argv[1][0] == '-'){
    std::cout<<"ERROR: Backward limit reached!!!!\t"<<asctime(timeinfo)<<std::endl;
    exit(1);
  }


  int fd=open_port();

  struct termios options;

  /*
   * Get the current options for the port...
   */
  tcgetattr(fd, &options);
  /*
   * Set the baud rates to 57600...
   */
  cfsetispeed(&options, B57600);
  cfsetospeed(&options, B57600);

  /*
   * Enable the receiver and set local mode...
   */

  options.c_cflag |= (CLOCAL | CREAD);
  options.c_cflag &= ~PARENB;
  options.c_cflag &= ~CSTOPB;
  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8;

  /*
    /*
    * Set the new options for the port...
    */

  tcsetattr(fd, TCSANOW, &options);

  memset(buf, 0, sizeof buf);

  std::string cmd = "H300J";
  cmd += argv[1];
  cmd +=":;";
  std::cout<<"sending: "<<cmd<<std::endl;
  n = write(fd, cmd.c_str(), cmd.size());
  if (n < 0){
    std::cerr<<"write() of "<<cmd.size()<<" bytes failed!\n"<<std::endl;
  }
  return 0;
}

