/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: Mon 09 May 2016 09:37:35 AM EDT
 ************************************************************************/

#include<iostream>
#include <string>
#include <assert.h>    
#include "leveldb/db.h"   
using namespace std;

int main(void) 
{       

    leveldb::DB      *db;    
    leveldb::Options  options;    
    options.create_if_missing = true;    

    // open
    leveldb::Status status = leveldb::DB::Open(options,"/tmp/testdb", &db);    
    assert(status.ok());    
    //
    string key = "name";    
    string value = "chenqi";    
    
    // write
    status = db->Put(leveldb::WriteOptions(), key, value);    
    assert(status.ok());    
    
    // read
    status = db->Get(leveldb::ReadOptions(), key, &value);    
    assert(status.ok());    
    //
    cout<<value<<endl;    
    
    // delete
    status = db->Delete(leveldb::WriteOptions(), key);    
    assert(status.ok());        
    //
    status = db->Get(leveldb::ReadOptions(),key, &value);    
    if(!status.ok()) {
        cerr<<key<<"    "<<status.ToString()<<endl;
    } else {
        cout<<key<<"==="<<value<<endl;    
    }   
    //
    // close 
    delete db;    
    //
    return 0;    
    }
    //                                                                             }
    //                                          }}

