// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_storageclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_storagedatasource.h"
#include <string.h>

hk_storagedatasource::hk_storagedatasource(hk_database* d,hk_presentation* p):hk_datasource(d,p)
{
#ifdef HK_DEBUG
    hkdebug("hk_storagedatasource::constructor");
#endif
    p_rows=0;
    p_debug_counter=0;
    p_enabled=false;
    p_true="1";
    p_false="0";
    use_batch_previous=false;
}


hk_storagedatasource::~hk_storagedatasource()
{
#ifdef HK_DEBUG
    hkdebug("hk_storagedatasource::destructor");
#endif
    delete_data();
}


void hk_storagedatasource::delete_data(void)
{
    vector<struct_raw_data*>::iterator it=p_data.begin();
    while(it!=p_data.end())
    {
        struct_raw_data* datarow=*it;
        it++;
        if (columns()!=NULL) for (unsigned int k=0;k<columns()->size();k++)
        {
            delete[] (datarow[k]).data;
        }
        delete[] datarow;
        datarow=NULL;

    }
    p_data.erase(p_data.begin(),p_data.end());
    p_rows=0;
}


hk_datasource::enum_datasourcetypes hk_storagedatasource::type()
{
    return ds_table;
}


list<hk_column*>* hk_storagedatasource::driver_specific_columns(void)
{
    return p_columns;
}


bool hk_storagedatasource::driver_specific_create_columns(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_storagedatasource::create_columns");
#endif
    return false;

}


bool hk_storagedatasource::driver_specific_enable(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_storagedatasource::driver_specific_enable");
#endif
    return false;
}


bool hk_storagedatasource::driver_specific_disable(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_storagedatasource::driver_specific_disable");
#endif
    delete_data();
    p_rows=0;
    p_counter=0;
    return true;

}


/*vector<struct_raw_data*>* hk_storagedatasource::data()
{
    return &p_data;
}
*/

struct_raw_data* hk_storagedatasource::columndata(unsigned long row, unsigned int fieldnr)
{
    if (p_columns==NULL) return NULL;
//  cout <<"p_data.size()=="<<p_data.size()<<" Name: "<<name()<<"Reihe"<<row<<endl;
    if (accessmode()==batchread &&p_data.size()<2) return NULL;
    if (accessmode()!=batchread &&( row>=p_data.size()||fieldnr >=p_columns->size())) return NULL;
    if (accessmode()==batchread) return &(p_data)[use_batch_previous?0:1][fieldnr];
//  cout <<"data at: row: "<<row<<" fielnr: "<<fieldnr<<" länge: "<<((p_data)[row][fieldnr]).length<<endl;//<<" data: "<<((p_data)[row][fieldnr]).data<<endl;
    return &(p_data)[row][fieldnr];

}


bool hk_storagedatasource::driver_specific_batch_goto_next(void)
{
    if (!use_batch_previous) return false;
    use_batch_previous=false;
    return true;

}


bool hk_storagedatasource::driver_specific_goto_row(unsigned long r)
{
#ifdef HK_DEBUG
    hkdebug("hk_storagedatasource::driver_specific_goto_row");
#endif

    if (p_enabled) if (r <p_rows) return true;
    return false;

}


unsigned long hk_storagedatasource::driver_specific_max_rows(void)
{
    return p_rows;
}


bool hk_storagedatasource::driver_specific_update_data(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_storagedatasource::driver_specific_update_data");
#endif
    list<hk_column*>::iterator it;
    it=p_columns->begin();
    while (it!=p_columns->end())
    {
        hk_column* xc=*it;
        if (xc->has_changed())
        {

            struct_raw_data*  data=(p_data[p_counter]);
            data[xc->fieldnumber()].length=xc->changed_data()->length;
            delete[] data[xc->fieldnumber()].data;
            char* buffer =NULL;
            if (xc->changed_data()->data)
            {
                buffer=new char[xc->changed_data()->length];
                memcpy(buffer,xc->changed_data()->data,xc->changed_data()->length);
            }
            data[xc->fieldnumber()].data=buffer;

        }
        it++;
    }

    return true;
}


bool hk_storagedatasource::driver_specific_delete_data_at(unsigned long position)
{
#ifdef HK_DEBUG
    hkdebug("hk_storagedatasource::driver_specific_delete_at");
#endif
    if (position>=p_data.size())
    {
//cout <<"hk_storagedatasource::driver_specific_delete_at error: position: "<<position<<">= max_rows() "<<max_rows()<<endl;
        return false;
    }
    struct_raw_data*  data=(p_data[position]);

    if (columns()!=NULL)
        for (unsigned int k=0;k<columns()->size();k++)
    {
        delete[] (data)[k].data;
    }
    vector<struct_raw_data*>::iterator it=find(p_data.begin(),p_data.end(),data);
    p_data.erase(it);
    delete[] data;
    data=NULL;
    if (p_rows>0 && accessmode()!=batchread) p_rows--;

    return true;
}


bool hk_storagedatasource::driver_specific_insert_data(void)
{

    return false;
}


void hk_storagedatasource::dump_data()
{

    vector <struct_raw_data*>::iterator it;
    cout<<endl<<"driverspecific DUMP"<<endl;
    cout<<"===="<<endl;
    int c=0;
    for (it=p_data.begin(); it!=p_data.end();it++)
    {
        cout <<c++<<": ";
        for (unsigned int k=0;k<p_columns->size();k++)
        {
//  cout<<"L: "<<((*it)[k]).length<<" \'"<<((*it)[k]).data<<"\' ";
            for(unsigned int l=0;l<((*it)[k]).length;l++)
            {

                if ((*it)[k].length>0&&(*it)[k].data)cout <<(*it)[k].data[l];
            }
            if ((*it)[k].data==NULL) cout<<"NULLvalue";
            cout <<" ";
        }
        cout <<endl;
    }
    cout<<endl;
    cout <<"================="<<endl<<endl;

}


bool hk_storagedatasource::insert_data(struct_raw_data* datarow)
{
    if (datarow==NULL) return false;
    switch (accessmode())
    {
        case standard:
        {
            p_data.insert(p_data.end(),datarow);
            p_rows++;
            return true;
        }
        case batchread:
        {
            while (p_data.size()>1) driver_specific_delete_data_at(0);
            if (p_data.size()==0)
            {
                struct_raw_data* dummy=new struct_raw_data[columns()->size()];
                p_data.insert(p_data.end(),dummy);
            }
            p_data.insert(p_data.end(),datarow);
            return true;
        }
        case batchwrite:
        {
            if (columns()!=NULL)
                for (unsigned int k=0;k<columns()->size();k++)
            {
                delete[] (datarow[k]).data;
            }
            delete[] datarow;
            p_rows++;
            return true;
        }
    }

    return false;
}


void hk_storagedatasource::set_maxrows(unsigned long p)
{

    p_rows=p;
}


bool hk_storagedatasource::driver_specific_batch_goto_previous(void)
{
    if (!use_batch_previous)
    {
        use_batch_previous=true;
        return true;
    }

    return false;
}


void hk_storagedatasource::memory_error(void)
{
//delete_data();
    show_warningmessage(hk_translate("Datasource too big. Try to limit the number of rows by defining a filter."));

}
