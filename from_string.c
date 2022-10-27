#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>

#define num_keywords 63

char keywords_arr[num_keywords][100]={
"auto",         "double",      "int",      "struct",
"break",        "else",        "long",       "switch",
"case",         "enum",        "register",   "typedef",
"char",         "extern",      "return",     "union",
"const",        "float",       "short",      "unsigned",
"continue",     "for",         "signed",     "void",
"default",      "goto",        "sizeof",     "volatile",
"do",           "if",          "static",     "while",
"asm",          "bool",        "catch",          "class",
"const_cast",   "delete",      "dynamic_cast",   "explicit", 
"export",       "false",       "friend",         "inline", 
"mutable",      "namespace",   "new",            "operator", 
"private",      "protected",   "public",         "reinterpret_cast",
"static_cast",  "template",    "this",           "throw",
"true",         "try",         "typeid",         "typename", 
"using",        "virtual",     "wchar_t" 
};

char token_arr[num_keywords][100]={
"AUTO",         "DOUBLE",      "INT",      "STRUCT",
"BREAK",        "ELSE",        "LONG",       "SWITCH",
"CASE",         "ENUM",        "REGISTER",   "TYPEDEF",
"CHAR",         "EXTERN",      "RETURN",     "UNION",
"CONST",        "FLOAT",       "SHORT",      "UNSIGNED",
"CONTINUE",     "FOR",         "SIGNED",     "VOID",
"DEFAULT",      "GOTO",        "SIZEOF",     "VOLATILE",
"DO",           "IF",          "STATIC",     "WHILE",
"ASM",          "BOOL",        "CATCH",          "CLASS",
"CONST_CAST",   "DELETE",      "DYNAMIC_CAST",   "EXPLICIT", 
"EXPORT",       "FALSE",       "FRIEND",         "INLINE", 
"MUTABLE",      "NAMESPACE",   "NEW",            "OPERATOR", 
"PRIVATE",      "PROTECTED",   "PUBLIC",         "REINTERPRET_CAST",
"STATIC_CAST",  "TEMPLATE",    "THIS",           "THROW",
"TRUE",         "TRY",         "TYPEID",         "TYPENAME", 
"USING",        "VIRTUAL",     "WCHAR_T"
};

///Contains the content of the file
char file_content[1000000];
int curr=0;

int line_number=1;

struct pair{
    char lexeme[1000];
    char token[100];
};

struct table_entry{
    char lexeme[1000];
    char token[100];
    struct table_entry* next;
};

struct table_entry* root;
struct table_entry* last_node;

struct table_entry* head_ident;

struct table_entry* newNode(char* lexeme,char* token){
    struct table_entry* node=(struct table_entry*)malloc(sizeof(struct table_entry));
    strcpy(node->token,token);
    strcpy(node->lexeme,lexeme);
    node->next=NULL;
}

