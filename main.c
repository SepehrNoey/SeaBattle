#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <math.h>
#define MAX_SHIP_LENGTH 20
struct Cordinate
{
    int x;
    int y;
};
typedef struct Cordinate Cordinate;

struct Map
{
    int map_size[2];// idx[0] -> x  // idx[1] -> y
    char turn_map[MAX_SHIP_LENGTH][MAX_SHIP_LENGTH];
    char full_map[MAX_SHIP_LENGTH][MAX_SHIP_LENGTH];

};
typedef Map Map;

struct Ship
{
    int ship_size;
    char expld_or_not[MAX_SHIP_LENGTH];
    char vrt_or_hrzt;
    Cordinate cordinates[2];   // idx[0] -> start  // idx[1] -> end
    struct Ship *next;

};
typedef struct Ship Ship;

struct Player
{
    char name[50];
    int coin;
    Map player_map;
    Ship *head;   // ships are ignored in list of users    
};
typedef struct Player Player;

struct Game_Data
{
    Player player1;
    Player player2;
    int turn ;     // to check who's turn is
    
};
typedef Game_Data Game_Data;

void first_place_make(Ship *current , int count ,Player *player);
void get_info_ship(int count , int j , Ship *current);
int menu(int choice ,FILE * user_data , int map_selected_size[2] , int ship_selected[21]);
Player newUser(FILE * user_data);
Player player_set(FILE * user_data);
Player player_from_list(FILE * user_data);  // return type is the coins of the player
Ship *putship(Player *player , int map_selected_size[2],int ship_selected[21]);    
int check_hrzt(Ship *current , int count , Player *player);
int check(Ship *current , int count, Player *player);



