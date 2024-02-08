#include <iostream>
#include <vector>
#include <limits> // for numeric_limits

using namespace std;
int processes, resources;

// Global variables to store information about the system.
vector<int> available;
vector<vector<int>> maximum;
vector<vector<int>> allocation;
vector<vector<int>> need;
vector<int> safeSequence;

// Function to get input from the user.
void getInput() {
    // Display purpose and functionality description.
    cout << "Banker's Algorithm for Deadlock Avoidance in C++ :\n";
    cout << "- Developed by Arbaaz Ali Hussain\n";
    cout << "- This program simulates the Banker's algorithm, a deadlock avoidance strategy,\n";
    cout << "- to manage resources allocated to processes and ensure a safe system state.\n\n";

    cout << "- The program takes user input for the number of processes, resources, allocation,\n";
    cout << "- and maximum resource requirements. It then checks if the system is initially in\n";
    cout << "- a safe state, and if so, displays the current state along with the SAFE sequence.\n\n";

    cout << "- Additionally, the program simulates a resource request from a specified process,\n";
    cout << "- updating the resource allocation and checking if the system remains in a safe state.\n\n";

    cout << "- Note: The code assumes a single resource type for simplicity.\n\n";

    // Get the number of processes from the user.
    do {
        cout << "Enter the number of Processes: ";
        cin >> processes;
        // Validate the entered number of processes.
        if (processes <= 0) {
            cout << "Invalid number of Processes. Please enter a positive number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (processes <= 0);

    // Get the number of resources from the user.
    do {
        cout << "Enter the number of Resources: ";
        cin >> resources;
        // Validate the entered number of resources.
        if (resources <= 0) {
            cout << "Invalid number of Resources. Please enter a positive number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (resources <= 0);

    // Resize vectors based on user input.
    available.resize(resources);
    maximum.resize(processes, vector<int>(resources, 0));
    allocation.resize(processes, vector<int>(resources, 0));
    need.resize(processes, vector<int>(resources, 0));

    // Get allocation matrix from the user.
    cout << "Enter the Allocation resources required by each process:" << endl;
    for (int i = 0; i < processes; ++i) {
        cout << "Process " << i << ": ";
        for (int j = 0; j < resources; ++j) {
            cin >> allocation[i][j];
        }
    }
    // Get maximum matrix from the user and validate input.
    cout << "Enter the Maximum resources required by each process:" << endl;
    for (int i = 0; i < processes; ++i) {
        cout << "Process " << i << ": ";
        for (int j = 0; j < resources; ++j) {
            do {
                cin >> maximum[i][j];
                // Ensure maximum resources are greater than or equal to allocation.
                if (maximum[i][j] < allocation[i][j]) {
                    cout << "Invalid input: Maximum resources should be greater than or equal to Allocation. Please re-enter." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            } while (maximum[i][j] < allocation[i][j]);
        }
    }
    // Get available resources from the user and validate input.
    cout << "Enter the Available resources:" << endl;
    for (int i = 0; i < resources; ++i) {
        do {
            cin >> available[i];
            // Ensure available resources are non-negative.
            if (available[i] < 0) {
                cout << "Invalid input: Available resources should be non-negative. Please re-enter." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (available[i] < 0);
    }
}
// Function to print the current state of the system.
void printTable() {
    // Print initial available values.
    cout << "Initial Available: ";
    for (int i = 0; i < resources; ++i) {
        cout << available[i] << " ";
    }
    cout << endl;

    cout << "Processes\tAllocation\tMax\t\tNeeds" << endl;
    for (int i = 0; i < processes; ++i) {
        // Print Processes
        cout << i << " ";
        cout << "\t\t\t";

        // Print Allocation
        for (int j = 0; j < resources; ++j) {
            cout << allocation[i][j] << " ";
        }
        cout << "\t\t";

        // Print Max
        for (int j = 0; j < resources; ++j) {
            cout << maximum[i][j] << " ";
        }
        cout << "\t";

        // Print Needs
        for (int j = 0; j < resources; ++j) {
            cout << need[i][j] << " ";
        }
        cout << endl;
    }
}
// Function to calculate the remaining needs of each process.
void calculateNeed() {
    for (int i = 0; i < processes; ++i) {
        for (int j = 0; j < resources; ++j) {
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
}
// Function to check if the system is in a safe state using Banker's algorithm.
bool isSafeState() {
    vector<int> work(available);
    vector<bool> finish(processes, false);
    int count = 0;

    while (count < processes) {
        bool found = false;
        for (int i = 0; i < processes; ++i) {
            if (!finish[i]) {
                int j;
                // Check if the needs of the process can be satisfied with available resources.
                for (j = 0; j < resources; ++j) {
                    if (need[i][j] > work[j])
                        break;
                }
                // If all needs can be satisfied, mark the process as finished.
                if (j == resources) {
                    // Update available resources after finishing the process.
                    for (int k = 0; k < resources; ++k) {
                        work[k] += allocation[i][k];
                    }
                    // Mark the process as finished and record its index in the safe sequence.
                    finish[i] = true;
                    safeSequence.push_back(i);
                    found = true;
                    count++;
                }
            }
        }
        // If no process can be finished in this iteration, break the loop.
        if (!found)
            break;
    }
    // Return whether all processes are finished, i.e., the system is in a safe state.
    return count == processes;
}
// Function to handle a resource request from a process.
bool requestResources(int process, const vector<int>& request) {
    // Check if the requested resources exceed the needs and available resources.
    for (int i = 0; i < resources; ++i) {
        if (request[i] > need[process][i] || request[i] > available[i]) {
            return false;
        }
    }

    // Temporarily update Allocation matrix
    for (int i = 0; i < resources; ++i) {
        available[i] -= request[i];
        allocation[process][i] += request[i];
    }

    // Temporarily update Need matrix
    for (int i = 0; i < resources; ++i) {
        need[process][i] -= request[i];
    }
    // Check if the system remains in a safe state after the resource request.
    if (isSafeState()) {
        // Permanent update if the system is still in a safe state.
        return true;
    } else {
        // Rollback changes if not in a safe state.
        for (int i = 0; i < resources; ++i) {
            available[i] += request[i];
            allocation[process][i] -= request[i];
            need[process][i] += request[i];
        }
        // Return false as the resource request would lead to an unsafe state.
        return false;
    }
}

// Function to display the SAFE sequence.
void SAFESequence() {
    cout << "The SAFE Sequence is: ";
    for (int i = 0; i < processes; ++i) {
        cout << "P" << safeSequence[i];
        if (i < processes - 1) {
            cout << ", ->";
        } else {
            cout << ".\n";
        }
    }
}
// Main function where the program execution starts.
int main() {
    getInput();
    calculateNeed();

    if (isSafeState()) {
        printTable();
        cout << "System is in a safe state." << endl;
        SAFESequence();
    } else {
        cout << "System is not in a safe state." << endl;
        return 0;
    }

    // Simulate a resource request.
    int processRequest;
    do {
        cout << "Enter the process requesting resources: ";

        // Check if the input is a valid integer.
        if (cin >> processRequest) {
            // Validate if the entered process number is within the valid range.
            if (processRequest < 0 || processRequest >= processes) {
                cout << "Invalid process number. Please enter a valid process number.\n";
            } else {
                break; // Exit the loop if valid input is provided.
            }
        } else {
            // Input is not a valid integer.
            cout << "Invalid input. Please enter a number.\n";
            cin.clear(); // Clear error flags.
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input.
        }
    } while (true);
    // Get resource request from the user.
    vector<int> request(resources, 0);
    cout << "Enter the resource request for process " << processRequest << ": ";
    for (int i = 0; i < resources; ++i) {
        cin >> request[i];
    }
    // Attempt to allocate resources based on the user's request.
    if (requestResources(processRequest, request)) {
        cout << "Resource request granted. System is still in a safe state." << endl;
    } else {
        cout << "Resource request denied. System is not in a safe state." << endl;
    }
    // Check and display the updated system state after resource request.
    if (isSafeState()) {
        printTable();
        cout << "System is in a safe state." << endl;
        SAFESequence();
    } else {
        cout << "System is not in a safe state." << endl;
        return 0;
    }

    return 0;
}
