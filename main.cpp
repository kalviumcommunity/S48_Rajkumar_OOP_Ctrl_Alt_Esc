#include <iostream>
#include <string>
#include <memory>
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
    string riddle = "I speak without a mouth and hear without ears. What am I?";
    string correctAnswer = "Echo";
public:
    void generatePuzzle() override {
        cout << "Puzzle: " << riddle << endl;
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
    string sequence = "2, 4, 8, 16, ?";
    string correctAnswer = "32";
public:
    void generatePuzzle() override {
        cout << "Puzzle: " << sequence << endl;
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
    unique_ptr<Puzzle> puzzle;
    bool isSolved = false;

public:
    Room(string name, unique_ptr<Puzzle> p) : roomName(name), puzzle(move(p)) {}

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
    unique_ptr<Room> rooms[10];
    int roomCount = 0;

public:
    GameEngine(Player* p) : player(p) {}

    void addRoom(unique_ptr<Room> room) {
        if (roomCount < 10) {
            rooms[roomCount++] = move(room);
        } else {
            cout << "Cannot add more rooms." << endl;
        }
    }

    void startGame() {
        cout << "Welcome, " << player->getName() << "! The game begins now!" << endl;

        for (int i = 0; i < roomCount; ++i) {
            Room& currentRoom = *rooms[i];
            if (currentRoom.isRoomSolved()) continue;

            currentRoom.enterRoom();
            currentRoom.startPuzzle();

            string playerAnswer;
            cout << "Enter your answer: ";
            getline(cin, playerAnswer);
            player->updateAttempts();

            if (currentRoom.checkSolution(playerAnswer)) {
                cout << "Correct! You have solved the puzzle!" << endl;
                continue; // Move to the next room
            } else {
                cout << "Incorrect! Do you need a hint? (y/n): ";
                string needHint;
                getline(cin, needHint);
                if (needHint == "y" || needHint == "Y") {
                    currentRoom.provideHint();
                    player->useHint();
                }
            }

            player->trackProgress();
        }

        cout << "Game over! Thanks for playing!" << endl;
    }
};

// Main Function
int main() {
    string playerName;
    cout << "Enter player name: ";
    getline(cin, playerName);

    Player player(playerName);
    GameEngine game(&player);

    game.addRoom(make_unique<Room>("Mystery Room", make_unique<RiddlePuzzle>()));
    game.addRoom(make_unique<Room>("Logic Room", make_unique<NumberSequencePuzzle>()));

    game.startGame();

    return 0;
}