/**This function generates the symbol table with keywords, delimiters and operators
*
*Each entry contains lexeme and token pair

Called by main() function (only once)
*/
void generate_initial_table(){
    root=newNode(keywords_arr[0],token_arr[0]);
    last_node=root;
    for(int i=1;i<63;i++){
        struct table_entry* node=newNode(keywords_arr[i],token_arr[i]);
        last_node->next=node;
        last_node=node;
    }
    for(int i=2;i<=28;i++){
        struct table_entry* node=newNode("","");
        switch (i){
            case 2:
                strcpy(node->lexeme,"+");
                strcpy(node->token,"PLUS_OPERATOR");
                break;
            case 3:
                strcpy(node->lexeme,"+-");
                strcpy(node->token,"ADDITION_ASSIGNMENT_OPERATOR");
                break;
            case 4:
                strcpy(node->lexeme,"-");
                strcpy(node->token,"MINUS_OPERATOR");
                break;
            case 5:
                strcpy(node->lexeme,"-=");
                strcpy(node->token,"SUBTRACTION_ASSIGNMENT_OPERATOR");
                break;
            case 6:
                strcpy(node->lexeme,"*");
                strcpy(node->token,"STAR_OPERATOR");
                break;
            case 7:
                strcpy(node->lexeme,"*=");
                strcpy(node->token,"MULTIPLICATION_ASSIGNMENT_OPERATOR");
                break;
            case 8:
                strcpy(node->lexeme,"/");
                strcpy(node->token,"DIVISION_OPERATOR");
                break;
            case 9:
                strcpy(node->lexeme,"/=");
                strcpy(node->token,"DIVISION_ASSIGNMENT_OPERATOR");
                break;
            case 10:
                strcpy(node->lexeme,"%");
                strcpy(node->token,"MODULOUS_OPERATOR");
                break;
            case 11:
                strcpy(node->lexeme,"=");
                strcpy(node->token,"ASSIGNMENT_OPERATOR");
                break;
            case 12:
                strcpy(node->lexeme,"==");
                strcpy(node->token,"COMPARISION_OPERATOR");
                break;
            case 13:
                strcpy(node->lexeme,">");
                strcpy(node->token,"GREATER_THAN_OPERATOR");
                break;
            case 14:
                strcpy(node->lexeme,">=");
                strcpy(node->token,"GREATER_THAN_OR_EQUAL_OPERATOR");
                break;
            case 15:
                strcpy(node->lexeme,"<");
                strcpy(node->token,"LESSER_THAN_OPERATOR");
                break;
            case 16:
                strcpy(node->lexeme,"<=");
                strcpy(node->token,"LESSER_THAN_OR_EQUAL_OPERATOR");
                break;
            case 17:
                strcpy(node->lexeme,"!");
                strcpy(node->token,"NOT_OPERATOR");
                break;
            case 18:
                strcpy(node->lexeme,"!=");
                strcpy(node->token,"NOT_EQUAL_OPERATOR");
                break;
            case 19:
                strcpy(node->lexeme,"++");
                strcpy(node->token,"INCREMENT_OPERATOR");
                break;
            case 20:
                strcpy(node->lexeme,"--");
                strcpy(node->token,"DECREMENT_OPERATOR");
                break;
            case 22:
                strcpy(node->lexeme,"&&");
                strcpy(node->token,"AND_OPERATOR");
                break;
            case 24:
                strcpy(node->lexeme,"||");
                strcpy(node->token,"OR_OPERATOR");
                break;
            case 25:
                strcpy(node->lexeme,"%=");
                strcpy(node->token,"MODULOUS_ASSIGNMENT_OPERATOR");
                break;
            case 26:
                strcpy(node->lexeme,"?");
                strcpy(node->token,"CONDITIONAL_QUESTION_MARK_OPERATOR");
                break;
            case 27:
                strcpy(node->lexeme,":");
                strcpy(node->token,"COLON_OPERATOR");
                break;
            case 28:
                strcpy(node->lexeme,":=");
                strcpy(node->token,"COLON_ASSIGNMENT_OPERATOR");
                break;
            default:
                strcpy(node->token,"ERROR");
                strcpy(node->lexeme,"");
                break;
        }
        if(strlen(node->lexeme)>0){
            last_node->next=node;
            last_node=node;
        }
    }
    for(int i=0;i<8;i++){
        struct table_entry* node=newNode("","");
        switch (i)
        {
        case 0:
            strcpy(node->lexeme,",");
            strcpy(node->token,"COMMA_DELIM");
            break;
        case 1:
            strcpy(node->lexeme,";");
            strcpy(node->token,"SEMICOLON_DELIM");
            break;
        case 2:
            strcpy(node->lexeme,"(");
            strcpy(node->token,"OPEN_BRACKET");
            break;
        case 3:
            strcpy(node->lexeme,")");
            strcpy(node->token,"CLOSE_BRACKET");
            break;
        case 4:
            strcpy(node->lexeme,"{");
            strcpy(node->token,"OPEN_FLOWER_BRACKET");
            break;
        case 5:
            strcpy(node->lexeme,"}");
            strcpy(node->token,"CLOSE_FLOWER_BRACKET");
            break;
        case 6:
            strcpy(node->lexeme,"[");
            strcpy(node->token,"OPEN_SQUARE_BRACKET");
            break;
        case 7:
            strcpy(node->lexeme,"]");
            strcpy(node->token,"CLOSE_SQUARE_BRACKET");
            break;
        default:
            strcpy(node->lexeme,"");
            strcpy(node->token,"ERROR");
            break;
        }
        if(strlen(node->lexeme)>0){
            last_node->next=node;
            last_node=node;
        }
    }
    head_ident=last_node;
}


///Searches through the symbol table for the lexeme
///
///If found returns pointer to table entry
///
///Else returns NULL pointer
struct table_entry* search(char lexeme[1000]){
    struct table_entry* temp=head_ident;
    while(temp!=NULL){
        if(strcmp(temp->lexeme,lexeme)==0){
            return temp;
        }
        temp=temp->next;
    }
    return NULL;
}

