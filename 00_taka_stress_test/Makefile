
SRCS:= main.cpp  pp_tst.cpp table_hdr.cpp table_body.cpp table.cpp xio.cpp

OBJS:= $(SRCS:.cpp=.o)
MSG = 
REP = mexes

NAME = pp_tst

CC = c++
STD_CPP = c++98
DEBUG = 
CFLAGS = -Werror -Wall -Wextra -std=$(STD_CPP)
INCLUDES = -I./

XLIB_XSTRING = libxstring.a
XLIB_PATH_XSTRING = ./xstring

LIBFLAGS_STATIC = -L $(XLIB_PATH_XSTRING) -lxstring
#=== DEBUG ====
ifeq ($(DEBUG), 1)
	CFLAGS := $(CFLAGS) -g -D__DEBUG__=1
endif
	
#=== COLORS ====
COL_Y = "\033[1;32m"
COL_G = "\033[1;33m"
COL_D = "\033[0m"


NAME : all
all : $(NAME)
$(NAME) : $(OBJS) $(XLIB_XSTRING) 
	@echo "\033[1;33mCompiling Executables: $(NAME) \033[0m"
	$(CC) $^ $(LIBFLAG_STATIC) $(CFLAGS) $(INCLUDES) -o $@
	@echo; echo "\033[1;32mCompilation Successful. \033[0m"
	@echo; echo

	
# obj files output
%.o : %.c
	@echo
	@echo "\033[1;33mCompiling OBJ files \033[0m"
	$(CC) -c $^ $(CFLAGS) $(INCLUDES) -o $@
	@echo

$(XLIB_XSTRING) :
	@echo "\033[1;33mCompiling library: libxstring.a \033[0m"
	@cd $(XLIB_PATH_XSTRING); make; cp $(XLIB_XSTRING) ../; cd ..
	@echo

# remove all object files
fclean:
	rm -rf *.o
	@cd $(XLIB_PATH_XSTRING); make fclean; cd ..

# remove final target files
clean: fclean
	rm -rf $(NAME)
	rm -rf ./*.a

# recompile everything
re: clean all

#=================== GIT ===============================
#define comments
push:	clean
	$(info Pushing to Git Repository)
ifeq ($(REP), 42)
	@git push wolfsburg main
	@echo $(COL_G)Pushed to repo: $(REP)$(COL_D)
#	@git rm -f --cached *.pdf *.md			#	2> /dev/null
else 
	@git add ../*
	@git ls-files; git status
	@git commit -m "$(MSG)"
	@git push mexes main
	@echo $(COL_G)Pushed to repo: mexes$(COL_D)
	@echo $(COL_G)$(MSG)$(COL_D)
endif

#endef # comments

.PHONY : all fclean clean re push debug
