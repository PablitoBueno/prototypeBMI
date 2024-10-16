#include <windows.h>
#include <string>
#include <sstream>
#include <fstream> // For saving the report to a file
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <stdexcept>

using namespace std;

// Class to represent a person
class Person {
private:
    float height, weight, bmi, bodyFatPercentage;
    int age;
    string name, gender;
    bool hasBodyFat;

    // Internal function to calculate the BMI
    void calculateBMI() {
        bmi = weight / (height * height);
    }

public:
    Person() : hasBodyFat(false), age(0), bmi(0.0), bodyFatPercentage(0.0) {}

    // Setters
    void setName(const string& n) { name = n; }
    void setHeight(float h) { height = h; calculateBMI(); }  // Updates BMI automatically when setting height
    void setWeight(float w) { weight = w; calculateBMI(); }  // Updates BMI automatically when setting weight
    void setAge(int a) { age = a; }
    void setGender(const string& g) { gender = g; }
    void setBodyFat(float bf) {
        if (bf >= 0) {
            bodyFatPercentage = bf / 100.0f; 
            hasBodyFat = true;
        } else {
            hasBodyFat = false;
        }
    }

    // Getters
    string getName() const { return name; }
    float getHeight() const { return height; }
    float getWeight() const { return weight; }
    int getAge() const { return age; }
    string getGender() const { return gender; }
    float getBMI() const { return bmi; }
    bool hasBodyFatData() const { return hasBodyFat; }
    float getBodyFatPercentage() const { return bodyFatPercentage; }

    // Returns the BMI category based on the current value
    string getBMICategory() const {
        if (bmi < 17) return "Very underweight";
        else if (bmi >= 17 && bmi < 18.49) return "Underweight";
        else if (bmi >= 18.5 && bmi < 24.99) return "Normal weight";
        else if (bmi >= 25 && bmi < 29.99) return "Overweight";
        else if (bmi >= 30 && bmi < 34.99) return "Obese (Grade I)";
        else if (bmi >= 35 && bmi < 39.99) return "Obese (Grade II)";
        else return "Obese (Grade III)";
    }
};

// Class for the calculator interface
class Interface {
private:
    HWND hWnd, hName, hHeight, hWeight, hAge, hGender, hBodyFat;
    Person person;

    static LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
    void addControls(HWND);
    void calculateBMI();
    void generateReport(); // New function to generate the report
    void showError(const string& message);
    void showBMIDialog(const string& result);
    float parseInput(const string& input);

public:
    Interface(HINSTANCE hInst);
    void show();
};

// Constructor of the Interface class
Interface::Interface(HINSTANCE hInst) {
    WNDCLASS wc = { 0 };
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = "BMIWindowClass";
    wc.lpfnWndProc = WindowProcedure;
    RegisterClass(&wc);

    hWnd = CreateWindow("BMIWindowClass", "BMI and Body Fat Calculator",
                        WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 400, 450,
                        NULL, NULL, hInst, this);
}