///This function can be used as a DFA for identifying *identifiers* and *keywords*
///
///The word is identified and checked in the list of keywords.
///
///If the word is found in list of keywords, it is considered as keyword and appropriate token is assigned.
///
///If the word is not found, then it is considered as identifier and a token **IDENTIFIER** is assigned.
///
///Update symbol table:
///
///Search through symbol table and if the entry for the lexeme already exists, don't do anything.
///
///Else, add the lexeme-token pair as an entry
struct pair dfa_ident_key(char ch){
    struct pair p;
    int state=1;
    if(isalpha(ch)){
        state=2;
    }
    else{
        state=-1;
        return p;
    }
    char lexeme[100]={'\0'};
    lexeme[0]=ch;
    int i=1;
    while(file_content[curr]!=EOF){
        ch=file_content[curr];
        curr++;
        if(isalnum(ch)==0 && ch!='_'){
            // fseek(file,-1L,SEEK_CUR); //Move 1 char back bcoz we read extra char
            curr=curr-1;
            lexeme[i]='\0';     //NOT SURE
            break;
        }
        lexeme[i]=ch;
        i++;
    }
    strcpy(p.lexeme,lexeme);
    for(int i=0;i<num_keywords;i++){
        if(strcmp(keywords_arr[i],lexeme)==0){
            strcpy(p.token,token_arr[i]);
            return p;
        }
    }
    struct table_entry* ptr=search(lexeme);
    if(ptr!=NULL){
        // printf("---Lexeme %s is already present in symbol table\n",lexeme);
    }
    else{
        // printf("-----Adding lexeme %s to symbol table\n",lexeme);
        struct table_entry* node=newNode(lexeme,"IDENTIFIER");
        last_node->next=node;
        last_node=node;
    }
    strcpy(p.token,"IDENTIFIER");
    return p;
}

///This function can be used as a DFA for identifying *number literals* and *floating point literals*
///
///For number literal, the token assigned is **INT_NUMBER**.
///
///For floating point literal, token assigned is **FLOATING_NUMBER**.
///
///@returns lexeme-token pair
struct pair dfa_int_float(char ch){
    struct pair p;
    int state=1;
    if(ch=='+'){
        state=2;
    }
    else if(ch=='-'){
        state=3;
    }
    else if(isdigit(ch)){
        state=4;
    }
    else{
        state=-1;
        return p;
    }

    char lexeme[100]={'\0'};
    lexeme[0]=ch;
    int i=1;
    while(file_content[curr]!=EOF){
        ch=file_content[curr];
        curr++;
        if(state==2 || state==3){
            if(isdigit(ch)){
                lexeme[i]=ch;
                i++;
                state=4;
            }
            else{
                // printf("ch is %c, so try operator\n",ch);
                // fseek(file,-1L,SEEK_CUR); //Move 1 char back bcoz we read extra char
                curr=curr-1;
                strcpy(p.token,"");
                return p;
            }
        }
        else if(state==4){
            if(isdigit(ch)){
                lexeme[i]=ch;
                i++;
                state=4;
            }
            else if(ch=='.'){
                lexeme[i]=ch;
                i++;
                state=5;
            }
            else{
                // fseek(file,-1L,SEEK_CUR); //Move 1 char back bcoz we read extra char
                curr--;
                break;
            }
        }
        else if(state==5){
            if(isdigit(ch)){
                lexeme[i]=ch;
                i++;
                state=6;
            }
            else{
                // fseek(file,-1L,SEEK_CUR);
                curr--;
                strcpy(p.token,"ERROR");
                strcpy(p.lexeme,lexeme);
                return p;
            }
        }
        else if(state==6){
            if(isdigit(ch)){
                lexeme[i]=ch;
                i++;
                state=6;
            }
            else{
                // fseek(file,-1L,SEEK_CUR);
                curr--;
                break;
            }
        }
    }
    if(state==4){
        strcpy(p.token,"INT_NUMBER");
    }
    else if(state==6){
        strcpy(p.token,"FLOATING_NUMBER");
    }
    else{
        strcpy(p.token,"ERROR");
    }
    strcpy(p.lexeme,lexeme);
    return p;
}

