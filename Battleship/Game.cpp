#include "Game.h"
#include <iomanip>
#include "FileUtil.h"
#include <cstdlib>

void Game::start(){
    BattleShipPlayer *Currentplayer = new BattleShipPlayer();
    bool GameRunStatus = true;
    FileUtil::PrintFile("Logo.txt");
    string WelcomeMessage = "Welcome to BattleShip\n---Enter Player Name:";
    cout <<setw(23) << "" << "Welcome to BattleShip\n" << setw(23) << "" << "Enter Player Name:";
    string name;
    do {
        cin >> name;
        if (name == "")
            cout << "Name Cannot be blank..\n";
    }while(name == "");

    BattleShipPlayer Player1(name);
    ComputerPlayer PlayerComputer("Computer");

    ifstream file;
    selectCSV:
    cout  << endl <<setw(20) << "" << "Choose a ShipPlacement File\n" << endl;
;
    FileUtil::SelectFileOfTypeFromCurrentDirectory(file, "csv","Select a file # choice to use as ShipPlacement:\n",20, "");
    try {
        if (!validateCSV(file)) {
            string exceptionMessage = "File contains Bad ship input\n Select another? (y/n)";
            throw exceptionMessage;
        }
    }catch (string &e){
        file.close();
        file.clear();
        cout << e << endl;
        string choice;
        cin >> choice;
        if(choice == "y"){
            goto selectCSV;
        } else if (choice  == "n"){
            cout << "exiting program...";
            exit(1);
        }else{
            exit(1);
        }
    }
    file.clear();
    file.seekg (0, ios::beg);

    Player1.setShips(FileToShips(file));
    file.close();

    PlayerComputer.setShips(GenerateEnemyShips());

    //once enemy ships populated ready to start the game
    int currentPlayer = 0;
    ship ms,es;
    string pos1,pos2; //player one shooot
    while(true) {
        ///-------------------------------------------------------------------------
        ///-----------------Show boards and results
        //clearScreen();
        cout << setw(24) << "" << Player1.getName() << "'s Board\n";
        DrawBoard(Player1, PlayerComputer, true);


        DrawBoard(PlayerComputer, Player1);
        cout << setw(20) << "" << PlayerComputer.getName() << "'s Board\n";


        ///---------------------------------------------------------------------
        ///--------------------Player one
        if(!pos1.empty()){
            cout << Player1.getName() << " Fired topedo at " << pos1 << GetTorpedoDamageResult(ms);
            cout << PlayerComputer.getName() << " Fired topedo at " << pos2 << GetTorpedoDamageResult(es);
        }
        cout << endl << setw(20) << "" <<"Fire at position?                    .....or Press Q to quit\n";
        cin >> pos1;
        for (auto &c: pos1)
            c = toupper(c); //convert to upper
        if(pos1 == "Q") {
            DrawBoard(PlayerComputer, Player1, true);
            cout << endl << setw(20) << "" << Player1.getName() << " Quit the game..\n";
            KeyToContinue();
            _exit(0);
        }
        if(!ValidateShot(pos1)){
            cout << endl << setw(20) << "" <<"Not a valid shot.. Try again\n";
            continue;
        }
        ms = Player1.shootAt(PlayerComputer,pos1);

        if(!PlayerComputer.isAlive()){
            cout << Player1.getName() << " WINS THE GAME";
            FileUtil::PrintFile("Win.txt");
            KeyToContinue();
            exit(0);
        }
        ///-------------------------------------------------------------------------
        ///-----------------Computer player
        pos2 = GenerateRandomPosition();
        if(!ValidateShot(pos2)){
            continue;
        }
        es = PlayerComputer.shootAt(Player1,pos2);

        if(!Player1.isAlive()){
            FileUtil::PrintFile("Gameover.txt");
            cout << PlayerComputer.getName() << " WINS THE GAME";
            break;
        }


    }
    KeyToContinue();
}

