#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class Student {
private:
    int id;
    string name;
    int age;

public:
    Student(int id, const string& name, int age){
        this->id = id;
        this->name = name;
        this->age = age;
    }

    int getID() const { return id; }
    string getName() const { return name; }
    int getAge() const { return age; }

    void display() const {
        cout << "ID: " << id << ", NAME: " << name << ", AGE: " << age << "\n";
    }

    void serialize(ofstream& ofs) const {
        ofs << id << "," << name << "," << age << "\n";
    }

    static Student deserialize(const string& line) {
        stringstream ss(line);
        string id_str, name, age_str;

        getline(ss, id_str, ',');
        getline(ss, name, ',');
        getline(ss, age_str, ',');

        int id = stoi(id_str);
        int age = stoi(age_str);

        return Student(id, name, age);
    }
};

class Database {
private:
    vector<Student> students;

public:
    void addStudent(const Student& student, const string& filename) {
        students.push_back(student);
        saveToCSV(filename);
    }

    void displayAll() const {
        for (const auto& student : students) {
            student.display();
        }
    }

    void saveToCSV(const string& filename) const {
        ofstream ofs(filename, ios::out);
        if (!ofs) {
            cerr << "Error: Could not open file for saving." << '\n';
            return;
        }

        for (const auto& student : students) {
            student.serialize(ofs);
        }

        ofs.close();
    }

    void load_from_file(const string& filename) {
        ifstream ifs(filename);
        if (!ifs) {
            cerr << "Error: Could not open file for loading." << '\n';
            return;
        }

        students.clear();
        string line;
        while (getline(ifs, line)) {
            if (!line.empty()) { 
                students.push_back(Student::deserialize(line));
            }
        }

        ifs.close();
    }

    void search_by_ID(const int id, const string& filename){
        ifstream ifs(filename);
        if(!ifs){
            cerr<<"Error: Could not open file for loading"<<'\n';
            return;
        }
        students.clear();
        string line;
        while(getline(ifs, line)){
            if(!line.empty())
                students.push_back(Student::deserialize(line));
        }
        for(const auto& stud : students){
            if(stud.getID() == id){
                stud.display();
            }
        }
    }

    void clearDatabase(const string& filename) {
        students.clear();
        clearFile(filename);
    }

private:
    void clearFile(const string& filename) {
        ofstream ofs(filename, ios::trunc);
        if (!ofs) {
            cerr << "Error: Could not open file for clearing." << '\n';
            return;
        }
        ofs.close();
        cout << "All data cleared from "<< '\n';
    }
};

int main() {
    Database db;
    db.load_from_file("studdata.csv");

    int input;
    do {
        cout << "Enter 1 to add data into database:\n";
        cout << "Enter 2 to display data:\n";
        cout << "Enter 3 to clear all the data:\n";
        cout << "Enter 4 to search by ID:\n";
        cout << "Enter 5 to stop: "<<"\n";
        cin >> input;

        int id;
        string name;
        int age;
        switch (input) {
            case 1: {
                cout << "Enter Student ID:\n";
                cin >> id;
                if(cin.fail()){
                    cerr<<"Enter a valid ID "<<'\n';
                    break;
                }
                cout << "Enter your name:\n";
                cin.ignore();
                getline(cin, name);
                cout << "Enter your age:\n";
                cin >> age;
                if(cin.fail()){
                    cerr<<"Enter a valid age "<<'\n';
                }
                cin.ignore();
                Student student(id, name, age);
                db.addStudent(student, "studdata.csv");
                break;
            }
            case 2: {
                db.displayAll();
                break;
            }
            case 3: {
                db.clearDatabase("studdata.csv");
                break;
            }
            case 4: {
                cout<<"Enter the ID: "<<"\n";
                cin.ignore();
                int id;
                cin>>id;
                db.search_by_ID(id,"studdata.csv");
                break;
            }
            case 5:{
                cout<<"Exiting the program thanks!"<<"\n";
                break;
            }
            default: {
                cout << "Enter a valid input!!\n";
                break;
            }
        }
    } while (input != 5);
}
