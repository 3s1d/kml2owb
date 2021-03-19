PROG=kml2owb
CPPFLAGS=-O3 -Werror -Wall -Wno-unused-result
BUILDDIR=build
LDFLAGS=-lboost_system -lboost_filesystem -lexpat -lboost_iostreams -lboost_regex -lkmlbase -lkmlconvenience -lkmlengine -lkmldom
OBJS=coord.o kml2owb.o wpt.o wpthandler.o xml.o

all: $(OBJS)
	mkdir -p $(BUILDDIR)
	$(CXX) -o $(BUILDDIR)/$(PROG) -I/usr/include $(OBJS) $(LDFLAGS)

clean:
	rm -rf *.o $(BUILDDIR)

install:
	cp $< build/$(PROG) /usr/local/bin

uninstall:
	rm -f /usr/local/bin/json2oab
