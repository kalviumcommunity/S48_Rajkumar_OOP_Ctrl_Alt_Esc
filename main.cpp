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

// Riddle Puzzle Class - Inherits from Puzzle
class RiddlePuzzle : public Puzzle {
private:
    string riddle;
    string correctAnswer;

public:
    RiddlePuzzle() : riddle(""), correctAnswer("") {}

    RiddlePuzzle(const string& r, const string& answer) : riddle(r), correctAnswer(answer) {}

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

    ~RiddlePuzzle() {}
};

class NumberSequencePuzzle : public Puzzle {
private:
    string sequence;
    string correctAnswer;

public:
    NumberSequencePuzzle() : sequence(""), correctAnswer("") {}

    NumberSequencePuzzle(const string& seq, const string& answer) : sequence(seq), correctAnswer(answer) {}

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

    ~NumberSequencePuzzle() {}
};

class MathPuzzle : public Puzzle {
private:
    string problem;
    string correctAnswer;

public:
    MathPuzzle() : problem(""), correctAnswer("") {}

    MathPuzzle(const string& p, const string& answer) : problem(p), correctAnswer(answer) {}

    void generatePuzzle() override {
        setProblem("What is 12 + 8?");
        setCorrectAnswer("20");
        cout << "Puzzle: " << getProblem() << endl;
    }

    bool checkAnswer(const string& answer) override {
        return answer == getCorrectAnswer();
    }

    void provideHint() override {
        cout << "Hint: It's a simple addition." << endl;
    }

    string getProblem() const {
        return problem;
    }

    void setProblem(const string& newProblem) {
        problem = newProblem;
    }

    string getCorrectAnswer() const {
        return correctAnswer;
    }

    void setCorrectAnswer(const string& newAnswer) {
        correctAnswer = newAnswer;
    }

    ~MathPuzzle() {}
};

class Room {
private:
    string roomName;
    Puzzle* puzzle;
    bool isSolved = false;

public:
    Room() : roomName("Default Room"), puzzle(nullptr) {}

    Room(string name, Puzzle* p) : roomName(name), puzzle(p) {}

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

class Player {
private:
    string playerName;
    int attempts = 0;

public:
    Player() : playerName("Unnamed Player"), attempts(0) {}

    Player(string name) : playerName(name) {}

    string getPlayerName() const {
        return playerName;
    }

    void setPlayerName(const string& name) {
        playerName = name;
    }

    int getAttempts() const {
        return attempts;
    }

    void updateAttempts() {
        attempts++;
    }

    ~Player() {}
};

class GameEngine {
private:
    Player* player;
    Room** rooms;
    int roomCount = 0;
    int maxRooms;
    int totalHintsTaken = 0;     // Track total hints taken
    int totalCorrectAnswers = 0; // Track total correct answers

public:
    GameEngine(Player* p, int maxRooms) : player(p), maxRooms(maxRooms) {
        rooms = new Room*[maxRooms];
    }

    ~GameEngine() {
        for (int i = 0; i < roomCount; ++i) {
            delete rooms[i];
        }
        delete[] rooms;
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
            if (currentRoom->getIsSolved()) continue;

            currentRoom->enterRoom();
            currentRoom->startPuzzle();

            string playerAnswer;
            cout << "Enter your answer: ";
            getline(cin, playerAnswer);
            player->updateAttempts();

            if (currentRoom->checkSolution(playerAnswer)) {
                cout << "Correct! You have solved the puzzle!" << endl;
                totalCorrectAnswers++; // Increase correct answer count
                continue;
            } else {
                cout << "Incorrect! Do you need a hint? (y/n): ";
                string needHint;
                getline(cin, needHint);
                if (needHint == "y" || needHint == "Y") {
                    currentRoom->provideHint();
                    totalHintsTaken++; // Increase hints taken count
                }
            }
        }

        cout << "Game over! Thanks for playing!" << endl;
        cout << "Total hints taken: " << totalHintsTaken << endl;
        cout << "Total correct answers: " << totalCorrectAnswers << endl;
    }
};

int main() {
    string playerName;
    cout << "Enter player name: ";
    getline(cin, playerName);

    Player* player = new Player(playerName);

    GameEngine* game = new GameEngine(player, 10);

    game->addRoom(new Room("Mystery Room", new RiddlePuzzle()));
    game->addRoom(new Room("Logic Room", new NumberSequencePuzzle()));
    game->addRoom(new Room("Math Room", new MathPuzzle()));

    game->startGame();

    delete game;
    delete player;

    return 0;
}
