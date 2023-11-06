#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

Transaction::Transaction( std::string ticker_symbol, unsigned int day_date, unsigned int month_date,
unsigned year_date, bool buy_sell_trans, unsigned int number_shares, double trans_amount
){
  symbol=ticker_symbol;
  day=day_date;
  year=year_date;
  month=month_date;
  if (buy_sell_trans)
  {
   trans_type= "Buy";  }
  else{
      trans_type= "Sell";
    }
  shares=number_shares;
  amount=trans_amount;
  trans_id=assigned_trans_id;
  assigned_trans_id +=1;
  p_next=nullptr;
}
Transaction::~Transaction(){

}
bool Transaction::operator<( Transaction const &other ){
  if (year<other.year){
    return true;}
  else if (year==other.year)
  {
    if (month<other.month)
    {
      return true;
    }else if (month==other.month)
    {
      if (day<other.day)
      {
        return true;
      }else if (day==other.day)
      {
        if (trans_id > other.trans_id)
        {
          return true;
        }
      }
      
      
    }
  }
  return false;
  
}

History::History(){
  p_head=nullptr;       // store data
}


// Destructor
// TASK 1
// go through linked list and delete each elements
History::~History(){
  
  while(p_head != nullptr){
    Transaction *p_temp=p_head;
        p_head=p_head->get_next();
        delete p_temp;
        p_temp=nullptr;
    }
}
void History::insert( Transaction *p_new_trans ){
  Transaction  *p_temp= p_head;
  if(p_head == nullptr){
    p_head = p_new_trans;
  }
  else{
    while(p_temp->get_next() != nullptr){
      p_temp =p_temp-> get_next();
    }
    p_temp->set_next(p_new_trans);
  }

}
void History::read_history(){
  ece150::open_file();
  while(ece150::next_trans_entry()){
    Transaction *entries = new Transaction{ece150::get_trans_symbol(),ece150::get_trans_day(),ece150::get_trans_month(),ece150::get_trans_year(),ece150::get_trans_type(),ece150::get_trans_shares(),ece150::get_trans_amount()};
    insert(entries);
  }
  ece150::close_file();
}
void History::print(){
  std::cout<<"========== BEGIN TRANSACTION HISTORY ============"<<std::endl;
  Transaction *p_temp=p_head;
  while(p_temp!= nullptr){
    p_temp->print();
    p_temp=p_temp->get_next();
    
  }
  std::cout<<"========== END TRANSACTION HISTORY ============"<<std::endl;
}


// Overloaded < operator.
// TASK 2


// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK 3
//





// Destructor
// TASK 3
//


// read_history(...): Read the transaction history from file.
// TASK 4
//

// insert(...): Insert transaction into linked list.
// TASK 5
//


// sort_by_date(): Sort the linked list by trade date.
// TASK 6
//



// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//
void History::update_acb_cgl(){
  Transaction*p_temp=p_head->get_next();
  Transaction *p_previous=p_head;
  p_previous->set_acb (p_previous->get_amount());
  p_previous->set_share_balance(p_previous->get_shares());
  p_previous->set_acb_per_share(p_previous->get_acb()/p_previous->get_share_balance());
  while( p_temp != nullptr){
    if (p_temp->get_trans_type() == true) {
      p_temp->set_acb (p_temp->get_amount() + p_previous->get_acb());
      p_temp->set_share_balance (p_temp->get_shares() + p_previous->get_share_balance());
      p_temp->set_acb_per_share(p_temp->get_acb() / p_temp->get_share_balance());
     

    } else{
      p_temp->set_share_balance (p_previous->get_share_balance() - p_temp->get_shares());
      p_temp->set_acb_per_share(p_previous->get_acb_per_share());
      p_temp->set_acb (p_previous->get_acb() - (p_temp->get_shares() * p_temp->get_acb_per_share()));
      p_temp->set_cgl(p_temp->get_amount() - (p_temp->get_shares() * p_previous->get_acb_per_share())); 

      }
      p_temp =p_temp->get_next();
      p_previous=p_previous->get_next();
  }
}

  



// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8
double History::compute_cgl( unsigned int year ) {


 
  Transaction *p_temp {p_head->get_next()};
  Transaction *p_previous {p_head};
  double cgl_sum {0.0};



    while ( p_temp != nullptr ) {
      if (( p_temp->get_trans_type() == false) && (p_temp->get_cgl() == 0) ) {
        p_temp->set_cgl(p_temp->get_amount() - (p_temp->get_shares() * p_previous->get_acb_per_share()));
      } 
      if (p_temp->get_year() == year) {
        cgl_sum = cgl_sum + p_temp->get_cgl();
      }       
      p_temp = p_temp->get_next();
      p_previous = p_previous->get_next();

    }

  return cgl_sum;


}
// print() Print the transaction history.
//TASK 9




// GIVEN
// get_p_head(): Full access to the linked list.

Transaction *History::get_p_head() { return p_head; }


void History::sort_by_date() {

/*
Transaction *p_temp {p_head};

if (p_head == nullptr) {
  //If the head points to a null pointer, start the list with the new transaction address
  p_head = p_new_trans;
} else { while (p_temp->get_next()!= nullptr) {
  //While the next address does not point to a null pointer, walk through the linked list
  p_temp = p_temp->get_next();
  }
    //if the next address points to a nullpointer (tail), then insert the new address and set it as the next entry
    p_temp -> set_next (p_new_trans); 
  }

}

*/ 

//Walk through the list, evaluating each pair of node (present and next) with a < statement
//If the < statement returns true (date of present is less than date of next), move the present node using insert() to the end of the list (points to null ptr) 

Transaction *p_temp {p_head};
//Temporarily stores the address while preventing modification to the head, also used to
//track a temporary head that moves further down the list, not constant so it can be modified

while (p_temp->get_next() != nullptr) {

  if (((*p_temp<(*p_temp->get_next())) == false) && (p_temp == p_head) ) {

    p_head = p_temp->get_next();
    insert (p_temp);
    p_temp = p_head;
  } else if (((*p_temp<(*p_temp->get_next())) == false) && (p_temp != p_head) ) {
    
    Transaction *p_previous {&*p_head};
    //p_prev tracks the previous node
    while (p_previous->get_next() != nullptr) {
      
      if (p_temp->get_next() == p_previous) {
        
        p_temp->set_next(nullptr);
        //Set the temporary head to point to nullptr (thereby making it an actual tail)
        break;
        //Break from the while loop once done
      } else if (p_previous -> get_next() == p_temp) {
          //If the previous node points towards the temporary head...
          p_previous->set_next (p_temp->get_next());
          //Set the next node of the previous node to the next node of the temporary head (of a list: 1, 2 ,3, with 2 being p_temp, 1 now points to 3, and 2 is inserted at the end)
          insert (p_temp);
          //Insert the temporary head at the end (the '2' that was mentioned in the previous comment)
          p_temp = p_temp->get_next();
          //Interestingly, this creates a scenario where the p_temp still points to its respective next node, therefore I can use the next node of that p_temp as my new temporary head
          break;
          //Break from the while loop once all done
        } else {
            p_previous = p_previous -> get_next();
            //Else if none of those things evaluate, the p_prev will "walk" another step into the list and try again
          }
    }

  } else { 
    p_temp = p_temp->get_next();
    //In the case that the left operand is not greater than the left operand, just advance to the next node and try again
  }
}

}

// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//

