APPNAME=ssstv-cli
CXX = g++
CXFLAGS=-Wall -pg -DSSTV_DEBUG -I../libssstv -L../libssstv
LDFLAGS += -lfreeimage -lopenal -lsndfile -lssstv -pg

CXFILES = Main.cc
	
OBJFILES = $(patsubst %.cc,%.o,$(CXFILES))

all: $(APPNAME)

$(APPNAME): $(OBJFILES)
	$(CXX) $(CXFLAGS) $(LDFLAGS) -o $@ $(OBJFILES)

$(OBJFILES): %.o: %.cc
	$(CXX) $(CXFLAGS) $(LDFLAGS) -c $< -o $@

clean:
	rm -f $(APPNAME)
	rm -f *.o
