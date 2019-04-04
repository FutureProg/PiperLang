LEX = flex
YACC = bison
CXX = g++
FLAGS = `llvm-config --cxxflags --ldflags --system-libs --libs all` -std=c++14
DEBUG = -D__PIPER_DEBUG__

all:
	$(LEX) -o lex.yy.cpp lexer.l
	$(YACC) grammar.y -d -t --report=state --graph -o grammar.tab.cpp
	$(CXX) grammar.tab.cpp $(FLAGS) $(DEBUG) lex.yy.cpp ../ast/genvisitor.cpp ../parser-lalr-main.cpp -o grammar

clean:
	rm -f *.yy.c *.cpp *.hpp grammar
