#include "node.h"

void node::continue_with_activities(int current_time)
{
        list<ongoing_activity> new_done_activities;
        for (list<ongoing_activity>::iterator it=ongoing_activities.begin(); it!=ongoing_activities.end();it++) //pokud si to sesortuji, muzu pouzit while cyklus, vyhodne jen pokud budu mit hodne aktivit s relativne malymi naroky na zdroje
        {
                (*it).remaining_time --;
                if ((*it).remaining_time==0)
                {
                        free_capacity+=(*(*it).pactivity).sources;
                        update_dependencies((*(*it).pactivity).name);
                        done_activities.push_back((*(*it).pactivity).get_done_activity(current_time-(*(*it).pactivity).true_time,(*it).artificial_dependencies));//mam to dodelane, zacalo to v ted-kolik to trva
                        new_done_activities.push_back(*it);//abych vedel, co mam vymazat
                }
        }
        for (list<ongoing_activity>::iterator it=new_done_activities.begin();it!=new_done_activities.end();it++)
                ongoing_activities.remove(*it);
}

void node::update_dependencies(string name)
{
        list<inaccessible_activity> old_inaccessible_activities;
        list<accessible_activity> new_accessible_activities;
        //artificial dependency on finished activity for accessible activities
        for(list<accessible_activity>::iterator it=accessible_activities.begin(); it!=accessible_activities.end();it++)
                (*it).artificial_dependencies.push_back(name);

        //delete dependency on finished activity from inaccessible activities
        for (list<inaccessible_activity>::iterator it=inaccessible_activities.begin(); it!=inaccessible_activities.end();it++)
        {
                (*it).unsatisfied_dependencies.remove(name);
                if ((*it).unsatisfied_dependencies.size ()==0)
                {
                        old_inaccessible_activities.push_back(*it);
                        new_accessible_activities.push_back((*(*it).pactivity).get_accessible_activity());
                }
        }

        if (new_accessible_activities.size()>0)
        {
                for (list<inaccessible_activity>::iterator it=old_inaccessible_activities.begin(); it!=old_inaccessible_activities.end();it++ )
                        inaccessible_activities.remove(*it);
                new_accessible_activities.sort();
                accessible_activities.merge(new_accessible_activities);
        }
}

vector<node> node::possible_evolutions()
{
        vector<node> found_posibilities;
        vector<accessible_activity> fast_accessible_activities(accessible_activities.begin(),accessible_activities.end()); //pomocny vektor aktivit, rychle se bude pristupovat ke kapacitam...
        if (ongoing_activities.size()>0)
                found_posibilities.push_back(*this);
        push_activity(list<accessible_activity> (),free_capacity,&fast_accessible_activities,0,&found_posibilities);
        return found_posibilities;
}

void node::push_activity(list<accessible_activity> possible_ongoing_activities,int remaining_capacity, vector<accessible_activity> * pfast_accessible_activities, unsigned pointer,vector<node> * pfound_posibilities)
{
        if (pointer<(*pfast_accessible_activities).size() && remaining_capacity >= (*(*pfast_accessible_activities)[pointer].pactivity).sources) //pokud pridavat lze...
        {
                while (pointer<(*pfast_accessible_activities).size() && remaining_capacity >= (*(*pfast_accessible_activities)[pointer].pactivity).sources) //...pak pridavej dokud to lze
                {
                        possible_ongoing_activities.push_back((*pfast_accessible_activities)[pointer]);
                        remaining_capacity-=(*(*pfast_accessible_activities)[pointer].pactivity).sources;
                        pointer++;
                        push_activity(possible_ongoing_activities,remaining_capacity,pfast_accessible_activities,pointer,pfound_posibilities);
                        (* pfound_posibilities).push_back(*this);
                        for (list<accessible_activity>::iterator it=possible_ongoing_activities.begin(); it!=possible_ongoing_activities.end();it++)
                        {
                                (*pfound_posibilities).back().ongoing_activities.push_back((*(*it).pactivity).get_ongoing_activity((*it).artificial_dependencies));
                                (*pfound_posibilities).back().accessible_activities.remove(*it);
                                (*pfound_posibilities).back().free_capacity -=(*(*it).pactivity).sources;
                        }
                        possible_ongoing_activities.pop_back();
                        remaining_capacity+=(*(*pfast_accessible_activities)[pointer-1].pactivity).sources;
                }
        }
}

int node::progress ()
{
        int MDs_utilization=0;
        for (list<done_activity>::iterator it=done_activities.begin(); it!=done_activities.end(); it++)
                MDs_utilization+=(*((*it).pactivity)).MDs;
        for (list<ongoing_activity>::iterator it=ongoing_activities.begin(); it!=ongoing_activities.end(); it++)
                MDs_utilization+=(*((*it).pactivity)).MDs -(*it).remaining_time*(*((*it).pactivity)).sources;
        return MDs_utilization;
}

void node::start_activities()
{
        while(accessible_activities.size()>0  &&  (*(*accessible_activities.begin()).pactivity).sources<=free_capacity)
        {
                ongoing_activities.push_back((*(*accessible_activities.begin()).pactivity).get_ongoing_activity((*accessible_activities.begin()).artificial_dependencies));
                free_capacity-=(*(*accessible_activities.begin()).pactivity ).sources;
                accessible_activities.pop_front();
        }
}