///This function can be used as a DFA for identifying *Operators*
///
///The DFA state transitions happens based on the character/operator.
///
///An appropriate token is identified based on the final state of the DFA
///
///@returns lexeme-token pair
struct pair dfa_operator(char ch){
    char lexeme[3]="\0\0\0";
    struct pair p;
    int state=1;
    if(ch=='+'){
        state=2;
        lexeme[0]=ch;
        // ch=fgetc(file);
        ch=file_content[curr];
        curr++;
        if(ch!=EOF){
            if(ch=='='){
                lexeme[1]=ch;
                state=3;
            }
            else if(ch=='+'){
                lexeme[1]=ch;
                state=19;
            }
            else{
                // fseek(file,-1L,SEEK_CUR);
                curr--;
            }
        }
        else{
            // fseek(file,-1L,SEEK_CUR);
            curr--;
        }
    }
    else if(ch=='-'){
        state=4;
        lexeme[0]=ch;
        // ch=fgetc(file);
        ch=file_content[curr];
        curr++;
        if(ch!=EOF){
            if(ch=='='){
                lexeme[1]=ch;
                state=5;
            }
            else if(ch=='-'){
                lexeme[1]=ch;
                state=20;
            }
            else{
                // fseek(file,-1L,SEEK_CUR);
                curr--;
            }
        }
        else{
            // fseek(file,-1L,SEEK_CUR);
            curr--;
        }
    }
    else if(ch=='*'){
        state=6;
        lexeme[0]=ch;
        // ch=fgetc(file);
        ch=file_content[curr];
        curr++;
        if(ch!=EOF){
            if(ch=='='){
                lexeme[1]=ch;
                state=7;
            }
            else{
                // fseek(file,-1L,SEEK_CUR);
                curr--;
            }
        }
        else{
            // fseek(file,-1L,SEEK_CUR);
            curr--;
        }
    }
    else if(ch=='/'){
        state=8;
        lexeme[0]=ch;
        // ch=fgetc(file);
        ch=file_content[curr];
        curr++;
        if(ch!=EOF){
            if(ch=='='){
                lexeme[1]=ch;
                state=9;
            }
            else{
                // fseek(file,-1L,SEEK_CUR);
                curr--;
            }
        }
        else{
            // fseek(file,-1L,SEEK_CUR);
            curr--;
        }
    }
    else if(ch=='%'){
        state=10;
        lexeme[0]=ch;
        // ch=fgetc(file);
        ch=file_content[curr];
        curr++;
        if(ch!=EOF){
            if(ch=='='){
                lexeme[1]=ch;
                state=25;
            }
            else{
                // fseek(file,-1L,SEEK_CUR);
                curr--;
            }
        }
        else{
            // fseek(file,-1L,SEEK_CUR);
            curr--;
        }
    }
    else if(ch=='='){
        state=11;
        lexeme[0]=ch;
        // ch=fgetc(file);
        ch=file_content[curr];
        curr++;
        if(ch!=EOF){
            if(ch=='='){
                lexeme[1]=ch;
                state=12;
            }
            else{
                // fseek(file,-1L,SEEK_CUR);
                curr--;
            }
        }
        else{
            // fseek(file,-1L,SEEK_CUR);
            curr--;
        }
    }
    else if(ch=='>'){
        state=13;
        lexeme[0]=ch;
        // ch=fgetc(file);
        ch=file_content[curr];
        curr++;
        if(ch!=EOF){
            if(ch=='='){
                lexeme[1]=ch;
                state=14;
            }
            else{
                // fseek(file,-1L,SEEK_CUR);
                curr--;
            }
        }
        else{
            // fseek(file,-1L,SEEK_CUR);
            curr--;
        }
    }
    else if(ch=='<'){
        state=15;
        lexeme[0]=ch;
        // ch=fgetc(file);
        ch=file_content[curr];
        curr++;
        if(ch!=EOF){
            if(ch=='='){
                lexeme[1]=ch;
                state=16;
            }
            else{
                // fseek(file,-1L,SEEK_CUR);
                curr--;
            }
        }
        else{
            // fseek(file,-1L,SEEK_CUR);
            curr--;
        }
    }
    else if(ch=='!'){
        state=17;
        lexeme[0]=ch;
        // ch=fgetc(file);
        ch=file_content[curr];
        curr++;
        if(ch!=EOF){
            if(ch=='='){
                lexeme[1]=ch;
                state=18;
            }
            else{
                // fseek(file,-1L,SEEK_CUR);
                curr--;
            }
        }
        else{
            // fseek(file,-1L,SEEK_CUR);
            curr--;
        }
    }
    else if(ch=='&'){
        state=21;
        lexeme[0]=ch;
        // ch=fgetc(file);
        ch=file_content[curr];
        curr++;
        if(ch!=EOF){
            if(ch=='&'){
                lexeme[1]=ch;
                state=22;
            }
            else{
                // fseek(file,-1L,SEEK_CUR);
                curr--;
            }
        }
        else{
            // fseek(file,-1L,SEEK_CUR);
            curr--;
        }
    }
    else if(ch=='|'){
        state=23;
        lexeme[0]=ch;
        // ch=fgetc(file);
        ch=file_content[curr];
        curr++;
        if(ch!=EOF){
            if(ch=='|'){
                lexeme[1]=ch;
                state=24;
            }
            else{
                // fseek(file,-1L,SEEK_CUR);
                curr--;
            }
        }
        else{
            // fseek(file,-1L,SEEK_CUR);
            curr--;
        }
    }
    else if(ch=='?'){
        state=26;
        lexeme[0]=ch;
    }
    else if(ch==':'){
        state=27;
        lexeme[0]=ch;
        // ch=fgetc(file);
        ch=file_content[curr];
        curr++;
        if(ch!=EOF){
            if(ch=='='){
                lexeme[1]=ch;
                state=28;
            }
            else{
                // fseek(file,-1L,SEEK_CUR);
                curr--;
            }
        }
        else{
            // fseek(file,-1L,SEEK_CUR);
            curr--;
        }
    }
    else{
        printf("Unknown operator %c\n",ch);
        strcpy(p.lexeme,"");
        strcpy(p.token,"ERROR");
        return p;
    }


