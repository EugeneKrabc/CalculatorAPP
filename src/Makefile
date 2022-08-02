FLAGS = g++ -g -std=c++17 -Wall -Wextra -Werror -lgtest

all: install 
	open ./Calculator

test: tests.cpp CalcModel.cpp CalcModel.h CalcControler.cpp CalcControler.h
	$(FLAGS) CalcModel.cpp CalcControler.cpp tests.cpp
	./a.out

check:
	$(FLAGS) CalcModel.cpp CalcControler.cpp tests.cpp
	CK_FORK=no leaks --atExit -- ./a.out


gcov_report: gcov_report_build
	rm -rf *.gcda
	rm -rf *.gcno
	rm -rf *.info

gcov_report_build: CalcModel.cpp CalcControler.cpp tests.cpp CalcModel.h CalcControler.h
	$(FLAGS) --coverage CalcModel.cpp CalcControler.cpp tests.cpp
	./a.out
	lcov -t "./a.out" -o gcov_report.info -c -d .
	genhtml -o report gcov_report.info
	open ./report/index.html

install: uninstall
	mkdir build
	cd build && qmake ../CalcView && make
	cp build/Calculator.app/Contents/MacOS/Calculator ./

uninstall:
	rm -rf build

clean:
	rm -rf *.o *.g* *.info *.gcov_report report *.a *.dSYM a.out CPPLINT.cfg build info.dvi Calculator.tgz Calculator

rebuild: clean uninstall all

reinstall: clean uninstall install

dvi:
	touch info.dvi
	@(echo This is engeeniring calculator. You can input any expression with parantheses, functions and so on, \
	no longer than 255 symbols. On pressing graph button graphic is displayed.\
	You can set up graphic range by changing Xmax Xmin and Ymax Ymin > info.dvi)

dist:
	tar cfz Calculator.tgz build

