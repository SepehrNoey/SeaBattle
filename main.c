#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <string.h>

int menu(int choice ,FILE * data);
int player_set(FILE * data);
int load(FILE * data);  // return type is the points of the player

int main(void){  // 10 * 10
    FILE * data = fopen("data.SB","r+");  /// loading data
    
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
        int player1 = player_set(data);  // return type is the point of player
        
        fseek(data,0,SEEK_SET); 
        
        int player2 = player_set(data);  // return type is the point of player
        
    }
    
    
}

int player_set(FILE * data) {  
    int innerChoice;
    
    system("cls");
    printf("Choose first player:\n\n1) Choose from available users\n2) New user");
    scanf("%d",&innerChoice);


    if (innerChoice == 1)
    {
        load(data);  // black box 
    }
    else if (innerChoice == 2)
    {
        newUser();  // black box
    }
    else 
    {
        puts("Invalid input.Try again");  // bazgasht be porsesh dobare
    }        

}

int load(FILE * data){
    
}
