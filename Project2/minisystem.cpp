#include <iostream>
#include <string>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

const int MAX_CLIENTS = 100;
const int MAX_TRAINERS = 20;
const int MAX_WORKOUTS = 10;
const int MAX_EXERCISES = 10;
const int MAX_LOGS = 20;
const int MAX_MEASUREMENTS = 20;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

struct Measurement {
    float weight;
    float height;
    string date;
};

struct Workout {
    int workoutID;
    string workoutName;
    string exercises[MAX_EXERCISES];
    int numExercises;
    int duration;
};

struct Client {
    int clientID;
    string username;
    string password;
    Workout workoutPlans[MAX_WORKOUTS];
    int numWorkouts = 0;
    string progressLogs[MAX_LOGS];
    int numLogs = 0;
    Measurement measurements[MAX_MEASUREMENTS];
    int numMeasurements = 0;
};

struct Trainer {
    int trainerID;
    string username;
    string password;
};

Client clients[MAX_CLIENTS];
int clientCount = 0;
Trainer trainers[MAX_TRAINERS];
int trainerCount = 0;

void initializeData() {
    clients[0] = { 1, "alice", "alice123" };
    clients[1] = { 2, "bob", "bob123" };
    clients[2] = { 3, "charlie", "charlie123" };
    clientCount = 3;

    trainers[0] = { 1, "sarah", "sarah123" };
    trainers[1] = { 2, "david", "david123" };
    trainerCount = 2;
}

Client* loginClient(string username, string password) {
    for (int i = 0; i < clientCount; i++) {
        if (clients[i].username == username && clients[i].password == password) {
            return &clients[i];
        }
    }
    return nullptr;
}

Trainer* loginTrainer(string username, string password) {
    for (int i = 0; i < trainerCount; i++) {
        if (trainers[i].username == username && trainers[i].password == password) {
            return &trainers[i];
        }
    }
    return nullptr;
}

void registerUser(bool isTrainer) {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    if (isTrainer) {
        trainers[trainerCount] = { trainerCount + 1, username, password };
        trainerCount++;
        cout << "Trainer registered successfully!\n";
    }
    else {
        clients[clientCount] = { clientCount + 1, username, password };
        clientCount++;
        cout << "Client registered successfully!\n";
    }
}

void logMeasurement(Client& client) {
    if (client.numMeasurements >= MAX_MEASUREMENTS) {
        cout << "Measurement limit reached!\n";
        return;
    }
    Measurement m;
    cout << "Enter weight (kg): ";
    cin >> m.weight;
    cout << "Enter height (cm): ";
    cin >> m.height;
    cout << "Enter date: ";
    cin >> m.date;

    client.measurements[client.numMeasurements++] = m;
    cout << "Measurement added!\n";
}

void viewWorkouts(Client& client) {
    if (client.numWorkouts == 0) {
        cout << "No workouts assigned.\n";
        return;
    }
    for (int i = 0; i < client.numWorkouts; i++) {
        cout << i + 1 << ". " << client.workoutPlans[i].workoutName << " (" << client.workoutPlans[i].duration << " mins)\n";
        for (int j = 0; j < client.workoutPlans[i].numExercises; j++) {
            cout << "   - " << client.workoutPlans[i].exercises[j] << endl;
        }
    }
}

void logWorkout(Client& client) {
    if (client.numWorkouts == 0) {
        cout << "No workouts assigned. Cannot log workout.\n";
        return;
    }
    if (client.numLogs >= MAX_LOGS) {
        cout << "Log limit reached.\n";
        return;
    }
    string log;
    cout << "Enter workout name you completed: ";
    cin.ignore();
    getline(cin, log);
    client.progressLogs[client.numLogs++] = log;
    cout << "Workout logged!\n";
}

void assignWorkoutToClient(Trainer& trainer) {
    cout << "Available Clients:\n";
    for (int i = 0; i < clientCount; i++) {
        cout << clients[i].clientID << ". " << clients[i].username << endl;
    }

    int clientID;
    cout << "Enter client ID to assign workout: ";
    cin >> clientID;

    Client* client = nullptr;
    for (int i = 0; i < clientCount; i++) {
        if (clients[i].clientID == clientID) {
            client = &clients[i];
            break;
        }
    }

    if (client == nullptr) {
        cout << "Client not found.\n";
        return;
    }

    if (client->numWorkouts >= MAX_WORKOUTS) {
        cout << "Workout limit reached for this client.\n";
        return;
    }

    Workout w;
    w.workoutID = client->numWorkouts + 1;
    cout << "Enter workout name: ";
    cin.ignore();
    getline(cin, w.workoutName);
    cout << "Enter number of exercises: ";
    cin >> w.numExercises;

    cin.ignore();
    for (int i = 0; i < w.numExercises; i++) {
        cout << "Exercise " << i + 1 << ": ";
        getline(cin, w.exercises[i]);
    }

    cout << "Enter duration (minutes): ";
    cin >> w.duration;

    client->workoutPlans[client->numWorkouts++] = w;
    cout << "Workout assigned!\n";
}

