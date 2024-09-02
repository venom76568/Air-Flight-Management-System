#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class time
{
    int hour;
    int min;
    int totalMin;

public:
    int getmin()
    {
        return min;
    }
    void setmin(int min)
    {
        this->min = min;
    }
    int gethour()
    {
        return hour;
    }
    void sethour(int hour)
    {
        this->hour = hour;
    }
    int gettotalMin()
    {
        return totalMin;
    }
    void setTotalMin(int totalMin)
    {
        this->totalMin = totalMin;
    }
};

class flightNode
{
    int flightID;
    time DT;
    time ETA;
    flightNode *next;

public:
    int getflightID()
    {
        return flightID;
    }
    void setflightID(int flightID)
    {
        this->flightID = flightID;
    }
    time getDT()
    {
        return DT;
    }
    void setDT(time DT)
    {
        this->DT = DT;
    }
    time getETA()
    {
        return ETA;
    }
    void setETA(time ETA)
    {
        this->ETA = ETA;
    }
    flightNode *getflightNode()
    {
        return next;
    }
    void setflightNode(flightNode *next)
    {
        this->next = next;
    }
    flightNode *createflightNode(int flightID, time DT, time ETA)
    {
        flightNode *newnode = new flightNode();
        newnode->setflightID(flightID);
        newnode->setDT(DT);
        newnode->setETA(ETA);
        return newnode;
    }
};

class bucketNode
{
    int bucketID;
    time beginETA;
    time endETA;
    flightNode *fhead;
    bucketNode *next;

public:
    int getbucketID()
    {
        return bucketID;
    }
    void setbucketID(int bucketID)
    {
        this->bucketID = bucketID;
    }
    time getbeginETA()
    {
        return beginETA;
    }
    void setbeginETA(time beginETA)
    {
        this->beginETA = beginETA;
    }
    time getendETA()
    {
        return endETA;
    }
    void setendETA(time endETA)
    {
        this->endETA = endETA;
    }
    flightNode *getfhead()
    {
        return fhead;
    }
    void setfhead(flightNode *fhead)
    {
        this->fhead = fhead;
    }
    bucketNode *getnextbucketNode()
    {
        return next;
    }
    void setnextbucketNode(bucketNode *next)
    {
        this->next = next;
    }
};
time addAdjustTime(time temp, int addMin)
{
    int min = temp.getmin();
    int hour = temp.gethour();
    int totalMin = temp.gettotalMin();
    min += addMin;
    if (min >= 60)
    {
        min -= 60;
        hour += 1;
    }
    if (hour >= 24)
    {
        hour -= 24;
    }
    totalMin = (hour * 60) + min;
    temp.setmin(min);
    temp.sethour(hour);
    temp.setTotalMin(totalMin);
    return temp;
}
bucketNode *createbucketNode(time beginETA)
{
    static int bucketID = 1000;
    bucketNode *newnode = new bucketNode();
    newnode->setbucketID(bucketID);
    bucketID++;
    newnode->setfhead(NULL);
    newnode->setnextbucketNode(NULL);
    newnode->setbeginETA(addAdjustTime(beginETA, 0));
    time temp = addAdjustTime(beginETA, 59);
    newnode->setendETA(temp);
    return newnode;
}
time calculateBucketBeginETA(time ETA, time curr)
{
    time temp;
    int diff = ETA.getmin() - curr.getmin();
    if (diff >= 0)
    {
        temp.sethour(ETA.gethour());
    }
    else
    {
        temp.sethour(ETA.gethour() - 1);
    }

    temp.setmin(curr.getmin());
    return temp;
}
void printTime(time temp)
{
    cout << temp.gethour() << "." << temp.getmin();
}
int timedeff(time A, time B)
{
    return abs(A.gettotalMin() - B.gettotalMin());
}

int maxtime(time A, time B)
{
    int ret_val;
    if (A.gettotalMin() > B.gettotalMin())
    {
        ret_val = -1;
    }
    else if (A.gettotalMin() < B.gettotalMin())
    {
        ret_val = 1;
    }
    else
    {
        ret_val = 0;
    }
    return ret_val;
}

void printFlight(flightNode *flight)
{
    cout << "ID - " << flight->getflightID() << "  ";
    cout << " DT- ";
    printTime(flight->getDT());
    cout << "  ETA - ";
    printTime(flight->getETA());
}
void printFlightPlan(bucketNode *bhead, int fID)
{
    int f = 0; // flag
    bucketNode *bptr;
    flightNode *fptr;
    bptr = bhead;
    fptr = NULL;
    if (bhead == NULL)
    {
        cout << "Error, Bucket List is empty";
    }
    else
    {
        while (bptr != NULL && f == 0)
        {
            fptr = bptr->getfhead();
            while (fptr != NULL && f == 0)
            {
                if (fptr->getflightID() == fID)
                {
                    f = 1;
                }
                else
                {
                    fptr = fptr->getflightNode();
                }
            }
            if (f == 0)
            {
                bptr = bptr->getnextbucketNode();
            }
        }

        // if flight plan does not found
        if (f == 0)
        {
            cout << "No such flight found\n";
        }
        else
        {
            printFlight(fptr);
        }
    }
}

