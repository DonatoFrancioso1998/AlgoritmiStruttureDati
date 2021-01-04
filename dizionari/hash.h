#ifndef HASH_H_
#define HASH_H_

#include "dictionary.h"
#include <string>

using std::string;

//hash è stato fatto anche per stringhe percèh la gestione è diversa

//abs ritorna il valore assoluto di the_key
template<class T>
class hash{
public:
  size_t operator()(const T the_key) const{
	  return abs(the_key);
  }
};

//cicla su tutti i caratteri della stringa (at()) modificando il valore di hash_value
//hash_value è un numero indicativo di quella key da cui poi ricavare la posizione
template<>
class hash<string>{
public:
  size_t operator()(const string the_key) const {
    unsigned long hash_value = 0;
    int length = (int) the_key.length();
    for (int i=0; i<length; i++)
      hash_value = 5 * hash_value + the_key.at(i);
   // return size_t(hash_value);
    return hash_value;
  }
};


template<class K, class E>
struct coppia;


template<class K,class E>
class hash_table:public dictionary<K,E> {
public:

    bool empty() const{
      return (dsize == 0);
    };
    int size() const{
      return dsize;
    };
    void erase(const K& k);	//cancella la coppia con la chiave k
   // void modify(const K& k, const E& e);  //sovrascrive la coppia con chiave k modificandone l elemento con e
    hash_table(int);	    //costruttore
    void create(){};
    int search(const K& ) const;  //restituisce la posizione della copia con chiave  k
    coppia< K, E>* find(const K& ) const;    //restituisce la coppia con chiave k
    void insert( coppia< K, E>& ); //inserisce una nuova coppia
    int returnSize()const{
    	return this->dsize;
    }
    void resize();

private:
    coppia<K,E>** table; //la tabella hash
    hash<K> hashm;
    int dsize; //numero di coppie nel dizionario
    int divisor; //divisiore delle funzioni hash
};


//setta gli elementi della tabella a null
template<class K, class E>
hash_table<K,E>::hash_table(int d){
	divisor = d;
	dsize=0;
	table = new coppia<K, E>* [divisor];
	  for (int i=0; i<divisor; i++)
	    table[i] = NULL;
}



template<class K, class E>
int hash_table<K,E>::search(const K& c)const{
	int i = (int) hashm(c) % divisor;   //restituisce l'home bucket, home bucket è il primo elemento in cui si accede alla lista
	  int j = i;
	  //se la posizione j della tabella è vuota o è proprio la c da parametro restituisco la posizione j altrimenti passo al bucket sucessivo
	  do {
	    if (table[j] == NULL || table[j]->chiave == c)
	      return j;
	    j = (j+1) % divisor;                    // the next bucket
	  } while (j != i);

	  return j;                                 // la tabella è piena
}

template<class K, class E>
coppia< K, E>* hash_table<K,E>::find(const K& c) const
{
  //mi restituisco la posizione di c nella tabella
  int b = search(c);
  // see if a match was found at table[b]
  if (table[b] == NULL || table[b]->chiave != c)
    return NULL;    // no match
  return table[b];  // matching pair
}

template<class K, class E>
void hash_table<K,E>::insert( coppia<K, E>& c){
	  // search the table for a matching element
	  int b = search(c.chiave);
	  // chack if matching element found
	  if (table[b] == NULL){
	    // no matching element and table not full
	    table[b] = new coppia<K,E> (c);
	    dsize++;
	  } else {
	    // check id duplicate or table full
	    if (table[b]->chiave == c.chiave)
	      // duplicate, change table[b]->second
	      table[b]->elemento = c.elemento;
	    else{
	      // table is full
	      // throw the exception hash_table_full();
	    }
	  }
}

template<class K, class E>
void hash_table<K,E>::erase(const K& k){
	int b = search(k);
	if(table[b]!=NULL){
		delete table[b];
		table[b] = NULL;
		dsize--;
	}
}

template<class K,class E>
void hash_table<K,E>::resize(){
    hash_table<K,E> doppia(this->divisor*2);
    //inserimento dal dizionario originale a quello di dimensione doppia
    for(int i=0;i<divisor;i++){
        if(this->table[i]!=NULL){
            mypair<K,E> coppia;
            coppia.chiave=this->table[i]->chiave;
            coppia.elemento=this->table[i]->elemento;
            doppia.insert(coppia);
        }
    }
    doppia.dsize=this->dsize;
    //cancellazione vecchia tabella del dizionario
    this->deleteTable();

    //ri-creazione vecchia tabella originale con dimensione della tabella di doppia dimensione
    this->divisor=doppia.divisor;
    this->dsize=doppia.dsize;
    this->table=new mypair<K,E>* [divisor];
    for (int i=0; i<this->divisor; i++)
        this->table[i] = NULL;
    for(int j=0;j<doppia.divisor;j++){
        if(doppia.table[j]!=NULL){
            mypair<K,E> coppia;
            coppia.chiave=doppia.table[j]->chiave;
            coppia.elemento=doppia.table[j]->elemento;
            this->insert(coppia);//non funziona
        }
    }
}


#endif /* HASH_H_ */
