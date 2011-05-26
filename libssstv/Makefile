LIBNAME=libssstv.so
CXX = g++
CXFLAGS=-Wall -fPIC -DSSTV_DEBUG
LDFLAGS += -lfreeimage -lopenal -lsndfile

CXFILES = FFT.cc DSPFunctions.cc\
	SoundOpenAl.cc SstvRx.cc SstvTx.cc \
	my_complex.cc SoundSndfile.cc \
	ImageFunctions.cc
	
OBJFILES = $(patsubst %.cc,%.o,$(CXFILES))

all: $(LIBNAME)

$(LIBNAME): $(OBJFILES)
	$(CXX) $(LDFLAGS) -shared -o $@ $(OBJFILES)

$(OBJFILES): %.o: %.cc
	$(CXX) $(CXFLAGS) $(LDFLAGS) -c $< -o $@

clean:
	rm -f $(LIBNAME)
	rm -f *.o
