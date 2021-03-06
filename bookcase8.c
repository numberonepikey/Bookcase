#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "neillsimplescreen.h"

#define MAX 9
#define MAXIMUM 9999999
#define CAP 999999998
#define SCALEFACTOR 2
#define COLOURS 8
#define BLACK 'K'
#define RED 'R'
#define GREEN 'G'
#define YELLOW 'Y'
#define BLUE 'B'
#define MAGENTA 'M'
#define CYAN 'C'
#define WHITE 'W'
#define EMPTY '.'

enum bool {false, true};
typedef enum bool bool;

enum axis {y, x, height, width, row};
typedef enum axis axis;

struct bookcase {
    char board[MAX][MAX];
    int parent;
    int number;
    axis height;
    axis width;
};

typedef struct bookcase Bookcase;

/*Function to check user has inputted arguments correctly*/
void check_args(int argc, char **argv);

/* Function to read in a file*/
void readfile(Bookcase *bkcs, char **argv); 

/* Function to print out parent bookcases*/
void print_bookcases(Bookcase *cases, int solution);

/*Function to print a bookcase*/
void print_bookcase(Bookcase bkcs);

/*Function to check height and width \
are within bookcase bounds*/
bool check_parameters(Bookcase *bkcs);

/*Function to check whether a bookcase is full*/
bool full_bookcase(Bookcase *bkcs);

/*Function to check if a bookcase has more books than rows*/ 
bool viable_bookcase(Bookcase *bkcs);

/*Function to count the number books in a bookcase*/
int count_books(Bookcase *bkcs);

/* Function to change book letters in a bookcase to uppercase*/
void uppercase(Bookcase *bkcs); 

/*Function to find the number of bookcases that \
would be printed in solution*/
int find_parents(Bookcase *cases, int solution);

/* Function to generate children until \
a happy bookcase is found*/
int find_solution(Bookcase *cases);

/*Function to realloc new space if shelf runs out of room*/
Bookcase* check_space(Bookcase *cases);

/*Function to find the endshelf in array of structures*/
int find_endshelf(Bookcase *cases);

/*Function to make children bookcases*/
int make_children(Bookcase *cases, int parent);

/* Function to make book move*/
void make_move(Bookcase *child, int start, int end); 

/*Function to get parent, height and width info*/
void get_info(Bookcase *child, Bookcase *parent); 

/*Function to check if a move is valid*/
bool valid_move(Bookcase *bkcs ,int start, int end);

/* Function to assign each structure an array number*/
void fill_number(Bookcase *cases, int number);

/* Function to map parent bookcase onto child bookcase*/
void copy_bookcase(Bookcase *child, Bookcase *parent); 

/* Testing function to empty bookcase*/
void test_clean_case(Bookcase *bkcs);

/* Function to check whether calloc has freed up space*/
void *ncalloc(int n, size_t size);

/*Function to remove a book from a row*/
bool remove_book(Bookcase *bkcs, axis row, char book);

/*Function to add a book to a row*/
bool add_book(Bookcase *bkcs, axis row, char book);

/*Function to find right most book of a row*/
char find_book(Bookcase *bkcs, int row);

/* Function to test if bookcase is happy*/
bool is_happy(Bookcase *bkcs);

/*Function to check whether first \
book of each row is different*/
bool books_in_rows(Bookcase *bkcs);

/* Function to determine if a row is happy*/
bool is_row_happy(Bookcase *bkcs, axis row); 

/* Function to check whether book is valid*/
bool is_valid_book(char book);

/*Function to check whether there is space in the row*/
bool is_space(Bookcase *bkcs, axis row);

/*Function to check whether a row is empty*/
bool is_row_empty(Bookcase *bkcs, axis row);

/* Function used for testing*/
bool test_strings(Bookcase *bkcs,\
 int row, char str[10]);

void test(void);

int main(int argc, char **argv) {

    Bookcase *cases;
    int parents, happy_bkcs;
    
    check_args(argc, argv);
    test();
    
    cases = ncalloc(MAXIMUM, sizeof(Bookcase));
    fill_number(cases, MAXIMUM);
    readfile(&cases[0], argv);
    uppercase(&cases[0]);

    if (!check_parameters(&cases[0])) {
        fprintf(stderr, "Please check bookcase height and width\n");
        exit(EXIT_FAILURE);
    }

    if (!viable_bookcase(&cases[0]) ||\
     full_bookcase(&cases[0])) {
        fprintf(stderr, "No Solution?\n");
        exit(EXIT_FAILURE);
    }

    if (argc == 2) {
        happy_bkcs = find_solution(cases);
        parents = find_parents(cases, happy_bkcs);
        printf("%d\n", parents);
    }

    if (argc == 3) {
        happy_bkcs = find_solution(cases);
        parents = find_parents(cases, happy_bkcs);
        printf("%d\n", parents);
        print_bookcases(cases, happy_bkcs);
    }

    free(cases);
    return 0;
}

void check_args(int argc, char **argv) {

    if (argc == 2 || (argc == 3 && strcmp(argv[2], \
    "verbose") == 0)) {
    } 
    else {
        fprintf(stderr, "Please check you are using \
        the correct arguments\n");
        exit(EXIT_FAILURE);
    }
}

