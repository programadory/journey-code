#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <stack>
#include <ctime>
#include <chrono>
/* C libraries */
#include "/data/data/com.termux/files/home/headers/termux.h"
#include "/data/data/com.termux/files/home/headers/colors.h"
#include "/data/data/com.termux/files/home/headers/texteditor.h"

using namespace std;

// myLog log_ = myLog("logs");
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
    
    cout << jpage->at(0); //Catching empty vector exception, if there's no pages just throw it.
    stream = jpage->back();
    stream[2] = '\0'; //Delimite a new finish to be treated for stoi()
    //////
    cmpday[0]=today->tm_mday;
    cmpday[1]=stoi(stream);
    cmpday[2]=cmpday[0]-cmpday[1]; //getting missing days
  
    int daynow[3] = {today->tm_mday, today->tm_mon+1, today->tm_year-100};
    int index=jpage->size();
  
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
        stream = stream+" - page"+to_string(++index); //formarting again before push
        jpage->push_back(stream); //pushing back to collection
        //printf("DATE: %s\n", stream.c_str());
      }
    }//when its a new journey day
    else{ //Saving memory func call
      stream  = dayformarter(daynow);
      stream = stream+" - page"+to_string(++index); 
      jpage->push_back(stream);
    }
   //ALGORITHM IS WORKING FINE!
    stream = "page"+to_string(index); //Reformat to create the real journey page.
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

