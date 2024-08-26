#include <iostream>
#include <string>
#include <memory>
using namespace std;

// Base Puzzle Class (Abstract Class)
class Puzzle {
public:
    virtual void generatePuzzle() = 0;
    virtual bool checkAnswer(string answer) = 0;
    virtual void provideHint() = 0;
    virtual ~Puzzle() {}
};

// Derived Class for Riddle Puzzle
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

    bool checkAnswer(string answer) override {
        return answer == this->correctAnswer;
    }

    void provideHint() override {
        cout << "Hint: It repeats what you say." << endl;
    }
};

// Derived Class for Number Sequence Puzzle
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

    bool checkAnswer(string answer) override {
        return answer == this->correctAnswer;
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
    bool isSolved;  // Flag to check if the puzzle has been solved

public:
    Room(string name, unique_ptr<Puzzle> p) : roomName(name), puzzle(move(p)), isSolved(false) {}

    Room(const Room&) = delete;

    Room(Room&& other) noexcept
        : roomName(move(other.roomName)), puzzle(move(other.puzzle)), isSolved(other.isSolved) {}

    void enterRoom() {
        cout << "You have entered the " << this->roomName << "." << endl;
    }

    void startPuzzle() {
        if (!this->isSolved) {
            this->puzzle->generatePuzzle();
        }
    }

    bool checkSolution(string answer) {
        if (!this->isSolved && this->puzzle->checkAnswer(answer)) {
            this->isSolved = true;
            return true;
        }
        return false;
    }

    void provideHint() {
        if (!this->isSolved) {
            this->puzzle->provideHint();
        }
    }

    bool isRoomSolved() const {
        return this->isSolved;
    }
};

// Player Class
class Player {
private:
    string playerName;
    int attempts;
    int hintsUsed;

public:
    Player(string name) : playerName(name), attempts(0), hintsUsed(0) {}

    string getName() const {
        return this->playerName;
    }

    void trackProgress() const {
        cout << this->playerName << " has made " << this->attempts << " attempts and used " << this->hintsUsed << " hints." << endl;
    }

    void updateAttempts() {
        this->attempts++;
    }

    void useHint() {
        this->hintsUsed++;
    }
};

// Game Engine Class
class GameEngine {
private:
    Player* player;
    static const int MAX_ROOMS = 10; // Maximum number of rooms
    unique_ptr<Room> rooms[MAX_ROOMS];
    int roomCount;

public:
    GameEngine(Player* p) : player(p), roomCount(0) {}

    void addRoom(unique_ptr<Room> room) {
        if (roomCount < MAX_ROOMS) {
            rooms[roomCount++] = move(room);
        } else {
            cout << "Cannot add more rooms, maximum capacity reached." << endl;
        }
    }

    void startGame() {
        cout << "Welcome, " << this->player->getName() << "! The game begins now!" << endl;

        for (int i = 0; i < roomCount; ++i) {
            Room& currentRoom = *rooms[i];
            if (currentRoom.isRoomSolved()) {
                continue;
            }

            currentRoom.enterRoom();
            currentRoom.startPuzzle();

            string playerAnswer;
            cout << "Enter your answer: ";
            getline(cin, playerAnswer);

            this->player->updateAttempts();
            if (currentRoom.checkSolution(playerAnswer)) {
                cout << "Correct! You have solved the puzzle!" << endl;
                break; // Stop the game immediately after the correct answer is given
            } else {
                cout << "Incorrect! Do you need a hint? (y/n): ";
                string needHint;
                getline(cin, needHint);
                if (needHint == "y" || needHint == "Y") {
                    currentRoom.provideHint();
                    this->player->useHint();
                }
            }

            this->player->trackProgress();
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