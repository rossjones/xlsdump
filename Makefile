#
# Fetch libxsl first from svn
#    svn checkout svn://svn.code.sf.net/p/libxls/code/trunk libxls-code
# Then:
#    ./configure --prefix=/usr/local; make; sudo make install
#
IDIR =/usr/local/include/
LDIR =/usr/local/lib/

CC=gcc
CFLAGS=-I$(IDIR) -pedantic -Wall -Wno-error=zero-length-array

BINDIR=bin
ODIR=obj

LIBS=-lxls -lxlsreader -L$(LDIR)

_OBJ =xlsdump.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	@mkdir -p $(ODIR)
	@mkdir -p $(BINDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

xlsdump: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) -o $(BINDIR)/xlsdump

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ $(BINDIR)/xlsdump