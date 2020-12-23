CC = gcc
FC = gfortran

CFLAGS= -I/usr/local/include -I/usr/include -Wall -Wno-missing-braces -Wno-unused-result -O3 -ffast-math
#CFLAGS= -I/usr/local/include -I/usr/include -Wall -Wno-missing-braces -Wno-unused-result -O
LDFLAGS = -L/usr/local/lib/gcc/10 -L/usr/local/lib -L/usr/lib
FFLAGS = -O2 -Wall -Wno-conversion
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
