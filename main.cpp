#include <iostream>
#include <map>
#include <list>

using namespace std;
#define Axc_Max_Len 100

struct Axc_array{
  unsigned int cellid;
  unsigned int startbit;
  unsigned int endbit;
};

enum Rat_type{
	Idle = 0,
	UMTS = 0x80000000,
	LTE = 0x08000000,
	GSM = 0x40000000
};

std::map<unsigned int,list<Axc_array*> > Axc_map;

unsigned int get_AxcBitLen(Rat_type rat_type){
	if( UMTS == rat_type)
		return 0x18;
	else if( LTE == rat_type)
		return 0x192;
	else if( GSM == rat_type)
    return 0x5;
  else
    return 0;
}

void init_data(unsigned int array_size){
  for(unsigned int i = 0; i < array_size; i++){
    list<Axc_array *>list_axc;

    Axc_map[i] = list_axc;
  }
}

void clear_data(unsigned int array_size){
  for(unsigned int i = 0; i < array_size; i++){
    std::map<unsigned int,list<Axc_array*> >::iterator pos = Axc_map.find(i);
    if(pos != Axc_map.end()){
      list<Axc_array*> list_axc = pos->second;
      list<Axc_array*>::iterator ki = list_axc.begin();
      for(;ki != list_axc.end(); ++ki){
        delete *ki;
        *ki = 0;
      }
      list_axc.clear();
      Axc_map.erase(pos);
    }
  }
}

void Add_data(unsigned int brdgpp,unsigned int cellid, Rat_type rat_type,unsigned int *axc_startbit){
  unsigned int bit_len = 0;
  unsigned int start_bit = 0;
  unsigned int end_bit = 0;
  Axc_array* axc_ptr = 0;
  std::map<unsigned int,list<Axc_array*> >::iterator pos = Axc_map.find(brdgpp);
  if(pos != Axc_map.end()){
    list<Axc_array*> list_axc = pos->second;

    bit_len = get_AxcBitLen(rat_type);
    if(list_axc.empty()){
      axc_ptr = new Axc_array;
      axc_ptr->cellid = cellid;
      axc_ptr->startbit = 0;
      axc_ptr->endbit = bit_len;
      list_axc.push_back(axc_ptr);
      Axc_map[brdgpp] = list_axc;
      *axc_startbit = start_bit;
      return;
    }
    Axc_array *axc_temp = list_axc.front();
    start_bit = axc_temp->startbit;
    axc_temp = list_axc.back();
    end_bit = axc_temp->endbit;
    /*非空的分支*/
    if(start_bit == 0 && end_bit != 0){
      unsigned int cur_bit = 0;
      /*中间有断层的分支*/
      list<Axc_array*>::iterator pos = list_axc.begin();
      cur_bit = (*pos)->endbit;
      ++pos;
      for(;pos != list_axc.end();){
        axc_ptr = *pos;
        if(axc_ptr->startbit - cur_bit >= bit_len){
          axc_ptr = new Axc_array;
          axc_ptr->cellid = cellid;
          axc_ptr->startbit = cur_bit;
          axc_ptr->endbit = cur_bit + bit_len;
          list_axc.insert(pos,axc_ptr);
          Axc_map[brdgpp] = list_axc;
          *axc_startbit = start_bit;
          return;
        }
        else{
          cur_bit = (*pos)->endbit;
          ++pos;
        }
      }
      /*for循环没退出，则是正常分支*/
      axc_ptr = new Axc_array;
      axc_ptr->cellid = cellid;
      axc_ptr->startbit = end_bit;
      axc_ptr->endbit = end_bit + bit_len;
      list_axc.push_back(axc_ptr);
      Axc_map[brdgpp] = list_axc;
      *axc_startbit = start_bit;
      return;
    }
    /*不连续的分支，前面有断层的分支*/
    else if(start_bit != 0){
      if(bit_len <= start_bit){
        axc_ptr = new Axc_array;
        axc_ptr->cellid = cellid;
        axc_ptr->startbit = 0;
        axc_ptr->endbit = bit_len;
        list_axc.push_front(axc_ptr);
        Axc_map[brdgpp] = list_axc;
        *axc_startbit = start_bit;
        return;
      }
      /*前面断层不够分的分支*/
      else{
        axc_ptr = new Axc_array;
        axc_ptr->cellid = cellid;
        axc_ptr->startbit = end_bit;
        axc_ptr->endbit = end_bit+bit_len;
        list_axc.push_back(axc_ptr);
        Axc_map[brdgpp] = list_axc;
        *axc_startbit = start_bit;
        return;
      }
    }
  }
}

void Delete_data(unsigned int brdgpp,unsigned int cellid, Rat_type rat_type){
  std::map<unsigned int,list<Axc_array*>>::iterator pos = Axc_map.find(brdgpp);
  if(pos != Axc_map.end()){
    list<Axc_array*> list_axc = pos->second;
    list<Axc_array*>::iterator pos = list_axc.begin();
    for(;pos != list_axc.end();){
      Axc_array *axc_ptr = *pos;
      if(cellid == axc_ptr->cellid){
        pos = list_axc.erase(pos);
      }
      else{
        ++pos;
      }
    }
    Axc_map[brdgpp] = list_axc;
  }
}

int main(){
  /*初始化3个Map*/
  init_data(3);
  unsigned int Axc_startbit = 0;
  Add_data(1,3,UMTS,&Axc_startbit);
  Add_data(1,3,UMTS,&Axc_startbit);
  Add_data(1,4,UMTS,&Axc_startbit);
  Add_data(1,4,UMTS,&Axc_startbit);

  Delete_data(1,3,UMTS);
  Delete_data(1,3,UMTS);

  Add_data(1,3,UMTS,&Axc_startbit);
  Add_data(1,3,UMTS,&Axc_startbit);

  Delete_data(1,3,UMTS);

  Add_data(1,5,LTE,&Axc_startbit);
  Add_data(1,3,UMTS,&Axc_startbit);
  Add_data(1,6,GSM,&Axc_startbit);

  Add_data(1,3,UMTS,&Axc_startbit);

  clear_data(3);
	return 0;
}