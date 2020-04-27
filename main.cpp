#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <list>

using namespace std;



//create lists for edges
list<int> *Graph;
//Resize list to number of resources, processes
void Resize(int nodes)
{
	Graph = new list<int>[nodes];
}

//function to add edges from a process to resource and vice-versa
void addEdge(int src, int dest)
{
	//directed so only src to dest
	Graph[src].push_back(dest);
}

//takes in adjacency matrix to connect directed edges to nodes
void addGraph(vector<vector<int>> Matrix)
{
	for (int i = 0; i < Matrix.size(); i++)
        {
                for (int j = 0; j < Matrix[i].size(); j++)
                {
			if(i != j)
			{
                		if(Matrix[i][j] == 1)
				{
					addEdge(i,j);
				}		
                	}
		}
        }
}

vector<int> DFSsearch(int start_node, int num_nodes)
{
	vector<bool> visit(num_nodes, false);
	vector<int> nums;
    // Create a queue, easier to implement stack
    stack<int> q;

    // Push the current source node.
    q.push(start_node);

    while (!q.empty())
    {
        // Pop a node of queue
        start_node = q.top();
        q.pop();
        if (!visit[start_node])
        {
            visit[start_node] = true;
	    nums.push_back(start_node);
	                
        }

        // Get all adjacent vertices of the popped vertex s
        // If a adjacent has not been visited, then push it
        // to the stack.
        for (auto i = Graph[start_node].begin(); i != Graph[start_node].end(); ++i)
            if (!visit[*i])
                q.push(*i);
    }
	return nums;

} 

bool ProcSearch(int proc_num, vector<int> num)
{
	for(int i = 0; i < num.size(); i++)
	{
		if(num.at(i) == proc_num)
		{
			return true;
		}
	}
	return false;
}
int main() {
    /****************VARIABLES***************/
    ifstream inFile;
    string fileName, currentLine, numProcesses, numResources, rStr;
    vector<vector<int>> adjMat;
    int processes, resources, *resourceList, Nodes;
    bool isKnot;
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

	//Knot detection algorithm
	//First Get number of nodes
	Nodes =  (processes + resources);
	//resize list
	Resize(Nodes);
	//add directed edges of graph
	addGraph(adjMat);
	vector<int> temp;

	
        for (int i = 0; i < adjMat.size(); i++)
        {
                for (int j = 0; j < adjMat[i].size(); j++)
                {
			cout << adjMat[i][j] << " ";
		}
		cout << endl;
	}
	
	for(int i = 0; i < processes; i++)
	{
		temp = DFSsearch(i, Nodes);
		for(int j = 0; j < processes; j++)
		  {	
			isKnot = ProcSearch(j, temp);
			if(isKnot == false)
			{
				cout << "Graph contains no knot..." << endl;
				exit(0);
			}
			
		}
		temp.clear();
	}	
	cout << "Graph contains a knot, deadlock in Graph... " << endl;

    return 0;
}