    switch (state){
        case 2:
            strcpy(p.token,"PLUS_OPERATOR");
            break;
        case 3:
            strcpy(p.token,"ADDITION_ASSIGNMENT_OPERATOR");
            break;
        case 4:
            strcpy(p.token,"MINUS_OPERATOR");
            break;
        case 5:
            strcpy(p.token,"SUBTRACTION_ASSIGNMENT_OPERATOR");
            break;
        case 6:
            strcpy(p.token,"STAR_OPERATOR");
            break;
        case 7:
            strcpy(p.token,"MULTIPLICATION_ASSIGNMENT_OPERATOR");
            break;
        case 8:
            strcpy(p.token,"DIVISION_OPERATOR");
            break;
        case 9:
            strcpy(p.token,"DIVISION_ASSIGNMENT_OPERATOR");
            break;
        case 10:
            strcpy(p.token,"MODULOUS_OPERATOR");
            break;
        case 11:
            strcpy(p.token,"ASSIGNMENT_OPERATOR");
            break;
        case 12:
            strcpy(p.token,"COMPARISION_OPERATOR");
            break;
        case 13:
            strcpy(p.token,"GREATER_THAN_OPERATOR");
            break;
        case 14:
            strcpy(p.token,"GREATER_THAN_OR_EQUAL_OPERATOR");
            break;
        case 15:
            strcpy(p.token,"LESSER_THAN_OPERATOR");
            break;
        case 16:
            strcpy(p.token,"LESSER_THAN_OR_EQUAL_OPERATOR");
            break;
        case 17:
            strcpy(p.token,"NOT_OPERATOR");
            break;
        case 18:
            strcpy(p.token,"NOT_EQUAL_OPERATOR");
            break;
        case 19:
            strcpy(p.token,"INCREMENT_OPERATOR");
            break;
        case 20:
            strcpy(p.token,"DECREMENT_OPERATOR");
            break;
        case 22:
            strcpy(p.token,"AND_OPERATOR");
            break;
        case 24:
            strcpy(p.token,"OR_OPERATOR");
            break;
        case 25:
            strcpy(p.token,"MODULOUS_ASSIGNMENT_OPERATOR");
            break;
        case 26:
            strcpy(p.token,"CONDITIONAL_QUESTION_MARK_OPERATOR");
            break;
        case 27:
            strcpy(p.token,"COLON_OPERATOR");
            break;
        case 28:
            strcpy(p.token,"COLON_ASSIGNMENT_OPERATOR");
            break;
        default:
            strcpy(p.token,"ERROR");
            strcpy(p.lexeme,"");
            return p;
            break;
    }

    strcpy(p.lexeme,lexeme);
    return p;
}

///This function can be used for identifying *Delimiters*
///
///As the delimiter is only 1 character, the token is identified by using the switch statement that takes the character as input.
///
///@returns lexeme-token pair
struct pair dfa_delim(char ch){
    struct pair p;
    char lexeme[2];
    switch (ch)
    {
    case ',':
        strcpy(p.token,"COMMA_DELIM");
        break;
    case ';':
        strcpy(p.token,"SEMICOLON_DELIM");
        break;
    case '(':
        strcpy(p.token,"OPEN_BRACKET");
        break;
    case ')':
        strcpy(p.token,"CLOSE_BRACKET");
        break;
    case '{':
        strcpy(p.token,"OPEN_FLOWER_BRACKET");
        break;
    case '}':
        strcpy(p.token,"CLOSE_FLOWER_BRACKET");
        break;
    case '[':
        strcpy(p.token,"OPEN_SQUARE_BRACKET");
        break;
    case ']':
        strcpy(p.token,"CLOSE_SQUARE_BRACKET");
        break;
    default:
        strcpy(p.token,"ERROR");
        strcpy(p.lexeme,"");
        return p;
        break;
    }
    lexeme[0]=ch;
    lexeme[1]='\0';
    strcpy(p.lexeme,lexeme);
    return p;
}

