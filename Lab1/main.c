//Name: Joseph Park
//ID: 24136956
//Lab 1
//Compile with:
//gcc main.c -lm -o execname

//There is a very rudimentary user input error check
//No file format check as it is assumed the file is formatted correctly
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define NULL_RECORD "NULLRECORD"
#define MAX_SIZE 50
#define MAX_FILE_LINE_SIZE 80
#define MAX_FILE_RECORD_SIZE 100
const char* COMMANDS[6] = {"read", "write", "delete", "print", "quit", NULL};
const int ARGS[6] = {1, 1, 1, 0, 0, -1};
/**
 * @brief The Record struct - Holds name, address, and phone fields
 */
struct Record {
    char* _name;
    char* _address;
    char* _phone;
};

//~~Record Helper Functions~~
struct Record createRecord (char* name, char* address, char* phone) {
    struct Record R;
    R._name = name;
    R._address = address;
    R._phone = phone;
    return R;
}
/**
 * @brief The RecordNode struct - The Linked List node fo Record
 */
struct RecordNode {
    struct Record _data;
    struct RecordNode* _next;
};

//~~ Record Linked List Functions~~
int size (struct RecordNode* head) {
    struct RecordNode* walker = head;
    int i = 0;
    while (walker != NULL) {
        ++i;
        walker = walker->_next;
    }
    return i;
}

void insertEnd (struct RecordNode** head, struct RecordNode* node) {
    if (strcmp((*head)->_data._name,NULL_RECORD) == 0) {
        (*head)->_data = node->_data;
        (*head)->_next = node->_next;
    }
    else {
        struct RecordNode* end = *head;
        while (end->_next != NULL) {
            end = end->_next;
        }
        end->_next = node;
    }
}
int deleteAt (struct RecordNode** head, int at) {
    if (at == 1) {
        struct RecordNode* temp = *head;
        *head = (*head)->_next;
        temp->_next = NULL;
        free(temp);
        return 1;
    }
    if (at > size (*head)) {
        return -1;
    }
    struct RecordNode* prev = *head;
    int i = 1;
    while(i < at - 1) {
        prev = prev->_next;
        ++i;
    }
    struct RecordNode* del = prev->_next;
    struct RecordNode* next = del->_next;
    prev->_next = next;
    del->_next = NULL;
    free(del);
    return 1;
}

void printList (struct RecordNode* head, int numbered) {
    struct RecordNode* testWalker = head;
    if (strcmp(head->_data._name,NULL_RECORD) == 0) {
        printf("Empty\n");
    }
    else {
        int i = 1;
        while (testWalker != NULL) {
            if (numbered != 0) {
                printf("%d\t", i++);
            }
            printf("%s\t%s\t%s\n",testWalker->_data._name,testWalker->_data._address,testWalker->_data._phone);
            testWalker = testWalker->_next;
        }
    }

}

//~~Main Loop Functions~~
void copy (char* source, char* destination) {
    int i = 0;
    while (source[i] != '\0') {
        destination[i] = source[i];
        ++i;
    }
    destination[i] = '\0';
}

int openFile (char* fileName, FILE** fp, char* mode) {
    *fp = fopen(fileName,mode);
    if (!(*fp)) {
        return 0;
    }
    return 1;
}
//Assumes token is allocated and has sufficient space
int grabToken (FILE* fp, char* token) {
    char c = fgetc(fp);
    if (c == EOF || c == '\n') {
        return -1;
    }
    ungetc(c,fp);
    c = fgetc(fp);
    int i = 0;
    while (c != '\t' && c != '\n') {
        token[i++] = c;
        c = fgetc(fp);
    }
    token[i] = '\0';
    if (c == '\n') {
        return 0;
    }
    return 1;
}

