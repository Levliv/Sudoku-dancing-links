project(DLXSolver)

set(HEADER_FILES
        SudokuDLXSolver.h
        Node.h
)

set(SOURCE_FILES
        SudokuDLXSolver.cpp
)

add_library(DLXSolver_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
