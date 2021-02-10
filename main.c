#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#define MAX_SHIP_LENGTH 21
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
typedef struct Map Map;

struct Ship
{
    int ship_size;
    char expld_or_not[MAX_SHIP_LENGTH][MAX_SHIP_LENGTH];
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
    char tag[20];
    char mode;  // bot or friend
    Player player1;
    Player player2;
    int turn ;     // to check who's turn is

};
typedef struct Game_Data Game_Data;

Ship *rand_putship(Player *player , int map_selected_size[2] , int ship_selected[MAX_SHIP_LENGTH]);
void get_info_ship(int count , int length , Ship *current , int map_selected_size[2]);
void get_info_ship_rand(int count , int length , Ship *current , int map_selected_size[2]);
int menu(Player *player1 , Player *player2 , int choice , int map_selected_size[2] , int ship_selected[MAX_SHIP_LENGTH]);
Player newUser();
Player player_set(void);
Player player_from_list(int *state);  
Ship *putship(Player *player , int map_selected_size[2],int ship_selected[MAX_SHIP_LENGTH]);
int is_placable(Ship *current , int count, Player *player , int length);
void place(Ship *current , Player *player , int length);
void play_with_friend(Player *player1, Player *player2 , int turn_maker);
void showmap(Player de_turn_player);
void save(Player player1 , Player player2 , int turn_maker);
int move(Player **de_turn_player , int *extraCoin , Ship *current);
int coin_maker(Player *turn_player , int move_res , int extraCoin);
void delete_ship(Ship **current ,Ship *last_ship ,  Player **player);
FILE *open(char name[20]);
void update_map(Player *de_turn_player ,Ship *current, int x , int y);
Game_Data load();
void settings(int ship_selected[MAX_SHIP_LENGTH],int map_selected_size[2]);
void showturn(Player *turn_player , Player *player1 , Player *player2);
void Score_board(void);
void save_user(Player * player);
int cmpfunc(const Player *a , const Player *b);

