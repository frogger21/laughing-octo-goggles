#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include <algorithm>
#include <time.h>
using namespace std;

/*
ATTEMPT AT FASTER THAN RECURSIVE VERSION OF KOSARAJU BY USING ITERATION!
*/

/*
	Structs
*/
struct Vertex {
	bool ExploredR; //was it explored
	long leader; //second DFS
	long finish; //first DFS
	vector<long> connectedVertices;  //A->B, then A is connected to B
	vector<long> connectedVerticesR; //reversed list A->B becomes A<-B
};

/*
	Functions
*/

void PrintVertices(Vertex *G, long max)
{
	for (long i = 1; i < max; i++)
	{
		std::cout << i << " : " << G[i].ExploredR << endl;
	}
}

void PrintEdges(Vertex *G, long max)
{
	for (long i = 1; i < max; i++)
	{
		for (long z = 0; z < G[i].connectedVertices.size(); z++)
		{
			std::cout << "A: " << i << ", B: " << G[i].connectedVertices[z] << endl;
		}
	}
}

//parses string line and looks for numbers to put into a list
//ignores all non number ascii
void split(string line, vector<long> &v)
{
	string::size_type sz;
	long NumCount = 0;
	long startPos, numLen, tracker, StringSize;
	long theNumber;
	StringSize = line.size();
	bool Num = false;
	bool temp = false;
	bool temp2 = false;
	for (long i = 0; i < StringSize; i++)
	{
		//is it a number?
		temp = false;
		if (line[i] >= 48 && line[i] <= 57)
		{
			temp = true;
		}
		//if it's not a number then 
		if (temp == false)
		{
			Num = false;
		}
		//previous char was not a number but current is: we got a new number
		if (Num == false && temp == true)
		{
			Num = true;
			startPos = i;
			numLen = 1; //we don't know yet fully
			tracker = i + 1;
			while (tracker < StringSize)
			{
				temp2 = false;
				if (line[tracker] <= 57 && line[tracker] >= 48)
				{
					temp2 = true;
				}
				if (temp2 == true)
				{
					numLen += 1;
					tracker++;
				}
				else
				{
					// don't need to check these anymore cuz we already know 
					//it's # -1 b/c we i++ in for loop
					i += numLen - 1;
					break;
				}
			}
			string test2 = line.substr(startPos, numLen);
			theNumber = stoll(test2, &sz); //convert string to long
			NumCount += 1;
			v.push_back(theNumber);
		}
	}
}

//just prints stack: 1, 2, 3, 4, 5, 6, 7 etc...
void PrintStack(vector<long> &S)
{
	for (long i = 0; i < S.size(); i++)
	{
		std::cout << S[i] << ", ";
	}
	std::cout << endl;
}

//for algorithm::sort
bool pairCompare(pair<long, long>& firstElem, pair<long, long>& secondElem)
{
	return firstElem.second > secondElem.second;
}