///This function can be used as a DFA for identifying *String literals* and *Character literals*
///
///For String literal, the token assigned is **STRING_LITERAL**.
///
///For Character literal, token assigned is **CHARACTER_LITERAL**.
///
///The code also checks the length of the character literal, and if it is >1, then it produces an error and terminates the process.
///
///@returns lexeme-token pair
struct pair dfa_string(char ch){
    // printf("initial char is %c\n",ch);
    int state=1;
    struct pair p;
    if(ch=='"'){
        state=2;
    }
    else if(ch=='\''){
        state=5;
    }
    else{
        state=-1;
        strcpy(p.lexeme,"");
        strcpy(p.token,"ERROR");
        return p;
    }
    // printf("state=%d\n",state);
    char lexeme[1000]={'\0'};
    lexeme[0]=ch;
    int i=1;
    while(file_content[curr]!=EOF){
        ch=file_content[curr];
        curr++;
        if(state==2){
            if(ch=='"'){
                state=4;
                lexeme[i]=ch;
                break;
            }
            if(ch=='\\'){
                // ch=fgetc(file);
                ch=file_content[curr];
                curr++;
                if(ch=='t'){
                    lexeme[i]='\t';
                }
                else if(ch=='n'){
                    lexeme[i]='\n';
                }
                else if(ch=='r'){
                    lexeme[i]='\r';
                }
                else if(ch=='"'){
                    lexeme[i]='\"';
                }
                else if(ch=='\\'){
                    lexeme[i]='\\';
                }
                else{
                    printf("ERR------Unknown escape sequence \\%c\n",ch);
                    strcpy(p.lexeme,lexeme);
                    strcpy(p.token,"ERROR");
                    printf("Exiting due to some ERROR\n");
                    exit(1);
                    return p;
                }
            }
            else{
                lexeme[i]=ch;
            }
            i++;
            state=3;
        }
        else if(state==3){
            if(ch=='"'){
                state=4;
                lexeme[i]=ch;
                break;
            }
            if(ch=='\\'){
                // ch=fgetc(file);
                ch=file_content[curr];
                curr++;
                if(ch=='t'){
                    lexeme[i]='\t';
                }
                else if(ch=='n'){
                    lexeme[i]='\n';
                }
                else if(ch=='r'){
                    lexeme[i]='\r';
                }
                else if(ch=='"'){
                    lexeme[i]='\"';
                }
                else if(ch=='\\'){
                    lexeme[i]='\\';
                }
                else{
                    printf("ERR------Unknown escape sequence \\%c\n",ch);
                    strcpy(p.lexeme,lexeme);
                    strcpy(p.token,"ERROR");
                    printf("Exiting due to some ERROR\n");
                    exit(1);
                    return p;
                }
            }
            else{
                lexeme[i]=ch;
            }
            // lexeme[i]=ch;
            i++;
            state=3;
        }
        else if(state==4){
            break;
        }
        else if(state==5){
            if(ch=='\''){
                // state=7;
                // lexeme[i]=ch;
                printf("Should have atlease 1 character inside quotes, found 0\n");
                strcpy(p.token,"ERROR");
                strcpy(p.lexeme,lexeme);
                return p;
            }
            if(ch=='\\'){
                // ch=fgetc(file);
                ch=file_content[curr];
                curr++;
                if(ch=='t'){
                    lexeme[i]='\t';
                }
                else if(ch=='n'){
                    lexeme[i]='\n';
                }
                else if(ch=='r'){
                    lexeme[i]='\r';
                }
                else if(ch=='"'){
                    lexeme[i]='\"';
                }
                else if(ch=='\\'){
                    lexeme[i]='\\';
                }
                else{
                    printf("ERR------Unknown escape sequence \\%c\n",ch);
                    strcpy(p.lexeme,lexeme);
                    strcpy(p.token,"ERROR");
                    printf("Exiting due to some ERROR\n");
                    exit(1);
                    return p;
                }
            }
            else{
                lexeme[i]=ch;
            }
            // lexeme[i]=ch;
            i++;
            state=6;
        }
        else if(state==6){
            // printf("state=%d\n",state);
            if(ch=='\''){
                state=7;
                // printf("state=%d char ended\n",state);
                lexeme[i]=ch;
                break;
            }
            // strcpy(p.lexeme,lexeme);
            lexeme[i]=ch;
            i++;
            state=6;
            strcpy(p.token,"ERROR");
            // return p;
            // lexeme[i]=ch;
            // i++;
            // state=6;
        }
        else if(state==7){
            // printf("state=%d\n",state);
            break;
        }
    }
    if(state==4){
        strcpy(p.lexeme,lexeme);
        strcpy(p.token,"STRING_LITERAL");
        return p;
    }
    else if(state==7){
        // int tot=0;
        // for(int i=1;i<strlen(lexeme)-1;i++){
        //     tot=tot+lexeme[i];
        // }
        // if(strlen(lexeme)>3 && tot>126){
        //     printf("This is not part of the ASCII character set\n");
        //     strcpy(p.lexeme,lexeme);
        //     strcpy(p.token,"ERROR");
        //     return p;
        // }
        // else{
        //     strcpy(p.lexeme,lexeme);
        //     strcpy(p.token,"CHARACTER_LITERAL");
        //     return p;
        // }
        strcpy(p.lexeme,lexeme);
        if(strcmp(p.token,"ERROR")==0){
            printf("\nERR------Character can only have 1 character\n");
            // printf("")
            return p;
        }
        strcpy(p.token,"CHARACTER_LITERAL");
        return p;
    }
    else{
        strcpy(p.lexeme,lexeme);
        strcpy(p.token,"ERROR");
        printf("Error in string literal (may be because of improper closure of string by quotes)");
        return p;
    }
}

