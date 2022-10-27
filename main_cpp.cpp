#include<bits/stdc++.h>
#include<cstdio>
using namespace std;
// #define num_keywords 10

int line_number=1;

struct lex_tok_pair{
    char lexeme[1000];
    char token[100];
};

vector<struct lex_tok_pair> keywords;

void get_keywords(){
    FILE *keywords_file=fopen("keywords.txt","r");
    char l[100]={'\0'},t[100]={'\0'};
    int lex=1;
    while(feof(keywords_file)==0){
        if(lex==1){
            // cout<<"l is "<<l<<endl;
            // fgets(l,100,keywords_file);
            char ch;
            int i=0;
            while((ch=fgetc(keywords_file))!=EOF){
                // cout<<ch<<" ";
                if(ch==' ' || ch=='\n'){
                    break;
                }
                l[i]=ch;
                i++;
            }
            l[i]='\0';
            // cout<<"\nl is "<<l<<endl;
            lex=0;
        }
        else{
            // fgets(t,100,keywords_file);
            char ch;
            int i=0;
            while((ch=fgetc(keywords_file))!=EOF){
                if(ch==' ' || ch=='\n'){
                    break;
                }
                t[i]=ch;
                i++;
            }
            t[i]='\0';
            struct lex_tok_pair p;
            strcpy(p.lexeme,l);
            strcpy(p.token,t);
            keywords.push_back(p);
            lex=1;
            // cout<<strlen(l)<<" "<<strlen(t)<<endl;
            for(int temp=0;temp<strlen(l);temp++){
                l[temp]='\0';
            }
            for(int temp=0;temp<strlen(t);temp++){
                t[temp]='\0';
            }
            // cout<<l<<" "<<t<<endl;
        }
    }
    fclose(keywords_file);
}



struct lex_tok_pair dfa_ident_key(char ch,FILE* file){
    struct lex_tok_pair p;
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
    while((ch=fgetc(file))!=EOF){
        if(isalnum(ch)==0 && ch!='_'){
            fseek(file,-1L,SEEK_CUR); //Move 1 char back bcoz we read extra char
            lexeme[i]='\0';     //NOT SURE
            break;
        }
        lexeme[i]=ch;
        i++;
    }
    strcpy(p.lexeme,lexeme);
    for(int i=0;i<keywords.size();i++){
        if(strcmp(keywords[i].lexeme,lexeme)==0){
            strcpy(p.token,keywords[i].token);
            return p;
        }
    }
    strcpy(p.token,"IDENTIFIER");
    return p;
}