void viewClientProgress(Trainer& trainer) {
    cout << "Available Clients:\n";
    for (int i = 0; i < clientCount; i++) {
        cout << clients[i].clientID << ". " << clients[i].username << endl;
    }

    int clientID;
    cout << "Enter client ID to view progress: ";
    cin >> clientID;

    Client* client = nullptr;
    for (int i = 0; i < clientCount; i++) {
        if (clients[i].clientID == clientID) {
            client = &clients[i];
            break;
        }
    }

    if (client == nullptr) {
        cout << "Client not found.\n";
        return;
    }

    cout << "--- Progress Logs ---\n";
    for (int i = 0; i < client->numLogs; i++) {
        cout << i + 1 << ". " << client->progressLogs[i] << endl;
    }
    cout << "--- Measurements ---\n";
    for (int i = 0; i < client->numMeasurements; i++) {
        cout << i + 1 << ". " << client->measurements[i].date << " | "
            << client->measurements[i].weight << "kg, "
            << client->measurements[i].height << "cm\n";
    }
}

void clientMenu(Client& client) {
    int choice;
    do {
        clearScreen();
        cout << "\n--- Client Menu ---\n";
        cout << "1. View Workouts\n";
        cout << "2. Log Workout\n";
        cout << "3. Log Measurement\n";
        cout << "4. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;
        clearScreen();

        switch (choice) {
        case 1: viewWorkouts(client); break;
        case 2: logWorkout(client); break;
        case 3: logMeasurement(client); break;
        case 4: cout << "Logging out...\n"; break;
        default: cout << "Invalid choice.\n";
        }

        if (choice != 4) {
            cout << "\nPress Enter to continue...";
            cin.ignore(); cin.get();
        }
    } while (choice != 4);
}

void trainerMenu(Trainer& trainer) {
    int choice;
    do {
        clearScreen();
        cout << "\n--- Trainer Menu ---\n";
        cout << "1. Assign Workout to Client\n";
        cout << "2. View Client Progress\n";
        cout << "3. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;
        clearScreen();

        switch (choice) {
        case 1: assignWorkoutToClient(trainer); break;
        case 2: viewClientProgress(trainer); break;
        case 3: cout << "Logging out...\n"; break;
        default: cout << "Invalid choice.\n";
        }

        if (choice != 3) {
            cout << "\nPress Enter to continue...";
            cin.ignore(); cin.get();
        }
    } while (choice != 3);
}

void mainMenu() {
    initializeData();
    int choice;
    do {
        clearScreen();
        cout << "\n--- Fitness Training System ---\n";
        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        clearScreen();

        if (choice == 1) {
            int type;
            cout << "Are you:\n1. Client\n2. Trainer\nChoice: ";
            cin >> type;

            string username, password;
            cout << "Username: ";
            cin >> username;
            cout << "Password: ";
            cin >> password;

            clearScreen();
            if (type == 1) {
                Client* client = loginClient(username, password);
                if (client != nullptr) {
                    clientMenu(*client);
                }
                else {
                    cout << "Invalid credentials.\n";
                }
            }
            else if (type == 2) {
                Trainer* trainer = loginTrainer(username, password);
                if (trainer != nullptr) {
                    trainerMenu(*trainer);
                }
                else {
                    cout << "Invalid credentials.\n";
                }
            }

        }
        else if (choice == 2) {
            int regType;
            cout << "Register as:\n1. Client\n2. Trainer\nChoice: ";
            cin >> regType;
            registerUser(regType == 2);

        }
        else if (choice == 3) {
            cout << "Exiting...\n";
        }
        else {
            cout << "Invalid choice.\n";
        }

        if (choice != 3) {
            cout << "\nPress Enter to continue...";
            cin.ignore(); cin.get();
        }

    } while (choice != 3);
}

int main() {
    mainMenu();
    return 0;
}
