//Santhoshkrishnachaitanya Chelikavada
#define _USE_MATH_DEFINES
#include <unordered_map>
#include<iostream>
#include <list>
#include <vector>
#include<map>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <functional>
#include<ctype.h>
#include<cmath>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include<ctime>
#include<time.h>

using namespace std;
const int DEFAULT_VAL = -1;      // must be less than 0

typedef unordered_map<string, list<string>> Graph;        // The graph is given in an adjacency list.
														  // Vertices are indexed from 0 to V-1
														  // The indices of the vertices adjacent to vertex i
														  // are in the list Graph[i].
														  // Graph can be directed or undirected.

class trainStopData {

private:
	string stop_id;
	string stop_name;
	double stop_lat;
	double stop_lon;
public:
	trainStopData(const string& stopidVal)
		:stop_id(stopidVal) {}

	//stopidVal, stopnameVal, stoplatVal, stoplonVal
	trainStopData(const string& stopidVal, const string& stopnameVal, const double& stoplatVal, const double& stoplonVal)
	{
		stop_id = stopidVal;
		stop_name = stopnameVal;
		stop_lat = stoplatVal;
		stop_lon = stoplonVal;



	}

	string get_id() const
	{
		return stop_id;
	}

	string get_stop_name() const
	{
		return stop_name;
	}
	double get_latitude() const
	{
		return stop_lat;
	}
	double get_longitude() const
	{
		return stop_lon;
	}

};
void parseTrainData(vector<trainStopData>& vecData) {
	//insert code here
	ifstream ifs("MTA_train_stop_data.txt");
	if (!ifs) {
		cout << "Could not open the file.\n";
		exit(1);
	}
	string firstLine;
	getline(ifs, firstLine);
	string dataLine;

	while (getline(ifs, dataLine))
	{
		stringstream dataStream(dataLine);
		string junk;
		string stopidVal, stopnameVal, latvalString, lonvalString;
		double stoplatVal, stoplonVal;
		getline(dataStream, stopidVal, ',');
		getline(dataStream, junk, ',');
		getline(dataStream, stopnameVal, ',');
		getline(dataStream, junk, ',');
		getline(dataStream, latvalString, ',');
		getline(dataStream, lonvalString, ',');
		stoplatVal = atof(latvalString.c_str());
		stoplonVal = atof(lonvalString.c_str());



		trainStopData data(stopidVal, stopnameVal, stoplatVal, stoplonVal);
		vecData.push_back(data);

		string extraLines;
		getline(ifs, extraLines);getline(ifs, extraLines); //removes the duplicates - that arise consecutively





	}






}

//This code is from Prof. Hellerstein and Prof. Frankl





double degrad(double d) {
	return d * M_PI / 180;
}


