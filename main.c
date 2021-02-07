#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#define MAX_SHIP_LENGTH 20

struct Map
{
    int map_size[2];// idx[0] -> x  // idx[1] -> y
    char turn_map[MAX_SHIP_LENGTH][MAX_SHIP_LENGTH];
    char res_map[MAX_SHIP_LENGTH][MAX_SHIP_LENGTH];

};
typedef Map Map;

struct Ship
{
    int ship_size;
    char expld_or_not[MAX_SHIP_LENGTH];
    char vrt_or_hrzt;
    int cordinates[2];   // idx[0] -> start  // idx[1] -> end
    int map_size[2];     // idx[0] -> x  // idx[1] -> y 
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


int menu(int choice ,FILE * user_data);
Player newUser(FILE * user_data);
Player player_set(FILE * user_data);
Player player_from_list(FILE * user_data);  // return type is the coins of the player




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
    while (choice != 6)
    {
        menu(choice , user_data);
        if(choice == 6) /// hala ye seri kar ha bayad bokonim  == 6 or != 6 (exit va save kardan)
        {

        }    
    }
    
        

    fclose(user_data);
    return 0;


}

int menu(int choice ,FILE * user_data){
    
    printf("1) Play with a Friend\n2) Play with \"Captain Bot\"\n3) Load last game\n4) Settings\n5) Score Board\n6) Exit\n");

    scanf("%d",&choice);
    if (choice == 6)
    {
        return choice;
    }
    
    ///playing function calls

    if (choice == 1)
    {
        Player player1 = player_set(user_data);  // return type is the coin of player
        putship();    
        
        fseek(user_data,0,SEEK_SET); 
        Player player2 = player_set(user_data);  // return type is the coin of player
        
    }

    
    
}

Player player_set(FILE * user_data) {  
    int innerChoice;
    system("cls");
    printf("Choose player:\n\n1) Choose from available users\n2) New user");
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