#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <queue>
#include <ctime>
#include <chrono>
/* C libraries */
#include "/data/data/com.termux/files/home/headers/termux.h"
#include "/data/data/com.termux/files/home/headers/colors.h"
#include "/data/data/com.termux/files/home/headers/texteditor.h"

using namespace std;

/*
Create a journey that are splited by pages. 
There's a file that stores page's keywords based on DDMMYY.
Each time user login it generates the pages have gone, miss out
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

string dayformarter(int* dmy){
  string dfs; //day formated string
  dfs = (dmy[0]<10) ? "0"+to_string(dmy[0])+"/" : to_string(dmy[0])+"/";
  dfs = (dmy[1]<10) ? dfs+"0"+to_string(dmy[1])+"/" : dfs+to_string(dmy[1])+"/";
  dfs = dfs+to_string(dmy[2]);
  
  return dfs;
}

string newPage(vector<string>* jpage, tm* today){
  
  int cmpday[3];
  string stream;
  
  try{ 
    
    cout << jpage->at(0); //Catching empty exception
    stream = jpage->back();
    stream[2] = '\0'; //Delimite a new finish to be treated for stoi()
    //////
    cmpday[0]=today->tm_mday;
    cmpday[1]=stoi(stream);
    cmpday[2]=cmpday[0]-cmpday[1]; //getting missing days
    // LOGQ(stream);
    // LOGQ(to_string(cmpday[1]));
    int daynow[3] = {today->tm_mday, today->tm_mon+1, today->tm_year-100};
    int dayc=jpage->size();
    LOGQ(to_string(dayc));
  
    /*1- if the day matches, pass
    * 2- if doesn't match for 1 day longer, pass 
    * 3- if doesnt't match for 2 days or longer, NO 
    */
    //When there's days missing out
    if(cmpday[2] > 1){
      //check for missing days 
      while(++cmpday[1]<=cmpday[0]){
        daynow[0]=cmpday[1]; //increasing days
        stream  = dayformarter(daynow); //Converting to dd/mm/yy formart
        stream = stream+" - page"+to_string(++dayc); //formarting again before push
        jpage->push_back(stream); //pushing back to collection
        //printf("DATE: %s\n", stream.c_str());
        LOGQ(stream);
      }
    }//when its a new journey day
    else{ //Saving memory func call
      stream  = dayformarter(daynow);
      stream = stream+" - page"+to_string(++dayc); 
      jpage->push_back(stream);
      LOGQ(stream);
      //printf("DATE: %s\n", stream.c_str());
    }
   //ALGORITHM IS WORKING FINE!
    stream = "page"+to_string(dayc); //Reformat to create the real journey page.
    return stream;
  }
  catch(const out_of_range& jexcept){ //Threating a jpages empty situation
  
    cmpday[0]=today->tm_mday;
    cmpday[1]=today->tm_mon+1;
    cmpday[2]=today->tm_year-100;
    stream = dayformarter(cmpday);
    stream = stream+" - page1";
    jpage->push_back(stream);
    stream = "page1"; 
    return stream;
  }
}

string timenow(tm* timelocal_){
  char buffer[7];
  strftime(buffer, 7, "%h%m%s", timelocal_);
  return string(buffer);
}

void cli(string, Colors, Colors, string, wchar_t*, string);

