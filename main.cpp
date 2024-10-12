#include <iostream>
#include <string>
using namespace std;

// Base Puzzle Class
class Puzzle {
public:
    virtual void generatePuzzle() = 0;
    virtual bool checkAnswer(const string& answer) = 0;
    virtual void provideHint() = 0;
    virtual ~Puzzle() {}
};

// Riddle Puzzle Class
class RiddlePuzzle : public Puzzle {
private:
    string riddle;
    string correctAnswer;
public:
    void generatePuzzle() override {
        setRiddle("I speak without a mouth and hear without ears. I have no body, but I come alive with wind. What am I?");
        setCorrectAnswer("Echo");
        cout << "Puzzle: " << getRiddle() << endl;
    }

    bool checkAnswer(const string& answer) override {
        return answer == getCorrectAnswer();
    }

    void provideHint() override {
        cout << "Hint: It repeats what you say." << endl;
    }

    // Accessors and Mutators
    string getRiddle() const {
        return riddle;
    }

    void setRiddle(const string& newRiddle) {
        riddle = newRiddle;
    }

    string getCorrectAnswer() const {
        return correctAnswer;
    }

    void setCorrectAnswer(const string& newAnswer) {
        correctAnswer = newAnswer;
    }
};

// Number Sequence Puzzle Class
class NumberSequencePuzzle : public Puzzle {
private:
    string sequence;
    string correctAnswer;
public:
    void generatePuzzle() override {
        setSequence("2, 4, 8, 16, ?");
        setCorrectAnswer("32");
        cout << "Puzzle: " << getSequence() << endl;
    }

    bool checkAnswer(const string& answer) override {
        return answer == getCorrectAnswer();
    }

    void provideHint() override {
        cout << "Hint: It doubles each time." << endl;
    }

    // Accessors and Mutators
    string getSequence() const {
        return sequence;
    }

    void setSequence(const string& newSequence) {
        sequence = newSequence;
    }

    string getCorrectAnswer() const {
        return correctAnswer;
    }

    void setCorrectAnswer(const string& newAnswer) {
        correctAnswer = newAnswer;
    }
};

// Room Class
class Room {
private:
    string roomName;
    Puzzle* puzzle; 
    bool isSolved = false;
    static int totalRooms;  // Static variable to track total rooms

public:
    Room(string name, Puzzle* p) : roomName(name), puzzle(p) {
        totalRooms++;  // Increment total rooms whenever a new room is created
    }

    void enterRoom() {
        cout << "You have entered the " << getRoomName() << "." << endl;
    }

    void startPuzzle() {
        if (!getIsSolved()) {
            puzzle->generatePuzzle();
        }
    }

    bool checkSolution(const string& answer) {
        if (!getIsSolved() && puzzle->checkAnswer(answer)) {
            setIsSolved(true);
            return true;
        }
        return false;
    }

    void provideHint() {
        if (!getIsSolved()) {
            puzzle->provideHint();
        }
    }

    bool isRoomSolved() const {
        return isSolved;
    }

    static int getTotalRooms() {  // Static function to access the static variable
        return totalRooms;
    }

    // Accessors and Mutators
    string getRoomName() const {
        return roomName;
    }

    void setRoomName(const string& name) {
        roomName = name;
    }

    bool getIsSolved() const {
        return isSolved;
    }

    void setIsSolved(bool solved) {
        isSolved = solved;
    }

    ~Room() {
        delete puzzle;
    }
};

// Initialize static variable
int Room::totalRooms = 0;

// Player Class
class Player {
private:
    string playerName;
    int attempts = 0;
    static int totalHintsUsed;  // Static variable to track hints used across all players
    static int totalPlayers;    // Static variable to track total number of players
    static int totalAttempts;   // Static variable to track total attempts across all players

public:
    Player(string name) : playerName(name) {
        totalPlayers++;  // Increment total players whenever a new player is created
    }

    void trackProgress() const {
        cout << getPlayerName() << " has made " << getAttempts() << " attempts and used " << totalHintsUsed << " hints." << endl;
    }

    void updateAttempts() {
        attempts++;
        totalAttempts++;  // Increment total attempts
    }

    void useHint() {
        totalHintsUsed++;  // Increment static hints variable
    }

    // Accessors and Mutators
    string getPlayerName() const {
        return playerName;
    }

    void setPlayerName(const string& name) {
        playerName = name;
    }

    int getAttempts() const {
        return attempts;
    }

    void setAttempts(int newAttempts) {
        attempts = newAttempts;
    }

    static int getTotalHintsUsed() {  // Static function to access the static variable
        return totalHintsUsed;
    }

    static int getTotalPlayers() {  // Static function to access total players
        return totalPlayers;
    }

    static int getTotalAttempts() {  // Static function to access total attempts
        return totalAttempts;
    }
};

// Initialize static variables
int Player::totalHintsUsed = 0;
int Player::totalPlayers = 0;
int Player::totalAttempts = 0;

// Game Engine Class
class GameEngine {
private:
    Player* player;
    Room** rooms; 
    int roomCount = 0;
    int maxRooms;

public:
    GameEngine(Player* p, int maxRooms) : player(p), maxRooms(maxRooms) {
        rooms = new Room*[maxRooms];
    }

    void addRoom(Room* room) {
        if (roomCount < maxRooms) {
            rooms[roomCount++] = room;
        } else {
            cout << "Cannot add more rooms." << endl;
        }
    }

    void startGame() {
        cout << "Welcome, " << player->getPlayerName() << "! The game begins now!" << endl;

        for (int i = 0; i < roomCount; ++i) {
            Room* currentRoom = rooms[i];
            if (currentRoom->isRoomSolved()) continue;

            currentRoom->enterRoom();
            currentRoom->startPuzzle();

            string playerAnswer;
            cout << "Enter your answer: ";
            getline(cin, playerAnswer);
            player->updateAttempts();

            if (currentRoom->checkSolution(playerAnswer)) {
                cout << "Correct! You have solved the puzzle!" << endl;
                continue; 
            } else {
                cout << "Incorrect! Do you need a hint? (y/n): ";
                string needHint;
                getline(cin, needHint);
                if (needHint == "y" || needHint == "Y") {
                    currentRoom->provideHint();
                    player->useHint();
                }
            }

            player->trackProgress();
        }

        cout << "Game over! Thanks for playing!" << endl;
        cout << "Total rooms created: " << Room::getTotalRooms() << endl;  // Access static function
        cout << "Total players: " << Player::getTotalPlayers() << endl;  // Access static function
        cout << "Total attempts: " << Player::getTotalAttempts() << endl;  // Access static function
        cout << "Total hints used: " << Player::getTotalHintsUsed() << endl;  // Access static function
    }

    ~GameEngine() {
        for (int i = 0; i < roomCount; ++i) {
            delete rooms[i];  
        }
        delete[] rooms;  
    }
};

// Main Function
int main() {
    string playerName;
    cout << "Enter player name: ";
    getline(cin, playerName);

    Player* player = new Player(playerName);

    GameEngine* game = new GameEngine(player, 10);

    game->addRoom(new Room("Mystery Room", new RiddlePuzzle()));
    game->addRoom(new Room("Logic Room", new NumberSequencePuzzle()));

    game->startGame();

    delete game;  
    delete player; 

    return 0;
}