bool is_pagelimit(string lastpage, string currentdmy_){
  lastpage[8] = '\0';
  const char* lpagedmy = lastpage.c_str();
  return (lpagedmy == currentdmy_) ? true : false; 
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
  const char* PPATH = "journeypath/pages/";
  const char* FILE = "journeypath/journeyquery";
  
  fstream jfile, jthis;
  vector<string> jpages;
  string guser, lastpage, line, text, status;
  wchar_t button, ibutton;
  tm* day = setDmy();
  int dmy[3]={day->tm_mday, day->tm_mon+1, day->tm_year-100};
  int rtsys;
  
  try{
    
    jfile.open(FILE, ios::in);
    
    if(checkPath(FILE, 'F') && !checkPermission(FILE))
        throw "Failure trying to acess the journeyquery file! R/W operations fails!!!";
    
    if(!jfile.is_open()){
      if(!checkPath(PATH))
        system("mkdir journeypath > /dev/null");
 
      if(checkPath(PATH) && !checkPath(PPATH))
        system("mkdir journeypath/pages > /dev/null");
        
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
  }
  
  jfile.open(FILE, ios::in);
  if(jfile >> line){
    jfile.close();
    jfile.open(FILE, ios::in);
    while(getline(jfile, line)){
      jpages.push_back(line);
    }
  }
  else{
    status="Write u first note";
  }
  jfile.close();
  /*****************************************************************END*/
  
  char letter;
  string setbar = "\n\n\n\n\n\n\n\n\n\n\n\n\n[`] exit | [/] left | [*] right | [-] delete\n";
  string message = "Page limit Reached, come tomorrow :3";
  
  do{
    
    CLEAR;
    cout << linkColor("++++++++++++ WELCOME TO THE JOURNEY APP ++++++++++++\n", SZ_DEFAULT, Blue);
    cout << linkColor("Mode: ", SZ_DEFAULT, Red) << status << "\n\n";
    listPages(jpages);
    cout << linkColor("\n\n\n\n\nNew Journey[+] | See/Edit [enter] | Delete [-] | Exit [x]", SZ_DEFAULT, Red) << "\n";
    STTY_ON;
    button = getwchar();
    STTY_OFF;
    
    if(button==120)
      break;
    
    switch(button) // enter 10 | + 43 | - 45 | x 120 | *42 | / 47
    {  
      case 43: /* Basically it has to check if the user has alredy made a journal today*/
            // Check by day if the user alredy wrote a journal page!
           {
            if(!jpages.empty() && is_pagelimit(jpages.back(), dayformarter(dmy))){
                status = linkColor(message.c_str(), message.size(), Fuchsia);
                break;
            }
            
            Editor* jeditor = new Editor();
            guser = newPage(&jpages, day);
            status="Editing "+guser;
            lastpage=jpages.back();
            
            while(true){ // button = ` - 96
    
              //Use threads here to make the CLI static!
              cli(status, Red, Red, setbar, &ibutton, jeditor->stream);
              letter = ibutton;
              
              switch(ibutton)
              {
                case 47: // button = / left
                      if(jeditor->cursor_it>0)
                        jeditor->cursorLeft(jeditor->stream, jeditor->cursor, &jeditor->cursor_it);
                      break;
                case 42: // button = * right
                      if(jeditor->cursor_it<jeditor->size)
                        jeditor->cursorRight(jeditor->stream, jeditor->cursor, &jeditor->cursor_it);
                      break;
                case 45: // button = - delete
                      if(jeditor->cursor_it>0){
                        jeditor->delStream(jeditor->stream, jeditor->cursor, &jeditor->cursor_it, jeditor->size, true);
                        --jeditor->size;
                      }
                      break;
                case 96: // button = ` exit & save - 126                                    //false = delete cursor.
                      jeditor->delStream(jeditor->stream, jeditor->cursor, &jeditor->cursor_it, jeditor->size, false);
                      
                      guser = PPATH+guser;
                      jthis.open(guser, ios::out); //pages block
                      jthis << jeditor->stream << endl;
                      jthis.close();
                      
                      jfile.open(FILE, ios::out); //journeyquery block
                      for(auto it: jpages)
                         jfile << it << endl;
                      jfile.close();
                      //
                      status="Saved!";
                      status=linkColor(status.c_str(), status.size(), Fuchsia);
                      break;
                      
                case 126: // ~ exit & discart
                      jpages.pop_back();
                      status = guser+" discarted!";
                      break;
                
                default:
                      if(jeditor->cursor_it <= jeditor->size)
                        jeditor->editStream(jeditor->stream, jeditor->cursor, letter, &jeditor->cursor_it, jeditor->size);
                      //////
                      ++jeditor->size;

              }
              if(ibutton==96 || ibutton==126) break;
            }
            ibutton='\0';
            delete jeditor;
            break;
          }
      
      case 10: // Enter - Read a journal page
            if(jpages.empty()){
              status="No pages created!";
              break;
            }
            
            status = linkColor("View",3, Red);
            cout << linkColor("Search: ",8,Red);
            getline(cin, guser);
            
            for(auto it: jpages){
              
              auto fpage = it.find(guser);
              if(fpage != std::string::npos){
                size_t begin = it.rfind(" ", fpage)+1, end = it.find(" ", fpage);
                guser = line = it.substr(begin, end-begin);
                break;
              }
              else{
                line = "\0";
              }
            }
            
            if(guser != line){
              status = guser+" not found!";
              break;
            } 
            
            text = "\0";
    
            jfile.open(PPATH+line, ios::in);
            while(getline(jfile, line)){
              text += line+"\n";
            }
            jfile.close();
           
            cli(status, Green, Green, "\n\n\n\n\n\n\n\n\n\n\n\n\n[+] Edit | [Enter] Exit\n", &ibutton, text);
              
            if(ibutton==43){
              Editor* jeditor = new Editor();
              jeditor->loadStream(text.c_str());
           
              while(true){
                
                cli(status, Red, Red, setbar, &ibutton, jeditor->stream);
                letter = ibutton;
                
                switch(ibutton)
                {
                  case 47: // button = / left
                        if(jeditor->cursor_it>0)
                          jeditor->cursorLeft(jeditor->stream, jeditor->cursor, &jeditor->cursor_it);
                        break;
                  case 42: // button = * right
                        if(jeditor->cursor_it<jeditor->size)
                          jeditor->cursorRight(jeditor->stream, jeditor->cursor, &jeditor->cursor_it);
                        break;
                  case 45: // button = - delete
                        if(jeditor->cursor_it>0){
                          jeditor->delStream(jeditor->stream, jeditor->cursor, &jeditor->cursor_it, jeditor->size, true);
                          --jeditor->size;
                        }
                        break;
                  case 96: // button = ` exit & save - 126                                 
                        jeditor->delStream(jeditor->stream, jeditor->cursor, &jeditor->cursor_it, jeditor->size, false);
                        
                        guser = PPATH+guser;
                        jthis.open(guser, ios::out); //pages block
                        jthis << jeditor->stream << endl;
                        jthis.close();
                        
                        jfile.open(FILE, ios::out); //journeyquery block
                        for(auto it: jpages)
                           jfile << it << endl;
                        jfile.close();
                        //
                        status="Edited!";
                        status=linkColor(status.c_str(), status.size(), Fuchsia);
                        break;
                        
                  case 126: // ~ exit & discart
                        jpages.pop_back();
                        status = guser+" discarted!";
                        break;
                  
                  default:
                        if(jeditor->cursor_it <= jeditor->size)
                          jeditor->editStream(jeditor->stream, jeditor->cursor, letter, &jeditor->cursor_it, jeditor->size);
                        //////
                        ++jeditor->size;
  
                }
              if(ibutton==96 || ibutton==126) break;
              }
            ibutton='\0';
            delete jeditor;
            break;
            }
            break;
            
      case 45:
            
            cout << linkColor("Search: ",8, Red);
            getline(cin, guser);
            string delpage;
            
            jfile.open(FILE, ios::out);
            for(auto it: jpages){
              auto fpage = it.find(guser);
              if(fpage != std::string::npos && guser != "\0"){ 
                //Deleting from file /pages/
                size_t begin = it.rfind(" ", fpage)+1, end = it.find(" ", fpage);
                status = delpage = it.substr(begin, end-begin);
                delpage  = "rm journeypath/pages/"+delpage+" > /dev/null";
                system(delpage.c_str());
                //Update list and removing from journeyquery
                jpages.erase(remove(jpages.begin(), jpages.end(), it));
                status += " deleted!";
                break;
              }
              else{
                jfile << it << endl;
                status = "Not found!";
              }
            }
            jfile.close();
            break;
       
    }
 
  }while(true);
  
  // log_.exitlog();
  
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

void cli(string status, Colors begin_color, Colors end_color, string comand_bar, wchar_t* button, string data_stream){
    
    CLEAR;
    cout << linkColor("++++++++++++ WELCOME TO THE JOURNEY APP ++++++++++++\n", SZ_DEFAULT, begin_color);
    cout << linkColor("Mode: ", SZ_DEFAULT, Red) << status << "\n\n";
    cout << data_stream;
    cout << linkColor(comand_bar.c_str(), comand_bar.size(), end_color);
    STTY_ON;
    *button = getchar();
    STTY_OFF;
}