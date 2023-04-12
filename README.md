# InfoHub

InfoHub to aplikacja desktopowa napisana w języku C++ z wykorzystaniem biblioteki Qt6. Aplikacja służy do wyświetlania wiadomości pobieranych z kanałów RSS/Atom. Użytkownik ma możliwość konfiguracji aplikacji, takiej jak zmiana języka interfejsu, melodii powiadomienia, częstotliwości automatycznego usuwania wiadomości oraz aktualizacji wiadomości. Ponadto użytkownik może dodawać kategorie wiadomości oraz źródła, z których te wiadomości będą pobierane. Każda kategoria może mieć własną ikonę. Wyświetlane wiadomości zbudowane są z tytułu, opisu oraz zdjęcia. Artykuł można ukryć, dodać do ulubionych lub po prostu w niego kliknąć, dzięki czemu przeniesieni zostaniemy do strony internetowej, gdzie znajduje się pełna wersja artykułu.

## Wymagania

Do uruchomienia aplikacji InfoHub wymagane są:

- Qt6
- C++17
- Kompilator C++ (np. gcc, clang)
- CMake
- Make

## Budowanie i uruchamianie

>Linux:

1. Sklonuj repozytorium.
2. Przejdź do katalogu z repozytorium i utwórz katalog `build`.
3. Przejdź do katalogu `build` i wykonaj polecenie `cmake ..` aby wygenerować pliki projektu.
4. Wykonaj polecenie `make -j5` aby zbudować projekt.
5. Aby uruchomić aplikację, wykonaj polecenie `./infohub`.

>Windows:

1. Sklonuj repozytorium.
2. Przejdź do katalogu z repozytorium i utwórz katalog `build`.
3. Przejdź do katalogu `build` i wykonaj polecenie `cmake -G"Unix Makefiles" -DCMAKE_MAKE_PROGRAM=<ścieżka do pliku make.exe> ..` aby wygenerować pliki projektu.
4. Wykonaj polecenie `ścieżka/do/pliku/make.exe -j5` aby zbudować projekt. 
5. Utwórz w dowolnym miejscu katalog `infohub-release`.
6. Otrzymany plik `infohub.exe` z kroku `4` przenieś do katalogu z kroku `5`.
7. W bibliotece `Qt6` istnieje narzędzie o nazwie `windeployqt6.exe`. Użyj tego narzędzia za pomocą polecenia: `ścieżka/do/programu/windeployqt6.exe infohub.exe`
8. Wygenerowały się wszystkie potrzebne pliki do uruchomienia aplikacji. Można uruchomić aplikację `infohub.exe`.

>Uwaga!

W przypadku problemów z wykrywaniem biblioteki Qt6, spróbuj dodać do pliku CMakeFiles.txt instrukcje definiujące `CMAKE_PREFIX_PATH` oraz `Qt6_DIR`. Przykład dla Qt zainstalowanym w systemie Windows:

`set(CMAKE_PREFIX_PATH "C:/Qt/6.5.0/mingw_64")`

`set(Qt6_DIR "C:/Qt/6.5.0/mingw_64/lib/cmake/Qt6")`

Ten zabieg powinien sprawić, że `cmake` zacznie widzieć bibliotekę Qt oraz jej moduły. 

## Struktura katalogów aplikacji

Po uruchomieniu aplikacji, nastąpi utworzenie następujących katalogów:

- assets - Katalog przechowujący ikony wgrane do programu. Ikony przeznaczone są do zmiany wyglądu kategori wiadomości.
- audio - Użytkonik chcący wgrać swój dźwięk powiadomień, powinien wrzucić plik dźwiękowy z rozszerzeniem `.wav` w te miejsce. Po uruchomieniu aplikacji lub przeładowaniu zakładki ustawień, plik zostanie wykryty i można będzie go wybrać w opcjach.
- cache - Katalog który przechowuje pobrane ze źródeł miniaturki do artykułów. Ten katalog jest skanowany i czyszczony przy uruchomieniu aplikacji, po spełnieniu odpowiednich warunków.
- lang - Aplikacja posiada wbudowaną obsługę dwóch języków - polski oraz angielski. W przypadku gdy użytkownik chce mieć ten program w swoim języku, to przetłumaczony plik JSON musi umieścić do tego katalogu. Po przeładowaniu widoku ustawień tłumaczenie powinno być dostępne w opcjach do wyboru. 


## Lista zagadnień związanych z aplikacją
- Operacje sieciowe (żądania HTTP)
- Operacje na plikach
- Operacje na JSON
- Operacje na XML
- Tworzenie GUI za pomocą Qt6
- Zarządanie pamięcią
- Podejście obiektowe (OOP)
- Wyrażenia lambda
- Wykorzystanie mechaniki Signals/Slots będącej częścią biblioteki Qt
- Wielowątkowość (skanowanie i pobieranie nowych artykułów w tle aplikacji)
- Operacje na kontenerach, iteratorach
- Sortowanie, filtrowanie, itp...
- Wieloplatformowość (testowane na Linux oraz Windows)