//kosaraju's two pass algorithm iterative not recursive to increase performance
//i tried to minimize function calls to improve performance
//it's long and messy...
void kosaraju(Vertex *G, long n, string OutFile, string OutFile2)
{
	std::cout << "****************************************\n";
	std::cout << "*Inside the Kosaraju iterative function!\n";
	/*
	STEP 1 RUN DFS ON REVERSE EDGES AND KEEP TRACK OF FINISHING TIME
	*/

	//--------------------------------------------------------------------------
	//Run DFS loop on G reversal & f(v) = finishing time of each v in G
	//--------------------------------------------------------------------------
	std::cout << " - Doing the first DFS on the reversed edges!\n";
	//from: http://stackoverflow.com/questions/24051386/kosaraju-finding-finishing-time-using-iterative-dfs
	//the difficult part of iterative DFS is to count the finish time.
	//the idea is to pop the stack but add v to another stack then we add 
	//the neighbours of v to the original stack
	//when time comes this v will pop again at the original stack
	//which we then know this was twice seen so it goes into if/else and we add time
	//so if we get a stack like ABCD on S
	//and ABCD on S2; if we pop D on s, we get S=ABC and we check if D exists in S2
	//if D exists in S2, we've already seen this (as it was explored before)
	//there now you can stamp its time and increment time
	// but in reality i don't use S2 because my vertex struct has a boolean for explored or not!
	long *T = new long(0); //global variable for # of nodes processed
	vector<long> S; //stack LIFO
	long v = 0;
	//assume nodes labeled 1 to n
	for (long i = n; i >= 1; i--)
	{
		//if i was not discovered then use DFS(G,i)
		if (G[i].ExploredR == false)
		{
			//call DFS
			S.push_back(i);
			while (S.empty() == false)
			{
				v = S.back();
				S.pop_back();
				if (G[v].ExploredR == false)
				{
					S.push_back(v);
					G[v].ExploredR = true; //now mark it as explored
					for (long j = 0; j < G[v].connectedVerticesR.size(); j++)
					{
						//go through all the edges
						if (G[G[v].connectedVerticesR[j]].ExploredR == false)
						{
							//only unexplored vertex added here
							S.push_back(G[v].connectedVerticesR[j]);
						}
					}
				}
				else
				{
					//this was already seen in S2 so this is 2nd time we see it
					if (G[v].finish == 0)
					{
						*T += 1;
						G[v].finish = *T;
						//cout << v << endl;
					}
				}
			}

		}
	}
	S.clear();  //dont need
	
	/*
	STEP 2 MAKE A NEW GRAPH WHERE: OLD GRAPH WAS A->B BECOMES A.finishTime -> B.finishTime
	*/
	//need new graph
	std::cout << " - Making a new Graph with the finish times as nodes!\n";
	Vertex *GG = new Vertex[n + 1];
	vector<long> temp;
	long temp2;
	for (long z = 0; z < n + 1; z++)
	{
		//initialize GG
		GG[z].ExploredR = false;
		GG[z].finish = 0;
		GG[z].leader = 0;
		for (long i2 = 0; i2 < G[z].connectedVertices.size(); i2++)
		{
			temp2 = G[z].connectedVertices[i2];
			temp.push_back(G[temp2].finish);
		}
		GG[G[z].finish].connectedVertices = temp;
		temp.clear();
	}

	/*
	STEP 3: RUN DFS ON NEW GRAPH AND NOW KEEP TRACK OF THE LEADER
	*/

	//Run DFS loop on G normal, processing nodes in decreasing order of finishing times
	std::cout << " - Running DFS again on the new graph!\n";
	long *S3 = new long(0); //leader
	map<long, long> leaderMap; //[leader vertex] = frequency seen
	map<long, long>::iterator it;
	//assume nodes labeled 1 to n
	for (long z = n; z >= 1; z--)
	{
		if (GG[z].ExploredR == false)
		{
			// node not yet discovered
			*S3 = z; //leader
			//do dfs
			S.push_back(z); //add to stack
			while (S.empty() == false)
			{			
				v = S.back(); 
				S.pop_back(); //pop stack
				GG[v].leader = *S3;
				if (GG[v].ExploredR == false)
				{
					//S.push_back(v); we dont need to do this anymore
					GG[v].ExploredR = true;
					//for map to use later in step 4
					it = leaderMap.find(GG[v].leader);
					if (it == leaderMap.end())
					{
						//this leader was never seen in the map
						leaderMap[GG[v].leader] = 1; //one case exists now
					}
					else {
						//this was seen so increment
						leaderMap[GG[v].leader] += 1;
					}

					//explore its neighbours
					for (long z2 = 0; z2 < GG[v].connectedVertices.size(); z2++)
					{
						if (GG[GG[v].connectedVertices[z2]].ExploredR == false)
						{
							//neighbour was unexplored so add to stack
							S.push_back(GG[v].connectedVertices[z2]);
						}
					}
				}
			}
		}
	}

	//testing 
	if (1 == 3) {
		cout << "Leaders\n";
		for (long i = 1; i <= n; i++)
		{
			cout << "V: " << i << ", leaders: " << GG[i].leader << endl;
		}
		cout << "MAPS" << endl;
		for (it = leaderMap.begin(); it != leaderMap.end(); ++it)
		{
			cout << it->first << " : " << it->second << endl;
		}
		system("PAUSE");
	}

	//more testing
	if (1 == 3) {
		cout << "Finish times!\n";
		for (long i = 1; i <= n; i++)
		{
			cout << "V: " << i << ", finish time: " << G[i].finish << endl;
		}
		cout << "Leaders\n";
		for (long i = 1; i <= n; i++)
		{
			cout << "V: " << i << ", leaders: " << GG[i].leader << endl;
		}
	}

	/*
	STEP 4: Print to a textfile the leader & its frequency
	*/
	std::cout << " - Printing list of leader nodes & their freqeuency!\n";
	ofstream myfile2;
	myfile2.open(OutFile);
	for (long zzz = 1; zzz <= n; zzz++)
	{
		myfile2 << zzz << ", " << GG[zzz].leader << endl;
	}
	myfile2.close();

	/*
	STEP 5: Copy values of leader frequency and put it into  vector and sort it
	*/
	std::cout << " - Sorting frequency values!\n";
	vector<pair<long, long>> sortfreq;
	pair<long, long> temp10;
	//convert array of maps into vector of pairs for sorting purposes
	for (it = leaderMap.begin(); it != leaderMap.end(); ++it)
	{
		temp10.first = it->first;
		temp10.second = it->second;
		sortfreq.push_back(temp10);
	}
	sort(sortfreq.begin(), sortfreq.end(), pairCompare); //sort it
	leaderMap.clear();
	
	std::cout << " - Printing sorted values into list!\n";
	ofstream myfile3;
	myfile3.open(OutFile2);
	std::cout << " - - - - - - - - - - -\n";
	std::cout << " - Top 5 biggest SCCs!\n";
	
	std::cout << " - Node, Size\n";
	std::cout << " - - - - - - -\n";
	for (long i = 0; i < sortfreq.size(); i++)
	{
		if (i < 5)
		{
			std::cout << " " << i + 1 << ": ";
			std::cout << sortfreq[i].first << ", " << sortfreq[i].second << endl;
		}
		myfile3 << sortfreq[i].first << ", " << sortfreq[i].second << endl;
	}
	std::cout << " - - - - - - - - - - -\n";
	myfile3.close();

	//delete dynamic memory
	sortfreq.clear();
	delete T;
	for (long i = 0; i < n + 1; i++)
	{
		GG[i].connectedVertices.clear();
		GG[i].connectedVerticesR.clear();
	}
	delete[] GG;
	delete S3;

	//everything is done now
	std::cout << "*Exiting Kosaraju!\n";
	std::cout << "****************************************\n";
}

