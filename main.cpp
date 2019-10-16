/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Rui
 *
 * Created on October 14, 2016, 3:35 PM
 */

#include <cstdlib>
#include <iostream> 
#include <fstream> 
#include <vector> 
#include <ctime>
#include <math.h>

using namespace std;
double start_time(0);
double finish_time(0);

/*Define constant values*/

#define Positive_infinity 100000000
#define Negative_infinity -100000000

/*We Now Declare Our Global C++ Variables Related to Our Input Data*/

int Num_of_Jobs;
int* Processing_Time;
int* Due_Date;

/*We Now Declare Our Global C++ Variables Related to Statistics*/
int Global_LB(Negative_infinity);
int Global_UB(Positive_infinity);
int Number_of_Explored_Nodes(0);
int* Best_Solution;

/*We define the Branch and Node Class with the different understanding of B&B*/
class Branch_Node {
public:
    
    int* Parent_Partial_Solution_LB;
    int** Child_Partial_Solution_LB;
    int* Key_Partial_Solution_LB;
//    int* Job_assigned_index;
    int Job_assigned_number;
    int* Job_unassigned_index;
    int Job_unassigned_number;
    int* Key_candidate_index;
    int Key_partial_number;
    
    Branch_Node() {
        Key_Partial_Solution_LB = new int[Num_of_Jobs+1];
        for (int i=0;i<=Num_of_Jobs;i++){
            Key_Partial_Solution_LB[i] = 0;
        }
    }
    
    void Get_Parent_Node(Branch_Node* Parent){
        Parent_Partial_Solution_LB = new int[Num_of_Jobs+1];
        for (int i=0;i<=Num_of_Jobs;i++){
            Parent_Partial_Solution_LB[i] = Parent->Key_Partial_Solution_LB[i];
        }
    }
    
    void Explore_Information(Branch_Node* Parent){
        Job_assigned_number = 0;
        
        for(int i=0;i<Num_of_Jobs;i++){
            if(Parent->Key_Partial_Solution_LB[i]>0){
                Job_assigned_number++;
            }
        }
        
        Job_unassigned_number = Num_of_Jobs - Job_assigned_number;
        Job_unassigned_index = new int[Job_unassigned_number];
        int Index_num = 0;
        for(int i=0;i<Num_of_Jobs;i++){
            int Judge_num = 0;
            for(int j=0;j<Num_of_Jobs;j++){
                if((i+1)==Parent->Key_Partial_Solution_LB[j]){
                    Judge_num = 1;
                }
            }
            if(Judge_num == 0){
                Job_unassigned_index[Index_num]=i+1;
                Index_num++;
            }
        }
    }
    
    void Generate_Child_Node(Branch_Node* Parent){
        if(Job_unassigned_number>0){
            Child_Partial_Solution_LB = new int*[Job_unassigned_number];
            for(int i=0;i<Job_unassigned_number;i++){
                Child_Partial_Solution_LB[i]=new int[Num_of_Jobs+1];
                for(int j=0;j<Num_of_Jobs;j++){
                    if(j<Job_assigned_number){
                        Child_Partial_Solution_LB[i][j] = Parent->Key_Partial_Solution_LB[j];
                    }else{
                        if(j==Job_assigned_number){
                            Child_Partial_Solution_LB[i][j] = Job_unassigned_index[i];
                        }else{
                            Child_Partial_Solution_LB[i][j] = 0;
                        }
                    }
                }
                int Local_LB =Parent->Key_Partial_Solution_LB[Num_of_Jobs];
                int Increment = 0;
                for(int k=0;k<Job_unassigned_number;k++){
                    int temp_index = Job_unassigned_index[k];
                    Increment += Processing_Time[temp_index-1];
                }
                int temp_index2 = Job_unassigned_index[i];
                Increment -= Due_Date[temp_index2 - 1];
                Local_LB += fmax(Increment,0);
                int Temp_LB = fmax(Local_LB,0);
                Child_Partial_Solution_LB[i][Num_of_Jobs] = Temp_LB;
                Number_of_Explored_Nodes++;
            }
        }
    }

