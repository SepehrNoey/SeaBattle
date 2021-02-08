#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
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
typedef struct Map Map;

struct Ship
{
    int ship_size;
    char expld_or_not[21][21];
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
typedef struct Game_Data Game_Data;

Ship *rand_putship(Player *player , int map_selected_size[2] , int ship_selected[21]);
void get_info_ship(int count , int length , Ship *current , int map_selected_size[2]);
void get_info_ship_rand(int count , int length , Ship *current , int map_selected_size[2]);
int menu(Player *player1 , Player *player2 , int choice ,FILE * user_data , int map_selected_size[2] , int ship_selected[21]);
Player newUser(FILE * user_data);
Player player_set(FILE * user_data);
Player player_from_list(FILE * user_data , int *state);  
Ship *putship(Player *player , int map_selected_size[2],int ship_selected[21]);
int is_placable(Ship *current , int count, Player *player , int length);
void place(Ship *current , Player *player , int length);

int main(void){  // 10 * 10

    FILE * user_data = fopen("user_data.sb","rb+");  /// loading user_data
    FILE * last_game_data = fopen("last_game_data.sb","rb+");  // loading last game
    if (user_data == NULL)
    {
        user_data = fopen("user_data.sb","wb+"); // it's the first run
    }
    if (last_game_data == NULL)
    {
        last_game_data = fopen("last_game_data.sb","wb+");
    }

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
    int ship_selected[21] = {0,4,3,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  // the value is the number and index is the size
    int start_or_not = 0;

    while (choice != 7 && start_or_not == 0)
    {
        menu(&player1,&player2,choice , user_data , map_selected_size , ship_selected);
        if(choice == 6) /// hala ye seri kar ha bayad bokonim  == 6 or != 6 (exit va save kardan)
        {

        }
    }



    fclose(user_data);
    return 0;


}

int menu(Player *player1 , Player *player2 , int choice ,FILE * user_data , int map_selected_size[2] , int ship_selected[21]){

    printf("1) Play with a Friend\n2) Play with \"Captain Bot\"\n3) Load game\n4) Load last game\n5) Settings\n6) Score Board\n7) Exit\n");

    scanf("%d",&choice);
    if (choice == 7)
    {
        return choice;
    }

    ///playing function calls

    if (choice == 1)
    {
        *player1 = player_set(user_data);
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
        fseek(user_data,0,SEEK_SET);
        *player2 = player_set(user_data);
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

        return 1;  // 1 means start game
    }
    else if (choice == 2)
    {
        /* code */
    }
    else if (choice == 3)
    {
        /* code */
    }
    else if (choice == 4)
    {
        /* code */
    }
    else if (choice == 5)
    {
        /* code */
    }
    else if (choice == 6)
    {
        /* code */
    }
    




}

Player player_set(FILE * user_data) {
    int innerChoice;
    system("cls");
    printf("Choose player:\n\n1) Choose from available users\n2) New user\n");
    scanf("%d",&innerChoice);
    int state = 0;

    while (state == 0)
    {
        if (innerChoice == 1)
        {
            return player_from_list(user_data , &state);  // returns player from the list of players
        }
        else if (innerChoice == 2)
        {
            state = 1;
            return newUser(user_data);           // newplayer
        }
        else
        {
            puts("Invalid input.Try again");  
            
        }
    }
}

Player player_from_list(FILE * user_data , int *state){
    Player temp;
    int num = 0;
    while (fread(&temp , sizeof(temp) , 1 , user_data) == 1)
    {
        num++;
        printf("%d) Player name: %s\tCoins: %d\n",num,temp.name,temp.coin);
    }
    if (num == 0)
    {
        system("cls");
        printf("Sorry.No users yet!\n");
        Sleep(2000);
        system("cls");
        *state = 0;
        return;
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
        
    fseek(user_data,choice - 1,SEEK_SET);
    int res = fread(&temp,sizeof(temp),1,user_data);
    
    if (res == 1)  // if found
    {
        temp.head = NULL;        
        return temp;
    }
    else
    {
        printf("ERROR occurred in reading.\n");
        *state = 0;
        return;
    }
}


Player newUser(FILE * user_data){

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
        return newUser(user_data);
    }
    /// end of process of checking existence


    //// process of making newuser
    Player newPlayer;
    newPlayer.coin = 0;
    newPlayer.head = (Ship *)malloc(sizeof(Ship));
    strcpy(newPlayer.name,tempName);
    fseek(user_data,0,SEEK_END);   // for preventing bug(if everything goes correct , user_data is automatic at end)
    fwrite(&newPlayer,sizeof(newPlayer),1,user_data);

    return newPlayer;

}

Ship *putship(Player *player , int map_selected_size[2],int ship_selected[21]){

    //// initialize
    player->player_map.map_size[0] = map_selected_size[0];
    player->player_map.map_size[1] = map_selected_size[1];
    for (size_t i = 0; i <= map_selected_size[0] ; i++)
    {
        for (size_t j = 0; j <= map_selected_size[1]; j++)
        {
            player->player_map.turn_map[i][j] = ' ';  //Unknown
            player->player_map.full_map[i][j] = 'W';  //Water
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
    int state = 0;

    for (size_t j = 0; j < 21; j++) // max number of ships is 20
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
                        free(current);
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
}

void get_info_ship(int count , int length , Ship *current , int map_selected_size[2]){
    for (size_t i = 1; i <= map_selected_size[0]; i++)
    {
        for (size_t j = 1; j <= map_selected_size[1]; j++)
        {
            current->expld_or_not[i][j] = 'H';  // H means healthy  // there are many unneeded elements
        }

    }
    current->next = NULL;
    current->ship_size = length;

    printf("Set ship %d-th with size %d.\n",count,length);
    printf("Do you want to put this ship vertical(v) or horizontal(h)?\n");
    getchar();
    scanf("%c",&(current->vrt_or_hrzt));
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
Ship *rand_putship(Player *player , int map_selected_size[2] , int ship_selected[21]){

    //// initialize
    player->player_map.map_size[0] = map_selected_size[0];
    player->player_map.map_size[1] = map_selected_size[1];
    for (size_t i = 0; i < map_selected_size[0] ; i++)
    {
        for (size_t j = 0; j < map_selected_size[1]; j++)
        {
            player->player_map.turn_map[i][j] = ' ';  //Unknown
            player->player_map.full_map[i][j] = 'W';  //Water
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
    int state = 0;

    for (size_t j = 0; j < 21; j++)
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
                        free(current);
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

}

void get_info_ship_rand(int count , int length , Ship *current , int map_selected_size[2]){
    for (size_t i = 1; i <= map_selected_size[0]; i++)
    {
        for (size_t j = 1; j <= map_selected_size[1]; j++)
        {
            current->expld_or_not[i][j] = 'H';  // H means healthy  // there are many unneeded elements
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
            player->player_map.full_map[i][current->cordinates[0].y] = 'H';   // setting ships //default condition of ships is Healthy
        }

    }
    else
    {
        for (size_t i = current->cordinates[0].y; i <= current->cordinates[1].y; i++)
        {
            player->player_map.full_map[current->cordinates[0].x][i] = 'H';
        }
    }
}