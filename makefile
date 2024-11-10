
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
	gcc $(FLAGS_DEFAULT) src/main.c -o cee_templ;

test/index.h: test/index.htm cee_templ
	./cee_templ < test/index.htm > test/index.h;

test/arbitrary_text.h: test/arbitrary_text.txt
	./cee_templ < test/arbitrary_text.txt > test/arbitrary_text.h;

test/json_test.h: test/json_test.json cee_templ
	cat test/json_test.json | ./cee_templ > test/json_test.h;

test_tmpl: test/test_cee_templates.c test/index.h cee_templ
	gcc $(FLAGS_DEFAULT) test/test_cee_templates.c -o test_tmpl;
	./test_tmpl;

plaintext_test: cee_templ test/plaintext_test.c test/arbitrary_text.h
	gcc $(FLAGS_DEFAULT) test/plaintext_test.c -o plaintext_test;
	./plaintext_test;

json_test: test/json_test.h test/json_test.c
	gcc $(FLAGS_DEFAULT) test/json_test.c -o json_test;
	./json_test;

clean:
	-rm ./cee_templ;
	-rm ./test_tmpl;
	-rm ./test/index.h;
	-rm ./plaintext_test;
	-rm ./test/arbitrary_text.h;
	-rm ./test/json_test.h;
	-rm ./json_test;
