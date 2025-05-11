# I. Cerintele proiectului

```
Teme de proiect:
Liste
1. Scrieți un program care definește tipul de date necesar pentru gestionarea
unei liste cu angajații dintr-o firmă și implementează operațiile asupra listei.
2. Scrieți un program care definește tipul de date necesar pentru gestionarea
unei liste cu medicamentele dintr-o farmacie și implementează operațiile
asupra listei.
Stiva
3. Scrieți un program care definește tipul de date necesar pentru gestionarea
unei stive cu pachetele dintr-un depozit și implementează operațiile asupra
stivei.
4. Scrieți un program care definește tipul de date necesar pentru gestionarea
unei stive cu trenuri dintr-un depou și implementează operațiile asupra stivei.
5. Scrieți un program care definește tipul de date necesar pentru gestionarea
unei stive de cărți dintr-un teanc și implementează operațiile asupra stivei.
6. Scrieți un program care definește tipul de date necesar pentru gestionarea
unei stive de mașini care sunt transportate cu un bac și implementează
operațiile asupra stivei.
Coada
7. Scrieți un program care definește tipul de date necesar pentru gestionarea
unei cozi cu programările la un cabinet medical și implementează operațiile
asupra cozii.
8. Scrieți un program care definește tipul de date necesar pentru gestionarea
unei cozi cu programările unor spectacole într-o sala și implementează
operațiile asupra cozii.
9. Scrieți un program care definește tipul de date necesar pentru gestionarea
unei cozi cu programările candidaților la un examen și implementează
operațiile asupra cozii.
10. Scrieți un program care definește tipul de date necesar pentru gestionarea
unei cozi cu clienții de la o cantină și implementează operațiile asupra cozii.
```

```cpp
///model de meniu pentru programele propuse
do {
// Check the return value of system()
    if (system("CLS"))
        system("clear");

    cout << "Operatii cu stiva" << endl;
    cout << "MENIU" << endl;
    cout << "1. Creare stiva vida" << endl;
    cout << "2. Adaugare in stiva(push)" << endl;

    cout << "3. Extragere din stiva(pop)" << endl;
    cout << "4. Afisare elemente din stiva" << endl;
    cout << "5. EXIT" << endl;
    cout << "__" << endl;
    cout << "Ce optiune doriti din meniu?" << endl;
    cin >> n;

    switch(n)
    {
        case 1:
            creareStivaVida(st,vf);
        case 2:
            adaugareStiva(st,vf);
            break;
        case 3:
            extragereStiva(st,vf);;
            break;
        case 4:
            afisareStiva(st,vf);
            break;
        case 5:
            cout<<"Problema incheiata";
            break;
        default:
            cout<<"Optiune inexistenta";
            break;
    }
} while (n!=5);

```

# II. Explicarea solutiei

Intregul proiect este scris intr-un singur fisier c++.
Include implementari ale tipurilor `Stiva` si `Coada`, impruna cu un meniu care poate fi folosit cu oricare din cele doua structuri de date.

### A. Structuile de date

Pentru implementarea tipurilor `Stack` ( stiva ) si `Queue` ( coada ) am folosit aritmetica simpla cu pointeri

Print intermediul conceptelor ( introduse in c++20 ), m-am asigurat ca orice structura de data neomogena `T` care doreste sa fie folosita in stiva sau coada
implementeaza urmatoarele functii:

| Conceptul      | Return type-ul | Numele functiei | Argumente                    | Descriere                                                                    |
| -------------- | -------------- | --------------- | ---------------------------- | ---------------------------------------------------------------------------- |
| InputBuildable | static T       | BuildFromInput  | -                            | Creaza o noua instanta a lui `T` folosind date<br />introduse de utilizator. |
| Validatable    | bool           | IsValid         | -                            | Asigura validitatea datelor instantei care o apeleaza                        |
| Streamable     | std::ostream&  | operator <<     | std::ostream& os, const T& t | Asigura ca structura poate fi afisata pe ecran                               |
| Assignable     | T&             | operator =      | const T& other               | Asigura ca structura  poate fi mutata / copiata                              |

### B. CLI

Tot prin intermediul conceptelor am asigurat ca o structura de date poate fi folosita impreuna cu meniul doar daca implementeaza urmatoarele:

| Conceptul           | Return type-ul | Numele     | Argumente  | Descriere                                                          |
| ------------------- | -------------- | ---------- | ---------- | ------------------------------------------------------------------ |
| Pushable            | void           | Push       | const T& t | Asigura ca la structura de date pot fi adaugate elemente           |
| Poppable            | void           | Pop        | -          | Asigura ca din structura de date poate fi eliminat ultimul element |
| Clearable           | void           | Clear      | -          | Asigura ca structura de date poate fi golita                       |
| EmptyVerfiable      | bool           | IsEmpty    | -          | Verifica daca structura de date este goala sau nu                  |
| LastMemberAccesible | T&             | LastMember | -          | Asigura ca ultimul element al structurii de date poate fi accesat  |

# III. Rezolvarea problemelor

Pentru a rezolva oricare problema din cele cerute, trebuie urmati acesti pasi:

1. Implementarea unei structuri de date care respecta toate conceptele primului tabel
2. Creerea unei instante a clasei `CLI` cu structura de date dorita ( atat stiva cat si coada pot fi folosite ca lista ).
3. Apelarea functiei `Loop` a instantei `CLI`

Spre exemplu, direct din `main.cpp`:

```cpp
#include <iostream>
//...

#define MAX_STR_LENGTH 256

struct Candidat
{
    char name[MAX_STR_LENGTH];
    unsigned short age;
    unsigned short appointmentTime;

    Candidat &operator=(const Candidat &other)
    {
        std::strcpy(name, other.name);
        appointmentTime = other.appointmentTime;
        age = other.age;
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const Candidat &candidat)
    {
        return os << '(' << candidat.name << ',' << candidat.age << ',' << candidat.appointmentTime << ')';
    }

    static Candidat BuildFromInput()
    {
        Candidat c;

        std::cout << "Scrie numele complet al candidatului: ";
        std::cin.getline(c.name, MAX_STR_LENGTH);
        c.name[MAX_STR_LENGTH - 1] = '\0';

        std::cout << "Scrie varsta candidatului: ";
        std::cin >> c.age;

        std::cout << "Scrie ora examenului: ";
        std::cin >> c.appointmentTime;

        return c;
    }

    bool IsValid()
    {
        return age >= 14 && appointmentTime >= 8 && appointmentTime <= 16;
    }
};

int main()
{
    CLI<Stack<Candidat>, Candidat> cli;
    cli.Loop();

    return 0;
}

```