void printBucket(bucketNode *bucket)
{
    cout << "Bucket ID - " << bucket->getbucketID();
    cout << " from ";
    printTime(bucket->getbeginETA());
    cout << " to ";
    printTime(bucket->getendETA());
}
void printAll(bucketNode *bhead)
{
    bucketNode *bptr;
    flightNode *fptr;
    bptr = bhead;
    fptr = NULL;
    if (bhead == NULL)
    {
        cout << "Error, No bucket in the list\n";
    }

    while (bptr != NULL)
    {
        printBucket(bptr);
        cout << "\nFlights: \n";
        fptr = bptr->getfhead();
        while (fptr != NULL)
        {
            printFlight(fptr);
            fptr = fptr->getflightNode();
            cout << "\n";
        }
        bptr = bptr->getnextbucketNode();
    }
}

bucketNode *InsertFlight(bucketNode *bhead, flightNode *newNode, time curr)
{
    if (bhead == NULL)
    {
        time beginETA = calculateBucketBeginETA(newNode->getETA(), curr);
        bhead = createbucketNode(beginETA);
        bhead->setfhead(newNode);
    }
    else
    {
        bucketNode *bptr = bhead;
        bucketNode *bprev = NULL;
        while (bptr != NULL && bptr->getbeginETA().gettotalMin() <= newNode->getETA().gettotalMin())
        {
            bprev = bptr;
            bptr = bptr->getnextbucketNode();
        }
        if (bprev != NULL)
        {
            if (timedeff(bprev->getbeginETA(), newNode->getETA()) <= 59)
            {
                flightNode *fptr = bprev->getfhead();
                flightNode *fprev = NULL;
                if (fptr == NULL) // probably wont occur as you dont maintain empty buckets
                {
                    bprev->setfhead(newNode);
                }
                else
                {
                    while (fptr != NULL && fptr->getDT().gettotalMin() <= newNode->getDT().gettotalMin())
                    {
                        fprev = fptr;
                        fptr = fptr->getflightNode();
                    }
                    if (fprev == NULL)
                    {
                        newNode->setflightNode(fptr);
                        bprev->setfhead(newNode);
                    }
                    else
                    {
                        newNode->setflightNode(fptr);
                        fprev->setflightNode(newNode);
                    }
                }
            }
            else
            {
                // if correct bucket not found, create bucket, insert it in the right place between bprev and bptr and then insert flight plan
                time beginETA = calculateBucketBeginETA(newNode->getETA(), curr);
                bucketNode *newbucket = createbucketNode(beginETA);
                newbucket->setnextbucketNode(bptr);
                bprev->setnextbucketNode(newbucket);
                newbucket->setfhead(newNode);
            }
        }
        else
        {
            // bptr == bhead
            // you need to create a bucket at the start and insert the flight node there
            time beginETA = calculateBucketBeginETA(newNode->getETA(), curr);
            bucketNode *newbucket = createbucketNode(beginETA);
            newbucket->setnextbucketNode(bptr);
            bhead = newbucket;
            newbucket->setfhead(newNode);
        }
    }
    return bhead;
}

bucketNode *cancelFlightPlan(bucketNode *bhead, int fID)
{
    int f = 0;
    bucketNode *bptr, *bprev;
    flightNode *fptr, *fprev;
    bptr = bhead;
    bprev = NULL;
    fptr = fprev = NULL;
    if (bhead == NULL)
    {
        cout << "Error, there is no bucket in the list";
    }
    else
    {
        // finding the flight plan
        while (bptr != NULL && f == 0)
        {
            fprev = fptr;
            fptr = bptr->getfhead();
            while (fptr != NULL && f == 0)
            {
                if (fptr->getflightID() == fID)
                {
                    f = 1;
                }
                else
                {
                    fprev = fptr;
                    fptr = fptr->getflightNode();
                }
            }
            if (f == 0)
            {
                bprev = bptr;
                bptr = bptr->getnextbucketNode();
            }
        }
    }
    if (f == 0)
    {
        cout << "No such flight found" << endl;
    }
    else
    {
        // if the flight is at start of the flight plan list or it is the only flight plan in the flight plan list
        if (fprev == NULL)
        {
            bptr->setfhead(fptr->getflightNode());
            delete (fptr);
            if (bptr->getfhead() == NULL)
            {
                // delete the bucket
                if (bprev == NULL)
                {
                    bhead = bptr->getnextbucketNode();
                    delete (bptr);
                }
                else
                {
                    bprev->setnextbucketNode(bptr->getnextbucketNode());
                    delete (bptr);
                }
            }
        }

        // if the flight is at middle or end of the flight plan list
        else
        {
            fprev->setflightNode(fptr->getflightNode());
            delete (fptr);
        }
        cout << "\nFlight with id " << fID << " cancelled. \n\033[0;33m------------------------------New List-------------------------------------------------------\033[0m\n"
             << endl;
        printAll(bhead);
    }
    return bhead;
}

