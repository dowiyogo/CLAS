g++ -o newAnalysis -O -w -Wall -fPIC -pthread -m64 -fpermissive \
  -I$ROOTSYS/include \
  newAnalysis.C \
  -L$ROOTSYS/lib -lCore -lCint -lRIO -lNet -lHist \
  -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lMinuit -lMLP -lTreePlayer -lTMVA -lXMLIO\
  -lPhysics -lMathCore -lThread -lGui -lm -ldl -lpthread -rdynamic \



