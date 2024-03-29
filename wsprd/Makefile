CC = gcc
FC = gfortran

# Add homebrew path for macOS
CFLAGS= -I/opt/homebrew/include -I/usr/local/include -I/usr/include -Wall -Wno-missing-braces -Wno-unused-result -O3 -ffast-math
#CFLAGS= -I/usr/local/include -I/usr/include -Wall -Wno-missing-braces -Wno-unused-result -O
# static libgfortran path for macOS
LDFLAGS = -L/opt/homebrew/lib/gcc/13 -L/opt/homebrew/lib -L/usr/lib
FFLAGS = -O2 -Wall -Wno-conversion -fbounds-check -fmax-stack-var-size=1048576
LIBS = -lfftw3f -lm -lgfortran

# Default rules
%.o: %.c $(DEPS)
	${CC} ${CFLAGS} -c $<
%.o: %.f
	${FC} ${FFLAGS} -c $<
%.o: %.F
	${FC} ${FFLAGS} -c $<
%.o: %.f90
	${FC} ${FFLAGS} -c $<
%.o: %.F90
	${FC} ${FFLAGS} -c $<

all:    wsprd wsprsim wsprsimwav wsprcode

DEPS =  wsprsim_utils.h wsprd_utils.h fano.h jelinek.h nhash.h

indexx.o: ./indexx.f90
	${FC} -o indexx.o ${FFLAGS} -c ./indexx.f90

OBJS1 = wsprd.o wsprsim_utils.o wsprd_utils.o tab.o fano.o jelinek.o nhash.o indexx.o osdwspr.o

wsprd: $(OBJS1)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

OBJS2 = wsprsim.o wsprsim_utils.o wsprd_utils.o tab.o fano.o nhash.o

wsprsim: $(OBJS2)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

OBJS3 = wsprsimwav.o wsprsim_utils.o wsprd_utils.o tab.o fano.o nhash.o

wsprsimwav: $(OBJS3)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

OBJS4 = wsprcode_s.o nhash_s.o

wsprcode: $(OBJS4)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

clean:
	$(RM) *.o wsprd wsprsim wsprsimwav wsprcode

format: .clang-format
	clang-format -style=file -i *.c *.h
	fprettify -w 4 *.f90
