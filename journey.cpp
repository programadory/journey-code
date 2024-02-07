#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <queue>
#include <ctime>
#include <chrono>
#include "/data/data/com.termux/files/home/headers/termux.h"
#include "/data/data/com.termux/files/home/headers/colors.h"
#define WAIT wait_click();

using namespace std;

/*
Create a journey that are splited by pages. 
There's a file that stores page's keywords based on DDMMYY.
Each time user log in it generates the pages have gone, miss out
formart [PAGE1 - DDMMYY], also generates a new one if uses chose [w] option to create a new Journey_day;

*/

void listPages(vector<string>);
bool checkPath(const string& path, char flag='p');
bool checkPermission(const string&);

tm* setDmy(){
  auto time_now = chrono::system_clock::now();
  time_t time_cast = chrono::system_clock::to_time_t(time_now);
  return localtime(&time_cast);
}

void newPage(vector<string> jpage, tm* today){
  
  //string conc_day_page;
  string current = jpage.back();
  current[6]='\0';
  string new_page;
  char dmyf[6];
  int i=0;
  for(auto ipages: current){
    if(ipages==' '){
     // ipages='\0';
      break;
    }
    dmyf[i] = ipages;
    i++;
    //conc_day_page = conc_day_page+ipages;
  } /* DOESNT WORK, DATA FORMART IS MESSIG UP!!! &*/ //FIX I
  int rec = atoi(current.c_str());
  printf("%s and %s and %d\n", dmyf, current.c_str(), rec);
  //printf("DAY: %d/%d/%d\n", today->tm_mday, today->tm_mon+1, today->tm_year+1900);
  
}

int main(){
  
  /* SETUP UP FILE PATH FIRST TIME OPEN THE APP
  * 1- Checkout directory exists
    2- Checkout journeyquery file exists
    3- Check out file R/W permissions.
    3- if those aren't yet, set the config file (journeyquery);
    4- Read file, copy files to vector<string> then if there's pages, fill all the empty range 
    of note-days within empty pages starting from last note registered.
    
  */
  const char* PATH = "journeypath";
  const char* FILE = "journeypath/journeyquery";
  fstream jfile;
  vector<string> jpages;
  queue<string>jnotes;
  string guser, getj, line, status;
  wchar_t button;
  int rtsys;
  tm* day = setDmy();
  
  try{
    
    jfile.open(FILE, ios::in);
    
    if(checkPath(FILE, 'F') && !checkPermission(FILE))
        throw "Failure trying to acess the journeyquery file! R/W operations fails!!!";
    
    if(!jfile.is_open()){
      if(!checkPath(PATH))
        system("mkdir journeypath > /dev/null");
        
      jfile.close();
      jfile.open(FILE, ios::out);
      ploopstream("No config file founded!", Red,1);
      ploopstream("Setting journeyquery file...", Red, 1);
      ploopstream("Please wait...", Red, 1);
      jfile.close();
    }
  }
  catch(const char* jexcept){
    
    ploopstream(jexcept, Red, 1);
    ploopstream("Fixing it, wait...", Red, 1);
    rtsys = system("$BASH && cd journeypath && chmod +rw journeyquery 2> logs");
    system("exit");
    
    if(rtsys==0){
      ploopstream("Done!", Green, 2);
    }
    else{
      cout << "Error during fixing operations, pls check logs file!!!\n";
      exit(0);
    }
    //exit(EXIT_FAILURE);
  }
  jfile.close();
  jfile.open(FILE, ios::in);
  jfile >> guser; //Getting reference inside journeyquery
  
  if(!jfile.eof()){
    jfile.close();
    jfile.open(FILE, ios::in);
    while(getline(jfile, line)){
      jpages.push_back(line);
    }
 
    newPage(jpages, day);
    
  }
  else{
    status="Write your first note";
  }
  
  WAIT;
  /*****************************************************************END*/
  
  do{
    
    CLEAR;
    cout << linkColor("++++++++++++ WELCOME TO THE JOURNEY APP ++++++++++++\n", Blue);
    cout << linkColor("Mode: ", Red) << status << "\n\n";
    listPages(jpages);
    cout << linkColor("\n\n\n\n\nNew Journey[+] | See [enter] | Delete [-] | Exit [x]", Red) << "\n";
    STTY_ON;
    button = getwchar();
    STTY_OFF;
    
    switch(button){   // enter 10 | + 43 | - 45 | x 120 
      case 43:
            //newPage();
          do{
            CLEAR; // | + save | - discart changes
            printVivid("++++++++++++ Create your first Journey ++++++++++++", Blue);
            cout << "_";
          
          }while(true);
    }
    
  }while(true);
 
  return 0;
}

void listPages(vector<string>qlist){
  for(auto it: qlist)
    cout << linkColor(it.c_str(), Fuchsia) << "\n";
}

bool checkPath(const string& path, char flag){
  string str = (flag=='p') ? "find "+path+" 2> /dev/null" : "find -P "+path;
  return (system(str.c_str()) == 0) ? true : false;
}
//Call checkPath() to check if it exists before continue to check permission executed. ?
bool checkPermission(const string& file){
  //if(checkPath(file))
  string str = "[ -r "+file+" ] && [ -w "+file+" ] && echo 0";
  return (system(str.c_str()) == 0) ? true : false;
}