BMI and Body Fat Calculator
Description
This is a desktop application written in C++ that calculates a person's Body Mass Index (BMI) and body fat percentage. The application includes a simple graphical user interface (GUI) where users can input their name, height, weight, age, gender, and body fat percentage. Based on this data, the application calculates and displays the BMI and the corresponding category, and generates a text report with all the results.

Features
BMI Calculation: Calculates BMI based on the input height and weight.
BMI Classification: Displays the BMI category (e.g., underweight, normal weight, overweight, obese).
Body Fat Input: Allows the user to input body fat percentage and includes it in the report.
Report Generation: Automatically generates a detailed text report with all entered data and results.
Graphical Interface: Features a simple and user-friendly interface built using the Windows API.
Prerequisites
C++ Compiler: To compile and run the code, you will need a C++ compiler that supports the Windows API, such as MinGW or Microsoft Visual Studio.
Operating System: This application is designed for Windows platforms.
Installation
Clone the repository:

bash
Copiar código
git clone https://github.com/PablitoBueno/prototypeBMI.git
cd prototypeBMI
Compile the code:

Using MinGW:

bash
Copiar código
g++ -o BMICalculator main.cpp -mwindows
Or using Visual Studio: Create a new project, add the source code (main.cpp), and compile it within the IDE.

Run the application: After compiling, execute the generated BMICalculator.exe file.

Usage
Interface:
Fill in the input fields:

Name: Enter your name.
Height: Input your height in meters (e.g., 1.75).
Weight: Input your weight in kilograms (e.g., 70).
Age: Enter your age in years.
Gender: Select your gender (Male or Female).
Body Fat (%): Optionally, input your body fat percentage.
Calculate BMI:

Click the "Calculate BMI" button to calculate and display your BMI and its classification.
Generate Report:

Click the "Generate Report" button to create a text file named BMI_Report.txt, which will contain all entered data and the calculation results.
Example of Generated Report:
BMI and Body Fat Report
-----------------------
Name: John Doe
Height: 1.75 meters
Weight: 70 kg
Age: 25 years
Gender: Male
BMI: 22.86
BMI Category: Normal weight
Body Fat Percentage: 15.0%

Limitations
This project is created for educational purposes only and is part of a student's learning experience in computer science.

It is not intended for medical or commercial use.
The calculations and classifications may not adhere to medical standards for health assessments.
Contributions
Feel free to fork this project and submit pull requests for improvements, bug fixes, or new features.
