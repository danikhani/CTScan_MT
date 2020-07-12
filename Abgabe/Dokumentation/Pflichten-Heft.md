# Pflichtenheft

## Ziel:
**Umsetzung der Anforderungen aus dem Lastenheft**

## 1 Umsetzungen:

1. Die Software wird mit c++ geschrieben. (Lastenheft 1.1, 1.2)
2. Der Quellcode wird in QTCreator kompiliert. (Lastenheft 4.3)
3. Die Software wird object orientalisch und Modular aufgebaut. Zusätzlich in einer getrennten Bibliothek werden die wiederverwendbaren Quellcodes ausgelagert. (Lastenheft 1.1, 1.2)
4. Die Vektorrechnung werden mithilfe von der zusätzlichen freien Bibliothek "Eigen" durchgeführt. (Lastenheft 1.1, 1.2, 4.4)
5. Für GUI und DLL Elementen werden jeweils in getrennten Classen definiert. (Lastenheft 1.1, 1.2)
5. Der Entwicklungsprozess wurde dokumentiert und mit git verwaltet. (Lastenheft 4.1, 4.2)

### 1.1 GUI-Umsetzungen

1. Das Bild wird in zwei 2D-Ansichten projiziert, damit der Benutzer  einfacher die Punkte in einem 3D-Raum mit mouse click auswählen kann. (Lastenheft 2.1)
2. Die Koordinaten von den Startpunkt und Endpunkt können auch Manuel eingegeben werden.(Lastenheft 2.1, Lastenheft 3.2)
3. Die HU-Werte, Fensterung und Threshhold der Ansichten sind mithilfe von Slidern einstellbar. (Lastenheft 3.3)
4. Die Bohrungslänge kann nach der Wahl der Start und Endpunkte direkt berechnet werden. (Lastenheft 2.2)
5. Die Ebenen orthogonal zur Bohrungsachse werden mit "Mindestens nearest neighbor methode" dargestellt. Diese Ebene kann sich mit einem Slider bewegen. (Lastenheft 2.3, Lastenheft 3.3)
6. Die Bohrungsdruchmesser kann vom Benutzer ausgewählt und in der orthogonalen Ansicht zur Bohrungsachse angezeigt. (Lastenheft 2.3)
7. Die orthogonalen Ansicht kann mithilfe von einem Slider vergrößert werden. (Lastenheft 3.2, 3.3, 2.5)
8. Die orthogonalen Ansicht kann mithilfe von einem Slider gedreht werden. (Lastenheft 3.2, 3.3, 2.5)
9. Die Fehler werden in einem Textbox auf der linken Seite des GUIs angezeigt.(Lastenheft 3.1)
10. Die Anleitungsschritte werden in einem Textbox auf der linken Seite des GUIs angezeigt.(Lastenheft 3.1)


### 1.2 DLL-Umsetzung

1. Pflichtenheft 1.1 Koordinaten werden hier übertragen und in zwei Vektoren gespeichert (Lastenheft 2.1)
2. Die Skalierung von einem Voxel wird in DLL gespeichert. (Lastenheft 3.3, 1.1, 1.2)
2. Die Arrays mit RAW-Daten werden in DLL gespeichert. (Lastenheft 3.3, 1.1, 1.2)
2. Abstand der zwei Punkten wird als Norm von dem Bohrungsvektor gerechnet. (Lastenheft 2.2)


