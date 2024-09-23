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
        this->riddle = "I speak without a mouth and hear without ears. I have no body, but I come alive with wind. What am I?";
        this->correctAnswer = "Echo";
        cout << "Puzzle: " << this->riddle << endl;
    }

    bool checkAnswer(const string& answer) override {
        return answer == correctAnswer;
    }

    void provideHint() override {
        cout << "Hint: It repeats what you say." << endl;
    }
};

// Number Sequence Puzzle Class
class NumberSequencePuzzle : public Puzzle {
private:
    string sequence;
    string correctAnswer;
public:
    void generatePuzzle() override {
        this->sequence = "2, 4, 8, 16, ?";
        this->correctAnswer = "32";
        cout << "Puzzle: " << this->sequence << endl;
    }

    bool checkAnswer(const string& answer) override {
        return answer == correctAnswer;
    }

    void provideHint() override {
        cout << "Hint: It doubles each time." << endl;
    }
};

// Room Class
class Room {
private:
    string roomName;
    Puzzle* puzzle; 
    bool isSolved = false;

public:
    Room(string name, Puzzle* p) : roomName(name), puzzle(p) {}

    void enterRoom() {
        cout << "You have entered the " << roomName << "." << endl;
    }

    void startPuzzle() {
        if (!isSolved) {
            puzzle->generatePuzzle();
        }
    }

    bool checkSolution(const string& answer) {
        if (!isSolved && puzzle->checkAnswer(answer)) {
            isSolved = true;
            return true;
        }
        return false;
    }

    void provideHint() {
        if (!isSolved) {
            puzzle->provideHint();
        }
    }

    bool isRoomSolved() const {
        return isSolved;
    }

    ~Room() {
        delete puzzle;
    }
};

// Player Class
class Player {
private:
    string playerName;
    int attempts = 0;
    int hintsUsed = 0;

public:
    Player(string name) : playerName(name) {}

    void trackProgress() const {
        cout << playerName << " has made " << attempts << " attempts and used " << hintsUsed << " hints." << endl;
    }

    void updateAttempts() {
        attempts++;
    }

    void useHint() {
        hintsUsed++;
    }

    string getName() const {
        return playerName;
    }
};

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
        cout << "Welcome, " << player->getName() << "! The game begins now!" << endl;

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