void writeToFile (FILE* fp, struct RecordNode* head) {
    int charLineCount = 0, recordFileCount = 0;
    struct RecordNode* walker = head;
    while (walker != NULL && recordFileCount < MAX_FILE_RECORD_SIZE) {
        if ((strlen(walker->_data._name) + strlen(walker->_data._address) + strlen(walker->_data._phone) + 2) <= MAX_FILE_LINE_SIZE) {
            fprintf(fp,"%s\t%s\t%s\n",walker->_data._name,walker->_data._address,walker->_data._phone);
        }
        else {
            charLineCount = 0;
            int i = 0;
            char check = walker->_data._name[i];
            while (check != '\0') {
                if (charLineCount < MAX_FILE_LINE_SIZE - 1) {
                    fprintf(fp,"%c",walker->_data._name[i]);
                    check = walker->_data._name[++i];
                    ++charLineCount;
                }
                else {
                    check = '\0';
                }
            }
            if (charLineCount < MAX_FILE_LINE_SIZE - 1) {
                fprintf(fp,"\t");
                ++charLineCount;

                i = 0;
                check = walker->_data._address[i];
                while (check != '\0') {
                    if (charLineCount < MAX_FILE_LINE_SIZE - 1) {
                        fprintf(fp,"%c",walker->_data._address[i]);
                        check = walker->_data._address[++i];
                        ++charLineCount;
                    }
                    else {
                        check = '\0';
                    }
                }
                if (charLineCount < MAX_FILE_LINE_SIZE - 1) {
                    fprintf(fp,"\t");
                    ++charLineCount;

                    fprintf(fp,"\t");
                    i = 0;
                    check = walker->_data._phone[i];
                    while (check != '\0') {
                        if (charLineCount < MAX_FILE_LINE_SIZE - 1) {
                            fprintf(fp,"%c",walker->_data._phone[i]);
                            check = walker->_data._phone[++i];
                            ++charLineCount;
                        }
                        else {
                            check = '\0';
                        }
                    }
                }
            }
            fprintf(fp,"\n");
        }
        ++recordFileCount;
        walker = walker->_next;
    }
}

void print (struct RecordNode* head) {
    printList (head,1);
}



//~~User input functions~~
int verifyLoop (char* test) {
    char se;
    unsigned int i;
    for (i = 0; test[i] != '\0'; ++i) {
        se = getchar();
        if (se != test[i]) {
            return 0;
        }
    }
    return 1;
}

int verify(char ini) {
    if (ini == 'r') {
        return verifyLoop("ead \0");
    }
    else if (ini == 'w') {
        return verifyLoop("rite \0");
    }
    else if (ini == 'p') {
        return verifyLoop("rint\0");
    }
    else if (ini == 'd') {
        return verifyLoop("elete \0");
    }
    else if (ini == 'q') {
        return verifyLoop("uit\0");
    }
    else {
        return 0;
    }
}

int getArg (char* arg) {
    char c = getchar();
    if (c == '\n') {
        return 0;
    }
    int i = 0;
    while (c != '\n' && i < MAX_SIZE) {
        if (c != ' ' && c != '\t') {
            arg[i++] = c;
        }
        c = getchar();
    }
    arg[i] = '\0';
    return 1;
}

int getNum(char* arg) {
    int i = 0;
    int size = 0;
    int num = 0;
    while (arg[size++] != '\0') {
    };
    --size;
    if (size == 0) {
        return -1;
    }
    --size;
    while (arg[i] != '\0') {
        num += pow(10,size--) * (arg[i++] - '0');
    }
    return num;
}

int main (int argc, char* argv[]) {
    struct RecordNode init;
    init._data._name = NULL_RECORD;
    struct RecordNode* head = &init;
//    struct RecordNode* head = NULL;

    int cont = 1;
    char ini;
    char arg[MAX_SIZE];
    while (cont) {
		//printf(":");
        ini = getchar();
        if (verify(ini)) {

            switch (ini) {
                case 'r': {
                    getArg(arg);
                    FILE* fp = NULL;
                    char name[MAX_SIZE], address[MAX_SIZE], phone[MAX_SIZE];

                    int res = openFile (arg,&fp,"r");
                    if (res == 1) {
                        if (strcmp (head->_data._name,NULL_RECORD) != 0) {
                            int si = size (head);
                            int i = si;
                            while (i >= 1) {
                                deleteAt (&head,i);
                                --i;
                            }
                            head = (struct RecordNode*) malloc(sizeof(struct RecordNode));
                            head->_data._name = NULL_RECORD;
                            head->_next = NULL;
                        }

                        int c = grabToken(fp,name);
                        while (c == 1) {
                            c = grabToken (fp,address);;
                            c = grabToken (fp,phone);

                            struct Record R;
                            R._name = (char*) malloc(MAX_SIZE);
                            R._address = (char*) malloc(MAX_SIZE);
                            R._phone = (char*) malloc(MAX_SIZE);

                            copy (name,R._name);
                            copy (address, R._address);
                            copy (phone, R._phone);

                            struct RecordNode* RN = (struct RecordNode*) malloc (sizeof(struct RecordNode));
                            RN->_data = R;
                            RN->_next = NULL;
                            insertEnd (&head,RN);
                            if (c == 0) {
                               c = grabToken(fp,name);
                            }
                        }
                    }
					else {
							//printf("Bad file\n");
					}
					if(fp != NULL){
						fclose(fp);
					}
                    break;
                }
                case 'w': {
                    getArg(arg);
                    FILE* fp = NULL;
                    int res = openFile (arg,&fp,"w");
                    if (res == 1) {
                        writeToFile(fp,head);
                    }
					else {
						//printf("Bad file\n");
					}
					if (fp != NULL) {
						fclose(fp);
					}
                    break;
                }
                case 'p': {
                    printList(head,1);
                    break;
                }
                case 'd': {
                    getArg(arg);
                    int num = getNum(arg);
                    if (num != -1) {
                        if (strcmp(head->_data._name, NULL_RECORD) != 0) {
							deleteAt(&head,num);
						}
						else {
							//printf("No records\n");
						}
                    }
					else {
						//printf("Bad argument\n");
					}
                    break;
                }
                case 'q': {
                    cont = 0;
                    break;
                }
            }
            //printf("Done with command\n");
        }
        else {
			//printf("Bad Command\n");
        }
    }
    return 0;
}