int main(void){  
    
    /////
    system("cls");
    puts("Welcome to SEA BATTLE!!\nIf you are ready , select one of the following choices.");  // start of program
    Sleep(3500);
    system("cls");
    /////

    Player player1;
    Player player2;

    int choice = 0;
    int map_selected_size[2] = {10,10};
    int ship_selected[MAX_SHIP_LENGTH] = {0,4,3,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  // the value is the number and index is the size

    while (choice != 7)
    {
        menu(&player1,&player2,choice , map_selected_size , ship_selected);
        if(choice == 7) /// hala ye seri kar ha bayad bokonim  == 6 or != 6 (exit va save kardan)
        {
            
        }
    }



    return 0;


}

int menu(Player *player1 , Player *player2 , int choice ,int map_selected_size[2] , int ship_selected[MAX_SHIP_LENGTH]){ 

    printf("1) Play with a Friend\n2) Play with \"Captain Bot\"\n3) Load game\n4) Load last game\n5) Settings\n6) Score Board\n7) Exit\n");
    scanf("%d",&choice);

    ///playing function calls

    if (choice == 1)
    {
        *player1 = player_set();
        system("cls");
        printf("Now , put ships for this player.\n\n");
        printf("1) Auto\n2) Manual\n");
        int innerChoice;
        scanf("%d",&innerChoice);
        if (innerChoice == 1)
        {
            player1->head = rand_putship(player1 , map_selected_size, ship_selected);
        }
        else if (innerChoice == 2)
        {
            player1->head = putship(player1 , map_selected_size,ship_selected);
        }
        
        printf("Now , second player:\n");
        Sleep(1500);
        *player2 = player_set();
        system("cls");
        printf("Now , put ships for this player.\n\n");
        printf("1) Auto\n2) Manual\n");
        scanf("%d",&innerChoice);
        if (innerChoice == 1)
        {
            player2->head = rand_putship(player2 , map_selected_size,ship_selected);
        }
        else if (innerChoice == 2)
        {
            player2->head = putship(player2 , map_selected_size,ship_selected);
        }

        play_with_friend(player1 , player2 , 0);
    }
    else if (choice == 2)
    {
        *player1 = player_set();
        system("cls");
        
    }
    else if (choice == 3)
    {
        Game_Data loaded = load();
        if (loaded.mode == 'b') 
        {
        //    play_with_bot();
        }
        else
        {
            int turn_maker = loaded.turn == 1 ? 0 : 1;
            play_with_friend(&loaded.player1 , &loaded.player2 ,turn_maker);
        }
    }
    else if (choice == 4)
    {
        Game_Data loaded;
        FILE * last_game_data = open("last_game_data.sb");
        int state = fread(&loaded , sizeof(Game_Data) , 1 , last_game_data);
        if (state != 1)
        {
            printf("Sorry.No data yet!\n");
            Sleep(2000);
        }
        else if (loaded.mode == 'b')
        {
        //    play_with_bot();
        }
        else
        {
            int turn_maker = loaded.turn == 1 ? 0 : 1;
            play_with_friend(&loaded.player1,&loaded.player2 , turn_maker);
        }
    }
    else if (choice == 5)
    {
        //settings(ship_selected,map_selected_size);
    }
    else if (choice == 6)
    {
        Score_board();
    }
    else if (choice == 7)
    {
        exit(0);
    }
}

Player player_set(void) {
    int innerChoice;
    system("cls");
    printf("Choose player:\n\n1) Choose from available users\n2) New user\n");
    scanf("%d",&innerChoice);
    int state = 0;

    while (state == 0)
    {
        if (innerChoice == 1)
        {
            return player_from_list(&state);  // returns player from the list of players
        }
        else if (innerChoice == 2)
        {
            state = 1;
            return newUser();           // newplayer
        }
        else
        {
            puts("Invalid input.Try again");  
            
        }
    }
}

Player player_from_list(int *state){
    Player temp;
    int num = 0;
    FILE * user_data = open("user_data.sb");
    while (fread(&temp , sizeof(temp) , 1 , user_data) == 1)
    {
        num++;
        printf("%d) Player name: %s\tCoins: %d\n",num,temp.name,temp.coin);
    }
    if (num == 0)
    {
        system("cls");
        printf("Sorry.No users yet!\n");
        printf("So, let's make a new user:\n");
        Sleep(2000);
        system("cls");
        *state = 0;
        fclose(user_data);
        return newUser();
    }
    
    puts("Please enter the number of player you want:");
    int choice = 0;

    while (choice <= 0 || choice > num)
    {
        scanf("%d",&choice);
        if (choice <= 0 || choice > num)
        {
            puts("Invalid input.Try again.");
        }
    }
        
    fseek(user_data,(choice - 1) * sizeof(temp),SEEK_SET);
    int res = fread(&temp,sizeof(temp),1,user_data);
    
    if (res == 1)  // if found
    {
        temp.head = NULL;
        fclose(user_data);        
        return temp;
    }
    else
    {
        printf("ERROR occurred in reading.\n");
        *state = 0;
        fclose(user_data);
        return;
    }
}


Player newUser(){
    FILE * user_data = open("user_data.sb");
    /// start of process of checking existence
    system("cls");
    char tempName[50];
    printf("Please enter player's name:\n");
    getchar();
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
        fclose(user_data);
        return newUser();
    }
    /// end of process of checking existence


    //// process of making newuser
    Player newPlayer;
    newPlayer.coin = 0;
    newPlayer.head = (Ship *)malloc(sizeof(Ship));
    strcpy(newPlayer.name,tempName);
    fseek(user_data,0,SEEK_END);   // for preventing bug(if everything goes correct , user_data is automatic at end)
    fwrite(&newPlayer,sizeof(newPlayer),1,user_data);
    fclose(user_data);
    return newPlayer;

}

