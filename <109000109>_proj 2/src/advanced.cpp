#include "./nthu_bike.h"
#include <fstream>
#include <cstdlib>
#include <string>
#include <iostream>
#include <math.h>
#include <cmath>
#include <algorithm>
#include <cstring>
using namespace std;

#define V 105

void floydWarshal_ad(int **cost)
{
    for (int k = 0; k < V; k++)
    {
        for (int i = 0; i < V; i++)
            for (int j = 0; j < V; j++)
                if (cost[i][k] + cost[k][j] < cost[i][j])
                    cost[i][j] = cost[i][k] + cost[k][j];
    }
}

class Transfer_Log_Node
{
private:
public:
    Transfer_Log_Node *next;
    Transfer_Log_Node *previous;
    int Bike_Id;
    int Start_Station;
    int End_Station;
    int Transfer_Start_Time;
    int Transfer_End_Time;
    int Bike_Rider;
    Transfer_Log_Node(int Bike_Id,
                      int Start_Station,
                      int End_Station,
                      int Transfer_Start_Time,
                      int Transfer_End_Time,
                      int Bike_Rider)
    {
        this->Bike_Id = Bike_Id;
        this->Start_Station = Start_Station;
        this->End_Station = End_Station;
        this->Transfer_Start_Time = Transfer_Start_Time;
        this->Transfer_End_Time = Transfer_End_Time;
        this->Bike_Rider = Bike_Rider;
        this->next = NULL;
        this->previous = NULL;
    }
};
class Transfer_Log_Node_List
{
private:
public:
    int size_ = 0;
    Transfer_Log_Node *head_;
    Transfer_Log_Node *tail_;
    Transfer_Log_Node *itr_;
    Transfer_Log_Node_List()
    {
        this->size_ = 0;
        this->head_ = NULL;
        this->tail_ = NULL;
        this->itr_ = NULL;
    };
    void insert_in_front_of_cur(Transfer_Log_Node *current, Transfer_Log_Node *Node)
    {
        if (current == this->head_)
        {
            Node->previous = NULL;
            Node->next = this->head_;
            this->head_->previous = Node;
            this->head_ = Node;
        }
        else
        {
            Node->previous = current->previous;
            current->previous->next = Node;
            Node->next = current;
            current->previous = Node;
        }
    }
    void insert_behind_cur(Transfer_Log_Node *current, Transfer_Log_Node *Node)
    {
        if (current == this->tail_)
        {
            Node->next = NULL;
            current->next = Node;
            Node->previous = current;
            this->tail_ = Node;
        }
        else
        {
            Node->next = current->next;
            Node->previous = current;
            current->next->previous = Node;
            current->next = Node;
        }
    }
    void insert_by_id(Transfer_Log_Node *Node)
    {
        Transfer_Log_Node *current = this->head_;
        if (current == NULL)
        {
            this->head_ = Node;
            this->tail_ = Node;
        }
        else
        {
            while (current != NULL)
            {
                if (Node->Bike_Rider < current->Bike_Rider)
                {
                    insert_in_front_of_cur(current, Node);
                    break;
                }
                else if (current == this->tail_)
                {
                    insert_behind_cur(current, Node);
                    break;
                }
                current = current->next;
            }
        }
        this->size_ += 1;
    }
    void insert_by_trans_time(Transfer_Log_Node *Node)
    {
        Transfer_Log_Node *current = this->head_;
        if (current == NULL)
        {
            this->head_ = Node;
            this->tail_ = Node;
        }
        else
        {
            while (current != NULL)
            {
                if (Node->Transfer_Start_Time < current->Transfer_Start_Time)
                {
                    insert_in_front_of_cur(current, Node);
                    break;
                }
                else if (current == this->tail_)
                {
                    insert_behind_cur(current, Node);
                    break;
                }
                else
                {
                    current = current->next;
                }
            }
        }
        this->size_ += 1;
    }
};

