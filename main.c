
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
struct issue{
    char issue_description[2000];
    char answer1[1000];
    int res1_1;
    int res1_2;
    int res1_3;
    char answer2[1000];
    int res2_1;
    int res2_2;
    int res2_3;
    int probability;
};

struct parameters{
    int people;
    int court;
    int treasury;
};

struct node{
struct issue data;
struct node* next;
};

int in_files_num;

 struct node* create_node(char name[100]){
    struct node *handler = NULL;
    FILE *fp;
    fp = fopen( name , "r");
    handler =(struct node*)malloc(sizeof(struct node));
    fgets( (handler->data.issue_description) ,1000, fp);
    fgets( (handler->data.answer1) , 1000, fp);
    fscanf(fp ,"%d" ,&(handler->data.res1_1));
    fscanf(fp ,"%d" ,&(handler->data.res1_2));
    fscanf(fp ,"%d" ,&(handler->data.res1_3));
    fgetc(fp);
    fgets( (handler->data.answer2) , 1000, fp);
    fscanf(fp ,"%d" ,&(handler->data.res2_1));
    fscanf(fp ,"%d" ,&(handler->data.res2_2));
    fscanf(fp ,"%d" ,&(handler->data.res2_3));
    fclose(fp);
    handler->data.probability = 3;
    return handler;
}

struct node *create_load_node( struct issue p){
        struct node* nn= (struct node*)malloc( sizeof( struct node));
        nn->data = p;
        nn->next = NULL;
        return nn;
}

void work_on_issue(struct node* list ,int i){
    struct node* current= list;
    while(1){
        if( i==0 ){
            break;
        }
        current = current->next;
        i--;
    }
    printf("%s", current->data.issue_description);
    printf("%s", current->data.answer1);
    printf("%s", current->data.answer2);
    (current->data.probability) += -1;
    printf("probability = %d\n", current->data.probability );
}

void work_on_parameters(struct node* list,int i,int choice,struct parameters* result ){
    struct node* current= list;
    while(1){
        if( i==0 ){
            break;
        }
        current = current->next;
        i--;
    }
    if( choice == 1 ){
        result->court += current->data.res1_1;
        result->people += current->data.res1_2;
        result->treasury += current->data.res1_3;
    }
    if( choice ==2 ){
        result->court += current->data.res2_1;
        result->people += current->data.res2_2;
        result->treasury += current->data.res2_3;
    }
}

void remove_issue( struct node **list, struct node * target){
    if( target == *list ){
        (*list) = (*list)->next;
        free( target );
    }
    else{
        struct node* current = (*list);
        while( current->next != target ){
            current = current->next;
        }
        current->next = target->next;
        free(target);
    }
}


void check_probability( int i , struct node** list , int *random  ){
    struct node* current= *list;
    while(1){
        if( i==0 ){
            break;
        }
        current = current->next;
        i--;
    }
    if( (current->data.probability) ==0 ){
        struct node* tmp= current->next;
        remove_issue( list , current );
        current = tmp;
        (*random)--;
        in_files_num--;
    }
}


struct node *create_list(){
    struct node *list=NULL;
    FILE* fp;
    fp=fopen("CHOICES.txt", "r");
    char name[100];
    fgets(name ,100, fp);
    if(name[strlen(name)-1] == '\n')
        name[strlen(name)-1] = 0;
    list = create_node(name);
    in_files_num++;
    struct node* start = list;
    while(1){
        if(feof(fp)){
            break;
        }
        fgets(name , 100, fp);
        if(name[strlen(name)-1] == '\n')
            name[strlen(name)-1] = 0;
        list->next=create_node( name );
        in_files_num++;
        list=list->next;
    }
    fclose(fp);
    return start;
}

void search_player_name(char strings[]){
    FILE *leader_board;
    leader_board = fopen(".\\leader_board.txt" , "a+");
    fseek(leader_board, 0, SEEK_SET);
     while(1){
        char tmp[30];
        fscanf(leader_board, "%s", tmp);
        if(  strcmp( tmp , strings) == 0 ){
        return ;
        }
        char c = fgetc(leader_board);
        if(c == EOF){
            break;
        }
        fseek(leader_board, -1, SEEK_CUR);
    }
    fputs( strings , leader_board);
    fprintf(leader_board, "\n");
    fclose(leader_board);
    return ;
}

struct leader{
    char name[30];
    struct parameters resource;
};

int compare( const void* a, const void* b){
        struct leader* x = (struct leader*)a;
        struct leader* y = (struct leader*)b;
        if( (x->resource).court   < (y->resource).court )
            return 1;
        if( (x->resource).court > (y->resource).court )
            return -1;
        if( (x->resource).people < (y->resource).people )
            return 1;
        if( (x->resource).people > (y->resource).people )
            return -1;
        if( (x->resource).treasury < (y->resource).treasury)
            return 1;
        if( (x->resource).treasury > (y->resource).treasury)
            return -1;
        return 0;}