time stringToTime(const string &timeStr)
{
    time t;
    int hour, min;
    char colon;
    stringstream ss(timeStr);
    ss >> hour >> colon >> min;
    t.sethour(hour);
    t.setmin(min);
    t.setTotalMin(hour * 60 + min);
    return t;
}

// Function to read and import flights from CSV
void loadFlightsFromFile(const string &filename, bucketNode **bbhead, time curr)
{
    bucketNode *bhead = *bbhead;
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Could not open the file!" << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string flightIDStr, DTStr, ETAStr;

        getline(ss, flightIDStr, ',');
        getline(ss, ETAStr, ',');
        getline(ss, DTStr, ',');

        int flightID = stoi(flightIDStr);
        time DT = stringToTime(DTStr);
        time ETA = stringToTime(ETAStr);

        flightNode *newFlight = new flightNode();
        newFlight->setflightID(flightID);
        newFlight->setDT(DT);
        newFlight->setETA(ETA);
        newFlight->setflightNode(NULL);

        bhead = InsertFlight(bhead, newFlight, curr);
    }

    file.close();
    *bbhead = bhead;
}
int main()
{
    bucketNode *bucketHead = nullptr;
    string filename = "FlightData.csv";
    int n, id;
    time currTime;
    currTime.sethour(0);
    currTime.setmin(0);
    currTime.setTotalMin(0);

    time prevTime;
    prevTime.sethour(0);
    prevTime.setmin(0);
    prevTime.setTotalMin(0);

    currTime = addAdjustTime(currTime, 0);
    prevTime = addAdjustTime(prevTime, 0);

    cout << "\033[0;34m\n------------------------------Loading Today's Flights----------------------------------------------\n";
    loadFlightsFromFile(filename, &bucketHead, currTime);

    do
    {
        cout << "\033[0;36m\n\n------------------------------Program Menu - ATC for the whole day--------------------------------------\n";
        cout << "\nPress 1: Display all flights.";
        cout << "\nPress 2: Display a particular flight.";
        cout << "\nPress 3: Cancel a particular flight.";
        cout << "\nPress 4: Add flight.\n";
        cout << "\nPress 5: Exit Program.\n";
        cin >> n;
        cout << "\033[0;37m";

        if (n == 1)
        {
            cout << "\n\033[0;33m------------------------------Today's Flights------------------------------\033[0m\n\n";
            printAll(bucketHead);
        }
        else if (n == 2)
        {
            cout << "\n\nEnter the ID of the flight: ";
            cin >> id;
            printFlightPlan(bucketHead, id);
        }
        else if (n == 3)
        {
            cout << "\nEnter the ID of the flight to be cancelled: ";
            cin >> id;
            bucketHead = cancelFlightPlan(bucketHead, id);
        }
        else if (n == 4)
        {
            int flightID;
            cout << "\nEnter flight ID:" << endl;
            cin >> flightID;
            time eta, dt;
            int eh, em, dh, dm;
            cout << "Enter expected time of arrival: " << endl;

            do
            {
                cout << "\nHour: " << endl;
                cin >> eh;
                eta.sethour(eh);

            } while (eta.gethour() > 23 && cout << "\nEnter valid hour.");
            do
            {
                cout << "\nMin: " << endl;
                cin >> em;
                eta.setmin(em);
            } while (eta.getmin() > 59 && cout << "\nEnter valid min.");
            eta = addAdjustTime(eta, 0);

            cout << "\nEnter departure time: " << endl;
            do
            {
                cout << "\nHour: " << endl;
                cin >> dh;
                dt.sethour(dh);

            } while (dt.gethour() > 23 && cout << "\nEnter valid hour.");
            do
            {
                cout << "\nMin: " << endl;
                cin >> dm;
                dt.setmin(dm);
            } while (dt.getmin() > 59 && cout << "\nEnter valid min.");
            dt = addAdjustTime(dt, 0);

            flightNode *newNode;
            newNode = newNode->createflightNode(flightID, dt, eta);
            bucketHead = InsertFlight(bucketHead, newNode, currTime);
            cout << "\nAdded flight with id" << flightID << endl;
            cout << "\n-------------------------------Updated flight list -----------------------\n";
            printAll(bucketHead);
        }
        else if (n > 5 || n < 1)
        {
            cout << "\nEnter valid number.";
        }
        prevTime = currTime;
    } while (n != 5);

    cout << "\n\033[0;33m------------------------------Thank you!------------------------------\033[0m\n\n";
    return 0;
}