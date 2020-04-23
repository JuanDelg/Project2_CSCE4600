#include <iostream>
#include <fstream>
#include <regex>
#include <vector>

using namespace std;

int main() {
    /****************VARIABLES***************/
    ifstream inFile;
    string fileName, currentLine, numProcesses, numResources, rStr;
    vector<vector<int>> adjMat;
    int processes, resources, *resourceList;
    /****************************************/
    //loop prompts for file name until correct name is given
    do{
        inFile.clear();
        cout << "Enter File name: ";
        cin >> fileName;
        inFile.open(fileName.c_str());
        if(!inFile){
            cout << "File does not exist." << endl;
        }
    }while(!inFile);

    //grab each line of the file
    while(getline(inFile, currentLine, '\n')){
        //if the line does not start with a '%' and the line is not empty
        if(currentLine[0] != '%' && currentLine != ""){
            //checks if regex found match
            smatch sm;
            //in the current line check for the pattern num_processes=<any number>
            if(regex_match(currentLine, sm, regex("(num_processes=)\\d"))){
                //grab the number from the string
                numProcesses = currentLine.substr(14, currentLine.size());
                //convert the string to an int
                processes = stoi(numProcesses);
                //create a list of resources for each process
                resourceList = new int[processes];
                //prime a regex string for later use
                rStr = "(\\d,){" + to_string(processes - 1) + "}\\d";

            }
            //similar process to the block above except
            //check that the string matches the pattern num_resources=<any number>
            else if(regex_match(currentLine, sm, regex("(num_resources=)\\d"))){
                numResources = currentLine.substr(14, currentLine.size());
                resources = stoi(numResources);

            }
            //check for the patter created in the first if block
                //(\d,){#processes - 1}(\d)
                //a number followed by a ',' appearing equal to the number of processes - 1
                //followed by any number
            else if(regex_match(currentLine, sm, regex(rStr))){
                //create a temp character array
                char *temp = new char[currentLine.size() + 1];
                //copy the current line into the array
                strcpy(temp, currentLine.c_str());
                //use strtok to save each number into the resource list
                resourceList[0] = stoi(string(strtok(temp, ",")));
                for(int i = 1; i < processes; i++){
                    resourceList[i] = stoi(string(strtok(NULL, ",")));
                }
            }
            else{
                //create a temp character array
                char *temp = new char[currentLine.size() + 1];
                strcpy(temp, currentLine.c_str());
                //create a vector of ints to hold each matrix
                vector<int> row;
                //check if the token is empty
                char* token = strtok(temp, ",");
                while(token != NULL){
                    //while not empty push back the number onto the vector
                    row.push_back(stoi(string(token)));
                    //search for the next number
                    token = strtok(NULL, ",");
                }
                //after the row has been filled push the vector back on to the ajdMat
                adjMat.push_back(row);
            }
        }
    }

    //Known after reading through the file
        //Number of processes
        //Number of resources
        //Number of resources for each process
        //The adjacency matrix

    delete [] resourceList;
    inFile.close();
    return 0;
}