Ship *putship(Player *player , int map_selected_size[2],int ship_selected[MAX_SHIP_LENGTH]){

    //// initialize
    player->player_map.map_size[0] = map_selected_size[0];
    player->player_map.map_size[1] = map_selected_size[1];
    for (size_t i = 0; i <= map_selected_size[0] ; i++)
    {
        for (size_t j = 0; j <= map_selected_size[1]; j++)
        {
            player->player_map.turn_map[i][j] = '-';  //Unknown
            player->player_map.full_map[i][j] = 'W';  //Water
        }
    }
    int num_ships = 0;
    for (size_t i = 0; i < MAX_SHIP_LENGTH; i++)
    {
        num_ships += ship_selected[i];
    }
    //// end initialize



    Ship *current = player->head;
    int count = 0; /// to find out the ship number
    int state = 0;

    for (size_t j = 0; j < MAX_SHIP_LENGTH; j++) // max number of ships is 20
    {
        if (ship_selected[j] != 0)
        {
            for (size_t i = 0; i < ship_selected[j]; i++)
            {
                count++;
                system("cls");
                printf("size of map : %d * %d\t total ships: %d\t",map_selected_size[0] , map_selected_size[1],num_ships);
                Ship *temp = current;
                if (count != 1)
                {
                    current = (Ship *)malloc(sizeof(Ship));
                    if (current == NULL)
                    {
                        printf("Couldn't allocate memeory.");

                    }
                }
                get_info_ship(count,j,current,map_selected_size);
                state = is_placable(current ,count , player , j);
                if (state != 1)
                {
                    i--;
                    count--;
                    state = 1;
                    if (current != player->head)
                    {
                        //free(current);
                        current = temp;
                    }

                }
                else
                {
                    temp->next = current;
                    place(current , player,j);
                }
            }
        }
    }
    return player->head;
}

void get_info_ship(int count , int length , Ship *current , int map_selected_size[2]){
    for (size_t i = 1; i <= map_selected_size[0]; i++)
    {
        for (size_t j = 1; j <= map_selected_size[1]; j++)
        {
            current->expld_or_not[i][j] = 'S';  // S(ship) means healthy  // there are many unneeded elements
        }

    }
    current->next = NULL;
    current->ship_size = length;

    printf("Set ship %d-th with size %d.\n",count,length);
    printf("Do you want to put this ship vertical(v) or horizontal(h)?\n");
    getchar();
    while (1)
    {
        scanf("%c",&(current->vrt_or_hrzt));
        if (current->vrt_or_hrzt == 'h' || current->vrt_or_hrzt == 'v')
        {
            break;
        }
        printf("Wrong input.Try again.\n");
    }
    printf("Enter start of ship with size %d: x,y\n",length);
    getchar();
    scanf("%d,%d",&current->cordinates[0].x , &current->cordinates[0].y);  // start
    printf("Enter end of this ship:\n");
    getchar();
    scanf("%d,%d",&current->cordinates[1].x , &current->cordinates[1].y);  // end
    printf("Ship %d-th completed.\n",count);
    Sleep(1500);

}


