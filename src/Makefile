# variables
OBJDIR = obj

OBJECTS = $(OBJDIR)/main.o \
          $(OBJDIR)/game.o \
          $(OBJDIR)/Video.o \
          $(OBJDIR)/Audio.o \
          $(OBJDIR)/Font.o \
          $(OBJDIR)/Tetrion.o \
          $(OBJDIR)/Block.o \
          $(OBJDIR)/Tetramino.o

INCLUDE = -I../include
SDL_FLAGS = `sdl-config --cflags`
LIB = `sdl-config --libs`
LD_FLAGS =


# targets
all: $(PROGNAME)

$(PROGNAME): $(OBJDIR) $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LIB) $(LD_FLAGS)

$(OBJDIR):
	mkdir $@

clean:
	-rm -rf $(OBJDIR) *.o $(PROGNAME) core a.out *.log tags


# dependancies
$(OBJDIR)/main.o: main.cpp ../include/game.h ../include/Tetrion.h ../include/Video.h ../include/Audio.h ../include/Block.h ../include/state.h
	$(CXX) -o $@ $(INCLUDE) $(CFLAGS) $(SDL_FLAGS) -c $<

$(OBJDIR)/game.o: game.cpp ../include/game.h ../include/Tetrion.h ../include/Video.h ../include/Audio.h ../include/Font.h ../include/Block.h ../include/Tetramino.h ../include/scores.h ../include/state.h
	$(CXX) -o $@ $(INCLUDE) $(CFLAGS) $(SDL_FLAGS) -c $<

$(OBJDIR)/Video.o: Video.cpp ../include/Video.h ../include/Font.h ../include/Tetrion.h ../include/Tetramino.h ../include/Block.h ../include/state.h
	$(CXX) -o $@ $(INCLUDE) $(CFLAGS) $(SDL_FLAGS) -c $<

$(OBJDIR)/Audio.o: Audio.cpp ../include/Audio.h
	$(CXX) -o $@ $(INCLUDE) $(CFLAGS) $(SDL_FLAGS) -c $<

$(OBJDIR)/Font.o: Font.cpp ../include/Font.h ../include/Video.h
	$(CXX) -o $@ $(INCLUDE) $(CFLAGS) $(SDL_FLAGS) -c $<

$(OBJDIR)/Tetrion.o: Tetrion.cpp ../include/Tetrion.h ../include/Block.h ../include/state.h
	$(CXX) -o $@ $(INCLUDE) $(CFLAGS) $(SDL_FLAGS) -c $<

$(OBJDIR)/Block.o: Block.cpp ../include/Block.h ../include/Video.h
	$(CXX) -o $@ $(INCLUDE) $(CFLAGS) $(SDL_FLAGS) -c $<

$(OBJDIR)/Tetramino.o: Tetramino.cpp ../include/Tetramino.h ../include/Tetrion.h ../include/Block.h ../include/Video.h
	$(CXX) -o $@ $(INCLUDE) $(CFLAGS) $(SDL_FLAGS) -c $<
