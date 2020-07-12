# SWEIDMT SS2020 Term Project

A program to plan a Femoral neck fracture surgeury from CT files.

## Overview
![Alt text](/Screenshot/GUI-Overview-Test.png?raw=true)

## Getting Started

Download the project using the QT project import feature.

### Features

(In German)
See the [Pflichenheft](/Abgabe/Dokumentation/Pflichten-Heft.md):
Abgabe/Dokumentation/Lasten-Heft.md

### Librarys

Eigen 3.3.7

### Documentation

* [Lastenheft](/Abgabe/Dokumentation/Lasten-Heft.md):
Abgabe/Dokumentation/Lasten-Heft.md

* [Pflichenheft](/Abgabe/Dokumentation/Pflichten-Heft.md):
Abgabe/Dokumentation/Lasten-Heft.md

* Risiko Analyse:
Abgabe/Dokumentation/RiskAnalysis_SWEIDMT_SS2020.xlsm
### Project structure

##### MyLib
MyLib has the codes which will can be usefull for other developers.

##### app
main is for starting the application.
app has the data of the loaded files and GUI.

###### DLL
app/Applicationdata will save all the Data from the loaded files and also temporary coordinates selected by the user.
###### GUI
app/sweidmt has all the GUI-widgets for the surgery. For this project only a Bore Viewer is planned.
app/imageLoader has all the GUI-widgets for the Bore Viewer


## Authors

* **Danial Hezarkhani** - *Development*

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
