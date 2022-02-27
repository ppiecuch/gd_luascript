# makefile for tolua library

TOLUA=../..

include $(TOLUA)/config

OBJS=	lua-all.o

T= $(TOLUA)/lib/liblua.a

all: $T

$T: $(OBJS)
	$(AR) $@ $(OBJS)
	$(RANLIB) $@

clean:
	rm -f $(OBJS) 
	
klean:
	rm -f $T

