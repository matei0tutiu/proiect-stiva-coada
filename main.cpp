#include <iostream>
#include <cstring>

//* CONCEPTS
template <typename T>
concept InputBuildable = requires {
    { T::BuildFromInput() } -> std::same_as<T>;
};

template <typename T>
concept Validatable = requires(T t) {
    { t.IsValid() } -> std::same_as<bool>;
};

template <typename T>
concept Streamable = requires(std::ostream &os, const T &t) {
    { os << t } -> std::same_as<std::ostream &>;
};

template <typename T>
concept Assignable = requires(T &a, T &b) {
    { a = b } -> std::same_as<T &>;
};

template <typename T>
concept UsableAsCLIData = InputBuildable<T> && Validatable<T> && Streamable<T> && Assignable<T>;

template <typename T>
concept Poppable = requires(T t) {
    { t.Pop() } -> std::same_as<void>;
};

template <typename T>
concept Clearable = requires(T t) {
    { t.Clear() } -> std::same_as<void>;
};

template <typename T>
concept EmptyVerifiable = requires(T t) {
    { t.IsEmpty() } -> std::same_as<bool>;
};

template <typename T, typename Arg>
concept Pushable = requires(T t, const Arg &arg) {
    { t.Push(arg) } -> std::same_as<void>;
};

template <typename T, typename Arg>
concept LastMemberAccesible = requires(T t) {
    { t.LastElement() } -> std::same_as<Arg &>;
};

template <typename T, typename Arg>
concept UsableWithCLI = Pushable<T, Arg> && Poppable<T> && Clearable<T> && EmptyVerifiable<T> && Streamable<T> && LastMemberAccesible<T, Arg>;

//* DATA STRUCTURES IMPLEMENTATION

template <typename T>
class Stack
{
public:
    Stack() : m_top(nullptr) {}
    ~Stack()
    {
        Clear();
    }

    void Push(const T &value)
    {
        SNode *newNode = new SNode();
        newNode->value = value;
        newNode->next = m_top;

        m_top = newNode;
    }

    void Pop()
    {
        if (m_top == nullptr)
            return;

        SNode *secondNode = m_top->next;
        delete m_top;
        m_top = secondNode;
    }

    void Clear()
    {
        while (m_top != nullptr)
            Pop();
    }

    T &LastElement()
    {
        if (m_top == nullptr)
            throw std::runtime_error("Cannot pop an empty stack");
        return (*m_top).value;
    }

    inline bool IsEmpty() { return m_top == nullptr; }
    size_t Size()
    {
        size_t res = 0;
        SNode *current = m_top;

        while (current != nullptr)
        {
            res++;
            current = current->next;
        }

        return res;
    }

    friend std::ostream &operator<<(std::ostream &os, const Stack &stack)
    {
        if (stack.m_top == nullptr)
            return os;

        Stack<T>::SNode *current = stack.m_top;
        os << "varf" << "\n  |\n  v\n";
        while (current != nullptr)
        {
            os << '(' << current->value << ')';
            if (current->next != nullptr)
                os << "\n  |\n  v\n";
            else
                os << '\n';
            current = current->next;
        }

        return os;
    }

private:
    struct SNode
    {
        SNode *next;
        T value;
    };

    SNode *m_top;
};

template <typename T>
class Queue
{
public:
    Queue() : m_bottom(nullptr), m_top(nullptr) {}
    ~Queue()
    {
        Clear();
    }

    void Push(const T &value)
    {
        // create the new node
        QNode *newNode = new QNode();
        newNode->value = value;
        newNode->next = nullptr;

        // add it at the bottom
        if (m_top != nullptr)
            m_top->next = newNode;
        if (m_bottom == nullptr)
            m_bottom = newNode;
        m_top = newNode;
    }

    void Pop()
    {
        if (m_bottom == nullptr)
            return;

        if (m_bottom == m_top)
            m_top = nullptr;

        QNode *secondNode = m_bottom->next;
        delete m_bottom;
        m_bottom = secondNode;
    }

    void Clear()
    {
        while (m_bottom != nullptr)
            Pop();
    }

    T &LastElement()
    {
        if (m_top == nullptr)
            throw std::runtime_error("Cannot read from an empty queue");
        return (*m_bottom).value;
    }

    inline bool IsEmpty() { return m_bottom == nullptr && m_top == nullptr; }

