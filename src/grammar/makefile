LEX = flex
YACC = bison
CXX = g++

all:
	$(LEX) -o lex.yy.cpp lexer.l
	$(YACC) grammar.y -d -t --report=state --graph -o grammar.tab.cpp
	$(CXX) grammar.tab.cpp lex.yy.cpp ../parser-lalr-main.cpp -o grammar

clean:
	rm -f *.yy.c *.cpp *.hpp grammar
