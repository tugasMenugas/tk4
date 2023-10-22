@echo off

REM Have to run many files in order program run correctly

REM Compile program from TK4 Folder
gcc main.c functions/book.c functions/transaction.c -o main

REM Run program
main.exe
