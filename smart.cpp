#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <unordered_map>
#include <string>

using namespace std;

class Patient {
public:
    int id;
    string name;
    int severity;

    Patient() : id(0), name(""), severity(0) {}
    Patient(int i, string n, int s) : id(i), name(n), severity(s) {}
};

class Doctor {
public:
    int id;
    string name;
    string specialization;

    Doctor() : id(0), name(""), specialization("") {}
    Doctor(int i, string n, string s) : id(i), name(n), specialization(s) {}
};

class Bed {
public:
    int id;
    bool occupied;

    Bed() : id(0), occupied(false) {}
    Bed(int i) : id(i), occupied(false) {}
};

class ComparePatient {
public:
    bool operator()(const Patient& a, const Patient& b) {
        return a.severity < b.severity;
    }
};

class HospitalSystem {
private:
    vector<Patient> patients;
    unordered_map<int, Doctor> doctors;
    vector<Bed> beds;
    priority_queue<Patient, vector<Patient>, ComparePatient> emergencyQueue;
    list<Patient> waitingList;

public:
    void addDoctor(int id, string name, string spec) {
        doctors.insert({id, Doctor(id, name, spec)});
        cout << "Doctor " << name << " added.\n";
    }

    void addBed(int id) {
        beds.push_back(Bed(id));
    }

    void addPatient(int id, string name, int severity) {
        Patient p(id, name, severity);
        patients.push_back(p);
        if (severity >= 3) {
            emergencyQueue.push(p);
            cout << "Emergency patient added!\n";
        } else {
            waitingList.push_back(p);
            cout << "Patient added to waiting list.\n";
        }
    }

    void assignBed() {
        for (auto &b : beds) {
            if (!b.occupied) {
                b.occupied = true;
                cout << "Bed " << b.id << " assigned.\n";
                return;
            }
        }
        cout << "No beds available!\n";
    }

    void treatPatient() {
        if (!emergencyQueue.empty()) {
            Patient p = emergencyQueue.top();
            emergencyQueue.pop();
            cout << "Treating CRITICAL: " << p.name << endl;
        }
        else if (!waitingList.empty()) {
            Patient p = waitingList.front();
            waitingList.pop_front();
            cout << "Treating NORMAL: " << p.name << endl;
        }
        else {
            cout << "No patients!\n";
        }
    }

    void showPatients() {
        for (const auto &p : patients) {
            cout << p.id << " " << p.name << " Severity: " << p.severity << endl;
        }
    }

    void findDoctor(int id) {
        auto it = doctors.find(id);
        if (it != doctors.end()) {
            cout << "Doctor: " << it->second.name << " (" << it->second.specialization << ")\n";
        } else {
            cout << "Doctor not found!\n";
        }
    }
};

int main() {
    HospitalSystem hs;
    int choice;

    hs.addDoctor(1, "Dr. Sharma", "Cardio");
    hs.addDoctor(2, "Dr. Khan", "Neuro");
    hs.addBed(101);
    hs.addBed(102);

    do {
        cout << "\n1. Add Patient\n2. Treat Patient\n3. Assign Bed\n4. Show Patients\n5. Find Doctor\n0. Exit\nSelection: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        if (choice == 1) {
            int id, severity;
            string name;
            cout << "Enter ID, Name, Severity: ";
            cin >> id >> name >> severity;
            hs.addPatient(id, name, severity);
        }
        else if (choice == 2) hs.treatPatient();
        else if (choice == 3) hs.assignBed();
        else if (choice == 4) hs.showPatients();
        else if (choice == 5) {
            int id;
            cout << "Enter ID: ";
            cin >> id;
            hs.findDoctor(id);
        }
    } while (choice != 0);

    return 0;
}