# Makefile 
CC	= gcc -std=c99
CFLAGS = -Wall -g
CXX = g++ -std=c++11
CXXFLAGS = -Wall -g
#
INCDIRS = -I./

.PHONY: clean

%.o:%.c
	$(CC) $(CCFLAGS) $(INCDIRS) -c $<

%.o:%.cpp
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $<

#
INCLUDES = card.h cardset.h 
DAIFUGOUINCLUDES = $(INCLUDES) dealer.h player.h \
				simpleplayer.h think_ta1.h  testplayer.h
# cheatingplayer.h Group1.h

PLAYINGCARDSOBJS = card.o cardset.o #cardset_remove.o cardset_draw.o
DAIFUGOUOBJS = dealer.o daifugou.o player.o simpleplayer.o testplayer.o
EXTRAOBJS = think_ta1.o #Group1.o Group2.o Group3.o group4.o \
	Group7.o group-no.6.o Group5.o Team8.o

#TAIKAIOBJS = dealer.o securedealer.o taikai.o player.o simpleplayer.o 

all: daifugou.exe 
#all: daifugou.exe taikai.exe

$(DAIFUGOUOBJS): $(DAIFUGOUINCLUDES)
$(PLAYINGCARDSOBJS): $(INCLUDES)
$(TAIKAIOBJS): $(DAIFUGOUINCLUDES) securedealer.h

libplayingcards.a: $(PLAYINGCARDSOBJS) $(INCLUDES)
	ar rcs libplayingcards.a $(PLAYINGCARDSOBJS) 

daifugou.exe: $(DAIFUGOUOBJS) $(EXTRAOBJS) libplayingcards.a
	$(CXX) $(CXXFLAGS) $(INCDIRS) -o daifugou.exe $(DAIFUGOUOBJS) $(EXTRAOBJS) -L. -lplayingcards

taikai.exe: $(TAIKAIOBJS) $(EXTRAOBJS) libplayingcards.a
	$(CXX) $(CXXFLAGS) $(INCDIRS) -o taikai.exe $(TAIKAIOBJS) $(EXTRAOBJS) -L. -lplayingcards

clean: 
	rm -rf $(PLAYINGCARDSOBJS) $(DAIFUGOUOBJS) daifugou.exe *.dSYM *~
#	rm -rf libplayingcards.a think_ta1.o

flush:
	rm -rf *.o *.exe *.a
