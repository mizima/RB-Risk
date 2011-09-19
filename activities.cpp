#include "activities.h"

bool operator< (accessible_activity x, accessible_activity y)
    {return (*x.pactivity).sources< (*y.pactivity).sources;}
bool operator==(activity x, activity y)
    {return x.name == y.name;}
bool operator==(accessible_activity x, accessible_activity y)
    {return *(x.pactivity)==*(y.pactivity);}
bool operator==(inaccessible_activity x, inaccessible_activity y)
    {return *(x.pactivity)==*(y.pactivity);}
bool operator== (ongoing_activity x, ongoing_activity y)
    {return *(x.pactivity)==*(y.pactivity);}

inaccessible_activity activity::get_inaccessible_activity()
{
        inaccessible_activity new_inaccessible_activity;
        new_inaccessible_activity.pactivity=this;
        new_inaccessible_activity.unsatisfied_dependencies =(*this).dependencies;
        return new_inaccessible_activity;
}

accessible_activity activity::get_accessible_activity()
{
        accessible_activity new_accessible_activity;
        new_accessible_activity.pactivity=this;
        return new_accessible_activity;
}

ongoing_activity activity::get_ongoing_activity(list<string> artificial_dependencies)
{
        ongoing_activity new_ongoing_activity;
        new_ongoing_activity.pactivity=this;
        new_ongoing_activity.remaining_time=(*this).true_time ;
        new_ongoing_activity.artificial_dependencies=artificial_dependencies;
        return new_ongoing_activity;
}

done_activity activity::get_done_activity(int start_time,list<string> artificial_dependencies)
{
        done_activity new_done_activity;
        new_done_activity.pactivity=this;
        new_done_activity.start_time =start_time;
        new_done_activity.artificial_dependencies=artificial_dependencies;
        return new_done_activity;
}

typedef std::gamma_distribution<double> MyDist;

//void activity::initialize_true_duration(mt19937 * peng)
void activity::initialize_true_duration()
{
        if (estimates[2]==estimates[0])
                true_time=estimates[0];
        else
        {
                double m=(estimates[1]-estimates[0])/(double)(estimates[2]-estimates[0]);

                MyDist gamma1(m*sharpness+1);
                MyDist gamma2((1-m)*sharpness+1);
                MyDist::input_type engval = eng();
                MyDist::result_type distval1 = gamma1(eng);
                MyDist::result_type distval2 = gamma2(eng);

                engval = engval; //ignorovat
                distval1 = distval1; //ignorovat
                distval2 = distval2; //ignorovat

                double x = gamma1(eng);
                double y = gamma2(eng);
                true_time=(int)ceil (x/(x+y)*(estimates[2]-estimates[0])+estimates[0]);
        }
}

