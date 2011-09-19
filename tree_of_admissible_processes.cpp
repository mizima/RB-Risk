#include "tree_of_admissible_processes.h"

string tree_of_admissible_processes::solve(string path, int heur_par)
{
    switch (load_input(path)) //nacte zadani a zkontroluje, je-li dost kapacit
    {
        case 0 :
            max_time = 0;
            for (list<activity>::iterator it=activities.begin(); it!=activities.end();it++)
                max_time += (*it).true_time;
            heuristic_threshold = heur_par;
            initialize_first_floor();
            while (!iterate_last_floor()); //iteruje dokud nenalezne reseni nebo nezjisti neresitelnost zadani
            return write_output();
        case 1 :
            return my_errors(1);
        case 2 :
            return my_errors(2);
    }
    return my_errors(4);
}

int tree_of_admissible_processes::load_input(string path)
{
    //nazev - optimisticky odhad - realisticky odhad - pesimisticky odhad - vira v realisticky odhad - zdroje - zavislosti
    ifstream input_file(path.c_str());
    if (input_file.is_open())
    {
        string buffer;
        getline(input_file, buffer);
        capacity=atoi(buffer.c_str());
        MDs=0;
        time_step=0;
        while (getline(input_file,buffer))
        {
            if (add_activity(buffer)>capacity)
            {
                input_file.close();
                return 2;
            }
            MDs+=activities.back().MDs;
        }
        return 0;
    }
    else
        return 1;
}

int tree_of_admissible_processes::add_activity(string buffer)
{
    activity new_activity;
    unsigned pointer=0;
    new_activity.name=get_word_from_string(buffer,&pointer);
    for (int i=0; i<3; i++)
        new_activity.estimates[i]=atoi(get_word_from_string(buffer,&pointer).c_str());
    new_activity.sharpness=atoi(get_word_from_string(buffer,&pointer).c_str());
    new_activity.true_time=new_activity.estimates[1];
    new_activity.sources=atoi(get_word_from_string(buffer,&pointer).c_str());
    while (pointer<buffer.size() && !(buffer[pointer]==char(',')))
        new_activity.dependencies.push_back(get_word_from_string(buffer,&pointer));
    new_activity.MDs=new_activity.true_time*new_activity.sources;
    activities.push_back(new_activity);
    return new_activity.sources;
}

void tree_of_admissible_processes::initialize_first_floor()
{
    node initial_node;
    initial_node.free_capacity=capacity;
    for (list<activity>::iterator it=activities.begin(); it!=activities.end();it++)
        if((*it).dependencies.size()==0)
            initial_node.accessible_activities.push_back((*it).get_accessible_activity());
        else
            initial_node.inaccessible_activities.push_back((*it).get_inaccessible_activity());
    initial_node.accessible_activities.sort();
    last_floor.push_back(initial_node);
}

bool tree_of_admissible_processes::iterate_last_floor()
{
    vector<node> new_nodes;
    time_step++;
    unsigned end_of_floor=last_floor.size();
    for (unsigned j=0;j<end_of_floor;j++)
    {
    (*last_floor.begin()).continue_with_activities(time_step);
    new_nodes=(*last_floor.begin()).possible_evolutions();
        if (new_nodes.size()==0)
            return 1;
        else
            last_floor.insert(last_floor.end(),new_nodes.begin(),new_nodes.end());
    last_floor.pop_front ();
    }
    cout << time_step << " " << last_floor.size() << " ";
    heuristic();
    cout << last_floor.size() << endl;

    emit changeProgress(100/(max_time/time_step)/2);

    return 0;
}

void tree_of_admissible_processes::heuristic()
{
    vector<int> heuristic_criterion_value(last_floor.size(),0);
    unsigned i=0;
    for(list<node>::iterator it=last_floor.begin();it!=last_floor.end();it++)
        heuristic_criterion_value[i++]=(*it).progress();														//pouzita heuristika
    int heuristic_reference=*max_element(heuristic_criterion_value.begin(),heuristic_criterion_value.end());

    //int heuristic_threshold=0;//max(capacity-(int)(last_floor.size()/1000),0);										//THRESHOLD
    cout << " " << (double) heuristic_reference/MDs << " " << heuristic_threshold << " ";

    list<list<node>::iterator> cut;
    i=0;
    for(list<node>::iterator it=last_floor.begin();it!=last_floor.end();it++)
        if((heuristic_reference-heuristic_criterion_value[i++])>heuristic_threshold)			//pokud je dana hodnota od referencni hodnota vzdalena vice nez o threshold, proces se bude z hlediska dalsiho vypoctu povazovat za neperspektivni
            cut.push_back (it);
    for(list<list<node>::iterator>::iterator it=cut.begin();it!=cut.end();it++)
        last_floor.erase(*it);
}

