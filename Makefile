LIBNAME=libssstv
CXX = g++
CXFLAGS=-Wall -pg -DSSTV_DEBUG
LDFLAGS += -lfreeimage -lopenal -lsndfile -pg

CXFILES = FFT.cc DSPFunctions.cc Main.cc \
	SoundOpenAl.cc SstvRx.cc SstvTx.cc \
	my_complex.cc SoundSndfile.cc \
	ImageFunctions.cc
	
OBJFILES = $(patsubst %.cc,%.o,$(CXFILES))

all: $(LIBNAME)

$(LIBNAME): $(OBJFILES)
	$(CXX) $(LDFLAGS) -o $@ $(OBJFILES)

$(OBJFILES): %.o: %.cc
	$(CXX) $(CXFLAGS) $(LDFLAGS) -c $< -o $@

clean:
	rm -f $(LIBNAME)
	rm -f *.o
