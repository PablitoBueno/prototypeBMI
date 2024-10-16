# BMI and Body Fat Calculator

## Description

This is a desktop application written in C++ that calculates a person's Body Mass Index (BMI) and body fat percentage. The application features a simple graphical user interface (GUI) that allows users to enter personal information such as name, height, weight, age, gender, and body fat percentage. Based on this data, it calculates and displays the BMI and the corresponding category, as well as generates a text report with the results.

## Features

- **BMI Calculation**: The application calculates BMI based on the entered height and weight.
- **BMI Classification**: It provides the BMI category (e.g., underweight, normal, overweight, obese).
- **Body Fat**: Allows input of body fat percentage and includes it in the calculation.
- **Report Generation**: Generates a detailed text report that includes all entered information and calculation results.
- **Graphical Interface**: The application has a user-friendly interface built using the Windows API.

## Prerequisites

- **C++ Compiler**: To compile and run the code, you need a C++ compiler that supports the Windows API, such as MinGW or Microsoft Visual Studio.
- **Operating System**: The application is designed for Windows.

## Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/PablitoBueno/prototypeBMI.git
Compile the code: Using MinGW:

g++ -o BMICalculator main.cpp -mwindows
Or using Visual Studio, create a new project and add the source code to the project.

Run the application: After compiling, execute the generated executable file BMI_App.exe.

Usage
Interface:
Fill in the input fields:

Name: Your name.
Height: Your height in meters (e.g., 1.75).
Weight: Your weight in kilograms (e.g., 70).
Age: Your age in years.
Gender: Select your gender (Male or Female).
Body Fat (%): Your body fat percentage (optional).
Calculate BMI: Click the "Calculate BMI" button to calculate your BMI and display the result.

Generate Report: Click the "Generate Report" button to create a text file named BMI_Report.txt that contains all the entered data and the results.

Example of Generated Report

BMI and Body Fat Report
-----------------------
Name: John Doe
Height: 1.75 meters
Weight: 70 kg
Age: 25 years
Gender: Male
BMI: 22.86
BMI Category: Normal weight
Body Fat Percentage: 15.0

Limitations
This application is designed for educational purposes only and is a project created by a student learning computer science.
The application is not intended for commercial use and may not meet medical standards for health assessments.
Contributions
Feel free to fork the project and submit pull requests for improvements, bug fixes, or new features. mas como eu ajusto a essa descrição?
