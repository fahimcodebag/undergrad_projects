#include <iostream>
#include<fstream>
#include<iomanip>
#include<string>
#include<vector>

using namespace std;
 
int main(){

ifstream dictionary;
string puzzle = ""; 
string word = "", test_word="";

int position = 0;
bool found = false;
vector <string> output{};
dictionary.open("C:/Users/fahim/Downloads/dictionary.txt");

if(!dictionary)
{
    cout<<"unable to open dictionary"<<endl;
    system("pause");
    return 1;
}
cout<<"Dictionary File is Open"<<endl;

cout<<"Enter The Puzzle"<<endl;
cin>>puzzle;

position = 0;
int n=3;
while(n<=puzzle.length()){
while(position<=puzzle.length()-n)
{
    test_word = puzzle.substr(position,n);
    
    while(!dictionary.eof())
    {
        
        dictionary>>word;
        if(word==test_word)
        { 
          
          output.push_back(word);
        
          found = true;
        
          break;
        }
        
    }
    dictionary.clear();

    dictionary.seekg(0,ios::beg);
    position++;
    found = false;
     
}
n++;
position=0;

}
cout<<"the found words are"<<endl;

for(int j=0;j<output.size();j++)
{
    cout<<output[j];
    if(j<output.size()-1)cout<<" , ";
}


dictionary.close();
return 0;
}++