void readfile(Bookcase *bkcs, char **argv) {

    FILE *fb;
    char str[MAX] = {0};
    int height = 0, width = 0, i = 0;

    if ((fb = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Cannot open file. Have you \
        specified argument correctly?\n");
        exit(EXIT_FAILURE);
    }
    
    if (fgets(str, MAX, fb) == NULL) {
        fprintf(stderr, "Cannot open file\n");
            exit(EXIT_FAILURE);
    }
    else {
        if (sscanf(str, "%d %d", &height, &width) != 2) {
            fprintf(stderr, "Cannot scan \
            height and width\n");
            exit(EXIT_FAILURE);
        }
    }

    bkcs->height = height;
    bkcs->width = width;

    while (fgets(str, MAX, fb) != NULL) {
        strcpy(bkcs->board[i++], str);
    }

    fclose(fb);    
}

void print_bookcases(Bookcase *cases, int solution) {

    int parent = solution;

    while (parent != 0) {
        print_bookcase(cases[parent]);
        parent = cases[parent].parent;
    } 
    print_bookcase(cases[0]);
}

void print_bookcase(Bookcase bkcs){

    axis y, x;
    axis height = bkcs.height;
    axis width = bkcs.width;

    for (y = 0; y < height; y++) {
        printf("\n");
        for (x = 0; x < width; x++) {

            if (bkcs.board[y][x] == BLACK){
                neillfgcol(black);
            }
            else if (bkcs.board[y][x] == RED) {
                neillfgcol(red);
            }
            else if (bkcs.board[y][x] == GREEN) {
                neillfgcol(green);
            }
            else if (bkcs.board[y][x] == YELLOW) {
                neillfgcol(yellow);
            }
            else if (bkcs.board[y][x] == BLUE) {
                neillfgcol(blue);
            }
            else if (bkcs.board[y][x] == MAGENTA) {
                neillfgcol(magenta);
            }
            else if (bkcs.board[y][x] == CYAN) {
                neillfgcol(cyan);
            }
            else if (bkcs.board[y][x] == WHITE) {
                neillfgcol(white);
            }
            else {
            }

            printf("%c", bkcs.board[y][x]);
        }
    }
    printf("\n");
}

bool check_parameters(Bookcase *bkcs) {

    axis height = bkcs->height;
    axis width = bkcs->width;

    if (height <= 0 || height > MAX || \
    width <= 0 || width > MAX) {
        return false;
    }
    return true;
}

bool full_bookcase(Bookcase *bkcs) {

    axis height = bkcs->height;
    axis width  = bkcs->width;
    axis y, x;

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {

            if (bkcs->board[y][x] == EMPTY) {
                return false;
            }
        }
    }
    return true;
}

bool viable_bookcase(Bookcase *bkcs) {

    int height = bkcs->height;

    if (count_books(bkcs) > height) {
        return false;
    }
    return true;
}

int count_books(Bookcase *bkcs) {

    axis y, x;
    axis height = bkcs->height;
    axis width = bkcs-> width;
    int hist[COLOURS] = {0, 0, 0, 0, 0, 0, 0, 0};
    register int count = 0;

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {

            if (is_valid_book(bkcs->board[y][x])) {

                switch (bkcs->board[y][x]) {
                    case BLACK:
                        hist[0] += 1;
                        break;
                    case RED:
                        hist[1] += 1;
                        break;
                    case GREEN:
                        hist[2] += 1;
                        break;
                    case YELLOW:
                        hist[3] += 1;
                        break;
                    case BLUE:
                        hist[4] += 1;
                        break;
                    case MAGENTA:
                        hist[5] += 1;
                        break;
                    case CYAN:
                        hist[6] += 1;
                        break;
                    case WHITE:
                        hist[7] += 1;
                        break;
                    default:
                        fprintf(stderr, "Invalid \
                        character error\n");
                        exit(EXIT_FAILURE);
                }
            }
        }
    }

    for (y = 0; y < COLOURS; y++) {
        if (hist[y] > 0) {
            count += 1;
        }
    }
    return count;
}

void uppercase(Bookcase *bkcs) {

    axis y, x;

    for (y = 0; y < MAX; y++) {
        for (x = 0; x < MAX; x++) {

            bkcs->board[y][x] = toupper(bkcs->board[y][x]);
        }
    }
}

int find_parents(Bookcase *cases, int solution) {

    register int count = 1; 

    while (solution != 0) {
        solution = cases[solution].parent;
        count++;
    }
    return count;

}

int find_solution(Bookcase *cases) {  

    int endshelf = 0, parent = 0, solution = 0; 

    if (is_happy(&cases[0])) {
        return 0;
    }

    do {
        cases = check_space(cases);
        endshelf = find_endshelf(cases);

        /*No Solution Condition*/
        if (endshelf >= CAP) {
            fprintf(stderr, "No Solution?\n");
            exit(EXIT_FAILURE);
        }
        solution = make_children(cases, parent);
        parent++;

    } while (solution == 0);

    return solution;
}

