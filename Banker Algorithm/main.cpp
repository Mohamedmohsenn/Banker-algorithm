#include <iostream>
#include<vector>
#include<sstream>

using namespace std;

class Banker
{
    int numOfRes,numOfProc;
    int **allocation,**maximum,**need;
    int *work,*available;
    bool *finish;
    vector<int> finalSequence;
    vector<int> extraElements;
    bool extraRecourceCheckConditions()
    {
        int* tmpArr = new int[numOfRes];
        for(int i = 0 ; i < numOfRes ; i++)
            tmpArr[i] = extraElements[i+1];

        for(int col = 0 ; col < numOfRes ; col++)
        {
            if(tmpArr[col] > need[extraElements[0]][col])
                return false;
        }

        for(int i = 0 ; i < numOfRes ; i++)
        {
            if(tmpArr[i] > available[i])
                return false;
        }

        return true;
    }

    void editAfterAdd()
    {
        int* tmpArr = new int[numOfRes];
        for(int i = 0 ; i < numOfRes ; i++)
        {
            tmpArr[i] = extraElements[i+1];
        }
        ///edit allocation
        for(int col = 0 ; col < numOfRes ; col++)
            allocation[extraElements[0]][col] += tmpArr[col];
        ///edit need
        for(int col = 0; col < numOfRes ; col++)
            need[extraElements[0]][col]-=tmpArr[col];

        ///edit available
        for(int i = 0 ; i < numOfRes ; i++)
        {
            available[i]-=tmpArr[i];
        }

        ///edit finish
        for(int i = 0 ; i < numOfProc ; i++)
        {
            finish[i] = false;
        }

        ///edit work
        for(int i = 0 ; i < numOfRes ; i++)
        {
            work[i]= available[i];
        }
    }
    void split(string x)
    {
        string tmp = "";
        for(int i=1 ; i < x.size() ; i++)
        {
            if(x[i] != ' ')
            {
                tmp+=x[i];
            }
            if(x[i] == ' ' || i == x.size()-1)
            {
                stringstream str(tmp);
                int x = 0;
                str>>x;
                extraElements.push_back(x);
                tmp="";
            }
        }
    }


    void printFinalSequence()
    {
        cout<<"\nFinal Sequence is : ";
        for(int i = 0 ; i < finalSequence.size() ; i++)
            cout<<"P"<<finalSequence[i]<<" ";
        cout<<endl;
    }

    bool compare(int *need, int *avail, int processNumber)
    {
        if(finish[processNumber])
        {
            return false;
        }
        for( int i = 0 ; i < numOfRes ; i++)
        {
            if(need[i] > avail[i])
                return false;
        }
        return true;
    }


    bool isResourcesAvailable()
    {
        for(int i = 0 ; i < numOfProc ; i++)
        {
            int *tmpNeedArr = new int[numOfRes];
            for(int j = 0 ; j < numOfRes ; j++)
            {
                tmpNeedArr[j] = need[i][j];
            }

            if(compare(tmpNeedArr,work,i))
            {
                return true;
            }

            delete []tmpNeedArr;
        }
        return false;
    }


    void printNeed()
    {
        for(int i = 0 ; i < numOfProc ; i++)
        {
            for(int j = 0 ; j < numOfRes ; j++)
            {
                cout<<need[i][j]<<" ";
            }
            cout<<endl;
        }
    }

    void printAvailableRecources()
    {
        for(int m = 0 ; m < numOfRes ; m++)
        {
            cout<<work[m]<<" ";
        }

    }

    void printAllocatedRecources()
    {
        for(int i = 0 ; i < numOfProc ; i++)
        {
            for(int j = 0 ; j < numOfRes ; j++)
            {
                cout<<allocation[i][j]<<" ";
            }
            cout<<endl;
        }
    }
    void printMaximumRecources()
    {
        for(int i = 0 ; i < numOfProc ; i++)
        {
            for(int j = 0 ; j < numOfRes ; j++)
            {
                cout<<maximum[i][j]<<" ";
            }
            cout<<endl;
        }
    }
public :
    void getNeed(int ** allocate, int ** maxi)
    {
        for(int i = 0 ; i < numOfProc ; i++)
        {
            for(int j = 0 ; j < numOfRes ; j++)
            {
                need[i][j] = maxi[i][j] - allocate[i][j];
            }
        }
    }

