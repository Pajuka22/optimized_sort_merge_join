/* This is a skeleton code for Optimized Merge Sort, you can make modifications as long as you meet 
   all question requirements*/  

#include <bits/stdc++.h>
#include "record_class.h"

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
void PrintJoin() {
    
    return;
}

//Use main memory to Merge and Join tuples 
//which are already sorted in 'runs' of the relations Dept and Emp 
void Merge_Join_Runs(fstream &join){
    string left = numEmpRuns < numDeptRuns ? "emp_run_" : "dept_run_";
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


    //Please delete the temporary files (runs) after you've joined both Emp.csv and Dept.csv

    return 0;
}