string Game::GetTorpedoDamageResult(ship &sh) {
    if(sh.type.empty()){
        return "  ..MISSED the target!!\n";
    }else if(sh.isAlive()) {
        return " ..HIT a target!!\n";
    }else{
        return " ...SUNK a " + sh.type +" !!\n";
    }
}

void DebugDisplayZShips(vector<ship> ships){
    for(auto e : ships) {
        cout << endl;
        for (auto a : e.DamagePositionMap)
            cout << a.first << " " << a.second << " ";
    }
    cout << endl;
};

bool Game::shootFromTo(BattleShipPlayer &player,BattleShipPlayer &other) {
    string pos;
    shoot:
    if (player.getName() != "Computer"){
        cout << endl << setw(20) << "" <<"Fire at position?                    .....or Press Q to quit\n";
        cin >> pos;
        for (auto &c: pos)
            c = toupper(c); //convert to upper
        if(pos == "Q") {
            return true;
        }
        if(!ValidateShot(pos)){
            cout << endl << setw(20) << "" <<"Not a valid shot.. Try again\n";
            goto shoot;
        }
    }else {
        pos = GenerateRandomPosition();
        if(!ValidateShot(pos)){
            goto shoot;
        }
    }

    if (player.isUsedShot(pos)){
        player.getUsedShots().insert (pos); //to-do convert this if to a  function
        if(player.getName() == "Computer")
            goto shoot;
    }else{
        player.getUsedShots().insert(pos);
    }
    cout << setw(20) << "" <<player.getName() <<" Torpedo Fired at at " << pos << ".. ";
    for(auto &sh : other.getShips()){
        if(sh.status) {
            if (sh.DamagePositionMap.find(pos) != sh.DamagePositionMap.end()){
                sh.DamagePositionMap[pos] = true;
                if (!sh.isAlive()) {
                    sh.status = false;
                    cout << setw(20) << "" << player.getName() << " SUNK a "<< sh.type <<"!!\n";
                }else{
                    cout << endl << setw(20) << ""  << player.getName()  << " shot Was a HIT!!\n";
                }
                return false;
            }
        }
    }
    cout << endl << setw(20) << ""  << player.getName() << " missed shot..\n";
    player.getMissedShots().insert(pos);
    return false;
}

Game::Game() {
    srand (time(NULL));
}

const bool Game::ValidateShot(string pos) {
    if(std::isdigit(pos[1]))
        return (pos[0] >= 'A' && pos[0] <= 'J' && pos[1] > '0' && std::stoi(pos.substr(1)) <= 10);
    return false;
}

bool Game::vCheckDown(shipType sz, std::string pos, std::map<std::string, bool> &board) {
    char letter = pos[0];
    char number = pos[1];
    std::string Key;
    for (char n = number; n < number + sz-1; n++) { //iterate up
        if(n < ':' ){
            Key = std::string(1, letter) + n;
        }else if (n == ':'){
            Key = std::string(1, letter) + "10";
        }else{
            return false;
        }
        if (ValidateShot(Key) && board.count(Key) == 0) {  ///check if key exist in board
            board[Key] = true; ///Good no Key so last add to the board
        }else {
            ///no good key doesnt exist
            return false;
        }
    }
    ///if we reach the the end it is valid placement
    return true;
}

vector<ship>  Game::FileToShips(ifstream &file) {
    vector<ship> ships;
    if (file.is_open() && !file.eof()) {
        char *type, *pos, *orientation;
        std::string line;
        size_t sz;
        while (file >> line && !file.eof()) {
            ship sh; //ships to add
            sh.status = true;
            char *temp = const_cast<char *>(line.c_str());

            type = strtok_r(temp, ",", &temp);
            pos = strtok_r(temp, ",", &temp);
            orientation = strtok_r(temp, ",", &temp);

            if (strcmp(type, "Carrier") == 0) {
                sz = 5;
                sh.type = "Carrier";
            } else if (strcmp(type, "Battleship") == 0) {
                sz = 4;
                sh.type = "Battleship";
            } else if (strcmp(type, "Cruiser") == 0) {
                sz = 3;
                sh.type = "Cruiser";
            } else if (strcmp(type, "Submarine") == 0) {
                sz = 3;
                sh.type = "Submarine";
            } else if (strcmp(type, "Destroyer") == 0) {
                sz = 2;
                sh.type = "Destroyer";
            }
            FillShips(sz,pos,*orientation,sh);
            ships.push_back(sh);
        }
    }
    return ships;
}