void show_leader_board(){
    FILE *fp;
    fp = fopen(".\\leader_board.txt" , "r");
    if(fp == NULL)
        printf("not open it");

    struct leader board[1000];
    int n=0;
    while(1){
        char tmp[30];
        fgets(tmp,30,fp);
        if (feof(fp))
            break;
        if(tmp[strlen(tmp)-1]=='\n')
            tmp[strlen(tmp)-1]=0;
        strcpy(board[n].name,tmp);
        char address[40];
        strcpy( address , ".\\");
        strcat( address , board[n].name);
        strcat( address , ".bin");
        FILE * fp_player;
        fp_player = fopen(address , "rb");
        if (fp_player == NULL) {
            printf("na baba\n");
            fflush(stdin);
        } else {
            fseek(fp_player, sizeof(board[n].name) + 8, SEEK_SET);
            fread(&(board[n].resource), sizeof(board[n].resource), 1, fp_player);
            n++;
            fclose(fp_player);
        }
    }
    fclose(fp);
    qsort( board , n , sizeof( struct leader) , compare);
    for ( int j=0 ; j < n ; j++){
        printf("%s = %d %d %d\n", board[j].name , board[j].resource.court ,board[j].resource.people, board[j].resource.treasury);
    }
}

void add_question(){
    FILE *fp;
    int new_question ;
    char tmp[200];
    char new_que_name[30];
    gets( new_que_name );
    char address[40] = {".\\"};
    strcat( address , new_que_name );
    strcat( address , ".txt");
    fp = fopen("CHOICES.txt" , "a");
    fputc( '\n' , fp );
    strcat( new_que_name , ".txt");
    fputs( new_que_name , fp );
    fclose( fp );
    FILE *ffp;
    ffp = fopen( address , "w");
    gets(tmp);
    fputs( tmp , ffp );
    fputc( '\n' , ffp );
    gets( tmp);
    fputs( tmp , ffp );
    fputc( '\n' , ffp );
    scanf("%d" , &new_question );
    fprintf( ffp , "%d\n" , new_question );
    scanf("%d" , &new_question );
    fprintf( ffp , "%d" , new_question );
    fputc( '\n' , ffp );
    scanf("%d" , &new_question );
    fprintf( ffp , "%d" , new_question );
    fputc( '\n' , ffp );
    scanf("\n");
    gets( tmp );
    fputs( tmp , ffp );
    fputc( '\n' , ffp );
    scanf("%d" , &new_question );
    fprintf( ffp , "%d" , new_question );
    fputc( '\n' , ffp );
    scanf("%d" , &new_question );
    fprintf( ffp , "%d" , new_question );
    fputc( '\n' , ffp );
    scanf("%d" , &new_question );
    fprintf( ffp , "%d" , new_question );
    fputc( '\n' , ffp );
    fclose( ffp );
    return ;
}


int main()
{
    system("color 5f");
    char player_name[30];
    scanf("%s", player_name);
    char address[40];
    strcpy( address , ".\\");
    strcat( address , player_name);
    strcat( address , ".bin");
    search_player_name( player_name );
    int mark = 0;
    struct node * start;
    int random;
    struct parameters* result= (struct parameters *) malloc(sizeof(*result));
    printf("Do you want to\n1-play a new game\n2-continue\n3-show leader board\n4-add question");
    int what_to_do;
    scanf("%d", &what_to_do);
    if( what_to_do == 4){
            getchar();
        add_question();
        return 0;
    }
    if( what_to_do == 3){
        show_leader_board();
        return 0;
    }
    if( what_to_do == 2){
        FILE* fffp;
        fffp = fopen(address , "rb");
        fread( player_name , sizeof(char), sizeof(player_name) , fffp);
        fread( &mark , sizeof(int) , 1, fffp );
        if( mark == 1){
            fread( &in_files_num , sizeof(int ) , 1 ,fffp );
            fread(result,sizeof(*result),1,fffp);
            struct issue load;
            struct node* current = NULL;
            for( int j=0 ; j< in_files_num ; j++){
                fread( &load, sizeof( load ) , 1, fffp );
                if( current == NULL){
                    start = current = create_load_node( load );
                }
                else{
                    current->next = create_load_node( load );
                    current = current->next;
                }
            }
        }
    }
    if(mark == 0){
        start = create_list();
        result->people = 50;
        result->court = 50;
        result->treasury = 50;
    }
    random = in_files_num;
    mark = 0;
    int AVERAGE =( (result->court) + (result->people) + (result->treasury) ) / 3;
    while((result->court) >0 &&  (result->people) >0 &&  (result->treasury) > 0 && AVERAGE >= 10){
        if(random == 0){
            start = create_list();
            random = in_files_num;
        }
        time_t t= time(NULL);
        srand(t);
        int i = rand()%random;
        work_on_issue(start,i);
        printf("%d %d\n" , random , i );
        struct node *current1 = start;
        while( current1 != NULL ){
            printf("probability = %d\n", current1->data.probability );
            current1 = current1->next ;}
        int choice;
        scanf("%d",&choice);
            if( choice == -1   ){
                mark = 1;
                break;
            }
        work_on_parameters(start ,i,choice, result );
        check_probability( i , &start , &random );
        printf("%d %d %d\n ",result->court,result->people,result->treasury);
        AVERAGE =( (result->court) + (result->people) + (result->treasury) ) / 3;
        printf("AVERAGE = %d\n\n\n\n",AVERAGE);
    }
    printf("Do you want to save? (y/n)\n");
    char save_mode;
    scanf(" %c", &save_mode);
    if(save_mode == 'n')
        return 0;
    FILE *ffp;
    ffp = fopen( address , "wb");
    fwrite( player_name , sizeof(char), sizeof(player_name) , ffp);
    fwrite( &mark , sizeof(int) , 1, ffp );
    fwrite( &in_files_num , sizeof(int ) , 1 ,ffp );
    fwrite( result , sizeof(*result),1,ffp);
    struct node* current= start;
    for( int j=0 ; j< in_files_num ; j++ ){
        fwrite(&current->data , sizeof( struct issue) ,1, ffp );
        current = current->next;
    }
    fclose(ffp);
}
