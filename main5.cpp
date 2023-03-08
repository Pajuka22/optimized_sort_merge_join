/* This is a skeleton code for Optimized Merge Sort, you can make modifications as long as you meet 
   all question requirements*/  

#include <bits/stdc++.h>
#include "record_class.h"
#include <stdio.h>
using namespace std;

//defines how many blocks are available in the Main Memory 
#define buffer_size 22

Records buffers[buffer_size]; //use this class object of size 22 as your main memory

int numEmpRuns = 0;
int numDeptRuns = 0;

/***You can change return type and arguments as you want.***/

//Sorting the buffers in main_memory and storing the sorted records into a temporary file (runs) 
void Sort_Buffer(bool emp){
    cout << "sorting" << endl;
    //Remember: You can use only [AT MOST] 22 blocks for sorting the records / tuples and create the runs
    int sorted[22] = {-1};
    for(int i = 0; i < buffer_size && buffers[i].no_values != -1; ++i){
        int min = i;
        for(int ii = i; ii < buffer_size && buffers[ii].no_values != -1; ++ii){
            int id1 = emp ? buffers[ii].emp_record.eid : buffers[ii].dept_record.managerid;
            int id2 = emp ? buffers[min].emp_record.eid : buffers[min].dept_record.managerid;
            if(id1 < id2) min  = ii;
        }
        Records swp = buffers[i];
        buffers[i] = buffers[min];
        buffers[min] = swp;
    }
    string fname = (emp ? string("emp") : string("dept")) + "_run_" + std::to_string(emp ? numEmpRuns : numDeptRuns);
    fstream output(fname, ios::out | ios::trunc);
    for(int i = 0; i < buffer_size && buffers[i].no_values != -1; ++i){
        if(emp){
            output << to_string((int)buffers[i].emp_record.eid) << ',' << buffers[i].emp_record.ename << "," << buffers[i].emp_record.age << ',' << to_string((int)buffers[i].emp_record.salary) << endl;
        }
        else{
            output << to_string((int)buffers[i].dept_record.did) << "," << buffers[i].dept_record.dname << "," << to_string((int)buffers[i].dept_record.budget) << "," << to_string((int)buffers[i].dept_record.managerid) << endl;
        }
    }
    ++(emp ? numEmpRuns : numDeptRuns);
    return;
}

//Prints out the attributes from empRecord and deptRecord when a join condition is met 
//and puts it in file Join.csv
void PrintJoin(bool sortingDept, fstream &output) {
    cout << endl << endl;
    if(sortingDept){
        cout << buffers[0].dept_record.managerid << "\t" << buffers[1].emp_record.eid << endl;
        output << buffers[0].dept_record.did << "," << buffers[0].dept_record.dname << "," << to_string((int)buffers[0].dept_record.budget) << "," <<buffers[0].dept_record.managerid << "," << buffers[1].emp_record.ename << "," << buffers[1].emp_record.age << "," << buffers[1].emp_record.salary << endl;
    }
    else{
        cout << buffers[0].emp_record.eid << "\t" << buffers[1].dept_record.managerid << endl;
        output << buffers[1].dept_record.did << "," << buffers[1].dept_record.dname << "," << to_string((int)buffers[1].dept_record.budget) << "," <<buffers[1].dept_record.managerid << "," << buffers[0].emp_record.ename << "," << buffers[0].emp_record.age << "," << buffers[0].emp_record.salary << endl;
    }
    cout << endl << endl;
    return;
}

//Use main memory to Merge and Join tuples 
//which are already sorted in 'runs' of the relations Dept and Emp 
void Merge_Join_Runs(fstream &join){
    bool sortingDept = numDeptRuns <= numEmpRuns;
    string l, r;
    Records (*GrabLRecord)(fstream&);
    Records (*GrabRRecord)(fstream&);
    int leftNum, rightNum;
    if(sortingDept){
        leftNum = numDeptRuns;
        rightNum = numEmpRuns;
        l = string("dept_run_");
        r = string("emp_run_");
        GrabLRecord = &Grab_Dept_Record;
        GrabRRecord = &Grab_Emp_Record;
    }
    else{
        leftNum = numEmpRuns;
        rightNum = numDeptRuns;
        l = string("emp_run_");
        r = string("dept_run_");
        GrabLRecord = &Grab_Emp_Record;
        GrabRRecord = &Grab_Dept_Record;
    }
    fstream left;
    fstream right[rightNum];
    
    for(int i = 0; i < leftNum; ++i){
        left.open(l+to_string(i), ios::in);
        for(int ii = 0; ii < rightNum; ++ii){
            cout << r + to_string(ii) << endl;
            right[ii].open(r+to_string(ii), ios::in | ios::binary);
        }
        int lComp, rComp;
        cout << "outer whiles" << endl;
        buffers[0] = GrabLRecord(left);
        while(buffers[0].no_values != -1){
            cout << buffers[0].emp_record.eid << "\t" << buffers[0].dept_record.managerid << endl;
            lComp = sortingDept ? buffers[0].dept_record.managerid : buffers[0].emp_record.eid;
            for(int ii = 0; ii < rightNum; ++ii){
                streamoff bitch = 0;
                cout << "inner whiles" << endl;
                do{
                    
                    bitch = right[ii].tellg();
                    buffers[1] = GrabRRecord(right[ii]);
                    cout << buffers[1].emp_record.eid << "\t" << buffers[1].dept_record.managerid << endl;
                    rComp = sortingDept ? buffers[1].emp_record.eid : buffers[1].dept_record.managerid;
                }while(buffers[1].no_values != -1 && lComp > rComp);
                right[ii].seekg(bitch);
                cout << endl;
                cout << lComp << "\t" << rComp << endl;
                cout << endl;
                if(lComp == rComp){
                    PrintJoin(sortingDept, join);
                    break;
                }
            }
            buffers[0] = GrabLRecord(left);
        }
    }
    //and store the Joined new tuples using PrintJoin()

    return;
}

int main() {

    //Open file streams to read and write
    //Opening out two relations Emp.csv and Dept.csv which we want to join
    fstream empin;
    fstream deptin;
    empin.open("Emp.csv", ios::in);
    deptin.open("Dept.csv", ios::in);   
    //Creating the Join.csv file where we will store our joined results
    fstream joinout;
    joinout.open("Join.csv", ios::out | ios::trunc);

    //1. Create runs for Dept and Emp which are sorted using Sort_Buffer()
    bool done = false;
    while(!done){
        for(int i = 0; i < buffer_size; ++i){
            buffers[i] = Grab_Emp_Record(empin);
            done = buffers[i].no_values == -1;
            if(done) break;
        }
        Sort_Buffer(true);
    }
    done = false;
    while(!done){
        for(int i = 0; i < buffer_size; ++i){
            buffers[i] = Grab_Dept_Record(deptin);
            done = buffers[i].no_values == -1;
            if(done) break;
        }
        Sort_Buffer(false);
    }

    //2. Use Merge_Join_Runs() to Join the runs of Dept and Emp relations 
    Merge_Join_Runs(joinout);

    //Please delete the temporary files (runs) after you've joined both Emp.csv and Dept.csv
    for(int i = 0; i < numDeptRuns; ++i){
        remove((string("dept_run_")+to_string(i)).c_str());
    }
    for(int i = 0; i < numEmpRuns; ++i){
        remove((string("emp_run_")+to_string(i)).c_str());
    }
    return 0;
}
