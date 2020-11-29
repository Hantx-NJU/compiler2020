# GNU make手册：http://www.gnu.org/software/make/manual/make.html
# ************ 遇到不明白的地方请google以及阅读手册 *************

# 编译器设定和编译选项
CC = gcc
FLEX = flex
BISON = bison
CFLAGS = -std=c99

# 编译目标：src目录下的所有.c文件
CFILES = $(shell find ./ -name "*.c")
OBJS = $(CFILES:.c=.o)
LFILE = $(shell find ./ -name "*.l")
YFILE = $(shell find ./ -name "*.y")
LFC = $(shell find ./ -name "*.l" | sed s/[^/]*\\.l/lex.yy.c/)
YFC = $(shell find ./ -name "*.y" | sed s/[^/]*\\.y/syntax.tab.c/)
LFO = $(LFC:.c=.o)
YFO = $(YFC:.c=.o)

parser: syntax $(filter-out $(LFO),$(OBJS))
	$(CC) -o parser $(filter-out $(LFO),$(OBJS)) -lfl -ly

syntax: lexical syntax-c
	$(CC) -c $(YFC) -o $(YFO)

lexical: $(LFILE)
	$(FLEX) -o $(LFC) $(LFILE)

syntax-c: $(YFILE)
	$(BISON) -o $(YFC) -d -v $(YFILE)

-include $(patsubst %.o, %.d, $(OBJS))

# 定义的一些伪目标
.PHONY: clean 
test:
	./parser ../Test/A-1.cmm
	./parser ../Test/A-2.cmm
	./parser ../Test/A-3.cmm
	./parser ../Test/A-4.cmm
	./parser ../Test/A-5.cmm
	./parser ../Test/A-6.cmm
	./parser ../Test/A-7.cmm
	./parser ../Test/A-8.cmm
	./parser ../Test/A-9.cmm
	./parser ../Test/A-10.cmm
	./parser ../Test/A-11.cmm
	./parser ../Test/A-12.cmm
	./parser ../Test/A-13.cmm
	./parser ../Test/A-14.cmm
	./parser ../Test/A-15.cmm
	./parser ../Test/A-16.cmm
	./parser ../Test/A-17.cmm
	./parser ../Test/A-18.cmm
	./parser ../Test/A-19.cmm
	./parser ../Test/A-20.cmm
	./parser ../Test/A-20.cmm
	./parser ../Test/B-1.cmm
	./parser ../Test/B-2.cmm
	./parser ../Test/C-1.cmm
	./parser ../Test/C-2.cmm
	./parser ../Test/D-1.cmm
	./parser ../Test/D-2.cmm
	./parser ../Test/D-3.cmm
	#./parser ../Test/E2_1.cmm
	#./parser ../Test/E2_2.cmm
	./parser ../Test/E2_3.cmm

clean:
	rm -f parser lex.yy.c syntax.tab.c syntax.tab.h syntax.output
	rm -f $(OBJS) $(OBJS:.o=.d)
	rm -f $(LFC) $(YFC) $(YFC:.c=.h)
	rm -f *~