    void Choose_Key_Partial_Solution(){
        Key_candidate_index = new int[Job_unassigned_number];
        
        for(int i=0;i<Job_unassigned_number;i++){
            Key_candidate_index[i] = 0;
        }
        
        Key_partial_number = 0;
        
        if(Job_unassigned_number>0){
            int Temp_count;
            int Temp_position;
            for(int i=0;i<Job_unassigned_number;i++){
                Temp_count = 0;
                for(int j=0;j<Job_unassigned_number;j++){
                    if(Child_Partial_Solution_LB[i][Num_of_Jobs]<=Child_Partial_Solution_LB[j][Num_of_Jobs]){
                        Temp_count++;
                    }
                }
                Temp_position = Job_unassigned_number-Temp_count;
                Key_candidate_index[Temp_position]=i;
            }
            int Key_partial_position = Key_candidate_index[Key_partial_number];
            for (int j=0;j<=Num_of_Jobs;j++){
                Key_Partial_Solution_LB[j] = Child_Partial_Solution_LB[Key_partial_position][j];
            }
        }
    }
    
    void Rechoose_Key_Partial_Solution(){
        Key_partial_number++;
        if(Key_partial_number>=Job_unassigned_number){
            Key_Partial_Solution_LB[Num_of_Jobs] = Positive_infinity;
        }else{
            int Key_partial_position = Key_candidate_index[Key_partial_number];
            for (int j=0;j<=Num_of_Jobs;j++){
                Key_Partial_Solution_LB[j] = Child_Partial_Solution_LB[Key_partial_position][j];
            }
        }
    }
    
//    void Copy_Branch_Node(Branch_Node* Parent){
//        Parent_Partial_Solution_LB = new int[Num_of_Jobs+1];
//        for(int i=0;i<=Num_of_Jobs;i++){
//            Parent_Partial_Solution_LB[i] = Parent->Parent_Partial_Solution_LB[i];
//            Key_Partial_Solution_LB[i] = Parent->Key_Partial_Solution_LB[i];
//        }
//        
//        Child_Partial_Solution_LB = new int*[Job_unassigned_number];
//        for(int i=0;i<Parent->Job_unassigned_number;i++){
//            Child_Partial_Solution_LB[i]=new int[Num_of_Jobs+1];
//            for(int j=0;j<=Num_of_Jobs;j++){
//                Child_Partial_Solution_LB[i][j]=Parent->Child_Partial_Solution_LB[i][j];
//            }
//        }
//        Job_assigned_number = Parent->Job_assigned_number;
//        Job_unassigned_number = Parent->Job_unassigned_number;
//        
//        Job_unassigned_index = new int[Job_unassigned_number];
//        Key_candidate_index = new int[Job_unassigned_number];
//        for(int i=0;i<Parent->Job_unassigned_number;i++){
//            Job_unassigned_index[i] = Parent->Job_unassigned_index[i];
//            Key_candidate_index[i] = Parent->Key_candidate_index[i];
//        }
//        Key_partial_number = Parent->Key_partial_number;
//    }
    
    virtual ~Branch_Node() {
        delete [] Parent_Partial_Solution_LB;
        delete [] Child_Partial_Solution_LB;
        delete [] Key_Partial_Solution_LB;
        delete [] Job_unassigned_index;
        delete [] Key_candidate_index;
    }
    
private:
    
};

/*Load the Input data with the same method*/
void Reading_The_Input_File() {
    /* For reading from a file we should use the ifstream function */

    ifstream InputFile;
    InputFile.open("Input.txt");
    InputFile>>Num_of_Jobs;

    /* You can now create the arrays of processing times, and due dates */

    Processing_Time = new int [Num_of_Jobs];
    Due_Date = new int [Num_of_Jobs];

    /* Add values to the generated arrays */

    for (int i = 0; i < Num_of_Jobs; i++) {
        InputFile >> Processing_Time[i];
        InputFile >> Due_Date[i];
    }

    /* We are done with Input File, so we close it.  */

    InputFile.close();
}

/*Declare the B&B tree*/
vector <Branch_Node*>Tree_of_Nodes;

/*Initialize the Node to set up the iteration*/
void Initialize_Node() {
    /* Create the zero node, which will not be added to the tree */
    Branch_Node* Zero_Node = new Branch_Node;
    Number_of_Explored_Nodes++;
    Global_LB = Zero_Node->Key_Partial_Solution_LB[Num_of_Jobs];
    /* Create the initial node */
    Branch_Node* Initial_Node = new Branch_Node;;
    Initial_Node->Get_Parent_Node(Zero_Node);
    Initial_Node->Explore_Information(Zero_Node);
    Initial_Node->Generate_Child_Node(Zero_Node);
    Initial_Node->Choose_Key_Partial_Solution();
    Tree_of_Nodes.push_back(Initial_Node);
    Global_LB = Initial_Node->Key_Partial_Solution_LB[Num_of_Jobs];
}
void Writing_The_Output_File() {
    finish_time = clock();
    ofstream OutputFile;
    OutputFile.open("Results.txt", ios::app);
    OutputFile << (finish_time-start_time)/CLOCKS_PER_SEC<<" "<<Global_UB << " " << Number_of_Explored_Nodes << " ";
    for (int i = 0; i < Num_of_Jobs; i++) {
        OutputFile << Best_Solution[Num_of_Jobs - i - 1] << " ";
    }
    OutputFile << endl;
    OutputFile.close();
}