int main(){
  
  /* SETUP UP FILE PATH FIRST TIME OPEN THE APP
  * 1- Checkout directory exists
    2- Checkout journeyquery file exists
    3- Check out file R/W permissions.
    3- if there's no file, set the config file (journeyquery);
    4- Read file, copy files to vector<string> then if there's pages, fill all the empty range 
    of note-days within empty pages starting from last note registered.
  */
  
  const char* PATH = "journeypath";
  const char* FILE = "journeypath/journeyquery";
  fstream jfile, jthis;
  vector<string> jpages;
  queue<string>jnotes;
  string guser, getj, line, status;
  wchar_t button, ibutton;
  tm* day = setDmy();
  int rtsys;
  
  try{
    
    jfile.open(FILE, ios::in);
    
    if(checkPath(FILE, 'F') && !checkPermission(FILE))
        throw "Failure trying to acess the journeyquery file! R/W operations fails!!!";
    
    if(!jfile.is_open()){
      if(!checkPath(PATH))
        system("mkdir journeypath > /dev/null");
        
      jfile.open(FILE, ios::out);
      ploopstream("No config file founded!", Red,1);
      ploopstream("Setting journeyquery file...", Red, 1);
      ploopstream("Please wait...", Red, 1);
    }
    jfile.close(); //Closing file general conditions.
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
  
  jfile.open(FILE, ios::in);
  jfile >> guser; //Getting first line to check .eof()
  LOG &guser;
  
  if(!jfile.eof()){
    jfile.close();
    jfile.open(FILE, ios::in);
    while(getline(jfile, line)){
      jpages.push_back(line);
    }
    jfile.close();
  }
  else{
    status="Write u first note";
  }
  
  /*****************************************************************END*/
  
  Editor jeditor = Editor();
  char letter;
  string setbar = "\n\n\n\n\n\n\n\n\n\n\n\n\n[`] exit | [/] left | [*] right | [-] delete\n";
  
  do{
    
    CLEAR;
    cout << linkColor("++++++++++++ WELCOME TO THE JOURNEY APP ++++++++++++\n", SZ_DEFAULT, Blue);
    cout << linkColor("Mode: ", SZ_DEFAULT, Red) << status << "\n\n";
    listPages(jpages);
    cout << linkColor("\n\n\n\n\nNew Journey[+] | See [enter] | Delete [-] | Exit [x]", SZ_DEFAULT, Red) << "\n";
    STTY_ON;
    button = getwchar();
    STTY_OFF;
    
    if(button==120)
      break;
    
    switch(button) // enter 10 | + 43 | - 45 | x 120 | *42 | / 47
    {  
      case 43:
            guser = newPage(&jpages, day);
            jthis.open(guser, ios::out | ios::app);
            status="Editing "+guser;
            
            while(ibutton!=96){ // button = ` - 96
    
              system("clear");
              //Use threads here to make the CLI static!
              cli(status, Red, Red, setbar, &ibutton, jeditor.stream);
              letter = ibutton;
              
              switch(ibutton)
              {
                case 47: // button = / left
                      if(jeditor.cursor_it>0)
                        jeditor.cursorLeft(jeditor.stream, jeditor.cursor, &jeditor.cursor_it);
                      break;
                case 42: // button = * right
                      if(jeditor.cursor_it<jeditor.size)
                        jeditor.cursorRight(jeditor.stream, jeditor.cursor, &jeditor.cursor_it);
                      break;
                case 45: // button = - delete
                      if(jeditor.cursor_it>0){
                        jeditor.delStream(jeditor.stream, jeditor.cursor, &jeditor.cursor_it, jeditor.size, true);
                        --jeditor.size;
                      }
                      break;
                case 96: // button = ` exit                                     //false = delete cursor.
                      jeditor.delStream(jeditor.stream, jeditor.cursor, &jeditor.cursor_it, jeditor.size, false);
                      break;
                      
                default:
                      if(jeditor.cursor_it <= jeditor.size)
                        jeditor.editStream(jeditor.stream, jeditor.cursor, letter, &jeditor.cursor_it, jeditor.size);
                      ////
                      ++jeditor.size;

              }
            }
            break;
            
    }
 
  }while(true);
  
  cout << "OUTPUT:\n\n" << jeditor.stream << "\n---------------------------------------------\n";
  
  return 0;
}

void listPages(vector<string>qlist){
  for(auto it: qlist)
    cout << it << "\n";
}

bool checkPath(const string& path, char flag){
  string str = (flag=='p') ? "find "+path+" 2> /dev/null" : "find -P "+path+" > /dev/null";
  return (system(str.c_str()) == 0) ? true : false;
}
//Call checkPath() to check if it exists before continue to check permission executed. ?
bool checkPermission(const string& file){
  //if(checkPath(file))
  string str = "[ -r "+file+" ] && [ -w "+file+" ] && echo 0 > /dev/null";
  return (system(str.c_str()) == 0) ? true : false;
}

void cli(string status, Colors begin_color, Colors end_color, string comand_bar, wchar_t* bt, string data_stream){
    
    cout << linkColor("++++++++++++ WELCOME TO THE JOURNEY APP ++++++++++++\n", SZ_DEFAULT, begin_color);
    cout << linkColor("Mode: ", SZ_DEFAULT, Red) << status << "\n\n";
    cout << data_stream;
    cout << linkColor(comand_bar.c_str(), comand_bar.size(), end_color);
    STTY_ON;
    *bt = getchar();
    STTY_OFF;
}