int main()
{
	//time the entire thing
	clock_t t3;
	t3 = clock();
	float seconds3;

	//create template for the graph
	const long n = 10000;//875714;//10876;//875714; //9;
	Vertex *G;
	G = new Vertex[n + 1]; //just ignore the 0 tiny waste of memory but so sick of the 0 start
	if (G == nullptr)
	{
		std::cout << "Something is wrong Exiting!\n";
		string USERRESPONSE;
		std::cin >> USERRESPONSE;
		exit(0);
	}

	 //initialize
	for (long i = 0; i < n + 1; i++)
	{
		G[i].ExploredR = false;
		G[i].finish = 0;
		G[i].leader = 0;
	}

	//open file with data
	long mmm = 0;//keep track of rows
	vector<long> temp1; //hold elements of split string to numbers
	long A, B; //A->B edges
	string line;
	string fileNom = "D:/coursera/stanford data struc and algos/4/SCC10000.txt"; //input file n=10																			  //string fileNom = "D:/coursera/stanford data struc and algos/4/SCC.txt"; //input file
	string OutNom = "D:/coursera/stanford data struc and algos/4/leaders.txt"; //outputs vertex:leader
	string OutNom2 = "D:/coursera/stanford data struc and algos/4/topfreq.txt"; //outputs top frequency of leader
	
	ifstream myfile(fileNom);
	clock_t t2;
	t2 = clock();
	float seconds2;

	std::cout << "Reading file! (This can be slow...)" << endl;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			mmm++;
			split(line, temp1);
			A = temp1[0];
			B = temp1[1];
			temp1.clear();
			G[A].connectedVertices.push_back(B);
			G[B].connectedVerticesR.push_back(A);
		}
	temp1.clear();
	myfile.close();
	t2 = clock() - t2;
	seconds2 = ((float)t2) / CLOCKS_PER_SEC;
	std::cout << " - Reading file took " << seconds2 << " seconds!\n";
	std::cout << "Done reading file!\n";
	std::cout << "Number of Edges: " << mmm << endl;
	}

//call kosaraju function that takes Graph, # of vertices, 2 filepaths for output
clock_t t;
float seconds;
t = clock();
kosaraju(G, n, OutNom, OutNom2);
t = clock() - t;
seconds = ((float)t) / CLOCKS_PER_SEC;
std::cout << " - Kosaraju took " << seconds << " seconds!\n";

//------------------------------//
//clear stuff
for (long i = 0; i < n + 1; i++)
{
	G[i].connectedVertices.clear();
	G[i].connectedVerticesR.clear();
}
delete[] G;

t3 = clock() - t3;
seconds3 = ((float)t3) / CLOCKS_PER_SEC;
std::cout << "-----------------------------------------------------\n";
std::cout << "The ENTIRE program took roughly: " << seconds3 << " seconds!\n";
std::cout << "-----------------------------------------------------\n";

//---------------------------------//
system("PAUSE");
}