int is_placable(Ship *current , int count , Player *player , int length){
    if (current->vrt_or_hrzt == 'h')
    {
        if (current->ship_size != current->cordinates[1].x - current->cordinates[0].x + 1)
        {
            printf("Wrong input.Size didn't match.\n");
            Sleep(2000);
            return 0;
        }
        if (current->cordinates[0].x < 1 || current->cordinates[1].x > player->player_map.map_size[0] || current->cordinates[0].y < 1 || current->cordinates[0].y > player->player_map.map_size[1])
        {
            printf("Can't place!");
            Sleep(2000);
            return 0;  // 0 means failed
        }

        if (current->cordinates[0].y != current->cordinates[1].y)
        {
            printf("The cordinates aren't horizontal.");
            Sleep(2000);
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
                        if (player->player_map.full_map[i][j] != 'W')
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
        if (current->ship_size != current->cordinates[1].y - current->cordinates[0].y + 1)
        {
            printf("Wrong input.Size didn't match.\n");
            Sleep(2000);
            return 0;
        }

        if (current->cordinates[0].y < 1 || current->cordinates[1].y > player->player_map.map_size[1] || current->cordinates[0].x < 1 || current->cordinates[0].x > player->player_map.map_size[0])
        {
            printf("Can't place!");
            Sleep(2000);
            return 0;  // 0 means failed
        }
        if (current->cordinates[0].x != current->cordinates[1].x)
        {
            printf("The cordinates aren't vertical.");
            Sleep(2000);
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
                        if (player->player_map.full_map[i][j] != 'W')
                        {
                            printf("Isn't placable here!");
                            Sleep(2000);
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
Ship *rand_putship(Player *player , int map_selected_size[2] , int ship_selected[MAX_SHIP_LENGTH]){

    //// initialize
    player->player_map.map_size[0] = map_selected_size[0];
    player->player_map.map_size[1] = map_selected_size[1];
    for (size_t i = 0; i < map_selected_size[0] ; i++)
    {
        for (size_t j = 0; j < map_selected_size[1]; j++)
        {
            player->player_map.turn_map[i][j] = '-';  //Unknown
            player->player_map.full_map[i][j] = 'W';  //Water
        }
    }
    int num_ships = 0;
    for (size_t i = 0; i < MAX_SHIP_LENGTH; i++)
    {
        num_ships += ship_selected[i];
    }
    //// end initialize

    Ship *current = player->head;
    int count = 0; /// to find out the ship number
    int state = 0;

    for (size_t j = 0; j < MAX_SHIP_LENGTH; j++)
    {
        if (ship_selected[j] != 0)
        {
            for (size_t i = 0; i < ship_selected[j]; i++)
            {
                count++;
                Ship *temp = current;
                if (count != 1)
                {
                    current = (Ship *)malloc(sizeof(Ship));
                    if (current == NULL)
                    {
                        printf("Couldn't allocate memeory.");

                    }
                }
                get_info_ship_rand(count,j,current,map_selected_size);
                state = is_placable(current,count,player , j);
                if (state != 1)
                {
                    i--;
                    count--;
                    state = 1;
                    if (current != player->head)
                    {
                        //free(current);
                        current = temp;
                    }
                }
                else
                {
                    temp->next = current;
                    place(current , player,j);
                }

            }
        }
    }
    return player->head;

}

void get_info_ship_rand(int count , int length , Ship *current , int map_selected_size[2]){
    for (size_t i = 1; i <= map_selected_size[0]; i++)
    {
        for (size_t j = 1; j <= map_selected_size[1]; j++)
        {
            current->expld_or_not[i][j] = 'S';  // H means healthy  // there are many unneeded elements
        }

    }
    current->next = NULL;
    current->ship_size = length;
    srand(time(0));

    current->vrt_or_hrzt = rand() % 2 ? 'h': 'v';
    if (current->vrt_or_hrzt == 'h')
    {
        current->cordinates[0].y = (rand() % map_selected_size[1]) + 1;
        current->cordinates[1].y = current->cordinates[0].y;
        int state = 0;
        int x;
        while (state == 0)
        {
            x = rand() % map_selected_size[0] + 1;
            if (x + length - 1 <= map_selected_size[0])
            {
                state = 1;
            }
        }
        current->cordinates[0].x = x;
        current->cordinates[1].x = x + length - 1;
    }
    else
    {
        current->cordinates[1].x = (rand() % map_selected_size[0]) + 1;
        current->cordinates[0].x = current->cordinates[1].x;
        int state = 0;
        int y;
        while (state == 0)
        {
            y = rand() % map_selected_size[1] + 1;
            if (y + length - 1 <= map_selected_size[1])
            {
                state = 1;
            }
        }
        current->cordinates[0].y = y;
        current->cordinates[1].y = y + length - 1;
    }
}

void place(Ship *current , Player *player , int length){
    if (current->vrt_or_hrzt == 'h')  // horizontal
    {
        for (size_t i = current->cordinates[0].x; i <= current->cordinates[1].x ; i++)
        {
            player->player_map.full_map[i][current->cordinates[0].y] = 'S';   // set ships //default condition of ships is Healthy
        }

    }
    else
    {
        for (size_t i = current->cordinates[0].y; i <= current->cordinates[1].y; i++)
        {
            player->player_map.full_map[current->cordinates[0].x][i] = 'S';
        }
    }
}
void play_with_friend(Player * player1, Player * player2 , int turn_maker){
    Player *turn_player;
    Player *de_turn_player;
    while (player1->head != NULL && player2->head != NULL)
    {
        turn_maker++;
        turn_player = turn_maker % 2 ? (player1) : (player2);
        de_turn_player = turn_maker % 2 ? (player2) : (player1);
        showturn(turn_player,player1 , player2);
        showmap(*de_turn_player);
        save(*player1,*player2 ,turn_maker);
        int extraCoin = 0;
        Ship *current = de_turn_player->head;
        int move_res = move(&de_turn_player,&extraCoin , current);
        turn_maker += coin_maker(turn_player,move_res,extraCoin);  // it's also turn_maker
        showmap(*de_turn_player);
        printf("\nPress 1 for continue.\n");
        int choice;
        scanf("%d",&choice);
        system("cls");
    }
    Sleep(2000);
    if (player1->head == NULL)
    {
        system("cls");
        printf("\n%s wins!\n\n",player2->name);
    }
    else
    {
        system("cls");
        printf("\n%s wins!\n\n",player1->name);
    }
    printf("(player1: %d)(player2 : %d)\n",player1->coin , player2->coin);
    printf("(player1: %s)(player2 : %s)\n",player1->name , player2->name);
    
}

void showmap(Player de_turn_player){
    int x = de_turn_player.player_map.map_size[0];
    int y = de_turn_player.player_map.map_size[1];
    for (size_t i = y; i >= 1; i--)             
    {
        int column = 1;
        for (size_t k = 1; k <= x; k++) 
        {
            printf("|%c|",de_turn_player.player_map.turn_map[column][i]);  // mitunim bejaye space chiz behtari bezarim
            column++;
        }
        printf("\n");
    }   
}
void save(Player player1 , Player player2 , int turn_maker){
    FILE * game_data = open("game_data.sb");
    FILE * last_game_data = open("last_game_data.sb");
    printf("\n1) Continue\n2) Save and continue\n3) Save and exit\n");
    int choice;
    scanf("%d",&choice);
    if (choice == 2 || choice == 3)
    {
        save_user(&player1);
        save_user(&player2);
        printf("Ok,enter a name for saving this match:\n");
        getchar();
        Game_Data temp;
        temp.mode = 'f';  // freind
        gets(temp.tag);
        temp.player1 = player1;
        temp.player2 = player2;
        temp.turn = turn_maker % 2 ? 1 : 2;        
        
        fseek(last_game_data , 0 , SEEK_SET);
        fwrite(&temp , sizeof(temp) , 1 , last_game_data);
        fclose(last_game_data);
        
        Game_Data temp2;

        while (fread(&temp2 , sizeof(temp) , 1 , game_data) == 1)
        {
            if (strcmp(temp2.tag , temp.tag) == 0)  // checking existing data with same tag
            {
                fseek(game_data, -1 * sizeof(Game_Data) , SEEK_CUR);
                fwrite(&temp , sizeof(temp) , 1 , game_data);
                printf("Saved successfully!\n");
                if (choice == 3)
                {
                    fclose(game_data);
                    Sleep(1500);
                    exit(0);
                }
                fclose(game_data);
                return;
            }
            
        }
        fseek(game_data , 0 , SEEK_END);
        fwrite(&temp , sizeof(temp) , 1 , game_data);
        printf("Saved successfully!\n");
        if (choice == 3)
        {
            fclose(game_data);
            Sleep(1500);
            exit(0);
        }        
        fclose(game_data);
        return;
        
    }
    fclose(game_data);
    fclose(last_game_data);
}
int move(Player **de_turn_player , int *extraCoin , Ship * current){   // three kind of returns : 3 -> Complete explosion      2 -> just explosion        0 -> water or choosen before   (all for giving coin)
    printf("\nPlease enter a cordinate to attack: x,y\n");
    int x , y;
    fflush(stdin);
    while (1)
    {
        scanf("%d,%d" , &x ,&y);
        if (!(x < 1 || x > (*de_turn_player)->player_map.map_size[0] || y < 1 || y > (*de_turn_player)->player_map.map_size[1]))
        {
            break;
        }
        printf("Invalid input.Try again.\n");
    }
    
    printf("\n ...\n ...\n");
    Sleep(2500);
    system("cls");
    Ship * last_ship = current;


    if ((*de_turn_player)->player_map.full_map[x][y] == 'W')//water
    {
        (*de_turn_player)->player_map.turn_map[x][y] = 'W';
        printf("Water :(\n");
        return 0;
    }
    else if ((*de_turn_player)->player_map.full_map[x][y] == 'C' || (*de_turn_player)->player_map.full_map[x][y] == 'E') //choosen before
    {
        printf("Can't choose.It has been choosen before!\n");
        Sleep(2000);
        return 0;
    }
    else if ((*de_turn_player)->player_map.full_map[x][y] == 'S')  // Ship(healthy part)
    {
        while (current != NULL)
        {
            if (current->vrt_or_hrzt == 'h')  // horizontal
            {
                if (x >= current->cordinates[0].x && x <= current->cordinates[1].x && y == current->cordinates[0].y)  // found
                {
                    if (current->ship_size == 1)
                    {
                        update_map(*de_turn_player ,current, x , y); // all maps (except E ones)are updated in update_map(but we should call this function length times) but expld updates here
                        printf("Ship destroyed :)\n");
                        int tempSize = current->ship_size;
                        delete_ship(&current,last_ship,de_turn_player);
                        *extraCoin = (int)(25 / tempSize);  
                        return 3;
                    }
                    else
                    {                        
                        current->expld_or_not[x][y] = 'E';
                        (*de_turn_player)->player_map.turn_map[x][y] = 'E';  // updating E ones
                        int state = 1; // default -> complete explosion
                        for (size_t i = current->cordinates[0].x; i <= current->cordinates[1].x ; i++)
                        {
                            if (current->expld_or_not[i][y] == 'S')
                            {
                                state = 0;  // it's not comlete explosion
                                break;
                            }
                        }
                        if (state == 1)
                        {
                            for (size_t i = current->cordinates[0].x; i <= current->cordinates[1].x; i++)
                            {
                                update_map(*de_turn_player,current,i,y);
                            }
                            printf("Ship destroyed :)\n");
                            int tempSize = current->ship_size;
                            delete_ship(&current,last_ship,de_turn_player);
                            *extraCoin = (int)(25 / tempSize);  
                            return 3;
                        }
                        else
                        {
                            (*de_turn_player)->player_map.turn_map[x][y] = 'E';
                            printf("Caused explosion :)\n");
                            return 2;
                        }
                    }
                }
            }
            else  // vertical
            {
                if (y >= current->cordinates[0].y && y <= current->cordinates[1].y && x == current->cordinates[0].x)  // found
                {
                    if (current->ship_size == 1)
                    {
                        update_map(*de_turn_player ,current, x , y); // all cordinates are updated in update_map(but we should call this function length times) but expld updates here
                        printf("Ship destroyed :)\n");
                        int tempSize = current->ship_size;
                        delete_ship(&current,last_ship,de_turn_player);
                        *extraCoin = (int) (25 / tempSize);
                        return 3;
                    }
                    else
                    {                        
                        current->expld_or_not[x][y] = 'E';
                        (*de_turn_player)->player_map.turn_map[x][y] = 'E';
                        int state = 1; // default -> complete explosion
                        for (size_t i = current->cordinates[0].y; i <= current->cordinates[1].y ; i++)
                        {
                            if (current->expld_or_not[x][i] == 'S')
                            {
                                state = 0;  // it's not comlete explosion
                                break;
                            }
                        }
                        if (state == 1)
                        {
                            for (size_t j = current->cordinates[0].y; j <= current->cordinates[1].y; j++)
                            {
                                update_map(*de_turn_player,current,x,j);
                            }
                            printf("Ship destroyed :)\n");
                            int tempSize = current->ship_size;
                            delete_ship(&current,last_ship,de_turn_player);
                            *extraCoin = (int)(25 / tempSize);  
                            return 3;
                        }
                        else
                        {
                            (*de_turn_player)->player_map.turn_map[x][y] = 'E';
                            printf("Caused explosion :)\n");
                            return 2;
                        }
                    }
                }                
            }
            last_ship = current;
            current = current->next;
        }
    }
}

void update_map(Player *de_turn_player ,Ship *current, int x , int y){ // is used just when complete explosion occurres
    for (size_t i = 1; i <=  de_turn_player->player_map.map_size[0]; i++)
    {
        for (size_t j = 1; j <= de_turn_player->player_map.map_size[1]; j++)
        {
            if (i == x && y == j)
            {
                    de_turn_player->player_map.full_map[x][y] = 'C';
                    de_turn_player->player_map.turn_map[x][y] = 'C';
                
            }
            if (current->vrt_or_hrzt == 'h')
            {
                if (pow(abs(x - i) , 2) + pow(abs(y - j) , 2) <= 2 && (i < current->cordinates[0].x || i > current->cordinates[1].x || j < current->cordinates[0].y || j > current->cordinates[0].y))
                {
                    de_turn_player->player_map.turn_map[i][j] = 'W';
                }
            }
            else
            {
                if (pow(abs(x - i) , 2) + pow(abs(y - j) , 2) <= 2 && (j < current->cordinates[0].y || j > current->cordinates[1].y || i < current->cordinates[0].x || i > current->cordinates[0].x))
                {
                    de_turn_player->player_map.turn_map[i][j] = 'W';
                }                
            }
        }
    }
}
int coin_maker(Player *turn_player , int move_res , int extraCoin){// three kind of returns : 3 -> Complete explosion      2 -> just explosion        0 -> water or choosen before   (all for giving coin)
    switch (move_res)
    {
    case 3:
        turn_player->coin += extraCoin + 1;
        return -1;  // for making turn
    case 2:
        turn_player->coin += 1;
        return -1;
    case 0:
        return 0;
    }    
}

void delete_ship(Ship **current ,Ship *last_ship ,  Player **player){
    if (last_ship == *current) // deleting the head
    {
        (*player)->head = (*player)->head->next;  // correct?
        *current = (*current)->next;  // now current is the next node
        last_ship->next = NULL;
        //free(last_ship);
    }
    else 
    {
        last_ship->next = (*current)->next;
        (*current)->next = NULL;
        *current = last_ship;  // now current is the node before
        //free(current);
    }
}

FILE *open(char name[20]){
    FILE *fpr = fopen(name, "rb+");
    if (fpr == NULL)
    {
        fpr = fopen(name,"wb+");
    }
    return fpr;
    
}

Game_Data load(){
    FILE *game_data = open("game_data.sb");
    Game_Data data ;
    system("cls");
    printf("List of saved games:\n\n");
    int state = 0;
    int num = 0;
    while(fread(&data,sizeof(Game_Data),1,game_data) == 1);
    {
        state = 1;
        num++;
        char mode[20] ;
        if (data.mode == 'b')
            strcpy(mode, "With \"Captain Bot\"");
        else
            strcpy(mode,"With Friend");        

        printf("%d) Game tag : %s\tMode : %s\n",num, data.tag ,mode);

    }
    if (state  == 1)
    {
        int choice;
        printf("Please enter the number of game you want:\n");
        scanf("%d",&choice);
        if (choice <= 0 || choice > num){
            printf("Invalid input.");
            fclose(game_data);
            return load();
            
        }
        else
        {
            fseek(game_data , (choice - 1) * sizeof(Game_Data) , SEEK_SET);
            fread(&data , sizeof(Game_Data) , 1 , game_data);
            fclose(game_data);
            return data;
        }
    }
    else
    {
        printf("Sorry . No games yet!\n");
        Sleep(2000);
        fclose(game_data);
        return;
    }
    
    
}

void showturn(Player *turn_player ,Player *player1 , Player *player2){
    printf("Turn : %s\t\t%s coins : %d\t%s coins : %d\n\n",turn_player->name ,player1->name , player1->coin , player2->name , player2->coin);
}

/*void settings(int ship_selected[MAX_SHIP_LENGTH],int map_selected_size[2]){
    system("cls");
    printf("1) Ships\n2) Map size\n3) Theme\n\n");
    int choice;
    scanf("%d",&choice);
    if (choice == 1)
    {
        printf("\nAttention.Max size of ship can be 20 !\n");
        Sleep(2000);
        system("cls");
        int el_needed = 0;
        for (size_t i = 1; i <= 20; i++)
        {
            printf("Number of ships with size %d : \n", i);
            scanf("%d",&ship_selected[i]);
            el_needed += (ship_selected[i]
        }

        
        
    }
    
}*/

void Score_board(void){
    FILE * user_data = open("user_data.sb");
    Player * temp = (Player *)malloc(sizeof(Player));
    int num = 0;
    while (fread(&temp[num], sizeof(Player) , 1, user_data) == 1)
    {
        num++;
        temp = (Player *)realloc(temp,(num + 1)* sizeof(Player));
    }
    fclose(user_data);
    qsort(temp , num , sizeof(Player),cmpfunc);
    for (size_t i = 0; i < num; i++)
        printf("%d) %s\tcoins : %d\n",i + 1 , temp[i].name, temp[i].coin);

}
void save_user(Player * player){
    FILE * user_data = open("user_data.sb");
    Player temp;
    while (fread(&temp,sizeof(Player) , 1 , user_data) == 1)
    {
        if (strcmp(temp.name , player->name) == 0)
        {
            temp.coin += player->coin;
            fseek(user_data,-1 * sizeof(Player) , SEEK_CUR);
            fwrite(&temp,sizeof(Player) , 1, user_data);
            fclose(user_data);
            return;
        }
        
    }
    printf("No user , ERROR!\n");
    fclose(user_data);
}


int cmpfunc(const Player *a , const Player *b){  
    if (a->coin > b->coin)
        return -1;
    else if (a->coin < b->coin)
        return 1;
    else 
        return 0;    
}