class Bike_Node
{
private:
public:
    Bike_Node *next;
    Bike_Node *previous;
    int ID;
    string Type;
    int Station;
    float Rental_price;
    int Rental_count;
    int TimeIndex;
    Bike_Node(int ID,
              string Type,
              int Station,
              float Rental_price,
              int Rental_count)
    {
        this->ID = ID;
        this->Type = Type;
        this->Station = Station;
        this->Rental_price = Rental_price;
        this->Rental_count = Rental_count;
        TimeIndex = 0;
        this->previous = NULL;
    }
};
class Bike_Node_List
{
private:
public:
    int size_ = 0;
    Bike_Node *head_;
    Bike_Node *tail_;
    Bike_Node *itr_;
    Bike_Node_List()
    {
        this->size_ = 0;
        this->head_ = NULL;
        this->tail_ = NULL;
        this->itr_ = NULL;
    };
    void insert_in_front_of_cur(Bike_Node *current, Bike_Node *Node)
    {
        if (current == this->head_)
        {
            Node->previous = NULL;
            Node->next = this->head_;
            this->head_->previous = Node;
            this->head_ = Node;
        }
        else
        {
            Node->previous = current->previous;
            current->previous->next = Node;
            Node->next = current;
            current->previous = Node;
        }
    }
    void insert_behind_cur(Bike_Node *current, Bike_Node *Node)
    {
        if (current == this->tail_)
        {
            Node->next = NULL;
            current->next = Node;
            Node->previous = current;
            this->tail_ = Node;
        }
        else
        {
            Node->next = current->next;
            Node->previous = current;
            current->next->previous = Node;
            current->next = Node;
        }
    }
    void insert_by_price(Bike_Node *Node)
    {
        Bike_Node *current = this->head_;
        if (current == NULL)
        {
            this->head_ = Node;
            this->tail_ = Node;
            Node->next = NULL;
            Node->previous = NULL;
        }
        else
        {
            while (current != NULL)
            {
                if (Node->Rental_price > current->Rental_price)
                {
                    insert_in_front_of_cur(current, Node);
                    break;
                }
                else if (Node->Rental_price == current->Rental_price)
                {
                    if (Node->ID < current->ID)
                    {
                        insert_in_front_of_cur(current, Node);
                        break;
                    }
                    else if (current == this->tail_)
                    {
                        insert_behind_cur(current, Node);
                        break;
                    }
                    else if (current->next->Rental_price != current->Rental_price)
                    {
                        insert_behind_cur(current, Node);
                        break;
                    }
                }
                else if (current == this->tail_)
                {
                    insert_behind_cur(current, Node);
                    break;
                }
                current = current->next;
            }
            // while (current != NULL)
            // {
            //     if (Node->Rental_price == current->Rental_price)
            //     {
            //         if (Node->ID < current->ID)
            //         {
            //             insert_in_front_of_cur(current, Node);
            //             break;
            //         }
            //         else
            //         {
            //             if (current == this->tail_)
            //             {
            //                 insert_behind_cur(current, Node);
            //                 break;
            //             }
            //             else if (current->next->Rental_price != current->Rental_price)
            //             {
            //                 insert_behind_cur(current, Node);
            //                 break;
            //             }
            //             else{
            //                 current = current->next;
            //             }
            //         }
            //     }
            //     else if (Node->Rental_price > current->Rental_price)
            //     {
            //         insert_in_front_of_cur(current, Node);
            //         break;
            //     }
            //     else if (current == this->tail_)
            //     {
            //         insert_behind_cur(current, Node);
            //         break;
            //     }
            //     else
            //     {
            //         current = current->next;
            //     }
            // }
        }
        this->size_ += 1;
    }
    void check()
    {
        Bike_Node *current = this->head_;
        if (current != this->tail_)
        {
            while (current != NULL)
            {
                if (current->next != NULL)
                {
                    if (current->Rental_price < current->next->Rental_price)
                    {
                        cout << "Order wrong" << endl;
                        Bike_Node *tmp = current->next;
                        this->Delete(current->next);
                        this->insert_in_front_of_cur(current, tmp);
                    }
                    else if (current->Rental_price == current->next->Rental_price)
                    {
                        if (current->ID > current->next->ID)
                        {
                            cout << "Order wrong" << endl;
                            Bike_Node *tmp = current->next;
                            this->Delete(current->next);
                            this->insert_in_front_of_cur(current, tmp);
                        }
                    }
                }
                current = current->next;
            }
        }
    }
    void insert_by_id(Bike_Node *Node)
    {
        Bike_Node *current = this->head_;
        if (current == NULL)
        {
            this->head_ = Node;
            this->tail_ = Node;
            Node->next = NULL;
            Node->previous = NULL;
        }
        else
        {
            while (current != NULL)
            {
                if (Node->ID < current->ID)
                {
                    insert_in_front_of_cur(current, Node);
                    break;
                }
                else if (current == this->tail_)
                {
                    insert_behind_cur(current, Node);
                    break;
                }
                current = current->next;
            }
        }
        this->size_ += 1;
    }
    void Delete(Bike_Node *Node)
    {
        if (Node->previous == NULL && Node->next == NULL)
        {
            this->head_ = NULL;
            this->tail_ = NULL;
        }
        else if (Node->previous == NULL)
        {
            this->head_ = Node->next;
            Node->next->previous = NULL;
            Node->next = NULL;
        }
        else if (Node->next == NULL)
        {
            this->tail_ = Node->previous;
            Node->previous->next = NULL;
            Node->previous = NULL;
        }
        else
        {
            Node->previous->next = Node->next;
            Node->next->previous = Node->previous;
            Node->previous = NULL;
            Node->next = NULL;
        }
    }
};