/*
 void getInput (char* input) {
    char c = getchar();
    int i = 0;
    while (c != '\n' && i < MAX_SIZE - 1){
        input[i] = c;
        ++i;
        c = getchar();
    }
    input[i] = '\0';
}

int findIn (char* str, char f, int from) {
    int i = from;
    while (str[i] != '\0') {
        if (str[i] == f) {
            return i;
        }
        ++i;
    }
    return -1;
}

char* substr (char* str, int b, int e) {
    char sub[b - e + 1];
    int i = b;
    while (i <= e) {
        sub[i - b] = str[i];
        ++i;
    }
    sub[i - b] = '\0';
    printf("%s\n",sub);
    return sub;
}

void tokenizeInput (char* input, char** args) { //Not saving val
    int i = 0;
    printf("input=%s\n",input);
    while (input[i] != '\0') {
        printf("i=%d\n",i);
        ++i;
    }
    int size = i;
    printf("size=%d\n",size);
    i = 0;
    int j = 0;
    while (i < size && j < MAX_SIZE - 1) {
        int b = i;
        int e = findIn (input,' ',b) - 1;
        printf("b=%d, e=%d\n",b,e);
        if (e == -2) {
            e = size - 1;
        }printf("b=%d, e=%d\n",b,e);
        int z = b;

        while (z <= e) {
            args[j][z - b] = input[z];
            printf("z=%d, %c\n",z,args[j][z-b]);
            ++z;
        }
        args[j][z - b] = '\0';
//        args[j] = substr (input,b,e);

        ++j;
        i = e + 1;
    }
    printf("token=%s\n",args[0]);
    args[j] = NULL;
}

int checkInput (char* input, char* com) {
    char arg[MAX_SIZE][MAX_SIZE];
    tokenizeInput (input,arg);
    printf("Done tokenizing=%s\n",arg[0]);
    char* command = lowerAll(arg[0]);
    printf("Done lowering=%s\n",command);
    int val = isValid(command,COMMANDS);
    printf("Done validating\n");
    if (val != -1) {
        com = COMMANDS[val];
        int i = 1;
        while (arg[i] != NULL) {
            ++i;
        }
        if (i == ARGS[val]) {
            if (matchStr(arg[i],COMMANDS[2]) == 1 && isdigit(arg[1])) {

                return 1;
            }
            else {
                return 0;
            }
            return 1;
        }
        else {
            return -1;
        }
    }
    else {
        return -2;
    }
}

void lowerAll (char* str) {
    int i = 0;
    while (str[i] != '\0') {
        str[i] = tolower(str[i]);
        ++i;
    }
}

int isValid (char* arg, char** valid) {
    int i = 0;
    while (valid[i] != NULL) {
        printf("i=%d\n",i);
        if (matchStr(arg,valid[i]) == 1) {
            return i;
        }
        printf("matchstr\n");
        ++i;
    }
    return -1;
}

int matchStr (char* str1, char* str2) {
    int i = 0;
    printf("1=%s,2=%s\n",str1,str2);
    while (str1[i] != '\0' && str1[i] != '\0') {
        if (str1[i] != str2[i]) {
            return -1;
        }
        ++i;
    }
    if (str1[i] == '\0' && str2[i] != '\0') {
        return -1;
    }
    else if(str1[i] != '\0' && str2[i] == '\0') {
        return -1;
    }
    else {
        return 1;
    }
}
*/