struct lex_tok_pair dfa_int_float(char ch,FILE* file){
    struct lex_tok_pair p;
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
    while((ch=fgetc(file))!=EOF){
        if(state==2 || state==3){
            if(isdigit(ch)){
                lexeme[i]=ch;
                i++;
                state=4;
            }
            else{
                // printf("ch is %c, so try operator\n",ch);
                fseek(file,-1L,SEEK_CUR); //Move 1 char back bcoz we read extra char
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
                fseek(file,-1L,SEEK_CUR); //Move 1 char back bcoz we read extra char
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
                fseek(file,-1L,SEEK_CUR);
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
                fseek(file,-1L,SEEK_CUR);
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

struct lex_tok_pair dfa_operator(char ch,FILE* file){
    char lexeme[3]="\0\0";
    struct lex_tok_pair p;
    int state=1;
    if(ch=='+'){
        state=2;
        lexeme[0]=ch;
        ch=fgetc(file);
        if(ch!=EOF){
            if(ch=='='){
                lexeme[1]=ch;
                state=3;
            }
            else{
                fseek(file,-1L,SEEK_CUR);
            }
        }
        else{
            fseek(file,-1L,SEEK_CUR);
        }
    }
    else if(ch=='-'){
        state=4;
        lexeme[0]=ch;
        ch=fgetc(file);
        if(ch!=EOF){
            if(ch=='='){
                lexeme[1]=ch;
                state=5;
            }
            else{
                fseek(file,-1L,SEEK_CUR);
            }
        }
        else{
            fseek(file,-1L,SEEK_CUR);
        }
    }
    else if(ch=='*'){
        state=6;
        lexeme[0]=ch;
        ch=fgetc(file);
        if(ch!=EOF){
            if(ch=='='){
                lexeme[1]=ch;
                state=7;
            }
            else{
                fseek(file,-1L,SEEK_CUR);
            }
        }
        else{
            fseek(file,-1L,SEEK_CUR);
        }
    }
    else if(ch=='/'){
        state=8;
        lexeme[0]=ch;
        ch=fgetc(file);
        if(ch!=EOF){
            if(ch=='='){
                lexeme[1]=ch;
                state=9;
            }
            else{
                fseek(file,-1L,SEEK_CUR);
            }
        }
        else{
            fseek(file,-1L,SEEK_CUR);
        }
    }
    else if(ch=='%'){
        state=10;
        lexeme[0]=ch;
    }
    else if(ch=='='){
        state=11;
        lexeme[0]=ch;
        ch=fgetc(file);
        if(ch!=EOF){
            if(ch=='='){
                lexeme[1]=ch;
                state=12;
            }
            else{
                fseek(file,-1L,SEEK_CUR);
            }
        }
        else{
            fseek(file,-1L,SEEK_CUR);
        }
    }
    else if(ch=='>'){
        state=13;
        lexeme[0]=ch;
        ch=fgetc(file);
        if(ch!=EOF){
            if(ch=='='){
                lexeme[1]=ch;
                state=14;
            }
            else{
                fseek(file,-1L,SEEK_CUR);
            }
        }
        else{
            fseek(file,-1L,SEEK_CUR);
        }
    }
    else if(ch=='<'){
        state=15;
        lexeme[0]=ch;
        ch=fgetc(file);
        if(ch!=EOF){
            if(ch=='='){
                lexeme[1]=ch;
                state=16;
            }
            else{
                fseek(file,-1L,SEEK_CUR);
            }
        }
        else{
            fseek(file,-1L,SEEK_CUR);
        }
    }
    else if(ch=='!'){
        state=17;
        lexeme[0]=ch;
        ch=fgetc(file);
        if(ch!=EOF){
            if(ch=='='){
                lexeme[1]=ch;
                state=18;
            }
            else{
                fseek(file,-1L,SEEK_CUR);
            }
        }
        else{
            fseek(file,-1L,SEEK_CUR);
        }
    }
    else{
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
        default:
            strcpy(p.token,"ERROR");
            strcpy(p.lexeme,"");
            return p;
            break;
    }

    strcpy(p.lexeme,lexeme);
    return p;
}

struct lex_tok_pair dfa_delim(char ch,FILE* file){
    struct lex_tok_pair p;
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

struct lex_tok_pair dfa_string(char ch,FILE* file){
    // printf("initial char is %c\n",ch);
    int state=1;
    struct lex_tok_pair p;
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
    while((ch=fgetc(file))!=EOF){
        if(state==2){
            if(ch=='"'){
                state=4;
                lexeme[i]=ch;
                break;
            }
            lexeme[i]=ch;
            i++;
            state=3;
        }
        else if(state==3){
            if(ch=='"'){
                state=4;
                lexeme[i]=ch;
                break;
            }
            lexeme[i]=ch;
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
            lexeme[i]=ch;
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
            lexeme[i]=ch;
            i++;
            state=6;
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
        int tot=0;
        for(int i=1;i<strlen(lexeme)-1;i++){
            tot=tot+lexeme[i];
        }
        if(strlen(lexeme)>3 && tot>126){
            printf("This is not part of the ASCII character set\n");
            strcpy(p.lexeme,lexeme);
            strcpy(p.token,"ERROR");
            return p;
        }
        else{
            strcpy(p.lexeme,lexeme);
            strcpy(p.token,"CHARACTER_LITERAL");
            return p;
        }
    }
    else{
        strcpy(p.lexeme,lexeme);
        strcpy(p.token,"ERROR");
        printf("Error in string literal (may be because of improper closure of string by quotes)");
        return p;
    }
}

struct lex_tok_pair scanner(FILE *file){
    char ch;
    struct lex_tok_pair p;
    strcpy(p.lexeme,"");
    strcpy(p.token,"");
    if((ch=fgetc(file))!=EOF){
        //start of a lexeme
        // printf("First char is %c\n",ch);
        if(isalpha(ch)){    //can ident start with _?
            p=dfa_ident_key(ch,file);
        }
        else if(isdigit(ch)){
            p=dfa_int_float(ch,file);
        }
        else if(ch=='+' || ch=='-'){
            fseek(file,-2L,SEEK_CUR);
            char temp=fgetc(file);
            fseek(file,+1L,SEEK_CUR);
            if(temp=='(' || temp=='='){     //YET TO DO FOR SPACES
                p=dfa_int_float(ch,file);
            }
            if(strcmp(p.token,"")==0){
                p=dfa_operator(ch,file);
            }
        }
        else if(ch=='*' || ch=='/' || ch=='%' || ch=='=' || ch=='>' || ch=='<' || ch=='!'){
            p=dfa_operator(ch,file);
        }
        else if(ch==',' || ch==';' || ch=='(' || ch==')' || ch=='{' || ch=='}' || ch=='[' || ch==']'){
            p=dfa_delim(ch,file);
        }
        else if(ch=='"' || ch=='\''){
            p=dfa_string(ch,file);
        }
        else if(ch=='#'){
            strcpy(p.lexeme,"#");
            strcpy(p.token,"HASH_SIGN");
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
            printf("LEXICAL ERROR\n");
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

void parser(char file_name[]){
    printf("filename is %s\n",file_name);
    FILE *inp_file_ptr=fopen(file_name,"r");
    if(inp_file_ptr==NULL){
        printf("Error in opening file\n");
        return;
    }
    char ch;
    printf("File read char by char\n");
    while((ch=fgetc(inp_file_ptr))!=EOF){
        printf("%c",ch);
    }
    rewind(inp_file_ptr);   //come back to 1st char
    printf("\n");
    struct lex_tok_pair p;
    do
    {
        p=scanner(inp_file_ptr);
        if(strcmp(p.token,"WHITE_SPACE")!=0 && strcmp(p.token,"WHITE_SPACE_TAB")!=0 && strcmp(p.token,"NEW_LINE")!=0)
        printf("lexeme is %s and token is %s line number %d\n",p.lexeme,p.token,line_number);
    } while (strcmp(p.token,"EOF")!=0);
    fclose(inp_file_ptr);
    printf("\nclosing file\n");
}

int main(){
    get_keywords();
    printf("enter file name or path : ");
    char file_name[200];
    // scanf("%s",file_name);
    // gets(file_name);
    cin>>file_name;
    parser(file_name);
}