int main(void){  // 10 * 10
    
    FILE * user_data = fopen("user_data.SB","rb+");  /// loading user_data
    FILE * last_game_data = fopen("last_game_data.SB","rb+");  // loading last game
    
    
    /////
    system("cls");
    puts("Welcome to SEA BATTLE!!\nIf you are ready , select one of the following choices.");  // start of program
    Sleep(3500);
    system("cls");
    /////
    
    int choice = 0;
    int map_selected_size[2] = {10,10};
    int ship_selected[21] = {0,4,3,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  // the value is the number and index is the size

    while (choice != 6)
    {
        menu(choice , user_data , map_selected_size , ship_selected);
        if(choice == 6) /// hala ye seri kar ha bayad bokonim  == 6 or != 6 (exit va save kardan)
        {

        }    
    }
    
        

    fclose(user_data);
    return 0;


}

int menu(int choice ,FILE * user_data , int map_selected_size[2] , int ship_selected[21]){
    
    printf("1) Play with a Friend\n2) Play with \"Captain Bot\"\n3) Load last game\n4) Settings\n5) Score Board\n6) Exit\n");

    scanf("%d",&choice);
    if (choice == 6)
    {
        return choice;
    }
    
    ///playing function calls

    if (choice == 1)
    {
        Player player1 = player_set(user_data);  
        player1.head = putship(&player1 , map_selected_size,ship_selected); 

        printf("Now , second player:\n");
        fseek(user_data,0,SEEK_SET); 
        
        Player player2 = player_set(user_data); 
        player2.head = putship(&player2 , map_selected_size , ship_selected); 
    }

    
    
}

Player player_set(FILE * user_data) {  
    int innerChoice;
    system("cls");
    printf("Choose player:\n\n1) Choose from available users\n2) New user\n");
    scanf("%d",&innerChoice);


    if (innerChoice == 1)
    {
        return player_from_list(user_data);  // returns player from the list of players
    }
    else if (innerChoice == 2)
    {
        return newUser(user_data);           // newplayer
    }
    else 
    {
        puts("Invalid input.Try again");  // bazgasht be porsesh dobare
    }        

}

Player player_from_list(FILE * user_data){
    Player temp;
    int num = 0;
    while (fread(&temp , sizeof(temp) , 1 , user_data) == 1)
    {
        num++;
        printf("%d) Player name: %s\tCoins: %d\n",num,temp.name,temp.coin);    
    }
    

    puts("Please enter the number of player you want:");
    int choice;
    scanf("%d",&choice);
    
    
    if (choice < 0 || choice > num)    // inja momkene badan aziat kone
    {
        puts("Invalid input!");
        return;

    }

    else  
    {
        fseek(user_data,choice - 1,SEEK_SET);
        int res = fread(&temp,sizeof(temp),1,user_data);
        if (res == 1)
        {
            return temp;
        }
        else
        {
            printf("ERROR occurred in reading.\n");
            exit(-1);
        }
        
    }
        
}


Player newUser(FILE * user_data){
    
    /// start of process of checking existence
    system("cls");
    char tempName[50];
    printf("Please enter player's name:\n");
    gets(tempName);
    int state = 0;

    Player temp;
    while (fread(&temp,sizeof(temp),1 , user_data) == 1)
    {
        if (strcmp(temp.name ,tempName) == 0)
        {
            printf("Wrong input.This name already exists!");
            Sleep(2000);
            state = 1;           
        }
        
    }
    if (state == 1)
    {
        fseek(user_data,0,SEEK_SET);
        return newUser(user_data);
    }
    /// end of process of checking existence


    //// process of making newuser
    Player newPlayer;
    newPlayer.coin = 0;
    strcpy(newPlayer.name,tempName);
    fseek(user_data,0,SEEK_END);   // for preventing bug(if everything goes correct , user_data is automatic at end)
    fwrite(&newPlayer,sizeof(newPlayer),1,user_data);  
    
    return newPlayer;
    
}

Ship *putship(Player *player , int map_selected_size[2],int ship_selected[21]){
    
    //// initialize
    player->player_map.map_size[0] = map_selected_size[0];
    player->player_map.map_size[1] = map_selected_size[1];
    for (size_t i = 0; i < map_selected_size[0] ; i++)
    {
        for (size_t j = 0; j < map_selected_size[1]; j++)
        {
            player->player_map.turn_map[i][j] = ' ';  //Unknown
            player->player_map.full_map[i][j] = 'N';  //None
        }
    }
    int num_ships = 0;
    for (size_t i = 0; i < 21; i++)
    {
        num_ships += ship_selected[i];
    }
    //// end initialize
    


    Ship *current = player->head;
    int count = 0; /// to find out the ship number 

    for (size_t j = 0; j < 21; j++)
    {
        if (ship_selected[j] != 0)
        {
            for (size_t i = 0; i < ship_selected[j]; i++)
            {
                count++;
                system("cls");
                printf("size of map : %d * %d\t total ships: %d",map_selected_size[0] , map_selected_size[1],num_ships);
                get_info_ship(count,j,current);
                first_place_make(current ,count , player);
                make_list();    
                //current = current->next; malloc
            }
            

            
            
        }
        
    }
    

    


}    

void get_info_ship(int count , int j , Ship *current){
    
    printf("Set ship %d-th with size %d.\n",count,j);
    printf("Do you want to put this ship vertical(v) or horizontal(h)?\n");
    scanf(" %c",&(current->vrt_or_hrzt));
    printf("Enter start of ship with size %d:(x,y)\n",j);
    scanf("(%d,%d)",&current->cordinates[0].x , &current->cordinates[0].y);  // start
    printf("Enter end of this ship:\n");
    scanf("(%d,%d)",&current->cordinates[1].x , &current->cordinates[1].y);  // end
    printf("Ship %d-th completed.\n",count);    
}

void first_place_make(Ship *current , int count,Player *player){
    int chk_res = check(current,count,player);


}

int check(Ship *current , int count , Player *player){
    if (current->vrt_or_hrzt == 'h')
    {
        if (current->cordinates[0].x < 1 || current->cordinates[1].x > player->player_map.map_size[0] || current->cordinates[0].y < 1 || current->cordinates[0].y > player->player_map.map_size[1])
        {
            printf("Can't place!");
            return 0;  // 0 means failed            
        }

        if (current->cordinates[0].y != current->cordinates[1].y)
        {
            printf("The cordinates aren't horizontal.");
            return 0;
        }
        int x = current->cordinates[0].x; // x & y for checing the parts of ship
        int y = current->cordinates[0].y;


        while (x <= current->cordinates[1].x)
        {
            for (size_t i = 1; i <= player->player_map.map_size[0]; i++)
            {
                for (size_t j = 1; j <= player->player_map.map_size[1]; j++)
                {    
                    if (pow(abs(x - i) , 2) + pow(abs(y - j) , 2) <= 2)
                    {
                        if (player->player_map.full_map[i][j] != 'N')
                        {
                            return 0;   // 0 means failed
                        }
                    }  
                }
            }
            x++;
        
        }
        return 1; // 1 means true
        

    }
    else
    {   
        if (current->cordinates[0].y < 1 || current->cordinates[1].y > player->player_map.map_size[1] || current->cordinates[0].x < 1 || current->cordinates[0].x > player->player_map.map_size[0])
        {
            printf("Can't place!");
            return 0;  // 0 means failed            
        }        
        if (current->cordinates[0].x != current->cordinates[1].x)
        {
            printf("The cordinates aren't vertical.");
            return 0;
        }
        
        int x = current->cordinates[0].x; // x & y for checing the parts of ship
        int y = current->cordinates[0].y;


        while (y <= current->cordinates[1].y)
        {
            for (size_t i = 1; i <= player->player_map.map_size[0]; i++)
            {
                for (size_t j = 1; j <= player->player_map.map_size[1]; j++)
                {    
                    if (pow(abs(x - i) , 2) + pow(abs(y - j) , 2) <= 2)
                    {
                        if (player->player_map.full_map[i][j] != 'N')
                        {
                            return 0;   // 0 means placable
                        }
                    }  
                }
            }
            y++;
        
        }
        return 1; // 1 means unplacable        


    }
    
}



make_list(){

}    

