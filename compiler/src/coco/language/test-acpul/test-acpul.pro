TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

#QMAKE_POST_LINK=test-acpul/valgrind.sh

SOURCES += main.cpp \
    ../Scanner.cpp \
    ../Parser.cpp \
    ../../../core/String.cpp \
    ../../../core/Processor.cpp \
    ../../../core/Object.cpp \
    ../../../core/Block.cpp \
    ../../../core/Name.cpp \
    ../../../core/ObjectsMap.cpp \
    ../../../core/Expression.cpp \
    ../../../core/Compiler.cpp \
    ../../../core/CodeInstruction.cpp \
    ../../../core/CodeBlock.cpp \
    VVCalculatorCPUTrace.cpp \
    ../../../core/Executor.cpp \
    ../../../core/ExecutionBlock.cpp \
    ../../../core/Core.cpp \
    ../../../core/CodeBuilder.cpp \
    ../../../../cocos2d-acpu/cocos2d-acpu/VVCalculatorCPUMemory.cpp \
    ../../../core/Interpreter.cpp \
    ../../../core/CoreTest.cpp \
    ../../../core/BuildFile.cpp \
    ../../../core/Dumper.cpp \
    ../../../core/CoreData.cpp \
    ../../../core/AScanner.cpp \
    ../../../core/AParser.cpp \
    ../../../core/AErrors.cpp \
    ../../../core/Error.cpp \
    ../../../core/FormulaInstance.cpp \
    ../../../core/CodeBlock_Compile.cpp \
    ../../../core/CodeBlock_Build.cpp \
    ../../../core/Utils.cpp \
    ../../../core/DumpHelper.cpp \
    ../../../core/DumpHelper_Exp1.cpp \
    ../../../core/DumpHelper_Exp0.cpp \
    ../../../core/DumpHelper_dumpFull.cpp

OTHER_FILES += \
    ../test.acpul \
    ../acpul.atg \
    .gdbinit

HEADERS += \
    ../Scanner.h \
    ../Scanner.frame \
    ../Parser.h \
    ../Parser.frame \
    ../../../../tree_hh/src/tree.hh \
    ../../../core/String.h \
    ../../../core/Processor.h \
    ../../../core/Object.h \
    ../../../core/Block.h \
    ../../../core/Name.h \
    ../../../core/ObjectsMap.h \
    ../../../core/Expression.h \
    ../../../core/Compiler.h \
    ../../../core/CodeInstruction.h \
    ../../../core/CodeBlock.h \
    ../../../core/Executor.h \
    ../../../core/ExecutionBlock.h \
    ../../../core/Core.h \
    ../../../core/CodeBuilder.h \
    ../../../../cocos2d-acpu/cocos2d-acpu/VVCalculatorCPUMemory.h \
    ../../../core/Interpreter.h \
    ../../../core/CoreTest.h \
    ../../../core/BuildFile.h \
    ../../../core/Dumper.h \
    ../../../core/CoreData.h \
    ../../../core/AScanner.h \
    ../../../core/AParser.h \
    ../../../core/AErrors.h \
    ../../../core/Error.h \
    ../../../core/FormulaInstance.h \
    ../../../core/Utils.h \
    ../../../core/DumpHelper.h

INCLUDEPATH += ../../../../tree_hh/src/ \
    ../../../../cocos2d-acpu/cocos2d-acpu/ \
    ../../../