class User_Node
{
private:
public:
    User_Node *next;
    User_Node *previous;
    int ID;
    string *Accept_Bike_Types;
    int Start_Time;
    int End_Time;
    int Start_Point;
    int End_Point;
    bool AcceptOrNot;
    int Bike_Id;
    int Bike_Start_Time;
    int Bike_End_Time;
    int Revenue;
    User_Node(int ID,
              string *Accept_Bike_Types,
              int Start_Time,
              int End_Time,
              int Start_Point,
              int End_Point)
    {
        this->ID = ID;
        this->Accept_Bike_Types = Accept_Bike_Types;
        this->Start_Time = Start_Time;
        this->End_Time = End_Time;
        this->Start_Point = Start_Point;
        this->End_Point = End_Point;
        this->AcceptOrNot = 0;
        this->Bike_Id = 0;
        this->Bike_Start_Time = 0;
        this->Bike_End_Time = 0;
        this->Revenue = 0;
        this->next = NULL;
        this->previous = NULL;
    }
    User_Node(int ID, bool AcceptOrNot, int Bike_Id, int Bike_Start_Time, int Bike_End_Time, int Revenue)
    {
        this->ID = ID;
        this->AcceptOrNot = AcceptOrNot;
        this->Bike_Id = Bike_Id;
        this->Bike_Start_Time = Bike_Start_Time;
        this->Start_Time = Bike_Start_Time;
        this->Bike_End_Time = Bike_End_Time;
        this->Revenue = Revenue;
        this->next = NULL;
        this->previous = NULL;
    }
};
class User_Node_List
{
private:
public:
    int size_ = 0;
    User_Node *head_;
    User_Node *tail_;
    User_Node *itr_;
    User_Node_List()
    {
        this->size_ = 0;
        this->head_ = NULL;
        this->tail_ = NULL;
        this->itr_ = NULL;
    };
    void insert_in_front_of_cur(User_Node *current, User_Node *Node)
    {
        if (current == this->head_)
        {
            Node->previous = NULL;
            Node->next = this->head_;
            this->head_->previous = Node;
            this->head_ = Node;
        }
        else
        {
            Node->previous = current->previous;
            current->previous->next = Node;
            Node->next = current;
            current->previous = Node;
        }
    }
    void insert_behind_cur(User_Node *current, User_Node *Node)
    {
        if (current == this->tail_)
        {
            Node->next = NULL;
            current->next = Node;
            Node->previous = current;
            this->tail_ = Node;
        }
        else
        {
            Node->next = current->next;
            Node->previous = current;
            current->next->previous = Node;
            current->next = Node;
        }
    }
    void insert_by_start_time(User_Node *Node)
    {
        User_Node *current = this->head_;
        if (current == NULL)
        {
            this->head_ = Node;
            this->tail_ = Node;
        }
        else
        {
            while (current != NULL)
            {
                if (Node->Start_Time == current->Start_Time)
                {
                    if (Node->ID < current->ID)
                    {
                        insert_in_front_of_cur(current, Node);
                        break;
                    }
                    else
                    {
                        if (current == this->tail_)
                        {
                            insert_behind_cur(current, Node);
                            break;
                        }
                        else if (current->next->Start_Time != Node->Start_Time)
                        {
                            insert_behind_cur(current, Node);
                            break;
                        }
                        current = current->next;
                    }
                }
                else if (Node->Start_Time < current->Start_Time)
                {
                    insert_in_front_of_cur(current, Node);
                    break;
                }
                else
                {
                    if (current == this->tail_)
                    {
                        insert_behind_cur(current, Node);
                        break;
                    }
                    current = current->next;
                }
            }
        }
        this->size_ += 1;
    }
    void insert_by_id(User_Node *Node)
    {
        User_Node *current = this->head_;
        if (current == NULL)
        {
            this->head_ = Node;
            this->tail_ = Node;
        }
        else
        {
            while (current != NULL)
            {
                if (Node->ID < current->ID)
                {
                    insert_in_front_of_cur(current, Node);
                    break;
                }
                else if (current == this->tail_)
                {
                    insert_behind_cur(current, Node);
                    break;
                }
                current = current->next;
            }
        }
        this->size_ += 1;
    }
    void Delete(User_Node *Node)
    {
        if (Node->previous == NULL && Node->next == NULL)
        {
            this->head_ = NULL;
            this->tail_ = NULL;
        }
        else if (Node->previous == NULL)
        {
            this->head_ = Node->next;
            Node->next->previous = NULL;
        }
        else if (Node->next == NULL)
        {
            this->tail_ = Node->previous;
            Node->previous->next = NULL;
        }
        else
        {
            Node->previous->next = Node->next;
            Node->next->previous = Node->previous;
        }
    }
};

