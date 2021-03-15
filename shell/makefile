TARGET = 0_input.c 1_display.c 2_cases.c 3_commands.c main.c
PROGRAM = shell
OBJECTS =$(TARGET:.c=.o)
all debug: $(PROGRAM)
 $(PROGRAM) : $(INCLUDES) $(OBJECTS)
				$(LINK.c) -o $@ $(OBJECTS) $(SLIBS)
# shell: 0_input.c 1_display.c 2_cases.c main.c
# 	gcc -o shell 0_input.c 1_display.c 2_cases.c main.c -I.