string tree_of_admissible_processes::write_output()
{
    string output_stream;
    for (list<node>::iterator it=last_floor.begin();it!=last_floor.end();it++)
        if ((*it).ongoing_activities.size()==0 && (*it).accessible_activities.size()==0)
        {
            if ((*it).inaccessible_activities.size()==0)
                output_stream+= export_process(&((*it).done_activities));
            else
                return my_errors(3);
        }
    return output_stream;
}

string tree_of_admissible_processes::export_process(list<done_activity> * pprocess)
{
    ostringstream output_stream;
    for(list<done_activity>::iterator it=(*pprocess).begin();it!=(*pprocess).end();it++)
    {
        output_stream << (*(*it).pactivity).name  << ',' << (*it).start_time ;
        for(list<string>::iterator it1=(*it).artificial_dependencies.begin();it1!=(*it).artificial_dependencies.end();it1++)
            output_stream  << ','<< (*it1);
            output_stream<<endl;
    }
    output_stream<<endl;
    return output_stream.str();
}

void tree_of_admissible_processes::initialize_sequence_of_activities(string path,bool dependency_on_fixed_time) //inicializuje posloupnost aktivit v procesu dle souboru daneho cestou
{
    ifstream input_file(path.c_str());
    string buffer;
    string artificial_dependency;
    activity aux;

    while (getline(input_file,buffer))
    {
        unsigned pointer=0;
        aux.name=get_word_from_string(buffer,&pointer);
        string start_time=get_word_from_string(buffer,&pointer);
        list<string> artificial_dependencies;
        while (pointer<buffer.size() && !(buffer[pointer]==char(',')))
            artificial_dependencies.push_back(get_word_from_string(buffer,&pointer));
            if (dependency_on_fixed_time)
            {
                if (start_time.size()>0 && start_time!="1")
                {
                    stringstream buffer_stream;
                    buffer_stream << (atoi(start_time.c_str())-1);
                    (*find (activities.begin(), activities.end(), aux)).dependencies.push_back(start_time);
                    add_activity(start_time + ',' + buffer_stream.str() + ','  + buffer_stream.str() + ',' + buffer_stream.str() + ',' + "0" + ',' + "0");	//davam tam duplicitni omezovace, musim pouzit .unique. slo by to hlidat, ale kdo vi, jeslti by se to zrychlilo...
                }
            }
            else
                for(list<string>::iterator  it=artificial_dependencies.begin();it!=artificial_dependencies.end();it++)
                    (*find (activities.begin(), activities.end(), aux)).dependencies.push_back(*it);
    }
    activities.unique();
    input_file.close();
}

/*
string tree_of_admissible_processes::mc_study(node process, int N)
{
        ostringstream realizations;
        mt19937 eng;
        eng.seed(unsigned(time(NULL)));
        for(int i=0;i<N;i++)
        {
                cout << i << endl;
                realizations << mc_realization(process, &eng) << endl;
        }
        return  realizations.str();
}
*/

void tree_of_admissible_processes::mc_study2(node process, int N, int* a)
{
        //mt19937 eng;
        //eng.seed(unsigned(time(NULL)));
    emit changeProgress(0);
    int j = 0;
    for(int i=0;i<N;i++)
    {
        j = mc_realization(process);
        a[j] += 1;
        emit changeProgress((100/(N/(i+1))/2 + 50));
    }
}

//int tree_of_admissible_processes::mc_realization(node process, mt19937 * peng)
int tree_of_admissible_processes::mc_realization(node process)
{
    time_step=0;
    for(list<activity>::iterator it=activities.begin();it!=activities.end();it++)
        (*it).initialize_true_duration();

    while (process.accessible_activities.size()+process.ongoing_activities.size()!=0)
    {
        process.continue_with_activities(time_step ++);
        process.start_activities();
    }
    return time_step;
}

#include "moc_tree_of_admissible_processes.cpp"