// {
// private:
// public:
//     int ID;
//     string Type;
//     int Station;
//     float Rental_price;
//     int Rental_count;
//     int TimeIndex;
//     Bike(int ID,
//          string Type,
//          int Station,
//          float Rental_price,
//          int Rental_count)
//     {
//         this->ID = ID;
//         this->Type = Type;
//         this->Station = Station;
//         this->Rental_price = Rental_price;
//         this->Rental_count = Rental_count;
//         TimeIndex = 0;
//     }
//     ~Bike()
//     {
//     }
// };

class Graph
{
private:
    int **adjMatrix;
    int numVertices;

public:
    // Initialize the matrix to zero
    Graph(int numVertices)
    {
        this->numVertices = numVertices;
        adjMatrix = new int *[numVertices];
        for (int i = 0; i < numVertices; i++)
        {
            adjMatrix[i] = new int[numVertices];
            for (int j = 0; j < numVertices; j++)
            {
                adjMatrix[i][j] = 1450;
                if (i == j)
                {
                    adjMatrix[i][j] = 0;
                }
            }
        }
    }
    // Add edges
    void addEdge(int i, int j, int value)
    {
        adjMatrix[i][j] = value;
        adjMatrix[j][i] = value;
    }
    // Remove edges
    void removeEdge(int i, int j)
    {
        adjMatrix[i][j] = 0;
        adjMatrix[j][i] = 0;
    }
    // Show the value
    int get_value(int i, int j)
    {
        return adjMatrix[i][j];
    }
    int **arr()
    {
        return adjMatrix;
    }
    ~Graph()
    {
        for (int i = 0; i < numVertices; i++)
            delete[] adjMatrix[i];
        delete[] adjMatrix;
    }
};