int make_children(Bookcase *cases, int parent) {

    axis height = cases[parent].height;
    int endshelf = find_endshelf(cases);
    axis y, x;

    for (y = 0; y < height; y++) {
        for (x = 0; x < height; x++) {

            if (valid_move(&cases[parent], y, x)) {

                copy_bookcase(&cases[endshelf], &cases[parent]);
                get_info(&cases[endshelf], &cases[parent]);
                make_move(&cases[endshelf], y, x);

                if (is_happy(&cases[endshelf])) {
                    return endshelf;
                }
                endshelf++;
            }
        }
    }
    return 0;
}

Bookcase* check_space(Bookcase *cases) {

    /*Anticipate whether to allocate more \
    space for new child bookcases*/
    int endshelf = find_endshelf(cases);
    int height = cases[0].height;
    int new_endshelf = endshelf + (height*(height-1));
    static int capacity = MAXIMUM-1;

    if (new_endshelf >= capacity) {
        cases = (Bookcase*) realloc(cases, \
        sizeof(Bookcase)*(MAXIMUM*SCALEFACTOR));
        fill_number(cases, SCALEFACTOR*(capacity+1));
        capacity = (MAXIMUM*SCALEFACTOR) -1; 

        if (cases == NULL) {
            fprintf(stderr, "Collection overflow: failed \
            to reallocate new space\n");
            exit(EXIT_FAILURE);
        }
    }
    return cases;
}

int find_endshelf(Bookcase *cases) {

    int endshelf = cases[0].height; 
    int num = 0;

    while (endshelf != 0) {
        num++;
        endshelf = cases[num].height;
    }
    return num;
}

void make_move(Bookcase *child, int start, int end) {

    char book = find_book(child, start);

    if (add_book(child, end, book)) {
        remove_book(child, start, book);
    } 
}

void get_info(Bookcase *child, Bookcase *parent) {

    child->parent = parent->number;
    child->height = parent->height;
    child->width  = parent->width;
}

void fill_number(Bookcase *cases, int number) {

    int i;
    
    for (i = 0; i < number; i++) {
        cases[i].number = i; 
    }
}

void copy_bookcase(Bookcase *child, Bookcase *parent) {

    axis y, x;

    for (y = 0; y < MAX; y++) {
        for (x = 0; x < MAX; x++) {

            child->board[y][x] = parent->board[y][x];
        }
    }
}

void test_clean_case(Bookcase *bkcs) {

    axis y, x;

    for (y = 0; y < MAX; y++) {
        for (x = 0; x < MAX; x++) {

            bkcs->board[y][x] = EMPTY;
        }
    }
    bkcs->height = 0;
    bkcs->width  = 0;
    bkcs->parent = 0;    
}

void *ncalloc(int n, size_t size) {

    void* v = calloc(n, size);

    if (v == NULL) {
        fprintf(stderr, "Cannot calloc space\n");
    }

    return v;
}

bool valid_move(Bookcase *bkcs ,int start, int end) {

    if (start == end) {
        return false;
    }

    if (is_row_empty(bkcs, start)) {
        return false;
    }
    if (!is_space(bkcs, end)){
        return false;
    }   

    return true;
}

bool remove_book(Bookcase *bkcs, axis row, char book) {

    int i;
    int width = bkcs->width;

    for (i = width-1; i >= 0; i--) {
        if (bkcs->board[row][i] == book) {

            bkcs->board[row][i] = EMPTY;
            return true;
        }
    }
    return false;
}

bool add_book(Bookcase *bkcs, axis row, char book){

    int i;
    int width = bkcs->width;

    for (i = 0; i < width; i++) {
        if (bkcs->board[row][i] == EMPTY) {

            bkcs->board[row][i] = book;
            return true;
        }
    }
    return false;
}

char find_book(Bookcase *bkcs, int row) {

    char book = EMPTY; 
    int i;
    int width = bkcs->width;

    for (i = 0; i < width; i++) {
        if (is_valid_book(bkcs->board[row][i])) {
            book = bkcs->board[row][i];
        }
    } 
    return book;
}

bool is_happy(Bookcase *bkcs) {

    axis y = 0;
    axis height = bkcs->height;

    while (y != height) {
        if (!is_row_happy(bkcs, y)) {
            return false;
        }
        y++;
    }

    if (!books_in_rows(bkcs)) {
        return false;
    }

    return true;
}

bool books_in_rows(Bookcase *bkcs) {

    axis x, y;
    axis height = bkcs->height;

    for (y = 0; y < height; y++) {
        for (x = y+1; x < height; x++) {

            if (is_valid_book(bkcs->board[y][0]) \
            && is_valid_book(bkcs->board[x][0])) {

                if (bkcs->board[y][0] == bkcs->board[x][0]) {
                    return false;
                }
            } 
        }
    }
    return true;
}

bool is_row_happy(Bookcase *bkcs, axis row) {

    axis y, x;
    axis width = bkcs->width;

    for (y = 0; y < width; y++) {
        for (x = y+1; x < width; x++) {

            if (is_valid_book(bkcs->board[row][y]) && \
            is_valid_book(bkcs->board[row][x])) { 

                if (bkcs->board[row][y] != \
                bkcs->board[row][x]) {
                return false;
                }
            }
        }
    }
    return true;
}

