#include <iostream>
#include <string>
#include <fstream>
#include <memory>

//class forward declaration -> Why is it not working? So i have to move the definition of Character before Engine
class Character;

class Character{
private:
    std::string _name;
    std::string _type;
    int _Max_HP;
public:
    int _attack_point;
    int _defense_point;
    int _HP;
    Character(std::string name,std::string type) : _name(name),_type(type){

    }
    void Show_stats(){
        std::cout << "Name: " << _name << std::endl;
        std::cout << "Attack Point: " << _attack_point << std::endl;
        std::cout << "Defense Point: " << _defense_point << std::endl;
        std::cout << "HP: " << _HP << "/" << _Max_HP << std::endl;
    }
    void rest(){
        _HP = _Max_HP;
    }
    bool isAlive(){
        return _HP > 0;
    }
};

class Engine{
private:
    int _day = 0;
public:
    void Fight(std::unique_ptr<Character>& Player, std::unique_ptr<Character>& Enemy){
        std::cout << "You choose to fight." << std::endl;
        std::cout << "A Goblin appears." << std::endl;
        while (Player->_HP > 0 || Enemy->_HP > 0){
            if (Enemy->_attack_point > Player->_defense_point){
                Player->_HP -= Enemy->_attack_point - Player->_defense_point;
            }
            if (Player->_attack_point > Enemy->_defense_point) {
                Enemy->_HP -= Player->_attack_point - Enemy->_defense_point;
            }
        }
        _day+=1;
    }

    void Rest(std::unique_ptr<Character>& Player){
        Player->rest();
        _day+=1;
    }

    void Save(std::string username, int HP){
        std::string filename;
        std::cout << "Type your file name." << std::endl;
        std::cin >> filename;
        std::ofstream myfile;
        myfile.open(filename+ std::string(".csv"));
        myfile << username << "\n";
        myfile << HP << "\n";
        myfile.close();
    }

    void Load(){
        std::string line;
        std::string filename;
        std::cout << "Type your file name." << std::endl;
        std::cin >> filename;
        std::ifstream myfile;
        myfile.open(filename+ std::string(".csv"));
        while ( getline (myfile,line) )
        {
            std::cout << line << '\n';
        }
        myfile.close();
    }

    void Exit(){
        atexit(this->Save());
        exit(0);
    }
};


namespace CharacterFactory
{
//    why work ? -> unique_ptr die at the end of scope?
    std::unique_ptr<Character> create_character(std::string& name,std::string type){
        std::unique_ptr<Character> C = std::make_unique<Character>(name,type);
        return C;
    }
    void serialize(){};
    void deserialize(){};
};

void main_game_loop(Engine& Game, std::unique_ptr<Character>& My_character){
    while(My_character->isAlive()){
        std::cout << "What do you want to do? " << std::endl;
        std::cout << "1. Fight " << std::endl;
        std::cout << "2. Rest " << std::endl;
        std::cout << "3. Show stats " << std::endl;
        std::cout << "4. Save " << std::endl;
        std::cout << "5. Exit " << std::endl;
        int decision;
        std::cin >> decision;
        switch(decision){
            case 1: {
                std::unique_ptr<Character> enemy = CharacterFactory::create_character((std::string &) "goblin", "enemy");
                Game.Fight(My_character, enemy);
                break;
            }
            case 2: {
                Game.Rest(My_character);
                break;
            }
            case 3: {
                My_character->Show_stats();
                break;
            }
            case 4: {
                Game.Save(user_name, HP);
                break;
            }
            case 5: {
                Game.Exit();
                break;
            }
        }
    }
}

//global variables
std::string user_name;
int HP = 5;
//


//Program starts from here.
int main() {
    Engine Game;
    int choice;
    std::cout << "1. New Game" << std::endl;
    std::cout << "2. Load Game" << std::endl;
    std::cout << "3. Exit" << std::endl;
    std::cin >> choice;

    if (choice==1){
        std::cout << "What's your name?" << std::endl;
        std::cin >> user_name;
        std::unique_ptr<Character> My_character = CharacterFactory::create_character(user_name,"player");
        main_game_loop(Game, My_character);
    }else if (choice==2){
        My_character = Game.Load();
        main_game_loop(Game, My_character);
    }else if (choice==3){
        Game.Exit();
    }
    return 0;
}