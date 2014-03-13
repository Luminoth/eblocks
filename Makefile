# general variables
CXX = g++
VERSION = 1.0.0

# application variables
PROGNAME=eblocks

# source directory variables
SRCDIR = src

# creatable directories
BINDIR = bin

# other directories
DATADIR = data


# compiler options
BASE_CFLAGS = -Wall -pipe
#CFLAGS = $(BASE_CFLAGS) -DDEBUG -g -gstabs+ -fno-default-inline
CFLAGS = $(BASE_CFLAGS) -DNDEBUG -O3 -fomit-frame-pointer -ffast-math -fno-common -funroll-loops


# targets
all: $(BINDIR)
	$(MAKE) -C $(SRCDIR) CXX="$(CXX)" PROGNAME="$(PROGNAME)" CFLAGS="$(CFLAGS)"
	cp $(SRCDIR)/$(PROGNAME) $(BINDIR)

$(BINDIR):
	mkdir $(BINDIR)

strip: all
	strip -sv $(BINDIR)/*

clean:
	$(MAKE) -C $(SRCDIR) PROGNAME="$(PROGNAME)" $@
	rm -rf $(BINDIR)

install: strip
	@echo "not done"

package: clean
	cp -R ../$(PROGNAME)/ ../$(PROGNAME)-$(VERSION)-src/
	mv ../$(PROGNAME)-$(VERSION)-src/ ./
	tar -cvf $(PROGNAME)-$(VERSION)-src.tar $(PROGNAME)-$(VERSION)-src/
	gzip $(PROGNAME)-$(VERSION)-src.tar
	rm -rf $(PROGNAME)-$(VERSION)-src/
	$(MAKE) all
	$(MAKE) -C $(SRCDIR) CXX="$(CXX)" PROGNAME="$(PROGNAME)" CFLAGS="$(CFLAGS)"
	tar -cvf $(PROGNAME)-$(VERSION).tar $(BINDIR) $(DATADIR)
	gzip $(PROGNAME)-$(VERSION).tar
	$(MAKE) clean
