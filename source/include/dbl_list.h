#ifndef DBL_LIST_H
#define DBL_LIST_H

#include<stdlib.h>

//This header holds double list classes that works with templates. These are 
//the second and third templates I ever programmed my whole life, and they
//are really tricky to get to work (see the include at end of file and
//macros at cpp file... these are weird, yet only solution I found). Templates
//(looks like that they) are somewhat slower than pure programmed classes. The
//main reason is because I couldnt find a way to put the node class outside
//the template class, so the only way to navigate through the nodes is by
//using functions... and calling them takes a small time. Of course, worrying
//about execution speed is a must for goods gamedevs (and memory usage, too).
//Lighter the code, older the pc that runs, but this project is more for my
//learning (and, hopefully, will help others as well). There is some tests I
//made, and classes with templates still supports operators overloading. Didnt
//test inheritance, but these might works as well. Deleting a template pointer
//that points to an allocated class deletes the class as well, making it
//easier to manage memory.
//
//I thought of doing these classes by the use of inheritance, but I thought
//it would make even slower (and hard to put on what I've already programmed
//by now).





//The first class is a double linked list, with head insertion. Head insertion
//is easy to programm, and removing ndoes from double lists is fast, as long
//as you are on the right node.
//
//There is an integer called ID. It is almost useless, but will serve on some
//searching functions. Since this list is not sorted... well...... searching is
//slow XD. Be sure to have only a single positive ID value for each object.

template<class T_obj>

//Here it is, the class!
class DOUBLE_LIST
{
private:
    //The node used to make the list
    struct NODE
    {
        //The (template) object it will hold
        T_obj *obj;
        
        //Cool... pointers
        NODE *next;
        NODE *previous;
        
        //Nearly useless integer
        int id;
    };
    
    //Cute pointer, to the head of the list.
    NODE *head;
    
    //Cutier pointer to what we are really pointing now
    NODE *actual;
    //The above pointer is used to make some functions use easier. Just read
    //their description =)
    
    //Number of elements inside the list
    int elements;
    
    //These search functions does not set actual to anything. They search
    //for an element and return its node address. These ones are used by this
    //class only. Since I could not manage to use return value on normal
    //way, there is a variable that hold the results.
    void private_search(int id);//by id
    void private_search(T_obj *obj);//By objects address
    
    //This is the variable that hold search results
    NODE *search_results;
    
    //This remove function receives a pointer to the node to be removed.
    //The object it pointed to is not deleted, giving chance for flexible
    //use and memory leaks.
    bool private_remove(NODE *node);
    
    //The delete function works exactly the same as remove function, but
    //deletes the object. Avoids memory leaks, but opens space for illegal
    //access =/
    bool private_delete(NODE *node);
public:
    //Prepare everything
    DOUBLE_LIST();
    
    //These functions adds elements to the list. Some of these holds limitations
    //Returns true on success, false on fail. If success, "actual" pointer will
    //point to the created node
    bool insert(T_obj *obj, int id); //Adds everything
    bool insert(T_obj *obj); //Adds a node, default id as 0
    bool operator+=(T_obj *obj);
                                //Adds a node in a cool way, id is default 0
    
    //A single function for UGH sorted insertion. If this will be used, be sure
    //to only insert elements with this function, as it is... well, sorted.
    //The other ones wont sort out anything.
    //
    //I hate coding insertion sort on lists =/
    bool sorted_insert(T_obj *obj, int id);
    
    //These functions remove the element and destroy the node. The removed
    //object is also DELETED from memory, so, beware. If the removed element
    //is the one that actual pointed to, it will point to the previous one,
    //or NULL, if there was not a previous.
    bool delete_obj(T_obj *obj);//Removes giving its pointer
    bool delete_obj(int id);//Remove by means of ID. The first one found, only.
    bool delete_obj();//Remove what "actual" pointer is pointing right now.
                      //"actual" will then point to previous.
    
    //These functions only destroy the node, but the object it pointed to
    //will stay on memory.
    bool remove_obj();//Uses the "actual" pointer. Beware. It will point to the
                      //previous element...
    bool remove_obj(int id);//Remove by knowing its id. First found, actually.
    bool remove_obj(T_obj *obj);//Removes giving the object address
    
    //Search functions. Kinda useful. They return true on finding, and "actual"
    //pointer will point to the object. If not found, "actual" points to NULL.
    bool search(T_obj *obj);//Weird search, using object address
    T_obj* search(int id);//First node with this id will appear. Returns
                          //object adress if found, NULL on nothing.
    
    //Pointers used to navigate through the list. They return the address
    //of the object inside the node the function moved into, and actual
    //will also point to it. NULL on no object, both actual and return value
    T_obj* next();
    T_obj* previous();
    //Cooler ones. These two moves, then points.
    T_obj* operator++(void);//Next
    T_obj* operator--(void);//Previous
    
    //Get what "actual" is pointing now. Returns NULL if there is no "actual"
    T_obj* get_obj();
    
    //Return the number of elements
    int get_elements();
    
    //Set "actual" to the head of the list, or NULL, if there is not one
    void reset_actual(void);
    
    //Returns true if "actual" is not NULL
    bool actual_valid();
    
    //Clear the list, and remove all objects from memory. This means, they
    //dont need to be manually deleted somewhere else
    void clear(void);
    
    //Clean the lists. This does not delete the objects, so careful with
    //memory leaks
    void clean(void);
    
    //Calls clear
    ~DOUBLE_LIST();
};

#endif
 
#include"dbl_list.cpp"