string Game::GenerateRandomPosition(){
    char c;
    int r;
    string pos= "";
    // initialize the random number generator
    r = rand() % 10 ;       // generate a random numberA1
    c = 'A' + r;            // Convert to a character from a-z

    pos+=c;
    r = rand() % 10 + 1 ;   // generate a random number
    c= '0'+ r;
    if(r != 10){
        pos+=c;
    } else{
        pos+="10";
    }
    return pos;
}

void Game::FillShips(int sz, string pos, char orientation, ship &sh){
    char letter = pos[0];
    const char number = pos[1];
    sh.status = true;
    std::string Key;
    switch(orientation){
        case 'H':
            for (char l = letter; l < letter + sz; ++l) { //iterate up
                Key = std::string(1, l) + number;
                sh.DamagePositionMap[Key] = false;
            }
            break;
        case 'V':
            for (char n = number; n < number + sz; n++) { //iterate up
                if(n < ':') {
                    Key = std::string(1, letter) + n;
                }else{
                    Key = std::string(1, letter) + "10";
                }
                sh.DamagePositionMap[Key] = false;
            }
            break;
    }
}

vector<ship> Game::GenerateEnemyShips() {
    vector<ship> ships;
    std::map<std::string, bool> board;
    auto genOrientation = [](){             //some lambda yummy
        int r;
        r = rand() % 2;
        if(r == 1)
            return 'V';
        return 'H';
    };

    int sz[5]= {5,4,3,3,2};
    for(const auto e : sz){
        char Orientation;
        string pos;
        do
        {
            Orientation = genOrientation();
            pos = GenerateRandomPosition();
            if (ValidateEntry(static_cast<shipType>(e), pos, Orientation, board)) {
                ship sh;
                sh.type = EnumMap[e];
                sh.status = true;
                FillShips(e, pos, Orientation, sh);
                ships.push_back(sh);
                break;
            }
        }while(true);
    }
    return ships;
}

bool Game::vCheckRight(shipType sz, std::string pos, std::map<std::string, bool> &board) {
    char letter = pos[0];
    char number = pos[1];
    for (char l = letter; l < letter + sz; l++) {
        std::string Key = std::string(1, l) + number;
        if (ValidateShot(Key) && board.count(Key) == 0) { //check if key exist in board
            board[Key] = true;
        }else {
            ///no good key doesnt exist
            return false;
        }
    }
    ///if we reach the the end it is valid placement
    return true;
}

bool Game::validateCSV(std::ifstream &file) {
    if (file.is_open() && !file.eof()) {
        std::map<std::string, bool> board;
        char *type, *pos, *orientation;
        std::string line;
        ship sh; //ships to add

        while (file >> line && !file.eof()) {
            // board = Tboard;
            char *temp = const_cast<char *>(line.c_str());

            type = strtok_r(temp, ",", &temp);
            pos = strtok_r(temp, ",", &temp);
            orientation = strtok_r(temp, ",", &temp);
            shipType Etype;

            if (strcmp(type, "Carrier") == 0) {
                Etype = Carrier;
            } else if (strcmp(type, "Battleship") == 0) {
                Etype = Battleship;
            } else if (strcmp(type, "Cruiser") == 0) {
                Etype = Cruiser;
            } else if (strcmp(type, "Submarine") == 0) {
                Etype = Submarine;
            } else if (strcmp(type, "Destroyer") == 0) {
                Etype = Destroyer;
            } else {
                return false;
            }
            if (!ValidateEntry(Etype, pos, *orientation, board)) {
                board.clear();
                return false;
            }
        }
        return true;
    }
}

