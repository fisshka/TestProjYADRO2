#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <algorithm>
#include <map>
#include <regex>

using namespace std;

int toInt(string s) {//перевод типа string в int(используется для вычисления времени)
    int rez = 0;
    for(int i = 0; i < s.size(); i++) {
        rez = rez * 10 + (s[i] - '0');
    }
    return rez;
}

class Client {
public:
    Client(string name, string Time) : clientName(name), clientTime(Time), clientInClub(false), clientAtTable(-1) {}
    //в конструкторе задается время, когда клиент пришел, имя, а также устанавливаются все параметры
    //(по умолчанию стол, который занимает клиент -1)
    int GetTable() const {
        return clientAtTable;
    }//геттер для стола
    bool getInClub()
    {
        return clientInClub;
    }

    bool Came() {
        //вызывается, когда клиент приходит - сразу происходит проверка на ошибки
        //если клиент уже в клубе - вызывается ошибка
        if (clientInClub) {
            return false;
        }
        clientInClub = true;
        return true;
    }

    bool SitAtTable(int table, const vector<Client>& clients) {
        //вызывается, когда клиент приходит - сразу происходит проверка на ошибки
        //в случае, если кто-то уже занимает введенный стол - вызывается ошибка
        for (const auto& client : clients) {
            if (client.GetTable() == table) {
                return false;
            }
        }
        clientAtTable = table;
        return true;
    }
    int GetTableTime(string leavingTime) const {
        int hours = ((leavingTime[0] - '0') * 10 + (leavingTime[1] - '0')) -
                    ((sitTime[0] - '0') * 10 + (sitTime[1] - '0'));
        int minutes = ((leavingTime[3] - '0') * 10 + (leavingTime[4] - '0')) -
                      ((sitTime[3] - '0') * 10 + (sitTime[4] - '0'));
        return hours * 60 + minutes;
    }
    void SetTime(string time) {
        clientTime = time;
    }




    void GoOut() {
        //в случае ухода параметры возвращаются в базовое значение
        clientAtTable = -1;
        clientInClub = false;
    }

    string GetClientName() const {
        //геттер для имени
        return clientName;
    }
    string GetClientTime() const {
        //геттер для имени
        return clientTime;
    }
void SetSitTime(string time) {
        sitTime = time;
    }





private:
    bool clientInClub;
    int clientAtTable;
    string clientName;
    string clientTime;
    string sitTime;
};
struct TableTime {
    //структура для хранения времени, проведенного за столом
    int tableId;
    int totalTime;
    int totalIncome;

    TableTime() : tableId(-1), totalTime(0), totalIncome(0) {}
    TableTime(int id) : tableId(id), totalTime(0), totalIncome(0) {}
};
//функция дла подсчета выручки
int calculateIncome(int totalTime, int price) {
    return ((totalTime + 59) / 60) * price;
}
string formatTime(int totalMinutes) {
    int hours = totalMinutes / 60;
    int minutes = totalMinutes % 60;
    char buffer[6];
    snprintf(buffer, sizeof(buffer), "%02d:%02d", hours, minutes);
    return string(buffer);
}//функция для перевода времени в нужный формат




bool compareByName(const Client& a, const Client& b) {
    return a.GetClientName() < b.GetClientName();
}//компаратор для сортировки клиентов по имени в алфавитном порядке

