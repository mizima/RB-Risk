#ifndef NODE_H
#define NODE_H


#include "activities.h"

class node
{
public:
        list<ongoing_activity> ongoing_activities; //probihajici aktivity
        list<accessible_activity> accessible_activities; //zpristupnene aktivity
        list<inaccessible_activity> inaccessible_activities; //nepristupne aktity
        list<done_activity> done_activities; //hotove aktivity
        int free_capacity; //aktualni nevyuzita kapacita

        void continue_with_activities(int current_time); //zajisti pokracovani rozdelaneho, v pripade konce upravi zavislosti
        void update_dependencies(string name); //opraveni zavislosti pri ukoncene aktivite
        vector<node> possible_evolutions(); //vrati vsechny moznosti mozne vyvoje systemu
        void push_activity(list<accessible_activity>  possible_ongoing_activities,int remaining_capacity, vector<accessible_activity> * pfast_accessible_activities,unsigned pointer, vector<node> * pfound_posibilities);//zarazuje mozne aktivity do procesu

        int progress();	//kolik mam hotoveho (=kolik MDs uz jsem cerpal)
        void start_activities();  //da do procesu vse, co by melo byt konano a lze delat
};

#endif // NODE_H
