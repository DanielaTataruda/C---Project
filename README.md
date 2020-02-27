# C - Project

## Opis aplikacji
Projekt został napisany w ramach kursu Wstęp do programowania w języku C(Styczeń 2018).
Program ma na celu generowanie kolorowych map fizycznych z map wysokości,
które użytkownik może wybrać z plików HGT. Aplikacja umożliwia generowanie
pojedyńczego miejsca (obszaru) lub wybranego kontynentu, dostępny jest również
wariant wygenerowania większego terenu poprzez podanie współrzędnych skrajnych
punktów. Wygenerowane mapy, będą zapisywanie w formacie PNG i nazwie
adekwatnej do nazwy kontynentu lub takiej jaką użytkownik poda dla własnego
obszaru.

Aplikacja do generowania map fizycznych składa się z dwóch okien.

* `Mapy` - jest to główne okno posiadające 7 przycisków, każdy z nich umożliwia
użytkownikowi wygenerowanie innego obszaru.

1. **"WSPÓŁRZEDNE GEOGRAFICZNE"** - ta funkcja umożliwia wygenerowanie
małego obszaru wybranego przez użytkownika poprzez wpisanie
współrzędnych geograficznych tego miejsca w podanym przez program
formacie.

2. **"WŁASNY OBSZAR"** - ta funkcja umożliwia wygenerowanie większego
obszaru poprzez wpisanie skrajnych współrzędnych tego miejsca. Wartości
należy wpisywać zgodnie z instrukcją wyświetlaną przez program.
S/W podajemy jako wartość ujemna, N/E podajemy jako wartość nieujemną.

3. **"EUROPA"** - przycisk automatycznie generuje mapę Europy.

4. **"AFRYKA"** - przycisk automatycznie generuje mapę Afryki.

5. **"AZJA"** - przycisk automatycznie generuje mapę Azji.

6. **"AMERYKI"** - przycisk automatycznie generuje mapę Ameryki Południowej
i Północnej.

7. **"AUSTRALIA"** - przycisk automatycznie generuje mapę Australii.

Każdy przycisk po wygenerowaniu automatycznie wyświetla mapę i zapisuje
na komputerze pod nazwą (w przypadku własnego obszaru sami podajemy
nazwe w przeciwnym wypadku program zapisuje pod nazwą zadanego kontynentu).

* `Paleta kolorów` - jest to drugie okno, które umożliwia wybranie zestawu
kolorystycznego w jakim będzie generowana mapa. Użytkownik ma do
wyboru 5 wariantów kolorystycznych określających (niziny, wyżyny, góry):

1. **Domyślny** (zielony, zółty, czerwony)

2. **Odwrócenie** (czerwony, zółty, zielony)

3. **Kobiecy** (odcienie różu)

4. **Czarno-biały** (biały, szary, czarny)

5. **Inwersja** (odwrócenie domyślnych kolorów)

Woda i martwe/nieznane pixele pozostają w standardowych kolorach, kolejno
niebieski i biały.

Aby zakończyć program wystarczy nacisnąć przycisk "X" znajdujący się w lewym
górnym rogu głównego okna `Mapy`. Taki sam przycisk znajduje się w oknie `Paleta
kolorów`, ale wciśnięcie go nie zamyka aplikacji tylko uniemożliwia wybranie
wariantów kolorystycznych(mapa wygeneruje się w ostatnim zapamiętanym wariancie)
