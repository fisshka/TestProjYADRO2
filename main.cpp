#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <algorithm>
#include <map>
#include <regex>

using namespace std;

int toInt(string s) {//������� ���� string � int(������������ ��� ���������� �������)
    int rez = 0;
    for(int i = 0; i < s.size(); i++) {
        rez = rez * 10 + (s[i] - '0');
    }
    return rez;
}

class Client {
public:
    Client(string name, string Time) : clientName(name), clientTime(Time), clientInClub(false), clientAtTable(-1) {}
    //� ������������ �������� �����, ����� ������ ������, ���, � ����� ��������������� ��� ���������
    //(�� ��������� ����, ������� �������� ������ -1)
    int GetTable() const {
        return clientAtTable;
    }//������ ��� �����
    bool getInClub()
    {
        return clientInClub;
    }

    bool Came() {
        //����������, ����� ������ �������� - ����� ���������� �������� �� ������
        //���� ������ ��� � ����� - ���������� ������
        if (clientInClub) {
            return false;
        }
        clientInClub = true;
        return true;
    }

    bool SitAtTable(int table, const vector<Client>& clients) {
        //����������, ����� ������ �������� - ����� ���������� �������� �� ������
        //� ������, ���� ���-�� ��� �������� ��������� ���� - ���������� ������
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
        //� ������ ����� ��������� ������������ � ������� ��������
        clientAtTable = -1;
        clientInClub = false;
    }

    string GetClientName() const {
        //������ ��� �����
        return clientName;
    }
    string GetClientTime() const {
        //������ ��� �����
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
    //��������� ��� �������� �������, ������������ �� ������
    int tableId;
    int totalTime;
    int totalIncome;

    TableTime() : tableId(-1), totalTime(0), totalIncome(0) {}
    TableTime(int id) : tableId(id), totalTime(0), totalIncome(0) {}
};
//������� ��� �������� �������
int calculateIncome(int totalTime, int price) {
    return ((totalTime + 59) / 60) * price;
}
string formatTime(int totalMinutes) {
    int hours = totalMinutes / 60;
    int minutes = totalMinutes % 60;
    char buffer[6];
    snprintf(buffer, sizeof(buffer), "%02d:%02d", hours, minutes);
    return string(buffer);
}//������� ��� �������� ������� � ������ ������




bool compareByName(const Client& a, const Client& b) {
    return a.GetClientName() < b.GetClientName();
}//���������� ��� ���������� �������� �� ����� � ���������� �������

int main(int argc, char * argv[]) {
    string timeOpening, timeClosing;
    map<int, TableTime> tableTimes;//������� ��� �������� ������� �� ����� � ������ �������, ������������ �� ������
    int n, komp;//����������, � ������� �������� ���������� ��������� ������ � �����
    //� ����������, � ������� �������� ����� ���������� ������
    ifstream in(argv[1]);
    if(!in.is_open())
        cout<<"Error!"<<endl;//������������� ������ ������ �����, ��������� � �������
        //���� �������� ������
    in >> n >> timeOpening >> timeClosing;
    int price;
    in >> price;
    komp = n;
    string tmp;
    int eventId, income = 0, table, number;
    string clientName, eventTime;
    char event;//���������� ��� �������� id �������
    queue<int> Queue;//������� ��� �������� ������� ��������
    vector<Client> clients;//������ ��� �������� ��������

    cout<< timeOpening << endl;
    std::regex pattern1("^[a-zA-Z0-9]+$");//������������� ���������� ��������� ��� �������� �� ������ ������
    std::regex pattern2("^([01][0-9]|2[0-3]):[0-5][0-9]$");//

    while (in >> eventTime>> event >> clientName) {

        number = -1;
        eventId = event - '0';//������� event � int ���
        //�������� �� ��, ����� �� ��������� ������ ���������� ������
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



        //����� ������ ������� � ������� ���� ��� ���������� � ������, ���� �� ��� �� ������
        //� �������� number �������� ����� -1
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

        //��������� ������� 1
        if (eventId == 1) {
            cout << eventTime << " " << 1 << " " << clientName << endl;
            if (toInt(eventTime) < toInt(timeOpening)) {
                //��������� ������ NotOpenYet, ��������� ������� ������� ������� � �������� ��������
                cout << eventTime << " " << 13 << " " << "NotOpenYet" << endl;
                clients.pop_back();
                continue;
            }
            if (clients[number].getInClub()) {
                //��������� ������ YouShallNotPass, �������� �� ��, ��������� �� ������ � �����
                cout << eventTime << " " << 13 << " " << "YouShallNotPass" << endl;
                clients.pop_back();
                continue;
            }
            clients[number].Came();
        } else if (eventId == 2) {
            //��������� ������� 1
            string tableString;
            in >> tableString;
            table = toInt(tableString);
            //���� ���������� table
            cout << eventTime << " " << 2 << " " << clientName << " " << table << endl;
            if (!clients[number].getInClub()) {
                //��������� ������ ClientUnknown, �������� �� ��, ��������� �� ������ � �����
                cout << eventTime << " " << 13 << " " << "ClientUnknown" << endl;
                continue;
            }


            if (clients[number].SitAtTable(table, clients)) {

                //������������ ������� ������� �� ������
                clients[number].SetSitTime(eventTime);
                if (tableTimes.find(table) == tableTimes.end()) {
                    tableTimes[table] = TableTime(table);

            }
            }
            else
            {
                //��������� ������ PlaceIsBusy, �������� �� ��,������ �� �����
                cout << eventTime << " " << 13 << " " << "PlaceIsBusy" << endl;
            }
                n--;


        }

        //��������� ������� 3
        else if (eventId == 3) {
            cout << eventTime << " " << 3 << " " << clientName << endl;
            if (komp <= Queue.size()) {
                //��������� ������� 11 - ���� ����� ���-�� ������ ������, ��� �������, �� ������ ������
                clients[number].GoOut();
            cout << eventTime << " " << 11 << " " << clientName << endl;
            } else if (n > 0) {
                //��������� ������ ICanWaitNoLonger!, �������� �� ��,�������� �� �����
                cout << eventTime << " " << 13 << " " << "ICanWaitNoLonger!" << endl;
                continue;
            } else {
                //���������� ������ ������� � �������
                Queue.push(number);
            }
        }
        //��������� ������� 4
        else if (eventId == 4) {
            //���������� ������ ���������� �����
            int freeTable = clients[number].GetTable();
            cout << eventTime << " " << 4 << " " << clientName << endl;
            //���������� ������� � �������, ������������ �� ������
            if (clients[number].GetTable() != -1) {
                int tableTime = clients[number].GetTableTime(eventTime);
                tableTimes[clients[number].GetTable()].totalTime += tableTime;
                tableTimes[clients[number].GetTable()].totalIncome += calculateIncome(tableTime, price);
            }
            clients[number].GoOut();
            //��������� ������� 12, ���� ������� �� �����
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

    //���������� �������� �� �������� � ������ ������� 11 ��� ���, ��� ������� � �����, � ������� ������� ��� ���
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



    //����� ������� � �������, ������������ �� ������
    for ( auto tableTime : tableTimes) {
        cout << tableTime.first << " " << tableTime.second.totalIncome << " " << formatTime(tableTime.second.totalTime) << endl;
    }




    //�������� ������ ��� �����
    in.close();



    return 0;
}