    size_t Size()
    {
        size_t res = 0;
        QNode *current = m_bottom;

        while (current != nullptr)
        {
            res++;
            current = current->next;
        }
        return res;
    }

    friend std::ostream &operator<<(std::ostream &os, const Queue &queue)
    {
        if (queue.m_bottom == nullptr)
            return os;

        Queue<T>::QNode *current = queue.m_bottom;
        os << "coada" << "\n  |\n  v\n";
        while (current != nullptr)
        {
            os << '(' << current->value << ')';
            if (current->next != nullptr)
                os << "\n  |\n  v\n";
            else
                os << "\n  ^\n  |\n";

            current = current->next;
        }
        os << "varf\n";
        return os;
    }

private:
    struct QNode
    {
        QNode *next;
        T value;
    };

    QNode *m_bottom, *m_top;
};

template <typename T, UsableAsCLIData Arg>
    requires UsableWithCLI<T, Arg>
class CLI
{
public:
    void Loop()
    {
        T data;
        unsigned short option = 0;

        do
        {
            if (system("CLS"))
                system("clear");

            PrintMenu();

            std::cin >> option;

            if (std::cin.fail())
            {
                std::cin.clear();               // clear the error flag
                std::cin.ignore(INT_MAX, '\n'); // discard input
                PrintAndWaitForEnter("Valorile introduse sunt invalide. Te rog sa alegi un numar.\nApasa ENTER pentru a reveni la meniu ");
            }
            else
            {
                std::cin.ignore(1, '\n');
                handleOption(data, option);
            }
        } while (option != 5);
    }

private:
    static constexpr char MENU_COLOR_SEQUENCE[] = "\033[34m";
    static constexpr char RESET_COLOR_SEQUENCE[] = "\033[0m";

    static const unsigned short CLEAR = 1;
    static const unsigned short PUSH = 2;
    static const unsigned short POP = 3;
    static const unsigned short PRINT = 4;
    static const unsigned short EXIT = 5;

    void handleOption(T &dataS, const unsigned short &option)
    {
        switch (option)
        {
        case CLEAR:
            dataS.Clear();
            PrintAndWaitForEnter("Structura a fost golita cu success. Apasa ENTER pentru a reveni la meniu ");
            break;
        case PUSH:
        {
            Arg newObject = Arg::BuildFromInput();
            std::cin.ignore(INT_MAX, '\n');
            if (!newObject.IsValid())
                PrintAndWaitForEnter("Validarea datelor a esuat. Apasa ENTER pentru a reveni la meniu ");
            else
            {
                dataS.Push(newObject);
                PrintAndWaitForEnter("Valorile au fost introduse. Apasa ENTER pentru a reveni la meniu ");
            }
            break;
        }

        case POP:
            if (dataS.IsEmpty())
                PrintAndWaitForEnter("Structura este goala. Apasa ENTER pentru a reveni la meniu ");
            else
            {
                Arg data = dataS.LastElement();
                dataS.Pop();
                std::cout << data;
                PrintAndWaitForEnter(" este valoarea extrasa. Apasa ENTER pentru a reveni la meniu ");
            }
            break;
        case PRINT:
            if (dataS.IsEmpty())
                PrintAndWaitForEnter("Structura este goala. Apasa ENTER pentru a reveni la meniu ");
            else
            {
                std::cout << dataS;
                PrintAndWaitForEnter("\n Apasa ENTER pentru a reveni la meniu ");
            }
            break;
        case EXIT:
            break;
        default:
            PrintAndWaitForEnter("Optiunea aleasa nu exista. Apasa ENTER pentru a reveni la meniu ");
            break;
        };
    }

    void PrintAndWaitForEnter(const char *message)
    {
        std::cout << MENU_COLOR_SEQUENCE << message << RESET_COLOR_SEQUENCE;
        std::cin.get();
    }

    void PrintMenu()
    {
        std::cout << MENU_COLOR_SEQUENCE;
        std::cout << "----MENIU----\n";
        std::cout << "1. Goleste\n";
        std::cout << "2. Adauga (push)\n";
        std::cout << "3. Extrage (pop)\n";
        std::cout << "4. Afiseaza\n";
        std::cout << "5. Exit\n";
        std::cout << "Scrie numarul corespunzator optiunii dorite:\n> ";
        std::cout << RESET_COLOR_SEQUENCE;
    }
};

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
