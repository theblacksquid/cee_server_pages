
FLAGS_DEFAULT=\
-g3                   \
-Wall                 \
-Wextra               \
-Wconversion          \
-Wdouble-promotion    \
-Wno-unused-parameter \
-Wno-unused-function  \
-Wno-sign-conversion  \
-fsanitize=undefined

cee_templ: src/main.c src/libblacksquid.h
	gcc $(FLAGS_DEFAULT) src/main.c -o cee_templ

