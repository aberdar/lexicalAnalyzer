#include <stdio.h>
#include <string.h>

#define BUFSIZE 80

FILE* file;
char c; // символ

char buf[BUFSIZE]; // буфер для накопления символов лексемы
int bufSize = 0; // размер буфера

int d = 0; //переменная для формирования числового значения константы
int j = 0; // номер строки в таблице, где находится лексема, найденная функцией

char TW[9][BUFSIZE] = {"", "begin", "while", "and", "or", "not", "do", "loop", "end"}; // таблица служебных слов М-языка
char TD[15][BUFSIZE] = {"",":=","!=","=", ":", ";", "<", ">", "*", "+", "-", "/", "(", ")","⟂" }; // таблица ограничителей М-языка
char TID[BUFSIZE][BUFSIZE]; // таблица идентификаторов анализируемой программы
int TNUM[BUFSIZE]; // таблица чисел-констант, используемых в программе
int tnumSize = 0; // размер таблицы чисел-констант
int readLine = 1; // определяет, на какой строке программы мы находимся

void gc() { // считывание из файла
    c = fgetc(file);
}

void add() // добавление символа с в конец буфера buf
{
    buf[bufSize] = c;
    bufSize++;
}

int putl(char T[][BUFSIZE]) // запись в таблицу Т лексемы из буфера buf, если ее там не было
{
    T[0][0]++;
    int size = T[0][0];
    for (int i = 0; i < BUFSIZE; i++) {
        T[size][i] = buf[i];
    }
    return size;
}

int look(char T[][BUFSIZE]) // поиск в таблице Т лексемы из буфера buf
{
    int size = T[0][0];
    for (int i = 1; i <= size; i++) {
        if (strcmp(buf, T[i]) == 0) { // сравнение строк
            return i;
        }
    }
    return 0;
}

void clear()   // очистка буфера buf
{
    for (int i = 0; i < bufSize; i++) {
        buf[i] = '\0';
    }
    bufSize = 0;
}

int putNum() // запись в TNUM константы из d, если ее там не было
{
    for (int v = 0; v <= tnumSize; v++) {
        if (d == TNUM[v] && d != 0) {
            return  v+1;
        }
    }

    TNUM[tnumSize] = d;
    d = 0;
    tnumSize++;
    return  tnumSize;
}

void makeLex(int k, int i) // формирование и вывод внутреннего представления лексемы
{
    if (k == 1) {
        printf("<%d,%d> ", k, i);
    }
    if (k == 2) {
        printf("<%d,%d> ", k, i);
    }
    if (k == 3) {
        printf("<%d,%d> ", k, i);
    }
    if (k == 4) {
        printf("<%d,%d> ", k, i);
    }
}

void printTWAndTD() {
    printf("\n\n1.TW: \n");
    for (int i = 1; i < 9; i++) {
        printf("%d. ", i);
        puts(TW[i]); // переход на новую строку после вывода
    }

    printf("\n2.TD: \n");
    for (int i = 1; i < 15; i++) {
        printf("%d. ", i);
        puts(TD[i]);
    }
}

void printTIDAndTNUM() {
    printf("\n3.TID: \n");
    for (int i = 1; i < TID[0][0] + 1; i++) {
        printf("%d. ", i);
        puts(TID[i]);
    }

    printf("\n4.TNUM: \n");
    for (int i = 1; i < tnumSize + 1; i++) {
        printf("%d. %d\n", i, TNUM[i - 1]);
    }
}

void printError(int errorType) {
    if (errorType == 0) {
        printf("The number is too large. (line %d)", readLine);
    }
    if (errorType == 1) {
        printf("Invalid assignment sign. (line %d)", readLine);
    }
    if (errorType == 2) {
        printf("Unexpected end files. (line %d)", readLine);
    }
    printTWAndTD();
}

int main(int argc, char* argv[])
{
    enum state {H, ID, NUM, COM, ASS, DLM, ER, FIN};
    enum state TC; // текущее состояние
    char copy;
    TC = H;

    TW[0][0] = 8;
    TD[0][0] = 14;

    file = fopen("prog.txt", "r");
    gc();

    printf("Output:\n");
    do {
        switch (TC) {
            case H:
                if (c == ' ') {
                    gc();
                } else if (c == '\n') {
                    printf("\n");
                    readLine++;
                    gc();
                } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c<= 'z')) {
                    clear();
                    add();
                    gc();
                    TC = ID;
                } else if (c >= '0' && c <= '9') {
                    d = c - '0';
                    gc();
                    TC = NUM;
                } else if ((c == ':') || (c == '!')) {
                    copy = c;
                    gc();
                    TC = ASS;
                } else if (c == '{') {
                    gc();
                    TC = COM;
                } else if (feof(file)) {
                    makeLex(2, 14);
                    printf("\nLexical analysis was successful.");
                    TC = FIN;
                } else {
                    TC = DLM;
                }
                break;

            case ID:
                if (((c >= 'A' && c <= 'Z') || (c >= 'a' && c<= 'z')) || (c >= '0' && c <= '9')) {
                    add();
                    gc();
                } else {
                    if (j = look(TW)) {
                        makeLex(1, j);
                    } else {
                        if (j = look(TID)) {
                            makeLex(3, j);
                        } else {
                            j = putl(TID);
                            makeLex(3, j);
                        }
                    }
                    TC = H;
                }
                break;

            case NUM:
                if (d > 1000) {
                    printError(0);
                    TC = ER;
                    break;
                }
                if (c >= '0' && c <= '9') {
                    d = d * 10 + (c - '0');
                    gc();
                } else {
                    makeLex(4, putNum());
                    TC = H;
                }
                break;

            case ASS:
                if (c == '=' && (copy == '!' || copy == ':')) {
                    if (copy == '!') {
                        makeLex(2, 2);
                    }
                    if (copy == ':') {
                        makeLex(2, 1);
                    }
                    TC = H;
                    gc();
                } else {
                    printError(1);
                    TC = ER;
                }
                break;

            case COM:
                if (c == EOF) {
                    printError(2);
                    TC = ER;
                }
                if (c != '}') {
                    gc();
                } else {
                    gc();
                    TC = H;
                }
                break;

            case FIN:
                makeLex(2, 14);
                break;

            case DLM:
                clear();
                add();
                if (j = look(TD)) {
                    makeLex(2, j);
                    copy = c;
                    gc();
                    if ((copy=='=')||(copy==';')||(copy=='*')||(copy=='+')||(copy=='-')||(copy=='/')||(copy=='(')
                    ||(copy=='<')||(copy=='>')) {
                        if (c == '=') {
                            printError(1);
                            TC = ER;
                            break;
                        }
                    }
                    TC = H;
                } else {
                    if(!feof(file)) {
                        printError(2);
                    }
                    TC = ER;
                }
                break;

            case ER:
                break;
        }
    } while (TC != FIN && TC != ER);

    if (TC == ER) {
        return 0;
    }

    printTWAndTD();
    printTIDAndTNUM();
    return 0;
}