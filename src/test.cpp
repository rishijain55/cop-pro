
#include <iostream>
#include <string>
using namespace std;




char* datasend(int* arr, int noofdata){
		int* list = new int[100];
		for (int i = 0; i < noofdata; i++)
		{
			/* code */
			list[i] = arr[i];

		}
		string sum = "";
		for (int i = 0; i < noofdata; i++)
		{
			/* code */
			sum += to_string(list[i]);
			sum += "_";
		}
		int len = sum.length();
		char* data = new char[len];

		for (size_t i = 0; i < len; i++)
		{
			/* code */
			data[i] = sum.at(i);
			

		}
	
		return data;
		

}


int* datarecv(char* arr, int noofdata){
	int* receiveddata = new int[noofdata];
	int count = 0;
	int itr = 0;
	string s = "";
	while(count < noofdata){
		if(arr[itr] == '_'){
			receiveddata[count] = std::stoi(s);
			count++;
			s = "";

		}
		else{
			s.push_back(arr[itr]);

		}

		itr++;

	}
			receiveddata[count] = std::stoi(s);
			count++;
			s = "";


	return receiveddata;

}


int main(){
    int* h;
    char* arr = new char[12];
    arr[0]= '0';
    arr[1] ='1';
    arr[2] ='2';
    arr[3] ='_';
    arr[4] = '1';
    arr[5] = '_';
    arr[6] = '2';
    arr[7] = '5';
    arr[8] = '1';
    arr[9] = '_';
    arr[10] = '2';
    h = datarecv(arr,3);
    cout << h[0] << endl;
    cout << h[1] << endl;
    cout << h[2] << endl;
    return 0;
}