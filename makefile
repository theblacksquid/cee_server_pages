
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

test/index.h: test/index.htm cee_templ
	./cee_templ < test/index.htm > test/index.h

test_tmpl: test/test_cee_templates.c test/index.h cee_templ
	gcc $(FLAGS_DEFAULT) test/test_cee_templates.c -o test_tmpl;
	./test_tmpl;

clean:
	-rm ./cee_templ;
	-rm ./test_tmpl;
	-rm ./test/index.h