int main(int argc, char * argv[]) {
    string timeOpening, timeClosing;
    map<int, TableTime> tableTimes;//словарь для хранения выручки со стола и общего времени, проведенного за столом
    int n, komp;//переменная, в которой хранится количество свободных столов в клубе
    //и переменная, в которой хранится общее количество столов
    ifstream in(argv[1]);
    if(!in.is_open())
        cout<<"Error!"<<endl;//инициализация потока именем файла, введенным с консоли
        //ввод основных данных
    in >> n >> timeOpening >> timeClosing;
    int price;
    in >> price;
    komp = n;
    string tmp;
    int eventId, income = 0, table, number;
    string clientName, eventTime;
    char event;//переменная для хранения id события
    queue<int> Queue;//очередь для хранения очереди клиентов
    vector<Client> clients;//вектор для хранения клиентов

    cout<< timeOpening << endl;
    std::regex pattern1("^[a-zA-Z0-9]+$");//использование регулярных выражений для проверки на нужный формат
    std::regex pattern2("^([01][0-9]|2[0-3]):[0-5][0-9]$");//

    while (in >> eventTime>> event >> clientName) {

        number = -1;
        eventId = event - '0';//перевод event в int тип
        //проверка на то, имеют ли введенные данные подходящий формат
        if ((!std::regex_match(clientName,pattern1) ||
             !std::regex_match(eventTime, pattern2) ) && eventId == 2) {
                string tmp;
                in>>tmp;

        cout << eventTime << " " << event << " " << clientName << " " << tmp << endl;
        return 0;
        }

        else if(!std::regex_match(clientName,pattern1) ||
             !std::regex_match(eventTime, pattern2))
        {
            cout << eventTime << " " << event << " " << clientName << endl;
        return 0;
        }



        //поиск номера клиента в векторе либо его добавление в вектор, если он там не найден
        //и значение number осталось равно -1
        for (int i = 0; i < clients.size(); i++) {
            if (clients[i].GetClientName() == clientName) {
                number = i;
                break;
            }
        }
        if (number == -1) {
            clients.emplace_back(clientName, eventTime);
            number = clients.size() - 1;
        }

        //обработка события 1
        if (eventId == 1) {
            cout << eventTime << " " << 1 << " " << clientName << endl;
            if (toInt(eventTime) < toInt(timeOpening)) {
                //обработка ошибки NotOpenYet, сравнение времени прихода клиента с временем открытия
                cout << eventTime << " " << 13 << " " << "NotOpenYet" << endl;
                clients.pop_back();
                continue;
            }
            if (clients[number].getInClub()) {
                //обработка ошибки YouShallNotPass, проверка на то, находится ли клиент в клубе
                cout << eventTime << " " << 13 << " " << "YouShallNotPass" << endl;
                clients.pop_back();
                continue;
            }
            clients[number].Came();
        } else if (eventId == 2) {
            //обработка события 1
            string tableString;
            in >> tableString;
            table = toInt(tableString);
            //ввод переменной table
            cout << eventTime << " " << 2 << " " << clientName << " " << table << endl;
            if (!clients[number].getInClub()) {
                //обработка ошибки ClientUnknown, проверка на то, находится ли клиент в клубе
                cout << eventTime << " " << 13 << " " << "ClientUnknown" << endl;
                continue;
            }


            if (clients[number].SitAtTable(table, clients)) {

                //установление времени сидения за столом
                clients[number].SetSitTime(eventTime);
                if (tableTimes.find(table) == tableTimes.end()) {
                    tableTimes[table] = TableTime(table);

            }
            }
            else
            {
                //обработка ошибки PlaceIsBusy, проверка на то,занято ли место
                cout << eventTime << " " << 13 << " " << "PlaceIsBusy" << endl;
            }
                n--;


        }

        //обработка события 3
        else if (eventId == 3) {
            cout << eventTime << " " << 3 << " " << clientName << endl;
            if (komp <= Queue.size()) {
                //обработка события 11 - если общее кол-во столов меньше, чем очередь, то клиент уходит
                clients[number].GoOut();
            cout << eventTime << " " << 11 << " " << clientName << endl;
            } else if (n > 0) {
                //обработка ошибки ICanWaitNoLonger!, проверка на то,свободны ли места
                cout << eventTime << " " << 13 << " " << "ICanWaitNoLonger!" << endl;
                continue;
            } else {
                //добавление номера клиента в очередь
                Queue.push(number);
            }
        }
        //обработка события 4
        else if (eventId == 4) {
            //сохранение номера свободного стола
            int freeTable = clients[number].GetTable();
            cout << eventTime << " " << 4 << " " << clientName << endl;
            //вычисление прибыли и времени, проведенного за столом
            if (clients[number].GetTable() != -1) {
                int tableTime = clients[number].GetTableTime(eventTime);
                tableTimes[clients[number].GetTable()].totalTime += tableTime;
                tableTimes[clients[number].GetTable()].totalIncome += calculateIncome(tableTime, price);
            }
            clients[number].GoOut();
            //обработка события 12, если очередь не пуста
            if (!Queue.empty()) {
                clients[Queue.front()].SitAtTable(freeTable, clients);
                cout << eventTime << " " << 12 << " " << clients[Queue.front()].GetClientName() << " " << freeTable << endl;
                clients[Queue.front()].SetSitTime(eventTime);
                Queue.pop();
                n--;
            }
            n++;
        }





    }

    //сортировка клиентов по алфавиту и запуск события 11 для тех, кто остался в клубе, и подсчет выручки для них
    sort(clients.begin(), clients.end(), compareByName);
    for (int i = 0; i < clients.size(); i++) {
        if (clients[i].getInClub()) {
            if (clients[i].GetTable() != -1) {
                int tableTime = clients[i].GetTableTime(timeClosing);
                tableTimes[clients[i].GetTable()].totalTime += tableTime;
                tableTimes[clients[i].GetTable()].totalIncome += calculateIncome(tableTime, price);
            }
            clients[i].GoOut();
            cout << timeClosing << " " << 11 << " " << clients[i].GetClientName() << endl;
        }
    }

    cout << timeClosing << endl;



    //вывод выручки и времени, проведенного за столом
    for ( auto tableTime : tableTimes) {
        cout << tableTime.first << " " << tableTime.second.totalIncome << " " << formatTime(tableTime.second.totalTime) << endl;
    }




    //закрытие потока для ввода
    in.close();



    return 0;
}
