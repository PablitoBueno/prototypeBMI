#include <windows.h>
#include <string>
#include <sstream>
#include <fstream> // Para salvar o relatório em arquivo
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <stdexcept>

using namespace std;

// Classe para representar a pessoa
class Person {
private:
    float height, weight, bmi, bodyFatPercentage;
    int age;
    string name, gender;
    bool hasBodyFat;

    // Função interna para calcular o BMI
    void calculateBMI() {
        bmi = weight / (height * height);
    }

public:
    Person() : hasBodyFat(false), age(0), bmi(0.0), bodyFatPercentage(0.0) {}

    // Setters
    void setName(const string& n) { name = n; }
    void setHeight(float h) { height = h; calculateBMI(); }  // Atualiza BMI automaticamente ao definir a altura
    void setWeight(float w) { weight = w; calculateBMI(); }  // Atualiza BMI automaticamente ao definir o peso
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

    // Retorna a categoria do BMI com base no valor atual
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

// Classe para a interface da calculadora
class Interface {
private:
    HWND hWnd, hName, hHeight, hWeight, hAge, hGender, hBodyFat;
    Person person;

    static LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
    void addControls(HWND);
    void calculateBMI();
    void generateReport(); // Nova função para gerar o relatório
    void showError(const string& message);
    void showBMIDialog(const string& result);
    float parseInput(const string& input);

public:
    Interface(HINSTANCE hInst);
    void show();
};

// Construtor da classe Interface
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

// Função para calcular o BMI e exibir o resultado
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

// Função para gerar o relatório em arquivo de texto
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

// Função para adicionar controles na interface
void Interface::addControls(HWND hWnd) {
    CreateWindow("static", "Name:", WS_VISIBLE | WS_CHILD, 50, 20, 150, 20, hWnd, NULL, NULL, NULL);
    hName = CreateWindow("edit", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 20, 150, 20, hWnd, NULL, NULL, NULL);
    CreateWindow("static", "Height (in meters):", WS_VISIBLE | WS_CHILD, 50, 70, 150, 20, hWnd, NULL, NULL, NULL);
    hHeight = CreateWindow("edit", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 70, 100, 20, hWnd, NULL, NULL, NULL);
    CreateWindow("static", "Weight (in kg):", WS_VISIBLE | WS_CHILD, 50, 120, 150, 20, hWnd, NULL, NULL, NULL);
    hWeight = CreateWindow("edit", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 120, 100, 20, hWnd, NULL, NULL, NULL);
    CreateWindow("static", "Age:", WS_VISIBLE | WS_CHILD, 50, 170, 150, 20, hWnd, NULL, NULL, NULL);
    hAge = CreateWindow("edit", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 170, 100, 20, hWnd, NULL, NULL, NULL);
    CreateWindow("static", "Gender:", WS_VISIBLE | WS_CHILD, 50, 220, 150, 20, hWnd, NULL, NULL, NULL);
    hGender = CreateWindow("combobox", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWN, 200, 220, 100, 100, hWnd, NULL, NULL, NULL);
    SendMessage(hGender, CB_ADDSTRING, 0, (LPARAM)"Male");
    SendMessage(hGender, CB_ADDSTRING, 0, (LPARAM)"Female");
    SendMessage(hGender, CB_SETCURSEL, 0, 0); // Preselect "Male"
    CreateWindow("static", "Body Fat (%):", WS_VISIBLE | WS_CHILD, 50, 270, 150, 20, hWnd, NULL, NULL, NULL);
    hBodyFat = CreateWindow("edit", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 270, 100, 20, hWnd, NULL, NULL, NULL);
    
    CreateWindow("button", "Calculate BMI", WS_VISIBLE | WS_CHILD, 50, 320, 150, 30, hWnd, (HMENU)1, NULL, NULL);
    CreateWindow("button", "Generate Report", WS_VISIBLE | WS_CHILD, 200, 320, 150, 30, hWnd, (HMENU)2, NULL, NULL);
}

// Callback da janela
LRESULT CALLBACK Interface::WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    Interface* pThis;
    if (msg == WM_CREATE) {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lp;
        pThis = (Interface*)pCreate->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);
        pThis->addControls(hWnd);
    } else {
        pThis = (Interface*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
        if (!pThis) return DefWindowProc(hWnd, msg, wp, lp);
    }
    
    switch (msg) {
    case WM_COMMAND:
        switch (wp) {
        case 1:
            pThis->calculateBMI();
            break;
        case 2:
            pThis->generateReport();
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, wp, lp);
    }
    return 0;
}

// Função main para criar a interface
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    Interface app(hInst);
    app.show();
    return 0;
}