void Game::KeyToContinue() {
    cout << '\n' << "Press a key to continue...";
    string c;
    cin >> c;
}

bool Game::ValidateEntry(shipType type, std::string pos, char orientation, std::map<std::string, bool> &board) {
    switch (orientation) {
        case 'V':
            return vCheckDown(type, pos, board);
        case 'H':
            return vCheckRight(type, pos, board);
        default:
            return false;
    }
}

#ifdef _WIN32
#define miss         L" \x25CE"
#define hit          L" \x25CF"
#define blank        L" \x25A1"
#define shipNoDamage L" \x25BD"
#define shipDamage   L" \x25BC"

void Game::sm(mode m) {
    _setmode(_fileno(stdout), m);
}

void Game::print(const wchar_t* c, bool tabNewL){
    if(tabNewL){
        wprintf(L"\n\t\t%c", c);
    }else{
        wprintf(c);
    }
}


#else
#define MISS  " ◎"
    #define HIT   " ●"
    #define EMPTY " ◌"
    #define DAMAGE " ▽"
    #define NODAMAGE " ▼"

    string miss = MISS;
    string hit = HIT;
    string blank = EMPTY;
    string shipNoDamage = NODAMAGE;
    string shipDamage =  DAMAGE;

     void Game::printNumRow(){
        cout << "\t\t ";
        for (char x = '1'; x <= '1'+8; x++)
            cout << " " << x;
        cout<< " " << 10;
    }

    void Game::sm(mode m){};
    void Game::print(std::string str, bool tabNewL){
        if(tabNewL){
            cout <<"\n\t\t" <<str;
        }else{
            cout << str;
        }
    }

#endif

void Game::DrawBoard(BattleShipPlayer player, BattleShipPlayer other, bool showBoats) {
    cout << "\t\t  ";
    for (char x = 'A'; x <= 'A'+9; x++)
        cout << x << "  ";
    cout << endl;
    sm(U16);
    for (char x = '1'; x <= '9'+1; x++) {
#ifdef _WIN32
        if(x != ':') {
            print(reinterpret_cast<const wchar_t *>(x), true);
        }else{
            sm(text);
            cout << "\n\t       10";
            sm(U16);
        }
#else
        print(std::string(1,x), true);
#endif
        if(!showBoats) {
            map<string, bool> pos;
            for (auto e : player.getShips())
                for (auto d :  e.DamagePositionMap)
                    if (d.second)
                        pos[d.first] = true;
            for (char y = 'A'; y <= 'A' + 9; y++) {
                string z;
                if (x != ':')
                    z = std::string(1, y) + x;
                else
                    z = std::string(1, y) + "10";
                if (pos.count(z) > 0 && pos.find(z)->second) {
                    print(hit, false); //hit
                } else if (other.getMissedShots().find(z) != other.getMissedShots().end()) {
                    print(miss, false); //miss
                } else {
                    print(blank, false); //blank
                }
            }
        }else{ //player section show boats and damage done
            map<string, bool> pos;
            for (auto e : player.getShips())
                for (auto d :  e.DamagePositionMap)
                    if (d.second)
                        pos[d.first] = true;
                    else
                        pos[d.first] = false;

            for (char y = 'A'; y <= 'A' + 9; y++) {
                string z;
                if (x != ':')
                    z = std::string(1, y) + x;
                else
                    z = std::string(1, y) + "10";
                if (pos.count(z) > 0 && pos.find(z)->second) {
                    print(shipNoDamage, false); //hit
                }else if(pos.count(z) > 0 && !pos.find(z)->second) {
                    print(shipDamage, false); //hit
                }else if (other.getMissedShots().find(z) != other.getMissedShots().end()) {
                    print(miss, false); //miss
                } else {
                    print(blank, false); //blank
                }
            }
        }
    }
    sm(text);
    cout << endl << endl;
}



void Game::clearScreen()
{
#ifdef _WIN32
    std::system("cls");
#else
    // Assume POSIX
    std::system ("clear");
#endif
}