    void allocate()
    {
        static bool flag = true;
        finalSequence.clear();
        extraElements.clear();

        cout<<"\nallocated is : "<<endl;
        printAllocatedRecources();

        cout<<"\nmaximum is : "<<endl;
        printMaximumRecources();

        cout<<"\nneed is : "<<endl;
        printNeed();

        int unfinishedProcessess = numOfProc;
        int i = 0;
        while(true)
        {
            int *tmpNeedArr = new int[numOfRes];
            int *tmpAllocationArr = new int[numOfRes];
            int index = 0;

            for(int j = 0 ; j < numOfRes ; j++)
            {
                tmpNeedArr[j] = need[i][j];
                tmpAllocationArr[j] = allocation[i][j];
                index = i;
            }

            if(compare(tmpNeedArr,work,i))
            {
                cout<<"\nAvailable recource : "<<endl;
                printAvailableRecources();
                cout<<endl;
                cout<<"Current Request : "<<endl;
                unfinishedProcessess--;
                for(int j = 0 ; j < numOfRes ; j++)
                {
                    cout<<need[index][j]<<" ";
                    work[j]+=tmpAllocationArr[j];
                    finish[i] = true;
                }
                finalSequence.push_back(index);
                cout<<"\nApproved"<<endl;
            }
            else
            {
                if(finish[i] == false)
                {
                    cout<<"\nAvailable recource : "<<endl;
                    printAvailableRecources();
                    cout<<endl;
                    cout<<"Current Request : "<<endl;
                    for(int j = 0 ; j < numOfRes ; j++)
                    {
                        cout<<need[index][j]<<" ";
                    }
                    cout<<"\nDenied"<<endl;
                }
            }
            delete[]tmpNeedArr;
            delete[]tmpAllocationArr;

            if(unfinishedProcessess !=0 && i == numOfProc-1)
            {
                i = -1;
            }
            else if(unfinishedProcessess == 0)
            {
                cout<<"\nFinal available recource : "<<endl;
                printAvailableRecources();
                cout<<endl;
                cout<<"safe \n";
                printFinalSequence();
                string x="";
                if(flag == true)
                    cin.ignore();
                getline(cin,x);
                if(x == "Quit")
                    return;
                else
                {
                    flag = false;
                    finalSequence.clear();
                    extraElements.clear();
                    split(x);
                    if(extraRecourceCheckConditions())
                    {
                        editAfterAdd();
                        allocate();
                        return;
                    }
                    else
                    {
                        cout<<"Not safe\n";
                        return;
                    }
                }
            }
            if(isResourcesAvailable() == false)
            {
                for(int index = i + 1 ; index < numOfProc ; index++)
                {
                    if(!finish[index])
                    {
                        cout<<"\nAvailable recource : "<<endl;
                        printAvailableRecources();
                        cout<<endl;
                        cout<<"\nCurrent Request : "<<endl;
                        for(int j = 0 ; j < numOfRes ; j++)
                            cout<<need[index][j]<<" ";
                        cout<<"\nDenied"<<endl;
                    }
                }
                cout<<"\nNot safe\n";
                return;
            }
            i++;
        }
    }

    friend istream& operator>> (istream &in,Banker& b);
    ~Banker()
    {
        delete[] allocation;
        delete[] maximum;
        delete[] need;
        delete[] work;
        delete[] available;
        delete[] finish;
    }

};

istream& operator>> (istream &in,Banker& b)
{
    cout<<"Enter the Number of proccessess : "<<endl;
    in>>b.numOfProc;
    cout<<"Enter the Number of recources : "<<endl;
    in>>b.numOfRes;
    b.allocation = new int*[b.numOfProc];
    b.maximum = new int*[b.numOfProc];
    b.need = new int*[b.numOfProc];
    b.finish = new bool[b.numOfProc];
    b.available = new int[b.numOfRes];
    b.work = new int[b.numOfRes];
    for(int i = 0 ; i < b.numOfProc ; i++)
    {
        b.finish[i] = false;
        b.allocation[i] = new int[b.numOfProc];
        b.maximum[i] = new int[b.numOfProc];
        b.need[i] = new int[b.numOfProc];
    }
    cout<<"Enter the available recouces : "<<endl;
    for(int i = 0 ; i < b.numOfRes ; i++)
    {
        cin>>b.available[i];
        b.work[i] = b.available[i];
    }
    cout<<"Enter the allocated recources : "<<endl;
    for(int i = 0 ; i < b.numOfProc ; i++)
    {
        for(int j = 0 ; j < b.numOfRes ; j++)
        {
            cin>>b.allocation[i][j];
        }
    }

    cout<<"Enter the maximum needed recources : "<<endl;
    for(int i = 0 ; i < b.numOfProc ; i++)
    {
        for(int j = 0 ; j < b.numOfRes ; j++)
        {
            cin>>b.maximum[i][j];
        }
    }

    b.getNeed(b.allocation,b.maximum);
    return in;

}
int main()
{
    Banker b;
    cin>>b;
    b.allocate();
    return 0;
}
