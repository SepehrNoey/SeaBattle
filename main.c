#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <string.h>

struct PLAYER
{
    char name[50];
    int coin;
};
typedef struct PLAYER PLAYER;


int menu(int choice ,FILE * data);
PLAYER player_set(FILE * data);
PLAYER player_from_list(FILE * data);  // return type is the coins of the player

int main(void){  // 10 * 10
    
    FILE * data = fopen("data.SB","rb+");  /// loading data
    
    /////
    system("cls");
    puts("Welcome to SEA BATTLE!!\nIf you are ready , select one of the following choices.");  // start of program
    Sleep(3500);
    system("cls");
    /////
    

    int choice = 0;
    while (choice != 6)
    {
        menu(choice , data);
        if(choice == 6) /// hala ye seri kar ha bayad bokonim  == 6 or != 6 (exit va save kardan)
        {

        }    
    }
    
        

    fclose(data);
    return 0;


}

int menu(int choice ,FILE * data){
    
    printf("1) Play with a Friend\n2) Play with \"Captain Bot\"\n3) Load last game\n4) Settings\n5) Score Board\n6) Exit\n");

    scanf("%d",&choice);
    if (choice == 6)
    {
        return choice;
    }
    
    ///playing function calls

    if (choice == 1)
    {
        PLAYER player1 = player_set(data);  // return type is the coin of player
        
        fseek(data,0,SEEK_SET); 
        
        PLAYER player2 = player_set(data);  // return type is the coin of player
        
    }
    
    
}

PLAYER player_set(FILE * data) {  
    int innerChoice;
    system("cls");
    printf("Choose player:\n\n1) Choose from available users\n2) New user");
    scanf("%d",&innerChoice);


    if (innerChoice == 1)
    {
        return player_from_list(data);  // black box // returns player from the list of players
    }
    else if (innerChoice == 2)
    {
        return newUser();  // black box // newplayer
    }
    else 
    {
        puts("Invalid input.Try again");  // bazgasht be porsesh dobare
    }        

}

PLAYER player_from_list(FILE * data){
    PLAYER temp;
    int num = 0;
    while (fread(&temp , sizeof(temp) , 1 , data) == 1)
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
        fseek(data,choice - 1,SEEK_SET);
        int res = fread(&temp,sizeof(temp),1,data);
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