///Scans through the comment lines
///
///Comments can be in the form of //Comment (single line comment) 
///
///Or
///
///(Multiple lines comment) /*This
///
///is a Comment*/ 
void comment(char ch){
    if(ch=='/'){
        while(file_content[curr]!=EOF){
            ch=file_content[curr];
            curr++;
            if(ch=='\n'){
                // line_number++;
                // fseek(file,-1L,SEEK_CUR);
                curr--;
                break;
            }
        }
    }
    else if(ch=='*'){
        int star=0;
        while(file_content[curr]!=EOF){
            ch=file_content[curr];
            curr++;
            if(ch=='\n'){
                line_number++;
            }
            if(ch=='*'){
                star=1;
            }
            else if(ch=='/'){
                if(star==1){
                    break;
                }
            }
            else{
                star=0;
            }
        }
    }
}

///This function acts as a DFA to choose DFAs
///
///To decide which DFA to use, the code initially scans 1 character and decides which DFA to use next
///
///For example:
///
/// If the character is a alphabet, then we call dfa_ident_key() function
///
///If the character is a digit, then we call dfa_int_float() function
///
///If the character is a operator, then we call dfa_operator() function
///
///If the character is " (double quote) or ' (single quote), then we call dfa_string() function
struct pair scanner(){
    char ch;
    struct pair p;
    strcpy(p.lexeme,"");
    strcpy(p.token,"");
    if(file_content[curr]!=EOF){
        ch=file_content[curr];
        curr++;
        if(ch=='/'){
            //check for comment
            if(file_content[curr]!=EOF){
                ch=file_content[curr];
                curr++;
                if(ch=='/' || ch=='*'){
                    comment(ch);
                    strcpy(p.lexeme,"COMMENT");
                    strcpy(p.token,"COMMENT");
                    return p;
                }
            }
            else{
                strcpy(p.lexeme,"/");
                strcpy(p.token,"DIVISION_OPERATOR");
                return p;
            }
        }
        if(isalpha(ch)){    //can ident start with _?
            p=dfa_ident_key(ch);
        }
        else if(isdigit(ch)){
            p=dfa_int_float(ch);
        }
        else if(ch=='+' || ch=='-'){
            long go_front=1;
            // fseek(file,-2L,SEEK_CUR);
            curr=curr-2;
            // char temp=fgetc(file);
            char temp=file_content[curr];
            curr++;
            while(temp==' '){
                // fseek(file,-2L,SEEK_CUR);
                curr=curr-2;
                // temp=fgetc(file);
                temp=file_content[curr];
                curr++;
                go_front++;
            }
            // fseek(file,go_front,SEEK_CUR);
            curr=curr+go_front;
            if(temp=='(' || temp=='='){     //YET TO DO FOR SPACES
                p=dfa_int_float(ch);
            }
            if(strcmp(p.token,"")==0){
                p=dfa_operator(ch);
            }
        }
        else if(ch=='*' || ch=='/' || ch=='%' || ch=='=' || ch=='>' || ch=='<' || ch=='!' || ch=='&' || ch=='|' || ch=='?' || ch==':'){
            p=dfa_operator(ch);
        }
        else if(ch==',' || ch==';' || ch=='(' || ch==')' || ch=='{' || ch=='}' || ch=='[' || ch==']'){
            p=dfa_delim(ch);
        }
        else if(ch=='"' || ch=='\''){
            p=dfa_string(ch);
        }
        else if(ch=='#'){
            char lexeme[100];
            int i=0;
            while(ch!='\n'){
                lexeme[i]=ch;
                i++;
                ch=file_content[curr];
                curr++;
            }
            curr--;
            strcpy(p.lexeme,lexeme);
            strcpy(p.token,"PRE_PROCESSING_DIRECTIVE");
        }
        else if(ch=='\t'){
            strcpy(p.lexeme,"\t");
            strcpy(p.token,"WHITE_SPACE_TAB");
        }
        else if(ch==' '){
            strcpy(p.lexeme," ");
            strcpy(p.token,"WHITE_SPACE");
        }
        else if(ch=='\n'){
            strcpy(p.lexeme,"\n");
            strcpy(p.token,"NEW_LINE");
            line_number++;
        }
        else{
            printf("LEXICAL ERROR--Starting with unknown symbol %c\n",ch);
            strcpy(p.token,"ERROR");
            char lexeme[1];
            lexeme[0]=ch;
            strcpy(p.lexeme,lexeme);
            return p;
        }
    }
    else{
        strcpy(p.lexeme,"EOF");
        strcpy(p.token,"EOF");
        return p;
    }
    if(p.token==""){
        printf("(ERROR) None of the tokens\n");
    }
    return p;
}