void advanced(string selectedCase)
{
    // insert your code here
    cout << "start your basic version of data structure final from here!" << endl;
    float discount_price;
    int rental_limit;

    Transfer_Log_Node_List transfer_logs_list;
    Bike_Node_List bikes_list;
    Bike_Node_List Stations_list[V];
    User_Node_List users_list;

    // bike_info.txt input
    // cout << " " << endl;
    // cout << "bike_info.txt input" << endl;
    char path_bike_info[100] = "testcases/";
    strcat(path_bike_info, selectedCase.c_str());
    strcat(path_bike_info, "/bike_info.txt");
    ifstream ifs_bike_info(path_bike_info, ios::in);
    if (!ifs_bike_info.is_open())
    {
        cout << "Failed to open file.\n";
    }
    string bike_type_template;
    float bike_ini_price_template;
    while (ifs_bike_info >> bike_type_template >> bike_ini_price_template)
    {
        discount_price = stof(bike_type_template);
        rental_limit = bike_ini_price_template;
        break;
    }
    while (ifs_bike_info >> bike_type_template >> bike_ini_price_template)
    {
        // cout << bike_type_template << " " << bike_ini_price_template << "\n";
        // Bike_info bike_info(bike_type_template, bike_ini_price_template);
        // bike_infos.push_back(bike_info);
    }
    ifs_bike_info.close();

    // bike.txt input and build the class of bike
    //  cout << " " << endl;
    //  cout << "bike.txt input" << endl;
    char path_bike[100] = "testcases/";
    strcat(path_bike, selectedCase.c_str());
    strcat(path_bike, "/bike.txt");
    ifstream ifs_bike(path_bike, ios::in);
    if (!ifs_bike.is_open())
    {
        cout << "Failed to open file.\n";
    }
    string bike_type;
    int bike_id;
    string station_id;
    float rental_price;
    int rental_count;
    while (ifs_bike >> bike_type >> bike_id >> station_id >> rental_price >> rental_count)
    {
        // cout << bike_type << " " << bike_id << " " << station_id << " " << rental_price << " " << rental_count << " " << "\n";
        station_id[0] = '0';
        int S = stoi(station_id);

        Bike_Node *Node = new Bike_Node(bike_id, bike_type, S, rental_price, rental_count);
        Stations_list[S].insert_by_price(Node);
        Stations_list[S].check();
    }
    ifs_bike.close();
    // for (int i = 0; i < V; i++)
    // {
    //     Bike_Node *aa =Stations_list[i].head_;
    //     while (aa != NULL)
    //     {
    //         cout << aa->Type << " " << aa->ID << " " << aa->Station << " " << aa->Rental_price << " " << aa->Rental_count << " "
    //              << "\n";
    //         aa = aa->next;
    //     }
    // }

    // map.txt input and build a graph
    // cout << " " << endl;
    // cout << "map.txt input" << endl;
    char path_map[100] = "testcases/";
    strcat(path_map, selectedCase.c_str());
    strcat(path_map, "/map.txt");
    ifstream ifs_map(path_map, ios::in);
    if (!ifs_map.is_open())
    {
        cout << "Failed to open file.\n";
    }
    string station_one;
    string station_two;
    int distance;
    Graph map(V);
    while (ifs_map >> station_one >> station_two >> distance)
    {
        // cout << station_one << " " << station_two << " " << distance << "\n";
        station_one[0] = '0';
        station_two[0] = '0';
        int S1 = stoi(station_one);
        int S2 = stoi(station_two);
        map.addEdge(S1, S2, distance);
    }
    ifs_map.close();
    floydWarshal_ad(map.arr());

    // user.txt input
    // cout << " " << endl;
    // cout << "user.txt input" << endl;
    char path_user[100] = "testcases/";
    strcat(path_user, selectedCase.c_str());
    strcat(path_user, "/user.txt");
    ifstream ifs_user(path_user, ios::in);
    if (!ifs_user.is_open())
    {
        cout << "Failed to open file.\n";
    }
    string user_ID;
    string user_Accept_Bike_Types;
    int user_Start_Time;
    int user_End_Time;
    string user_Start_Point;
    string user_End_Point;
    while (ifs_user >> user_ID >> user_Accept_Bike_Types >> user_Start_Time >> user_End_Time >> user_Start_Point >> user_End_Point)
    {
        // cout << user_ID << " " << user_Accept_Bike_Types << " " << user_Start_Time << " " << user_End_Time << " " << user_Start_Point << " " << user_End_Point << "\n";
        user_ID[0] = '0';
        int U = stoi(user_ID);
        user_Start_Point[0] = '0';
        int SP = stoi(user_Start_Point);
        user_End_Point[0] = '0';
        int EP = stoi(user_End_Point);
        // vector<string> user_Accept_Bike_Types_vector;
        // string vector[10];
        string *vector = new string[100];
        char str[user_Accept_Bike_Types.length() + 1];
        strcpy(str, user_Accept_Bike_Types.c_str());
        char spliter[] = ",";
        char *pch;
        pch = strtok(str, spliter);
        int count = 0;
        while (pch != NULL)
        {
            vector[count] = pch;
            pch = strtok(NULL, spliter);
            count++;
        }
        // cout << "User: " << U << " " << vector[0] << " " << user_Start_Time << " " << user_End_Time << endl;
        count = 0;
        // for(int i = 0; i <= 10; i ++) {
        //     cout << vector[i] << endl;
        // }
        // cout << "\n" << endl;
        User_Node *Node = new User_Node(U, vector, user_Start_Time, user_End_Time, SP, EP);
        users_list.insert_by_start_time(Node);
    }
    ifs_user.close();

    User_Node *bb = users_list.head_;
    // while (bb != NULL)
    // {
    //     cout << bb->ID << setw(6) << bb->Start_Time << setw(6) << bb->End_Time << setw(6) << bb->Start_Point << setw(6) << bb->End_Point << "\n";
    //     bb = bb->next;
    // }

    // start to solve the request
    bool select = false;
    User_Node *user = users_list.head_;
    while (user != NULL)
    {
        select = false;
        while ((user->End_Time - user->Start_Time > map.get_value(user->Start_Point, user->End_Point)) && (user->Start_Time >= 0) && (user->End_Time <= 1440))
        { // check that if user has enough time or not
            if (Stations_list[user->Start_Point].head_ != NULL)
            { // if the station where user start is not empty
                Bike_Node *bike_in_the_station = Stations_list[user->Start_Point].head_;

                while (bike_in_the_station != NULL)
                {
                    // if (user->ID == 19)
                    // {
                    //     cout << " 1. " << user->ID << " " << bike_in_the_station->ID << " " << bike_in_the_station->Rental_price << " " << bike_in_the_station->Type << endl;
                    // }
                    if (bike_in_the_station->Rental_count < rental_limit)
                    { // check if the bike is under the rental limit
                        if (bike_in_the_station->TimeIndex == 0 || bike_in_the_station->TimeIndex <= user->Start_Time)
                        { // III. If no bike is available at Start_time, the rental request should be rejected with no charge. (we assume users do not wait for bikes.)
                            int i = 0;
                            while (user->Accept_Bike_Types[i].compare("") == 1)
                            {
                                // if (user->ID == 19)
                                // {
                                //     //cout << " 2. " << user->ID << " " << bike_in_the_station->previous->ID << " " << bike_in_the_station->previous->Rental_price << " " << bike_in_the_station->Type << " " << user->Accept_Bike_Types[i] << endl;
                                //     cout << " 2. " << user->ID << " " << bike_in_the_station->ID << " " << bike_in_the_station->Rental_price << " " << bike_in_the_station->Type << " " << user->Accept_Bike_Types[i] << endl;
                                //     cout << " 2. " << user->ID << " " << bike_in_the_station->next->ID << " " << bike_in_the_station->next->Rental_price << " " << bike_in_the_station->Type << " " << user->Accept_Bike_Types[i] << endl;
                                //     cout << " 2. " << user->ID << " " << bike_in_the_station->next->next->ID << " " << bike_in_the_station->next->next->Rental_price << " " << bike_in_the_station->Type << " " << user->Accept_Bike_Types[i] << endl;
                                // }
                                if (bike_in_the_station->Type.compare(user->Accept_Bike_Types[i]) == 0)
                                { // if the bike that the user want is the same with the bike in the station or not
                                    // Bike_Node *j = Stations_list[user->End_Point].head_;
                                    // while (j != NULL)
                                    // {
                                    //     // Stations_list[user->End_Point].check();
                                    //     if (user->ID == 19)
                                    //     {
                                    //         cout << "Station :" << j->Station << " ID : " << j->ID << " price: " << j->Rental_price << endl;
                                    //         cout << "\n";
                                    //     }
                                    //     j = j->next;
                                    // }
                                    // cout << "\n";

                                    user->Bike_Id = bike_in_the_station->ID;
                                    user->Bike_Start_Time = user->Start_Time;
                                    user->Bike_End_Time = user->Start_Time + map.get_value(user->Start_Point, user->End_Point);
                                    user->Revenue = floor(bike_in_the_station->Rental_price * map.get_value(user->Start_Point, user->End_Point));

                                    Transfer_Log_Node *Node = new Transfer_Log_Node(bike_in_the_station->ID, user->Start_Point, user->End_Point, user->Start_Time, user->Start_Time + map.get_value(user->Start_Point, user->End_Point), user->ID);
                                    transfer_logs_list.insert_by_id(Node);

                                    Stations_list[user->Start_Point].Delete(bike_in_the_station); // Pop from the original  station

                                    bike_in_the_station->TimeIndex = user->Start_Time + map.get_value(user->Start_Point, user->End_Point); // Update the time index of the bike
                                    bike_in_the_station->Rental_price = round((bike_in_the_station->Rental_price - discount_price) * 10) / 10;
                                    bike_in_the_station->Rental_count += 1;
                                    // if (bike_in_the_station->Rental_count >= rental_limit)
                                    // {
                                    //     cout << " Bike : " << bike_in_the_station->ID << " Reach limit" << endl;
                                    // }
                                    bike_in_the_station->Station = user->End_Point;

                                    Stations_list[user->End_Point].insert_by_price(bike_in_the_station);
                                    Stations_list[user->End_Point].check();
                                    // j = Stations_list[user->End_Point].head_;
                                    // while(j != NULL)
                                    // {
                                    //     // Stations_list[user->End_Point].check();
                                    //     if (user->ID == 19)
                                    //     {
                                    //         cout << "Station :" << j->Station << " ID : " << j->ID << " price: " << j->Rental_price << endl;
                                    //         cout << "\n";
                                    //     }
                                    //     j = j->next;
                                    // }
                                    // cout << "\n";
                                    select = true;
                                    user->AcceptOrNot = 1;
                                    break;
                                }
                                i++;
                            }
                        }
                    }
                    if (select)
                    {
                        break;
                    }
                    bike_in_the_station = bike_in_the_station->next;
                }
            }
            // user can wait
            if ((select) || (user->End_Time - user->Start_Time <= map.get_value(user->Start_Point, user->End_Point)))
            {
                break;
            }
            user->Start_Time++;
        }
        user = user->next;
    }
    // cout << " " << endl;
    // cout << "station_status.txt" << endl;
    char path_station_status[100] = "result/";
    strcat(path_station_status, selectedCase.c_str());
    strcat(path_station_status, "/station_status.txt");
    ofstream out_station_status(path_station_status);
    // Bike_Node_List new_Stations_list[V];
    Bike_Node_List *new_Stations_list = new Bike_Node_List[V];
    for (int i = 0; i < V; i++)
    {
        if (Stations_list[i].head_ != NULL)
        {
            Bike_Node *it = Stations_list[i].head_;
            while (it != NULL)
            {
                Bike_Node *temp = it->next;
                Stations_list[i].Delete(it);
                new_Stations_list[i].insert_by_id(it);
                it = temp;
            }
        }
    }
    for (int i = 0; i < V; i++)
    {
        if (new_Stations_list[i].head_ != NULL)
        {
            Bike_Node *it = new_Stations_list[i].head_;
            while (it != NULL)
            {
                // cout << "S" << it->Station << " " << it->ID << " " << it->Type << " " << it->Rental_price << " " << it->Rental_count << endl;
                out_station_status << "S" << it->Station << " " << it->ID << " " << it->Type << " " << it->Rental_price << " " << it->Rental_count << endl;
                it = it->next;
            }
        }
    }
    out_station_status.close(); // 關閉檔案

    // cout << " " << endl;
    // cout << "user_result.txt" << endl;
    char path_user_result[100] = "result/";
    strcat(path_user_result, selectedCase.c_str());
    strcat(path_user_result, "/user_result.txt");
    ofstream out_user_result(path_user_result);
    User_Node *t = users_list.head_;
    User_Node_List new_user_list;
    while (t != NULL)
    {
        User_Node *temp = t->next;
        users_list.Delete(t);
        new_user_list.insert_by_id(t);
        t = temp;
    }
    User_Node *tt = new_user_list.head_;
    while (tt != NULL)
    {
        // cout << "U" << tt->ID << " " << tt->AcceptOrNot << " " << tt->Bike_Id << " " << tt->Bike_Start_Time << " " << tt->Bike_End_Time << " " << tt->Revenue << endl;
        out_user_result << "U" << tt->ID << " " << tt->AcceptOrNot << " " << tt->Bike_Id << " " << tt->Bike_Start_Time << " " << tt->Bike_End_Time << " " << tt->Revenue << endl;
        tt = tt->next;
    }
    out_user_result.close(); // 關閉檔案

    // //
    //     Transfer_Log_Node_List *t_list = new Transfer_Log_Node_List;
    //     char path_trans[100] = "sorted_ans/";
    //     strcat(path_trans, selectedCase.c_str());
    //     strcat(path_trans, "/transfer_log.txt");
    //     ifstream ifs_trans(path_trans, ios::in);
    //     if (!ifs_trans.is_open())
    //     {
    //         cout << "Failed to open file.\n";
    //     }
    //     int bike_id_;
    //     string Start_;
    //     string End_;
    //     int Start_t;
    //     int End_t;
    //     string R_id;
    //     while (ifs_trans >> bike_id_ >> Start_ >> End_ >> Start_t >> End_t >> R_id)
    //     {
    //         Start_[0] = '0';
    //         End_[0] = '0';
    //         R_id[0] = '0';
    //         int S = stoi(Start_);
    //         int E = stoi(End_);
    //         int R = stoi(R_id);
    //         Transfer_Log_Node *Node = new Transfer_Log_Node(bike_id_, S, E, Start_t, End_t, R);
    //         t_list->insert_by_trans_time(Node);
    //     }
    //     ifs_trans.close();

    //     Transfer_Log_Node *ma = transfer_logs_list.head_;
    //     Transfer_Log_Node_List *m_list = new Transfer_Log_Node_List;
    //     while(ma != NULL)
    //     {
    //         Transfer_Log_Node *Node = new Transfer_Log_Node(ma->Bike_Id, ma->Start_Station, ma->End_Station, ma->Transfer_Start_Time, ma->Transfer_End_Time, ma->Bike_Rider);
    //         m_list->insert_by_trans_time(Node);
    //         ma = ma->next;
    //     }

    //     Transfer_Log_Node *ta = t_list->head_;
    //     ma = m_list->head_;
    //     while(ma != NULL)
    //     {
    //         if ((ta->Bike_Id != ma->Bike_Id) || (ta->Start_Station != ma->Start_Station) || (ta->End_Station != ma->End_Station) || (ta->Transfer_Start_Time != ma->Transfer_Start_Time) || (ta->Transfer_End_Time != ma->Transfer_End_Time) || (ta->Bike_Rider != ma->Bike_Rider))
    //         {
    //             cout << " Ans : " << ta->Bike_Id << " " << ta->Start_Station << " " << ta->End_Station << " " << ta->Transfer_Start_Time << " " << ta->Transfer_End_Time << " " << ta->Bike_Rider << endl;
    //             cout << " Myy : " << ma->Bike_Id << " " << ma->Start_Station << " " << ma->End_Station << " " << ma->Transfer_Start_Time << " " << ma->Transfer_End_Time << " " << ma->Bike_Rider << endl;
    //             cout << "\n";
    //         }
    //         else {
    //             cout << " Check " << endl;
    //         }
    //         ta = ta->next;
    //         ma = ma->next;
    //     }
    // //

    // cout << " " << endl;
    // cout << "transfer_log.txt" << endl;
    char path_transfer_log[100] = "result/";
    strcat(path_transfer_log, selectedCase.c_str());
    strcat(path_transfer_log, "/transfer_log.txt");
    ofstream out_transfer_log(path_transfer_log);
    Transfer_Log_Node *current = transfer_logs_list.head_;
    while (current != NULL)
    {
        // cout << current->Bike_Id << " "
        //      << "S" << current->Start_Station << " "
        //      << "S" << current->End_Station << " " << current->Transfer_Start_Time << " " << current->Transfer_End_Time << " "
        //      << "U" << current->Bike_Rider << endl;
        out_transfer_log << current->Bike_Id << " "
                         << "S" << current->Start_Station << " "
                         << "S" << current->End_Station << " " << current->Transfer_Start_Time << " " << current->Transfer_End_Time << " "
                         << "U" << current->Bike_Rider << endl;
        current = current->next;
    }
    out_transfer_log.close(); // 關閉檔案
}