int main(int argc, char** argv) {
    Reading_The_Input_File();
    Best_Solution = new int [Num_of_Jobs];
    for (int i = 0; i < Num_of_Jobs; i++) {
        Best_Solution[i] = 0;
    }
    Initialize_Node();

    while (Tree_of_Nodes.size() > 0 && Global_UB > Global_LB) {
        
        if(Tree_of_Nodes.size()<=1){
            Branch_Node* New_Node = new Branch_Node;
            New_Node->Get_Parent_Node(Tree_of_Nodes.front());
            New_Node->Explore_Information(Tree_of_Nodes.front());
            New_Node->Generate_Child_Node(Tree_of_Nodes.front());
            New_Node->Choose_Key_Partial_Solution();
            Tree_of_Nodes.front()->Rechoose_Key_Partial_Solution();
            Tree_of_Nodes.push_back(New_Node);
        }
        else{
            int Min_num = 0;
            for (int i=0;i<Tree_of_Nodes.size();i++){
                int Great_num = 0;
                for(int j=0;j<Tree_of_Nodes.size();j++){
                    if(Tree_of_Nodes.at(i)->Key_Partial_Solution_LB[Num_of_Jobs]<=
                            Tree_of_Nodes.at(j)->Key_Partial_Solution_LB[Num_of_Jobs]){
                        Great_num++;
                    }
                }
                if(Great_num==Tree_of_Nodes.size()){
                    Min_num = i;
                    break;
                }
            }
            Branch_Node* New_Node = new Branch_Node;
            New_Node->Get_Parent_Node(Tree_of_Nodes.at(Min_num));
            New_Node->Explore_Information(Tree_of_Nodes.at(Min_num));
            New_Node->Generate_Child_Node(Tree_of_Nodes.at(Min_num));
            New_Node->Choose_Key_Partial_Solution();
            if (New_Node->Job_unassigned_number ==1 && Global_UB > New_Node->Key_Partial_Solution_LB[Num_of_Jobs]) {
                Global_UB = New_Node->Key_Partial_Solution_LB[Num_of_Jobs];
                for (int j = 0; j < Num_of_Jobs; j++) {
                    Best_Solution[j] = New_Node->Key_Partial_Solution_LB[j];
                }
            }
            Tree_of_Nodes.at(Min_num)->Rechoose_Key_Partial_Solution();
            Tree_of_Nodes.push_back(New_Node);
        }
        
        int Min_number = 0;
            for (int i=0;i<Tree_of_Nodes.size();i++){
                int Great_num = 0;
                for(int j=0;j<Tree_of_Nodes.size();j++){
                    if(Tree_of_Nodes.at(i)->Key_Partial_Solution_LB[Num_of_Jobs]<=
                            Tree_of_Nodes.at(j)->Key_Partial_Solution_LB[Num_of_Jobs]){
                        Great_num++;
                    }
                }
                if(Great_num==Tree_of_Nodes.size()){
                    Min_number = i;
                    break;
                }
            }
        
//        for (int i=0;i<=Num_of_Jobs;i++){
//            cout<<Tree_of_Nodes.at(Min_number)->Key_Partial_Solution_LB[i]<<"; ";
//        }
//        cout<<endl;
        
        Global_LB = Tree_of_Nodes.at(Min_number)->Key_Partial_Solution_LB[Num_of_Jobs];

    }
    
    Writing_The_Output_File();
//    cout << "-------------------------------------------------" << endl;
//    cout << "Optimal_Objective_Value: " << Global_UB << endl;
//    cout << "Number_of_Explored_Nodes: " << Number_of_Explored_Nodes << endl;
//    cout << "Optimal_Solution: ";
//    for (int i = 0; i < Num_of_Jobs; i++) {
//        cout << Best_Solution[i] << " ";
//    }

    return 0;
    cout << endl;
}