///Parser calls scanner which returns the nextToken
///
///Parser calls scanner() function, which returns lexeme-token pair
///
///The parser prints the lexeme-token pair if the token is NOT whitespaces, new line, comment
///
///Parser keeps on calling scanner till the scanner returns *EOF* token (End Of File token)
///
///@param file_name: The entered file name which needs to be compiled
void parser(char file_name[]){
    printf("filename is %s\n",file_name);
    FILE *inp_file_ptr=fopen(file_name,"r");
    if(inp_file_ptr==NULL){
        printf("Error in opening file\n");
        return;
    }
    char ch;
    printf("Reading file character by character and storing it\n");
    int i=0;
    while((ch=fgetc(inp_file_ptr))!=EOF){
        file_content[i]=ch;
        i++;
        // printf("%c",ch);
    }
    file_content[i]=EOF;
    file_content[i+1]='\0';
    fclose(inp_file_ptr);
    printf("\nFile reading completed, closing file\n");
    // rewind(inp_file_ptr);   //come back to 1st char
    printf("\n");
    struct pair p;
    do
    {
        p=scanner();
        if(strcmp(p.token,"PRE_PROCESSING_DIRECTIVE")!=0 && strcmp(p.token,"WHITE_SPACE")!=0 && strcmp(p.token,"WHITE_SPACE_TAB")!=0 && strcmp(p.token,"NEW_LINE")!=0 && strcmp(p.token,"COMMENT")!=0 && strcmp(p.token,"ERROR")!=0 && strcmp(p.token,"EOF")!=0)
            printf("lexeme is %s and token is %s line number %d\n",p.lexeme,p.token,line_number);
            // printf("Token %s,\t\tstring %s, line number %d\n",p.token,p.lexeme,line_number);
        if(strcmp(p.token,"ERROR")==0){
            printf("Error in lexeme \nAnd exiting program");
            exit(1);
        }
    } while (strcmp(p.token,"EOF")!=0);
}


int main(){
    generate_initial_table();
    printf("enter file name or path : ");
    char file_name[200];
    // scanf("%s",file_name);
    gets(file_name);
    int correct_file=1;
    int n=strlen(file_name);
    if(file_name[n-4]!='.'){
        correct_file=0;
    }
    if(file_name[n-3]!='s'){
        correct_file=0;
    }
    if(file_name[n-2]!='p'){
        correct_file=0;
    }
    if(file_name[n-1]!='p'){
        correct_file=0;
    }
    if(!correct_file){
        printf("Incorrect file type. Acceptable file type: *.spp\n");
    }
    parser(file_name);
}