void Interface::show() {
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Interface::showError(const string& message) {
    MessageBox(hWnd, message.c_str(), "Input Error", MB_ICONERROR | MB_OK);
}

void Interface::showBMIDialog(const string& result) {
    MessageBox(hWnd, result.c_str(), "BMI Result", MB_OK | MB_ICONINFORMATION);
}

float Interface::parseInput(const string& input) {
    string processedInput = input;
    replace(processedInput.begin(), processedInput.end(), ',', '.');
    float value;
    stringstream ss(processedInput);
    ss >> value;
    if (ss.fail() || !ss.eof()) {
        showError("Invalid number format!");
        return -1;
    }
    return value;
}

// Function to calculate the BMI and display the result
void Interface::calculateBMI() {
    char name[30], height[10], weight[10], age[10], bodyFat[10];
    int genderSelected = SendMessage(hGender, CB_GETCURSEL, 0, 0);
    GetWindowText(hName, name, 30);
    GetWindowText(hHeight, height, 10);
    GetWindowText(hWeight, weight, 10);
    GetWindowText(hAge, age, 10);
    GetWindowText(hBodyFat, bodyFat, 10);
    
    string nameStr(name), heightStr(height), weightStr(weight), bodyFatStr(bodyFat), ageStr(age);
    float heightVal = parseInput(heightStr);
    float weightVal = parseInput(weightStr);
    float bodyFatVal = bodyFatStr.empty() ? -1 : parseInput(bodyFatStr);
    int ageVal = ageStr.empty() ? 0 : atoi(ageStr.c_str());
    string genderStr = (genderSelected == 0) ? "Male" : "Female";
    
    if (heightVal <= 0 || weightVal <= 0) {
        showError("Please enter valid height and weight values!");
        return;
    }
    
    person.setName(nameStr);
    person.setHeight(heightVal);
    person.setWeight(weightVal);
    person.setAge(ageVal);
    person.setGender(genderStr);
    person.setBodyFat(bodyFatVal);
    
    stringstream resultStream;
    resultStream << "Your BMI is: " << person.getBMI() << "\n" << person.getBMICategory();
    if (person.hasBodyFatData()) {
        resultStream << "\nBody Fat Percentage: " << person.getBodyFatPercentage() * 100 << "%";
    } else {
        resultStream << "\nNo Body Fat Percentage provided.";
    }
    showBMIDialog(resultStream.str());
}

// Function to generate the report in a text file
void Interface::generateReport() {
    ofstream reportFile("BMI_Report.txt");
    if (reportFile.is_open()) {
        reportFile << "BMI and Body Fat Report\n";
        reportFile << "-----------------------\n";
        reportFile << "Name: " << person.getName() << "\n";
        reportFile << "Height: " << person.getHeight() << " meters\n";
        reportFile << "Weight: " << person.getWeight() << " kg\n";
        reportFile << "Age: " << person.getAge() << " years\n";
        reportFile << "Gender: " << person.getGender() << "\n";
        reportFile << "BMI: " << person.getBMI() << "\n";
        reportFile << "BMI Category: " << person.getBMICategory() << "\n";
        if (person.hasBodyFatData()) {
            reportFile << "Body Fat Percentage: " << person.getBodyFatPercentage() * 100 << "%\n";
        } else {
            reportFile << "Body Fat Percentage: Not provided\n";
        }
        reportFile.close();
        MessageBox(hWnd, "Report generated successfully!", "Success", MB_OK | MB_ICONINFORMATION);
    } else {
        MessageBox(hWnd, "Error opening the file for writing!", "Error", MB_OK | MB_ICONERROR);
    }
}

// Function to add controls to the interface
void Interface::addControls(HWND hWnd) {
    CreateWindow("static", "Name:", WS_VISIBLE | WS_CHILD, 50, 20, 150, 20, hWnd, NULL, NULL, NULL);
    hName = CreateWindow("edit", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 20, 150, 20, hWnd, NULL, NULL, NULL);
    CreateWindow("static", "Height (in meters):", WS_VISIBLE | WS_CHILD, 50, 70, 150, 20, hWnd, NULL, NULL, NULL);
    hHeight = CreateWindow("edit", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 70, 100, 20, hWnd, NULL, NULL, NULL);
    CreateWindow("static", "Weight (in kg):", WS_VISIBLE | WS_CHILD, 50, 120, 150, 20, hWnd, NULL, NULL, NULL);
    hWeight = CreateWindow("edit", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 120, 100, 20, hWnd, NULL, NULL, NULL);
    CreateWindow("static", "Age:", WS_VISIBLE | WS_CHILD, 50, 170, 150, 20, hWnd, NULL, NULL, NULL);
    hAge = CreateWindow("edit", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 170, 50, 20, hWnd, NULL, NULL, NULL);
    CreateWindow("static", "Gender:", WS_VISIBLE | WS_CHILD, 50, 220, 150, 20, hWnd, NULL, NULL, NULL);
    hGender = CreateWindow("combobox", "", WS_VISIBLE | WS_CHILD | WS_BORDER | CBS_DROPDOWNLIST, 200, 220, 100, 20, hWnd, NULL, NULL, NULL);
    SendMessage(hGender, CB_ADDSTRING, 0, (LPARAM)"Male");
    SendMessage(hGender, CB_ADDSTRING, 0, (LPARAM)"Female");
    SendMessage(hGender, CB_SETCURSEL, 0, 0); // Set default selection to Male
    CreateWindow("static", "Body Fat (%):", WS_VISIBLE | WS_CHILD, 50, 270, 150, 20, hWnd, NULL, NULL, NULL);
    hBodyFat = CreateWindow("edit", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 270, 100, 20, hWnd, NULL, NULL, NULL);
    CreateWindow("button", "Calculate BMI", WS_VISIBLE | WS_CHILD, 50, 320, 100, 30, hWnd, (HMENU)1, NULL, NULL);
    CreateWindow("button", "Generate Report", WS_VISIBLE | WS_CHILD, 200, 320, 120, 30, hWnd, (HMENU)2, NULL, NULL);
}

// Window Procedure to handle messages
LRESULT CALLBACK Interface::WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static Interface* instance = nullptr;
    if (msg == WM_NCREATE) {
        instance = reinterpret_cast<Interface*>(lParam);
    }
    if (instance) {
        switch (msg) {
        case WM_COMMAND:
            if (LOWORD(wParam) == 1) {
                instance->calculateBMI();
            }
            else if (LOWORD(wParam) == 2) {
                instance->generateReport(); // Handle report generation
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        }
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

// Main function of the application
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    Interface app(hInstance);
    app.addControls(app.hWnd);
    app.show();
    return 0;
}