bool is_valid_book(char book) {

    switch (book) {
        case BLACK:
        case RED:
        case GREEN:
        case YELLOW:
        case BLUE:
        case MAGENTA:
        case CYAN:
        case WHITE:
            return true;
        case EMPTY:
            return false;
        default :
            fprintf(stderr, "Invalid bookcase \
            definition within bookcase\n");
            exit(EXIT_FAILURE);
    }
}

bool is_space(Bookcase *bkcs, axis row) {

    axis height = bkcs->height;
    axis width = bkcs->width;

    if (row >= height || row < 0) {
        fprintf(stderr, "Row invalid, \
        please pick another\n");
        exit(EXIT_FAILURE);
    }

    if (bkcs->board[row][width-1] == EMPTY) {
            return true;
    }
    return false;
}

bool is_row_empty(Bookcase *bkcs, axis row) {

    axis x;
    axis height = bkcs->height;
    axis width = bkcs->width;

    if (row >= height || row < 0) {
        fprintf(stderr, "Row invalid, \
        please pick another\n");
        exit(EXIT_FAILURE);
    }

    for (x = 0; x < width; x++) {
        if (bkcs->board[row][x] != EMPTY) {
            return false;
        }
    }
    return true;
}

bool test_strings(Bookcase *bkcs, \
int row, char str[10]) {

    int i;
    char str2[MAX+1];

    /* Fill string with row from array*/
    for (i = 0; i < MAX; i++) {
        str2[i] = bkcs->board[row][i];
    }
    str2[MAX] = '\0';

    if (strcmp(str2, str) == 0) {
        return true;
    }    

    return false;
}

