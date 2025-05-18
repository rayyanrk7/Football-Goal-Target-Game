#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

class Player {
private:
    string name;
    int goals;

public:
    Player(string playerName, int playerGoals) : name(playerName), goals(playerGoals) {}

    int getGoals() const {
        return goals;
    }

    string getName() const {
        return name;
    }
};

class Game {
private:
    vector<Player> players;
    int targetGoals;
    int totalGoals;
    int careerSlots;
    int doubleSlots;
    int tripleSlots;
    int quadSlots;

public:
    Game(int target) : targetGoals(target), totalGoals(0), careerSlots(3), doubleSlots(3), tripleSlots(2), quadSlots(2) {}

    void loadPlayersFromFile(const string& filename) {
        ifstream file(filename);
        string name;
        int goals;

        if (file.is_open()) {
            while (file >> name >> goals) {
                players.emplace_back(name, goals);
            }
            file.close();
        } else {
            cerr << "Unable to open file.\n";
        }
    }

    Player getRandomPlayer() {
        int randomIndex = rand() % players.size();
        return players[randomIndex];
    }

    void placePlayer(Player player, string slotType) {
        int multiplier = 1;
        if (slotType == "Career" && careerSlots > 0) {
            multiplier = 1;
            careerSlots--;
        } else if (slotType == "Double" && doubleSlots > 0) {
            multiplier = 2;
            doubleSlots--;
        } else if (slotType == "Triple" && tripleSlots > 0) {
            multiplier = 3;
            tripleSlots--;
        } else if (slotType == "Quad" && quadSlots > 0) {
            multiplier = 4;
            quadSlots--;
        } else {
            cout << "Invalid slot type or no slots available. Try again.\n";
            return;
        }

        int addedGoals = player.getGoals() * multiplier;
        totalGoals += addedGoals;
        cout << "Placed " << player.getName() << " in " << slotType << " slot. Added " << addedGoals << " goals.\n";
        cout << "Current total goals: " << totalGoals << endl;

        // Display remaining slots for each type
        cout << "Remaining slots -> Career: " << careerSlots << "/3, Double: " << doubleSlots << "/3, "
             << "Triple: " << tripleSlots << "/2, Quad: " << quadSlots << "/2\n";
    }

    void run() {
        srand(static_cast<unsigned int>(time(0))); // Seed random number generator

        while (careerSlots > 0 || doubleSlots > 0 || tripleSlots > 0 || quadSlots > 0) {
            cout << "\nPress Enter to select a random player, or type 'exit' to quit: ";
            string input;
            getline(cin, input);
            if (input == "exit") break;

            Player selectedPlayer = getRandomPlayer();
            cout << "Selected Player: " << selectedPlayer.getName() << " with " << selectedPlayer.getGoals() << " goals.\n";

            cout << "Choose a slot to place the player (Career, Double, Triple, Quad): ";
            string slotType;
            getline(cin, slotType);

            placePlayer(selectedPlayer, slotType);

            if (totalGoals >= targetGoals) {
                cout << "\nCongratulations! You have met the target goals of " << targetGoals << "!\n";
                return;
            }
        }

        cout << "\nGame over! You accumulated " << totalGoals << " goals.\n";
        if (totalGoals >= targetGoals) {
            cout << "You met the target goals!\n";
        } else {
            cout << "You did not meet the target goals. Better luck next time!\n";
        }
    }
};

int main() {
    int targetGoals;
    cout << "Enter the target number of goals: ";
    cin >> targetGoals;
    cin.ignore(); // Ignore the newline character left in the buffer

    Game game(targetGoals);

    // Load players from a file
    game.loadPlayersFromFile("goals.txt");

    // Run the game
    game.run();

    return 0;
}