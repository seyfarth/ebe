#include <iostream>
#include <string>
#include <algorithm>
#include "symbols.h"
#include "parse.h"

using namespace std;

struct compare {
    bool operator () ( string s, string t) const {
        return s < t;
    }
};

map<string,label> label_map;
map<string,struc> struc_map;

struc *current_struc=0;

struc *start_struc(char *s)
{
    struc t;
    t.name = s;
    t.loc = 0;
    struc_map[s] = t;
    current_struc = &struc_map[s];
    return current_struc;
}

void insert_label ( char *s, int loc )
{
    label l;

    if ( current_struc ) {
        l.name = s;
        l.loc = loc;
        l.size = 0;
        l.format = "hex1";
        current_struc->parts.push_back(l);
    } else {
        if ( label_map.count(s) == 0 ) {
            l.name = s;
            l.loc = loc;
            l.size = 0;
            l.format = "hex1";
            label_map[s] = l;
        }
    }
}

void set_size ( char *s, int size )
{
    label l;
    if ( current_struc ) {
        current_struc->parts.back().size = size;
    } else {
        if ( label_map.count(s) > 0 ) {
            label_map[s].size = size;
        }
    }
}

void set_location ( char *s, int loc )
{
    label l;
    if ( current_struc ) {
        current_struc->parts.back().loc = loc;
    } else {
        if ( label_map.count(s) > 0 ) {
            label_map[s].loc = loc;
        }
    }
}

void set_format ( string s, string f )
{
    label l;
    if ( current_struc ) {
        current_struc->parts.back().format = f;
    } else {
        if ( label_map.count(s) > 0 ) {
            label_map[s].format = f;
        }
    }
}

int get_size ( string s )
{
    label l;
    if ( label_map.count(s) > 0 ) {
        return label_map[s].size;
    }
}

int get_location ( string s )
{
    label l;
    if ( label_map.count(s) > 0 ) {
        return label_map[s].loc;
    }
}

string get_format ( string s )
{
    label l;
    if ( label_map.count(s) > 0 ) {
        return label_map[s].format;
    }
}

void dump_labels()
{
    map<string,label>::iterator it, itstart, itend; 
    label l;

    itstart = label_map.lower_bound("");
    itend = label_map.upper_bound("zzzzzzzz");
    
    for ( it = itstart; it != itend; it++ ) {
        l = it->second;
        if ( l.size > 0 ) {
            cout << "global " << l.name << " " << l.format
                 << " " << l.loc << " " << l.size << endl;
        }
    }
}

void dump_strucs()
{
    map<string,struc>::iterator it;
    struc s;
    label l;

    for ( it = struc_map.begin(); it != struc_map.end(); it++ ) {
        s = it->second;
        cout << "struc " << s.name << " " << s.loc << endl;
        for ( int i=0; i < s.parts.size(); i++ ) {
            l = s.parts[i];
            cout << "member " << l.name << " " << l.format
                 << " " << l.loc << " " << l.size << endl;
        }
        cout << "endstruc" << endl;
    }
    cout << "end" << endl;
}