// Code below is adapted from http://www.movable-type.co.uk/scripts/latlong.html
// FYI: That website has an applet that computes the haversine distance. 
// It also has a link that will show the locations on a map,
// with a line between them.
double haverdist(double lat1, double longit1, double lat2, double longit2)
{
	double r = 6371;
	double dlat = degrad(lat2 - lat1);
	double dlongit = degrad(longit2 - longit1);
	double a = sin(dlat / 2)*sin(dlat / 2) +
		cos(degrad(lat1))*cos(degrad(lat2))*sin(dlongit / 2)*sin(dlongit / 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
	return r*c;
}


class isStopOnRoute
{
private:
	char route;
public:
	isStopOnRoute(const char& value) :route(value) {}
	bool operator() (const trainStopData& data) const
	{
		return ((data.get_id())[0] == route);
	}
};

class isSubwayStop
{
private:
	string stopId;
public:
	isSubwayStop(const string& value) :stopId(value) {}
	bool operator() (const trainStopData& data) const
	{
		return (data.get_id() == stopId);
	}
};

class isSubwayStopNearX
{
private:
	double longitude, latitude, d;
public:
	isSubwayStopNearX(double one, double two, double three) :longitude(one)
		, latitude(two), d(three) {}
	bool operator() (const trainStopData& data) const
	{
		return (haverdist(data.get_latitude(), data.get_longitude(), latitude, longitude) <= d);
	}
};

class printTrainStopInfo
{

public:
	void operator() (const trainStopData& data)const
	{
		cout << "STOP ID: " << data.get_id() << endl;
		cout << "STOP NAME: " << data.get_stop_name() << endl;
		cout << "STOP LATITUDE: " << data.get_latitude() << endl;
		cout << "STOP LONGITUDE: " << data.get_longitude() << endl;

	}


};

template<class Iterator, class Functor, class FunctorTwo>
int perform_if(Iterator itrStart, Iterator itrEnd, Functor pred, FunctorTwo op)
{
	int how_many = 0;
	Iterator itr = itrStart;
	while (itr != itrEnd)
	{
		if (pred(*itr))
		{
			op(*itr);
			how_many++;

		}

		itr++;


	}
	if (how_many == 0)
	{
		cout << "NO INFORMATION OBTAINED" << endl;
	}
	return how_many;




}

bool notFoundInMap(string s, Graph graph)
{
	return ((graph.find(s)) == graph.end());
}


void MTADataInsert(vector<trainStopData>& vecData)
{
	ifstream ifs("MTA_train_stop_data.txt");
	if (!ifs) {
		cout << "Could not open the MTA file.\n";
		exit(1);
	}
	string firstLine;
	getline(ifs, firstLine);
	string dataLine;

	while (getline(ifs, dataLine))
	{
		stringstream dataStream(dataLine);
		string junk;
		string stopidVal;

		getline(dataStream, stopidVal, ',');
		trainStopData data(stopidVal);
		vecData.push_back(data);
		string extraLines;
		getline(ifs, extraLines);getline(ifs, extraLines); //removes the duplicates - that arise consecutively


	}

	ifs.close();
	ifs.clear();

}

//20

struct LineAndTime
{
	string Line;
	string Time;



};


void populateLineAndTimeHashMap(map<string, map<char, vector<string>>>& mapVal)
{	
	//populated with first 2000 to avoid long compilation times.
	int totalLines = 2000;
	ifstream ifs("stop_times.txt");
	if (!ifs) {
		cout << "Could not open the MTA file.\n";
		exit(1);
	}
	string junk;
	getline(ifs, junk);
	string line;
	int nLines = 0;
	while (getline(ifs, line) && nLines < totalLines)
	{
		stringstream dataStream(line);
		string railLine;
		getline(dataStream, railLine, ',');
		char Line = railLine[26];

		string arrivalTime;
		getline(dataStream, arrivalTime, ',');
		string junk;
		getline(dataStream, junk, ',');
		string stopidVal;
		getline(dataStream, stopidVal, ',');


		cout << Line << "	" << arrivalTime << "	" << stopidVal << endl;
		mapVal[stopidVal][Line].push_back(arrivalTime);
		nLines++;

	}
}

void displayTimes(string stopIDVal, map<string,map<char,vector<string>>>& findMap, char lineVal)
{	

	map<string, map<char, vector<string>>>::const_iterator itr = findMap.find(stopIDVal);

	if (itr != findMap.end())
	{

		cout << itr->first << endl;
		map<char, vector<string>> secondMap = itr->second;

		vector<string> printTimes;

		if (secondMap.find(lineVal) != secondMap.end())
		{
			printTimes = secondMap.find(lineVal)->second;


			cout << "FOR: " << stopIDVal << endl;
			cout << "FOR LINE: " << lineVal << endl;
			cout << "LIST OF TIMES: " << endl;
			for (int counter = 0;counter < printTimes.size();counter++)
			{
				cout << printTimes[counter] << endl;



			}
		}
		else
		{
			cout << "WRONG INPUT" << endl;
		}





	}
	else
	{
		cout << "Wrong Input" << endl;
	}

	

}





void insertTransfers(unordered_map < string, list<string>>& putMap)
{
	ifstream ifs("transfers.txt");
	if (!ifs)
	{
		cerr << "Could Not Open the Transfers text file" << endl;
		exit(1);
	}
	string firstLine;
	getline(ifs, firstLine);
	string dataLine;
	while (getline(ifs, dataLine))
	{
		stringstream dataStream(dataLine);
		string fromID, toID;
		getline(dataStream, fromID, ',');
		getline(dataStream, toID, ',');
		if (fromID != toID)
		{
			list<string> existingList = putMap[fromID];
			existingList.push_back(toID);
			putMap[fromID] = existingList;
		}

	}
	ifs.close();
	ifs.clear();


}

// Code obtained from HW 10
void createAdjacencyList(unordered_map<string, list<string>>& putMap) {
	//insert code here

	vector<trainStopData> vecData;
	MTADataInsert(vecData);
	//For the first stop;
	list<string> tempList;
	if (vecData[0].get_id()[0] == vecData[1].get_id()[0])
	{
		tempList.push_front(vecData[1].get_id());
		putMap[vecData[0].get_id()] = tempList;
	}

	tempList.clear();

	for (int counter = 1;counter < vecData.size() - 1;counter++)
	{

		if ((vecData[counter]).get_id()[0] == vecData[counter + 1].get_id()[0])
		{
			tempList.push_front(vecData[counter + 1].get_id());
		}
		if ((vecData[counter]).get_id()[0] == vecData[counter - 1].get_id()[0])
		{
			tempList.push_front(vecData[counter - 1].get_id());
		}
		putMap[vecData[counter].get_id()] = tempList;
		tempList.clear();

	}

	if (vecData[vecData.size() - 1].get_id()[0] == vecData[vecData.size() - 2].get_id()[0])
	{
		tempList.push_front(vecData[vecData.size() - 2].get_id());
		putMap[vecData[vecData.size() - 1].get_id()] = tempList;
		tempList.clear();
	}


	insertTransfers(putMap);

	

}




struct vertexInf                    // Stores information for a vertex
{
	int dist;  // distance to vertex from the source
	string prev;  // previous node in BFS tree
};

void printpath(const vector<trainStopData>& data, unordered_map<string, vertexInf>& vinfo, string sourceID, string endID, bool printDetails)
{
	if (printDetails == true)
	{
		stack<vertexInf> t;
		int length = 0;
		vertexInf current = vinfo.find(endID)->second;
		while (current.dist != 0)
		{
			t.push(current);
			current = vinfo.find(current.prev)->second;
		}

		cout << endl << endl << "PATH FROM " << sourceID << " TO " << endID << " IS AS FOLLOWS: ";

		while (!t.empty())
		{
			cout << t.top().prev << " - " << endl;

			perform_if(data.begin(), data.end(), isSubwayStop(t.top().prev), printTrainStopInfo());



			t.pop();
			length++;
		}
		cout << endID;
		cout << endl << "LENGTH: " << length << endl;

		cout << endl << endl;

	}
}

void shortestpaths(const vector<trainStopData>& data, const Graph & g, string sourceStopID, string endID, bool printDetails)
{
	queue<string> q;             // q is the queue of vertex numbers

	if (notFoundInMap(sourceStopID, g) || notFoundInMap(endID, g))
	{
		cout << "Bad index" << endl;
		return;
	}

	unordered_map<string, vertexInf> vertices;               // stores BFS info for the vertices
	for (Graph::const_iterator itr = g.begin(); itr != g.end();itr++)
	{
		vertices[itr->first].dist = DEFAULT_VAL;
		vertices[itr->first].prev = DEFAULT_VAL;
	}


	// info for vertex j is in position j

	vertices[sourceStopID].dist = 0;

	q.push(sourceStopID);
	while (!q.empty())
	{
		string v = q.front();
		q.pop();



		for (list<string>::const_iterator w = g.find(v)->second.begin(); w != g.find(v)->second.end(); w++)
		{

			if (vertices[*w].dist == DEFAULT_VAL)          // distance of *w from source not determined yet
			{
				vertices[*w].dist = vertices[v].dist + 1;
				vertices[*w].prev = v;
				q.push(*w);
			}
		}
	}

	printpath(data,vertices, sourceStopID, endID,true);


	/*
	for (int j = 0; j < vertices.size(); j++)        // print distances from source and paths
	{
	cout << "vertex " << j << endl;
	cout << "distance: " << vertices[j].dist << endl;
	cout << "shortest path: ";
	printpath(j, vertices);
	cout << endl;
	}
	*/
}



void printpath(unordered_map<string, vertexInf>& vinfo, string sourceID, string endID)
{
	stack<vertexInf> t;
	int length = 0;
	vertexInf current = vinfo.find(endID)->second;
	while (current.dist != 0)
	{
		t.push(current);
		current = vinfo.find(current.prev)->second;
	}

	cout << endl << endl << "PATH FROM " << sourceID << " TO " << endID << " IS AS FOLLOWS: ";

	while (!t.empty())
	{
		cout << t.top().prev << " - ";
		t.pop();
		length++;
	}
	cout << endID;
	cout << endl << "LENGTH: " << length << endl;

	cout << endl << endl;
}








// Breadth First Search
// The unweighted shortest path algorithm on the graph g, with vertex i as the source
// Prints the length (number of edges) of the shortest path from the source to every vertex
// in the graph

void shortestpaths(const Graph & g, string sourceStopID, string endID)
{
	queue<string> q;             // q is the queue of vertex numbers

	if (notFoundInMap(sourceStopID, g) || notFoundInMap(endID, g))
	{
		cout << "Bad index" << endl;
		return;
	}

	unordered_map<string, vertexInf> vertices;               // stores BFS info for the vertices
	for (Graph::const_iterator itr = g.begin(); itr != g.end();itr++)
	{
		vertices[itr->first].dist = DEFAULT_VAL;
		vertices[itr->first].prev = DEFAULT_VAL;
	}


	// info for vertex j is in position j

	vertices[sourceStopID].dist = 0;

	q.push(sourceStopID);
	while (!q.empty())
	{
		string v = q.front();
		q.pop();



		for (list<string>::const_iterator w = g.find(v)->second.begin(); w != g.find(v)->second.end(); w++)
		{

			if (vertices[*w].dist == DEFAULT_VAL)          // distance of *w from source not determined yet
			{
				vertices[*w].dist = vertices[v].dist + 1;
				vertices[*w].prev = v;
				q.push(*w);
			}
		}
	}

	printpath(vertices, sourceStopID, endID);


	/*
	for (int j = 0; j < vertices.size(); j++)        // print distances from source and paths
	{
	cout << "vertex " << j << endl;
	cout << "distance: " << vertices[j].dist << endl;
	cout << "shortest path: ";
	printpath(j, vertices);
	cout << endl;
	}
	*/
}

void displayGraph(unordered_map<string, list<string>>& adjList)
{
	for (unordered_map<string, list<string>>::iterator itr = adjList.begin();itr != adjList.end();itr++)
	{
		cout << itr->first << ": ";
		for (list<string>::iterator itr2 = (itr->second).begin();itr2 != (itr->second).end();itr2++)
		{
			cout << "-" << *itr2;

		}

		cout << endl;
	}
}


void nextArriving(map<string, map<char, vector<string>>>& hashMap)
{
	cout << "ENTER THE STOP ID OF YOUR CLOSEST STATION: " << endl;
	
	string stopID;
	cin >> stopID;

	cout << "ENTER THE LINE OF THE TRAIN: " << endl;
	char lineVal;
	cin >> lineVal;

	
	vector<string> times = hashMap[stopID][lineVal];
	cout << "ENTER TIME NOW: " << endl;
	cout << "HOURS: ";
	double hrsVal,mins;
	cin >> hrsVal;
	cout << "MINS: ";
	cin >> mins;

	for (int counter = 0;counter < times.size();counter++)
	{	
		string timeCount = times[counter];
		char Val = timeCount[1];
		char* pointerVal = &Val;
		if (hrsVal < atof(pointerVal))
		{	
			
			cout << "NEXT ARRIVAL OF YOUR TRAIN: " << endl;
			cout << timeCount << endl;
			break;
		}


	}
	
	
}



void menu(const vector<trainStopData>& data)
{
	int option = -1;
	bool continueVal = true;
	while (continueVal)
	{
		cout << "CHOOSE FROM THE FOLLOWING OPTIONS: " << endl;
		cout << "1. Print out the information about all the train stops on a specific route." << endl;
		cout << "2. Print out the information about a specific train stop." << endl;
		cout << "3.Print out all the train stops within a certain distance." << endl;
		cout << "4. Display the shortest length between two stops." << endl;
		cout << "5. Display an adjacency list representing the links between each station. " << endl;
		cout << "6. Get arrival times for a stop ID and a Line" << endl;
		cout << "7. Print info of all stations in your path" << endl;
		cout << "8. Find the next arriving train." << endl;
		cout << "10. QUIT" << endl;
		cout << "Enter Option: ";
		cin >> option;
		if (option == 10)
		{
			break;
		}
		switch (option)
		{
		case 1:
		{
			char routeVal;
			cout << "ENTER ROUTE: ";
			cin >> routeVal;
			perform_if(data.begin(), data.end(), isStopOnRoute(routeVal), printTrainStopInfo());
			break;

		}
		case 2:
		{
			string stopID;
			cout << "Enter Stop ID: ";
			cin >> stopID;
			perform_if(data.begin(), data.end(), isSubwayStop(stopID), printTrainStopInfo());
			break;

		}
		case 3:
		{
			double lat, lon, dist;
			cout << "ENTER LATITUDE AND LONGITUDE AND DISTANCE: ";
			cin >> lat >> lon >> dist;
			perform_if(data.begin(), data.end(), isSubwayStopNearX(lat, lon, dist), printTrainStopInfo());
			break;
		}
		case 4:
		{

			unordered_map < string, list<string>> adjList;
			createAdjacencyList(adjList);
			
			cout << "ENTER THE SOURCE ID: ";
			string one, two;
			cin >> one;
			cout << endl << "ENTER THE END ID: ";
			cin >> two;
			cout << endl;


			shortestpaths(adjList, one, two);

		}

		case 5:
		{
			unordered_map < string, list<string>> adjList;
			createAdjacencyList(adjList);
			displayGraph(adjList);
		}

		case 6:
		{
			map<string, map<char, vector<string>>> timeMap;
			populateLineAndTimeHashMap(timeMap);
			string stopid;
			char line;
			cout << "ENTER STOP ID: ";
			cin >> stopid;
			cout << "Enter Line";
			cin >> line;
			displayTimes(stopid, timeMap, line);
			break;

		}

		case 7:
		{

			unordered_map < string, list<string>> adjList;
			createAdjacencyList(adjList);

			cout << "ENTER THE SOURCE ID: ";
			string one, two;
			cin >> one;
			cout << endl << "ENTER THE END ID: ";
			cin >> two;
			cout << endl;


			shortestpaths(data,adjList, one, two,true);
			break;

		}
		case 8:
		{


			map<string, map<char, vector<string>>> timeMap;
			populateLineAndTimeHashMap(timeMap);
			
			nextArriving(timeMap);
			break;
		}
		

		case 10:

		{
			exit(0);
		}

		}

		cout << endl << "DO YOU WISH TO CONTINUE? (Enter 1 or a 0) " << endl;
		cin >> continueVal;




	}





}





int main()
{
	

	vector<trainStopData> data;
	parseTrainData(data);
	

	menu(data);
	






}


