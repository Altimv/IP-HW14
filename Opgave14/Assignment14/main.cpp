#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <vector>
#define NDEBUG
#include <cassert>

using namespace std;



struct Length{
    int minutes;
    int seconds;
};

struct Track{
    string artist;
    string cd;
    int year;
    int track;
    string title;
    string tags;
    Length time;
    string country;
};

vector<Track> Tracks;
typedef Track singletrack;

bool operator< (const Length& a, const Length& b)
{
    if(a.minutes == b.minutes){
        return a.seconds < b.seconds;
    }
    return a.minutes < b.minutes;
}

bool operator== (const Length& a, const Length& b)
{
    return(a.minutes==b.minutes && a.seconds==b.seconds);
}



bool operator< (const Track& a, const Track& b)
{
    if(a.artist==b.artist){
        if(a.cd==b.cd){
            return a.track<b.track;
        }
        return a.cd<b.cd;
    }
    return a.artist<b.artist;
}

bool operator== (const Track& a, const Track& b)
{
    return (a.artist==b.artist && a.cd==b.cd && a.track==b.track);
}

bool operator>(const Track& a, const Track& b)
{

	return b < a ;
}

bool operator<=(const Track& a, const Track& b)
{

	return !(b < a) ;
}

bool operator>=(const Track& a, const Track& b)
{
	return b <= a ;
}


istream& operator>> (istream& in, Length& lengte)
{
    char colon;
    in >> lengte.minutes >> colon >> lengte.seconds;
    return in;
}

ostream& operator<< (ostream& out, const Length lengte)
{
    out << lengte.minutes << ':' ;
    if (lengte.seconds < 10)
    out << '0' ;
    out << lengte.seconds ;
    return out ;
}

istream& operator>> (istream& infile, Track& track)
{
    string temp;
    getline(infile,track.artist);
    getline(infile,track.cd);
    getline(infile,temp);
    track.year = atoi(temp.c_str());
    getline(infile,temp);
    track.track = atoi(temp.c_str());
    getline(infile,track.title);
    getline(infile,track.tags);
    infile>>track.time;
    getline(infile,temp);
    getline(infile,track.country);
    getline(infile,temp);
    return infile;
}

ostream& operator<< (ostream& out, const Track track)
{
    out << track.artist << " " << track.cd << " [" << track.track << "] (" << track.time << ")" ;
    return out ;
}


int read_tracks(ifstream& DBStracks, vector<Track>& Tracks)
{
    //Pre-condition:
    assert(DBStracks.is_open());
    //Post-condition: all tracks are read, size of vector Tracks is returned
    do {
        Track nexttrack;
        DBStracks >> nexttrack;
        if(DBStracks){
            Tracks.push_back(nexttrack);
        }
    }while(DBStracks);
    return Tracks.size();
}

int read_file(string filename)
{
    //Pre-condition:
    assert(true);
    //Post-condition:
    ifstream DBStracks (filename.c_str());
    if(!DBStracks.is_open()){
        cout << "Couldn't open file. " << endl;
        return -1;
    }
    cout << "File is opened. " << endl;
    int numberoftracks = read_tracks(DBStracks, Tracks);
    DBStracks.close();
    return numberoftracks;
}

void show_musicDBS (vector<Track>& Tracks, int numberoftracks)
{
    for(int i=0;i<numberoftracks;i++){
        cout << i+1 << ". " << Tracks[i] << endl;
    }
}

void swap(vector<singletrack>& listoftracks, int i, int j)
{
    //Pre-condition:
    assert(i>=0&&j>=0&&i<listoftracks.size()&&j<listoftracks.size());
    //Post-condition: listoftracks[i] and listoftracks[j] are swapped

    const singletrack temp = listoftracks[i];
    listoftracks[i] =  listoftracks[j];
    listoftracks[j] = temp;
}

void dnfpart1(vector<singletrack>& data, int first, int last, int& red, int&blue)
{
    red = first-1;
    blue = last+1;
    int white = last+1;
    const singletrack PIVOT = data[first+(last-first)/2];
    while(red<white-1){
        const int NEXT = white -1;
        if(data[NEXT].time<PIVOT.time){
            red++;
            swap(data, red, NEXT);
        }
        else if(data[NEXT].time==PIVOT.time)
            white--;
        else{
            white--;
            blue--;
            swap(data, NEXT, blue);
        }
    }
}

void quicksortpart1(vector<singletrack>& data, int first, int last)
{
    //Pre-condition:
    assert(first>=0 && last<size(data));
    //Post-condition: data[first] to data[last] is sorted
    if(first>=last) return;
    else{
        int red, blue;
        dnfpart1(data, first, last, red, blue);
        quicksortpart1(data, first, red);
        quicksortpart1(data, blue, last);
    }
}

void dnfpart2(vector<singletrack>& data, int first, int last, int& red, int&blue)
{
    red = first-1;
    blue = last+1;
    int white = first-1;
    const singletrack PIVOT = data[first+(last-first)/2];
    while(blue>white+1){
        const int NEXT = white +1;
        if(data[NEXT].time<PIVOT.time){
            white++;
            red++;
            swap(data,red,NEXT);
        } else if (data[NEXT].time==PIVOT.time) {
            white++;
        } else {
            blue--;
            swap(data,NEXT,blue);
        }
    }
}

void quicksortpart2(vector<singletrack>& data, int first, int last)
{
    //Pre-condition:
    assert(first>=0 && last<size(data));
    //Post-condition: data[first] to data[last] is sorted
    if(first>=last) return;
    else{
        int red, blue;
        dnfpart2(data, first, last, red, blue);
        quicksortpart2(data, first, red);
        quicksortpart2(data, blue, last);
    }
}

/*void sortiterative(vector<singletrack>& v, int n)
{
    if(n>1){
        singletrack a = v[n-1];
        int postemp = 0;
        for (int j =n-1;j>0;j--){
            for(int i=0;i<n;i++){
                if(v[i]>a){
                    postemp = i;
                }
            }
            n--;
            if(!postemp==n-1){
                swap(v, postemp, n-1);
            }
        }

    }
}*/

int largest (vector<singletrack>& data, int low, int up) {
    int POS = up;
    if(low >= up)
        return low;
    while(low < up-1) {
        if(data[low] > data[POS]) {
            int temp = low;
            low = POS;
            POS = temp;
        }
        low++;
    }
    return POS;
}

void sort(vector<singletrack>& data, int n) {
    while(n > 1) {
        int POS = largest(data, 0, n);
        swap (data, POS-1, n);
        n--;
    }
}

int main()
{
    int numberoftracks = read_file("Nummers.txt");
    if(numberoftracks<0){
        cout << "Reading of file failed." << endl;
        return 0;
    }
    int first = 0;
    int last = Tracks.size()-1;
    //quicksortpart1(Tracks, first, last);
    //quicksortpart2(Tracks, first, last);
    sort(Tracks, last);
    show_musicDBS(Tracks, numberoftracks);
    return 0;
}