void test(void) {

    int solution, i;
    Bookcase *cases;

    cases = (Bookcase*) ncalloc(MAXIMUM, sizeof(Bookcase));

    /*Test is_row_empty funtion*/
    test_clean_case(&cases[0]);
    cases[0].height = MAX;
    cases[0].width = MAX;
    for (i = 0; i < MAX; i++) {
        assert(is_row_empty(&cases[0], i));
        assert(test_strings(&cases[0], i, "........."));
    }

    cases[0].board[5][1] = 'R';
    cases[0].board[0][1] = 'Y';
    cases[0].board[8][3] = 'R';
    assert(!is_row_empty(&cases[0], 5));
    assert(!is_row_empty(&cases[0], 0));
    assert(!is_row_empty(&cases[0], 8));
    assert(is_row_empty(&cases[0], 6));
    cases[0].board[5][1] = '.';
    cases[0].board[0][1] = '.';
    cases[0].board[8][3] = '.';
    assert(is_row_empty(&cases[0], 5));
    assert(is_row_empty(&cases[0], 0));
    assert(is_row_empty(&cases[0], 8));

    /*Test is_space function*/
    test_clean_case(&cases[0]);
    cases[0].height = MAX;
    cases[0].width = MAX;
    for (i = 0; i < MAX; i++) {
        assert(is_space(&cases[0], i));
    }

    cases[0].board[0][8] = 'R';
    cases[0].board[1][8] = 'R';
    cases[0].board[5][5] = 'R';
    cases[0].board[8][8] = '$';
    assert(!is_space(&cases[0], 0));
    assert(!is_space(&cases[0], 1));
    assert(is_space(&cases[0], 5));
    assert(!is_space(&cases[0], 8));
    cases[0].board[0][8] = '.';
    cases[0].board[1][8] = '.';
    cases[0].board[5][5] = '.';
    cases[0].board[8][8] = '.';
    assert(is_space(&cases[0], 0));
    assert(is_space(&cases[0], 1));
    assert(is_space(&cases[0], 5));
    assert(is_space(&cases[0], 8));

    /* Test is_valid_book function*/
    assert(is_valid_book('B'));
    assert(is_valid_book('K'));
    assert(is_valid_book('C'));
    assert(is_valid_book('W'));

    assert(!is_valid_book('.'));

    /*Test is_row_happy function*/
    test_clean_case(&cases[0]);
    cases[0].height = MAX;
    cases[0].width = MAX;
    for (i = 0; i < MAX; i++) {
        assert(is_row_happy(&cases[0], i));
    }

    cases[0].board[0][0] = 'R';
    cases[0].board[0][1] = 'R';
    cases[0].board[0][2] = 'R';
    assert(is_row_happy(&cases[0], 0));
    cases[0].board[0][1] = 'Y';
    assert(!is_row_happy(&cases[0], 0));
    cases[0].board[0][1] = '.';
    assert(is_row_happy(&cases[0], 0));
    cases[0].board[0][0] = '.';
    assert(is_row_happy(&cases[0], 0));
    cases[0].board[0][2] = '.';
    assert(is_row_happy(&cases[0], 0));
    cases[0].board[4][1] = 'B';
    cases[0].board[4][2] = 'R';
    assert(!is_row_happy(&cases[0], 4));

    /*Test books_in_rows function*/
    test_clean_case(&cases[0]);
    cases[0].height = MAX;
    cases[0].width = MAX;
    assert(books_in_rows(&cases[0]));
    cases[0].board[0][0] = 'Y';
    cases[0].board[1][0] = 'R';
    cases[0].board[2][0] = 'G';
    cases[0].board[3][0] = 'B';
    cases[0].board[4][0] = 'C';
    assert(books_in_rows(&cases[0]));
    cases[0].board[4][0] = 'Y';
    assert(!books_in_rows(&cases[0]));

    /*Test if is_happy function*/
    test_clean_case(&cases[0]);
    cases[0].height = MAX;
    cases[0].width = MAX;
    cases[0].board[8][8] = 'Y';
    cases[0].board[8][7] = 'Y';
    assert(is_happy(&cases[0]));
    cases[0].board[8][6] = 'B';
    assert(!is_happy(&cases[0]));

    for (i = 0; i < MAX; i++) {
        cases[0].board[3][i] = 'C';
    }

    assert(!is_happy(&cases[0]));

    cases[0].board[8][6] = '.';
    assert(is_happy(&cases[0]));

    for (i = 0; i < MAX; i++) {
        cases[0].board[3][i] = '.';
    }
    assert(is_happy(&cases[0]));

    for (i = 0; i < MAX; i++) {
        cases[0].board[8][i] = 'Y';
        cases[0].board[2][i] = 'Y';
    }
    assert(!is_happy(&cases[0]));

    /*Test find_book function*/
    test_clean_case(&cases[0]);
    cases[0].height = MAX;
    cases[0].width = MAX;
    cases[0].board[0][1] = 'Y';
    assert(find_book(&cases[0], 0) == 'Y');
    cases[0].board[0][2] = 'C';
    assert(find_book(&cases[0], 0) == 'C');
    cases[0].board[0][3] = 'Y';
    assert(find_book(&cases[0], 0) != 'C');
    assert(find_book(&cases[0], 0) == 'Y');

    for (i = 0; i < MAX; i++) {
        cases[0].board[0][i] = 'B';
    }
    assert(find_book(&cases[0], 0) == 'B');
    cases[0].board[0][8] = 'Y';
    assert(find_book(&cases[0], 0) == 'Y');
    for (i = 0; i < MAX; i++) {
        cases[0].board[0][i] = '.';
    }
    assert(find_book(&cases[0], 0) == '.');

    /*Test add book function*/
    test_clean_case(&cases[0]);
    cases[0].height = MAX;
    cases[0].width = MAX;
    add_book(&cases[0], 7, 'M');
    assert(test_strings(&cases[0], 7, "M........"));
    add_book(&cases[0], 7, 'C');
    assert(test_strings(&cases[0], 7, "MC......."));  

    /*Test whether fails to add to full book shelf*/
    for (i = 0; i< MAX; i++) {
        cases[0].board[7][i] = 'C';
    }
    add_book(&cases[0], 7, 'M');
    assert(!add_book(&cases[0], 7, 'M'));
    assert(test_strings(&cases[0], 7, "CCCCCCCCC"));

    /* Test remove_book function*/
    test_clean_case(&cases[0]);
    cases[0].height = MAX;
    cases[0].width = MAX;
    cases[0].board[4][4] = 'M';
    remove_book(&cases[0], 4, 'M');
    assert(test_strings(&cases[0], 4, "........."));

    for (i = 0; i < MAX; i++) {
        cases[0].board[3][i] = 'G';
    }
    remove_book(&cases[0], 3, 'G');
    assert(test_strings(&cases[0], 3, "GGGGGGGG."));

    cases[0].board[3][2] = 'B';
    for (i = 0; i < MAX; i++) {
        remove_book(&cases[0], 3, 'G');
    }
    assert(test_strings(&cases[0], 3, "..B......"));

    /* Test valid_move function*/
    test_clean_case(&cases[0]);
    cases[0].height = MAX;
    cases[0].width = MAX;
    assert(!valid_move(&cases[0], 0, 1));
    cases[0].board[0][7] = 'C';
    assert(valid_move(&cases[0], 0, 1));
    cases[0].board[1][7] = 'R';
    assert(valid_move(&cases[0], 0, 1));
    cases[0].board[1][8] = 'R';
    assert(!valid_move(&cases[0], 0, 1));
    assert(!valid_move(&cases[0], 7, 7));
    
    /*Test test_clean_case function*/
    test_clean_case(&cases[0]);
    assert(cases[0].board[3][5] == '.');
    assert(cases[0].board[8][8] == '.');
    assert(cases[0].board[0][0] == '.');

    /*Test copy_bookcase function*/
    copy_bookcase(&cases[1], &cases[0]);
    assert(cases[1].board[2][2] == '.');
    assert(cases[1].board[7][4] == '.');
    assert(cases[2].board[8][2] != '.');

    cases[0].board[6][0] = 'R';
    cases[0].board[6][1] = 'G';
    cases[0].board[4][7] = 'Y';
    copy_bookcase(&cases[1], &cases[0]);
    assert(cases[1].board[6][0] == 'R');
    assert(cases[1].board[6][1] == 'G');
    assert(cases[2].board[4][7] != 'Y');
    assert(cases[1].board[4][7] == 'Y');

    /* Test make_move function*/
    test_clean_case(&cases[0]);
    cases[0].height = MAX;
    cases[0].width = MAX;
    cases[0].board[0][1] = 'C';
    cases[0].board[0][2] = 'Y';
    make_move(&cases[0], 0, 1);
    assert(cases[0].board[1][0] == 'Y');
    assert(cases[0].board[0][2] == '.');
    make_move(&cases[0], 0, 1);
    assert(cases[0].board[1][1] == 'C');
    assert(cases[0].board[0][1] == '.');

    cases[0].board[5][8] = 'R';
    for (i = 0; i < MAX; i++) {
        cases[0].board[4][i] = 'Y';
    }
    make_move(&cases[0], 5, 4);
    assert(cases[0].board[5][8] == 'R');
    assert(cases[0].board[4][8] == 'Y');

    /*Test fill_number function*/
    fill_number(cases, MAXIMUM);
    assert(cases[0].number   ==   0);
    assert(cases[45].number  ==  45);
    assert(cases[432].number == 432);

    /* Test get_info*/
    cases[45].height = 6;
    cases[45].width = 6;
    get_info(&cases[34], &cases[45]);
    assert(cases[34].parent == 45);
    assert(cases[34].width  == 6);
    assert(cases[34].height == 6);

    cases[103].height = 7;
    cases[103].width  = 8;
    get_info(&cases[98], &cases[103]);
    assert(cases[98].parent == 103);
    assert(cases[98].width  == 8);
    assert(cases[98].height == 7);

    cases[967].height = 2;
    cases[967].width  = 2;
    get_info(&cases[7], &cases[967]);
    assert(cases[7].parent == 967);
    assert(cases[7].width  == 2);
    assert(cases[7].height == 2);

    /*Test find_endshelf function*/
    test_clean_case(&cases[0]);
    assert(find_endshelf(cases) == 0);
    for (i = 0; i < 100; i++) {
        cases[i].height = 4;
    }
    assert(find_endshelf(cases) == 100);
    for (i = 0; i < 478; i++) {
        cases[i].height = 4;
    }
    assert(find_endshelf(cases) == 478);
    for (i = 0; i < 923; i++) {
        cases[i].height = 4;
    }
    assert(find_endshelf(cases) == 923);

    /*Test check_space function*/
    test_clean_case(&cases[MAXIMUM -8]);
    for (i = 0; i < MAXIMUM-2; i++) {
        cases[i].height = MAX;
    }
    cases = check_space(cases);
    cases[MAXIMUM + 9].height   = 5;
    cases[MAXIMUM + 22].height  = 5;
    cases[MAXIMUM + 32].height  = 5;
    cases[MAXIMUM + 100].height = 5;
    cases[MAXIMUM + 2].board[3][2] = 'Y';
    cases[MAXIMUM + 2].board[4][7] = 'W';
    cases[MAXIMUM + 2].board[3][2] = 'C';

    /*Test make_children function*/
    for (i = 0; i < 478; i++) {
        test_clean_case(&cases[i]);
    }
    cases[0].height = 2;
    cases[0].width = 3;
    make_children(cases, 0);
    assert(find_endshelf(cases) == 1);
    assert(make_children(cases, 0) == 0);

    cases[0].board[0][0] = 'Y';
    cases[0].board[1][0] = 'R';
    assert(make_children(cases, 0) == 0);
    assert(find_endshelf(cases) == 3);
    assert(cases[1].board[1][1] = 'Y');
    assert(cases[2].board[0][1] = 'R');
    assert(cases[1].height == 2);
    assert(cases[2].width == 3);

    for (i = 0; i < 10000; i++) {
        test_clean_case(&cases[i]);
    }
    cases[0].height = 6;
    cases[0].width = 7;
    cases[0].board[0][0] = 'K';
    cases[0].board[1][0] = 'R';
    cases[0].board[2][0] = 'C';
    cases[0].board[3][0] = 'B';
    cases[0].board[4][0] = 'G';
    cases[0].board[5][0] = 'M';
    assert(make_children(cases, 0) == 0);
    assert(find_endshelf(cases) == 31);
    assert(cases[3].board[3][1]  == 'K');
    assert(cases[30].board[4][1] == 'M');
    assert(cases[24].board[2][0] == 'C');
    assert(cases[18].height == 6);
    assert(cases[22].width == 7);
    assert(cases[29].parent == 0);

    for (i = 0; i < 10000; i++) {
        test_clean_case(&cases[i]);
    }
    cases[0].height = 9;
    cases[0].width = 9; 
    cases[0].board[0][0] = 'Y';
    cases[0].board[1][0] = 'Y';
    cases[0].board[2][0] = 'Y';
    cases[0].board[3][0] = 'Y';
    cases[0].board[4][0] = 'Y';
    cases[0].board[5][0] = 'Y';
    cases[0].board[6][0] = 'Y';
    cases[0].board[7][0] = 'Y';
    cases[0].board[8][0] = 'Y';
    assert(make_children(cases, 0) == 0);
    assert(find_endshelf(cases) == 73);
    assert(cases[59].height == 9);
    assert(cases[55].width  == 9);
    assert(cases[22].parent == 0);

    for (i = 0; i < 10000; i++) {
        test_clean_case(&cases[i]);
    }
    cases[0].height = 3;
    cases[0].width = 3; 
    cases[0].board[0][0] = 'Y';
    cases[0].board[0][1] = 'G';
    cases[0].board[0][2] = 'Y';
    cases[0].board[1][0] = 'M';
    cases[0].board[2][0] = 'Y';
    assert(make_children(cases, 0) == 0);
    assert(find_endshelf(cases) == 5);
    assert(cases[3].height == 3);
    assert(cases[2].width  == 3);
    assert(cases[3].parent == 0);

    for (i = 0; i < 10000; i++) {
        test_clean_case(&cases[i]);
    }
    for (i = 0; i < 4; i++) {
        cases[i].height = 4;
    }
    cases[0].height = 3;
    cases[0].width = 3; 
    cases[0].board[0][0] = 'Y';
    cases[0].board[0][1] = 'G';
    cases[0].board[0][2] = 'Y';
    cases[0].board[1][0] = 'M';
    cases[0].board[2][0] = 'Y';
    assert(make_children(cases, 0) == 0);
    assert(find_endshelf(cases) == 8);

    for (i = 0; i < 10000; i++) {
        test_clean_case(&cases[i]);
    }
    for (i = 0; i < 7656; i++) {
        cases[i].height = 4;
    }
    cases[1087].height = 3;
    cases[1087].width = 3; 
    cases[1087].board[0][0] = 'Y';
    cases[1087].board[0][1] = 'G';
    cases[1087].board[0][2] = 'Y';
    cases[1087].board[1][0] = 'M';
    cases[1087].board[2][0] = 'Y';
    make_children(cases, 1087);
    assert(find_endshelf(cases) == 7660);
    assert(cases[7658].height == 3);
    assert(cases[7658].width  == 3);
    assert(cases[7658].parent == 1087);

    for (i = 0; i < 10000; i++) {
        test_clean_case(&cases[i]);
    }
    cases[0].height = 3;
    cases[0].width  = 3; 
    cases[0].board[0][0] = 'Y';
    cases[0].board[1][0] = 'G';
    cases[0].board[2][0] = 'Y';
    cases[0].board[2][1] = '.';
    assert(make_children(cases, 0) == 2);

    for (i = 0; i < 10000; i++) {
        test_clean_case(&cases[i]);
    }
    cases[0].height = 2;
    cases[0].width  = 2; 
    cases[0].board[0][0] = 'Y';
    cases[0].board[0][1] = '.';
    cases[0].board[1][0] = 'B';
    cases[0].board[1][1] = 'Y';
    assert(make_children(cases, 0) == 1);
    
    
    /* Test find_solutions function*/
    assert(find_solution(cases) == 2);

    for (i = 0; i < 10000; i++) {
        test_clean_case(&cases[i]);
    }
    cases[0].height = 2;
    cases[0].width  = 2; 
    cases[0].board[0][0] = 'R';
    cases[0].board[0][1] = 'R';
    cases[0].board[1][0] = 'C';
    cases[0].board[1][1] = 'C';
    assert(find_solution(cases) == 0);


    for (i = 0; i < 10000; i++) {
        test_clean_case(&cases[i]);
    }
    cases[0].height = 2;
    cases[0].width  = 2; 
    cases[0].board[0][0] = 'Y';
    cases[0].board[0][1] = '.';
    cases[0].board[1][0] = 'B';
    cases[0].board[1][1] = 'Y';
    assert(find_solution(cases) == 1);


    for (i = 0; i < 10000; i++) {
        test_clean_case(&cases[i]);
    }
    cases[0].height = 7;
    cases[0].width  = 6; 
    cases[0].board[0][0] = 'C';
    cases[0].board[1][0] = 'B';
    cases[0].board[2][0] = 'K';
    cases[0].board[3][0] = 'Y';
    cases[0].board[4][0] = 'W';
    cases[0].board[5][0] = 'W';
    assert(find_solution(cases) == 29);
    
    /*Test find_parents function*/
    for (i = 0; i < 999998; i++) {
        test_clean_case(&cases[i]);
    }
    cases[0].height = 3;
    cases[0].width  = 3; 
    cases[0].board[0][0] = 'Y';
    cases[0].board[1][0] = 'B';
    cases[0].board[1][1] = 'B';
    cases[0].board[1][2] = 'Y';
    cases[0].board[2][0] = 'Y';
    cases[0].board[2][1] = 'B';

    solution = find_solution(cases);
    assert(find_parents(cases, solution) == 4);

    for (i = 0; i < 999998; i++) {
        test_clean_case(&cases[i]);
    }
    cases[0].height = 2;
    cases[0].width  = 2; 
    cases[0].board[0][0] = 'Y';
    cases[0].board[1][0] = 'B';
    cases[0].board[1][1] = 'Y';

    solution = find_solution(cases);
    assert(find_parents(cases, solution) == 2);
    
    for (i = 0; i < 999998; i++) {
        test_clean_case(&cases[i]);
    }
    cases[0].height = 2;
    cases[0].width  = 3; 
    cases[0].board[0][0] = 'C';
    cases[0].board[0][1] = 'C';
    cases[0].board[1][0] = 'B';
    cases[0].board[1][1] = 'B';

    solution = find_solution(cases);
    assert(find_parents(cases, solution) == 1);

    /*Test uppercase function*/
    cases[0].board[0][0] = 'y';
    cases[0].board[1][0] = 'r';
    cases[0].board[2][0] = 'g';
    cases[0].board[3][0] = 'b';
    cases[0].board[4][0] = 'c';
    uppercase(&cases[0]);
    assert(cases[0].board[0][0] == 'Y');
    assert(cases[0].board[1][0] == 'R');
    assert(cases[0].board[2][0] == 'G');
    assert(cases[0].board[3][0] == 'B');
    assert(cases[0].board[4][0] != 'c');

    /*Test count_books*/
    test_clean_case(&cases[0]);
    cases[0].height = 5;
    cases[0].width  = 5; 
    cases[0].board[0][0] = 'Y';
    cases[0].board[1][0] = 'R';
    cases[0].board[2][0] = 'W';
    cases[0].board[3][0] = 'K';
    cases[0].board[4][0] = 'C';
    assert(count_books(&cases[0]) == 5);

    test_clean_case(&cases[0]);
    assert(count_books(&cases[0]) == 0);

    test_clean_case(&cases[0]);
    cases[0].height = 7;
    cases[0].width  = 6; 
    cases[0].board[0][0] = 'C';
    cases[0].board[1][0] = 'B';
    cases[0].board[2][0] = 'K';
    cases[0].board[3][0] = 'Y';
    cases[0].board[4][0] = 'M';
    cases[0].board[5][0] = 'W';
    assert(count_books(&cases[0]) == 6);

    test_clean_case(&cases[0]);
    cases[0].height = 3;
    cases[0].width  = 3; 
    cases[0].board[0][0] = 'Y';
    cases[0].board[1][0] = 'Y';
    cases[0].board[2][0] = 'Y';
    cases[0].board[2][2] = 'Y';
    assert(count_books(&cases[0]) == 1);

    /*Test viable_bookcase function*/
    test_clean_case(&cases[0]);
    cases[0].height = 3;
    cases[0].width  = 3; 
    cases[0].board[0][0] = 'Y';
    cases[0].board[0][1] = 'R';
    cases[0].board[1][0] = 'W';
    cases[0].board[1][2] = 'K';
    cases[0].board[2][0] = 'C';
    assert(viable_bookcase(&cases[0]) == false);

    test_clean_case(&cases[0]);
    cases[0].height = 7;
    cases[0].width  = 6; 
    cases[0].board[0][0] = 'C';
    cases[0].board[1][0] = 'B';
    cases[0].board[2][0] = 'K';
    cases[0].board[3][0] = 'Y';
    cases[0].board[4][0] = 'M';
    cases[0].board[5][0] = 'W';
    assert(viable_bookcase(&cases[0]) == true);

    test_clean_case(&cases[0]);
    cases[0].height = 2;
    cases[0].width  = 3; 
    cases[0].board[0][0] = 'C';
    cases[0].board[0][1] = 'C';
    cases[0].board[1][0] = 'B';
    cases[0].board[1][1] = 'B';
    assert(viable_bookcase(&cases[0]) == true);

    /*Test full_bookcase function*/
    test_clean_case(&cases[0]);
    cases[0].height = 2;
    cases[0].width  = 2; 
    cases[0].board[0][0] = 'C';
    cases[0].board[0][1] = 'C';
    cases[0].board[1][0] = 'B';
    cases[0].board[1][1] = 'B';
    assert(full_bookcase(&cases[0]));

    test_clean_case(&cases[0]);
    cases[0].height = 2;
    cases[0].width  = 2; 
    cases[0].board[0][0] = 'C';
    cases[0].board[0][1] = 'C';
    cases[0].board[1][0] = 'B';
    cases[0].board[1][1] = '.';
    assert(!full_bookcase(&cases[0]));

    test_clean_case(&cases[0]);
    cases[0].height = 3;
    cases[0].width  = 3; 
    cases[0].board[0][0] = 'C';
    cases[0].board[0][1] = 'B';
    cases[0].board[0][2] = 'K';
    cases[0].board[1][0] = 'Y';
    cases[0].board[1][1] = 'M';
    cases[0].board[1][2] = 'W';
    cases[0].board[2][0] = 'R';
    cases[0].board[2][1] = 'M';
    cases[0].board[2][2] = 'C';
    assert(full_bookcase(&cases[0]));

    /*Test check_parameters function*/

    cases[0].height = MAX;
    cases[0].width = MAX;
    assert(check_parameters(&cases[0]));
    cases[0].height = 10;
    assert(!check_parameters(&cases[0]));
    cases[0].height = 5;
    cases[0].width = 0;
    assert(!check_parameters(&cases[0]));

    free(cases);
}

